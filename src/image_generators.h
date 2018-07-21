#ifndef SRC_IMAGE_GENERATORS_H
#define SRC_IMAGE_GENERATORS_H

#include <opencv2/opencv.hpp>
#include <random>

const int HEIGHT = 1024, WIDTH = 576;
//constexpr int HEIGHT = 576, WIDTH = 1024;

namespace gen {
    cv::Mat matrix_of_ones(int height=HEIGHT, int width=WIDTH) {
        cv::Mat result(height, width, CV_8UC1);
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                result.at<uchar>(i, j) = 1;
            }
        }
        return result;
    }

    cv::Mat square_comp_rand_size(double p = 0.5, int height = HEIGHT, int width = WIDTH) {
        cv::Mat result(height, width, CV_8UC1);
        std::random_device device;
        std::default_random_engine gen(device());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        const int HEIGHT_SM = height / 5, WIDTH_SM = width / 4;
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

    cv::Mat square_comp_max_area(double p = 0.5, int height = HEIGHT, int width = WIDTH) {
        cv::Mat result(height, width, CV_8UC1);
        std::random_device device;
        std::default_random_engine gen(device());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        const int HEIGHT_SM = height / 5, WIDTH_SM = width / 4;
        auto width_lb = static_cast<int>(static_cast<double>(WIDTH_SM) / std::sqrt(2)) + 1;
        auto height_lb = static_cast<int>(static_cast<double>(HEIGHT_SM) / std::sqrt(2)) + 1;
        int left = 0;
        for (int i = 0; i < 5; ++i) {
            int up = 0;
            for (int j = 0; j < 4; ++j) {
                if (distr(gen) > p) {
                    up += WIDTH_SM;
                    continue;
                }
                for (int k = left; k < left + height_lb; ++k) {
                    for (int l = up; l < up + width_lb; ++l) {
                        result.at<uchar>(k, l) = 1;
                    }
                }
                up += WIDTH_SM;
            }
            left += HEIGHT_SM;
        }
        return result;
    }
}
#endif //SRC_IMAGE_GENERATORS_H
