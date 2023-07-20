#include "Camera.h"

glkit::Camera::Camera(glm::vec3 position,
                      glm::vec3 up,
                      float yaw,
                      float pitch) : mPosition(position),
                                     mFront(glm::vec3(0.0f, 0.0f, -1.0f)),
                                     mWorldUp(up), mYaw(yaw),
                                     mPitch(pitch),
                                     mSpeed(SPEED),
                                     mSensitivity(SENSITIVITY),
                                     mZoom(ZOOM)
{
}

void glkit::Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront = glm::normalize(front);
    mRight = glm::normalize(glm::cross(mFront, mWorldUp));
    mUp = glm::normalize(glm::cross(mRight, mFront));
}
void glkit::Camera::processKeyBoard(CAMERA_MOVEMENT direction, float deltaTime)
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

void glkit::Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
void glkit::Camera::processMouseScroll(float yoffset)
{
    mZoom += yoffset / 10;
    if (mZoom < 0.2f)
    {
        mZoom = 0.2f;
    }

    if (mZoom > 3.0f)
    {
        mZoom = 3.0f;
    }
}

glm::vec3 glkit::Camera::getPosition()
{
    return mPosition;
}

float glkit::Camera::getZoom()
{
    return mZoom;
}

glm::mat4 glkit::Camera::getViewMatrix()
{
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

glkit::Camera::~Camera()
{
}