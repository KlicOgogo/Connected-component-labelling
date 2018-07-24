#ifndef SRC_TEST_H
#define SRC_TEST_H

#include <chrono>
#include <iostream>
#include <deque>
#include <opencv2/opencv.hpp>
#include "image_generators.h"
#include "component_data.h"
#include "algorithm.h"
#include <iomanip>

using namespace std::chrono;

constexpr int N_TIMES_FUNC = 100, N_TIMES = 5;

void test_func() noexcept {
    std::cout << "FUNCTIONAL GENERATORS SPEED TESTS:\n\n";
    std::cout << std::fixed;
    cv::setNumThreads(1);
    cv::setUseOptimized(false);
    std::deque<std::string> types = {"(x + y) % C < d", "((x + y) * x * y / (1 + x * x + y * y )) % C < d"};
    for (const int &type : {0, 1}) {
        std::cout << types[type] << " generator test:\n";
        std::deque<float> my_data, opencv_data;
        for (const int &connectivity : {8, 4}) {
            milliseconds my_sum_time(0), opencv_sum_time(0);
            for (int i = 0; i < N_TIMES_FUNC; ++i) {
                auto image = gen::func_components(type);
                std::deque<ComponentData> data;
                cv::Mat labels, centroids, stats;
                auto opencv_start = steady_clock::now();
                cv::connectedComponentsWithStats(image, labels, stats, centroids, connectivity);
                opencv_sum_time += duration_cast<milliseconds>(steady_clock::now() - opencv_start);
                auto my_start = steady_clock::now();
                one_component_at_a_time(image, data, connectivity);
                my_sum_time += duration_cast<milliseconds>(steady_clock::now() - my_start);
            }
            my_data.emplace_back(static_cast<float>(my_sum_time.count()) / N_TIMES_FUNC);
            opencv_data.emplace_back(static_cast<float>(opencv_sum_time.count()) / N_TIMES_FUNC);
        }
        std::cout << std::setw(30) << " " << std::setw(20) <<
                  "8-connectivity" << std::setw(18) << "4-connectivity\n";
        std::cout << "    My algorithm av. time (in ms.):" << std::setw(15) <<
                  std::setprecision(2) << my_data[0] << std::setw(17) << my_data[1];
        std::cout << "\nOpenCV algorithm av. time (in ms.):" << std::setw(15) <<
                  std::setprecision(2) << opencv_data[0] << std::setw(17) << opencv_data[1] << "\n\n";
    }
}


void test() {
    std::cout << "STOCHASTIC GENERATORS SPEED TESTS:\n\n";
    std::cout << std::fixed;
    cv::setNumThreads(1);
    cv::setUseOptimized(false);
    const std::deque<std::string> TEST_TYPES = {"Dense", "Mean", "Sparse", "Median"};
    const std::deque<std::deque<float>> P_RANGES = {std::deque<float>(20, 0.95),
                                                    std::deque<float>({0.05, 0.1, 0.15, 0.2, 0.25,
                                                                       0.3, 0.35, 0.4, 0.45, 0.5,
                                                                       0.55, 0.6, 0.65, 0.7, 0.75,
                                                                       0.8, 0.85, 0.9, 0.95, 1.}),
                                                    std::deque<float>(20, 0.1),
                                                    std::deque<float>(20, 0.5)};
    const std::deque<std::string> FIG_TYPES = {"Grid rectangles", "Random rectangles", "Grid big rectangles",
                                               "Grid rhombuses", "Random rhombuses", "Grid big rhombuses",
                                               "Grid triangles", "Random triangles", "Grid big triangles",
                                               "Grid rect. triangles", "Random rect. triangles",
                                               "Grid big rect. triangles", "Grid something",
                                               "Random something", "Grid big something"};
    for (int type = 0; type < TEST_TYPES.size(); ++type) {
        const int DIVISOR = N_TIMES * P_RANGES[type].size();
        for (const int &connectivity : {8, 4}) {
            std::cout << std::setw(6) << TEST_TYPES[type] << " test (" << connectivity << "-connectivity):\n";
            std::deque<float> my_data, opencv_data;
            for (const std::string &fig_type : FIG_TYPES) {
                milliseconds my_sum_time(0), opencv_sum_time(0);
                for (int j = 0; j < N_TIMES; ++j) {
                    for (const float &p : P_RANGES[type]) {
                        auto image = gen::by_fig_type(fig_type, p);
                        std::deque<ComponentData> data;
                        cv::Mat labels, centroids, stats;
                        auto opencv_start = steady_clock::now();
                        cv::connectedComponentsWithStats(image, labels, stats, centroids, connectivity);
                        opencv_sum_time += duration_cast<milliseconds>(steady_clock::now() - opencv_start);
                        auto my_start = steady_clock::now();
                        one_component_at_a_time(image, data, connectivity);
                        my_sum_time += duration_cast<milliseconds>(steady_clock::now() - my_start);
                    }
                }
                opencv_data.emplace_back(static_cast<float>(opencv_sum_time.count()) / DIVISOR);
                my_data.emplace_back(static_cast<float>(my_sum_time.count()) / DIVISOR);
            }
            std::cout << "    My algorithm av. time (in ms.):";
            for (const float &time : my_data) {
                std::cout << std::setw(7) << std::setprecision(2) << time;
            }
            std::cout << "\nOpenCV algorithm av. time (in ms.):";
            for (const float &time : opencv_data) {
                std::cout << std::setw(7) << std::setprecision(2) << time;
            }
            std::cout << "\n\n";
        }
    }
}

#endif //SRC_TEST_H
