#pragma once

const char* VertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;

out vec2 texCoord;
flat out int instanceID;

uniform mat4 view;
uniform mat4 proj;
uniform vec4 planetData[10]; // x: dist, y: scale, z: selfRot, w: orbitRot

mat4 rotateY(float angle) {
    return mat4(cos(angle), 0, sin(angle), 0, 0, 1, 0, 0, -sin(angle), 0, cos(angle), 0, 0, 0, 0, 1);
}

void main() {
    vec4 data = planetData[gl_InstanceID];
    
    // 1. Поворот вокруг своей оси и масштаб
    vec4 localPos = rotateY(data.z) * vec4(pos * data.y, 1.0);
    // 2. Смещение на радиус орбиты
    vec4 worldPos = localPos + vec4(data.x, 0, 0, 0);
    // 3. Вращение по орбите вокруг Солнца
    vec4 finalPos = rotateY(data.w) * worldPos;

    gl_Position = proj * view * finalPos;
    texCoord = uv;
    instanceID = gl_InstanceID;
})";

const char* FragShaderSource = R"(
#version 330 core
in vec2 texCoord;
flat in int instanceID;
uniform sampler2D texSamplers[10];

void main() {
    gl_FragColor = texture(texSamplers[instanceID], texCoord);
})";
