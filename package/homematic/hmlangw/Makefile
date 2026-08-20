include $(TOPDIR)/rules.mk

PKG_NAME:=hmlangw
PKG_VERSION:=1.1.0
PKG_RELEASE:=2

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/hmlangw
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Homematic BidCoS LAN Gateway
  DEPENDS:=+libstdcpp
endef

define Package/hmlangw/description
 hmlangw implements a Homematic BidCoS-over-LAN gateway
 for HM-MOD-RPI-PCB and compatible Homematic RF modules.
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-c $(PKG_BUILD_DIR)/hmframe.cpp \
		-o $(PKG_BUILD_DIR)/hmframe.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-c $(PKG_BUILD_DIR)/hmlangw.cpp \
		-o $(PKG_BUILD_DIR)/hmlangw.o

	$(TARGET_CXX) $(TARGET_LDFLAGS) \
		-o $(PKG_BUILD_DIR)/hmlangw \
		$(PKG_BUILD_DIR)/hmlangw.o \
		$(PKG_BUILD_DIR)/hmframe.o \
		-pthread
endef

define Package/hmlangw/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hmlangw \
		$(1)/usr/bin/hmlangw

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/etc/init.d/hmlangw \
		$(1)/etc/init.d/hmlangw
endef

$(eval $(call BuildPackage,hmlangw))
