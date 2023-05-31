/**
 * KeyCodes.hh
 * Created by kate on 5/31/23.
 * */
#ifndef KATE_ENGINE_KEYCODES_HH
#define KATE_ENGINE_KEYCODES_HH

#include <GLFW/glfw3.h>

#if defined(USE_GLFW_KEY_CODES)
    #define KT_KEY_SPACE              GLFW_KEY_SPACE
    #define KT_KEY_APOSTROPHE         GLFW_KEY_APOSTROPHE   /* ' */
    #define KT_KEY_COMMA              GLFW_KEY_COMMA        /* , */
    #define KT_KEY_MINUS              GLFW_KEY_MINUS        /* - */
    #define KT_KEY_PERIOD             GLFW_KEY_PERIOD       /* . */
    #define KT_KEY_SLASH              GLFW_KEY_SLASH        /* / */
    #define KT_KEY_0                  GLFW_KEY_0
    #define KT_KEY_1                  GLFW_KEY_1
    #define KT_KEY_2                  GLFW_KEY_2
    #define KT_KEY_3                  GLFW_KEY_3
    #define KT_KEY_4                  GLFW_KEY_4
    #define KT_KEY_5                  GLFW_KEY_5
    #define KT_KEY_6                  GLFW_KEY_6
    #define KT_KEY_7                  GLFW_KEY_7
    #define KT_KEY_8                  GLFW_KEY_8
    #define KT_KEY_9                  GLFW_KEY_9
    #define KT_KEY_SEMICOLON          GLFW_KEY_SEMICOLON    /* ; */
    #define KT_KEY_EQUAL              GLFW_KEY_EQUAL        /* = */
    #define KT_KEY_A                  GLFW_KEY_A
    #define KT_KEY_B                  GLFW_KEY_B
    #define KT_KEY_C                  GLFW_KEY_C
    #define KT_KEY_D                  GLFW_KEY_D
    #define KT_KEY_E                  GLFW_KEY_E
    #define KT_KEY_F                  GLFW_KEY_F
    #define KT_KEY_G                  GLFW_KEY_G
    #define KT_KEY_H                  GLFW_KEY_H
    #define KT_KEY_I                  GLFW_KEY_I
    #define KT_KEY_J                  GLFW_KEY_J
    #define KT_KEY_K                  GLFW_KEY_K
    #define KT_KEY_L                  GLFW_KEY_L
    #define KT_KEY_M                  GLFW_KEY_M
    #define KT_KEY_N                  GLFW_KEY_N
    #define KT_KEY_O                  GLFW_KEY_O
    #define KT_KEY_P                  GLFW_KEY_P
    #define KT_KEY_Q                  GLFW_KEY_Q
    #define KT_KEY_R                  GLFW_KEY_R
    #define KT_KEY_S                  GLFW_KEY_S
    #define KT_KEY_T                  GLFW_KEY_T
    #define KT_KEY_U                  GLFW_KEY_U
    #define KT_KEY_V                  GLFW_KEY_V
    #define KT_KEY_W                  GLFW_KEY_W
    #define KT_KEY_X                  GLFW_KEY_X
    #define KT_KEY_Y                  GLFW_KEY_Y
    #define KT_KEY_Z                  GLFW_KEY_Z
    #define KT_KEY_LEFT_BRACKET       GLFW_KEY_LEFT_BRACKET     /* [ */
    #define KT_KEY_BACKSLASH          GLFW_KEY_BACKSLASH        /* \ */
    #define KT_KEY_RIGHT_BRACKET      GLFW_KEY_RIGHT_BRACKET    /* ] */
    #define KT_KEY_GRAVE_ACCENT       GLFW_KEY_GRAVE_ACCENT     /* ` */
    #define KT_KEY_WORLD_1            GLFW_KEY_WORLD_1          /* non-US #1 */
    #define KT_KEY_WORLD_2            GLFW_KEY_WORLD_2          /* non-US #2 */

    /* Function keys */
    #define KT_KEY_ESCAPE             GLFW_KEY_ESCAPE
    #define KT_KEY_ENTER              GLFW_KEY_ENTER
    #define KT_KEY_TAB                GLFW_KEY_TAB
    #define KT_KEY_BACKSPACE          GLFW_KEY_BACKSPACE
    #define KT_KEY_INSERT             GLFW_KEY_INSERT
    #define KT_KEY_DELETE             GLFW_KEY_DELETE
    #define KT_KEY_RIGHT              GLFW_KEY_RIGHT
    #define KT_KEY_LEFT               GLFW_KEY_LEFT
    #define KT_KEY_DOWN               GLFW_KEY_DOWN
    #define KT_KEY_UP                 GLFW_KEY_UP
    #define KT_KEY_PAGE_UP            GLFW_KEY_PAGE_UP
    #define KT_KEY_PAGE_DOWN          GLFW_KEY_PAGE_DOWN
    #define KT_KEY_HOME               GLFW_KEY_HOME
    #define KT_KEY_END                GLFW_KEY_END
    #define KT_KEY_CAPS_LOCK          GLFW_KEY_CAPS_LOCK
    #define KT_KEY_SCROLL_LOCK        GLFW_KEY_SCROLL_LOCK
    #define KT_KEY_NUM_LOCK           GLFW_KEY_NUM_LOCK
    #define KT_KEY_PRINT_SCREEN       GLFW_KEY_PRINT_SCREEN
    #define KT_KEY_PAUSE              GLFW_KEY_PAUSE
    #define KT_KEY_F1                 GLFW_KEY_F1
    #define KT_KEY_F2                 GLFW_KEY_F2
    #define KT_KEY_F3                 GLFW_KEY_F3
    #define KT_KEY_F4                 GLFW_KEY_F4
    #define KT_KEY_F5                 GLFW_KEY_F5
    #define KT_KEY_F6                 GLFW_KEY_F6
    #define KT_KEY_F7                 GLFW_KEY_F7
    #define KT_KEY_F8                 GLFW_KEY_F8
    #define KT_KEY_F9                 GLFW_KEY_F9
    #define KT_KEY_F10                GLFW_KEY_F10
    #define KT_KEY_F11                GLFW_KEY_F11
    #define KT_KEY_F12                GLFW_KEY_F12
    #define KT_KEY_F13                GLFW_KEY_F13
    #define KT_KEY_F14                GLFW_KEY_F14
    #define KT_KEY_F15                GLFW_KEY_F15
    #define KT_KEY_F16                GLFW_KEY_F16
    #define KT_KEY_F17                GLFW_KEY_F17
    #define KT_KEY_F18                GLFW_KEY_F18
    #define KT_KEY_F19                GLFW_KEY_F19
    #define KT_KEY_F20                GLFW_KEY_F20
    #define KT_KEY_F21                GLFW_KEY_F21
    #define KT_KEY_F22                GLFW_KEY_F22
    #define KT_KEY_F23                GLFW_KEY_F23
    #define KT_KEY_F24                GLFW_KEY_F24
    #define KT_KEY_F25                GLFW_KEY_F25
    #define KT_KEY_KP_0               GLFW_KEY_KP_0
    #define KT_KEY_KP_1               GLFW_KEY_KP_1
    #define KT_KEY_KP_2               GLFW_KEY_KP_2
    #define KT_KEY_KP_3               GLFW_KEY_KP_3
    #define KT_KEY_KP_4               GLFW_KEY_KP_4
    #define KT_KEY_KP_5               GLFW_KEY_KP_5
    #define KT_KEY_KP_6               GLFW_KEY_KP_6
    #define KT_KEY_KP_7               GLFW_KEY_KP_7
    #define KT_KEY_KP_8               GLFW_KEY_KP_8
    #define KT_KEY_KP_9               GLFW_KEY_KP_9
    #define KT_KEY_KP_DECIMAL         GLFW_KEY_KP_DECIMAL
    #define KT_KEY_KP_DIVIDE          GLFW_KEY_KP_DIVIDE
    #define KT_KEY_KP_MULTIPLY        GLFW_KEY_KP_MULTIPLY
    #define KT_KEY_KP_SUBTRACT        GLFW_KEY_KP_SUBTRACT
    #define KT_KEY_KP_ADD             GLFW_KEY_KP_ADD
    #define KT_KEY_KP_ENTER           GLFW_KEY_KP_ENTER
    #define KT_KEY_KP_EQUAL           GLFW_KEY_KP_EQUAL
    #define KT_KEY_LEFT_SHIFT         GLFW_KEY_LEFT_SHIFT
    #define KT_KEY_LEFT_CONTROL       GLFW_KEY_LEFT_CONTROL
    #define KT_KEY_LEFT_ALT           GLFW_KEY_LEFT_ALT
    #define KT_KEY_LEFT_SUPER         GLFW_KEY_LEFT_SUPER
    #define KT_KEY_RIGHT_SHIFT        GLFW_KEY_RIGHT_SHIFT
    #define KT_KEY_RIGHT_CONTROL      GLFW_KEY_RIGHT_CONTROL
    #define KT_KEY_RIGHT_ALT          GLFW_KEY_RIGHT_ALT
    #define KT_KEY_RIGHT_SUPER        GLFW_KEY_RIGHT_SUPER
    #define KT_KEY_MENU               GLFW_KEY_MENU
#endif

#endif//KATE_ENGINE_KEYCODES_HH
