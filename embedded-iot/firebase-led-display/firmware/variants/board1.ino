

#include "Arduino.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>

//----------------------------------------Embedded fonts (no external files needed)
#ifndef SYSTEM5x7_H
#define SYSTEM5x7_H
#define SYSTEM5x7_WIDTH 5
#define SYSTEM5x7_HEIGHT 7
#define SystemFont5x7 System5x7
static const uint8_t System5x7[] PROGMEM = {
0x0, 0x0, 0x05, 0x07, 0x20, 0x60,
0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x5F, 0x00, 0x00,
0x00, 0x07, 0x00, 0x07, 0x00,
0x14, 0x7F, 0x14, 0x7F, 0x14,
0x24, 0x2A, 0x7F, 0x2A, 0x12,
0x23, 0x13, 0x08, 0x64, 0x62,
0x36, 0x49, 0x55, 0x22, 0x50,
0x00, 0x05, 0x03, 0x00, 0x00,
0x00, 0x1C, 0x22, 0x41, 0x00,
0x00, 0x41, 0x22, 0x1C, 0x00,
0x08, 0x2A, 0x1C, 0x2A, 0x08,
0x08, 0x08, 0x3E, 0x08, 0x08,
0x00, 0x50, 0x30, 0x00, 0x00,
0x08, 0x08, 0x08, 0x08, 0x08,
0x00, 0x60, 0x60, 0x00, 0x00,
0x20, 0x10, 0x08, 0x04, 0x02,
0x3E, 0x51, 0x49, 0x45, 0x3E,
0x00, 0x42, 0x7F, 0x40, 0x00,
0x42, 0x61, 0x51, 0x49, 0x46,
0x21, 0x41, 0x45, 0x4B, 0x31,
0x18, 0x14, 0x12, 0x7F, 0x10,
0x27, 0x45, 0x45, 0x45, 0x39,
0x3C, 0x4A, 0x49, 0x49, 0x30,
0x01, 0x71, 0x09, 0x05, 0x03,
0x36, 0x49, 0x49, 0x49, 0x36,
0x06, 0x49, 0x49, 0x29, 0x1E,
0x00, 0x36, 0x36, 0x00, 0x00,
0x00, 0x56, 0x36, 0x00, 0x00,
0x00, 0x08, 0x14, 0x22, 0x41,
0x14, 0x14, 0x14, 0x14, 0x14,
0x41, 0x22, 0x14, 0x08, 0x00,
0x02, 0x01, 0x51, 0x09, 0x06,
0x32, 0x49, 0x79, 0x41, 0x3E,
0x7E, 0x11, 0x11, 0x11, 0x7E,
0x7F, 0x49, 0x49, 0x49, 0x36,
0x3E, 0x41, 0x41, 0x41, 0x22,
0x7F, 0x41, 0x41, 0x22, 0x1C,
0x7F, 0x49, 0x49, 0x49, 0x41,
0x7F, 0x09, 0x09, 0x01, 0x01,
0x3E, 0x41, 0x41, 0x51, 0x32,
0x7F, 0x08, 0x08, 0x08, 0x7F,
0x00, 0x41, 0x7F, 0x41, 0x00,
0x20, 0x40, 0x41, 0x3F, 0x01,
0x7F, 0x08, 0x14, 0x22, 0x41,
0x7F, 0x40, 0x40, 0x40, 0x40,
0x7F, 0x02, 0x04, 0x02, 0x7F,
0x7F, 0x04, 0x08, 0x10, 0x7F,
0x3E, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x09, 0x09, 0x09, 0x06,
0x3E, 0x41, 0x51, 0x21, 0x5E,
0x7F, 0x09, 0x19, 0x29, 0x46,
0x46, 0x49, 0x49, 0x49, 0x31,
0x01, 0x01, 0x7F, 0x01, 0x01,
0x3F, 0x40, 0x40, 0x40, 0x3F,
0x1F, 0x20, 0x40, 0x20, 0x1F,
0x7F, 0x20, 0x18, 0x20, 0x7F,
0x63, 0x14, 0x08, 0x14, 0x63,
0x03, 0x04, 0x78, 0x04, 0x03,
0x61, 0x51, 0x49, 0x45, 0x43,
0x00, 0x00, 0x7F, 0x41, 0x41,
0x02, 0x04, 0x08, 0x10, 0x20,
0x41, 0x41, 0x7F, 0x00, 0x00,
0x04, 0x02, 0x01, 0x02, 0x04,
0x40, 0x40, 0x40, 0x40, 0x40,
0x00, 0x01, 0x02, 0x04, 0x00,
0x20, 0x54, 0x54, 0x54, 0x78,
0x7F, 0x48, 0x44, 0x44, 0x38,
0x38, 0x44, 0x44, 0x44, 0x20,
0x38, 0x44, 0x44, 0x48, 0x7F,
0x38, 0x54, 0x54, 0x54, 0x18,
0x08, 0x7E, 0x09, 0x01, 0x02,
0x08, 0x14, 0x54, 0x54, 0x3C,
0x7F, 0x08, 0x04, 0x04, 0x78,
0x00, 0x44, 0x7D, 0x40, 0x00,
0x20, 0x40, 0x44, 0x3D, 0x00,
0x00, 0x7F, 0x10, 0x28, 0x44,
0x00, 0x41, 0x7F, 0x40, 0x00,
0x7C, 0x04, 0x18, 0x04, 0x78,
0x7C, 0x08, 0x04, 0x04, 0x78,
0x38, 0x44, 0x44, 0x44, 0x38,
0x7C, 0x14, 0x14, 0x14, 0x08,
0x08, 0x14, 0x14, 0x18, 0x7C,
0x7C, 0x08, 0x04, 0x04, 0x08,
0x48, 0x54, 0x54, 0x54, 0x20,
0x04, 0x3F, 0x44, 0x40, 0x20,
0x3C, 0x40, 0x40, 0x20, 0x7C,
0x1C, 0x20, 0x40, 0x20, 0x1C,
0x3C, 0x40, 0x30, 0x40, 0x3C,
0x44, 0x28, 0x10, 0x28, 0x44,
0x0C, 0x50, 0x50, 0x50, 0x3C,
0x44, 0x64, 0x54, 0x4C, 0x44,
0x00, 0x08, 0x36, 0x41, 0x00,
0x00, 0x00, 0x7F, 0x00, 0x00,
0x00, 0x41, 0x36, 0x08, 0x00,
0x08, 0x08, 0x2A, 0x1C, 0x08,
0x08, 0x1C, 0x2A, 0x08, 0x08
};
#endif

