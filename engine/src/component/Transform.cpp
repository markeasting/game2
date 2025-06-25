
#include "component/Transform.h"
// #include <type_traits>

void Transform::add(Ref<Transform> transform) {

    // static_assert(std::is_base_of_v<Object3D, T>);
    assert(transform.get() != this);

    // if (object->parent) {
    //     // @TODO remove from parent
    // }

    transform->m_parent = this;
    m_children.push_back(transform);
}

vec3 Transform::getPosition() {
    return m_position;
}

quat Transform::getRotation() {
    return m_rotation;
}

vec3 Transform::getScale() {
    return m_scale;
}

void Transform::setPosition(const vec3& position) {
    m_position = position;
    m_worldPosMatrixNeedsUpdate = true;
}

void Transform::setRotation(const quat& rotation) {
    m_rotation = glm::normalize(rotation); // @TODO check if required?
    m_worldPosMatrixNeedsUpdate = true;
}

void Transform::setRotation(const vec3& euler) {

    m_rotation = quat(vec3(
        glm::radians(euler.x), 
        glm::radians(euler.y), 
        glm::radians(euler.z)
    ));
    m_rotation = glm::normalize(m_rotation);
    
    m_worldPosMatrixNeedsUpdate = true;

}

void Transform::setScale(float uniformScale) {
    setScale(vec3(uniformScale));
}

void Transform::setScale(const vec3& scale) {
    m_scale = scale;
    m_worldPosMatrixNeedsUpdate = true;
}

void Transform::translate(vec3 translation) {
    m_position += translation;
    m_worldPosMatrixNeedsUpdate = true;
}

void Transform::rotate(float angle, const vec3& direction) {
    m_rotation = glm::rotate(m_rotation, glm::radians(angle), direction);
    m_rotation = glm::normalize(m_rotation);
    m_worldPosMatrixNeedsUpdate = true;
}

mat4 Transform::getWorldPositionMatrix() {
    if (m_worldPosMatrixNeedsUpdate) {

        m_worldPositionMatrix = glm::scale(
            glm::translate(
                glm::mat4(1),
                m_position
            ) * glm::mat4_cast(
                m_rotation
            ), 
            m_scale
        );

        if (m_parent)
            m_worldPositionMatrix = m_parent->m_worldPositionMatrix * m_worldPositionMatrix;

        for (auto&& child : m_children) {
            child->m_worldPosMatrixNeedsUpdate = true;
        }

        m_worldPosMatrixNeedsUpdate = false;
    }
    
    return m_worldPositionMatrix;
}
