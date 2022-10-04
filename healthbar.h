#include <simplecpp>

class Health
{
private:
    Rectangle rect; 
    // the rectangle representing the healthbar

public:
    Health(double center_x, double center_y, double width=12, double height=10, Color color=COLOR(0,0,0)) {
        // healthbar constructor
        rect = Rectangle(center_x, center_y, width, height);
        //color will be chosen while calling the constructor
        rect.setColor(color);
        rect.setFill(true);
    }
};