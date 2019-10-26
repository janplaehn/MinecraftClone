// scene.cc

#include "scene.h"

scene::node::node()
   : id_(-1)
   , transform_(glm::mat4(1.0f))
{
}

scene::scene()
   : node_id_generator_(0)
   , node_count_(0)
{
   nodes_.reserve(1024);
}

scene::~scene()
{
   nodes_.clear();
}
