//
// Created by Krisu on 2020-02-06.
//

#ifndef RENDER_ENGINE_GUIMANAGER_HPP
#define RENDER_ENGINE_GUIMANAGER_HPP


#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

/*
 * Handling GUI things.
 * GUIManager is using imgui, while window is using glfw in implementation
 */

#include "Renderer.hpp"

class GUIManager {
public:
    explicit GUIManager(const Renderer &renderer) : renderer(renderer) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(renderer.window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    ~GUIManager() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void drawUI() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* Drawing UI... */

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };

private:
    const Renderer &renderer;
};



#endif //RENDER_ENGINE_GUIMANAGER_HPP
