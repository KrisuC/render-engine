//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_GAMEOBJECT_HPP
#define RENDER_ENGINE_GAMEOBJECT_HPP

#include <memory>
#include <unordered_map>
#include <string>

#include "Component.hpp"

/*
 * GameObject is an entity to be rendered in the scene.
 * A GameObject is a composite of a Mesh, a Material and a Shader who will
 * be applied to the GameObject at Render pass
 *
 * Mesh, Material and Shader are NOT maintained by GameObject but by other
 * classes (maybe Scene or some user-defined Type)
 */

class NoComponent : std::exception {  };

class GameObject {
public:
    GameObject() = default;

    GameObject(GameObject const&) = delete;
    GameObject& operator=(GameObject const&) = delete;

    GameObject(GameObject &&) = default;
    GameObject& operator=(GameObject &&) = default;

    template<typename T, typename... Args>
    T& CreateComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, T>,
                      "T must be derived class of Component");

        T* p_component = new T{args...};
        p_component->owner = this;

        componentsMap.insert(std::make_pair(
                typeid(T).hash_code(), p_component));
        return *p_component;
    }

    template<typename T>
    T& GetComponent() {
        const auto id = typeid(T).hash_code();
        auto iter = componentsMap.find(id);
        if (iter != componentsMap.end()) {
            return *static_cast<T*>(iter->second);
        } else {
            throw NoComponent();
        }
    }

private:
    std::unordered_map<
            decltype(typeid(Component).hash_code()),
            Component*
    > componentsMap;

    friend class Scene;
};


#endif //RENDER_ENGINE_GAMEOBJECT_HPP
