#ifndef CHARACTER_H_H
#define CHARACTER_H_H

#include<glad/glad.h>
#include<glm/glm.hpp>

struct Character
{
   GLuint textureId;
   glm::ivec2 size;
   glm::ivec2 bearing;
   GLuint advance;
};

#endif