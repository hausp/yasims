
#ifndef WRAPPER_CAIRO_HPP
#define WRAPPER_CAIRO_HPP

#include <gtk/gtk.h>

class Cairo {
 public:

    void clear();
    void close_path();
    void destroy();
    bool draw(cairo_t*);
    void fill();
    void line_to(double, double);
    void move_to(double, double);
    void point(double, double);
    void rectangle(double, double, double, double);
    void set_color(double, double, double, double = 1);
    void stroke();
    void stroke_preserve();
    bool update(GtkWidget*);

 private:
    cairo_t* cr = nullptr;
    cairo_surface_t*  surface = nullptr;
};

#endif /* WRAPPER_CAIRO_HPP */
