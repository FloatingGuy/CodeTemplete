LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := lightpointer
LOCAL_SRC_FILES := lightpointer.cpp
LOCAL_SHARED_LIBRARIES := \
	libcutils  \
	libutils  \

	# libbinder
# LOCAL_C_INCLUDES := \
	$(TOP)/frameworks/rs/cpp/

include $(BUILD_EXECUTABLE)