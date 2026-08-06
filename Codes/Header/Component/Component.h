#pragma once
#include "ResourceManager/Property.h"
#include <vector>
#include <memory>

class GameObject;

class Component
{
    friend class ComponentSystem;

protected:

    bool m_awaked = false;

    bool m_started = false;

    std::vector<
        std::unique_ptr<PropertyBase>>
        m_properties;

    template<typename T>
    void RegisterProperty(
        const std::string& name,
        T* value);

public:

    GameObject* gameObject = nullptr;

    bool enabled = true;

    virtual ~Component() = default;

    virtual void Awake() {}
    virtual void Start() {}
    virtual void Update(float dt) {}
    virtual void FixedUpdate(float dt) {}
    virtual void LateUpdate(float dt) {}
    virtual const char* GetComponentName() const = 0;

    void SetGameObject(GameObject* obj)
    {
        gameObject = obj;
    }

    GameObject* GetGameObject() const
    {
        return gameObject;
    }
    auto& GetProperties()
    {
        return m_properties;
    }

};