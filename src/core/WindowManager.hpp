//
// Created by Krisu on 2019-12-29.
//

#ifndef RENDER_ENGINE_WINDOWMANAGER_HPP
#define RENDER_ENGINE_WINDOWMANAGER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <type_traits>

#include "IO.hpp"


class WindowManager {
public:
    WindowManager();
    ~WindowManager();

    WindowManager(WindowManager const&) = delete;
    WindowManager& operator=(WindowManager const&) = delete;

    void SetVsync(bool on);

    void SetMSAA(int samples);

    bool ShouldEnd();

    double GetDeltaTime() const;

    void InitializeOpenGL();

    void ResetViewport();;

    std::pair<int, int> GetWindowSize() const;

    void Close();

    void UpdateBeforeRendering();

    void UpdateAfterRendering();

private:
    GLFWwindow *window = nullptr;

    friend class GUI;
};



#endif //RENDER_ENGINE_WINDOWMANAGER_HPP
