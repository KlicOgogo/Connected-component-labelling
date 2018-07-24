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

constexpr int N_TIMES_FUNC = 50, N_TIMES = 5;

void test_func() noexcept {
    std::cout << "\nFUNCTIONAL GENERATORS SPEED TESTS:\n\n";
    cv::setNumThreads(1);
    cv::setUseOptimized(false);
    std::deque<std::string> types = {"(x + y) % C < d", "((x + y) * x * y / (1 + x * x + y * y )) % C < d"};
    for (int type = 0; type < 2; ++type) {
        for (const int &connectivity : {8, 4}) {
            std::chrono::milliseconds my_sum_time(0), opencv_sum_time(0);
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
            std::cout << types[type] << " generator test with " << connectivity << "-connectivity:\n";
            std::cout << "    My algorithm: average time: " << std::setw(7) <<
                      static_cast<double>(my_sum_time.count()) / N_TIMES_FUNC << " ms.\n";
            std::cout << "OpenCV algorithm: average time: " << std::setw(7) <<
                      static_cast<double>(opencv_sum_time.count()) / N_TIMES_FUNC << " ms.\n\n";
        }
    }
}


void test() {
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
                                               "Grid rect. triangles", "Random rect. triangles", "Grid big rect. triangles",
                                               "Grid something", "Random something", "Grid big something"};
    for (const int &connectivity : {8, 4}) {
        for (int type = 0; type < TEST_TYPES.size(); ++type) {
            for (const std::string &fig_type : FIG_TYPES) {
                std::chrono::milliseconds my_sum_time(0), opencv_sum_time(0);
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
                std::cout << TEST_TYPES[type] << " test with " << connectivity <<
                          "-connectivity and " << fig_type << " generator:\n";
                std::cout << "My one component at a time algorithm:\naverage time = " <<
                          static_cast<double>(my_sum_time.count()) / N_TIMES / P_RANGES[type].size() <<
                          " milliseconds.\n";
                std::cout << "OpenCV algorithm:\naverage time = " <<
                          static_cast<double>(opencv_sum_time.count()) / N_TIMES / P_RANGES[type].size() <<
                          " milliseconds.\n\n";
            }
        }
    }
}

#endif //SRC_TEST_H
