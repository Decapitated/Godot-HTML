#include "ultralight_manager.hpp"

using namespace GodotHTML;

RefPtr<Renderer> UManager::renderer;

UManager::UManager() {
    InitPlatform();
    CreateRenderer();
};

UManager::~UManager() {
    if(file_system != nullptr)
    {
        delete file_system;
        file_system = nullptr;
    }
}

void UManager::InitPlatform()
{
    ultralight::Config config;
    config.resource_path_prefix = "gdhtml/resources/";
    Platform::instance().set_config(config);

    ///
    /// Use the OS's native font loader
    ///
    Platform::instance().set_font_loader(GetPlatformFontLoader());

    file_system = new GodotFileSystem();
    Platform::instance().set_file_system(file_system);

    ///
    /// Use the default logger (writes to a log file)
    ///
    Platform::instance().set_logger(GetDefaultLogger("ultralight.log"));
}

void UManager::CreateRenderer()
{
    ///
    /// Create our Renderer (call this only once per application).
    /// 
    /// The Renderer singleton maintains the lifetime of the library
    /// and is required before creating any Views.
    ///
    /// You should set up the Platform handlers before this.
    ///
    renderer = Renderer::Create();
}

RefPtr<Renderer> UManager::GetRenderer() {
    return renderer;
};

void UManager::UpdateLogic() {
    renderer->Update();
};

void UManager::RenderFrame() {
    renderer->Render();
};