// skybox.cc

#include "skybox.h"
#include "image.h"
#include "render_system_utility.h"

// skybox texture source: 
// - https://garrysmods.org/download/12004/skybox-minecraft (Modified)

skybox::skybox(render_system &render_sys)
   : render_sys_(&render_sys)
{

}

skybox::~skybox()
{
}

bool skybox::init()
{
   if (!load_shader_program(render_sys_, 
                            program_,
                            "assets/skybox/vertex_shader.txt",
                            "assets/skybox/fragment_shader.txt"))
   {
      HC_ASSERT(false);
   }

   const char *filenames[] = 
   {
      "assets/skybox/xpos.png",
      "assets/skybox/xneg.png",
      "assets/skybox/ypos.png",
      "assets/skybox/yneg.png",
      "assets/skybox/zpos.png",
      "assets/skybox/zneg.png",
   };

   image faces[RENDER_SYSTEM_CUBE_MAP_FACE_COUNT];

   // note: load all cube map images
   for (int index = 0; index < RENDER_SYSTEM_CUBE_MAP_FACE_COUNT; index++)
   {
      if (!faces[index].load_from_file(filenames[index]))
      {
         HC_ASSERT(false);
      }
   }

   // note: check that all sizes match
   const int width = faces[0].width_;
   const int height = faces[0].height_;
   for (int index = 1; index < RENDER_SYSTEM_CUBE_MAP_FACE_COUNT; index++)
   {
      if (faces[index].width_ != width || faces[index].height_ != height)
      {
         HC_ASSERT(false);
      }
   }

   // note: create cube map
   const void *data[RENDER_SYSTEM_CUBE_MAP_FACE_COUNT] = {};
   for (int index = 0; index < RENDER_SYSTEM_CUBE_MAP_FACE_COUNT; index++)
   {
      data[index] = faces[index].data_;
   }

   if (!render_sys_->create_cubemap(cubemap_,
                                    RENDER_SYSTEM_TEXTURE_FORMAT_RGBA8,
                                    width,
                                    height,
                                    data))
   {
      HC_ASSERT(false);
   }

   // note: release all cube map faces
   for (int index = 0; index < RENDER_SYSTEM_CUBE_MAP_FACE_COUNT; index++)
   {
      faces[index].release();
   }

   // note: create sampler state
   if (!render_sys_->create_sampler_state(sampler_,
                                          RENDER_SYSTEM_SAMPLER_FILTER_LINEAR,
                                          RENDER_SYSTEM_SAMPLER_ADDRESS_MODE_CLAMP,
                                          RENDER_SYSTEM_SAMPLER_ADDRESS_MODE_CLAMP,
                                          RENDER_SYSTEM_SAMPLER_ADDRESS_MODE_CLAMP))
   {
      HC_ASSERT(false);
   }

   // note: create cubemap vertex buffer
   {
      const float Q = 0.5f;
      const v3 vertices[] =
      {
         // x positive
         {  Q,  Q, -Q },
         {  Q,  Q,  Q },
         {  Q, -Q,  Q },
         {  Q, -Q,  Q },
         {  Q, -Q, -Q },
         {  Q,  Q, -Q },

         // x negative
         { -Q,  Q,  Q },
         { -Q,  Q, -Q },
         { -Q, -Q, -Q },
         { -Q, -Q, -Q },
         { -Q, -Q,  Q },
         { -Q,  Q,  Q },

         // y positive
         { -Q,  Q,  Q },
         {  Q,  Q,  Q },
         {  Q,  Q, -Q },
         {  Q,  Q, -Q },
         { -Q,  Q, -Q },
         { -Q,  Q,  Q },

         // y negative
         { -Q, -Q, -Q },
         {  Q, -Q, -Q },
         {  Q, -Q,  Q },
         {  Q, -Q,  Q },
         { -Q, -Q,  Q },
         { -Q, -Q, -Q },

         // z positive
         { -Q,  Q, -Q },
         {  Q,  Q, -Q },
         {  Q, -Q, -Q },
         {  Q, -Q, -Q },
         { -Q, -Q, -Q },
         { -Q,  Q, -Q },

         // z negative
         {  Q,  Q,  Q },
         { -Q,  Q,  Q },
         { -Q, -Q,  Q },
         { -Q, -Q,  Q },
         {  Q, -Q,  Q },
         {  Q,  Q,  Q },
      };

      if (!render_sys_->create_vertex_buffer(vertex_buffer_,
                                             RENDER_SYSTEM_BUFFER_ACCESS_STATIC,
                                             sizeof(vertices),
                                             vertices))
      {
         HC_ASSERT(false);
      }
   }

   // note: define vertex format
   vertex_format_.add_attribute(vertex_format::ATTRIBUTE_SEMANTIC_POSITION,
                                vertex_format::ATTRIBUTE_FORMAT_FLOAT,
                                3,
                                false);

   return true;
}

void skybox::exit()
{
   render_sys_->destroy_shader(program_);
   render_sys_->destroy_texture(cubemap_);
   render_sys_->destroy_sampler_state(sampler_);
   render_sys_->destroy_vertex_buffer(vertex_buffer_);
}

void skybox::render(const camera &cam)
{
   (void)cam;

   glm::mat4 view(cam.view_);
   view[3][0] = 0.0f;
   view[3][1] = 0.0f;
   view[3][2] = 0.0f;

   // note: pipeline settings
   render_sys_->set_shader_program(program_);
   render_sys_->set_blend_state(false);
   render_sys_->set_depth_state(false, false);
   render_sys_->set_rasterizer_state(RENDER_SYSTEM_CULL_MODE_FRONT);

   // note: shader uniforms
   render_sys_->set_shader_uniform(program_,
                                   RENDER_SYSTEM_UNIFORM_TYPE_MATRIX,
                                   "u_projection",
                                   1,
                                   glm::value_ptr(cam.projection_));
   render_sys_->set_shader_uniform(program_,
                                   RENDER_SYSTEM_UNIFORM_TYPE_MATRIX,
                                   "u_view",
                                   1,
                                   glm::value_ptr(view));

   // note: input assembly
   render_sys_->set_vertex_buffer(vertex_buffer_);
   render_sys_->set_vertex_format(vertex_format_);

   // note: resource bindings
   render_sys_->set_texture(cubemap_, 0, RENDER_SYSTEM_TEXTURE_TARGET_TEXTURE_CUBEMAP);
   render_sys_->set_sampler_state(sampler_, 0);

   // note: draw call
   render_sys_->draw(RENDER_SYSTEM_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                     0,
                     36);
}
