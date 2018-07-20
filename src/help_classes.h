#ifndef SRC_HELP_CLASSES_H
#define SRC_HELP_CLASSES_H

class Point {
public:
    Point() = default;
    Point(int x, int y): x_(x), y_(y) {}
    Point(const Point &p) = default;
    Point& operator=(const Point &p) {
        x_ = p.x();
        y_ = p.y();
        return *this;
    }
    bool operator !=(const Point &p) const {
        return (x_ != p.x()) || (y_ != p.y());
    }
    int x() const {
        return x_;
    }
    int y() const {
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
    Rectangle& operator=(const Rectangle &r) {
        left_top_ = r.left_top();
        right_bottom_ = r.right_bottom();
        return *this;
    }
    Point left_top() const {
        return left_top_;
    }
    Point right_bottom() const {
        return right_bottom_;
    }
    void set_left_top(const Point &left_top) {
        left_top_ = left_top;
    }
    void set_right_bottom(const Point &right_bottom) {
        right_bottom_ = right_bottom;
    }
private:
    Point left_top_, right_bottom_;
};

class ComponentData {
public:
    ComponentData() = default;
    ComponentData(Rectangle border, int number): border_(border),
                                                 number_(number) {}

    Rectangle border() const {
        return border_;
    }
    int number() const {
        return number_;
    }
    void set_border(const Rectangle &border) {
        border_ = border;
    }
    void set_number(int number) {
        number_ = number;
    }
private:
    Rectangle border_;
    int number_;
};


#endif //SRC_HELP_CLASSES_H
