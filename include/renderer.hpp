//
// Created by Krisu on 2019-10-27.
//

#ifndef RENDER_LAB_RENDERER_HPP
#define RENDER_LAB_RENDERER_HPP

class Renderer {
public:
    virtual void initContext() = 0;
    virtual void render() = 0;
};

#endif //RENDER_LAB_RENDERER_HPP
