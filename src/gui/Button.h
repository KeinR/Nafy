#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <string>
#include <functional>

#include "renderable.h"
#include "../shaders/Shader.h"
#include "Rectangle.h"
#include "../core/callback.h"
#include "TextRec.h"
#include "Color.h"

#include "../text/Text.h"
#include "../text/TextCrawl.h"

#include "../math/Polygon.h"
#include "../math/Circle.h"
#include "../math/Bounds.h"

/*
* A pretty intense abstraction, ButtonBase is a template that is typedef'd
* to Button and CrawlButton, both wrapping a TextRec and a TextRecCrawl respectively.
* It extends the renderable class so that it can function as a renderable node, and
* mouseMoveCallback and mouseClickCallback so that it can fulfill the functions of a button.
* 
* Button (Both Button and CrawlButton) functions by taking user events and invoking user-provided
* callbacks (onClick, onEnter, etc.) depending on the position of the mouse and its state.
* It implements a lot of the setters and getters of its wrapped class because it needs to adjust its
* node locations.
* The nodes are how the button keeps track of the corners of its displayed area. It detects with the
* conventional hitbox, however since TextRec*s can have curved corners, it must also determine if the
* user is hovering over or has clicked the button by taking the hypotenuse of the triangle made from
* the mouse to the inner corner. The inner corner is found by going the distance of the corner radius
* into the heart of the button - like if the button were an actual circle, it'd be it's center.
* I may be a little unclear, so here's a diagram:
* 
* Here we have a button with rounded corners
*   ________
*  /        \ <- here's one such corner (pretend it's actually rounded)
*  |        |
*  |    -> _| The place where those two lines meet (kinda' pointed at by the arrow) is the "inner corner",
*  \_______|/
* 
* The button takes the distance from that point, and checks if the distance between that and the mouse
* is less than the radius of the corner.
* 
* The "conventional hitbox" may be a little overkill, but it's needed because those curved corners can have
* _different radii_ (please save me). This means that we use - yes that's right - polygons to determine if the
* mouse is hovering over the button. For details, please see ../math/Polygon.h 
* 
* It's worthy to note that the button doesn't do anything on its own when it's clicked, etc. so as to provide
* the user with the most control (aside from perhaps later, mouse detection will be done in the render loop, TODO) 
*/

// TODO: Abstract this to where it's just detecting moust clicks

namespace nafy {
    template<class T, class S>
    class ButtonBase: public renderable, public mouseMoveCallback, public mouseClickCallback {
    public:
        typedef std::function<void(ButtonBase<T,S> *caller)> move_callback_func;
        typedef std::function<void(ButtonBase<T,S> *caller, int button, int mods)> press_callback_func;
    private:
        T display;

        // Should the button detect user input and dispatch the events to its callbacks?
        bool enabled;

        // Cached data relating to the position of the nodes.
        // Used to detect if the user is hovering over or pressing the button
        // Nodes: the smoothed corners
        Circle nodes[4];
        // boxes: the inner polygon sections of the button
        Polygon boxes[5];
        // bounds: used to efficiently determine if the mouse is close enough
        // to warrent more accurate calculation
        Bounds bounds;

        // User-defined callbacks
        press_callback_func onClick;
        press_callback_func onRelease;
        move_callback_func onEnter;
        move_callback_func onLeave;

        // Is the mouse hovering over the button right now?
        bool hovering;

        // Initialize all parts of the button
        void init();
        // Updates the mouse detection nodes (nodes, boxes, bounds)
        void updateNodes();
        // Does this button contain that point?
        // Uses bounds, nodes and boxes to determine so, in that order.
        bool containPoint(double xPos, double yPos);
        // Disable checking of mouse input events by removing itself from the Context's dispatch
        void disable();
        // Enable checking of mouse input events by adding itself from the Context's dispatch
        void enable();
        void steal(ButtonBase &other);
        void copy(const ButtonBase &other);
    public:
        // Takes default sprite shader, prim shader, and font from the current context.
        ButtonBase();
        // Alternatively, specify them yourself.
        // These are passed along to the wrapped TextRec*
        // textShader requirements: model, sampler0
        // shapeShader requirements: model, color
        ButtonBase(const Font::type &textFont, const shader_t &textShader, const shader_t &shapeShader);
        ~ButtonBase();
        ButtonBase(ButtonBase &&other);
        ButtonBase(const ButtonBase &other);
        ButtonBase &operator=(ButtonBase &&other);
        ButtonBase &operator=(const ButtonBase &other);

        // Set callbacks
        void setOnClick(const press_callback_func &callback);
        void setOnRelease(const press_callback_func &callback);
        void setOnEnter(const move_callback_func &callback);
        void setOnLeave(const move_callback_func &callback);

        // Get callbacks
        press_callback_func getOnClick();
        press_callback_func getOnRelease();
        move_callback_func getOnEnter();
        move_callback_func getOnLeave();

        // Generic setters
        void setX(int value);
        void setY(int value);
        void setWidth(unsigned int value);
        void setHeight(unsigned int value);
        // Margin is the distance between the edge of
        // the inner text and the outer edge of the rectangle in
        // TextRec*
        void setMargin(unsigned int value);

        // Generic getters
        int getX();
        int getY();
        unsigned int getWidth();
        unsigned int getHeight();
        unsigned int getMargin();

        // Corner radius setters
        // Set the corner radius for all corners
        void setCornerRadius(unsigned int radius);
        // Set by index; going clockwise, starting from top-left inclusive
        void setCornerRadius(int index, unsigned int radius);
        // One corner at a time
        void setCornerRadiusTopLeft(unsigned int radius);
        void setCornerRadiusTopRight(unsigned int radius);
        void setCornerRadiusBottomRight(unsigned int radius);
        void setCornerRadiusBottomLeft(unsigned int radius);

        // Corner radius getters
        void getCornerRadii(unsigned int data[4]);
        unsigned int getCornerRadius(int index);
        unsigned int getCornerRadiusTopLeft();
        unsigned int getCornerRadiusTopRight();
        unsigned int getCornerRadiusBottomRight();
        unsigned int getCornerRadiusBottomLeft();

        // Set/get the enabled status, ie, if it will
        // process and dispatch events, of the button
        void setEnabled(bool value);
        bool isEnabled();

        // Is the mouse hovering over this button?
        bool isHovering();

        // Simulate a mouse press and release on this button,
        // calling the onClick and onRelease functions
        void trigger();
        // Calls generate() on TextRec, important to finalize the display
        // Will also properly center the Textrec* if the wrapped text's textAlign
        // is set to Font::textAlign::center
        void generate();
        // Renders the display
        void render() override;
    
        // Input callbacks for the Context to call
        void mouseMoved(double mouseX, double mouseY) override;
        void mouseClicked(bool isPressed, int button, int mods) override;

        // wrapped in display, defined here for convinience
        Color &getColor();
        Rectangle &getBox();
        S &getText();

        // Get the wrapped display
        T &getDisplay();
    };

    // Aliase ugly templates to cleaner names
    typedef ButtonBase<TextRec, Text> Button;
    typedef ButtonBase<TextRecCrawl, TextCrawl> CrawlButton;
}

#endif
