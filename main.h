#pragma once
#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>
#include <vector>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include "camera.h"
#include "shaders.h"

using namespace std;

int VERTICES;

GLuint VBO;
GLuint Program;
GLuint texture_sun;
GLuint texture_mercury;
GLuint texture_venus;
GLuint texture_earth;
GLuint texture_mars;
GLuint texture_jupiter;
GLuint texture_saturn;
GLuint texture_uranus;
GLuint texture_neptune;
GLuint texture_pluto;

GLint A_vertex;
GLint A_uvs;
GLint U_mvp;
GLint U_offsets;

Camera cam;
vector<glm::vec4> offsets;
vector<float> speeds_sun;
vector<float> speeds_axis;

struct Vertex
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat s;
    GLfloat t;
};

void SetIcon(sf::Window& wnd);
void checkOpenGLerror();
void ShaderLog(unsigned int shader);
void InitShader();
void LoadAttrib(GLuint prog, GLint& attrib, const char* attr_name);
void LoadUniform(GLuint prog, GLint& attrib, const char* attr_name);
void LoadTexture(GLenum tex_enum, GLuint& tex, const char* path);
void InitVBO();
void InitTextures();
void Init();  
void Draw(sf::Window& window);
void Rotate();  
void ReleaseShader();
void ReleaseVBO();
void Release();

void load_obj(const char* filename, vector<Vertex>& out)
{
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> uvs;
    
    ifstream in(filename, ios::in);
    if (!in)
    {
        cerr << "Can't open obj " << filename << endl;
        return;
    }

    string line;
    while (getline(in, line))
    {
        if (line.length() < 2) continue;

        string s = line.substr(0, 2);
        if (s == "v ")
        {
            istringstream s(line.substr(2));
            glm::vec3 v;
            s >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if (s == "vt")
        {
            istringstream s(line.substr(3));
            glm::vec2 uv;
            s >> uv.x >> uv.y;
            uvs.push_back(uv);
        }
        else if (s == "vn")
        {
            istringstream s(line.substr(3));
            glm::vec3 n;
            s >> n.x >> n.y >> n.z;
            normals.push_back(n);
        }
        else if (s == "f ")
        {
            istringstream s(line.substr(2));
            string s1, s2, s3;
            s >> s1 >> s2 >> s3;
            unsigned int v1, v2, v3, uv1, uv2, uv3, n1, n2, n3;
            sscanf_s(s1.c_str(), "%d/%d/%d", &v1, &uv1, &n1);
            sscanf_s(s2.c_str(), "%d/%d/%d", &v2, &uv2, &n2);
            sscanf_s(s3.c_str(), "%d/%d/%d", &v3, &uv3, &n3);
            
            Vertex ve1 = { vertices[v1 - 1].x, vertices[v1 - 1].y, vertices[v1 - 1].z, uvs[uv1 - 1].x, uvs[uv1 - 1].y };
            Vertex ve2 = { vertices[v2 - 1].x, vertices[v2 - 1].y, vertices[v2 - 1].z, uvs[uv2 - 1].x, uvs[uv2 - 1].y };
            Vertex ve3 = { vertices[v3 - 1].x, vertices[v3 - 1].y, vertices[v3 - 1].z, uvs[uv3 - 1].x, uvs[uv3 - 1].y };
            
            out.push_back(ve1);
            out.push_back(ve2);
            out.push_back(ve3);
        }
    }
}