include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=eq3-char-loop
PKG_RELEASE:=1
PKG_LICENSE:=GPL-2.0-or-later

include $(INCLUDE_DIR)/package.mk

define KernelPackage/eq3-char-loop
  SUBMENU:=Other modules
  TITLE:=eQ-3 Homematic char loop driver
  FILES:=$(PKG_BUILD_DIR)/eq3_char_loop.ko
  AUTOLOAD:=$(call AutoLoad,61,eq3_char_loop)
endef

define KernelPackage/eq3-char-loop/description
 Kernel loop driver used by multimacd to provide
 /dev/eq3loop, /dev/mmd_bidcos and /dev/mmd_hmip.
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	+$(KERNEL_MAKE) \
		M="$(PKG_BUILD_DIR)" \
		modules
endef

$(eval $(call KernelPackage,eq3-char-loop))
