#ifndef GFILESYSTEM_H
#define GFILESYSTEM_H

#include <Ultralight/Ultralight.h>
#include <AppCore/Platform.h>
#include <JavaScriptCore/JavaScript.h>

#include <godot_cpp/classes/file_access.hpp>

#include "MimeTypes.h"

using namespace ultralight;

class GodotFileSystem : public FileSystem
{
public:
    GodotFileSystem() {}
    ~GodotFileSystem() override {}

    ultralight::String SetupPath(const ultralight::String& file_path)
    {
        return ("res://"+file_path);
    }

    bool FileExists(const ultralight::String& file_path) override
    {
        return godot::FileAccess::file_exists(SetupPath(file_path).utf8().data());
    }

    ultralight::String GetFileCharset(const ultralight::String& file_path) override
    {
        return "utf-8";
    }

    ultralight::String GetFileMimeType(const ultralight::String& file_path) override
    {
        return MimeTypes::getType(SetupPath(file_path).utf8().data());
    }

    RefPtr<Buffer> OpenFile(const ultralight::String& file_path) override
    {
        godot::PackedByteArray data = godot::FileAccess::get_file_as_bytes(SetupPath(file_path).utf8().data());
        return Buffer::CreateFromCopy(data.ptr(), data.size());
    }

};

#endif
