#ifndef GCLIPBOARD_H
#define GCLIPBOARD_H

#include <Ultralight/Ultralight.h>
#include <AppCore/Platform.h>
#include <JavaScriptCore/JavaScript.h>

#include <godot_cpp/classes/display_server.hpp>

namespace ultralight
{
    class GodotClipboard : public Clipboard
    {
    public:
        GodotClipboard() {}
        ~GodotClipboard() override {}

        void Clear() override
        {
            godot::DisplayServer::get_singleton()->clipboard_set("");
        }

        String ReadPlainText() override
        {
            return String(godot::DisplayServer::get_singleton()->clipboard_get().utf8().get_data());
        }

        void WritePlainText(const String& text) override
        {
            godot::DisplayServer::get_singleton()->clipboard_set(godot::String(text.utf8().data()));
        }
    };
};

#endif
