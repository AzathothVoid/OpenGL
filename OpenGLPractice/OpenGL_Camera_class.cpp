#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"

#include <vector>

glm::mat4 lookAt(glm::vec3 Position, glm::vec3 Direction, glm::vec3 Up) {
    glm::vec3 CForward = -glm::normalize(Position - Direction);
    glm::vec3 CRight = glm::normalize(glm::cross(glm::normalize(Up), CForward));
    glm::vec3 CUp = glm::normalize(glm::cross(CForward, CRight));

    glm::mat4 translation = glm::mat4(1.0f);
    translation[3][0] = Position.x;
    translation[3][1] = Position.y;
    translation[3][2] = Position.z;

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = CRight.x;
    rotation[1][0] = CRight.y;
    rotation[2][0] = CRight.z;

    rotation[0][1] = CUp.x;
    rotation[1][1] = CUp.y;
    rotation[2][1] = CUp.z;

    rotation[0][2] = CForward.x;
    rotation[1][2] = CForward.y;
    rotation[2][2] = CForward.z;

    return rotation * translation;
}

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

//default Camera Values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float SPEED = 5.0f;

class Camera {
public:
    //camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 Up;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;

        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;

        /*Position.y = 0.0f;*/
    }
    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean ConstrainPitch = true) {
        xOffset *= MouseSensitivity;
        yOffset *= MouseSensitivity;

        Yaw += xOffset;
        Pitch += yOffset;

        if (ConstrainPitch) {
            if (Pitch > 89.0)
                Pitch = 89.0;
            if (Pitch < -89.0)
                Pitch = -89.0;
        }

        updateCameraVectors();
    }
    void ProcessMouseScroll(float yOffset) {
        Zoom -= (float)yOffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

#endif