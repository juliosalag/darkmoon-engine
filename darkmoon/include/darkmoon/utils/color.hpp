#pragma once

struct Color {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 255;
};

#define WHITE       Color { 255, 255, 255, 255 }
#define GRAY        Color { 100, 100, 100, 255 }
#define BLACK       Color {   0,   0,   0, 255 }

#define RED         Color { 255,   0,   0, 255 }
#define GREEN       Color {   0, 255,   0, 255 }
#define BLUE        Color {   0,   0, 255, 255 }

// RED
// rgba(189, 90, 99)

// CREAM
// rgba(237, 219, 196)

// BLUE
// rgba(0, 152, 219)

// MID BLUE
// rgba(37, 36, 70) 

// DARK BLUE
// rgba(32, 21, 51)