#pragma once

#include <vector>
#include <glm/glm.hpp>

struct Triangle
{
    Triangle(std::initializer_list<glm::vec3> c) : vertices(c) {};

    std::vector<glm::vec3> vertices;

    std::vector<Triangle> subdivide() const;

    static glm::vec3 computeMidpointOnSphere(glm::vec3 a, glm::vec3 b)
    {
        float radius = 1.0f;
        return radius * glm::normalize(a + b);
    }
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
};

class IcoSphere
{
public:
    IcoSphere(int levels);
    std::vector<Vertex> getVertices() const;
    std::vector<uint16_t> getIndices() const;
private:
    std::vector<Triangle> triangles_;
};

