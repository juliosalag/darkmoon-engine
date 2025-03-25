struct Color {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 255;
};

#define WHITE       Color { 255, 255, 255, 255 }
#define GRAY        Color { 100, 100, 100, 255 }
#define BLACK       Color {   0,   0,   0, 255 }