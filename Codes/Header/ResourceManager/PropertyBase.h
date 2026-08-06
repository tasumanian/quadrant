#pragma once

#include <string>

enum class PropertyType
{
    Bool,
    Int,
    Float,
    Vec2,
    Vec3,
    Vec4,
    String,
    Enum,
    Asset,
    Object
};

struct PropertyMetadata
{
    std::string displayName;

    std::string category = "Default";

    std::string tooltip;

    bool visible = true;

    bool readOnly = false;

    float min = 0.0f;

    float max = 0.0f;

    float speed = 0.1f;
};

class PropertyBase
{
public:

    virtual ~PropertyBase() = default;

    virtual PropertyType GetType() const = 0;

    virtual void* GetRawPointer() = 0;

    virtual const std::string& GetName() const = 0;

    virtual PropertyMetadata& GetMetadata() = 0;
};