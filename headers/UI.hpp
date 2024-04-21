#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <string>
#include <vector>
#include <functional>


class UIComponent {
public:
    Rectangle bounds;
    virtual std::string Name() = 0;
    virtual void Draw() = 0;
    virtual bool HandleClick(Vector2 click_position) = 0;

};

class Label : public UIComponent {
public:
    std::string text;
    Color color;

    std::string Name()
    {
        return text;
    }

    void Draw() override
    {
        DrawText(text.c_str(), bounds.x, bounds.y, 64, color);
    }

    bool HandleClick(Vector2 click_position) override
    {
        return false;
    }
};

class Button : public UIComponent {
public:
    std::string text;
    std::function<void()> customClickHandler;

    int width;

    std::string Name()
    {
        return text;
    }

    void Draw() override
    {
        DrawRectangleRec(bounds, GRAY);
        DrawText(text.c_str(), bounds.x + ((width - MeasureText(text.c_str(), 20))/2), bounds.y + 16, 20, BLACK);
    }

    bool HandleClick(Vector2 click_position) override
    {
        if (CheckCollisionPointRec(click_position, bounds))
        {
            if (customClickHandler) customClickHandler();
            return true;
        }

        return false;
    }
};

class CheckBox : public UIComponent {
public:
    std::string text;
    std::string alttext = "default";
    std::string display;

    bool state = true;
    Color color;

    std::function<void()> customClickHandler;

    std::string Name()
    {
        return display;
    }

    void Draw() override
    {
        DrawRectangleRec({bounds.x-2, bounds.y-2, bounds.width+2, bounds.height+2}, BLACK);
        if (state)
        {
            color = BLUE;
            display = text;
        }
        else
        {
            color = RED;
            if (alttext=="default")
            {
                display = text;
            }
            else
            {
                display = alttext;
            }
        }
        DrawRectangleRec(bounds, color);
        DrawText(display.c_str(), bounds.x + bounds.width + 4, bounds.y, 20, BLACK);
    }

    bool HandleClick(Vector2 click_position) override
    {
        if (CheckCollisionPointRec(click_position, bounds))
        {
            if (customClickHandler) customClickHandler();

            if (state)
            {
                state = false;
            }
            else
            {
                state = true;
            }
            return true;
        }

        return false;
    }
};

class UIContainer : public UIComponent {
public:
    std::vector<UIComponent*> children;

    std::string Name()
    {
        return "";
    }

    void AddChild(UIComponent* child)
    {
        children.push_back(child);
    }

    void Draw() override
    {
        for (size_t i = 0; i < children.size(); ++i)
        {
            if (children[i])
            {
                children[i]->Draw();
            }
        }
    }

    bool HandleClick(Vector2 click_position) override
    {
        for (size_t i = children.size(); i > 0; --i)
        {
            if (children[i - 1]->HandleClick(click_position))
            {
                return true;
            }
        }

        return false;
    }
};

class UILibrary {
public:
    UIContainer root_container;

    void Update()
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            root_container.HandleClick(GetMousePosition());
        }
    }

    void Draw()
    {
        root_container.Draw();
    }
};