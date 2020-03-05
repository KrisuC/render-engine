//
// Created by Krisu on 2020/2/29.
//

#ifndef RENDER_ENGINE_COMPONENT_HPP
#define RENDER_ENGINE_COMPONENT_HPP

/*
 * In a typical rendering application:
 *
 *
 * All Components' BeforeRenderLoop() called...
 *
 * while (!renderer.ShouldEnd()) {
 *
 *      All Components' BeforeRenderPass() called...
 *
 *      All Components' InRenderPass() called...
 *
 *      All Components' AfterRenderPass() called...
 *
 * }
 */

class Component {
public:

    virtual void BeforeRenderLoop() { }

    virtual void BeforeRenderPass() { }

    virtual void InRenderPass() { }

    virtual void AfterRenderPass() { }

};


#endif //RENDER_ENGINE_COMPONENT_HPP
