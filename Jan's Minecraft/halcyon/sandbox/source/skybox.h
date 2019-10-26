// skybox.h

#ifndef SKYBOX_H_INCLUDED
#define SKYBOX_H_INCLUDED

#include <halcyon.h>
#include "camera.h"

struct skybox
{
   skybox(render_system &render_sys);
   ~skybox();

   bool init();
   void exit();

   void render(const camera &cam);

   render_system *render_sys_;
   shader_program program_;
   texture cubemap_;
   sampler_state sampler_;
   vertex_buffer vertex_buffer_;
   vertex_format vertex_format_;
};

#endif // SKYBOX_H_INCLUDED