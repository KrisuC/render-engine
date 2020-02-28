//
// Created by Krisu on 2020-02-05.
//

#include "Scene.hpp"

void Scene::setSkybox(Skybox *sb) {
    if (!skybox) {
        delete skybox;
    }
    skybox = sb;
}
