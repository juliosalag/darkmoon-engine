#pragma once
#include <GLFW/glfw3.h>

namespace DarkMoon {
    // Key codes for alphanumeric keys
    #define D_KEY_A         GLFW_KEY_A
    #define D_KEY_B         GLFW_KEY_B
    #define D_KEY_C         GLFW_KEY_C
    #define D_KEY_D         GLFW_KEY_D
    #define D_KEY_E         GLFW_KEY_E
    #define D_KEY_F         GLFW_KEY_F
    #define D_KEY_G         GLFW_KEY_G
    #define D_KEY_H         GLFW_KEY_H
    #define D_KEY_I         GLFW_KEY_I
    #define D_KEY_J         GLFW_KEY_J
    #define D_KEY_K         GLFW_KEY_K
    #define D_KEY_L         GLFW_KEY_L
    #define D_KEY_M         GLFW_KEY_M
    #define D_KEY_N         GLFW_KEY_N
    #define D_KEY_O         GLFW_KEY_O
    #define D_KEY_P         GLFW_KEY_P
    #define D_KEY_Q         GLFW_KEY_Q
    #define D_KEY_R         GLFW_KEY_R
    #define D_KEY_S         GLFW_KEY_S
    #define D_KEY_T         GLFW_KEY_T
    #define D_KEY_U         GLFW_KEY_U
    #define D_KEY_V         GLFW_KEY_V
    #define D_KEY_W         GLFW_KEY_W
    #define D_KEY_X         GLFW_KEY_X
    #define D_KEY_Y         GLFW_KEY_Y
    #define D_KEY_Z         GLFW_KEY_Z
    #define D_KEY_0         GLFW_KEY_0
    #define D_KEY_1         GLFW_KEY_1
    #define D_KEY_2         GLFW_KEY_2
    #define D_KEY_3         GLFW_KEY_3
    #define D_KEY_4         GLFW_KEY_4
    #define D_KEY_5         GLFW_KEY_5
    #define D_KEY_6         GLFW_KEY_6
    #define D_KEY_7         GLFW_KEY_7
    #define D_KEY_8         GLFW_KEY_8
    #define D_KEY_9         GLFW_KEY_9

    // Key codes for special keys
    #define D_KEY_SPACE     GLFW_KEY_SPACE
    #define D_KEY_ENTER     GLFW_KEY_ENTER
    #define D_KEY_ESCAPE    GLFW_KEY_ESCAPE
    #define D_KEY_LEFT      GLFW_KEY_LEFT
    #define D_KEY_RIGHT     GLFW_KEY_RIGHT
    #define D_KEY_UP        GLFW_KEY_UP
    #define D_KEY_DOWN      GLFW_KEY_DOWN

    // Key codes for function keys (F1 - F25)
    #define D_KEY_F1        GLFW_KEY_F1
    #define D_KEY_F2        GLFW_KEY_F2
    #define D_KEY_F3        GLFW_KEY_F3
    #define D_KEY_F4        GLFW_KEY_F4
    #define D_KEY_F5        GLFW_KEY_F5
    #define D_KEY_F6        GLFW_KEY_F6
    #define D_KEY_F7        GLFW_KEY_F7
    #define D_KEY_F8        GLFW_KEY_F8
    #define D_KEY_F9        GLFW_KEY_F9
    #define D_KEY_F10       GLFW_KEY_F10
    #define D_KEY_F11       GLFW_KEY_F11
    #define D_KEY_F12       GLFW_KEY_F12
    #define D_KEY_F13       GLFW_KEY_F13
    #define D_KEY_F14       GLFW_KEY_F14
    #define D_KEY_F15       GLFW_KEY_F15
    #define D_KEY_F16       GLFW_KEY_F16
    #define D_KEY_F17       GLFW_KEY_F17
    #define D_KEY_F18       GLFW_KEY_F18
    #define D_KEY_F19       GLFW_KEY_F19
    #define D_KEY_F20       GLFW_KEY_F20
    #define D_KEY_F21       GLFW_KEY_F21
    #define D_KEY_F22       GLFW_KEY_F22
    #define D_KEY_F23       GLFW_KEY_F23
    #define D_KEY_F24       GLFW_KEY_F24
    #define D_KEY_F25       GLFW_KEY_F25

    // Key codes for modifier keys
    #define D_KEY_LEFT_SHIFT       GLFW_KEY_LEFT_SHIFT
    #define D_KEY_RIGHT_SHIFT      GLFW_KEY_RIGHT_SHIFT
    #define D_KEY_LEFT_CONTROL     GLFW_KEY_LEFT_CONTROL
    #define D_KEY_RIGHT_CONTROL    GLFW_KEY_RIGHT_CONTROL
    #define D_KEY_LEFT_ALT         GLFW_KEY_LEFT_ALT
    #define D_KEY_RIGHT_ALT        GLFW_KEY_RIGHT_ALT
    #define D_KEY_LEFT_SUPER       GLFW_KEY_LEFT_SUPER
    #define D_KEY_RIGHT_SUPER      GLFW_KEY_RIGHT_SUPER
    #define D_KEY_MENU             GLFW_KEY_MENU

