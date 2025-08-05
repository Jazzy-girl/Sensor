# /src/main/cpp/Android.mk
# Deprecated: Google officially deprecated the ndk-build system (which is Makefile-based) in favor of CMake for new projects and actively encourages migrating existing projects.

# Define the root of your source files
LOCAL_PATH := $(call my-dir)

# Clear variables from previous modules (important for multiple modules)
include $(CLEAR_VARS)

# Define the name of your shared library
# This name should match the 'android.app.lib_name' meta-data in AndroidManifest.xml
LOCAL_MODULE := accelerometer-udp

# Specify your C++ source files
LOCAL_SRC_FILES := Main.cpp udp_sender.cpp

# Add main.cpp if you're using NativeActivity directly
# LOCAL_SRC_FILES += main.cpp

# Specify the NDK headers needed
LOCAL_C_INCLUDES := \
    $(NDK_ROOT)/sources/android/native_app_glue \
    $(NDK_ROOT)/sources/android/ndk_helper \
    $(NDK_ROOT)/platforms/android-$(APP_PLATFORM)/arch-$(TARGET_ARCH_ABI)/usr/include

# Specify the shared libraries to link against
# -landroid for android_native_app_glue
# -llog for __android_log_print
# -landroid_native_app_glue links against the static native_app_glue library
# -lEGL and -lGLESv1_CM are often needed for NativeActivity, even if not rendering directly
LOCAL_LDLIBS := -landroid -llog -lEGL -lGLESv1_CM

# Define the static library for native_app_glue (needed for android_main)
LOCAL_STATIC_LIBRARIES := android_native_app_glue

# Build as a shared library
include $(BUILD_SHARED_LIBRARY)

# Include the native_app_glue module which defines the static library
$(call import-module,android/native_app_glue) 
