#include "DrawContext.h"
#include <SFML/Graphics.hpp>


static inline sf::Vector2f GetSfVec(const CMPUT350::Point2D& pnt) {
    return sf::Vector2f(pnt.x,pnt.y);
}

static inline sf::Color GetSfColor(const CMPUT350::RGBColor& c) {
    return sf::Color(c.r,c.g,c.b);
}


namespace CMPUT350 {


DrawContext::DrawContext(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Font> font)
    : mWindow(window), mFont(font) {}


inline sf::Text DrawContext::GetSfText(const std::string &text, int pixelSize, Point2D p,RGBColor c) {

    sf::Text my_text(*mFont, text, pixelSize);
    my_text.setFillColor(sf::Color(c.r,c.g,c.b));
    my_text.setPosition(sf::Vector2f(p.x,p.y));
    return my_text;

}

void DrawContext::DrawCenteredText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {


    auto my_text = GetSfText(text, pixelSize, p, c);
    auto text_bounding_box = my_text.getLocalBounds();

    my_text.setOrigin(text_bounding_box.position + (text_bounding_box.size / 2.0f));
    // The texts local coordinate system goes beyoned an sf::Text's bounding box.
    // The origin of an sf::Text WILL (by default) be (0,0)
    // But that marks the origin of an sf::Texts local origin coordinate system, not the top left corner of thebounding box
    // So we have to offset FROM the top left of the bounding box to its center.


    mWindow->draw(my_text);



}

void DrawContext::DrawText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {

     mWindow->draw(GetSfText(text, pixelSize, p, c));

}

void DrawContext::DrawCircle(Point2D p, float radius, RGBColor c) {

    sf::CircleShape circle(radius); // FIXME: Should probably throw some error if the radius is 0.

    circle.setPosition(sf::Vector2f(p.x, p.y));
    circle.setFillColor(sf::Color(c.r,c.g,c.b));

    mWindow->draw(circle);


}

void DrawContext::DrawRect(Rect r, RGBColor c) {


    sf::RectangleShape rectangle;

    rectangle.setPosition({r.topLeft.x, r.topLeft.y});
    rectangle.setSize(sf::Vector2f(r.width,r.height));
    rectangle.setFillColor(sf::Color(c.r,c.g,c.b));

    mWindow->draw(rectangle);


}

void DrawContext::FrameRect(Rect r, float width, RGBColor c) {}

/**
 * @brief Draws a line between two points with a specified width and color.
 *
 * @param from The starting point of the line (Point2D).
 * @param to The ending point of the line (Point2D).
 * @param width The width of the line in pixels.
 * @param c The color of the line, specified as an RGBColor object.
 *
 * This function calculates the distance and angle between the two points
 * and uses a polygone shape to represent the line. The line is drawn
 * relative to the world offset and rendered onto the associated window.
 */
void DrawContext::DrawLine(Point2D from, Point2D to, float width, RGBColor c) {

    Point2D end((to.x - from.x),(from.y - to.y));
    end.Normalize();
    end*=(width/2); // perpendicular vector that goes to each vertex of the rectangle.

    sf::ConvexShape line;

    auto relative = to - from; // vector from 'from' to 'ti'

    line.setPointCount(4);
    
    line.setPoint(0, GetSfVec( end));
    line.setPoint(1, GetSfVec(relative + end));

    line.setPoint(2, GetSfVec(relative - end));
    line.setPoint(3, GetSfVec(end * -1));

    line.setFillColor(GetSfColor(c));
    line.setPosition(GetSfVec(from));

    mWindow->draw(line);



    






}

int DrawContext::GetWindowWidth() { return mWindow->getSize().x; }

int DrawContext::GetWindowHeight() { return mWindow->getSize().y; }

}  // namespace CMPUT350
