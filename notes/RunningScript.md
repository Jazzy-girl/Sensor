RunningScript.notes

How to use build.sh:

Save the script: Save the above content as build.sh in your project's root directory.
Make it executable: chmod +x build.sh
Update Paths: Crucially, update NDK_HOME, ANDROID_SDK_ROOT, BUILD_TOOLS_VERSION, and PLATFORM_VERSION to match your Android SDK/NDK installation and desired API levels.
Update Package/Lib Names: Ensure PACKAGE_NAME and LIB_NAME match your AndroidManifest.xml and Android.mk.
Run: ./build.sh
Disclaimer: This Makefile and build.sh are a simplified representation. Real-world Android projects built with Makefiles (e.g., using ndk-build for a larger project) usually involve more complex Android.mk files and often an Application.mk file to define target ABIs, STL, etc. Debugging pure NDK builds without the integrated IDE tools can also be significantly harder.

My Strong Recommendation:
While I've provided the Makefile approach as requested, I strongly advise against it for new Android NDK projects. The official toolchain (Gradle + CMake in Android Studio) provides:

Better cross-platform compatibility (for different OSes, NDK versions).
Simplified dependency management.
Integrated debugging.
Better support for multiple ABIs.
A much smoother development experience.