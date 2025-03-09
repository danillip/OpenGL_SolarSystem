#include "Comet.h"
#include <gtc/matrix_transform.hpp>
#include <iostream>

Comet::Comet(const glm::vec3& pos, const glm::vec3& vel, float life)
    : position(pos), velocity(vel), lifetime(life)
{
    // ������� ����� ��� ������ � ������ ��������, 0.5f
    // ����� �������� � ������ ����� ������ ��� ���������
    sphere = new Sphere(0.5f, 36, 18);
}

Comet::~Comet() {
    if (sphere)
        delete sphere;
}

void Comet::update(float dt) {
    position += velocity * dt;
    lifetime -= dt;
}

void Comet::draw(Shader& shader) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    shader.setMat4("model", modelMatrix);
    // ��������� ��������, ������ ����� ���� (��������, 0.7, 0.7, 0.7)
    shader.setInt("useTexture", 0);
    shader.setVec3("objectColor", glm::vec3(0.7f, 0.7f, 0.7f));
    if (sphere)
        sphere->draw();
}

bool Comet::isDead(float boundaryRadius) const {
    return (lifetime <= 0.0f) || (glm::length(position) > boundaryRadius);
}