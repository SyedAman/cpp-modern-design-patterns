#include <iostream>

class Rectangle
{
    float width;
    float height;

public:
    Rectangle(float width, float height)
        : width(width), height(height)
    {}

    float getWidth() const
    {
        return width;
    }

    float getHeight() const
    {
        return height;
    }

    void setWidth(float width)
    {
        this->width = width;
    }

    void setHeight(float height)
    {
        this->height = height;
    }

    virtual float area() const
    {
        return width * height;
    }

    virtual float perimeter() const
    {
        return 2 * (width + height);
    }
};

class Square : public Rectangle
{
public:
    Square(float side)
        : Rectangle(side, side)
    {}
};

class Renderer
{
public:
    virtual void draw(const Rectangle& rectangle, const char* penType) const = 0;
};

class RectangleRenderer : public Renderer
{
public:
    void draw(const Rectangle& rectangle, const char* penType) const override
    {
        // Do an ASCII art drawing of the rectangle
        std::cout << "Rectangle drawn using " << penType << std::endl;

        // Draw the top the rectangle
        for (int i = 0; i < rectangle.getWidth(); i++)
        {
            std::cout << penType;
        }
        std::cout << std::endl;

        // Draw the sides of the rectangle
        for (int i = 0; i < rectangle.getHeight() - 2; i++)
        {
            std::cout << penType;
            for (int j = 0; j < rectangle.getWidth() - 2; j++)
            {
                std::cout << " ";
            }
            std::cout << penType << std::endl;
        }

        // Draw the bottom of the rectangle
        for (int i = 0; i < rectangle.getWidth(); i++)
        {
            std::cout << penType;
        }

        std::cout << std::endl;
    }
};

void process(Rectangle& rectangle, int newWidth)
{
    int width = rectangle.getWidth();
    int oldArea = rectangle.area();
    
    rectangle.setWidth(newWidth);
    int newArea = rectangle.area();

    std::cout << "Old area: " << oldArea << std::endl;
    std::cout << "New area: " << newArea << std::endl;
}

int main()
{
    Rectangle rectangle(4, 5);
    RectangleRenderer rectangleRenderer;
    rectangleRenderer.draw(rectangle, "*");
    
    // Violation of Liskov Substitution Principle because we are ignoring either the width or the height of the rectangle.
    // Objects should be replaceable with their subtype without affecting the correctness of the program.
    process(rectangle, 10.0f);
    Square square(6.0f);
    process(square, 25.0f);

    return 0;
}