#ifndef ARIAL_BLACK_16_H
#define ARIAL_BLACK_16_H
#define ARIAL_BLACK_16_WIDTH 10
#define ARIAL_BLACK_16_HEIGHT 16
const static uint8_t Arial_Black_16[] PROGMEM = {
0x30, 0x86, 0x0A, 0x10, 0x20, 0x60,
0x00, 0x03, 0x07, 0x0B, 0x09, 0x0E, 0x0B, 0x03, 0x05, 0x05,
0x06, 0x09, 0x03, 0x05, 0x03, 0x04, 0x08, 0x06, 0x08, 0x08,
0x09, 0x08, 0x08, 0x08, 0x08, 0x08, 0x03, 0x03, 0x09, 0x08,
0x09, 0x08, 0x0C, 0x0C, 0x09, 0x09, 0x09, 0x09, 0x08, 0x0A,
0x0A, 0x03, 0x09, 0x0C, 0x08, 0x0C, 0x0A, 0x0A, 0x09, 0x0A,
0x0A, 0x09, 0x0B, 0x0A, 0x0C, 0x10, 0x0C, 0x0B, 0x09, 0x05,
0x04, 0x05, 0x08, 0x08, 0x03, 0x09, 0x09, 0x09, 0x09, 0x09,
0x06, 0x09, 0x09, 0x03, 0x04, 0x0A, 0x03, 0x0D, 0x09, 0x09,
0x09, 0x09, 0x06, 0x08, 0x06, 0x09, 0x09, 0x0F, 0x0B, 0x09,
0x07, 0x06, 0x02, 0x06, 0x09, 0x08,
0xFE, 0xFE, 0xFE, 0x1D, 0x1D, 0x1D,
0x1E, 0x1E, 0x1E, 0x00, 0x1E, 0x1E, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x30, 0x30, 0xF0, 0xFE, 0x3E, 0x30, 0x30, 0xF0, 0xFE, 0x3E, 0x30, 0x06, 0x1E, 0x1F, 0x07, 0x06, 0x06, 0x1E, 0x1F, 0x07, 0x06, 0x06,
0x38, 0x7C, 0xFE, 0xE6, 0xFF, 0xC6, 0xCE, 0x8C, 0x0C, 0x04, 0x0C, 0x1C, 0x18, 0x3F, 0x19, 0x1F, 0x0F, 0x07,
0x3C, 0x7E, 0x42, 0x42, 0x7E, 0x3C, 0x80, 0x60, 0x10, 0x8C, 0x82, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0C, 0x02, 0x01, 0x00, 0x0F, 0x1F, 0x10, 0x10, 0x1F, 0x0F,
0x00, 0x80, 0x9C, 0xFE, 0xFE, 0xE6, 0xBE, 0x3E, 0x9C, 0x80, 0x80, 0x07, 0x0F, 0x1F, 0x19, 0x18, 0x19, 0x1F, 0x0F, 0x0F, 0x1F, 0x1D,
0x1E, 0x1E, 0x1E, 0x00, 0x00, 0x00,
0xE0, 0xF8, 0xFC, 0x1E, 0x02, 0x0F, 0x3F, 0x7F, 0xF0, 0x80,
0x02, 0x1E, 0xFC, 0xF8, 0xE0, 0x80, 0xF0, 0x7F, 0x3F, 0x0F,
0x08, 0x68, 0x3E, 0x3E, 0x68, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xC0, 0xC0, 0xC0, 0xF8, 0xF8, 0xF8, 0xC0, 0xC0, 0xC0, 0x01, 0x01, 0x01, 0x0F, 0x0F, 0x0F, 0x01, 0x01, 0x01,
0x00, 0x00, 0x00, 0xDC, 0x7C, 0x3C,
0x80, 0x80, 0x80, 0x80, 0x80, 0x03, 0x03, 0x03, 0x03, 0x03,
0x00, 0x00, 0x00, 0x1C, 0x1C, 0x1C,
0x00, 0x80, 0x78, 0x06, 0x18, 0x07, 0x00, 0x00,
0xF8, 0xFC, 0xFE, 0x06, 0x06, 0xFE, 0xFC, 0xF8, 0x07, 0x0F, 0x1F, 0x18, 0x18, 0x1F, 0x0F, 0x07,
0x60, 0x70, 0x38, 0xFE, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F,
0x18, 0x1C, 0x1E, 0x06, 0x86, 0xFE, 0xFC, 0x78, 0x18, 0x1C, 0x1E, 0x1F, 0x1B, 0x19, 0x18, 0x18,
0x08, 0x1C, 0x1E, 0xC6, 0xC6, 0xFE, 0xFC, 0x38, 0x06, 0x0E, 0x1E, 0x18, 0x18, 0x1F, 0x0F, 0x07,
0x80, 0xC0, 0xF0, 0x38, 0x1C, 0xFE, 0xFE, 0xFE, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x1F, 0x1F, 0x1F, 0x03,
0xF0, 0xFE, 0xFE, 0x66, 0x66, 0xE6, 0xC6, 0x86, 0x06, 0x0E, 0x1E, 0x18, 0x18, 0x1F, 0x0F, 0x07,
0xF0, 0xFC, 0xFE, 0x46, 0x66, 0xEE, 0xCE, 0x8C, 0x03, 0x0F, 0x1F, 0x18, 0x18, 0x1F, 0x0F, 0x07,
0x06, 0x06, 0x06, 0x86, 0xE6, 0xF6, 0x1E, 0x06, 0x00, 0x00, 0x1C, 0x1F, 0x1F, 0x01, 0x00, 0x00,
0x38, 0xFC, 0xFE, 0xC6, 0xC6, 0xFE, 0xFC, 0x38, 0x07, 0x0F, 0x1F, 0x18, 0x18, 0x1F, 0x0F, 0x07,
0x78, 0xFC, 0xFE, 0x86, 0x86, 0xFE, 0xFC, 0xF0, 0x04, 0x0C, 0x1D, 0x19, 0x18, 0x1F, 0x0F, 0x03,
0x70, 0x70, 0x70, 0x1C, 0x1C, 0x1C,
0x70, 0x70, 0x70, 0xDC, 0x7C, 0x3C,
0xE0, 0xE0, 0xE0, 0xF0, 0x70, 0x70, 0x70, 0x38, 0x38, 0x03, 0x03, 0x03, 0x07, 0x07, 0x07, 0x07, 0x0E, 0x0E,
0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
0x38, 0x38, 0x70, 0x70, 0x70, 0xF0, 0xE0, 0xE0, 0xE0, 0x0E, 0x0E, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03,
0x18, 0x1C, 0x9E, 0xC6, 0xE6, 0xFE, 0x7C, 0x38, 0x00, 0x00, 0x1D, 0x1D, 0x1D, 0x00, 0x00, 0x00,
0xE0, 0x18, 0xC4, 0xF4, 0x3A, 0x0A, 0x0A, 0xF2, 0xFA, 0x7C, 0x08, 0xF0, 0x07, 0x18, 0x27, 0x2F, 0x48, 0x48, 0x4C, 0x4F, 0x4F, 0x28, 0x36, 0x11,
0x00, 0x80, 0xE0, 0xF8, 0xFE, 0x1E, 0xFE, 0xF8, 0xE0, 0x80, 0x00, 0x00, 0x1C, 0x1F, 0x0F, 0x07, 0x06, 0x06, 0x06, 0x07, 0x0F, 0x1F, 0x1C, 0x10,
0xFE, 0xFE, 0xFE, 0xC6, 0xC6, 0xFE, 0xBC, 0x98, 0x00, 0x1F, 0x1F, 0x1F, 0x18, 0x18, 0x18, 0x1F, 0x0F, 0x07,
0xF0, 0xFC, 0xFC, 0x0E, 0x06, 0x0E, 0x1E, 0x1C, 0x08, 0x03, 0x0F, 0x1F, 0x1C, 0x18, 0x1C, 0x1F, 0x0E, 0x06,
0xFE, 0xFE, 0xFE, 0x06, 0x06, 0x0E, 0xFE, 0xFC, 0xF0, 0x1F, 0x1F, 0x1F, 0x18, 0x18, 0x1C, 0x1F, 0x0F, 0x03,
0xFE, 0xFE, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x06, 0x1F, 0x1F, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
0xFE, 0xFE, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0x06, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00,
0xF0, 0xFC, 0xFC, 0x0E, 0x06, 0xC6, 0xCE, 0xDE, 0xDC, 0xC8, 0x03, 0x0F, 0x0F, 0x1C, 0x18, 0x18, 0x1C, 0x1F, 0x0F, 0x0F,
0xFE, 0xFE, 0xFE, 0xC0, 0xC0, 0xC0, 0xC0, 0xFE, 0xFE, 0xFE, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F,
0xFE, 0xFE, 0xFE, 0x1F, 0x1F, 0x1F,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFE, 0x06, 0x0F, 0x1F, 0x1C, 0x18, 0x18, 0x1F, 0x0F, 0x07,
0xFE, 0xFE, 0xFE, 0xC0, 0xE0, 0xF0, 0xF8, 0xDC, 0x0E, 0x06, 0x02, 0x00, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1F, 0x1E, 0x18, 0x10,
0xFE, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18,
0xFE, 0xFE, 0xFE, 0x3E, 0xF8, 0x80, 0x80, 0xF8, 0x3E, 0xFE, 0xFE, 0xFE, 0x1F, 0x1F, 0x1F, 0x00, 0x03, 0x1F, 0x1F, 0x03, 0x00, 0x1F, 0x1F, 0x1F,
0xFE, 0xFE, 0xFE, 0x7C, 0xF0, 0xE0, 0x80, 0xFE, 0xFE, 0xFE, 0x1F, 0x1F, 0x1F, 0x00, 0x01, 0x03, 0x0F, 0x1F, 0x1F, 0x1F,
0xF0, 0xFC, 0xFC, 0x0E, 0x06, 0x06, 0x0E, 0xFC, 0xFC, 0xF0, 0x03, 0x0F, 0x0F, 0x1C, 0x18, 0x18, 0x1C, 0x0F, 0x0F, 0x03,
0xFE, 0xFE, 0xFE, 0xC6, 0xC6, 0xC6, 0xFE, 0x7E, 0x3C, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xF0, 0xFC, 0xFC, 0x0E, 0x06, 0x06, 0x0E, 0xFC, 0xFC, 0xF0, 0x03, 0x0F, 0x0F, 0x1C, 0x18, 0x1E, 0x1C, 0x1F, 0x1F, 0x33,
0xFE, 0xFE, 0xFE, 0xC6, 0xC6, 0xC6, 0xFE, 0x7E, 0x3C, 0x00, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x03, 0x0F, 0x1F, 0x1C, 0x10,
0x38, 0x7C, 0xFE, 0xE6, 0xE6, 0xEE, 0xDE, 0xDC, 0x98, 0x06, 0x0E, 0x1E, 0x1C, 0x18, 0x19, 0x1F, 0x0F, 0x07,
0x06, 0x06, 0x06, 0x06, 0xFE, 0xFE, 0xFE, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00,
0xFE, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFE, 0x07, 0x0F, 0x1F, 0x1C, 0x18, 0x18, 0x1C, 0x1F, 0x0F, 0x07,
0x1E, 0xFE, 0xFC, 0xF0, 0x80, 0x00, 0x80, 0xF0, 0xFC, 0xFE, 0x1E, 0x02, 0x00, 0x00, 0x03, 0x1F, 0x1F, 0x1C, 0x1F, 0x1F, 0x03, 0x00, 0x00, 0x00,
0xFE, 0xFE, 0xF8, 0x00, 0x80, 0xF8, 0xFE, 0x3E, 0xFE, 0xF8, 0x80, 0x00, 0xF8, 0xFE, 0xFE, 0x06, 0x00, 0x0F, 0x1F, 0x1F, 0x1F, 0x07, 0x01, 0x00, 0x01, 0x07, 0x1F, 0x1F, 0x1F, 0x0F, 0x00, 0x00,
0x06, 0x1E, 0x3C, 0xF8, 0xF0, 0xE0, 0xF0, 0xF8, 0x3C, 0x1E, 0x06, 0x02, 0x18, 0x1E, 0x0F, 0x07, 0x03, 0x01, 0x03, 0x07, 0x0F, 0x1E, 0x18, 0x10,
0x02, 0x0E, 0x1E, 0x7E, 0xF8, 0xE0, 0xF8, 0x7E, 0x1E, 0x0E, 0x02, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00,
0x00, 0x06, 0x06, 0xC6, 0xE6, 0xF6, 0x3E, 0x1E, 0x0E, 0x1C, 0x1E, 0x1F, 0x1B, 0x19, 0x18, 0x18, 0x18, 0x18,
0xFE, 0xFE, 0xFE, 0x06, 0x06, 0xFF, 0xFF, 0xFF, 0xC0, 0xC0,
0x06, 0x78, 0x80, 0x00, 0x00, 0x00, 0x07, 0x18,
0x06, 0x06, 0xFE, 0xFE, 0xFE, 0xC0, 0xC0, 0xFF, 0xFF, 0xFF,
0x40, 0x70, 0x7C, 0x1E, 0x1E, 0x7C, 0x70, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
0x02, 0x06, 0x04, 0x00, 0x00, 0x00,
0x40, 0x60, 0x70, 0x30, 0xB0, 0xB0, 0xF0, 0xF0, 0xE0, 0x0E, 0x1F, 0x1F, 0x1B, 0x19, 0x09, 0x1F, 0x1F, 0x1F,
0xFE, 0xFE, 0xFE, 0x60, 0x30, 0x30, 0xF0, 0xE0, 0xC0, 0x1F, 0x1F, 0x1F, 0x0C, 0x18, 0x18, 0x1F, 0x0F, 0x07,
0xC0, 0xE0, 0xF0, 0x70, 0x30, 0x30, 0x70, 0x60, 0x40, 0x07, 0x0F, 0x1F, 0x1C, 0x18, 0x18, 0x1C, 0x0C, 0x04,
0xC0, 0xE0, 0xF0, 0x30, 0x30, 0x60, 0xFE, 0xFE, 0xFE, 0x07, 0x0F, 0x1F, 0x18, 0x18, 0x0C, 0x1F, 0x1F, 0x1F,
0xC0, 0xE0, 0xF0, 0xB0, 0xB0, 0xB0, 0xF0, 0xE0, 0xC0, 0x07, 0x0F, 0x1F, 0x1D, 0x19, 0x19, 0x1D, 0x0D, 0x05,
0x30, 0xFC, 0xFE, 0xFE, 0x36, 0x36, 0x00, 0x1F, 0x1F, 0x1F, 0x00, 0x00,
0xC0, 0xE0, 0xF0, 0x30, 0x30, 0x60, 0xF0, 0xF0, 0xF0, 0x47, 0xCF, 0xDF, 0xD8, 0xD8, 0xCC, 0xFF, 0x7F, 0x3F,
0xFE, 0xFE, 0xFE, 0x20, 0x30, 0x30, 0xF0, 0xF0, 0xE0, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F,
0xF6, 0xF6, 0xF6, 0x1F, 0x1F, 0x1F,
0x00, 0xF6, 0xF6, 0xF6, 0xC0, 0xFF, 0xFF, 0x7F,
0xFE, 0xFE, 0xFE, 0xC0, 0xE0, 0xF0, 0xF0, 0x30, 0x10, 0x00, 0x1F, 0x1F, 0x1F, 0x03, 0x01, 0x07, 0x1F, 0x1E, 0x1C, 0x10,
0xFE, 0xFE, 0xFE, 0x1F, 0x1F, 0x1F,
0xF0, 0xF0, 0xF0, 0x20, 0x30, 0xF0, 0xF0, 0xE0, 0x20, 0x30, 0xF0, 0xF0, 0xE0, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x1F, 0x1F, 0x1F,
0xF0, 0xF0, 0xF0, 0x20, 0x30, 0x30, 0xF0, 0xF0, 0xE0, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F,
0xC0, 0xE0, 0xF0, 0x70, 0x30, 0x70, 0xF0, 0xE0, 0xC0, 0x07, 0x0F, 0x1F, 0x1C, 0x18, 0x1C, 0x1F, 0x0F, 0x07,
0xF0, 0xF0, 0xF0, 0x60, 0x30, 0x70, 0xF0, 0xE0, 0xC0, 0xFF, 0xFF, 0xFF, 0x0C, 0x18, 0x18, 0x1F, 0x0F, 0x07,
0xC0, 0xE0, 0xF0, 0x30, 0x30, 0x60, 0xF0, 0xF0, 0xF0, 0x07, 0x0F, 0x1F, 0x18, 0x18, 0x0C, 0xFF, 0xFF, 0xFF,
0xF0, 0xF0, 0xF0, 0x20, 0x30, 0x10, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00,
0xE0, 0xF0, 0xF0, 0x90, 0x90, 0xB0, 0x30, 0x20, 0x08, 0x19, 0x1B, 0x13, 0x13, 0x1F, 0x1F, 0x0E,
0x30, 0xFC, 0xFC, 0xFE, 0x30, 0x30, 0x00, 0x0F, 0x1F, 0x1F, 0x18, 0x18,
0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0x0F, 0x1F, 0x1F, 0x18, 0x18, 0x08, 0x1F, 0x1F, 0x1F,
0x10, 0xF0, 0xF0, 0xC0, 0x00, 0xE0, 0xF0, 0xF0, 0x10, 0x00, 0x00, 0x07, 0x1F, 0x1C, 0x1F, 0x07, 0x00, 0x00,
0x10, 0xF0, 0xF0, 0xE0, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0xE0, 0xF0, 0xF0, 0x10, 0x00, 0x00, 0x07, 0x1F, 0x1E, 0x0F, 0x03, 0x00, 0x03, 0x0F, 0x1E, 0x1F, 0x07, 0x00, 0x00,
0x10, 0x30, 0x70, 0xE0, 0xC0, 0x80, 0xC0, 0xE0, 0x70, 0x30, 0x10, 0x10, 0x18, 0x1E, 0x0F, 0x07, 0x03, 0x07, 0x0F, 0x1E, 0x18, 0x10,
0x10, 0xF0, 0xF0, 0xC0, 0x00, 0xC0, 0xF0, 0xF0, 0x30, 0xC0, 0xC0, 0xC7, 0xFF, 0xFC, 0x3F, 0x0F, 0x01, 0x00,
0x30, 0x30, 0x30, 0xF0, 0xF0, 0xF0, 0x30, 0x1C, 0x1E, 0x1F, 0x1B, 0x19, 0x18, 0x18,
0x00, 0x00, 0xFC, 0xFE, 0xFE, 0x06, 0x03, 0x03, 0x7F, 0xFF, 0xFC, 0xC0,
0xFE, 0xFE, 0xFF, 0xFF,
0x06, 0xFE, 0xFE, 0xFC, 0x00, 0x00, 0xC0, 0xFC, 0xFF, 0x7F, 0x03, 0x03,
0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0xC0, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,
0xF8, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xF8, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F
};
#endif

