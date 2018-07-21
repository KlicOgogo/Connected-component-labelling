#ifndef SRC_COMPONENT_DATA_H
#define SRC_COMPONENT_DATA_H

#include <opencv2/opencv.hpp>
#include <utility>

class ComponentData {
public:
    cv::Rect2i border;
    uchar number{};
    ComponentData() = default;
    ComponentData(const ComponentData &data) = default;
    ComponentData(cv::Rect2i border_, uchar number_): border(std::move(border_)), number(number_) {}
};

#endif //SRC_COMPONENT_DATA_H
