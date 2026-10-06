# ESP32 P10 Smart Display

An ESP32-based LED display system using HUB12 P10 LED panels.

## Hardware

- ESP32
- Red P10 HUB12 LED panels
- HUB12 interface
- 5 V power supply
- Audio hardware for speech/audio experiments

The display was developed and tested with multi-panel arrangements including 3 panels across and multiple rows.

## Work

- Direct P10 panel driving
- HUB12 timing and row scanning
- Custom display rendering
- Text positioning and scaling
- Dual-chain/top-bottom panel control
- ESP32 FreeRTOS task separation
- Local web interface
- HTTPS web server
- Text-to-text display control
- Speech-related interface experiments
- TTS/STT integration experiments

## Interface

The system was designed around an ESP32-hosted local web interface so a phone or computer connected to the board can send display data through the network.

## Development

Arduino IDE 1.8.19 and ESP32 Dev Module were used during development.

Hardware-specific firmware is being organised separately from the web interface and supporting components.
