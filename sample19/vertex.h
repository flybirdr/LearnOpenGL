#ifndef VERTEX_H
#define VERTEX_H
#include <glm/glm.hpp>

struct Vertex
{
   glm::vec3 postion;
   glm::vec3 normal;
   glm::vec2 texCoords;
};

#endif