#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gl/glut.h>
#include "Shader.h"
#include "Planet.h"
#include "Asteroids.h"
#include "Comet.h"
#include <cstdlib>
#include "Camera.h"
#include "Texture.h"      // Texture
#include "Background.h"   
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <cstdio> 
#include <locale.h>

//assimp библиотека для моделей
//SOIL

// Глобальные переменные симуляции
float simulationTime = 0.0f;
bool isPaused = false;
bool schematicMode = false; // схематичный (true) или реальный режим (false)

// Переменная анимированного фона
bool useCustomBackground = false;

bool useComets = true;     // Флаг включения/выключения комет

// Временные переменные для расчёта deltaTime
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Объект камеры
Camera camera;

// Переменные для мыши
bool firstMouse = true;
double lastX = 400, lastY = 300;

// Глобалки
std::vector<Comet*> comets;
float cometSpawnTimer = 0.0f; // Таймер для спауна новых комет
const float cometSpawnInterval = 2.0f; // каждые 2 секунды спаун новой кометы
const float cometLifetime = 10.0f; // время жизни кометы
const float cometBoundary = 350.0f; // предел за который комета исчезает

// Callback клавиатуры
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_SPACE)
            isPaused = !isPaused;
        if (key == GLFW_KEY_M)
            schematicMode = !schematicMode;
        // Переключение между схематичным и реальным режимом с помощью R
        if (key == GLFW_KEY_R)
            schematicMode = !schematicMode;

        // Можно добавить клавишу для переключения фона, например F:
        if (key == GLFW_KEY_F)
            useCustomBackground = !useCustomBackground;

        // Переключение комет по клавише C
        if (key == GLFW_KEY_C)
            useComets = !useComets;
    }
}


// Callback мыши для вращения камеры
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // инвертировано: вверх — положительное значение
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// Callback скролла для зума
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll((float)yoffset);
}

int main() {
    setlocale(LC_ALL, "ru");
    // Инициализация GLFW, создание окна, установка колбэков и т.д.
    if (!glfwInit()) { /* ... */ }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Solar System", nullptr, nullptr);
    if (!window) { /* ... */ }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { /* ... */ }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");

    Planet sun("Sun", 5.0f, 0.0f, 0.0f, 0.0f);
    sun.setIsSun(true);

    std::vector<Planet> planets;
    planets.emplace_back("Mercury", 1.0f, schematicMode ? 10.0f : 57.9f, 4.74f, 6.138f);
    planets.emplace_back("Venus", 1.2f, schematicMode ? 15.0f : 108.2f, 3.50f, -6.52f);
    planets.emplace_back("Earth", 1.3f, schematicMode ? 20.0f : 149.6f, 2.98f, 7.29f);
    planets.emplace_back("Mars", 1.1f, schematicMode ? 25.0f : 227.9f, 2.41f, 7.09f);

    Planet* earth = &planets[2];
    earth->addSatellite(new Satellite("Moon", 0.27f, 3.0f, 1.0f, 10.0f, earth));

    Planet* mars = &planets[3];
    mars->addSatellite(new Satellite("Phobos", 0.05f, 1.5f, 0.5f, 8.0f, mars));
    mars->addSatellite(new Satellite("Deimos", 0.03f, 2.5f, 0.7f, 6.0f, mars));

    Asteroids asteroids;
    asteroids.init();

    Background background("Planets/Cosmos", 78, 0.05f);
    Shader backgroundShader("background_vertex.glsl", "background_fragment.glsl");

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        bool accelerate = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime, accelerate);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime, accelerate);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime, accelerate);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime, accelerate);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.ProcessKeyboard(UP, deltaTime, accelerate);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, deltaTime, accelerate);
        if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
            camera.ProcessMouseScroll(1.0f * deltaTime * 20.0f);
        if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
            camera.ProcessMouseScroll(-1.0f * deltaTime * 20.0f);

        if (!isPaused)
            simulationTime += 0.1f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.f / 600.f, 1.0f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();

        if (useCustomBackground) {
            backgroundShader.use();
            background.Draw(backgroundShader);
        }

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setVec3("lightPos", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setVec3("viewPos", camera.Position);

        sun.draw(shader, simulationTime, glm::mat4(1.0f));
        for (auto& planet : planets)
            planet.draw(shader, simulationTime, glm::mat4(1.0f));
        asteroids.draw(shader, simulationTime);

        // Обновляем и отрисовываем кометы только если флаг включен
        if (useComets) {
            cometSpawnTimer += deltaTime;
            if (cometSpawnTimer >= cometSpawnInterval) {
                cometSpawnTimer = 0.0f;
                float angle = static_cast<float>(rand()) / RAND_MAX * 360.0f;
                float distance = 320.0f + static_cast<float>(rand()) / RAND_MAX * 30.0f;
                float x = distance * cos(glm::radians(angle));
                float z = distance * sin(glm::radians(angle));
                float y = (rand() % 40 - 20) * 1.0f;
                glm::vec3 spawnPos = glm::vec3(x, y, z);
                glm::vec3 dir = -glm::normalize(spawnPos);
                float speed = 20.0f + static_cast<float>(rand()) / RAND_MAX * 10.0f;
                glm::vec3 velocity = dir * speed;
                comets.push_back(new Comet(spawnPos, velocity, cometLifetime));
            }
            for (size_t i = 0; i < comets.size(); ) {
                comets[i]->update(deltaTime);
                if (comets[i]->isDead(cometBoundary)) {
                    delete comets[i];
                    comets.erase(comets.begin() + i);
                }
                else {
                    comets[i]->draw(shader);
                    i++;
                }
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Освобождаем память для комет
    for (auto comet : comets)
        delete comet;

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}