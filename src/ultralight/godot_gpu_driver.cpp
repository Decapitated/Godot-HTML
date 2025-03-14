#include <godot_gpu_driver.hpp>

#include <godot_cpp/classes/rendering_server.hpp>

using namespace ultralight;

GodotGPUDriver::GodotGPUDriver():
    rd(godot::RenderingServer::get_singleton()->create_local_rendering_device())
{}

GodotGPUDriver::~GodotGPUDriver()
{
}

void GodotGPUDriver::BeginSynchronize()
{
}

void GodotGPUDriver::EndSynchronize()
{
}

uint32_t GodotGPUDriver::NextTextureId()
{
	return 0;
}

void GodotGPUDriver::CreateTexture(uint32_t texture_id, RefPtr<Bitmap> bitmap)
{
}

void GodotGPUDriver::UpdateTexture(uint32_t texture_id, RefPtr<Bitmap> bitmap)
{
}

void GodotGPUDriver::DestroyTexture(uint32_t texture_id)
{
}

uint32_t GodotGPUDriver::NextRenderBufferId()
{
	return 0;
}

void GodotGPUDriver::CreateRenderBuffer(uint32_t render_buffer_id, const RenderBuffer &buffer)
{
}

void GodotGPUDriver::DestroyRenderBuffer(uint32_t render_buffer_id)
{
}

uint32_t GodotGPUDriver::NextGeometryId()
{
	return 0;
}

void GodotGPUDriver::CreateGeometry(uint32_t geometry_id, const VertexBuffer &vertices, const IndexBuffer &indices)
{
}

void GodotGPUDriver::UpdateGeometry(uint32_t geometry_id, const VertexBuffer &vertices, const IndexBuffer &indices)
{
}

void GodotGPUDriver::DestroyGeometry(uint32_t geometry_id)
{
}

void GodotGPUDriver::UpdateCommandList(const CommandList &list)
{
}
