
#include "wrapper/Cairo.hpp"

void Cairo::clear() {
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
}

void Cairo::close_path() {
    cairo_close_path(cr);
}

void Cairo::destroy() {
    if (surface) cairo_surface_destroy(surface);
    if (cr) cairo_destroy(cr);
}

bool Cairo::draw(cairo_t* _cr) {
    cairo_set_source_surface(_cr, surface, 0, 0);
    cairo_paint(_cr);
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
    cairo_set_source_rgba(cr, r / 255, g / 255, b / 255, a);
}

void Cairo::stroke() {
    cairo_stroke(cr);
}

void Cairo::stroke_preserve() {
    cairo_stroke_preserve(cr);
}

bool Cairo::update(GtkWidget* widget) {
    if (surface) cairo_surface_destroy(surface);
    if (cr) cairo_destroy(cr);

    surface = gdk_window_create_similar_surface(
        gtk_widget_get_window(widget),
        CAIRO_CONTENT_COLOR,
        gtk_widget_get_allocated_width(widget),
        gtk_widget_get_allocated_height(widget)
    );

    cr = cairo_create(surface);

    clear();

    return true;
}