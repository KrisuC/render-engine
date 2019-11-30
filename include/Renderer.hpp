//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_RENDERER_HPP
#define RENDER_ENGINE_RENDERER_HPP

#include <glad/glad.h>

class Renderer {
public:
    virtual void prepareScene() = 0;
    virtual void renderScene() = 0;
};


#endif //RENDER_LAB_RENDERER_HPP
