//
// Created by Krisu on 2020-02-05.
//

#ifndef RENDER_ENGINE_IBL_HPP
#define RENDER_ENGINE_IBL_HPP

/*
 * IBL is responsible for holding precomputed maps(irradiance map,
 * prefilter map and brdfLUT) for image based rendering
 */

class IBL {
public:
    unsigned int irradianceMap = 0;
    unsigned int prefilterMap = 0;
    unsigned int brdfLUTTexture = 0;
};


#endif //RENDER_ENGINE_IBL_HPP
