#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shaders.h"

struct Vertex { glm::vec3 p; glm::vec2 uv; };
GLuint Program, VBO, textures[10];
int vertexCount = 0;
Camera cam;

std::vector<glm::vec4> planetParams(10); 
std::vector<float> orbitSpeeds = {0.0f, 0.008f, 0.006f, 0.004f, 0.003f, 0.002f, 0.0015f, 0.001f, 0.0008f, 0.0005f};
std::vector<float> selfRotSpeeds = {0.005f, 0.02f, 0.002f, 0.015f, 0.014f, 0.03f, 0.028f, 0.02f, 0.018f, 0.01f};

void load_obj(const std::string& path, std::vector<Vertex>& out) {
    std::vector<glm::vec3> temp_p; std::vector<glm::vec2> temp_uv;
    std::ifstream f(path); std::string line;
    if (!f.is_open()) return;
    while (std::getline(f, line)) {
        std::stringstream ss(line); std::string type; ss >> type;
        if (type == "v") { glm::vec3 v; ss >> v.x >> v.y >> v.z; temp_p.push_back(v); }
        else if (type == "vt") { glm::vec2 v; ss >> v.x >> v.y; temp_uv.push_back(v); }
        else if (type == "f") {
            for (int i = 0; i < 3; i++) {
                std::string s; ss >> s;
                int vIdx = std::stoi(s.substr(0, s.find('/'))) - 1;
                int uvIdx = std::stoi(s.substr(s.find('/') + 1, s.find_last_of('/') - s.find('/') - 1)) - 1;
                out.push_back({temp_p[vIdx], temp_uv[uvIdx]});
            }
        }
    }
}

void setupTexture(int i, const char* path) {
    sf::Image img; if (!img.loadFromFile(path)) return;
    glGenTextures(1, &textures[i]);
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, textures[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void init() {
    glewInit();
    glEnable(GL_DEPTH_TEST);

    planetParams = {
        {0.0f, 15.0f, 0, 0}, {25.0f, 2.5f, 0, 0}, {40.0f, 3.5f, 0, 0}, {55.0f, 4.0f, 0, 0}, 
        {70.0f, 3.2f, 0, 0}, {95.0f, 8.0f, 0, 0}, {120.0f, 7.0f, 0, 0}, {145.0f, 5.5f, 0, 0},
        {165.0f, 5.0f, 0, 0}, {185.0f, 2.0f, 0, 0}
    };

    GLuint vS = glCreateShader(GL_VERTEX_SHADER); glShaderSource(vS, 1, &VertexShaderSource, 0); glCompileShader(vS);
    GLuint fS = glCreateShader(GL_FRAGMENT_SHADER); glShaderSource(fS, 1, &FragShaderSource, 0); glCompileShader(fS);
    Program = glCreateProgram(); glAttachShader(Program, vS); glAttachShader(Program, fS); glLinkProgram(Program);

    std::vector<Vertex> mesh;
    load_obj("sphere.obj", mesh); 
    vertexCount = mesh.size();

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(Vertex), mesh.data(), GL_STATIC_DRAW);

    const char* texPaths[] = {"textures/sun_map.jpg", "textures/mercury_map.jpg", "textures/venus_map.jpg", "textures/earth_map.jpg", "textures/mars_map.jpg", "textures/jupiter_map.jpg", "textures/saturn_map.jpg", "textures/uranus_map.jpg", "textures/neptune_map.jpg", "textures/pluto_map.jpg"};
    for(int i=0; i<10; i++) setupTexture(i, texPaths[i]);

    glUseProgram(Program);
    GLint samplers[10] = {0,1,2,3,4,5,6,7,8,9};
    glUniform1iv(glGetUniformLocation(Program, "texSamplers"), 10, samplers);
}

int main() {
    sf::ContextSettings settings(24);
    sf::Window window(sf::VideoMode(1200, 800), "Solar Suzanne System", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    init();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) if (event.type == sf::Event::Closed) window.close();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) cam.Move(1, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) cam.Move(-1, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) cam.Move(0, -1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) cam.Move(0, 1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) cam.Rotate(1, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) cam.Rotate(-1, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) cam.Rotate(0, -1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) cam.Rotate(0, 1);

        for(int i=0; i<10; i++) {
            planetParams[i].z += selfRotSpeeds[i]; 
            planetParams[i].w += orbitSpeeds[i];
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(Program);
        glUniformMatrix4fv(glGetUniformLocation(Program, "view"), 1, GL_FALSE, glm::value_ptr(cam.GetView()));
        glUniformMatrix4fv(glGetUniformLocation(Program, "proj"), 1, GL_FALSE, glm::value_ptr(cam.proj));
        glUniform4fv(glGetUniformLocation(Program, "planetData"), 10, (float*)planetParams.data());

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3)); glEnableVertexAttribArray(1);

        glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, 10);
        window.display();
    }
    return 0;
}
