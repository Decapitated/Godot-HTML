#ifndef UMANAGER_H
#define UMANAGER_H

#include <Ultralight/Ultralight.h>
#include <AppCore/Platform.h>
#include <JavaScriptCore/JavaScript.h>

#include "godot_file_system.h"

namespace GodotHTML {
    class UltralightManager {
        private:
            static UltralightManager* singleton;

            RefPtr<Renderer> renderer; // Ultralight renderer
            GodotFileSystem* file_system = nullptr;

            int32_t frame_count = 0;

            void InitPlatform();
            void CreateRenderer();

        public:
            UltralightManager();
            ~UltralightManager();

            static UltralightManager* GetSingleton();

            RefPtr<Renderer> GetRenderer();

            void UpdateLogic();
            void RenderFrame();
    };
}

#endif