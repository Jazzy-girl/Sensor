To build an Android application with native C/C++ components (using the NDK) from the command line on Linux, specifically with Makefiles (ndk-build), you'll need to install the following tools:

sources : developeer.android.com
          makimo.com for tombstone files (created on crash)
Essential Tools:
Java Development Kit (JDK):

Android's build system (even for native apps) relies on Java. You'll need a compatible JDK. OpenJDK is generally fine on Linux.
How to install (Ubuntu/Debian example):
Bash

sudo apt update
sudo apt install openjdk-17-jdk # Or a newer LTS version like openjdk-21-jdk
Verify installation: java -version and javac -version
Android SDK Command-line Tools:

This package contains essential tools like sdkmanager, aapt (Android Asset Packaging Tool), apksigner, zipalign, and adb (Android Debug Bridge).
Installation:
Go to the Android Studio download page: https://developer.android.com/studio#downloads
Scroll down to "Command line tools only" and download the Linux version (a .zip file).
Unzip it to a location of your choice. A common convention is ~/Android/Sdk/cmdline-tools. Let's assume you put it in ~/Android/Sdk.
Important: The sdkmanager executable is inside cmdline-tools/latest/bin (or similar depending on the version you download). You often need to create the latest directory and move the unzipped contents into it.
Bash

mkdir -p ~/Android/Sdk/cmdline-tools/latest
mv <path_to_unzipped_command_line_tools>/* ~/Android/Sdk/cmdline-tools/latest/

Add to PATH: Add the platform-tools and cmdline-tools/latest/bin directories to your system's PATH environment variable in your ~/.bashrc or ~/.profile file:
Bash

export ANDROID_SDK_ROOT="$HOME/Android/Sdk"
export PATH="$PATH:$ANDROID_SDK_ROOT/platform-tools"
export PATH="$PATH:$ANDROID_SDK_ROOT/cmdline-tools/latest/bin"
Then run source ~/.bashrc (or ~/.profile) to apply changes.
Android NDK (Native Development Kit):

This is the core for building C/C++ code for Android. It contains the cross-compilers, build tools (ndk-build), and native APIs.
Installation (using sdkmanager): This is the recommended way to get the NDK.
Bash

# List available NDK versions
sdkmanager --list | grep "ndk;"

# Install a specific NDK version (e.g., latest LTS, typically 'ndk;25.2.9519653' or 'ndk;26.1.10909125')
# Replace the version with what you found in the --list output.
sdkmanager "ndk;<version_number>"
The NDK will be installed inside ~/Android/Sdk/ndk/<version_number>. You'll use this path for your NDK_HOME in the build.sh script.
Android SDK Platform:

You need the SDK platform for the target Android API level (e.g., android-34). This provides the android.jar and other resources needed for compilation.
Installation (using sdkmanager):
Bash

# List available platforms
sdkmanager --list | grep "platforms;android-"

# Install a specific platform (e.g., API 34)
sdkmanager "platforms;android-34"
Android SDK Build Tools:

These are version-specific tools like aapt2, dx, d8, etc., which are crucial for packaging APKs.
Installation (using sdkmanager):
Bash

# List available build tools
sdkmanager --list | grep "build-tools;"

# Install a specific version (e.g., 34.0.0, often the latest stable)
sdkmanager "build-tools;34.0.0"
Other Helpful Tools:
unzip: For extracting the downloaded SDK command-line tools and NDK if you download them manually. Most Linux distributions have this pre-installed or easily available via package manager (sudo apt install unzip).
make: The build system itself relies on make. Usually pre-installed on Linux. (sudo apt install make if not).
Text Editor: Any text editor (nano, vim, VS Code, Sublime Text, etc.) to edit your C++ files, AndroidManifest.xml, and Android.mk.
adb: While installed with platform-tools, knowing adb commands for debugging (e.g., adb logcat, adb install, adb shell) is invaluable.
Summary of Installation Steps (Command-line focused):
Install Java JDK.
Download and set up Android SDK Command-line Tools.
Set ANDROID_SDK_ROOT and update PATH environment variables.
Use sdkmanager to install:
The desired NDK version (ndk;<version>)
The target SDK Platform (platforms;android-<API_level>)
The latest Build Tools (build-tools;<version>)
(Optional but recommended for command line) Platform Tools (platform-tools) - this includes adb.
Accept Licenses: After installing with sdkmanager, you might need to accept licenses:
Bash

sdkmanager --licenses
Review the licenses and type y to accept them.
Once these tools are installed and your environment variables are correctly set up, you should be able to navigate to your project directory (the one containing src/main/cpp and your AndroidManifest.xml) and run the build.sh script we discussed previously to build and install your native Android app.