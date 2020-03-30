//
// Created by Krisu on 2020/2/29.
//

#ifndef RENDER_ENGINE_COMPONENT_HPP
#define RENDER_ENGINE_COMPONENT_HPP

/*
 * In a typical rendering application:
 *
 *
 * All Components' Start() called...
 *
 * while renderLoop not end {
 *
 *      for object in objectList {
 *          for component in object {
 *              component.Update
 *          }
 *          for component in object {
 *              component.LateUpdate
 *          }
 *      }
 *
 * }
 */

class GameObject;

class Component {
public:
    /*  will be called before render loop for all components */
    virtual void Start() { }

    /* will be called in the render loop, before each frame rendererd */
    virtual void Update() { }

    /* will be called in the render loop, after each frame rendererd */
    virtual void LateUpdate() { }

    /* get the owner of the Component */
    GameObject& GetGameObject() { return *owner; }

private:
    /* owner of this Component */
    GameObject *owner = nullptr;
    
    friend class GameObject;
};


#endif //RENDER_ENGINE_COMPONENT_HPP
