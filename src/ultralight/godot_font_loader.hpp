#ifndef GFONTLOADER_H
#define GFONTLOADER_H

#include <Ultralight/Ultralight.h>

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace ultralight
{
    class GodotFontLoader : public FontLoader
    {
    public:
        GodotFontLoader() {}
        ~GodotFontLoader() override {}

        String fallback_font() const override
        {
            auto system_fonts = godot::OS::get_singleton()->get_system_fonts();
            CRASH_COND_MSG(system_fonts.size() == 0, "No system fonts to fallback on.");
            return String(system_fonts[0].utf8().get_data());
        }

        String fallback_font_for_characters(const String &characters, int weight, bool italic) const override
        {
            return fallback_font();
        }

        RefPtr<FontFile> Load(const String &family, int weight, bool italic) override
        {
            auto font_family = godot::String(family.utf8().data());
            auto font_path = godot::OS::get_singleton()->get_system_font_path(font_family, weight, 100, italic);
            auto font_file = FontFile::Create(String(font_path.utf8().get_data()));
            return font_file;
        } 

    };
};

#endif
