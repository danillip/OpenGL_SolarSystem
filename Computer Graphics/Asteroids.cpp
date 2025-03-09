#include "Asteroids.h"
#include <gtc/matrix_transform.hpp>
#include <cstdlib>

Asteroids::Asteroids() : asteroidSphere(nullptr) {
}

Asteroids::~Asteroids() {
    if (asteroidSphere)
        delete asteroidSphere;
}

void Asteroids::init() {
    int numAsteroids = 100;
    float innerRadius = 250.0f;
    float outerRadius = 300.0f;
    for (int i = 0; i < numAsteroids; i++) {
        float angle = ((float)i / numAsteroids) * 360.0f;
        float radius = innerRadius + static_cast <float>(rand()) / (static_cast <float>(RAND_MAX / (outerRadius - innerRadius)));
        float x = radius * cos(glm::radians(angle));
        float z = radius * sin(glm::radians(angle));
        float y = (rand() % 20 - 10);
        positions.push_back(glm::vec3(x, y, z));
    }
    asteroidSphere = new Sphere(0.5f, 18, 9);
}

void Asteroids::draw(Shader& shader, float simulationTime) {
    shader.setInt("useTexture", 0); // Отключаем текстуры астероидам
    for (auto& pos : positions) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        float angle = simulationTime * 20.0f;  // Вращение астероидов
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        shader.setVec3("objectColor", glm::vec3(0.6f, 0.6f, 0.6f));  // Серый цвет
        asteroidSphere->draw();
    }
}