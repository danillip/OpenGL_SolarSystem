#ifndef PLANET_H
#define PLANET_H

#include "Shader.h"
#include "Sphere.h"
#include "Model.h" 
#include <glm.hpp>
#include <string>
#include "Satellite.h"

class Planet {
public:
    Planet(const std::string& name, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed);
    ~Planet();
    void addSatellite(Satellite* satellite);  // Метод для добавления спутников
    void draw(Shader& shader, float simulationTime, const glm::mat4& parentTransform);
    void setIsSun(bool isSun);

    // Запрещаем копирование
    Planet(const Planet&) = delete;
    Planet& operator=(const Planet&) = delete;

    // Реализуем move‑конструктор
    Planet(Planet&& other) noexcept
        : name(std::move(other.name)),
        radius(other.radius),
        orbitRadius(other.orbitRadius),
        orbitSpeed(other.orbitSpeed),
        rotationSpeed(other.rotationSpeed),
        isSun(other.isSun),
        sphere(other.sphere),
        model(other.model)
    {
        other.sphere = nullptr;
        other.model = nullptr;
    }

    // Реализуем move‑оператор присваивания
    Planet& operator=(Planet&& other) noexcept {
        if (this != &other) {
            name = std::move(other.name);
            radius = other.radius;
            orbitRadius = other.orbitRadius;
            orbitSpeed = other.orbitSpeed;
            rotationSpeed = other.rotationSpeed;
            isSun = other.isSun;

            if (sphere) { delete sphere; }
            if (model) { delete model; }
            sphere = other.sphere;
            model = other.model;

            other.sphere = nullptr;
            other.model = nullptr;
        }
        return *this;
    }



private:
    std::string name;
    float radius;
    float orbitRadius;
    float orbitSpeed;
    float rotationSpeed;
    bool isSun;
    Sphere* sphere; // используется для планет
    Model* model;   // используется для Солнца

    std::vector<Satellite*> satellites;  // Храним спутники
};

#endif
