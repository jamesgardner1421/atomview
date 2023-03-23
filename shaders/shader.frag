#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inPosition;

layout(location = 0) out vec4 outColor;

void main() {
    mat4 modelView = ubo.view;
    mat4 viewModel = inverse(modelView);
    vec3 cameraPos = vec3(viewModel[3]);
    vec3 viewWS = inPosition - cameraPos;

    float _OutlineWidth = 0.5f;
    float _OutlineSoftness = 2.0f;
    float _OutlinePower = 1.5f;
    vec4 _OutlineColor = vec4(1.0f);

    float edge1 = 1 - _OutlineWidth;
    float edge2 = edge1 + _OutlineSoftness;
    float fresnel = pow(1.0 - clamp(1.0, 0.0, dot(inNormal, viewWS)), _OutlinePower);
    vec4 outline = smoothstep(0.0f, fresnel, 1.0f) * _OutlineColor;

    float ambientStrength = 0.4;
    vec3 lightColor = {1.0f, 1.0f, 1.0f};

    vec3 lightPosition = {0.0f, 0.0f, 3.0f};
    vec3 lightDirection = normalize(inPosition - lightPosition);

    vec3 ambient = ambientStrength * lightColor;

    float diff = max(dot(inNormal, -lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * inColor;
    outColor = vec4(result, 1.0);
}
