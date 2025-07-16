
#include "gameobject/GameObject.h"


GameObject::GameObject(
    const std::string& name,
    std::vector<Ref<Component>> components
) : name(name) {

    m_components = std::move(components);

    m_transform = addComponent<Transform>();

    for (auto& component : m_components) {
        component->gameObject = this;
        component->onCreate();
    }
}

void GameObject::update(float time, float dt) {
    for (auto& component : m_components) {
        if (!component->enabled) continue;
        component->update(time, dt);
    }
}