//----------------------------------------Pin definitions (shared control lines + two DATA pins)
#define PIN_DMD_nOE     22
#define PIN_DMD_A       19
#define PIN_DMD_B       21
#define PIN_DMD_CLK     18
#define PIN_DMD_SCLK     2
#define PIN_DMD_R_DATA  23   // bottom chain DATA
#define PIN_DMD_R_DATA2 26   // top chain DATA

// Direct GPIO register access. Every pin above is < 32, so the low registers are correct.
// REG_WRITE / GPIO_OUT_W1TS_REG are stable across ESP32 Arduino core 2.x and 3.x.
#define DMD_HI(pin)  REG_WRITE(GPIO_OUT_W1TS_REG, (1UL << (pin)))
#define DMD_LO(pin)  REG_WRITE(GPIO_OUT_W1TC_REG, (1UL << (pin)))
#define DMD_SET(pin, v) do { if (v) DMD_HI(pin); else DMD_LO(pin); } while (0)
#define DMD_NOP()    __asm__ __volatile__("nop; nop")

#define LIGHT_DMD_ROW_01_05_09_13() { DMD_LO(PIN_DMD_B); DMD_LO(PIN_DMD_A); }
#define LIGHT_DMD_ROW_02_06_10_14() { DMD_LO(PIN_DMD_B); DMD_HI(PIN_DMD_A); }
#define LIGHT_DMD_ROW_03_07_11_15() { DMD_HI(PIN_DMD_B); DMD_LO(PIN_DMD_A); }
#define LIGHT_DMD_ROW_04_08_12_16() { DMD_HI(PIN_DMD_B); DMD_HI(PIN_DMD_A); }
#define LATCH_DMD_SHIFT_REG_TO_OUTPUT() { DMD_HI(PIN_DMD_SCLK); DMD_NOP(); DMD_LO(PIN_DMD_SCLK); }
#define OE_DMD_ROWS_OFF() { DMD_LO(PIN_DMD_nOE); }
#define OE_DMD_ROWS_ON()  { DMD_HI(PIN_DMD_nOE); }

