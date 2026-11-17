#pragma once

#include <cmath>
#include <glm/glm.hpp>

// View frustum made of 6 planes, extracted from a view-projection matrix
// (OpenGL clip space, column-major glm matrices).
class Frustum
{
public:
    void Update(const glm::mat4& viewProj)
    {
        const glm::vec4 r0 = Row(viewProj, 0);
        const glm::vec4 r1 = Row(viewProj, 1);
        const glm::vec4 r2 = Row(viewProj, 2);
        const glm::vec4 r3 = Row(viewProj, 3);

        m_planes[0] = r3 + r0; // left
        m_planes[1] = r3 - r0; // right
        m_planes[2] = r3 + r1; // bottom
        m_planes[3] = r3 - r1; // top
        m_planes[4] = r3 + r2; // near
        m_planes[5] = r3 - r2; // far

        for (glm::vec4& p : m_planes)
        {
            const float len = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
            if (len > 0.0f)
                p /= len;
        }
    }

    // Axis-aligned box given by center and half-extents (world space).
    // Returns false only when the box is completely outside one plane.
    bool IsBoxVisible(const glm::vec3& center, const glm::vec3& extents) const
    {
        for (const glm::vec4& p : m_planes)
        {
            const float distance = p.x * center.x + p.y * center.y + p.z * center.z + p.w;
            const float radius = extents.x * std::fabs(p.x)
                               + extents.y * std::fabs(p.y)
                               + extents.z * std::fabs(p.z);
            if (distance < -radius)
                return false;
        }
        return true;
    }

private:
    static glm::vec4 Row(const glm::mat4& m, int i)
    {
        return glm::vec4(m[0][i], m[1][i], m[2][i], m[3][i]);
    }

    glm::vec4 m_planes[6] = {};
};
