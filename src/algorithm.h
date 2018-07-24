#ifndef SRC_ALGORITHM_H
#define SRC_ALGORITHM_H

#include <deque>
#include <opencv2/opencv.hpp>
#include "component_data.h"

void one_component_at_a_time(cv::Mat &image, std::deque<ComponentData> &data,
                             int connectivity = 8) noexcept;

#endif //SRC_ALGORITHM_H
