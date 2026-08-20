// SPDX-License-Identifier: GPL-2.0
/*
 * MediaTek MT7986 raw UART backend for generic_raw_uart
 *
 * Based conceptually on piVCCU dw_apb_raw_uart/pl011_raw_uart
 * and Linux drivers/tty/serial/8250/8250_mtk.c.
 */

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/serial_reg.h>
#include <linux/delay.h>

#include "stack_protector.include"
#include "generic_raw_uart.h"

#define MODULE_NAME "mtk_raw_uart"
#define TX_CHUNK_SIZE 1

#define MTK_UART_REGSHIFT 2

#define MTK_UART_HIGHS          0x09
#define MTK_UART_SAMPLE_COUNT   0x0a
#define MTK_UART_SAMPLE_POINT   0x0b
#define MTK_UART_FRACDIV_L      0x15
#define MTK_UART_FRACDIV_M      0x16
#ifndef UART_IIR_CTI
#define UART_IIR_CTI 0x0c
#endif

struct mtk_raw_uart_port {
	struct clk *baud_clk;
	struct clk *bus_clk;
	struct device *dev;
	resource_size_t mapbase;
	void __iomem *membase;
	int irq;
};

static struct mtk_raw_uart_port *mtk_port;
static inline void mtk_write(unsigned int reg, unsigned int value)
{
	writel(value, mtk_port->membase + (reg << MTK_UART_REGSHIFT));
}

static inline unsigned int mtk_read(unsigned int reg)
{
	return readl(mtk_port->membase + (reg << MTK_UART_REGSHIFT));
}

static void mtk_raw_uart_configure(void)
{
	static const unsigned short fraction_L_mapping[] = {
		0, 1, 0x5, 0x15, 0x55, 0x57,
		0x57, 0x77, 0x7f, 0xff, 0xff
	};
	static const unsigned short fraction_M_mapping[] = {
		0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 3
	};

	unsigned long clk;
	unsigned int quot;
	unsigned int tmp;
	unsigned int fraction;

	clk = clk_get_rate(mtk_port->baud_clk);

	/* Disable all UART interrupts while configuring. */
	mtk_write(UART_IER, 0);

	/* Disable FIFO while changing the UART parameters. */
	mtk_write(UART_FCR, 0);

	/*
	 * MediaTek-specific baud rate setup.
	 *
	 * Linux 8250_mtk uses HIGHS=3 for baud >= 115200 and
	 * calculates the divisor from uartclk / (256 * baud).
	 */
	mtk_write(MTK_UART_HIGHS, 0x03);

	quot = DIV_ROUND_UP(clk, 256 * BAUD);

	mtk_write(UART_LCR, UART_LCR_DLAB);
	mtk_write(UART_DLL, quot & 0xff);
	mtk_write(UART_DLM, (quot >> 8) & 0xff);

	/* 8N1 and clear DLAB. */
	mtk_write(UART_LCR, UART_LCR_WLEN8);

	tmp = (clk / (BAUD * quot)) - 1;

	mtk_write(MTK_UART_SAMPLE_COUNT, tmp);
	mtk_write(MTK_UART_SAMPLE_POINT, (tmp >> 1) - 1);

	fraction = ((clk * 100) / BAUD / quot) % 100;
	fraction = DIV_ROUND_CLOSEST(fraction, 10);

	if (fraction > 10)
		fraction = 10;

	mtk_write(MTK_UART_FRACDIV_L,
		  fraction_L_mapping[fraction]);
	mtk_write(MTK_UART_FRACDIV_M,
		  fraction_M_mapping[fraction]);

	/* Enable/reset FIFOs. */
	mtk_write(UART_FCR,
		  UART_FCR_ENABLE_FIFO |
		  UART_FCR_CLEAR_RCVR |
		  UART_FCR_CLEAR_XMIT |
		  UART_FCR_TRIGGER_1);
}

static void mtk_raw_uart_rx_chars(struct generic_raw_uart *raw_uart)
{
	unsigned int status;

	for (;;) {
		enum generic_raw_uart_rx_flags flags =
			GENERIC_RAW_UART_RX_STATE_NONE;
		unsigned int data;

		status = mtk_read(UART_LSR);

		if (!(status & (UART_LSR_DR | UART_LSR_BI)))
			break;

		if (status & UART_LSR_BI)
			flags |= GENERIC_RAW_UART_RX_STATE_BREAK;

		if (status & UART_LSR_PE)
			flags |= GENERIC_RAW_UART_RX_STATE_PARITY;

		if (status & UART_LSR_FE)
			flags |= GENERIC_RAW_UART_RX_STATE_FRAME;

		if (status & UART_LSR_OE)
			flags |= GENERIC_RAW_UART_RX_STATE_OVERRUN;

		data = mtk_read(UART_RX);

		generic_raw_uart_handle_rx_char(
			raw_uart, flags, (unsigned char)data);
	}

	generic_raw_uart_rx_completed(raw_uart);
}

