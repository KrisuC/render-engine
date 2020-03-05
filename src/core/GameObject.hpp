//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_GAMEOBJECT_HPP
#define RENDER_ENGINE_GAMEOBJECT_HPP

#include <memory>
#include <unordered_map>
#include <map>
#include <string>

#include "Component.hpp"

/*
 * GameObject is an entity to be rendered in the scene.
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

        auto component_id = type_hash<T>();
        if (componentsMap.find(component_id) != componentsMap.end()) {
            throw std::runtime_error("Recreation of component is forbidden");
        }

        T* p_component = new T{args...};
        p_component->owner = this;

        componentsMap.insert(std::make_pair(
                component_id, p_component));
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
    template <typename T>
    static auto type_hash() -> decltype(typeid(T).hash_code()) {
        return typeid(T).hash_code();
    }

private:
    std::unordered_map<
            decltype(type_hash<Component>()),
            Component*
    > componentsMap;

    friend class Scene;
};


#endif //RENDER_ENGINE_GAMEOBJECT_HPP
