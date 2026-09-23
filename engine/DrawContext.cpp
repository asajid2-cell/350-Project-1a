#include "DrawContext.h"
#include <SFML/Graphics.hpp>


namespace CMPUT350 {

DrawContext::DrawContext(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Font> font)
    : mWindow(window), mFont(font) {}

void DrawContext::DrawCenteredText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {

}

void DrawContext::DrawText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {

    sf::Text my_text(*mFont, text, pixelSize);
    my_text.setFillColor(sf::Color(c.r,c.g,c.b));
    my_text.setPosition(sf::Vector2f(p.x,p.y));
    mWindow->draw(my_text);

}

void DrawContext::DrawCircle(Point2D p, float radius, RGBColor c) {
    /*
    Let me use this function to illustrate my confusion:
    When we write Galaga, suppose we have a BulletObject that takes the shape of a circle.
    Exactly how are we supposed to use this function? We need the actual sf::Shape stored somewhere for when we 
    want toe bullet to change positions. The point of DrawContext is so that Galaga doesnt have to worry about sfml. But 
    I'm not sure what DrawContext is supposed to accomplish if the draw functions DONT return an sf::Shape to assign to a GraphicsObject.
    
    */

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




}

int DrawContext::GetWindowWidth() { return mWindow->getSize().x; }

int DrawContext::GetWindowHeight() { return mWindow->getSize().y; }

}  // namespace CMPUT350
