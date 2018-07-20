#ifndef SRC_IMG_GEN_H
#define SRC_IMG_GEN_H

#include <opencv2/opencv.hpp>
#include <random>

constexpr int height = 1024, width = 576;
constexpr int height_sm = 205, width_sm = 144;


cv::Mat generate_image(double p=0.5) {
    cv::Mat result(height, width, CV_8UC1);
    std::random_device device;
    std::default_random_engine gen(device());
    std::uniform_real_distribution<double> distr(0.0, 1.0);
    int left = 0;
    for (int i = 0; i < 5; ++i) {
        int up = 0;
        for (int j = 0; j < 4; ++j) {
            if (distr(gen) > p) {
                up += width_sm;
                continue;
            }
            int comp_width = 61 + static_cast<int>(41 * distr(gen));
            int comp_height = 104 + static_cast<int>(41 * distr(gen));
            for (int k = left; k < left + comp_height; ++k) {
                for (int l = up; l < up + comp_width; ++l) {
                    result.at<uchar>(k, l) = 1;
                }
            }
            up += width_sm;
        }
        left += height_sm;
    }
    return result;
}

#endif //SRC_IMG_GEN_H
