LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	CVE_2016_0810.cpp \

LOCAL_SHARED_LIBRARIES := \
	liblog \
	libdl \

LOCAL_STATIC_LIBRARIES := \
	libtoolkit \

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../include/wheeljack \
	$(LOCAL_PATH)/../libtoolkit/include \
	$(LOCAL_PATH)/ \
	bionic \

ifeq ($(shell expr $(PLATFORM_SDK_VERSION) "<" 23), 1)
	LOCAL_SHARED_LIBRARIES += libstlport
	LOCAL_C_INCLUDES += \
		bionic/libstdc++/include \
		external/stlport/stlport
endif

ifeq ($(shell expr $(PLATFORM_SDK_VERSION) "=" 23), 1)
	LOCAL_C_INCLUDES += \
		frameworks/base/media/jni/soundpool
else
	LOCAL_C_INCLUDES += \
		frameworks/av/include/media
endif

LOCAL_MODULE:= CVE_2016_0810

include $(BUILD_SHARED_LIBRARY)
