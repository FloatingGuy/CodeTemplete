LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := weightpointer.cpp
LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils
LOCAL_MODULE := weightpointer
include $(BUILD_EXECUTABLE)