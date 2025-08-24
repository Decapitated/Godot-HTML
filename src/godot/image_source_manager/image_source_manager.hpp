#ifndef IMAGESOURCEMANAGER_H
#define IMAGESOURCEMANAGER_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

#include <Ultralight/Ultralight.h>

namespace godot {
    class ImageSourceManager : public Object {
	    GDCLASS(ImageSourceManager, Object);
    
        private:
            static ImageSourceManager* singleton;

        protected:
            static void _bind_methods();

        public:
            ImageSourceManager();
            ~ImageSourceManager();

            static ImageSourceManager *get_singleton();
            
            bool IsImageSource(const String& file_path) const;
			bool ImageSourceExists(const String& file_path) const;
            PackedByteArray GetImageSource(const String& file_path) const;
    };
}

#endif