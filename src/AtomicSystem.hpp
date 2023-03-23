#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <chemfiles.hpp>

class AtomicSystem
{
public:
    AtomicSystem(std::string filename);

    std::vector<glm::vec3> getPositions();
    std::vector<glm::vec3> getColors();

    glm::vec3 getCentre() const;
    glm::vec3 getInitialCameraPosition() const;

    const std::vector<int>& getSelectedAtoms() const { return m_SelectedAtoms; }
    void selectAtoms(const std::vector<int>& selection);

private:
    std::string filename_;

    chemfiles::Trajectory trajectory_;
    chemfiles::Frame frame_;

    std::vector<int> m_SelectedAtoms;
};

