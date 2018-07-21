#ifndef SRC_COMPONENT_DATA_H
#define SRC_COMPONENT_DATA_H

#include <opencv2/opencv.hpp>
#include <utility>

class ComponentData {
public:
    cv::Rect2i border_;
    uchar number_{};
    ComponentData() = default;
    ComponentData(const ComponentData &data) = default;
    ComponentData(cv::Rect2i border, uchar number): border_(std::move(border)), number_(number) {}
};

#endif //SRC_COMPONENT_DATA_H
