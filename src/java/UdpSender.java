// 3
// src/main/java/com/yourpackagename/yourappname/UdpSender.java
// Create a class to encapsulate your JNI calls.
package java;

public class UdpSender {

    static {
        // Load the native library when the class is loaded
        System.loadLibrary("accelerometer-udp"); // Name matches add_library in CMakeLists.txt
    }

    // Native methods (JNI signatures)
    public native void nativeStartUdpSender(String ipAddress, int port);
    public native void nativeStopUdpSender();
    public native void nativeQueueAccelerometerData(long timestamp, float x, float y, float z);
}
