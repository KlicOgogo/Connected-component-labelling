#ifndef SRC_COMPONENT_DATA_H
#define SRC_COMPONENT_DATA_H

class ComponentData {
public:
    int top{}, bottom{}, left{}, right{};
    uchar number{};
    ComponentData() = default;
    ComponentData(const ComponentData &data) = default;
    ComponentData(int top_, int bottom_, int left_,
             int right_, uchar number_): top(top_), bottom(bottom_),
                                         left(left_), right(right_), number(number_) {}
};

#endif //SRC_COMPONENT_DATA_H
