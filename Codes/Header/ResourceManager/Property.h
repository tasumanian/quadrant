#pragma once

#include <string>
#include "ResourceManager/PropertyBase.h"
#include "ResourceManager/PropertyTraits.h"

template<typename T>
class Property
    : public PropertyBase
{
public:

    Property(
        const std::string& name,
        T* value,
        const PropertyMetadata& metadata = {})
        :
        m_name(name),
        m_value(value),
        m_metadata(metadata)
    {
        if (m_metadata.displayName.empty())
            m_metadata.displayName = name;
    }

    PropertyType GetType() const override
    {
        return PropertyTraits<T>::Type;
    }

    void* GetRawPointer() override
    {
        return m_value;
    }

    T* GetValue()
    {
        return m_value;
    }

    const std::string& GetName() const override
    {
        return m_name;
    }

    PropertyMetadata& GetMetadata() override
    {
        return m_metadata;
    }

private:

    std::string m_name;

    T* m_value;

    PropertyMetadata m_metadata;
};