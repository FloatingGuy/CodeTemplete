LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES:= \
 		test.cpp

LOCAL_SHARED_LIBRARIES := \
	libbinder liblog libc libcutils libutils \
	libdl libmedia

#error: undefined reference to 'android::IMediaPlayerService::asInterface(android::sp<android::IBinder> const&)'

LOCAL_WHOLE_STATIC_LIBRARIES := \



LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)	\
	bionic \
    $(TOP)/frameworks/av/include/ \
    $(TOP)/frameworks/native/include/hardware/ \
    $(TOP)/frameworks/native/include/binder  \
    $(TOP)/frameworks/rs/server 


LOCAL_CFLAGS += -Werror -Wno-error=deprecated-declarations -Wall
#LOCAL_CLANG := true

LOCAL_MODULE:= CVE_2016_0811
include $(BUILD_EXECUTABLE)

