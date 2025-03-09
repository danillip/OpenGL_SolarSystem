#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// Возможные направления движения камеры
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Начальные параметры камеры
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 50.0f;     // базовая скорость
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
    // Атрибуты камеры
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Углы Эйлера
    float Yaw;
    float Pitch;
    // Параметры камеры
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Конструктор с векторами
    Camera(glm::vec3 position = glm::vec3(0.0f, 50.0f, 150.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH);

    // Возвращает матрицу вида
    glm::mat4 GetViewMatrix();

    // Обработка ввода с клавиатуры deltaTime – время между кадрами
    // Если accelerate == true скорость умножается (при зажатом SHIFT)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime, bool accelerate = false);

    // Обработка ввода мышью (смещения по x и y)
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // Обработка зума (колёсиком или клавишами)
    void ProcessMouseScroll(float yoffset);

private:
    // Пересчитывает Front, Right, Up в зависимости от текущих углов
    void updateCameraVectors();
};

#endif
