#pragma once

#include <memory>

class GameObject; // Forward declaration

/**
 * @brief Component base class.
 */
class Component : public std::enable_shared_from_this<Component> {
public:

    GameObject* gameObject; // Parent object

    bool enabled = true; // Whether the component is enabled

    virtual ~Component() = default;

    virtual void onCreate() {};
    
    virtual void update(float time, float dt) {};

    // template<typename T>
    // Ref<T> getComponent() const;
};
