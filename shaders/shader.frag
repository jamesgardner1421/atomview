#version 450

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inPosition;

layout(location = 0) out vec4 outColor;

void main() {
    float ambientStrength = 0.1;
    vec3 lightColor = {1.0f, 1.0f, 1.0f};

    vec3 lightPosition = {0.0f, 0.0f, 3.0f};
    vec3 lightDirection = normalize(inPosition - lightPosition);

    vec3 ambient = ambientStrength * lightColor;

    float diff = max(dot(inNormal, -lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * inColor;
    outColor = vec4(result, 1.0);
}
