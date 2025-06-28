KeyConcepts

1 Android NDK (Native Development Kit): This is essential for writing C/C++ code for Android.

2. JNI (Java Native Interface): This is the bridge between your Java/Kotlin Android code and your C/C++ native code. You'll use JNI to call your C++ function from Java/Kotlin and to potentially pass sensor data from Java/Kotlin to C++.

3. Android Sensors: You'll typically access the accelerometer data in Java/Kotlin (using SensorManager and SensorEventListener) and then pass this data to your C++ code for network transmission. While it's possible to access sensors directly in C++ via the NDK, it's often more straightforward to handle sensor callbacks in Java/Kotlin.
4. UDP Sockets (C++): You'll use standard C++ socket programming to create a UDP socket, bind it (optional for client), and send data.

5. Permissions: Your Android app will need internet and sensor permissions.

6. Threading: Network operations should never be performed on the main Android UI thread. You'll need to use a separate thread for your UDP sending logic.


onSensorChanged

public abstract void onSensorChanged (SensorEvent event)   // event is not owned by the caller, so copy out what is needed 

Called when there is a new sensor event. Note that "on changed" is somewhat of a misnomer, as this will also be called if we have a new reading from a sensor with the exact same sensor values (but a newer timestamp).

See SensorManager for details on possible sensor types.

https://developer.android.com/reference/android/hardware/SensorEvent