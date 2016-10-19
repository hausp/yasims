
#ifndef SPAWNER_HPP
#define SPAWNER_HPP

#include "wrapper/Cairo.hpp"

class Spawner {
 public:
    Spawner(double, double, double = 30, double = 30);

    void draw(Cairo&);
 private:
    double x;
    double y;
    double width;
    double height;
    int count = 0;
    Color color = {255, 255, 255, 255};
};

#endif /* SPAWNER_HPP */