static irqreturn_t mtk_raw_uart_irq(int irq, void *context)
{
	struct generic_raw_uart *raw_uart = context;
	unsigned int iid;
	int handled = 0;
	int loops = 0;

	/*
	 * Never spin indefinitely in IRQ context.
	 *
	 * With no Homematic module connected the RX input can be in an
	 * undefined state. A UART interrupt which cannot be acknowledged
	 * must never be allowed to lock up the router.
	 */
	while (loops++ < 32) {
		iid = mtk_read(UART_IIR);

		if (iid & UART_IIR_NO_INT)
			break;

		switch (iid & 0x0f) {
		case UART_IIR_RDI:
		case UART_IIR_CTI:
		case UART_IIR_RLSI:
			mtk_raw_uart_rx_chars(raw_uart);
			handled = 1;
			break;

		case UART_IIR_THRI:
			generic_raw_uart_tx_queued(raw_uart);
			handled = 1;
			break;

		default:
			/*
			 * Observe/acknowledge possible line status.
			 * If the source remains pending, the loop limit below
			 * will disable UART interrupts safely.
			 */
			(void)mtk_read(UART_LSR);
			handled = 1;
			break;
		}
	}

	if (loops > 32) {
		dev_err_ratelimited(
			mtk_port->dev,
			"UART IRQ did not clear, disabling UART interrupts\n");

		mtk_write(UART_IER, 0);
		return IRQ_HANDLED;
	}

	return handled ? IRQ_HANDLED : IRQ_NONE;
}

static int mtk_raw_uart_start_connection(struct generic_raw_uart *raw_uart)
{
	int ret;
	mtk_raw_uart_configure();

	ret = request_irq(
		mtk_port->irq,
		mtk_raw_uart_irq,
		0,
		dev_name(mtk_port->dev),
		raw_uart);

	if (ret) {
		dev_err(mtk_port->dev,
			"unable to request IRQ %d: %d\n",
			mtk_port->irq, ret);
		return ret;
	}

	/* Idle state: RX interrupt only. */
	mtk_write(UART_IER, UART_IER_RDI);

	return 0;
}

static void mtk_raw_uart_stop_connection(struct generic_raw_uart *raw_uart)
{

	mtk_write(UART_IER, 0);
	free_irq(mtk_port->irq, raw_uart);
	mtk_write(UART_FCR, 0);
}

static bool mtk_raw_uart_isready_for_tx(struct generic_raw_uart *raw_uart)
{
	return !!(mtk_read(UART_LSR) & UART_LSR_THRE);
}

static void mtk_raw_uart_tx_chars(
	struct generic_raw_uart *raw_uart,
	unsigned char *chr,
	int index,
	int len)
{

	mtk_write(UART_TX, chr[index]);
}

static void mtk_raw_uart_init_tx(struct generic_raw_uart *raw_uart)
{
	/*
	 * TX: keep RX enabled and use THRI for queued bytes.
	 * RX stays enabled and THRI drives the remaining TX bytes.
	 */
	mtk_write(UART_IER, UART_IER_RDI | UART_IER_THRI);
}

static void mtk_raw_uart_stop_tx(struct generic_raw_uart *raw_uart)
{
	/* Transmission finished: return to RX-only mode. */
	mtk_write(UART_IER, UART_IER_RDI);
}

static struct raw_uart_driver mtk_raw_uart = {
	.owner = THIS_MODULE,
	.start_connection = mtk_raw_uart_start_connection,
	.stop_connection = mtk_raw_uart_stop_connection,
	.init_tx = mtk_raw_uart_init_tx,
	.isready_for_tx = mtk_raw_uart_isready_for_tx,
	.tx_chars = mtk_raw_uart_tx_chars,
	.stop_tx = mtk_raw_uart_stop_tx,
	.tx_chunk_size = TX_CHUNK_SIZE,
	.tx_bulktransfer_size = 1,
};

static int mtk_raw_uart_probe(struct platform_device *pdev)
{
	struct resource *res;
	int ret;

	mtk_port = devm_kzalloc(
		&pdev->dev,
		sizeof(*mtk_port),
		GFP_KERNEL);

	if (!mtk_port)
		return -ENOMEM;

	mtk_port->dev = &pdev->dev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;

	mtk_port->mapbase = res->start;

	mtk_port->membase = devm_ioremap_resource(
		&pdev->dev, res);

	if (IS_ERR(mtk_port->membase))
		return PTR_ERR(mtk_port->membase);

	mtk_port->irq = platform_get_irq(pdev, 0);
	if (mtk_port->irq < 0)
		return mtk_port->irq;

	mtk_port->baud_clk =
		devm_clk_get(&pdev->dev, "baud");

	if (IS_ERR(mtk_port->baud_clk))
		return PTR_ERR(mtk_port->baud_clk);

	ret = clk_prepare_enable(mtk_port->baud_clk);
	if (ret)
		return ret;

	mtk_port->bus_clk =
		devm_clk_get(&pdev->dev, "bus");

	if (!IS_ERR(mtk_port->bus_clk)) {
		ret = clk_prepare_enable(mtk_port->bus_clk);
		if (ret) {
			clk_disable_unprepare(mtk_port->baud_clk);
			return ret;
		}
	}

	dev_info(
		&pdev->dev,
		"MTK raw UART initialized: mapbase=%pa irq=%d baudclk=%lu busclk=%ld\n",
		&mtk_port->mapbase,
		mtk_port->irq,
		clk_get_rate(mtk_port->baud_clk),
		IS_ERR(mtk_port->bus_clk)
			? -1L
			: clk_get_rate(mtk_port->bus_clk));

	return 0;
}

static int mtk_raw_uart_remove(struct platform_device *pdev)
{
	if (!IS_ERR(mtk_port->bus_clk))
		clk_disable_unprepare(mtk_port->bus_clk);

	clk_disable_unprepare(mtk_port->baud_clk);

	return 0;
}

static const struct of_device_id mtk_raw_uart_of_match[] = {
	{ .compatible = "pivccu,mt7986-uart" },
	{ }
};

module_raw_uart_driver(
	MODULE_NAME,
	mtk_raw_uart,
	mtk_raw_uart_of_match);

MODULE_ALIAS("platform:mtk-raw-uart");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
MODULE_DESCRIPTION(
	"MediaTek MT7986 raw UART backend for Homematic generic_raw_uart");
MODULE_AUTHOR("OpenWrt Homematic port");