#define GRAPHICS_NORMAL  0
#define GRAPHICS_INVERSE 1
#define GRAPHICS_TOGGLE  2
#define GRAPHICS_OR      3
#define GRAPHICS_NOR     4

#define PATTERN_ALT_0    0
#define PATTERN_ALT_1    1
#define PATTERN_STRIPE_0 2
#define PATTERN_STRIPE_1 3

#define DMD_PIXELS_ACROSS 32
#define DMD_PIXELS_DOWN   16
#define DMD_BITSPERPIXEL   1
#define DMD_RAM_SIZE_BYTES ((DMD_PIXELS_ACROSS*DMD_BITSPERPIXEL/8)*DMD_PIXELS_DOWN)

static byte bPixelLookupTable[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

#define FONT_LENGTH 0
#define FONT_FIXED_WIDTH 2
#define FONT_HEIGHT 3
#define FONT_FIRST_CHAR 4
#define FONT_CHAR_COUNT 5
#define FONT_WIDTH_TABLE 6

//----------------------------------------DMD class (full implementation, inline in this file)
class DMD
{
public:
        DMD(byte panelsWide, byte panelsHigh)
        {
                DisplaysWide  = panelsWide;
                DisplaysHigh  = panelsHigh;
                DisplaysTotal = DisplaysWide * DisplaysHigh;
                row1 = DisplaysTotal << 4;
                row2 = DisplaysTotal << 5;
                row3 = ((DisplaysTotal << 2) * 3) << 2;

                bDMDScreenRAM = (byte *) malloc(DisplaysTotal * DMD_RAM_SIZE_BYTES);
                if (bDMDScreenRAM) clearScreen(true);

                Font = NULL;
                bDMDByte = 0;
        }

        bool ramOk() { return bDMDScreenRAM != NULL; }

        // Shared control pins are set up once, not per instance.
        static void initPins()
        {
                pinMode(PIN_DMD_A,       OUTPUT);
                pinMode(PIN_DMD_B,       OUTPUT);
                pinMode(PIN_DMD_CLK,     OUTPUT);
                pinMode(PIN_DMD_SCLK,    OUTPUT);
                pinMode(PIN_DMD_nOE,     OUTPUT);
                pinMode(PIN_DMD_R_DATA,  OUTPUT);
                pinMode(PIN_DMD_R_DATA2, OUTPUT);

                digitalWrite(PIN_DMD_A,       LOW);
                digitalWrite(PIN_DMD_B,       LOW);
                digitalWrite(PIN_DMD_CLK,     LOW);
                digitalWrite(PIN_DMD_SCLK,    LOW);
                digitalWrite(PIN_DMD_nOE,     LOW);
                digitalWrite(PIN_DMD_R_DATA,  HIGH);
                digitalWrite(PIN_DMD_R_DATA2, HIGH);
        }

        void writePixel(unsigned int bX, unsigned int bY, byte bGraphicsMode, byte bPixel)
        {
                unsigned int uiDMDRAMPointer;
                if (bX >= (unsigned int)(DMD_PIXELS_ACROSS*DisplaysWide) ||
                    bY >= (unsigned int)(DMD_PIXELS_DOWN * DisplaysHigh)) return;

                byte panel=(bX/DMD_PIXELS_ACROSS) + (DisplaysWide*(bY/DMD_PIXELS_DOWN));
                bX=(bX % DMD_PIXELS_ACROSS) + (panel<<5);
                bY=bY % DMD_PIXELS_DOWN;
                uiDMDRAMPointer = bX/8 + bY*(DisplaysTotal<<2);
                byte lookup = bPixelLookupTable[bX & 0x07];

                switch (bGraphicsMode) {
                case GRAPHICS_NORMAL:
                        if (bPixel == true) bDMDScreenRAM[uiDMDRAMPointer] &= ~lookup;
                        else bDMDScreenRAM[uiDMDRAMPointer] |= lookup;
                        break;
                case GRAPHICS_INVERSE:
                        if (bPixel == false) bDMDScreenRAM[uiDMDRAMPointer] &= ~lookup;
                        else bDMDScreenRAM[uiDMDRAMPointer] |= lookup;
                        break;
                case GRAPHICS_TOGGLE:
                        if (bPixel == true) {
                                if ((bDMDScreenRAM[uiDMDRAMPointer] & lookup) == 0) bDMDScreenRAM[uiDMDRAMPointer] |= lookup;
                                else bDMDScreenRAM[uiDMDRAMPointer] &= ~lookup;
                        }
                        break;
                case GRAPHICS_OR:
                        if (bPixel == true) bDMDScreenRAM[uiDMDRAMPointer] &= ~lookup;
                        break;
                case GRAPHICS_NOR:
                        if ((bPixel == true) && ((bDMDScreenRAM[uiDMDRAMPointer] & lookup) == 0)) bDMDScreenRAM[uiDMDRAMPointer] |= lookup;
                        break;
                }
        }

        void drawString(int bX, int bY, const char *bChars, byte length, byte bGraphicsMode)
        {
                if (Font == NULL) return;
                if (bX >= (DMD_PIXELS_ACROSS*DisplaysWide) || bY >= DMD_PIXELS_DOWN * DisplaysHigh) return;
                uint8_t height = pgm_read_byte(this->Font + FONT_HEIGHT);
                if (bY+height<0) return;

                int strWidth = 0;
                this->drawLine(bX -1 , bY, bX -1 , bY + height, GRAPHICS_INVERSE);
                for (int i = 0; i < length; i++) {
                        int charWide = this->drawChar(bX+strWidth, bY, bChars[i], bGraphicsMode);
                        if (charWide > 0) {
                                strWidth += charWide ;
                                this->drawLine(bX + strWidth , bY, bX + strWidth , bY + height, GRAPHICS_INVERSE);
                                strWidth++;
                        } else if (charWide < 0) {
                                return;
                        }
                        if ((bX + strWidth) >= DMD_PIXELS_ACROSS * DisplaysWide || bY >= DMD_PIXELS_DOWN * DisplaysHigh) return;
                }
        }

        void clearScreen(byte bNormal)
        {
                if (!bDMDScreenRAM) return;
                if (bNormal) memset(bDMDScreenRAM,0xFF,DMD_RAM_SIZE_BYTES*DisplaysTotal);
                else memset(bDMDScreenRAM,0x00,DMD_RAM_SIZE_BYTES*DisplaysTotal);
        }

        void drawLine(int x1, int y1, int x2, int y2, byte bGraphicsMode)
        {
                int dy = y2 - y1;
                int dx = x2 - x1;
                int stepx, stepy;
                if (dy < 0) { dy = -dy; stepy = -1; } else { stepy = 1; }
                if (dx < 0) { dx = -dx; stepx = -1; } else { stepx = 1; }
                dy <<= 1;
                dx <<= 1;
                writePixel(x1, y1, bGraphicsMode, true);
                if (dx > dy) {
                        int fraction = dy - (dx >> 1);
                        while (x1 != x2) {
                                if (fraction >= 0) { y1 += stepy; fraction -= dx; }
                                x1 += stepx;
                                fraction += dy;
                                writePixel(x1, y1, bGraphicsMode, true);
                        }
                } else {
                        int fraction = dx - (dy >> 1);
                        while (y1 != y2) {
                                if (fraction >= 0) { x1 += stepx; fraction -= dy; }
                                y1 += stepy;
                                fraction += dx;
                                writePixel(x1, y1, bGraphicsMode, true);
                        }
                }
        }

        void drawCircle(int xCenter, int yCenter, int radius, byte bGraphicsMode)
        {
                int x = 0;
                int y = radius;
                int p = (5 - radius * 4) / 4;
                drawCircleSub(xCenter, yCenter, x, y, bGraphicsMode);
                while (x < y) {
                        x++;
                        if (p < 0) { p += 2 * x + 1; }
                        else { y--; p += 2 * (x - y) + 1; }
                        drawCircleSub(xCenter, yCenter, x, y, bGraphicsMode);
                }
        }

        void drawBox(int x1, int y1, int x2, int y2, byte bGraphicsMode)
        {
                drawLine(x1, y1, x2, y1, bGraphicsMode);
                drawLine(x2, y1, x2, y2, bGraphicsMode);
                drawLine(x2, y2, x1, y2, bGraphicsMode);
                drawLine(x1, y2, x1, y1, bGraphicsMode);
        }

        void drawFilledBox(int x1, int y1, int x2, int y2, byte bGraphicsMode)
        {
                for (int b = x1; b <= x2; b++) drawLine(b, y1, b, y2, bGraphicsMode);
        }

        void drawTestPattern(byte bPattern)
        {
                unsigned int ui;
                int numPixels=DisplaysTotal * DMD_PIXELS_ACROSS * DMD_PIXELS_DOWN;
                int pixelsWide=DMD_PIXELS_ACROSS*DisplaysWide;
                for (ui = 0; ui < (unsigned int)numPixels; ui++) {
                        switch (bPattern) {
                        case PATTERN_ALT_0:
                                if ((ui & pixelsWide) == 0) writePixel((ui & (pixelsWide-1)), ((ui & ~(pixelsWide-1)) / pixelsWide), GRAPHICS_NORMAL, ui & 1);
                                else writePixel((ui & (pixelsWide-1)), ((ui & ~(pixelsWide-1)) / pixelsWide), GRAPHICS_NORMAL, !(ui & 1));
                                break;
                        case PATTERN_ALT_1:
                                if ((ui & pixelsWide) == 0) writePixel((ui & (pixelsWide-1)), ((ui & ~(pixelsWide-1)) / pixelsWide), GRAPHICS_NORMAL, !(ui & 1));
                                else writePixel((ui & (pixelsWide-1)), ((ui & ~(pixelsWide-1)) / pixelsWide), GRAPHICS_NORMAL, ui & 1);
                                break;
                        case PATTERN_STRIPE_0:
                                writePixel((ui & (pixelsWide-1)), ((ui & ~(pixelsWide-1)) / pixelsWide), GRAPHICS_NORMAL, ui & 1);
                                break;
                        case PATTERN_STRIPE_1:
                                writePixel((ui & (pixelsWide-1)), ((ui & ~(pixelsWide-1)) / pixelsWide), GRAPHICS_NORMAL, !(ui & 1));
                                break;
                        }
                }
        }

        //--------------------------------------------------------------------------
        // Clocks one row-group out to BOTH chains simultaneously.
        // The old digitalRead(SS) guard was removed - SS floats on this board and a
        // LOW reading silently killed the entire refresh.
        //--------------------------------------------------------------------------
        void scanDualBitBang(DMD &otherChain, byte dataPinThis, byte dataPinOther)
        {
                if (!bDMDScreenRAM || !otherChain.bDMDScreenRAM) return;

                int rowsizeA = DisplaysTotal << 2;
                int rowsizeB = otherChain.DisplaysTotal << 2;
                int rowsizeMax = (rowsizeA > rowsizeB) ? rowsizeA : rowsizeB;
                int padA = rowsizeMax - rowsizeA;
                int padB = rowsizeMax - rowsizeB;

                int offsetA = rowsizeA * bDMDByte;
                int offsetB = rowsizeB * bDMDByte;

                for (int i = 0; i < rowsizeMax; i++) {
                        byte bytesA[4], bytesB[4];

                        if (i >= padA) {
                                int j = i - padA;
                                bytesA[0] = bDMDScreenRAM[offsetA + j + row3];
                                bytesA[1] = bDMDScreenRAM[offsetA + j + row2];
                                bytesA[2] = bDMDScreenRAM[offsetA + j + row1];
                                bytesA[3] = bDMDScreenRAM[offsetA + j];
                        } else {
                                bytesA[0] = bytesA[1] = bytesA[2] = bytesA[3] = 0xFF;
                        }

                        if (i >= padB) {
                                int j = i - padB;
                                bytesB[0] = otherChain.bDMDScreenRAM[offsetB + j + otherChain.row3];
                                bytesB[1] = otherChain.bDMDScreenRAM[offsetB + j + otherChain.row2];
                                bytesB[2] = otherChain.bDMDScreenRAM[offsetB + j + otherChain.row1];
                                bytesB[3] = otherChain.bDMDScreenRAM[offsetB + j];
                        } else {
                                bytesB[0] = bytesB[1] = bytesB[2] = bytesB[3] = 0xFF;
                        }

                        for (int p = 0; p < 4; p++) {
                                byte byteA = bytesA[p];
                                byte byteB = bytesB[p];
                                for (int k = 7; k >= 0; k--) {
                                        DMD_SET(dataPinThis,  (byteA >> k) & 0x01);
                                        DMD_SET(dataPinOther, (byteB >> k) & 0x01);
                                        DMD_NOP();
                                        DMD_HI(PIN_DMD_CLK);
                                        DMD_NOP();
                                        DMD_LO(PIN_DMD_CLK);
                                }
                        }
                }

                OE_DMD_ROWS_OFF();
                LATCH_DMD_SHIFT_REG_TO_OUTPUT();

                switch (bDMDByte) {
                case 0: LIGHT_DMD_ROW_01_05_09_13(); bDMDByte=1; break;
                case 1: LIGHT_DMD_ROW_02_06_10_14(); bDMDByte=2; break;
                case 2: LIGHT_DMD_ROW_03_07_11_15(); bDMDByte=3; break;
                case 3: LIGHT_DMD_ROW_04_08_12_16(); bDMDByte=0; break;
                }
                otherChain.bDMDByte = bDMDByte;

                OE_DMD_ROWS_ON();
        }

        void selectFont(const uint8_t * font) { this->Font = font; }

        int drawChar(const int bX, const int bY, const unsigned char letter, byte bGraphicsMode)
        {
                if (Font == NULL) return -1;
                if (bX > (DMD_PIXELS_ACROSS*DisplaysWide) || bY > (DMD_PIXELS_DOWN*DisplaysHigh) ) return -1;
                unsigned char c = letter;
                uint8_t height = pgm_read_byte(this->Font + FONT_HEIGHT);

                if (c == ' ') {
                        int charWide = charWidth(' ');
                        this->drawFilledBox(bX, bY, bX + charWide, bY + height, GRAPHICS_INVERSE);
                        return charWide;
                }

                uint8_t width = 0;
                uint8_t bytes = (height + 7) / 8;
                uint8_t firstChar = pgm_read_byte(this->Font + FONT_FIRST_CHAR);
                uint8_t charCount = pgm_read_byte(this->Font + FONT_CHAR_COUNT);
                uint16_t index = 0;

                if (c < firstChar || c >= (firstChar + charCount)) return 0;
                c -= firstChar;

                if (pgm_read_byte(this->Font + FONT_LENGTH) == 0 && pgm_read_byte(this->Font + FONT_LENGTH + 1) == 0) {
                        width = pgm_read_byte(this->Font + FONT_FIXED_WIDTH);
                        index = c * bytes * width + FONT_WIDTH_TABLE;
                } else {
                        for (uint8_t i = 0; i < c; i++) index += pgm_read_byte(this->Font + FONT_WIDTH_TABLE + i);
                        index = index * bytes + charCount + FONT_WIDTH_TABLE;
                        width = pgm_read_byte(this->Font + FONT_WIDTH_TABLE + c);
                }

                if (bX < -width || bY < -height) return width;

                for (uint8_t j = 0; j < width; j++) {
                        for (uint8_t i = bytes - 1; i < 254; i--) {
                                uint8_t data = pgm_read_byte(this->Font + index + j + (i * width));
                                int offset = (i * 8);
                                if ((i == bytes - 1) && bytes > 1) offset = height - 8;
                                for (uint8_t k = 0; k < 8; k++) {
                                        if ((offset+k >= i*8) && (offset+k <= height)) {
                                                if (data & (1 << k)) writePixel(bX + j, bY + offset + k, bGraphicsMode, true);
                                                else writePixel(bX + j, bY + offset + k, bGraphicsMode, false);
                                        }
                                }
                        }
                }
                return width;
        }

        int charWidth(const unsigned char letter)
        {
                if (Font == NULL) return 0;
                unsigned char c = letter;
                if (c == ' ') c = 'n';
                uint8_t width = 0;
                uint8_t firstChar = pgm_read_byte(this->Font + FONT_FIRST_CHAR);
                uint8_t charCount = pgm_read_byte(this->Font + FONT_CHAR_COUNT);
                if (c < firstChar || c >= (firstChar + charCount)) return 0;
                c -= firstChar;
                if (pgm_read_byte(this->Font + FONT_LENGTH) == 0 && pgm_read_byte(this->Font + FONT_LENGTH + 1) == 0) {
                        width = pgm_read_byte(this->Font + FONT_FIXED_WIDTH);
                } else {
                        width = pgm_read_byte(this->Font + FONT_WIDTH_TABLE + c);
                }
                return width;
        }

private:
        void drawCircleSub(int cx, int cy, int x, int y, byte bGraphicsMode)
        {
                if (x == 0) {
                        writePixel(cx, cy + y, bGraphicsMode, true);
                        writePixel(cx, cy - y, bGraphicsMode, true);
                        writePixel(cx + y, cy, bGraphicsMode, true);
                        writePixel(cx - y, cy, bGraphicsMode, true);
                } else if (x == y) {
                        writePixel(cx + x, cy + y, bGraphicsMode, true);
                        writePixel(cx - x, cy + y, bGraphicsMode, true);
                        writePixel(cx + x, cy - y, bGraphicsMode, true);
                        writePixel(cx - x, cy - y, bGraphicsMode, true);
                } else if (x < y) {
                        writePixel(cx + x, cy + y, bGraphicsMode, true);
                        writePixel(cx - x, cy + y, bGraphicsMode, true);
                        writePixel(cx + x, cy - y, bGraphicsMode, true);
                        writePixel(cx - x, cy - y, bGraphicsMode, true);
                        writePixel(cx + y, cy + x, bGraphicsMode, true);
                        writePixel(cx - y, cy + x, bGraphicsMode, true);
                        writePixel(cx + y, cy - x, bGraphicsMode, true);
                        writePixel(cx - y, cy - x, bGraphicsMode, true);
                }
        }

        byte *bDMDScreenRAM;
        const uint8_t* Font;
        byte DisplaysWide;
        byte DisplaysHigh;
        byte DisplaysTotal;
        int row1, row2, row3;
        volatile byte bDMDByte;
};

//======================================================================================
// SKETCH
//======================================================================================

//--------------------------------------------------------------------------------------
// They were shared publicly.
//--------------------------------------------------------------------------------------
#define WIFI_SSID              "YOUR_WIFI_SSID"
#define WIFI_PASSWORD          "YOUR_WIFI_PASSWORD"

#define FIREBASE_API_KEY       "YOUR_FIREBASE_API_KEY"
#define FIREBASE_DATABASE_URL  "https://futmx-led-display-default-rtdb.firebaseio.com"

#define FIREBASE_USER_EMAIL    "eogar88@gmail.com"
#define FIREBASE_USER_PASSWORD "YOUR_FIREBASE_USER_PASSWORD"

#define DEVICE_PATH_LINE1      "/devices/led-001/command/line1"
#define DEVICE_PATH_LINE2      "/devices/led-001/command/line2"

//--------------------------------------------------------------------------------------
// PANEL LAYOUT
// GPIO 26 = PHYSICAL TOP ROW, GPIO 23 = PHYSICAL BOTTOM ROW
//--------------------------------------------------------------------------------------
#define DATA_PIN_TOP    26
#define DATA_PIN_BOTTOM 23

#define PANELS_WIDE     3
#define SCREEN_WIDTH    (PANELS_WIDE * DMD_PIXELS_ACROSS)   // 96

DMD dmd_top(PANELS_WIDE, 1);
DMD dmd_bottom(PANELS_WIDE, 1);

//--------------------------------------------------------------------------------------
// FIREBASE OBJECTS
//--------------------------------------------------------------------------------------
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

//--------------------------------------------------------------------------------------
// TEXT BUFFERS
//--------------------------------------------------------------------------------------
#define TEXT_LEN 128

char topText[TEXT_LEN]    = "SMART LED DISPLAY";
char bottomText[TEXT_LEN] = "WELCOME TO FUTMINNA";

char firebaseTopText[TEXT_LEN]    = "SMART LED DISPLAY";
char firebaseBottomText[TEXT_LEN] = "WELCOME TO FUTMINNA";

SemaphoreHandle_t textMutex    = NULL;   // guards the firebase* buffers
SemaphoreHandle_t displayMutex = NULL;   // guards the DMD frame buffers

volatile bool firebaseTextChanged = false;

//--------------------------------------------------------------------------------------
// SCROLL STATE
//--------------------------------------------------------------------------------------
int topX = SCREEN_WIDTH;
int bottomX = SCREEN_WIDTH;

int topWidth = 0;
int bottomWidth = 0;

unsigned long lastScroll = 0;
const unsigned long scrollDelay = 40;    // ms per pixel step

bool needsRedraw = true;

//======================================================================================
// TEXT MEASUREMENT / LAYOUT
//======================================================================================

int getTextWidth(DMD &display, const char *text)
{
  int width = 0;
  for (int i = 0; text[i] != '\0'; i++) {
    width += display.charWidth(text[i]) + 1;
  }
  return width;
}

void prepareText()
{
  topWidth    = getTextWidth(dmd_top, topText);
  bottomWidth = getTextWidth(dmd_bottom, bottomText);

  topX    = (topWidth    <= SCREEN_WIDTH) ? (SCREEN_WIDTH - topWidth)    / 2 : SCREEN_WIDTH;
  bottomX = (bottomWidth <= SCREEN_WIDTH) ? (SCREEN_WIDTH - bottomWidth) / 2 : SCREEN_WIDTH;

  needsRedraw = true;
}

//======================================================================================
// RENDER - writes both frame buffers. Blanks the panels while the buffers are being
// rewritten so a half-drawn frame is never latched out.
//======================================================================================

void renderFrame()
{
  xSemaphoreTake(displayMutex, portMAX_DELAY);

  OE_DMD_ROWS_OFF();

  dmd_top.clearScreen(true);
  dmd_bottom.clearScreen(true);

  dmd_top.drawString(topX, 0, topText, strlen(topText), GRAPHICS_NORMAL);
  dmd_bottom.drawString(bottomX, 0, bottomText, strlen(bottomText), GRAPHICS_NORMAL);

  OE_DMD_ROWS_ON();

  xSemaphoreGive(displayMutex);
}

//======================================================================================
// DISPLAY REFRESH TASK - core 1, higher priority than loop()
// Runs continuously so brightness no longer depends on how fast loop() spins.
//======================================================================================

void displayTask(void *parameter)
{
  for (;;) {
    xSemaphoreTake(displayMutex, portMAX_DELAY);
    dmd_top.scanDualBitBang(dmd_bottom, DATA_PIN_TOP, DATA_PIN_BOTTOM);
    xSemaphoreGive(displayMutex);

    vTaskDelay(1);   // ~1 ms per row group -> roughly 200+ Hz frame rate, and lets loop() run
  }
}

//======================================================================================
// NETWORK HELPERS
//======================================================================================

bool connectWiFi(unsigned long timeoutMs)
{
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);            // modem sleep breaks long TLS handshakes
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");
  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start > timeoutMs) {
      Serial.println();
      Serial.println("WiFi timeout.");
      return false;
    }
    Serial.print(".");
    delay(400);
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Gateway:    ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("RSSI:       ");
  Serial.println(WiFi.RSSI());
  return true;
}

