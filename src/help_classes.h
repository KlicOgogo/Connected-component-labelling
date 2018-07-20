#ifndef SRC_HELP_CLASSES_H
#define SRC_HELP_CLASSES_H

class Point {
public:
    Point() = default;
    Point(int x, int y): x_(x), y_(y_) {}
    Point(const Point &p) = default;
    int x() {
        return x_;
    }
    int y() {
        return y_;
    }
    void setX(int x) {
        x_ = x;
    }
    void setY(int y) {
        y_ = y;
    }
private:
    int x_, y_;
};

class Rectangle {
public:
    Rectangle() = default;
    Rectangle(const Point &left_top,
              const Point &right_bottom): left_top_(left_top),
                                          right_bottom_(right_bottom) {}
    Rectangle(const Rectangle &r) = default;
private:
    Point left_top_, right_bottom_;
};

class ComponentData {
public:
    ComponentData() = default;
    ComponentData(Rectangle border, int number): border_(border),
                                                 number_(number) {}
private:
    Rectangle border_;
    int number_;
};


#endif //SRC_HELP_CLASSES_H
