//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_GAMEOBJECT_HPP
#define RENDER_ENGINE_GAMEOBJECT_HPP

#include <memory>
#include <unordered_map>
#include <string>

/*
 * GameObject is an entity to be rendered in the scene.
 * A GameObject is a composite of a Mesh, a Material and a Shader who will
 * be applied to the GameObject at Render pass
 *
 * Mesh, Material and Shader are NOT maintained by GameObject but by other
 * classes (maybe Scene or some user-defined Type)
 */

class no_component : std::exception {  };

class GameObject {
public:
    GameObject() = default;

    GameObject(GameObject const&) = delete;
    GameObject& operator=(GameObject const&) = delete;

    GameObject(GameObject &&) = default;
    GameObject& operator=(GameObject &&) = default;

    template<typename T, typename... Args>
    T& CreateComponent(Args&&... args) {
        T* p_component = new T{args...};
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
            throw no_component();
        }
    }

private:
    std::unordered_map<
            decltype(typeid(int).hash_code()),
            void*
    > componentsMap;
};


#endif //RENDER_ENGINE_GAMEOBJECT_HPP
