#!/bin/bash
# build.sh
# This shell script is to automate the ndk-build command and thn usd 'adb' to install th APK 
# See : RunningScript.md  and BuidingSDK.md in notes/

### IN PROGRESS (the ------- is as far as I have gone so far 250728) ###

# NOTES 
# $HOME : this is your <home> directory like /home/ry or /home/tt - in the envirionment  ('env | grep HOME')
# $PWD  : is the current working directory   TODO: replace $PWD with $DVLP_DIR

# --- Configuration ---
# Set these paths according to the host system
# Ensure NDK_HOME points to the NDK installation directory
export NDK_HOME="${HOME}/Android/Sdk/ndk/25.1.8937393" # <--- TODO: Update this to the NDK path and version!
ANDROID_SDK_ROOT="${HOME}/Android/Sdk"                 # <--- TODO: Update this to the SDK path
BUILD_TOOLS_VERSION="36.0.0"                           # <--- TODO: Update to the installed build tools version
PLATFORM_VERSION="android-36"                          # <--- TODO: Update to the target Android API level

# Output directory for compiled native libraries (created by ndk-build)
LIBS_DIR="lib"

# Development Root - all files for project live under this  (customize for your system if not this)
DVLP_DIR="/home/rywilson/Sensor/Sensor"

# Package name for the APK (must match AndroidManifest.xml)
PACKAGE_NAME="dev.corticalsystmatics.android_sensor" 

# Name of the shared library (must match Android.mk LOCAL_MODULE)
LIB_NAME="accelerometer-udp"

# --- Clean previous builds ---  TODO: need a flag to trig 'clean' or 'fresh' build
echo "--- Cleaning previous builds ---"
rm -rf obj "$LIBS_DIR" gen bin temp
mkdir -p "$LIBS_DIR"

# --- Build Native Code with ndk-build ---
echo "--- Building native code with ndk-build ---"

# cd src/cpp    # ndk-build expects Android.mk in jni/ or the specified path
# Using NDK_PROJECT_PATH and APP_BUILD_SCRIPT to point to Android.mk
"$NDK_HOME"/ndk-build \
  NDK_PROJECT_PATH="$DVLP_DIR" \
  APP_BUILD_SCRIPT="$DVLP_DIR/src/cpp/Android.mk" \
  APP_PLATFORM="latest" \
  APP_ABI="arm64-v8a" \
  APP_STL="c++_static" \
  V=1 # Verbose output

if [ $? -ne 0 ]; then
  echo "Native build failed!"
  exit 1
fi
echo "Native build successful."

# --- Create APK structure and compile Java/resources (even for NativeActivity) ---
# Even for NativeActivity, a minimal APK structure and manifest are needed.
# This part mimics what Android Studio's build tools do.
echo "--- Preparing APK resources and manifest ---"

# Create directories for compiled classes and resources
mkdir -p bin/classes

# Copy compiled native libraries to the correct APK structure
# ndk-build outputs to libs/ABI/lib<name>.so, so we need to copy it
# ?? NOT sure we need/want $PWD - replace with DVLP_DIR ??
mkdir -p "$LIBS_DIR"/arm64-v8a
cp "$PWD"/obj/local/arm64-v8a/lib"${LIB_NAME}".so "$LIBS_DIR"/arm64-v8a/lib"${LIB_NAME}".so

# Copy AndroidManifest.xml (and resources if any, though none for headless NativeActivity)
# This is a very minimal step for a pure native app.
# For more complex apps, you'd use aapt to process resources and assets.
# We just need the Manifest and the native libs.

# --- Build Unsigned APK ---
echo "--- Building unsigned APK ---"

mkdir -p assets
mkdir -p res
mkdir -p gen
"$ANDROID_SDK_ROOT"/build-tools/"$BUILD_TOOLS_VERSION"/aapt package \
    -f -m -J gen/ \
    -S res \
    -A assets \
    -M xml/AndroidManifest.xml \
    -I "$ANDROID_SDK_ROOT"/platforms/"$PLATFORM_VERSION"/android.jar \
    -F bin/unsigned.apk \
    "$LIBS_DIR" # Include the libs directory so aapt bundles them

if [ $? -ne 0 ]; then
  echo "APK packaging failed!"
  exit 1
fi
echo "APK packaging successful."

# --- Sign APK (for installation) ---
echo "--- Signing APK ---"
# Create a debug keystore if you don't have one
DEBUG_KEYSTORE="${HOME}/.android/debug.keystore"
if [ ! -f "$DEBUG_KEYSTORE" ]; then
    echo "Creating debug keystore..."
    keytool -genkeypair -v -keystore "$DEBUG_KEYSTORE" \
            -alias androiddebugkey -storepass android -keypass android \
            -keyalg RSA -keysize 2048 -validity 10000 \
            -dname "CN=Android Debug, O=Android, C=US"
fi

"$ANDROID_SDK_ROOT"/build-tools/"$BUILD_TOOLS_VERSION"/apksigner sign \
    --ks "$DEBUG_KEYSTORE" \
    --ks-key-alias androiddebugkey \
    --ks-pass pass:android \
    --key-pass pass:android \
    --out bin/"${PACKAGE_NAME}".apk \
    bin/unsigned.apk

if [ $? -ne 0 ]; then
  echo "APK signing failed!"
  exit 1
fi
echo "APK signing successful: bin/${PACKAGE_NAME}.apk"

# --- Install APK ---
echo "--- Installing APK ---"
"$ANDROID_SDK_ROOT"/platform-tools/adb install -r bin/"${PACKAGE_NAME}".apk

if [ $? -ne 0 ]; then
  echo "APK installation failed! Is a device/emulator connected and ADB working?"
  exit 1
fi
echo "APK installed successfully."
echo "You can now run 'adb logcat -s NativeMain android-sensor' to see logs."