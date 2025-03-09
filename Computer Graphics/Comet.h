#ifndef COMET_H
#define COMET_H

#include "Shader.h"
#include "Sphere.h"
#include <glm.hpp>
#include <string>

class Comet {
public:
    //  онструктор принимает начальную позицию, скорость и врем€ жизни (в секундах)
    Comet(const glm::vec3& position, const glm::vec3& velocity, float lifetime);
    ~Comet();

    // ќбновление состо€ни€ кометы (dt Ц врем€ между кадрами)
    void update(float dt);
    // ќтрисовка кометы
    void draw(Shader& shader);
    // ѕроверка пора ли удалить комету
    bool isDead(float boundaryRadius) const;

private:
    glm::vec3 position;
    glm::vec3 velocity;
    float lifetime; // оставшеес€ врем€ жизни

    Sphere* sphere; // Ѕудем использовать сферу дл€ кометы
};

#endif