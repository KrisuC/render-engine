//
// Created by Krisu on 2019-12-06.
//

#ifndef RENDER_ENGINE_SKYBOX_HPP
#define RENDER_ENGINE_SKYBOX_HPP

#include <Object.hpp>

class Skybox : public Object {
    void prepare() override;
    void render() override;
};


#endif //RENDER_ENGINE_SKYBOX_HPP
