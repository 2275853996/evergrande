LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := jni_sensor_factory.cpp \

LOCAL_MULTILIB := 32

LOCAL_MODULE := libjnisensorfactor
LOCAL_MODULE_TAGS := optional
LOCAL_SHARED_LIBRARIES := liblog libutils libutils libbinder
LOCAL_C_INCLUDES := $(LOCAL_PATH)/ \
		$(TOP)/frameworks/base/include/
include $(BUILD_SHARED_LIBRARY)


