//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_LAB_RENDERER_HPP
#define RENDER_LAB_RENDERER_HPP

#include <glad/glad.h>

class Renderer {
public:
    Renderer() = default;
    void drawScene();
    void drawObject();
};


#endif //RENDER_LAB_RENDERER_HPP
