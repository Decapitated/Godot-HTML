#include "image_source_manager.hpp"

using namespace godot;

ImageSourceManager* ImageSourceManager::singleton = nullptr;

void ImageSourceManager::_bind_methods()
{
    
}

ImageSourceManager::ImageSourceManager() {
    singleton = this;
};

ImageSourceManager::~ImageSourceManager() {
    singleton = nullptr;
}

ImageSourceManager *ImageSourceManager::get_singleton()
{
    return singleton;
}

bool ImageSourceManager::IsImageSource(const godot::String &file_path) const
{
    return file_path.ends_with(".imgsrc");
}

// ALERT: Check if image source resource exists virtually.
bool ImageSourceManager::ImageSourceExists(const godot::String &file_path) const
{
    return false;
}

PackedByteArray ImageSourceManager::GetImageSource(const godot::String& file_path) const
{
    return PackedByteArray();
}
