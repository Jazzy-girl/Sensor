#include <android_native_app_glue.h>
#include <jni.h>
#include <android/log.h>

// This is where the app begins its execution.
void android_main(struct android_app* app) {
    app_dummy(); // This initializes the glue.

    // Your app's main loop goes here.
    while (true) {
        // Check for events (input, lifecycle, etc.)
        int ident;
        int events;
        struct android_poll_source* source;

        // Poll for events.
        while ((ident = ALooper_pollAll(-1, NULL, &events, (void**)&source)) >= 0) {
            // Process events.
            if (source != NULL) {
                source->process(app, source);
            }

            // If the app has been requested to quit, break the loop.
            if (app->destroyRequested != 0) {
                return;
            }
        }

        // Add your code to process UDP data or other tasks here
        // e.g., you might want to call your UDP sender code here
    }
}
