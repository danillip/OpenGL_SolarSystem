#include "Planet.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>

Planet::Planet(const std::string& name, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed)
    : name(name), radius(radius), orbitRadius(orbitRadius), orbitSpeed(orbitSpeed), rotationSpeed(rotationSpeed),
    isSun(false), sphere(nullptr), model(nullptr)
{
    // Если планета "Sun", "Earth" или "Mars", загружаем модель из соответствующего файла
    if (name == "Sun" || name == "Earth" || name == "Mars") {
        std::string modelPath;
        if (name == "Sun")
            modelPath = "Planets/Sun.obj";
        else if (name == "Earth")
            modelPath = "Planets/Earth/LI4TDM8A874IO8I8N0FH5F6BQ.obj";
        else if (name == "Mars")
            modelPath = "Planets/Mars/model.obj";
        std::cout << "Загружаем модель " << name << " из '" << modelPath << "'" << std::endl;
        model = new Model(modelPath);
        if (!model) {
            std::cerr << "Не удалось загрузить модель " << name << "!" << std::endl;
        }
    }
    else {
        // Для остальных планет создаём сферу
        sphere = new Sphere(radius, 36, 18);
    }
}

Planet::~Planet() {
    if (sphere)
        delete sphere;
    if (model)
        delete model;

    // Удаляем все спутники
    for (Satellite* satellite : satellites)
        delete satellite;
}

void Planet::setIsSun(bool isSunFlag) {
    isSun = isSunFlag;
}

void Planet::addSatellite(Satellite* satellite) {
    satellites.push_back(satellite);
}

void Planet::draw(Shader& shader, float simulationTime, const glm::mat4& parentTransform) {
    glm::mat4 modelMatrix = parentTransform;

    // Для всех планет, кроме Солнца, применяем орбитальное движение
    if (name != "Sun") {
        float angleOrbit = simulationTime * orbitSpeed;
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angleOrbit), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(orbitRadius, 0.0f, 0.0f));
    }

    float angleSelf = simulationTime * rotationSpeed;
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angleSelf), glm::vec3(0.0f, 1.0f, 0.0f));

    shader.setMat4("model", modelMatrix);

    if (model != nullptr) {
        // Если модель загружена (для Sun, Earth и Mars)
        if (name == "Sun") {
            shader.setInt("useTexture", 1);
            shader.setBool("isSun", true);
            shader.setFloat("emissionStrength", 10.0f); // Настройте для эффекта сияния Солнца
            shader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        }
        else {
            shader.setInt("useTexture", 1);
            shader.setBool("isSun", false);
            shader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        }
        model->Draw();
    }
    else if (sphere) {
        shader.setInt("useTexture", 0);
        shader.setBool("isSun", false);
        if (name == "Mercury")
            shader.setVec3("objectColor", glm::vec3(0.5f, 0.5f, 0.5f));
        else if (name == "Venus")
            shader.setVec3("objectColor", glm::vec3(0.9f, 0.6f, 0.2f));
        else if (name == "Earth")
            shader.setVec3("objectColor", glm::vec3(0.2f, 0.3f, 1.0f));
        else if (name == "Mars")
            shader.setVec3("objectColor", glm::vec3(0.8f, 0.3f, 0.1f));
        else
            shader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));

        sphere->draw();
    }

    // Отрисовываем спутников
    for (Satellite* satellite : satellites) {
        satellite->draw(shader, simulationTime, modelMatrix);
    }
}