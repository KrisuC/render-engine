//
// Created by Krisu on 2019-12-06.
//

#include "CookTorrancePbrObj.hpp"

CookTorrancePbrObj::CookTorrancePbrObj() :
    shader( "./shaders/cookTorrancePBR.vert",
            "./shaders/cookTorrancePBR.frag"),
    albedo("./resources/pbr/rustediron2_basecolor.png"),
    normal("./resources/pbr/rustediron2_normal.png"),
    metallic("./resources/pbr/rustediron2_metallic.png"),
    roughness("./resources/pbr/rustediron2_roughness.png"),
    ao("./resources/pbr/ao.png") {}

void CookTorrancePbrObj::prepare() {

}

void CookTorrancePbrObj::render() {

}
