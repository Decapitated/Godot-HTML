#ifndef GGPUDRIVER_H
#define GGPUDRIVER_H

#include <Ultralight/Ultralight.h>
#include <Ultralight/platform/GPUDriver.h>

#include <godot_cpp/classes/rendering_device.hpp>

#include <unordered_map>

namespace ultralight
{
    class GodotGPUDriver : public GPUDriver
    {
    private:
        godot::RenderingDevice* rd;

        struct RenderBufferData {
            RID texture;
            RID framebuffer;
            int64_t format;
        };

        // Resource ID mappings
        std::unordered_map<uint32_t, godot::RID> textures;
        std::unordered_map<uint32_t, RenderBufferData> render_buffers;
        std::unordered_map<uint32_t, std::pair<godot::RID, godot::RID>> geometries; // vertex buffer, index buffer
        
        uint32_t next_texture_id = 1;
        uint32_t next_buffer_id = 1;
        uint32_t next_geometry_id = 1;

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