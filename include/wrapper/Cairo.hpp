
#ifndef WRAPPER_CAIRO_HPP
#define WRAPPER_CAIRO_HPP

#include <gtk/gtk.h>
#include <list>
#include <mutex>

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
    void set_color(double, double, double, double = 1);
    void stroke();
    void stroke_preserve();
    bool update(GtkWidget*, GdkEventConfigure*, gpointer);
    void set_callbacks(GtkWidget*);

    bool request();
    void draw_request();

    bool done();

 private:
    GtkWidget* drawing_area = nullptr;
    cairo_t* cr = nullptr;
    cairo_surface_t*  surface = nullptr;
    gint handler_id;
    bool _done = true;
    std::mutex mutex;

};

inline bool Cairo::done() {
    std::lock_guard<std::mutex> lock(mutex);
    return _done;
}

#endif /* WRAPPER_CAIRO_HPP */
