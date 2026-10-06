# System Architecture

## Sender / Emergency Node
Generates emergency events and packets containing information such as node ID, alert type, timestamp, latitude, longitude and hop count.

## Relay Node
Receives and forwards emergency packets toward the base receiver.

## Receiver / Base Node
Provides local notification through OLED, LEDs and buzzer.

## Communication
ESP-NOW provides direct ESP32-to-ESP32 communication without requiring a conventional Wi-Fi access point for node-to-node communication.

## Future Improvements
Authenticated encryption, replay protection, sequence numbers, node authentication, dynamic routing, acknowledgements, battery monitoring and larger field testing.
