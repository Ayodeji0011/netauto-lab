# Firebase Controlled LED Display

An ESP32 LED display project using Firebase Realtime Database for remote control and display configuration.

## Architecture

Mobile/Web Interface
        |
     Firebase
        |
   Realtime Database
        |
       ESP32
        |
    P10 Display

## Features

The system was designed to receive display settings remotely, including:

- Text
- Display effects
- Font selection
- Font size
- Frame settings
- Display colour
- Device-specific settings

## Device

A device-node structure was used so individual display boards could retrieve their assigned configuration.

## Development

The ESP32 firmware was developed using Arduino IDE.

Private Firebase credentials and API secrets are intentionally excluded from this repository.
