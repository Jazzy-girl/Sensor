// src/main/java/com/yourpackagename/yourappname/MainActivity.java
// This is to handle UI, sensor registration, and interaction with the UdpSender class 
package java;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity implements SensorEventListener {

    private static final String TAG = "MainActivity";
    private SensorManager sensorManager;
    private Sensor accelerometer;
    private UdpSender udpSender;

    private EditText ipAddressInput;
    private EditText portInput;
    private Button startButton;
    private Button stopButton;
    private TextView dataDisplay;

    private String targetIp = "192.168.1.100"; // Replace with your target IP
    private int targetPort = 12345; // Replace with your target port

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main); // Assuming you have activity_main.xml layout

        ipAddressInput = findViewById(R.id.ipAddressInput);
        portInput = findViewById(R.id.portInput);
        startButton = findViewById(R.id.startButton);
        stopButton = findViewById(R.id.stopButton);
        dataDisplay = findViewById(R.id.dataDisplay);

        ipAddressInput.setText(targetIp);
        portInput.setText(String.valueOf(targetPort));

        udpSender = new UdpSender();

        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        if (sensorManager != null) {
            accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
            if (accelerometer == null) {
                Log.e(TAG, "Accelerometer not found on this device.");
                dataDisplay.setText("Accelerometer not available.");
            }
        } else {
            Log.e(TAG, "SensorManager not available.");
            dataDisplay.setText("SensorManager not available.");
        }

        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                targetIp = ipAddressInput.getText().toString();
                try {
                    targetPort = Integer.parseInt(portInput.getText().toString());
                } catch (NumberFormatException e) {
                    Log.e(TAG, "Invalid port number", e);
                    dataDisplay.setText("Invalid port number.");
                    return;
                }
                startSending();
            }
        });

        stopButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stopSending();
            }
        });
    }

    private void startSending() {
        if (accelerometer != null) {
            sensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_GAME);
            udpSender.nativeStartUdpSender(targetIp, targetPort);
            startButton.setEnabled(false);
            stopButton.setEnabled(true);
            dataDisplay.setText("Sending data to " + targetIp + ":" + targetPort);
            Log.d(TAG, "Accelerometer listening and UDP sender started.");
        } else {
            Log.e(TAG, "Cannot start: Accelerometer not available.");
            dataDisplay.setText("Cannot start: Accelerometer not available.");
        }
    }

    private void stopSending() {
        if (accelerometer != null) {
            sensorManager.unregisterListener(this);
        }
        udpSender.nativeStopUdpSender();
        startButton.setEnabled(true);
        stopButton.setEnabled(false);
        dataDisplay.setText("Stopped sending data.");
        Log.d(TAG, "Accelerometer listener unregistered and UDP sender stopped.");
    }

    @Override
    protected void onPause() {
        super.onPause();
        stopSending(); // Stop when activity goes into background
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        stopSending(); // Ensure everything is stopped when activity is destroyed
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            long timestamp = event.timestamp; // Nanoseconds
            float x = event.values[0];
            float y = event.values[1];
            float z = event.values[2];

            // Queue data for native sender thread
            udpSender.nativeQueueAccelerometerData(timestamp, x, y, z);

            // Update UI (optional, for debugging)
            runOnUiThread(() -> {
                String data = String.format("X: %.2f, Y: %.2f, Z: %.2f (%.2fHz)",
                                            x, y, z, (float) (1e9 / (System.nanoTime() - lastTimestamp)));
                dataDisplay.setText(data);
                lastTimestamp = System.nanoTime();
            });
        }
    }

    private long lastTimestamp = 0; // For simple frequency estimate

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // Not typically used for accelerometer data
    }
}
