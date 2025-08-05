/* Main.cpp
Note on NativeActivity:
If you're using android_main as the entry point, your android_main function and handleAppCmd, getSensorEvents 
should ideally be in a file like main.cpp, and udp_sender.cpp would contain the UDP sending logic. This separates concerns.

src/main/cpp/udp_sender.cpp (No JNI headers or extern "C" JNIEXPORT functions)
This would be the core C++ UDP and threading logic, called directly from main.cpp (your android_main context).

src/main/cpp/main.cpp (Entry point for NativeActivity)

This file contains the android_main function and the sensor/looper logic, calling functions from udp_sender.cpp.
*/

// This is a simplified example of main.cpp
// Full implementation as provided in previous answer with ASensorManager and ALooper
#include <android_native_app_glue.h>
#include <android/sensor.h>
#include <android/log.h>
#include <string>
// Include your udp_sender.h if you create one
// #include "udp_sender.h" // For declarations of startUdpSender, stopUdpSender etc.

// #define UDP_IP "192.168.216.133"
#define UDP_IP "10.0.2.2"
#define UDP_PORT 12345
#define LOG_TAG "NativeMain"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Forward declarations for functions defined in udp_sender.cpp
void startUdpSender(const char* ipAddress, int port);
void stopUdpSender();
void queueAccelerometerData(long timestamp, float x, float y, float z); // New function signature

// Sensor related globals and callback (as in previous 'pure C++' answer)
static ASensorManager* sensorManager;
static const ASensor* accelerometerSensor;
static ASensorEventQueue* sensorEventQueue;
static ALooper* looper;

// enum {
//     LOOPER_ID_USER = 3,
// };

static int getSensorEvents(int fd, int events, void* data) {
    LOGI("getSensorEvents triggered with events: %d", events);
    if ((events & ALOOPER_EVENT_INPUT) != 0) {
        ASensorEvent sensorEvent;
        while (ASensorEventQueue_getEvents(sensorEventQueue, &sensorEvent, 1) > 0) {
            LOGI("Received sensor type: %d", sensorEvent.type);
            if (sensorEvent.type == ASENSOR_TYPE_ACCELEROMETER) {
                // Call the function from udp_sender.cpp
                queueAccelerometerData(sensorEvent.timestamp,
                                        sensorEvent.vector.v[0],
                                        sensorEvent.vector.v[1],
                                        sensorEvent.vector.v[2]);
            }
        }
    }
    return 1;
}

static void handleAppCmd(struct android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            LOGI("APP_CMD_INIT_WINDOW received");
            if (app->window != NULL) {
                // Initialize sensors and start UDP sender
                // sensorManager = ASensorManager_getInstance();
                sensorManager = ASensorManager_getInstanceForPackage(NULL);
                accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
                looper = ALooper_forThread();
                sensorEventQueue = ASensorManager_createEventQueue(sensorManager, looper, LOOPER_ID_USER, getSensorEvents, NULL);
                ASensorEventQueue_setEventRate(sensorEventQueue, accelerometerSensor, 20000); // 50 Hz
                ASensorEventQueue_enableSensor(sensorEventQueue, accelerometerSensor);

                startUdpSender(UDP_IP, UDP_PORT); // !!! change IP AND PORT !!!
                LOGI("App initialized, accelerometer and UDP sender started.");
            }
            break;
        case APP_CMD_TERM_WINDOW:
            stopUdpSender();
            if (sensorEventQueue != NULL) {
                ASensorEventQueue_disableSensor(sensorEventQueue, accelerometerSensor);
                ASensorManager_destroyEventQueue(sensorManager, sensorEventQueue);
                sensorEventQueue = NULL;
                LOGI("App terminated, accelerometer and UDP sender stopped.");
            }
            break;
        case APP_CMD_DESTROY:
            stopUdpSender();
            LOGI("App destroyed.");
            break;
        // Handle other commands as needed
    }
}

void android_main(struct android_app* app) 
{
    app->onAppCmd = handleAppCmd;

    int ident;
    int events;
    struct android_poll_source* source;

    while (true) 
        {
        if (ASensorEventQueue_hasEvents(sensorEventQueue)) {
            LOGI("Sensor queue has events ready");
        }
        ident = ALooper_pollAll(-1, NULL, &events, (void**)&source);    // see ALooper.notes
        if (ident >= 0) 
            {
            if (source != NULL) 
                {
                source->process(app, source);
                }
            }
        if (app->destroyRequested != 0) {
            break;
        }
    }
}

// Add a queueAccelerometerData function to udp_sender.cpp that simply adds to the queue
// void queueAccelerometerData(long timestamp, float x, float y, float z) {
//     AccelerometerData data = {timestamp, x, y, z};
//     {
//         std::unique_lock<std::mutex> lock(dataMutex);
//         dataQueue.push(data);
//     }
// }
