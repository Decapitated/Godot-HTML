#include "godot_gpu_driver.hpp"

#include "utilities.hpp"

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/rd_texture_format.hpp>
#include <godot_cpp/classes/rd_texture_view.hpp>
#include <godot_cpp/classes/rd_attachment_format.hpp>

using namespace ultralight;
using namespace godot;

GodotGPUDriver::GodotGPUDriver()
{
    auto rs = RenderingServer::get_singleton();
    auto _rd = rs->get_rendering_device();
    rd = _rd->create_local_device();
}

GodotGPUDriver::~GodotGPUDriver()
{
    // Cleanup all resources
    for (const auto& pair : textures)
    {
        rd->free_rid(pair.second);
    }

    for (const auto& pair : render_buffers) {
        const auto& rb_data = pair.second;
        rd->free_rid(rb_data.texture);
        rd->free_rid(rb_data.framebuffer);
    }

    for (const auto& pair : geometries)
    {
        rd->free_rid(pair.second.first);  // vertex buffer
        rd->free_rid(pair.second.second); // index buffer
    }

    // Free the RenderingDevice itself
    if (rd) {
        memdelete(rd);
        rd = nullptr;
    }
}

void GodotGPUDriver::BeginSynchronize()
{
    rd->sync();
}

void GodotGPUDriver::EndSynchronize()
{
    rd->submit();
}

uint32_t GodotGPUDriver::NextTextureId()
{
    return next_texture_id++;
}

#pragma region Texture

void GodotGPUDriver::CreateTexture(uint32_t texture_id, RefPtr<Bitmap> bitmap)
{
    Ref<RDTextureFormat> format = memnew(RDTextureFormat);
    format->set_width(bitmap->width());
    format->set_height(bitmap->height());
    format->set_depth(1);
    format->set_array_layers(1);
    format->set_mipmaps(1);
    format->set_format(RenderingDevice::DATA_FORMAT_R8G8B8A8_UNORM);
    format->set_samples(RenderingDevice::TEXTURE_SAMPLES_1);
    format->set_usage_bits(RenderingDevice::TEXTURE_USAGE_SAMPLING_BIT | 
                          RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT);

    Ref<RDTextureView> view = memnew(RDTextureView);
    view->set_swizzle_r(RenderingDevice::TEXTURE_SWIZZLE_R);
    view->set_swizzle_g(RenderingDevice::TEXTURE_SWIZZLE_G);
    view->set_swizzle_b(RenderingDevice::TEXTURE_SWIZZLE_B);
    view->set_swizzle_a(RenderingDevice::TEXTURE_SWIZZLE_A);

    PackedByteArray texture_data = utilities::BitmapToPackedByteArray(bitmap);

    TypedArray<PackedByteArray> typed_texture_data;
    typed_texture_data.push_back(texture_data);

    RID texture = rd->texture_create(format, view, typed_texture_data);
    textures[texture_id] = texture;
}

void GodotGPUDriver::UpdateTexture(uint32_t texture_id, RefPtr<Bitmap> bitmap)
{
    if (textures.find(texture_id) == textures.end()) return;

    bitmap->SwapRedBlueChannels();
    auto pixels = bitmap->LockPixelsSafe();
    PackedByteArray texture_data = PackedByteArray();
    texture_data.resize(pixels.size());
    memcpy(texture_data.ptrw(), pixels.data(), pixels.size());
    
    rd->texture_update(textures[texture_id], 0, texture_data);
}

void GodotGPUDriver::DestroyTexture(uint32_t texture_id)
{
    if (textures.find(texture_id) != textures.end()) {
        rd->free_rid(textures[texture_id]);
        textures.erase(texture_id);
    }
}

#pragma endregion // Texture

#pragma region RenderBuffer

uint32_t GodotGPUDriver::NextRenderBufferId()
{
    return next_buffer_id++;
}

void GodotGPUDriver::CreateRenderBuffer(uint32_t render_buffer_id, const RenderBuffer& buffer)
{
    // Create the color attachment texture
    Ref<RDTextureFormat> format = memnew(RDTextureFormat);
    format->set_width(buffer.width);
    format->set_height(buffer.height);
    format->set_depth(1);
    format->set_array_layers(1);
    format->set_mipmaps(1);
    format->set_format(RenderingDevice::DATA_FORMAT_R8G8B8A8_UNORM);
    format->set_samples(RenderingDevice::TEXTURE_SAMPLES_1);
    format->set_usage_bits(RenderingDevice::TEXTURE_USAGE_COLOR_ATTACHMENT_BIT |
                           RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT |
                           RenderingDevice::TEXTURE_USAGE_CAN_COPY_FROM_BIT);

    Ref<RDTextureView> view = memnew(RDTextureView);
    RID color_texture = rd->texture_create(format, view);

    // Create attachment description
    TypedArray<RDAttachmentFormat> attachments;
    Ref<RDAttachmentFormat> color_attachment = memnew(RDAttachmentFormat);
    color_attachment->set_format(RenderingDevice::DATA_FORMAT_R8G8B8A8_UNORM);
    color_attachment->set_samples(RenderingDevice::TEXTURE_SAMPLES_1);
    color_attachment->set_usage_flags(RenderingDevice::TEXTURE_USAGE_COLOR_ATTACHMENT_BIT);
    attachments.push_back(color_attachment);

    // Create framebuffer format
    int64_t fb_format = rd->framebuffer_format_create(attachments);

    // Create the framebuffer with the color attachment
    TypedArray<RID> textures;
    textures.push_back(color_texture);
    RID framebuffer = rd->framebuffer_create(textures, fb_format);

    // Store both the texture and framebuffer
    render_buffers[render_buffer_id] = {color_texture, framebuffer, fb_format};
}

