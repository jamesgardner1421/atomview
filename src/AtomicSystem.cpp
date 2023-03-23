#include "AtomicSystem.hpp"
#include <iostream>

AtomicSystem::AtomicSystem(std::string filename)
    : filename_(filename), trajectory_(filename_), frame_(trajectory_.read()),
    m_SelectedAtoms({})
{
}

std::vector<glm::vec3> AtomicSystem::getPositions()
{
    std::vector<glm::vec3> output;
    for (auto p : frame_.positions())
    {
        output.push_back({p[0], p[1], p[2]});
    }
    return output;
}

std::vector<glm::vec3> AtomicSystem::getColors()
{
    std::vector<glm::vec3> output;
    for (const auto& atom : frame_)
    {
        if (atom.type() == "N")
        {
            output.push_back({0.0f, 0.0f, 1.0f});
        }
        else if (atom.type() == "O")
        {
            output.push_back({1.0f, 0.0f, 0.0f});
        }
        else if (atom.type() == "C")
        {
            output.push_back({0.3f, 0.3f, 0.3f});
        }
        else if (atom.type() == "Au")
        {
            output.push_back({1.0f, 0.85f, 0.0f});
        }
        else
        {
            output.push_back({1.0f, 1.0f, 1.0f});
        }
    }

    return output;
}

glm::vec3 AtomicSystem::getCentre() const
{
    const auto& cell = frame_.cell();

    /* if (cell.shape() == chemfiles::UnitCell::INFINITE) */
    {
        glm::vec3 max{0.0f};
        glm::vec3 min{0.0f};
        for (const auto& p : frame_.positions())
        {
            if (p[0] > max.x) max.x = p[0];
            if (p[0] < min.x) min.x = p[0];

            if (p[1] > max.y) max.y = p[1];
            if (p[1] < min.y) min.y = p[1];

            if (p[2] > max.z) max.z = p[2];
            if (p[2] < min.z) min.z = p[2];
        }
        return (max + min) / 2.0f;
    }

    /* const auto& matrix = cell.matrix(); */
    /* glm::vec3 output{0.0f}; */
    /* for (size_t i=0; i < 3; i++) */
    /* { */
    /*     std::cout << matrix[i][0] << std::endl; */
    /*     output.x += matrix[i][0]; */
    /*     output.y += matrix[i][1]; */
    /*     output.z += matrix[i][2]; */
    /* } */
    /* std::cout << output.x << std::endl; */
    /* std::cout << output.y << std::endl; */
    /* std::cout << output.z << std::endl; */
    /* return output / 2.0f; */
}

glm::vec3 AtomicSystem::getInitialCameraPosition() const
{
    glm::vec3 max{0.0f};
    for (const auto& p : frame_.positions())
    {
        if (p[0] > max.x) max.x = p[0];
        if (p[1] > max.y) max.y = p[1];
        if (p[2] > max.z) max.z = p[2];
    }
    /* return {1.5f * max.x, max.y, max.z + 10.0f}; */
    return max + 10.0f;
    /* return (max + min) / 2.0f; */
    /* return {10.0f, 0.0f, 10.0f}; */
}

void AtomicSystem::selectAtoms(const std::vector<int>& selection)
{
    m_SelectedAtoms = selection;
}

