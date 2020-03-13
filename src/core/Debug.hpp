//
// Created by Krisu on 2020/2/29.
//

#ifndef RENDER_ENGINE_DEBUG_HPP
#define RENDER_ENGINE_DEBUG_HPP

#include <iostream>

class Texture;
class Shadow;

#ifdef ENGINE_DEBUG

template <typename T>
void DEBUG_LOG(T first) {
    std::clog << first << std::endl;
}

template <typename T, typename...Args>
void DEBUG_LOG(T first, Args...args) {
    std::clog << first << " ";
    if constexpr (sizeof...(args) > 0) {
        DEBUG_LOG(args...);
    }
}

void DEBUG_TEXTURE2D(const Texture &t);

void DEBUG_SHADOW_MAP(const Shadow &s);

#else

template <typename T, typename...Args>
void DEBUG_LOG(T first, Args...args) { /* empty */ }

void DEBUG_TEXTURE2D(const Texture &t) { }

#endif


#endif //RENDER_ENGINE_DEBUG_HPP
