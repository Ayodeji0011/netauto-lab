# Decentralized Multi-Hop Emergency Communication System

**Design and Implementation of a Decentralized Multi-Hop Emergency Communication System for Smart Campus Safety Using ESP-NOW Protocol**

FUTMinna — Telecommunication Engineering

Supervisor: Engr. Umar Abdullahi

## Overview
ESP32-based decentralized emergency communication using ESP-NOW, GPS, OLED, keypad, RTC and relay nodes.

## Architecture
Emergency Node -> ESP-NOW -> Relay Node(s) -> Base Receiver -> OLED / LEDs / Buzzer

## Technologies
- ESP32
- ESP-NOW
- NEO-6M GPS
- DS3231 RTC
- OLED
- Matrix keypad
- Multi-hop wireless communication
- Embedded C/C++
- Arduino IDE

## Firmware
See `firmware/esp-now/nodes/` for sender, relay and receiver implementations.

## Security Note
The academic design discusses CCMP, while some prototype firmware uses XOR masking for demonstration. The prototype should not be described as production-grade cryptography or as a completed CCMP implementation.
