#ifndef VIEWRECT_H
#define VIEWRECT_H

#include "ultralight_manager.hpp"

#include <queue>

#include <Ultralight/KeyEvent.h>
#include <Ultralight/MouseEvent.h>
#include <Ultralight/ScrollEvent.h>

#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace ultralight;

namespace godot {

    // ALERT: Map generated by Claude AI. Someone should probably double check this. 😳
    const std::unordered_map<int, int> WindowsKeyMap = {
        {Key::KEY_NONE, KeyCodes::GK_UNKNOWN},
        {Key::KEY_ESCAPE, KeyCodes::GK_ESCAPE},
        {Key::KEY_TAB, KeyCodes::GK_TAB},
        {Key::KEY_BACKTAB, KeyCodes::GK_TAB}, // No direct equivalent, mapping to TAB
        {Key::KEY_BACKSPACE, KeyCodes::GK_BACK},
        {Key::KEY_ENTER, KeyCodes::GK_RETURN},
        {Key::KEY_KP_ENTER, KeyCodes::GK_RETURN},
        {Key::KEY_INSERT, KeyCodes::GK_INSERT},
        {Key::KEY_DELETE, KeyCodes::GK_DELETE},
        {Key::KEY_PAUSE, KeyCodes::GK_PAUSE},
        {Key::KEY_PRINT, KeyCodes::GK_PRINT},
        {Key::KEY_SYSREQ, KeyCodes::GK_SNAPSHOT}, // Closest equivalent
        {Key::KEY_CLEAR, KeyCodes::GK_CLEAR},
        {Key::KEY_HOME, KeyCodes::GK_HOME},
        {Key::KEY_END, KeyCodes::GK_END},
        {Key::KEY_LEFT, KeyCodes::GK_LEFT},
        {Key::KEY_UP, KeyCodes::GK_UP},
        {Key::KEY_RIGHT, KeyCodes::GK_RIGHT},
        {Key::KEY_DOWN, KeyCodes::GK_DOWN},
        {Key::KEY_PAGEUP, KeyCodes::GK_PRIOR},
        {Key::KEY_PAGEDOWN, KeyCodes::GK_NEXT},
        {Key::KEY_SHIFT, KeyCodes::GK_SHIFT},
        {Key::KEY_CTRL, KeyCodes::GK_CONTROL},
        {Key::KEY_META, KeyCodes::GK_LWIN}, // Assuming META is Windows key
        {Key::KEY_ALT, KeyCodes::GK_MENU},
        {Key::KEY_CAPSLOCK, KeyCodes::GK_CAPITAL},
        {Key::KEY_NUMLOCK, KeyCodes::GK_NUMLOCK},
        {Key::KEY_SCROLLLOCK, KeyCodes::GK_SCROLL},
        {Key::KEY_F1, KeyCodes::GK_F1},
        {Key::KEY_F2, KeyCodes::GK_F2},
        {Key::KEY_F3, KeyCodes::GK_F3},
        {Key::KEY_F4, KeyCodes::GK_F4},
        {Key::KEY_F5, KeyCodes::GK_F5},
        {Key::KEY_F6, KeyCodes::GK_F6},
        {Key::KEY_F7, KeyCodes::GK_F7},
        {Key::KEY_F8, KeyCodes::GK_F8},
        {Key::KEY_F9, KeyCodes::GK_F9},
        {Key::KEY_F10, KeyCodes::GK_F10},
        {Key::KEY_F11, KeyCodes::GK_F11},
        {Key::KEY_F12, KeyCodes::GK_F12},
        {Key::KEY_F13, KeyCodes::GK_F13},
        {Key::KEY_F14, KeyCodes::GK_F14},
        {Key::KEY_F15, KeyCodes::GK_F15},
        {Key::KEY_F16, KeyCodes::GK_F16},
        {Key::KEY_KP_MULTIPLY, KeyCodes::GK_MULTIPLY},
        {Key::KEY_KP_DIVIDE, KeyCodes::GK_DIVIDE},
        {Key::KEY_KP_SUBTRACT, KeyCodes::GK_SUBTRACT},
        {Key::KEY_KP_PERIOD, KeyCodes::GK_DECIMAL},
        {Key::KEY_KP_ADD, KeyCodes::GK_ADD},
        {Key::KEY_KP_0, KeyCodes::GK_NUMPAD0},
        {Key::KEY_KP_1, KeyCodes::GK_NUMPAD1},
        {Key::KEY_KP_2, KeyCodes::GK_NUMPAD2},
        {Key::KEY_KP_3, KeyCodes::GK_NUMPAD3},
        {Key::KEY_KP_4, KeyCodes::GK_NUMPAD4},
        {Key::KEY_KP_5, KeyCodes::GK_NUMPAD5},
        {Key::KEY_KP_6, KeyCodes::GK_NUMPAD6},
        {Key::KEY_KP_7, KeyCodes::GK_NUMPAD7},
        {Key::KEY_KP_8, KeyCodes::GK_NUMPAD8},
        {Key::KEY_KP_9, KeyCodes::GK_NUMPAD9},
        {Key::KEY_MENU, KeyCodes::GK_MENU},
        {Key::KEY_HELP, KeyCodes::GK_HELP},
        {Key::KEY_BACK, KeyCodes::GK_BROWSER_BACK},
        {Key::KEY_FORWARD, KeyCodes::GK_BROWSER_FORWARD},
        {Key::KEY_STOP, KeyCodes::GK_BROWSER_STOP},
        {Key::KEY_REFRESH, KeyCodes::GK_BROWSER_REFRESH},
        {Key::KEY_VOLUMEDOWN, KeyCodes::GK_VOLUME_DOWN},
        {Key::KEY_VOLUMEMUTE, KeyCodes::GK_VOLUME_MUTE},
        {Key::KEY_VOLUMEUP, KeyCodes::GK_VOLUME_UP},
        {Key::KEY_MEDIAPLAY, KeyCodes::GK_MEDIA_PLAY_PAUSE},
        {Key::KEY_MEDIASTOP, KeyCodes::GK_MEDIA_STOP},
        {Key::KEY_MEDIAPREVIOUS, KeyCodes::GK_MEDIA_PREV_TRACK},
        {Key::KEY_MEDIANEXT, KeyCodes::GK_MEDIA_NEXT_TRACK},
        {Key::KEY_HOMEPAGE, KeyCodes::GK_BROWSER_HOME},
        {Key::KEY_FAVORITES, KeyCodes::GK_BROWSER_FAVORITES},
        {Key::KEY_SEARCH, KeyCodes::GK_BROWSER_SEARCH},
        {Key::KEY_STANDBY, KeyCodes::GK_SLEEP},
        {Key::KEY_LAUNCHMAIL, KeyCodes::GK_MEDIA_LAUNCH_MAIL},
        {Key::KEY_LAUNCHMEDIA, KeyCodes::GK_MEDIA_LAUNCH_MEDIA_SELECT},
        {Key::KEY_LAUNCH0, KeyCodes::GK_MEDIA_LAUNCH_APP1},
        {Key::KEY_LAUNCH1, KeyCodes::GK_MEDIA_LAUNCH_APP2},
        {Key::KEY_SPACE, KeyCodes::GK_SPACE},
        {Key::KEY_EXCLAM, KeyCodes::GK_OEM_1},
        {Key::KEY_QUOTEDBL, KeyCodes::GK_OEM_7},
        {Key::KEY_NUMBERSIGN, KeyCodes::GK_OEM_3},
        {Key::KEY_DOLLAR, KeyCodes::GK_OEM_4},
        {Key::KEY_PERCENT, KeyCodes::GK_OEM_5},
        {Key::KEY_AMPERSAND, KeyCodes::GK_OEM_6},
        {Key::KEY_APOSTROPHE, KeyCodes::GK_OEM_7},
        {Key::KEY_PARENLEFT, KeyCodes::GK_OEM_4},
        {Key::KEY_PARENRIGHT, KeyCodes::GK_OEM_6},
        {Key::KEY_ASTERISK, KeyCodes::GK_OEM_2},
        {Key::KEY_PLUS, KeyCodes::GK_OEM_PLUS},
        {Key::KEY_COMMA, KeyCodes::GK_OEM_COMMA},
        {Key::KEY_MINUS, KeyCodes::GK_OEM_MINUS},
        {Key::KEY_PERIOD, KeyCodes::GK_OEM_PERIOD},
        {Key::KEY_SLASH, KeyCodes::GK_OEM_2},
        {Key::KEY_0, KeyCodes::GK_0},
        {Key::KEY_1, KeyCodes::GK_1},
        {Key::KEY_2, KeyCodes::GK_2},
        {Key::KEY_3, KeyCodes::GK_3},
        {Key::KEY_4, KeyCodes::GK_4},
        {Key::KEY_5, KeyCodes::GK_5},
        {Key::KEY_6, KeyCodes::GK_6},
        {Key::KEY_7, KeyCodes::GK_7},
        {Key::KEY_8, KeyCodes::GK_8},
        {Key::KEY_9, KeyCodes::GK_9},
        {Key::KEY_COLON, KeyCodes::GK_OEM_1},
        {Key::KEY_SEMICOLON, KeyCodes::GK_OEM_1},
        {Key::KEY_LESS, KeyCodes::GK_OEM_COMMA},
        {Key::KEY_EQUAL, KeyCodes::GK_OEM_PLUS},
        {Key::KEY_GREATER, KeyCodes::GK_OEM_PERIOD},
        {Key::KEY_QUESTION, KeyCodes::GK_OEM_2},
        {Key::KEY_AT, KeyCodes::GK_OEM_3},
        {Key::KEY_A, KeyCodes::GK_A},
        {Key::KEY_B, KeyCodes::GK_B},
        {Key::KEY_C, KeyCodes::GK_C},
        {Key::KEY_D, KeyCodes::GK_D},
        {Key::KEY_E, KeyCodes::GK_E},
        {Key::KEY_F, KeyCodes::GK_F},
        {Key::KEY_G, KeyCodes::GK_G},
        {Key::KEY_H, KeyCodes::GK_H},
        {Key::KEY_I, KeyCodes::GK_I},
        {Key::KEY_J, KeyCodes::GK_J},
        {Key::KEY_K, KeyCodes::GK_K},
        {Key::KEY_L, KeyCodes::GK_L},
        {Key::KEY_M, KeyCodes::GK_M},
        {Key::KEY_N, KeyCodes::GK_N},
        {Key::KEY_O, KeyCodes::GK_O},
        {Key::KEY_P, KeyCodes::GK_P},
        {Key::KEY_Q, KeyCodes::GK_Q},
        {Key::KEY_R, KeyCodes::GK_R},
        {Key::KEY_S, KeyCodes::GK_S},
        {Key::KEY_T, KeyCodes::GK_T},
        {Key::KEY_U, KeyCodes::GK_U},
        {Key::KEY_V, KeyCodes::GK_V},
        {Key::KEY_W, KeyCodes::GK_W},
        {Key::KEY_X, KeyCodes::GK_X},
        {Key::KEY_Y, KeyCodes::GK_Y},
        {Key::KEY_Z, KeyCodes::GK_Z},
        {Key::KEY_BRACKETLEFT, KeyCodes::GK_OEM_4},
        {Key::KEY_BACKSLASH, KeyCodes::GK_OEM_5},
        {Key::KEY_BRACKETRIGHT, KeyCodes::GK_OEM_6},
        {Key::KEY_ASCIICIRCUM, KeyCodes::GK_OEM_5},
        {Key::KEY_UNDERSCORE, KeyCodes::GK_OEM_MINUS},
        {Key::KEY_QUOTELEFT, KeyCodes::GK_OEM_3},
        {Key::KEY_BRACELEFT, KeyCodes::GK_OEM_4},
        {Key::KEY_BAR, KeyCodes::GK_OEM_5},
        {Key::KEY_BRACERIGHT, KeyCodes::GK_OEM_6},
        {Key::KEY_ASCIITILDE, KeyCodes::GK_OEM_3},
    };

    class ViewRect : public TextureRect {
        GDCLASS(ViewRect, TextureRect);

        private:
            enum Event { mouse, key, scroll };
            std::queue<Event> events;
            std::queue<MouseEvent> mouse_events;
            std::queue<KeyEvent> key_events;
            std::queue<ScrollEvent> scroll_events;

            RefPtr<View> view; // Ultralight view
            Ref<ImageTexture> image_texture;
            Ref<Image> image;

            void HandleMouseButton(InputEventMouseButton* event);
            void HandleMouseMotion(InputEventMouseMotion* event);
            void HandleKey(InputEventKey* event);

            void RenderFrame();
            void SizeChanged();

            void CopyBitmapToTexture(RefPtr<Bitmap> bitmap);

        protected:
            static void _bind_methods();

        public:
            ViewRect();
            ~ViewRect();

            void SetView(RefPtr<View> p_view);
            RefPtr<View> GetView();

            void _process(double delta) override;
            void _gui_input(const Ref<InputEvent> &event) override;
    };

}

#endif
