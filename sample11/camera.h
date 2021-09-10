#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

enum CAMERA_MOVEMENT
{
   FORWARD,
   BACKWARD,
   LEFT,
   RIGHT
};
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.4f;
const float ZOOM = 45.0f;

class Camera
{
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

   void updateCameraVectors()
   {
      glm::vec3 front;
      front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
      front.y = sin(glm::radians(mPitch));
      front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
      mFront = glm::normalize(front);
      mRight = glm::normalize(glm::cross(mFront, mWorldUp));
      mUp = glm::normalize(glm::cross(mRight, mFront));
   }

public:
   Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
       : mPosition(position), mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mWorldUp(up), mYaw(yaw), mPitch(pitch),
         mSpeed(SPEED), mSensitivity(SENSITIVITY), mZoom(ZOOM)
   {
   }

   float getZoom(){
      return mZoom;
   }

   glm::mat4 getViewMatrix()
   {
      return glm::lookAt(mPosition, mPosition + mFront, mUp);
   }
   void processKeyBoard(CAMERA_MOVEMENT direction, float deltaTime)
   {
      float distance = mSpeed * deltaTime;
      if (direction == FORWARD)
      {
         mPosition += mFront * distance;
      }
      if (direction == BACKWARD)
      {
         mPosition -= mFront * distance;
      }
      if (direction == LEFT)
      {
         mPosition -= mRight * distance;
      }
      if (direction == RIGHT)
      {
         mPosition += mRight * distance;
      }
   }

   void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
   {
      xoffset *= mSensitivity;
      yoffset *= mSensitivity;
      mYaw += xoffset;
      mPitch += yoffset;
      if (constrainPitch)
      {
         if (mPitch > 89.0f)
            mPitch = 89.0f;
         if (mPitch < -89.0f)
            mPitch = -89.0f;
      }
      updateCameraVectors();
   }

   void processMouseScroll(float yoffset)
   {
      mZoom -= yoffset;
      if (mZoom < 0.2f)
      {
         mZoom = 0.2f;
      }

      if (mZoom > 45.0f)
      {
         mZoom = 45.0f;
      }
   }

   ~Camera();
};

// Camera::Camera(/* args */)
// {
// }

Camera::~Camera()
{
}

#endif