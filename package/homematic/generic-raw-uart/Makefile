include $(TOPDIR)/rules.mk

PKG_NAME:=generic-raw-uart
PKG_RELEASE:=1

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

define KernelPackage/generic-raw-uart
  SUBMENU:=Other modules
  TITLE:=Generic raw UART core for Homematic RF modules
  FILES:= \
	$(PKG_BUILD_DIR)/generic_raw_uart.ko
  AUTOLOAD:=$(call AutoProbe,generic_raw_uart)
endef

define KernelPackage/generic-raw-uart/description
 Generic low-latency raw UART core for Homematic RF modules.
 Hardware-specific UART backends are provided by separate packages.
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(KERNEL_MAKE) \
		M="$(PKG_BUILD_DIR)" \
		modules
endef

$(eval $(call KernelPackage,generic-raw-uart))
