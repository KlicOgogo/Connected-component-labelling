#ifndef SRC_TEST_H
#define SRC_TEST_H

#include <chrono>
#include <iostream>
#include <deque>
#include <opencv2/opencv.hpp>
#include "image_generators.h"
#include "one_component_at_a_time.h"
#include "component_data.h"
#include "one_comp.h"

const std::deque<std::string> TEST_TYPES = {"Dense", "Mean", "Sparse", "Median"};

/*
 * range 0 - generate matrices with different expected number of components;
 * range 1 - generate matrices with 18 expected components;
 * range 2 - generate matrices with 2 expected components;
 * range 3 - generate matrices with 10 expected components;
 */
const std::deque<std::deque<float>> P_RANGES = {std::deque<float>(20, 0.9),
                                                std::deque<float>({0.05, 0.1, 0.15, 0.2, 0.25,
                                                                   0.3, 0.35, 0.4, 0.45, 0.5,
                                                                   0.55, 0.6, 0.65, 0.7, 0.75,
                                                                   0.8, 0.85, 0.9, 0.95, 1.}),
                                                std::deque<float>(20, 0.1),
                                                std::deque<float>(20, 0.5)};
const std::deque<std::string> FIG_TYPES = {"Grid rectangles", "Random rectangles",
                                           "Grid rhombuses", "Random rhombuses",
                                           "Grid triangles", "Random triangles",
                                           "Grid rect. triangles", "Random rect. triangles",
                                           "Grid something", "Random something"};

using namespace std::chrono;

namespace bmark {
    constexpr int N_TIMES = 100;


    void test_dim_func() noexcept {
        std::deque<std::string> types = {"(x + y) % c < d", "((x + y) * x * y / (1 + x * x + y * y )) % C < d"};
        for (int connectivity = 4; connectivity < 9; connectivity += 4) {
            for (int type = 0; type < 2; ++type) {
                std::chrono::milliseconds dim2_sum_time(0), dim1_sum_time(0);
                for (int i = 0; i < N_TIMES; ++i) {
                    auto image1 = gen::func_components(type);
                    auto image2 = image1.clone();
                    std::deque<ComponentData> data1, data2;
                    auto dim1_start = steady_clock::now();
                    one_component_at_a_time1d(image1, data1, connectivity);
                    dim1_sum_time += duration_cast<milliseconds>(steady_clock::now() - dim1_start);
                    auto dim2_start = steady_clock::now();
                    one_component_at_a_time2d(image2, data2, connectivity);
                    dim2_sum_time += duration_cast<milliseconds>(steady_clock::now() - dim2_start);
                }
                std::cout << types[type] << " generator test with " << connectivity << "-connectivity:\n";
                std::cout << "My one component at a time algorithm (1-dimensional):\naverage time = " <<
                          static_cast<double>(dim1_sum_time.count()) / N_TIMES << " milliseconds.\n";
                std::cout << "My one component at a time algorithm (2-dimensional):\naverage time = " <<
                          static_cast<double>(dim2_sum_time.count()) / N_TIMES << " milliseconds.\n\n";
            }
        }
    }

    void test() {
        cv::setNumThreads(1);
        cv::setUseOptimized(false);
        for (int connectivity = 4; connectivity < 9; connectivity += 4) {
            for (int type = 0; type < TEST_TYPES.size(); ++type) {
                for (const std::string &fig_type : FIG_TYPES) {
                    std::chrono::milliseconds my_sum_time(0), opencv_sum_time(0);
                    for (int j = 0; j < N_TIMES / 10; ++j) {
                        for (const float &p : P_RANGES[type]) {
                            auto image = gen::by_fig_type(fig_type, p);
                            std::deque<ComponentData> data;
                            cv::Mat labels, centroids, stats;
                            auto opencv_start = steady_clock::now();
                            cv::connectedComponentsWithStats(image, labels, stats, centroids, connectivity);
                            opencv_sum_time += duration_cast<milliseconds>(steady_clock::now() - opencv_start);
                            auto my_start = steady_clock::now();
                            //one_component_at_a_time1d(image, data, connectivity);
                            my_sum_time += duration_cast<milliseconds>(steady_clock::now() - my_start);
                        }
                    }
                    std::cout << TEST_TYPES[type] << " test with " << connectivity <<
                              "-connectivity and" << fig_type << "generator :\n";
                    std::cout << "My one component at a time algorithm:\naverage time = " <<
                              static_cast<double>(my_sum_time.count()) / (2 * N_TIMES) << " milliseconds.\n";
                    std::cout << "OpenCV algorithm:\naverage time = " <<
                              static_cast<double>(opencv_sum_time.count()) / (2 * N_TIMES) << " milliseconds.\n\n";
                }
            }
        }
    }

    void test_dim() {
        for (int connectivity = 4; connectivity < 9; connectivity += 4) {
            for (int type = 0; type < TEST_TYPES.size(); ++type) {
                for (const std::string &fig_type : FIG_TYPES) {
                    std::chrono::milliseconds dim1_sum_time(0), dim2_sum_time(0);
                    for (int j = 0; j < N_TIMES / 10; ++j) {
                        for (const float &p : P_RANGES[type]) {
                            auto image1 = gen::by_fig_type(fig_type, p);
                            auto image2 = image1.clone();
                            std::deque<ComponentData> data1, data2;
                            auto dim2_start = steady_clock::now();
                            one_component_at_a_time2d(image2, data2, connectivity);
                            dim2_sum_time += duration_cast<milliseconds>(steady_clock::now() - dim2_start);
                            auto dim1_start = steady_clock::now();
                            one_component_at_a_time1d(image1, data1, connectivity);
                            dim1_sum_time += duration_cast<milliseconds>(steady_clock::now() - dim1_start);
                        }
                    }
                    std::cout << TEST_TYPES[type] << " test with " << connectivity <<
                              "-connectivity and " << fig_type << " generator :\n";
                    std::cout << "My one component at a time algorithm (1-dimensional):\naverage time = " <<
                              static_cast<double>(dim1_sum_time.count()) / (2 * N_TIMES) << " milliseconds.\n";
                    std::cout << "My one component at a time algorithm (2-dimensional):\naverage time = " <<
                              static_cast<double>(dim2_sum_time.count()) / (2 * N_TIMES) << " milliseconds.\n\n";
                }
            }
        }
    }
}
#endif //SRC_TEST_H
