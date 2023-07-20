#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace glkit
{
   class Camera
   {
   public:
      const static float YAW = -90.0f;
      const static float PITCH = 0.0f;
      const static float SPEED = 2.5f;
      const static float SENSITIVITY = 0.20f;
      const static float ZOOM = 1.0f;
      enum CAMERA_MOVEMENT
      {
         FORWARD,
         BACKWARD,
         LEFT,
         RIGHT
      };

   private:
      glm::vec3 mPosition;
      glm::vec3 mFront;
      glm::vec3 mUp;
      glm::vec3 mRight;
      glm::vec3 mWorldUp;

      float mYaw;
      float mPitch;
      float mSpeed;
      float mSensitivity;
      float mZoom;

      void updateCameraVectors();

   public:
      Camera(
          glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
          glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
          float yaw = YAW,
          float pitch = PITCH);

      glm::vec3 getPosition();

      float getZoom();

      glm::mat4 getViewMatrix();

      void processKeyBoard(CAMERA_MOVEMENT direction, float deltaTime);

      void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

      void processMouseScroll(float yoffset);

      ~Camera();
   };

};
#endif