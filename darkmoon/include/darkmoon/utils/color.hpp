#pragma once

struct Color {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 255;
};

// --- Basic ---
#define WHITE           Color { 255, 255, 255, 255 }
#define LIGHT_GRAY      Color { 168, 176, 192, 255 }
#define GRAY            Color { 112, 136, 144, 255 }
#define DARK_GRAY       Color {  80,  96, 104, 255 }
#define BLACK           Color {   0,   0,   0, 255 }

// --- Dark tier ---
#define DARK_RED        Color { 112,  24,  16, 255 }
#define DARK_GREEN      Color {  24, 120,  24, 255 }
#define DARK_BLUE       Color {  24,  48, 144, 255 }
#define DARK_MAGENTA    Color { 144,  24,  96, 255 }
#define DARK_CYAN       Color {  48, 213, 200, 255 }
#define DARK_YELLOW     Color { 232, 184,  32, 255 }
#define DARK_BROWN      Color { 112,  72,  16, 255 }
#define DARK_VIOLET     Color {  88,   8, 112, 255 }
#define DARK_ORANGE     Color { 160,  64,   0, 255 }

// --- Base tier ---
#define RED             Color { 216,  40,  40, 255 }
#define GREEN           Color {  48, 208,  24, 255 }
#define BLUE            Color {  40, 160, 240, 255 }
#define MAGENTA         Color { 200,  72, 144, 255 }
#define CYAN            Color {   0, 255, 255, 255 }
#define YELLOW          Color { 248, 232,  40, 255 }
#define BROWN           Color { 160, 120,  64, 255 }
#define VIOLET          Color { 168,  80, 200, 255 }
#define ORANGE          Color { 200, 104,  40, 255 }

// --- Light tier ---
#define LIGHT_RED       Color { 248,  72,  96, 255 }
#define LIGHT_GREEN     Color { 192, 248, 144, 255 }
#define LIGHT_BLUE      Color { 168, 232, 248, 255 }
#define LIGHT_MAGENTA   Color { 248, 192, 224, 255 }
#define LIGHT_CYAN      Color { 135, 206, 255, 255 }
#define LIGHT_YELLOW    Color { 248, 248, 144, 255 }
#define LIGHT_BROWN     Color { 208, 176, 136, 255 }
#define LIGHT_VIOLET    Color { 208, 136, 232, 255 }
#define LIGHT_ORANGE    Color { 248, 200, 136, 255 }