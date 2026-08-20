include $(TOPDIR)/rules.mk

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

PKG_NAME:=mtk-raw-uart
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)

define KernelPackage/mtk-raw-uart
  SUBMENU:=Other modules
  TITLE:=MediaTek MT7986 raw UART for Homematic
  DEPENDS:=+kmod-generic-raw-uart
  FILES:=$(PKG_BUILD_DIR)/mtk_raw_uart.ko
  AUTOLOAD:=$(call AutoLoad,60,mtk_raw_uart)
endef

define KernelPackage/mtk-raw-uart/description
 Raw UART backend for MediaTek MT7986 UART1 used by Homematic
 HM-MOD-RPI-PCB together with generic_raw_uart.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)

	$(CP) ./src/mtk_raw_uart.c $(PKG_BUILD_DIR)/
	$(CP) ./src/Makefile $(PKG_BUILD_DIR)/

	$(CP) ../generic-raw-uart/src/generic_raw_uart.h \
		$(PKG_BUILD_DIR)/

	$(CP) ../generic-raw-uart/src/stack_protector.include \
		$(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(KERNEL_MAKE) \
		M="$(PKG_BUILD_DIR)" \
		modules
endef

$(eval $(call KernelPackage,mtk-raw-uart))
