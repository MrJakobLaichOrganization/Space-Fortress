#pragma once

#include "guiwindow.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <imgui.h>
#include <optional>
#include <task/order.hpp>
#include <task/task.hpp>
#include <vector>

class OrderWindow : public GuiWindow
{
public:
    OrderWindow(const std::vector<OrderType>& orders) : m_orders(orders)
    {
    }

    void draw() override
    {
        ImGui::Begin("orders", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        for (int idx = 0; idx < m_orders.size(); ++idx)
        {
            const auto& order = m_orders[idx];

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f + (m_activeOrder != idx) * 0.5f);
            if (ImGui::Button(stringifyOrder(order)))
            {
                // "Uncheck" if already order selected
                if (m_activeOrder == idx)
                {
                    m_activeOrder = -1;
                }
                else
                {
                    m_activeOrder = idx;
                }
            }
            ImGui::PopStyleVar();
            ImGui::SameLine();
        }

        // Move the window to bottom middle
        const auto size = ImGui::GetWindowSize();
        const auto screenSize = ImGui::GetIO().DisplaySize;
        ImGui::SetWindowPos(ImVec2{screenSize.x / 2 - size.x / 2, screenSize.y - size.y});
        
        ImGui::End();
    }

    [[nodiscard]] std::optional<OrderType> getActiveOrder() const
    {
        if (m_activeOrder == -1)
        {
            return std::nullopt;
        }

        return m_orders[m_activeOrder];
    }

private:
    const char* stringifyOrder(OrderType order)
    {
        switch (order)
        {
            case OrderType::MOVE:
                return "Move";
            default:
                return "ERR_ORDER";
        }
    }
    int m_activeOrder = -1;
    std::vector<OrderType> m_orders;
};