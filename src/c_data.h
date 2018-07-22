#ifndef SRC_C_DATA_H
#define SRC_C_DATA_H

#include <opencv2/core/hal/interface.h>

class CompData {
public:
    int top{}, bottom{}, left{}, right{};
    uchar number{};
    CompData() = default;
    CompData(const CompData &data) = default;
    CompData(int top_, int bottom_, int left_,
             int right_, uchar number_): top(top_), bottom(bottom_),
                                         left(left_), right(right_), number(number_) {}
};

#endif //SRC_C_DATA_H
