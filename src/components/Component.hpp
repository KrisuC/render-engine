//
// Created by Krisu on 2020/2/29.
//

#ifndef RENDER_ENGINE_COMPONENT_HPP
#define RENDER_ENGINE_COMPONENT_HPP


class Component {
    virtual void BeforeRenderLoop() { }
    virtual void BeforeRenderPass() { }
    virtual void InRenderPass() { }
    virtual void AfterRenderPass() { }
};


#endif //RENDER_ENGINE_COMPONENT_HPP
