#ifndef UTILITIES_H
#define UTILITIES_H

#include <Ultralight/Ultralight.h>

#include <godot_cpp/variant/packed_byte_array.hpp>

using namespace ultralight;
using namespace godot;

namespace utilities
{

static PackedByteArray BitmapToPackedByteArray(RefPtr<Bitmap> bitmap)
{
    bitmap->SwapRedBlueChannels();
    auto pixels = bitmap->LockPixelsSafe();
    PackedByteArray texture_data = PackedByteArray();
    texture_data.resize(pixels.size());
    memcpy(texture_data.ptrw(), pixels.data(), pixels.size());
    return texture_data;
}

}

#endif