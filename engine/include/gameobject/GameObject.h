#pragma once

#include "component/Component.h"
#include "component/Transform.h"

#include <memory>
#include <stdexcept>
#include <vector>

class GameObject : public std::enable_shared_from_this<GameObject> {
public:

    std::string name = "";

    Ref<Transform> m_transform = nullptr;

    // GameObject();

    /**
     * @brief Create a GameObject with a name and a list of components.
     * @note A <Transform> component is automatically added. 
     */
    GameObject(
        const std::string& name,
        std::vector<Ref<Component>> components = {}
    );

    virtual ~GameObject() = default;

    template<typename T, typename... Args>
    Ref<T> addComponent(Args&&... args) {

        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
        
        auto component = ref<T>(std::forward<Args>(args)...);
        
        component->gameObject = this; //std::make_shared<GameObject>(this);
        
        m_components.push_back(component); // std::move(component)
        
        component->onCreate();
        
        // componentMap[typeid(T)].push_back(component);

        return component;
    }

    template<typename T>
    std::shared_ptr<T> tryGetComponent() const {
        return getComponent<T>(false);
    }

    template<typename T>
    std::shared_ptr<T> getComponent(bool strict = true) const {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
 
        for (const auto& c : m_components) {
            if (auto comp = std::dynamic_pointer_cast<T>(c)) {
                return comp;
            }
        }
        
        // @todo convert to map for O(1) access
        // auto it = componentMap.find(typeid(T));
        // if (it != componentMap.end() && !it->second.empty()) {
        //     return std::static_pointer_cast<T>(it->second.front());
        // }

        if (strict) {
            throw std::runtime_error("Component '" + std::string(typeid(T).name()) + "' not found in GameObject '" + name  + "'.");
        }

        return nullptr;
    }

    template<typename T>
    bool hasComponent() const {
        
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
        
        for (const auto& c : m_components) {
            if (std::dynamic_pointer_cast<T>(c)) {
                return true;
            }
        }
    }

    virtual void update(float time, float dt);

// protected:

    /**
     * @brief Array of all components for quick iterator access. 
     */
    std::vector<Ref<Component>> m_components = {};

    // std::unordered_map<
    //     std::type_index, 
    //     std::vector<Ref<Component>>
    // > componentMap = {};
};
