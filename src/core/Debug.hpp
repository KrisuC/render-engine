//
// Created by Krisu on 2020/2/29.
//

#ifndef RENDER_ENGINE_DEBUG_HPP
#define RENDER_ENGINE_DEBUG_HPP

#include <iostream>

class Texture;

template <typename T, typename...Args>
void DEBUG_LOG(T first, Args...args);

void DEBUG_TEXTURE2D(const Texture &t);

#endif //RENDER_ENGINE_DEBUG_HPP
