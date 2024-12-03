#pragma once

namespace DarkMoon {
    struct Color {
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
        unsigned char a = 255;
    };

    // Custom engine color palette (UI Two Wonders)
#define D_WHITE           DarkMoon::Color { 255, 255, 255, 255 }
#define D_BLACK           DarkMoon::Color {   0,   0,   0, 255 }
#define D_GRAY            DarkMoon::Color { 100, 100, 100, 255 }

#define D_AQUA            DarkMoon::Color { 108, 198, 215, 255 }
#define D_AQUA_LIGHT      DarkMoon::Color { 154, 222, 235, 255 }
#define D_AQUA_DARK       DarkMoon::Color {  40, 141, 160, 255 }

#define D_BLUE            DarkMoon::Color { 121, 145, 221, 255 }
#define D_BLUE_LIGHT      DarkMoon::Color { 164, 181, 238, 255 }
#define D_BLUE_DARK       DarkMoon::Color {  56,  84, 175, 255 }

#define D_YELLOW          DarkMoon::Color { 255, 215, 125, 255 }
#define D_YELLOW_LIGHT    DarkMoon::Color { 255, 227, 165, 255 }
#define D_YELLOW_DARK     DarkMoon::Color { 255, 194,  59, 255 }

#define D_ORANGE          DarkMoon::Color { 255, 188, 125, 255 }
#define D_ORANGE_LIGHT    DarkMoon::Color { 255, 209, 165, 255 }
#define D_ORANGE_DARK     DarkMoon::Color { 255, 155,  59, 255 }

#define D_KAIWA           DarkMoon::Color { 113, 230, 138, 255 }
#define D_KAIWA_LIGHT     DarkMoon::Color { 157, 242, 176, 255 }
#define D_KAIWA_DARK      DarkMoon::Color {  45, 195,  78, 255 }

#define D_CORAL_PINK      DarkMoon::Color { 255, 173, 165, 255 }
#define D_CORAL_PINK_LIGHT DarkMoon::Color { 255, 216, 212, 255 }
#define D_CORAL_PINK_DARK DarkMoon::Color { 255, 137, 125, 255 }

#define D_RED             DarkMoon::Color { 255,  78,  59, 255 }

#define D_LAVENDER        DarkMoon::Color { 219, 170, 205, 255 }
#define D_LAVENDER_LIGHT  DarkMoon::Color { 238, 212, 231, 255 }
#define D_LAVENDER_DARK   DarkMoon::Color { 162,  78, 139, 255 }

#define D_ROSE            DarkMoon::Color { 247, 192, 201, 255 }
#define D_ROSE_LIGHT      DarkMoon::Color { 251, 223, 228, 255 }
#define D_ROSE_DARK       DarkMoon::Color { 206,  98, 115, 255 }

#define D_GREEN           DarkMoon::Color { 196, 234, 182, 255 }
#define D_GREEN_LIGHT     DarkMoon::Color { 225, 245, 218, 255 }
#define D_GREEN_DARK      DarkMoon::Color { 115, 186,  89, 255 }

#define D_VIOLET          DarkMoon::Color { 234, 248, 192, 255 }
#define D_VIOLET_LIGHT    DarkMoon::Color { 244, 252, 223, 255 }
#define D_VIOLET_DARK     DarkMoon::Color { 179, 206,  99, 255 }

#define D_LEMON           DarkMoon::Color { 255, 252, 198, 255 }
#define D_LEMON_LIGHT     DarkMoon::Color { 255, 254, 226, 255 }
#define D_LEMON_DARK      DarkMoon::Color { 218, 213, 104, 255 }

#define D_PALE_PINK       DarkMoon::Color { 255, 208, 198, 255 }
#define D_PALE_PINK_LIGHT DarkMoon::Color { 255, 231, 226, 255 }
#define D_PALE_PINK_DARK  DarkMoon::Color { 218, 124, 104, 255 }

#define D_MINT            DarkMoon::Color { 168, 216, 186, 255 }
#define D_MINT_LIGHT      DarkMoon::Color { 210, 237, 220, 255 }
#define D_MINT_DARK       DarkMoon::Color {  75, 158, 106, 255 }

#define D_PINK            DarkMoon::Color { 251, 162, 183, 255 }
#define D_PINK_LIGHT      DarkMoon::Color { 254, 211, 221, 255 }
#define D_PINK_DARK       DarkMoon::Color { 247, 121, 150, 255 }

#define D_MAGENTA         DarkMoon::Color { 235,  55,  97, 255 }
}