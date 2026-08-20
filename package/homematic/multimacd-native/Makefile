include $(TOPDIR)/rules.mk

PKG_NAME:=multimacd-native
PKG_VERSION:=1
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/OpenCCU/OpenCCU-Base.git
PKG_SOURCE_VERSION:=5e9d5a4f59aeea8f951dacd1ba29edc60fc314b6
PKG_MIRROR_HASH:=skip

PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/multimacd-native
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Native OpenCCU multimacd for Homematic
  DEPENDS:=+libstdcpp +libopenssl +kmod-eq3-char-loop +kmod-generic-raw-uart
endef

define Package/multimacd-native/description
 Native OpenWrt/musl build of OpenCCU multimacd and libelvutils.
 Sources are pinned to OpenCCU-Base commit
 5e9d5a4f59aeea8f951dacd1ba29edc60fc314b6.
endef

define Build/Prepare
	$(call Build/Prepare/Default)

	grep -q 'sys/select.h' \
		$(PKG_BUILD_DIR)/src/libelvutils/MulticastSender.cpp || \
	sed -i '/unistd.h/a #include <sys/select.h>' \
		$(PKG_BUILD_DIR)/src/libelvutils/MulticastSender.cpp

	# musl: strerror_r() returns int instead of GNU char*
	sed -i 's/char \*err = strerror_r( errno, buffer, sizeof( buffer ) );/strerror_r( errno, buffer, sizeof( buffer ) ); char *err = buffer;/g' \
		$(PKG_BUILD_DIR)/src/multimacd/BidcosMacController.cpp
endef

define Build/Compile
	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/Actor.cpp \
		-o $(PKG_BUILD_DIR)/Actor.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/ActorMessage.cpp \
		-o $(PKG_BUILD_DIR)/ActorMessage.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/ConsoleLogger.cpp \
		-o $(PKG_BUILD_DIR)/ConsoleLogger.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/Crc16.cpp \
		-o $(PKG_BUILD_DIR)/elv-Crc16.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/FileLogger.cpp \
		-o $(PKG_BUILD_DIR)/FileLogger.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/Hexfile.cpp \
		-o $(PKG_BUILD_DIR)/Hexfile.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/Logger.cpp \
		-o $(PKG_BUILD_DIR)/Logger.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/md5.cpp \
		-o $(PKG_BUILD_DIR)/md5.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/MulticastSender.cpp \
		-o $(PKG_BUILD_DIR)/MulticastSender.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/OSCompatLinux.cpp \
		-o $(PKG_BUILD_DIR)/OSCompatLinux.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/PropertyMap.cpp \
		-o $(PKG_BUILD_DIR)/PropertyMap.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/PThreadCondition.cpp \
		-o $(PKG_BUILD_DIR)/PThreadCondition.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/SyslogLogger.cpp \
		-o $(PKG_BUILD_DIR)/SyslogLogger.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/TimeZoneInfo.cpp \
		-o $(PKG_BUILD_DIR)/TimeZoneInfo.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/TimerQueue.cpp \
		-o $(PKG_BUILD_DIR)/TimerQueue.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/TimerTarget.cpp \
		-o $(PKG_BUILD_DIR)/TimerTarget.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/TimestampLinux.cpp \
		-o $(PKG_BUILD_DIR)/TimestampLinux.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/unsigned_char_traits.cpp \
		-o $(PKG_BUILD_DIR)/unsigned_char_traits.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/UpdateFile.cpp \
		-o $(PKG_BUILD_DIR)/UpdateFile.o

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall -fPIC \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-c $(PKG_BUILD_DIR)/src/libelvutils/utils.cpp \
		-o $(PKG_BUILD_DIR)/utils.o

	$(TARGET_CXX) -shared $(TARGET_LDFLAGS) \
		-o $(PKG_BUILD_DIR)/libelvutils.so \
		$(PKG_BUILD_DIR)/Actor.o \
		$(PKG_BUILD_DIR)/ActorMessage.o \
		$(PKG_BUILD_DIR)/ConsoleLogger.o \
		$(PKG_BUILD_DIR)/elv-Crc16.o \
		$(PKG_BUILD_DIR)/FileLogger.o \
		$(PKG_BUILD_DIR)/Hexfile.o \
		$(PKG_BUILD_DIR)/Logger.o \
		$(PKG_BUILD_DIR)/md5.o \
		$(PKG_BUILD_DIR)/MulticastSender.o \
		$(PKG_BUILD_DIR)/OSCompatLinux.o \
		$(PKG_BUILD_DIR)/PropertyMap.o \
		$(PKG_BUILD_DIR)/PThreadCondition.o \
		$(PKG_BUILD_DIR)/SyslogLogger.o \
		$(PKG_BUILD_DIR)/TimeZoneInfo.o \
		$(PKG_BUILD_DIR)/TimerQueue.o \
		$(PKG_BUILD_DIR)/TimerTarget.o \
		$(PKG_BUILD_DIR)/TimestampLinux.o \
		$(PKG_BUILD_DIR)/unsigned_char_traits.o \
		$(PKG_BUILD_DIR)/UpdateFile.o \
		$(PKG_BUILD_DIR)/utils.o \
		-lcrypto -pthread

	echo '#define MULTIMACD_VERSION "OpenCCU-Base-5e9d5a4f"' \
		> $(PKG_BUILD_DIR)/generated-multimacd-version.h

	find $(PKG_BUILD_DIR)/src/multimacd \
		-type f -name '*.cpp' -print \
		> $(PKG_BUILD_DIR)/multimacd-sources.rsp

	$(TARGET_CXX) $(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS) \
		-std=c++11 -Wall \
		-D_TIME_BITS=64 -D_FILE_OFFSET_BITS=64 \
		-I$(PKG_BUILD_DIR)/src/multimacd \
		-I$(PKG_BUILD_DIR)/src/libelvutils \
		-I$(PKG_BUILD_DIR) \
		@$(PKG_BUILD_DIR)/multimacd-sources.rsp \
		$(TARGET_LDFLAGS) \
		-L$(PKG_BUILD_DIR) \
		-Wl,-rpath-link,$(PKG_BUILD_DIR) \
		-o $(PKG_BUILD_DIR)/multimacd \
		-lelvutils -lcrypto -pthread -lrt -lm
endef

define Package/multimacd-native/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/multimacd \
		$(1)/usr/bin/multimacd

	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libelvutils.so \
		$(1)/usr/lib/libelvutils.so

	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/multimacd.conf \
		$(1)/etc/config/multimacd.conf

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/multimacd.init \
		$(1)/etc/init.d/multimacd
endef

$(eval $(call BuildPackage,multimacd-native))
