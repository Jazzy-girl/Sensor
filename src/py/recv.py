import socket

''' Receiver - this should als be C++ 
    This is to test that messages are being sent from Android App 'android-sensor' '''

UDP_IP = "192.168.216.133"  # Listen on all available interfaces
UDP_PORT = 12345    # Must match the port in your Android app

sock = socket.socket(socket.AF_INET,  # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening for UDP data on {UDP_IP}:{UDP_PORT}")

try:
    while True:
        data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes  TT: may want to trim this down to actual size of message 
        message = data.decode('utf-8')
        print(f"Received from {addr}: {message}")

        # You can parse the message here  TT: for dvlp we will use the C String, but future messages will be binary 
        try:
            timestamp_ns, x, y, z = map(float, message.split(','))
            timestamp_ms = timestamp_ns / 1_000_000.0
            print(f"  Parsed Data: Timestamp_ms={timestamp_ms:.2f}, X={x:.2f}, Y={y:.2f}, Z={z:.2f}")
        except ValueError:
            print("  Failed to parse data.")

except KeyboardInterrupt:
    print("Server stopped.")
finally:
    sock.close()
