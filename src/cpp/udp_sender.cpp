#include <android_native_app_glue.h>
#include <jni.h>
#include <android/log.h>
#include <string>
#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


#define LOG_TAG "UdpSender"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
// This is where the app begins its execution.

struct AccelerometerData {
    long timestamp;
    float x, y, z;
};

std::thread senderThread;
std::queue<AccelerometerData> dataQueue;
std::mutex dataMutex;
std::condition_variable dataCond;
std::atomic<bool> running(false);

int socket_fd;
struct sockaddr_in server_addr;

void queueAccelerometerData(long timestamp, float x, float y, float z) {
    LOGI("Sending: %ld, %.2f, %.2f, %.2f", timestamp, x, y, z);
    std::unique_lock<std::mutex> lock(dataMutex);
    dataQueue.push({timestamp, x, y, z});
    dataCond.notify_one();
}

void udpSendLoop(){
    while (running){
        AccelerometerData data; 
        {
            std::unique_lock<std::mutex> lock(dataMutex);
            dataCond.wait(lock, [] {return !dataQueue.empty() || !running;});
            if (!running && dataQueue.empty()) break;

            data = dataQueue.front();
            dataQueue.pop();
        }
    

    std::string msg = std::to_string(data.timestamp) + "," +
                    std::to_string(data.x) + "," +
                    std::to_string(data.y) + "," +
                    std::to_string(data.z);

    sendto(socket_fd, msg.c_str(), msg.length(), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    }
}

void startUdpSender(const char* ipAddress, int port) {
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        LOGE("Failed to create socket");
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress, &server_addr.sin_addr);

    running = true;
    senderThread = std::thread(udpSendLoop);
    LOGI("UDP sender started");
}

void stopUdpSender() {
    running = false;
    dataCond.notify_all();
    if (senderThread.joinable()){
        senderThread.join();
    }

    if (socket_fd >= 0){
        close(socket_fd);
    }

    LOGI("UDP sender stopped");
}


