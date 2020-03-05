//
// Created by Krisu on 2020-02-05.
//

#ifndef RENDER_ENGINE_IBL_HPP
#define RENDER_ENGINE_IBL_HPP

#include "Texture.hpp"

/*
 * IBL is responsible for holding precomputed maps(irradiance map,
 * prefilter map and brdfLUT) for image based rendering
 */

class IBL {
public:
    IBL() = default;
    IBL(Texture && irradiance,
        Texture && prefilter,
        Texture && brdfLUT) :
          irradiance(std::move(irradiance)),
          prefilter(std::move(prefilter)),
          brdfLUT(std::move(brdfLUT)) { }

    Texture irradiance;
    Texture prefilter;
    Texture brdfLUT;
};


#endif //RENDER_ENGINE_IBL_HPP