    // Key codes for navigation keys
    #define D_KEY_TAB              GLFW_KEY_TAB
    #define D_KEY_BACKSPACE        GLFW_KEY_BACKSPACE
    #define D_KEY_INSERT           GLFW_KEY_INSERT
    #define D_KEY_DELETE           GLFW_KEY_DELETE
    #define D_KEY_PAGE_UP          GLFW_KEY_PAGE_UP
    #define D_KEY_PAGE_DOWN        GLFW_KEY_PAGE_DOWN
    #define D_KEY_HOME             GLFW_KEY_HOME
    #define D_KEY_END              GLFW_KEY_END

    // Key codes for numeric keypad keys
    #define D_KEY_KP_0            GLFW_KEY_KP_0
    #define D_KEY_KP_1            GLFW_KEY_KP_1
    #define D_KEY_KP_2            GLFW_KEY_KP_2
    #define D_KEY_KP_3            GLFW_KEY_KP_3
    #define D_KEY_KP_4            GLFW_KEY_KP_4
    #define D_KEY_KP_5            GLFW_KEY_KP_5
    #define D_KEY_KP_6            GLFW_KEY_KP_6
    #define D_KEY_KP_7            GLFW_KEY_KP_7
    #define D_KEY_KP_8            GLFW_KEY_KP_8
    #define D_KEY_KP_9            GLFW_KEY_KP_9
    #define D_KEY_KP_DECIMAL      GLFW_KEY_KP_DECIMAL
    #define D_KEY_KP_DIVIDE       GLFW_KEY_KP_DIVIDE
    #define D_KEY_KP_MULTIPLY     GLFW_KEY_KP_MULTIPLY
    #define D_KEY_KP_SUBTRACT     GLFW_KEY_KP_SUBTRACT
    #define D_KEY_KP_ADD          GLFW_KEY_KP_ADD
    #define D_KEY_KP_ENTER        GLFW_KEY_KP_ENTER
    #define D_KEY_KP_EQUAL        GLFW_KEY_KP_EQUAL

    // Key codes for system keys
    #define D_KEY_PRINT_SCREEN    GLFW_KEY_PRINT_SCREEN
    #define D_KEY_PAUSE           GLFW_KEY_PAUSE
    #define D_KEY_CAPS_LOCK       GLFW_KEY_CAPS_LOCK
    #define D_KEY_NUM_LOCK        GLFW_KEY_NUM_LOCK
    #define D_KEY_SCROLL_LOCK     GLFW_KEY_SCROLL_LOCK

    // Key codes for media keys
    #define D_KEY_VOLUME_MUTE            GLFW_KEY_VOLUME_MUTE
    #define D_KEY_VOLUME_UP              GLFW_KEY_VOLUME_UP
    #define D_KEY_VOLUME_DOWN            GLFW_KEY_VOLUME_DOWN
    #define D_KEY_MEDIA_PLAY             GLFW_KEY_MEDIA_PLAY
    #define D_KEY_MEDIA_PAUSE            GLFW_KEY_MEDIA_PAUSE
    #define D_KEY_MEDIA_STOP             GLFW_KEY_MEDIA_STOP
    #define D_KEY_MEDIA_NEXT_TRACK       GLFW_KEY_MEDIA_NEXT_TRACK
    #define D_KEY_MEDIA_PREVIOUS_TRACK   GLFW_KEY_MEDIA_PREVIOUS_TRACK
    #define D_KEY_MEDIA_FAST_FORWARD     GLFW_KEY_MEDIA_FAST_FORWARD
    #define D_KEY_MEDIA_REWIND           GLFW_KEY_MEDIA_REWIND

    // Key codes for special characters
    #define D_KEY_GRAVE_ACCENT    GLFW_KEY_GRAVE_ACCENT
    #define D_KEY_MINUS           GLFW_KEY_MINUS
    #define D_KEY_EQUAL           GLFW_KEY_EQUAL
    #define D_KEY_LEFT_BRACKET    GLFW_KEY_LEFT_BRACKET
    #define D_KEY_RIGHT_BRACKET   GLFW_KEY_RIGHT_BRACKET
    #define D_KEY_BACKSLASH       GLFW_KEY_BACKSLASH
    #define D_KEY_SEMICOLON       GLFW_KEY_SEMICOLON
    #define D_KEY_APOSTROPHE      GLFW_KEY_APOSTROPHE
    #define D_KEY_COMMA           GLFW_KEY_COMMA
    #define D_KEY_PERIOD          GLFW_KEY_PERIOD
    #define D_KEY_SLASH           GLFW_KEY_SLASH
    #define D_KEY_WORLD_1         GLFW_KEY_WORLD_1
    #define D_KEY_WORLD_2         GLFW_KEY_WORLD_2

