#ifndef SRC_IMAGE_GENERATORS_H
#define SRC_IMAGE_GENERATORS_H

#include <random>

constexpr int HEIGHT = 1024, WIDTH = 576;
//constexpr int HEIGHT = 576, WIDTH = 1024;


namespace gen {
    /*
     * Function divides image into 20 equal(almost) parts
     * and generate a rectangle of some size (depends on type) inside them with probability p.
     * max_size == false - random size, smaller than part_size / sqrt(2);
     * max_size == true - size equal to window size / sqrt(2);
     */
    cv::Mat rect_components(double p = 0.5, bool max_size = false, int height = HEIGHT, int width = WIDTH) {
        cv::Mat result(cv::Mat::zeros(height, width, CV_8UC1));
        std::random_device device;
        std::default_random_engine gen(device());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        const int HEIGHT_SM = height / 5, WIDTH_SM = width / 4;
        auto width_lb = static_cast<int>(static_cast<double>(WIDTH_SM) / std::sqrt(2));
        auto height_lb = static_cast<int>(static_cast<double>(HEIGHT_SM) / std::sqrt(2));
        int up = 0;
        for (int i = 0; i < 5; ++i) {
            int left = 0;
            for (int j = 0; j < 4; ++j) {
                if (distr(gen) < p) {
                    int comp_width{}, comp_height{};
                    if (!max_size) {
                        comp_width = width_lb - 40 + static_cast<int>(41 * distr(gen));
                        comp_height = height_lb - 40 + static_cast<int>(41 * distr(gen));
                    } else {
                        comp_width = width_lb;
                        comp_height = height_lb;
                    }
                    for (int k = up; k < up + comp_height; ++k) {
                        for (int l = left; l < left + comp_width; ++l) {
                            result.at<uchar>(k, l) = 1;
                        }
                    }
                }
                left += WIDTH_SM;
            }
            up += HEIGHT_SM;
        }
        return result;
    }

    /* Same idea but generating rhombuses or triangles and sometimes they can intersect each other.
     * type == 0 - rhombus;
     * type == 1 - triangle;
     * type == 2 - rectangular triangle;
     */
    cv::Mat figure_components(double p = 0.5, int type = 0, int height = HEIGHT, int width = WIDTH) {
        cv::Mat result(cv::Mat::zeros(height, width, CV_8UC1));
        std::random_device device;
        std::default_random_engine gen(device());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        const int HEIGHT_SM = height / 5, WIDTH_SM = width / 4;
        int up = 0;
        for (int i = 0; i < 5; ++i) {
            int left = 0;
            for (int j = 0; j < 4; ++j) {
                if (distr(gen) < p) {
                    auto rh_size = static_cast<int>((std::min(HEIGHT_SM, WIDTH_SM) / 3 + 50 * distr(gen)));
                    cv::Point2i center(left + WIDTH_SM / 2, up + HEIGHT_SM / 2);
                    result.at<uchar>(center) = 100;
                    std::deque<cv::Point2i> q_old, q_new;
                    q_old.emplace_back(center);
                    for (int k = 0; k < rh_size; ++k) {
                        while (!q_old.empty()) {
                            auto top = q_old.front();
                            q_old.pop_front();
                            if (top.x > 0 && result.at<uchar>(top.y, top.x - 1) != 100) {
                                result.at<uchar>(top.y, top.x - 1) = 100;
                                q_new.emplace_back(cv::Point2i(top.x - 1, top.y));
                            }
                            if (top.y > 0 && result.at<uchar>(top.y - 1, top.x) != 100) {
                                result.at<uchar>(top.y - 1, top.x) = 100;
                                q_new.emplace_back(cv::Point2i(top.x, top.y - 1));
                            }
                            if (type == 0 || type == 1) {
                                if (top.x < result.size().width - 1 && result.at<uchar>(top.y, top.x + 1) != 100) {
                                    result.at<uchar>(top.y, top.x + 1) = 100;
                                    q_new.emplace_back(cv::Point2i(top.x + 1, top.y));
                                }
                            }
                            if (type == 0) {
                                if (top.y < result.size().height - 1 && result.at<uchar>(top.y + 1, top.x) != 100) {
                                    result.at<uchar>(top.y + 1, top.x) = 100;
                                    q_new.emplace_back(cv::Point2i(top.x, top.y + 1));
                                }
                            }
                        }
                        q_old = q_new;
                        q_new = {};
                    }
                }
                left += WIDTH_SM;
            }
            up += HEIGHT_SM;
        }
        return result;
    }

    /* generates an image using mapping from coordinates to bool.
     * type == 0 => (x + y) % c < d, where (y,x) - image pixel;
     * type == 1 => (x | y) % c < d, where (y,x) - image pixel;
     * type == 2 => (x ^ y) % c < d, where (y,x) - image pixel;
     */
    cv::Mat func_components(int type = 0, int height = HEIGHT, int width = WIDTH) {
        cv::Mat result(cv::Mat::zeros(height, width, CV_8UC1));
        std::random_device device;
        std::default_random_engine gen(device());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        auto lambda0 = [](int x, int y, int c, int d) { return (x + y) % c < d; };
        auto lambda3 = [](int x, int y, int c, int d) { return (x | y) % c < d; };
        auto lambda4 = [](int x, int y, int c, int d) { return (x ^ y) % c < d; };
        std::vector<bool (*)(int, int, int, int)> lambdas = {lambda0, lambda3, lambda4};
        int d = 45 - static_cast<int>(20 * distr(gen));
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (lambdas[type](i, j, 100, d)) {
                    result.at<uchar>(i, j) = 1;
                }
            }
        }
        return result;
    }
}
#endif //SRC_IMAGE_GENERATORS_H
