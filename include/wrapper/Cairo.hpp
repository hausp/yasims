
#ifndef WRAPPER_CAIRO_HPP
#define WRAPPER_CAIRO_HPP

#include <gtk/gtk.h>
#include <list>

struct Color {
    int r;
    int g;
    int b;
    int a;
};

class Cairo {
    struct area {
        int x;
        int y;
        int width;
        int height;
    };

 public:
    using Instance = int;

    Cairo();
    ~Cairo();

    void clear();
    void close_path();
    void destroy();
    bool draw(cairo_t*);
    void fill();
    void line_to(double, double);
    void move_to(double, double);
    void point(double, double);
    void rectangle(double, double, double, double);
    void set_color(double, double, double, double = 255);
    void set_color(Color);
    void stroke();
    void stroke_preserve();
    void queue_draw();
    bool update(GtkWidget*, GdkEventConfigure*, gpointer);
    bool drawed();

 private:
    GtkWidget* drawing_area = nullptr;
    cairo_t* cr = nullptr;
    cairo_surface_t*  surface = nullptr;
    gint handler_id;
    bool _drawed = true;

    void set_callbacks(GtkWidget*);
};

inline bool Cairo::drawed() {
    return _drawed;
}


#endif /* WRAPPER_CAIRO_HPP */