    // Key codes for gamepad buttons
    #define D_GAMEPAD_AXIS_LAST             GLFW_GAMEPAD_AXIS_LAST
    #define D_GAMEPAD_AXIS_LEFT_TRIGGER     GLFW_GAMEPAD_AXIS_LEFT_TRIGGER
    #define D_GAMEPAD_AXIS_LEFT_X           GLFW_GAMEPAD_AXIS_LEFT_X
    #define D_GAMEPAD_AXIS_LEFT_Y           GLFW_GAMEPAD_AXIS_LEFT_Y
    #define D_GAMEPAD_AXIS_RIGHT_TRIGGER    GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
    #define D_GAMEPAD_AXIS_RIGHT_X          GLFW_GAMEPAD_AXIS_RIGHT_X
    #define D_GAMEPAD_AXIS_RIGHT_Y          GLFW_GAMEPAD_AXIS_RIGHT_Y
    #define D_GAMEPAD_BUTTON_A              GLFW_GAMEPAD_BUTTON_A
    #define D_GAMEPAD_BUTTON_B              GLFW_GAMEPAD_BUTTON_B
    #define D_GAMEPAD_BUTTON_BACK           GLFW_GAMEPAD_BUTTON_BACK
    #define D_GAMEPAD_BUTTON_CIRCLE         GLFW_GAMEPAD_BUTTON_CIRCLE
    #define D_GAMEPAD_BUTTON_CROSS          GLFW_GAMEPAD_BUTTON_CROSS
    #define D_GAMEPAD_BUTTON_DPAD_DOWN      GLFW_GAMEPAD_BUTTON_DPAD_DOWN
    #define D_GAMEPAD_BUTTON_DPAD_LEFT      GLFW_GAMEPAD_BUTTON_DPAD_LEFT
    #define D_GAMEPAD_BUTTON_DPAD_RIGHT     GLFW_GAMEPAD_BUTTON_DPAD_RIGHT
    #define D_GAMEPAD_BUTTON_DPAD_UP        GLFW_GAMEPAD_BUTTON_DPAD_UP
    #define D_GAMEPAD_BUTTON_GUIDE          GLFW_GAMEPAD_BUTTON_GUIDE
    #define D_GAMEPAD_BUTTON_LAST           GLFW_GAMEPAD_BUTTON_LAST
    #define D_GAMEPAD_BUTTON_LEFT_BUMPER    GLFW_GAMEPAD_BUTTON_LEFT_BUMPER
    #define D_GAMEPAD_BUTTON_LEFT_THUMB     GLFW_GAMEPAD_BUTTON_LEFT_THUMB
    #define D_GAMEPAD_BUTTON_RIGHT_BUMPER   GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER
    #define D_GAMEPAD_BUTTON_RIGHT_THUMB    GLFW_GAMEPAD_BUTTON_RIGHT_THUMB
    #define D_GAMEPAD_BUTTON_SQUARE         GLFW_GAMEPAD_BUTTON_SQUARE
    #define D_GAMEPAD_BUTTON_START          GLFW_GAMEPAD_BUTTON_START
    #define D_GAMEPAD_BUTTON_TRIANGLE       GLFW_GAMEPAD_BUTTON_TRIANGLE
    #define D_GAMEPAD_BUTTON_X              GLFW_GAMEPAD_BUTTON_X
    #define D_GAMEPAD_BUTTON_Y              GLFW_GAMEPAD_BUTTON_Y

    // Key codes for mouse buttons
    #define D_MOUSE_BUTTON_1    GLFW_MOUSE_BUTTON_1
    #define D_MOUSE_BUTTON_2    GLFW_MOUSE_BUTTON_2
    #define D_MOUSE_BUTTON_3    GLFW_MOUSE_BUTTON_3
    #define D_MOUSE_BUTTON_4    GLFW_MOUSE_BUTTON_4
    #define D_MOUSE_BUTTON_5    GLFW_MOUSE_BUTTON_5

    #define D_MOUSE_BUTTON_LEFT     GLFW_MOUSE_BUTTON_LEFT
    #define D_MOUSE_BUTTON_RIGHT    GLFW_MOUSE_BUTTON_RIGHT
    #define D_MOUSE_BUTTON_MIDDLE   GLFW_MOUSE_BUTTON_MIDDLE
    #define D_MOUSE_BUTTON_SIDE_1   GLFW_MOUSE_BUTTON_4
    #define D_MOUSE_BUTTON_SIDE_2   GLFW_MOUSE_BUTTON_5

    #define D_MOUSE_SCROLL_UP       GLFW_MOUSE_SCROLL_UP
    #define D_MOUSE_SCROLL_DOWN     GLFW_MOUSE_SCROLL_DOWN
    #define D_MOUSE_SCROLL_LEFT     GLFW_MOUSE_SCROLL_LEFT
    #define D_MOUSE_SCROLL_RIGHT    GLFW_MOUSE_SCROLL_RIGHT
};