LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= cve_2016_0810.cpp
#$(call all-subdir-c-files)  
#cve_2016_0810.cpp

LOCAL_MODULE_TAGS := optional
#LOCAL_CXX = g++
#LOCAL_PRELINK_MODULE := false
#LD_FLAGS := -shared
LOCAL_SHARED_LIBRARIES := \
	liblog \
	libdl \
	libutils \
	libcutils \
	libc \
	

LOCAL_STATIC_LIBRARIES := \

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/ \
	bionic \
	frameworks/rs/server \
	frameworks/av/media/libstagefright/mpeg2ts \
	frameworks/native/include/   \

#	frameworks/av/include/media \
#	frameworks/base/media/jni/soundpool \


ifeq ($(shell expr $(PLATFORM_SDK_VERSION) "=" 23), 1)
	LOCAL_SHARED_LIBRARIES += libstlport
	LOCAL_C_INCLUDES += \
		frameworks/base/media/jni/soundpool
else
	LOCAL_C_INCLUDES += \
		frameworks/av/include/media 
endif


LOCAL_MODULE:= CVE_2016_0810

include $(BUILD_EXECUTABLE)
