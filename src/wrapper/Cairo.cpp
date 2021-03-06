
#include "wrapper/Cairo.hpp"
#include "wrapper/Signal.hpp"
#include <iostream>

namespace {
    struct DrawCallback {
        static bool callback(GtkWidget* w, cairo_t* cr, gpointer c) {
            return static_cast<Cairo*>(c)->draw(cr);
        }
    };
}

Cairo::Cairo() {
    aw::Signal<Cairo>::set_receiver(*this);
}

Cairo::~Cairo() {
    destroy();
}

void Cairo::clear() {
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
}

void Cairo::close_path() {
    cairo_close_path(cr);
}

void Cairo::destroy() {
    cairo_destroy(cr);
}

bool Cairo::draw(cairo_t* _cr) {
    cairo_set_source_surface(_cr, surface, 0, 0);
    cairo_paint(_cr);
    _drawed = true;
    return false;
}

void Cairo::fill() {
    cairo_fill(cr);
}

void Cairo::line_to(double x, double y) {
    cairo_set_line_width(cr, 2);
    cairo_line_to(cr, x, y);
}

void Cairo::move_to(double x, double y) {
    cairo_move_to(cr, x, y);
}

void Cairo::point(double x, double y) {
    cairo_set_line_width(cr, 2);
    cairo_rectangle(cr, x - 1, y - 1, 2, 2);
    cairo_fill(cr);
}

void Cairo::rectangle(double x, double y, double l, double h) {
    cairo_rectangle(cr, x, y, l, h);
}

void Cairo::set_color(double r, double g, double b, double a) {
    cairo_set_source_rgba(cr, r / 255, g / 255, b / 255, a / 255);
}

void Cairo::set_color(Color c) {
    cairo_set_source_rgba(cr, c.r / 255, c.g / 255, c.b / 255, c.a / 255);
}

void Cairo::stroke() {
    cairo_stroke(cr);
}

void Cairo::stroke_preserve() {
    cairo_stroke_preserve(cr);
}

bool Cairo::update(GtkWidget* widget, GdkEventConfigure*, gpointer) {
    cairo_surface_destroy(surface);
    cairo_destroy(cr);

    surface = gdk_window_create_similar_surface(
        gtk_widget_get_window(widget),
        CAIRO_CONTENT_COLOR,
        gtk_widget_get_allocated_width(widget),
        gtk_widget_get_allocated_height(widget)
    );

    cr = cairo_create(surface);

    clear();

    if (drawing_area != widget) {
        set_callbacks(widget);
    }

    return true;
}

void Cairo::set_callbacks(GtkWidget* widget) {
    if (drawing_area) {
        g_signal_handler_disconnect(widget, handler_id);
    }

    drawing_area = widget;

    handler_id = g_signal_connect(
        widget,
        "draw", 
        G_CALLBACK(&DrawCallback::callback),
        static_cast<gpointer>(this)
    );
}

void Cairo::queue_draw() {
    _drawed = false;
    gtk_widget_queue_draw(drawing_area);
}