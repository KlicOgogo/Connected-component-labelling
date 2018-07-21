#ifndef SRC_IMG_GEN_H
#define SRC_IMG_GEN_H

#include <opencv2/opencv.hpp>
#include <random>

constexpr int HEIGHT = 1024, WIDTH = 576;
constexpr int HEIGHT_SM = 205, WIDTH_SM = 144;


cv::Mat generate(double p = 0.5) {
    cv::Mat result(HEIGHT, WIDTH, CV_8UC1);
    std::random_device device;
    std::default_random_engine gen(device());
    std::uniform_real_distribution<double> distr(0.0, 1.0);
    auto width_lb = static_cast<int>(static_cast<double>(WIDTH_SM) / std::sqrt(2)) - 40;
    auto height_lb = static_cast<int>(static_cast<double>(HEIGHT_SM) / std::sqrt(2)) - 40;
    int left = 0;
    for (int i = 0; i < 5; ++i) {
        int up = 0;
        for (int j = 0; j < 4; ++j) {
            if (distr(gen) > p) {
                up += WIDTH_SM;
                continue;
            }
            int comp_width = width_lb + static_cast<int>(41 * distr(gen));
            int comp_height = height_lb + static_cast<int>(41 * distr(gen));
            for (int k = left; k < left + comp_height; ++k) {
                for (int l = up; l < up + comp_width; ++l) {
                    result.at<uchar>(k, l) = 1;
                }
            }
            up += WIDTH_SM;
        }
        left += HEIGHT_SM;
    }
    return result;
}

#endif //SRC_IMG_GEN_H
