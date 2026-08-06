#pragma once

#include "ResourceManager/PropertyBase.h"
#include <glm/glm.hpp>

template<typename T>
struct PropertyTraits;

template<>
struct PropertyTraits<bool>
{
    static constexpr PropertyType Type = PropertyType::Bool;
};
template<>
struct PropertyTraits<float>
{
    static constexpr PropertyType Type = PropertyType::Float;
};
template<>
struct PropertyTraits<glm::vec3>
{
    static constexpr PropertyType Type = PropertyType::Vec3;
};