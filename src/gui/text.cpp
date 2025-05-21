#include "gui.hpp"

namespace
{
std::vector<std::string> splitStrLines(std::string_view str)
{
    std::vector<std::string> ret;
    std::string tmp;

    for (auto it = str.begin(); it != str.end(); ++it)
    {
        if (*it == '\n')
        {
            ret.push_back(tmp);
            tmp.clear();
            continue;
        }

        tmp += *it;
    }
    if (tmp.size())
    {
        ret.push_back(tmp);
    }

    return ret;
}
} // namespace

Text::Text(std::string_view text,
           const sf::Font& font,
           sf::Vector2f pos,
           sf::Vector2f dims,
           TextCentering horizontalCentering,
           TextCentering verticalCentering,
           GuiElement* parent) :
    m_horizontalCentering{horizontalCentering},
    m_verticalCentering{verticalCentering},
    m_font{font},
    m_text{text},
    GuiElement(pos, dims, parent)
{
    auto lines = splitStrLines(text);

    for (const auto& line : lines)
    {
        m_lines.push_back(sf::Text(font, line));
        m_lines.back().setFillColor({0, 0, 0});
    }
    updateDimensions();
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& line : m_lines)
    {
        target.draw(line, states);
    }
    GuiElement::draw(target, states);
}
void Text::onResize(sf::Vector2u newDims)
{
    updateDimensions();
    GuiElement::onResize(newDims);
}

void Text::setText(std::string_view text)
{
    m_text = text;
    m_lines.clear();
    auto lines = splitStrLines(text);

    for (const auto& line : lines)
    {
        m_lines.push_back(sf::Text(m_font, line));
        m_lines.back().setFillColor({0, 0, 0});
    }
    updateDimensions();
}

void Text::updateDimensions()
{
    const auto position = pixelPos();
    const auto size = pixelSize();
    const auto centerPos = calculateCenter();
    // Aligns the lines horizontally
    for (auto& line : m_lines)
    {
        auto txtSize = line.getLocalBounds().size;
        switch (m_horizontalCentering)
        {
            case TextCentering::LEFT:
                line.setPosition({position.x, 0});
                break;
            case TextCentering::CENTER:
                line.setPosition({centerPos.x - (txtSize.x / 2.f), 0});
                break;
            case TextCentering::RIGHT:
                line.setPosition({size.x + position.x - txtSize.x, 0});
                break;
        }
        line.setFillColor({0, 0, 0});
    }

    // Aligns vertically
    float totalTextH = 0;
    std::uint32_t yOff = 0;

    for (const auto& line : m_lines)
    {
        totalTextH += line.getLocalBounds().size.y + yPadding;
    }
    for (auto& line : m_lines)
    {
        auto lineSize = line.getLocalBounds().size;
        switch (m_verticalCentering)
        {
            case TextCentering::UP:
                line.setPosition({line.getPosition().x, position.y + static_cast<float>(yOff)});
                break;
            case TextCentering::CENTER:
                line.setPosition({line.getPosition().x, centerPos.y - ((lineSize.y / 2.f) * m_lines.size()) + yOff});
                break;
            case TextCentering::DOWN:
                line.setPosition({line.getPosition().x, position.y + size.y - totalTextH - yPadding + yOff});
                break;
        }
        yOff += lineSize.y + yPadding;
    }
}