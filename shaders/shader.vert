#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

struct ModelData {
    mat4 model;
    vec3 color;
    bool selected;
};

layout(std140, set = 1, binding = 0) readonly buffer StorageBuffer {
    ModelData models[];
} storageBuffer;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outPosition;

void main() {
    mat4 model = storageBuffer.models[gl_InstanceIndex].model;
    gl_Position = ubo.proj * ubo.view * model * vec4(inPosition, 1.0f);
    outPosition = vec3(model * vec4(inPosition, 1.0f));
    if (storageBuffer.models[gl_InstanceIndex].selected)
    {
        outColor = vec3(0.0f, 0.0f, 0.0f);
    }
    else
    {
        outColor = storageBuffer.models[gl_InstanceIndex].color;
    }
    outNormal = mat3(transpose(inverse(model))) * inNormal;
}
