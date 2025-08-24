#ifndef GFILESYSTEM_H
#define GFILESYSTEM_H

#include <godot_cpp/classes/file_access.hpp>

#include "MimeTypes.hpp"

namespace ultralight
{
    class GodotFileSystem : public FileSystem
    {
    public:
        GodotFileSystem() {}
        ~GodotFileSystem() override {}

        godot::String AdjustPath(const godot::String& file_path)
        {
            if(file_path.begins_with("inspector/"))
            {
                return "addons/gdhtml/" + file_path;
            }
            return file_path;
        }

        godot::String SetupPath(const godot::String& file_path)
        {
            return "res://" + AdjustPath(file_path);
        }

        bool IsImageSource(const godot::String& file_path)
        {
            return file_path.ends_with(".imgsrc");
        }

        bool ImageSourceExists(const godot::String& file_path)
        {
            return false;
        }

        bool FileExists(const String& file_path) override
        {
            auto gd_file_path = godot::String(file_path.utf8().data());
            if(IsImageSource(gd_file_path))
            {
                return ImageSourceExists(gd_file_path);
            }
            return godot::FileAccess::file_exists(SetupPath(gd_file_path));
        }

        String GetFileCharset(const String& file_path) override
        {
            return "utf-8";
        }

        String GetFileMimeType(const String& file_path) override
        {
            return MimeTypes::getType(file_path.utf8().data());
        }

        RefPtr<Buffer> OpenFile(const String& file_path) override
        {
            auto gd_file_path = godot::String(file_path.utf8().data());
            godot::PackedByteArray data = godot::FileAccess::get_file_as_bytes(SetupPath(gd_file_path));
            return Buffer::CreateFromCopy(data.ptr(), data.size());
        }
    };
};

#endif
