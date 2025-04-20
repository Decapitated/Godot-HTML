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

        godot::String AdjustPath(godot::String file_path)
        {
            if(file_path.begins_with("inspector/"))
            {
                file_path = "addons/gdhtml/" + file_path;
            }
            return file_path;
        }

        String SetupPath(const String& file_path)
        {
            auto adjusted_file_path = String(
                AdjustPath(
                    godot::String(file_path.utf8().data())
                ).utf8().get_data()
            );
            return "res://" + adjusted_file_path;
        }

        bool FileExists(const String& file_path) override
        {
            return godot::FileAccess::file_exists(SetupPath(file_path).utf8().data());
        }

        String GetFileCharset(const String& file_path) override
        {
            return "utf-8";
        }

        String GetFileMimeType(const String& file_path) override
        {
            return MimeTypes::getType(SetupPath(file_path).utf8().data());
        }

        RefPtr<Buffer> OpenFile(const String& file_path) override
        {
            godot::PackedByteArray data = godot::FileAccess::get_file_as_bytes(SetupPath(file_path).utf8().data());
            return Buffer::CreateFromCopy(data.ptr(), data.size());
        }
    };
};

#endif
