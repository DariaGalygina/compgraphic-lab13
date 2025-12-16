#include "main.h"

void Init()
{
    speeds_sun = {
        0.00, 0.002, 0.003, 0.004, 0.005, 0.006, 0.007, 0.008, 0.009, 0.010,
    };
    speeds_axis = {
        0.01, 0.04, 0.06, 0.08, 0.1, 0.12, 0.14, 0.16, 0.18, 0.002,
    };
    
    offsets = {
        {0.0, 5.0, 0.0, 0.0},
        {15.0, 1.0, 0.0, 0.0},
        {20.0, 1.5, 0.0, 0.0},
        {25.0, 1.5, 0.0, 0.0},
        {30.0, 1.0, 0.0, 0.0},
        {40.0, 3.0, 0.0, 0.0},
        {50.0, 2.5, 0.0, 0.0},
        {60.0, 2.0, 0.0, 0.0},
        {70.0, 2.0, 0.0, 0.0},
        {80.0, 1.0, 0.0, 0.0}
    };
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    InitShader();
    InitVBO();
    InitTextures();
}

void Rotate()
{
    for (size_t i = 0; i < offsets.size(); i++)
    {
        offsets[i].z = fmod(offsets[i].z + speeds_axis[i], 2.0f * static_cast<float>(M_PI));
        offsets[i].w = fmod(offsets[i].w + speeds_sun[i], 2.0f * static_cast<float>(M_PI));
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    sf::Window window(sf::VideoMode(1000, 1000), "Shapes", sf::Style::Default, sf::ContextSettings(24));
    SetIcon(window);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);
    glewInit();
    Init();

    for (int i = 0; i < 30; ++i) { 
        cam.S();
    }
    
    bool paused_sun = false;
    bool paused_axis = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                goto EXIT_IS_RIGHT_HERE;
            }
            else if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    cam.PitchPlus();
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    cam.PitchMinus();
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    cam.YawPlus();
                }
                else if (event.key.code == sf::Keyboard::Left)
                {
                    cam.YawMinus();
                }
                else if (event.key.code == sf::Keyboard::W)
                {
                    cam.W();
                }
                else if (event.key.code == sf::Keyboard::S)
                {
                    cam.S();
                }
                else if (event.key.code == sf::Keyboard::A)
                {
                    cam.A();
                }
                else if (event.key.code == sf::Keyboard::D)
                {
                    cam.D();
                }
                else if (event.key.code == sf::Keyboard::F1)
                {
                    cam.Perspective();
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    cam.Reset();
                    for (int i = 0; i < 30; ++i) { 
                        cam.S();
                    }
                }
                else if (event.key.code == sf::Keyboard::Space)
                {
                    paused_sun = !paused_sun;
                    if (paused_sun)
                    {
                        speeds_sun = {
                            0.00, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                        };
                    }
                    else
                    {
                        speeds_sun = {
                            0.00, 0.002, 0.003, 0.004, 0.005, 0.006, 0.007, 0.008, 0.009, 0.010,
                        };
                    }
                }
                else if (event.key.code == sf::Keyboard::RAlt || event.key.code == sf::Keyboard::LAlt)
                {
                    paused_axis = !paused_axis;
                    if (paused_axis)
                    {
                        speeds_axis = {
                            0.00, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                        };
                    }
                    else
                    {
                        speeds_axis = {
                            0.01, 0.04, 0.06, 0.08, 0.1, 0.12, 0.14, 0.16, 0.18, 0.002,
                        };
                    }
                }
            }

        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Rotate();
        Draw(window);
        window.display();
    }
EXIT_IS_RIGHT_HERE:
    Release();
    return 0;
}

void InitVBO()
{
    glGenBuffers(1, &VBO);
    vector<Vertex> data;
    load_obj("sphere.obj", data);
    VERTICES = static_cast<int>(data.size());
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * sizeof(Vertex), data.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    checkOpenGLerror();
}

