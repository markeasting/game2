#pragma once

#include "common/ref.h"
#include <string>
#include <iostream>

class IUniform {
public:

    // std::string m_name;
    int m_location = -1;
    int m_value = -1;

    IUniform() {};

    virtual ~IUniform() = default;

    void setLocation(int location) {
        m_location = location;
    }

    virtual void set() {};
    virtual void bind() {};

protected:
    bool m_needsUpdate = true;
};

template <typename T>
class Uniform : public IUniform {
public:

    T m_value;

    Uniform(T value = {}) 
        : IUniform(), m_value(value) {}

    void set(T value) { 
        m_needsUpdate = true; 
        m_value = value;
    }

    void bind() override;
};

template <typename T>
Ref<Uniform<T>> uniform(T value = {}) {
    return ref<Uniform<T>>(value);
}
