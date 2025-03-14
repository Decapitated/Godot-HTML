#ifndef GGPUDRIVER_H
#define GGPUDRIVER_H

#include <Ultralight/Ultralight.h>
#include <Ultralight/platform/GPUDriver.h>

#include <godot_cpp/classes/rendering_device.hpp>

namespace ultralight
{
    class GodotGPUDriver : public GPUDriver
    {
    private:
        godot::Ref<godot::RenderingDevice> rd;
    public:
        GodotGPUDriver();
        ~GodotGPUDriver();
        void BeginSynchronize() override;
        void EndSynchronize() override;
        uint32_t NextTextureId() override;
        void CreateTexture(uint32_t texture_id, RefPtr<Bitmap> bitmap) override;
        void UpdateTexture(uint32_t texture_id, RefPtr<Bitmap> bitmap) override;
        void DestroyTexture(uint32_t texture_id) override;
        uint32_t NextRenderBufferId() override;
        void CreateRenderBuffer(uint32_t render_buffer_id, const RenderBuffer &buffer) override;
        void DestroyRenderBuffer(uint32_t render_buffer_id) override;
        uint32_t NextGeometryId() override;
        void CreateGeometry(uint32_t geometry_id, const VertexBuffer &vertices, const IndexBuffer &indices) override;
        void UpdateGeometry(uint32_t geometry_id, const VertexBuffer &vertices, const IndexBuffer &indices) override;
        void DestroyGeometry(uint32_t geometry_id) override;
        void UpdateCommandList(const CommandList &list) override;
    };
};

#endif