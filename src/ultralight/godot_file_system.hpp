#ifndef GFILESYSTEM_H
#define GFILESYSTEM_H

#include <godot_cpp/classes/file_access.hpp>

#include "MimeTypes.hpp"
#include "godot/image_source_manager/image_source_manager.hpp"

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

        bool FileExists(const String& file_path) override
        {
            auto gd_file_path = godot::String(file_path.utf8().data());
            const auto img_src_manager = godot::ImageSourceManager::get_singleton();
            if(img_src_manager->IsImageSource(gd_file_path))
            {
                return img_src_manager->ImageSourceExists(gd_file_path);
            }
            return godot::FileAccess::file_exists(SetupPath(gd_file_path));
        }

        String GetFileCharset(const String& file_path) override
        {
            return "utf-8";
        }

        String GetFileMimeType(const String& file_path) override
        {
            const auto img_src_manager = godot::ImageSourceManager::get_singleton();
            if(img_src_manager->IsImageSource(file_path.utf8().data()))
            {
                return "text/plain";
            }
            return MimeTypes::getType(file_path.utf8().data());
        }

        RefPtr<Buffer> OpenFile(const String& file_path) override
        {
            auto gd_file_path = godot::String(file_path.utf8().data());
            godot::PackedByteArray data;

            const auto img_src_manager = godot::ImageSourceManager::get_singleton();
            if(img_src_manager->IsImageSource(gd_file_path))
            {
                data = img_src_manager->GetImageSource(gd_file_path);
            }
            else
            {
                data = godot::FileAccess::get_file_as_bytes(SetupPath(gd_file_path));
            }
            return Buffer::CreateFromCopy(data.ptr(), data.size());
        }
    };
};

#endif
