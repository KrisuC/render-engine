//
// Created by Krisu on 2019-10-27.
//

#ifndef RENDER_LAB_RENDERER_HPP
#define RENDER_LAB_RENDERER_HPP

class Renderer {
public:
    virtual void render() = 0;
    virtual ~Renderer() = default;

protected:
    virtual void processInput() {};
};

#endif //RENDER_LAB_RENDERER_HPP
