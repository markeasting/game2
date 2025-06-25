
#include "component/Component.h"
#include "gameobject/GameObject.h"

// template<typename T>
// Ref<T> Component::getComponent() const {

//     static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

//     if (!gameObject) {
//         throw std::runtime_error("GameObject pointer is null.");
//     }

//     return gameObject->getComponent<T>();
// }
