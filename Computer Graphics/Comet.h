#ifndef COMET_H
#define COMET_H

#include "Shader.h"
#include "Sphere.h"
#include <glm.hpp>
#include <string>

class Comet {
public:
    // ����������� ��������� ��������� �������, �������� � ����� ����� (� ��������)
    Comet(const glm::vec3& position, const glm::vec3& velocity, float lifetime);
    ~Comet();

    // ���������� ��������� ������ (dt � ����� ����� �������)
    void update(float dt);
    // ��������� ������
    void draw(Shader& shader);
    // �������� ���� �� ������� ������
    bool isDead(float boundaryRadius) const;

private:
    glm::vec3 position;
    glm::vec3 velocity;
    float lifetime; // ���������� ����� �����

    Sphere* sphere; // ����� ������������ ����� ��� ������
};

#endif