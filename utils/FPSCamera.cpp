//
// Created by Raphaël Dantzer on 05/11/16.
//

#include "FPSCamera.h"

FPSCamera::FPSCamera() :
        _front(glm::vec3(0.f, 0.f, -1.f)),
        _movementSpeed(SPEED),
        _mouseSensitivity(SENSITIVITY),
        _zoom(ZOOM),
        _position(glm::vec3(0.f, 0.f, 0.f)),
        _worldUp(glm::vec3(0.f, 1.f, 0.f)),
        _yaw(YAW),
        _pitch(PITCH)
{
    update();
}

FPSCamera::FPSCamera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) :
        _front(glm::vec3(0.f, 0.f, -1.f)),
        _movementSpeed(SPEED),
        _mouseSensitivity(SENSITIVITY),
        _zoom(ZOOM),
        _position(position),
        _worldUp(up),
        _yaw(yaw),
        _pitch(pitch)
{
    update();
}

FPSCamera::~FPSCamera() {
}

glm::mat4 FPSCamera::getViewMat4() const {
    return glm::lookAt(_position, _position + _front, _up);
}

void FPSCamera::update() {
    glm::vec3 front;

    front.x = cosf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));
    front.y = sinf(glm::radians(_pitch));
    front.z = sinf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));

    _front = glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

void FPSCamera::mouseEvent(GLfloat x, GLfloat y, bool constraint) {
    x *= _mouseSensitivity;
    y *= _mouseSensitivity;

    _yaw += x;
    _pitch += y;

    if (constraint)
        glm::clamp(_pitch, -89.f, 89.f);

    update();
}



void FPSCamera::keyboardEvent(FPSCamera::e_CameraMovement movement, GLfloat delta) {
    const GLfloat velocity = _movementSpeed * delta;

    switch (movement) {
        case FPSCamera::e_CameraMovement::FORWARD :
            _position += _front * velocity;
            break;
        case FPSCamera::e_CameraMovement::BACKWARD :
            _position -= _front * velocity;
            break;
        case FPSCamera::e_CameraMovement::RIGHT :
            _position += _right * velocity;
            break;
        case FPSCamera::e_CameraMovement::LEFT :
            _position -= _right * velocity;
            break;
    }
}

void FPSCamera::mouseScrollEvent(GLfloat y) {
    if (_zoom >= 1.f && _zoom <= 45.f)
        _zoom -= y;
    glm::clamp(_zoom, 1.f, 45.f);
}
