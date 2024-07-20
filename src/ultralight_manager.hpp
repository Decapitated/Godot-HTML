#ifndef UMANAGER_H
#define UMANAGER_H

#include <Ultralight/Ultralight.h>
#include <AppCore/Platform.h>
#include <JavaScriptCore/JavaScript.h>

#include "godot_file_system.h"

namespace GodotHTML {
    class UManager {
        private:
            static RefPtr<Renderer> renderer; // Ultralight renderer

            GodotFileSystem* file_system = nullptr;

            void InitPlatform();
            void CreateRenderer();

        public:
            UManager();
            ~UManager();

            static RefPtr<Renderer> GetRenderer();

            void UpdateLogic();
            void RenderFrame();
    };
}

#endif