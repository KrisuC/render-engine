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

// TODO: Add child-parent relationship

class NoComponent : std::exception {  };

class GameObject {
public:
    GameObject() = default;

    GameObject(GameObject const&) = delete;
    GameObject& operator=(GameObject const&) = delete;

    GameObject(GameObject &&) = default;
    GameObject& operator=(GameObject &&) = default;

    template<typename T, typename... Args>
    T& CreateComponent(Args&&... args);

    template<typename T>
    T& GetComponent();

private:
    template <typename T>
    static auto type_hash() -> decltype(typeid(T).hash_code()) {
        return typeid(T).hash_code();
    }

private:
    std::unordered_map<
            size_t,
            std::unique_ptr<Component>
    > components_map;

    friend class Scene;
};


template<typename T, typename... Args>
T& GameObject::CreateComponent(Args &&... args) {
    static_assert(std::is_base_of_v<Component, T>,
                  "T must be derived class of Component");

    auto hash = type_hash<T>();
    if (components_map.find(hash) != components_map.end()) {
        throw std::runtime_error("Recreation of component is forbidden");
    }

    auto up_component = std::make_unique<T>(std::forward<Args>(args)...);
    up_component->owner = this;
    /* get reference pointer for return */
    T* ret_p = up_component.get();

    components_map.insert({hash, std::move(up_component)});
    return *ret_p;
}

template<typename T>
T& GameObject::GetComponent() {
    static_assert(std::is_base_of_v<Component, T>,
                  "T must be derived class of Component");

    const auto id = typeid(T).hash_code();
    auto iter = components_map.find(id);
    if (iter != components_map.end()) {
        return *static_cast<T*>(iter->second.get());
    } else {
        throw NoComponent();
    }
}


#endif //RENDER_ENGINE_GAMEOBJECT_HPP