void GodotGPUDriver::DestroyRenderBuffer(uint32_t render_buffer_id)
{
    if (render_buffers.find(render_buffer_id) != render_buffers.end()) {
        const auto& data = render_buffers[render_buffer_id];
        rd->free_rid(data.texture);
        rd->free_rid(data.framebuffer);
        render_buffers.erase(render_buffer_id);
    }
}

#pragma endregion // RenderBuffer

#pragma region Geometry

uint32_t GodotGPUDriver::NextGeometryId()
{
    return next_geometry_id++;
}

void GodotGPUDriver::CreateGeometry(uint32_t geometry_id, const VertexBuffer& vertices, const IndexBuffer& indices)
{
    // Create vertex buffer
    PackedByteArray vertex_data;
    vertex_data.resize(vertices.size);
    memcpy(vertex_data.ptrw(), vertices.data, vertices.size);
    
    RID vertex_buffer = rd->vertex_buffer_create(vertices.size, vertex_data);

    // Create index buffer
    PackedByteArray index_data;
    index_data.resize(indices.size);
    memcpy(index_data.ptrw(), indices.data, indices.size);
    
    RID index_buffer = rd->index_buffer_create(
        indices.size,
        RenderingDevice::INDEX_BUFFER_FORMAT_UINT32,
        index_data
    );

    geometries[geometry_id] = std::make_pair(vertex_buffer, index_buffer);
}

void GodotGPUDriver::UpdateGeometry(uint32_t geometry_id, const VertexBuffer& vertices, const IndexBuffer& indices)
{
    if (geometries.find(geometry_id) == geometries.end()) return;

    // Update vertex buffer
    PackedByteArray vertex_data;
    vertex_data.resize(vertices.size);
    memcpy(vertex_data.ptrw(), vertices.data, vertices.size);
    
    rd->buffer_update(geometries[geometry_id].first, 0, vertices.size, vertex_data);

    // Update index buffer
    PackedByteArray index_data;
    index_data.resize(indices.size);
    memcpy(index_data.ptrw(), indices.data, indices.size);
    
    rd->buffer_update(geometries[geometry_id].second, 0, indices.size, index_data);
}

void GodotGPUDriver::DestroyGeometry(uint32_t geometry_id)
{
    if (geometries.find(geometry_id) != geometries.end()) {
        rd->free_rid(geometries[geometry_id].first);  // Free vertex buffer
        rd->free_rid(geometries[geometry_id].second); // Free index buffer
        geometries.erase(geometry_id);
    }
}

#pragma endregion // Geometry

void GodotGPUDriver::UpdateCommandList(const CommandList& list)
{
    int draw_list = -1;
    for(int i = 0; i < list.size; i++)
    {
        const Command& cmd = list.commands[i];
        switch (cmd.command_type) {
            case CommandType::ClearRenderBuffer:
                {
                    // End current draw list if one exists
                    if (draw_list != -1) {
                        rd->draw_list_end();
                    }
                    
                    // Start new draw list with clear
                    // Note: We'll need the current render buffer ID from somewhere - 
                    // either stored in GPUState or tracked separately
                    const auto& rb_data = render_buffers[cmd.gpu_state.render_buffer_id];
                    PackedColorArray clear_colors;
                    // Note: Clear color would need to come from GPUState
                    clear_colors.push_back(Color(0,0,0,1)); // Default black clear
                    
                    draw_list = rd->draw_list_begin(
                        rb_data.framebuffer,
                        RenderingDevice::INITIAL_ACTION_CLEAR,
                        RenderingDevice::FINAL_ACTION_READ,
                        RenderingDevice::INITIAL_ACTION_CLEAR,
                        RenderingDevice::FINAL_ACTION_READ,
                        clear_colors
                    );
                }
                break;

            case CommandType::DrawGeometry:
                {
                    if (geometries.find(cmd.geometry_id) != geometries.end()) {
                        auto geometry = geometries[cmd.geometry_id];
                        rd->draw_list_bind_vertex_array(draw_list, geometry.first);
                        rd->draw_list_bind_index_array(draw_list, geometry.second);
                        // Use the actual indices count and offset from the command
                        rd->draw_list_draw(draw_list, true, 1, cmd.indices_count);
                    }
                }
                break;
        }
    }

    if (draw_list != -1) {
        rd->draw_list_end();
    }
}