// An IP address alone does not mean the hotspot has a working route to the internet.
bool checkInternet()
{
  HTTPClient http;
  http.setConnectTimeout(8000);
  http.setTimeout(8000);

  if (!http.begin("http://clients3.google.com/generate_204")) {
    Serial.println("Internet test: could not start HTTP client.");
    return false;
  }

  int code = http.GET();
  http.end();

  Serial.print("Internet test HTTP code: ");
  Serial.println(code);

  if (code == 204 || code == 200) {
    Serial.println("Internet reachable.");
    return true;
  }

  Serial.println("NO INTERNET ROUTE.");
  Serial.println("If you are on a phone hotspot, turn MOBILE DATA ON.");
  return false;
}

// Firebase cannot validate the TLS certificate without a correct clock.
bool syncTime(unsigned long timeoutMs)
{
  configTime(0, 0, "pool.ntp.org", "time.nist.gov", "time.google.com");

  Serial.print("Waiting for NTP time");
  unsigned long start = millis();
  time_t now = time(nullptr);

  while (now < 1700000000UL) {
    if (millis() - start > timeoutMs) {
      Serial.println();
      Serial.println("NTP SYNC FAILED - UDP port 123 may be blocked on this network.");
      return false;
    }
    delay(400);
    Serial.print(".");
    now = time(nullptr);
  }

  Serial.println();
  Serial.print("Epoch: ");
  Serial.println((long)now);
  Serial.print("UTC:   ");
  Serial.print(ctime(&now));
  return true;
}

