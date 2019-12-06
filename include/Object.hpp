//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_OBJECT_HPP
#define RENDER_ENGINE_OBJECT_HPP

class Object {
public:
    virtual void prepare() = 0;
    virtual void render() = 0;
};


#endif //RENDER_ENGINE_OBJECT_HPP
