#pragma once

#include <imgui-SFML.h>
#include <imgui.h>

class GuiWindow {
    public:
    virtual ~GuiWindow() = default;

    virtual void draw() = 0;
};