//======================================================================================
// FIREBASE TASK - core 0, 20 KB stack.
// Firebase.begin() and every network call live here. The Arduino setup()/loop() task
// only has ~8 KB of stack, which is not enough for the BearSSL handshake - that is what
// made the original sketch hang at "Calling Firebase.begin()...".
//======================================================================================

void applyIncoming(const char *dest, const String &value, char *buffer)
{
  xSemaphoreTake(textMutex, portMAX_DELAY);
  strncpy(buffer, value.c_str(), TEXT_LEN - 1);
  buffer[TEXT_LEN - 1] = '\0';
  xSemaphoreGive(textMutex);

  Serial.print(dest);
  Serial.print(": ");
  Serial.println(value);
}

void firebaseTask(void *parameter)
{
  Serial.println();
  Serial.println("======================================");
  Serial.println("       FIREBASE TASK STARTING");
  Serial.println("======================================");
  Serial.print("Task stack high water mark at start: ");
  Serial.println(uxTaskGetStackHighWaterMark(NULL));

  //------------------------------------------------------------------
  // Wait until the network is genuinely usable before touching Firebase
  //------------------------------------------------------------------
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(pdMS_TO_TICKS(500));
  }

  while (!checkInternet()) {
    Serial.println("Retrying internet check in 5 s...");
    vTaskDelay(pdMS_TO_TICKS(5000));
  }

  while (!syncTime(20000)) {
    Serial.println("Retrying NTP in 5 s...");
    vTaskDelay(pdMS_TO_TICKS(5000));
  }

  //------------------------------------------------------------------
  // Firebase configuration
  //------------------------------------------------------------------
  Serial.println();
  Serial.println("Configuring Firebase...");

  config.api_key      = FIREBASE_API_KEY;
  config.database_url = FIREBASE_DATABASE_URL;

  auth.user.email    = FIREBASE_USER_EMAIL;
  auth.user.password = FIREBASE_USER_PASSWORD;

  // Prints exactly which auth stage is running / failing.
  config.token_status_callback = tokenStatusCallback;

  // Never block forever.
  config.timeout.socketConnection  = 10 * 1000;
  config.timeout.serverResponse    = 10 * 1000;
  config.timeout.wifiReconnect     = 10 * 1000;
  config.timeout.rtdbKeepAlive     = 45 * 1000;
  config.timeout.rtdbStreamReconnect = 1000;

  config.max_token_generation_retry = 5;

  Serial.print("Free heap before Firebase.begin(): ");
  Serial.println(ESP.getFreeHeap());
  Serial.println("Calling Firebase.begin()...");

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);

  Serial.println("Firebase.begin() returned.");
  Serial.print("Free heap after Firebase.begin(): ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("Task stack high water mark after begin: ");
  Serial.println(uxTaskGetStackHighWaterMark(NULL));

  //------------------------------------------------------------------
  // Wait for the auth token
  //------------------------------------------------------------------
  Serial.println("Waiting for Firebase authentication...");
  unsigned long authStart = millis();

  while (!Firebase.ready() && millis() - authStart < 60000) {
    vTaskDelay(pdMS_TO_TICKS(500));
  }

  if (Firebase.ready()) {
    Serial.println("FIREBASE AUTHENTICATION SUCCESS!");
  } else {
    Serial.println("Firebase not ready yet - will keep retrying in the poll loop.");
  }

  //------------------------------------------------------------------
  // Poll loop
  //------------------------------------------------------------------
  for (;;) {

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected. Reconnecting...");
      WiFi.reconnect();
      vTaskDelay(pdMS_TO_TICKS(3000));
      continue;
    }

    if (!Firebase.ready()) {
      Serial.println("Firebase not ready yet...");
      vTaskDelay(pdMS_TO_TICKS(1000));
      continue;
    }

    bool got = false;

    if (Firebase.getString(fbdo, DEVICE_PATH_LINE1)) {
      applyIncoming("Firebase line1", fbdo.stringData(), firebaseTopText);
      got = true;
    } else {
      Serial.print("line1 ERROR: ");
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.getString(fbdo, DEVICE_PATH_LINE2)) {
      applyIncoming("Firebase line2", fbdo.stringData(), firebaseBottomText);
      got = true;
    } else {
      Serial.print("line2 ERROR: ");
      Serial.println(fbdo.errorReason());
    }

    if (got) firebaseTextChanged = true;

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

//======================================================================================
// SETUP
//======================================================================================

void setup()
{
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("======================================");
  Serial.println(" ESP32 SMART LED DISPLAY STARTED");
  Serial.println("======================================");

  //--------------------------------------------------
  // Mutexes
  //--------------------------------------------------
  textMutex    = xSemaphoreCreateMutex();
  displayMutex = xSemaphoreCreateMutex();

  //--------------------------------------------------
  // Panels
  //--------------------------------------------------
  DMD::initPins();

  if (!dmd_top.ramOk() || !dmd_bottom.ramOk()) {
    Serial.println("FATAL: could not allocate display RAM. Halting.");
    for (;;) delay(1000);
  }

  dmd_top.selectFont(Arial_Black_16);
  dmd_bottom.selectFont(Arial_Black_16);

  dmd_top.clearScreen(true);
  dmd_bottom.clearScreen(true);

  prepareText();
  renderFrame();

  // Start refreshing immediately so the panels show text while WiFi/Firebase come up.
  xTaskCreatePinnedToCore(
    displayTask,
    "DisplayTask",
    4096,
    NULL,
    2,            // above loop() so refresh is never starved
    NULL,
    1             // core 1, same as loop()
  );

  //--------------------------------------------------
  // WiFi
  //--------------------------------------------------
  Serial.println("Starting WiFi...");

  int attempts = 0;
  while (!connectWiFi(25000)) {
    attempts++;
    Serial.print("WiFi attempt ");
    Serial.print(attempts);
    Serial.println(" failed. Retrying...");
    WiFi.disconnect(true);
    delay(1000);
  }

  //--------------------------------------------------
  // Firebase task on core 0 with a stack large enough for TLS
  //--------------------------------------------------
  xTaskCreatePinnedToCore(
    firebaseTask,
    "FirebaseTask",
    20480,        // was 10000 - too small for the BearSSL handshake
    NULL,
    1,
    NULL,
    0             // core 0, away from the display
  );

  Serial.println("Setup complete.");
}

//======================================================================================
// LOOP - scroll bookkeeping only. Refresh happens in displayTask.
//======================================================================================

void loop()
{
  //--------------------------------------------------
  // Apply new Firebase text
  //--------------------------------------------------
  if (firebaseTextChanged) {

    xSemaphoreTake(textMutex, portMAX_DELAY);

    bool changed = false;

    if (strcmp(topText, firebaseTopText) != 0) {
      strncpy(topText, firebaseTopText, TEXT_LEN - 1);
      topText[TEXT_LEN - 1] = '\0';
      changed = true;
    }

    if (strcmp(bottomText, firebaseBottomText) != 0) {
      strncpy(bottomText, firebaseBottomText, TEXT_LEN - 1);
      bottomText[TEXT_LEN - 1] = '\0';
      changed = true;
    }

    xSemaphoreGive(textMutex);

    firebaseTextChanged = false;

    if (changed) {
      prepareText();
      Serial.println("Display text updated from Firebase.");
      Serial.print("TOP:    "); Serial.println(topText);
      Serial.print("BOTTOM: "); Serial.println(bottomText);
    }
  }

  //--------------------------------------------------
  // Scroll
  //--------------------------------------------------
  if (millis() - lastScroll >= scrollDelay) {
    lastScroll = millis();

    if (topWidth > SCREEN_WIDTH) {
      topX--;
      if (topX < -topWidth) topX = SCREEN_WIDTH;
      needsRedraw = true;
    }

    if (bottomWidth > SCREEN_WIDTH) {
      bottomX--;
      if (bottomX < -bottomWidth) bottomX = SCREEN_WIDTH;
      needsRedraw = true;
    }
  }

  //--------------------------------------------------
  // Redraw only when something actually moved
  //--------------------------------------------------
  if (needsRedraw) {
    renderFrame();
    needsRedraw = false;
  }

  vTaskDelay(1);   // yield to the display and idle tasks
}