void InitTextures()
{
    LoadTexture(GL_TEXTURE0, texture_sun, "textures/sun_map.jpg");
    LoadTexture(GL_TEXTURE1, texture_mercury, "textures/mercury_map.jpg");
    LoadTexture(GL_TEXTURE2, texture_venus, "textures/venus_map.jpg");
    LoadTexture(GL_TEXTURE3, texture_earth, "textures/earth_map.jpg");
    LoadTexture(GL_TEXTURE4, texture_mars, "textures/mars_map.jpg");
    LoadTexture(GL_TEXTURE5, texture_jupiter, "textures/jupiter_map.jpg");
    LoadTexture(GL_TEXTURE6, texture_saturn, "textures/saturn_map.jpg");
    LoadTexture(GL_TEXTURE7, texture_uranus, "textures/uranus_map.jpg");
    LoadTexture(GL_TEXTURE8, texture_neptune, "textures/neptune_map.jpg");
    LoadTexture(GL_TEXTURE9, texture_pluto, "textures/pluto_map.jpg");
}

void LoadAttrib(GLuint prog, GLint& attrib, const char* attr_name)
{
    attrib = glGetAttribLocation(prog, attr_name);
    if (attrib == -1)
    {
        std::cout << "could not bind attrib " << attr_name << std::endl;
        return;
    }
}

void LoadUniform(GLuint prog, GLint& attrib, const char* attr_name)
{
    attrib = glGetUniformLocation(prog, attr_name);
    if (attrib == -1)
    {
        std::cout << "could not bind uniform " << attr_name << std::endl;
        return;
    }
}

void LoadTexture(GLenum tex_enum, GLuint& tex, const char* path)
{
    glGenTextures(1, &tex);
    glActiveTexture(tex_enum);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    sf::Image img;
    if (!img.loadFromFile(path))
    {
        std::cout << "could not load texture " << path << std::endl;
        return;
    }

    sf::Vector2u size = img.getSize();
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void InitShader()
{
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &VertexShaderSource, NULL);
    glCompileShader(vShader);
    std::cout << "vertex shader \n";
    ShaderLog(vShader);

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &FragShaderSource, NULL);
    glCompileShader(fShader);
    std::cout << "fragment shader \n";
    ShaderLog(fShader);

    Program = glCreateProgram();
    glAttachShader(Program, vShader);
    glAttachShader(Program, fShader);
    glLinkProgram(Program);

    int link;
    glGetProgramiv(Program, GL_LINK_STATUS, &link);

    if (!link)
    {
        std::cout << "error attach shaders \n";
        return;
    }

    LoadAttrib(Program, A_vertex, "coord");
    LoadAttrib(Program, A_uvs, "uv");
    LoadUniform(Program, U_mvp, "mvp");
    LoadUniform(Program, U_offsets, "offsets");
    checkOpenGLerror();
    glUseProgram(Program);
    GLint textures[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    glUniform1iv(glGetUniformLocation(Program, "tex"), 10, textures);
    glUseProgram(0);
}

void Draw(sf::Window& window)
{
    window.setTitle("Solar System in OpenGL");
    glUseProgram(Program);
    glUniformMatrix4fv(U_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
    glUniform4fv(glGetUniformLocation(Program, "offsets"), 10,
        glm::value_ptr(offsets[0]));
    glEnableVertexAttribArray(A_vertex);
    glEnableVertexAttribArray(A_uvs);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(A_vertex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(A_uvs, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArraysInstanced(GL_TRIANGLES, 0, VERTICES, 10);
    glDisableVertexAttribArray(A_vertex);
    glDisableVertexAttribArray(A_uvs);
    glUseProgram(0);
    checkOpenGLerror();
}

void Release()
{
    ReleaseShader();
    ReleaseVBO();
}

void ReleaseVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO);
}

void ReleaseShader()
{
    glUseProgram(0);
    glDeleteProgram(Program);
}

void ShaderLog(unsigned int shader)
{
    int infologLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
    if (infologLen > 1)
    {
        int charsWritten = 0;
        std::vector<char> infoLog(infologLen);
        glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
        std::cout << "InfoLog: " << infoLog.data() << std::endl;
        exit(1);
    }
}

void checkOpenGLerror()
{
    GLenum errCode;
    const GLubyte* errString;
    if ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        std::cout << "OpenGL error: " << errString << std::endl;
    }
}

void SetIcon(sf::Window& wnd)
{
    sf::Image image;
    if (!image.loadFromFile("icon.png"))
    {
        std::cout << "error load icon \n";
        return;
    }

    wnd.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
}