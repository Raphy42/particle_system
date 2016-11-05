//
// Created by Raphaël Dantzer on 05/11/16.
//

#ifndef PARTICLE_SYSTEM_FPSCAMERA_H
#define PARTICLE_SYSTEM_FPSCAMERA_H


#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <OpenGL/OpenGL.h>
#include <cmath>
#include <glm/glm.hpp>

#define YAW         (-90.0f)
#define PITCH       (0.0f)
#define SPEED       (3.0f)
#define SENSITIVITY (0.25f)
#define ZOOM        (45.0f)

class FPSCamera {
public:
    enum class e_CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    FPSCamera();
    FPSCamera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch);
    ~FPSCamera();

    glm::mat4       getViewMat4() const;

    void            keyboardEvent(e_CameraMovement movement, GLfloat delta);
    void            mouseEvent(GLfloat x, GLfloat y, bool constraint = true);
    void            mouseScrollEvent(GLfloat y);

protected:
    void            update();

private:
    glm::vec3       _position;
    glm::vec3       _front;
    glm::vec3       _up;
    glm::vec3       _right;
    glm::vec3       _worldUp;

    GLfloat         _yaw;
    GLfloat         _pitch;

    GLfloat         _movementSpeed;
    GLfloat         _mouseSensitivity;
    GLfloat         _zoom;
};


#endif //PARTICLE_SYSTEM_FPSCAMERA_H
