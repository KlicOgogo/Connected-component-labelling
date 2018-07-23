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

using namespace std::chrono;

/*
 * type = 0 - generate matrices with different number of components
 * type = 1 - generate matrices with ~18 components
 * type = 2 - generate matrices with ~2 components
 * type = 3 - generate matrices with ~10 components
 */
std::deque<float> choose_p(int type = 0) {
    std::deque<float> p_range;
    if (type == 0) {
        p_range = std::deque<float>(20, 0.9);
    } else if (type == 1) {
        p_range = {0.05, 0.1, 0.15, 0.2, 0.25,
                   0.3, 0.35, 0.4, 0.45, 0.5,
                   0.55, 0.6, 0.65, 0.7, 0.75,
                   0.8, 0.85, 0.9, 0.95, 1.};
    } else if (type == 2) {
        p_range = std::deque<float>(20, 0.1);
    } else if (type == 3) {
        p_range = std::deque<float>(20, 0.5);
    } else {
        p_range = {};
    }
    return p_range;
}

namespace bmark {
    constexpr int N_TIMES = 5;

    void test(int type = 0, int connectivity = 8) {
        cv::setNumThreads(1);
        auto p_range = choose_p(type);
        std::deque<std::string> types = {"Dense", "Mean", "Sparse", "Median"};
        std::chrono::milliseconds my_sum_time(0), opencv_sum_time(0);
        for (int i = 0; i < N_TIMES; ++i) {
            for (const float &p : p_range) {
                auto image = gen::square_comp_rand_size(p);
                std::deque<ComponentData> data;
                cv::Mat labels, centroids, stats;
                auto opencv_start = steady_clock::now();
                cv::connectedComponentsWithStats(image, labels, stats, centroids, connectivity);
                opencv_sum_time += duration_cast<milliseconds>(steady_clock::now() - opencv_start);
                auto my_start = steady_clock::now();
                one_component_at_a_time1d(image, data, connectivity);
                //cv::imshow("Labelled", image);
                //cv::waitKey(0);
                my_sum_time += duration_cast<milliseconds>(steady_clock::now() - my_start);
            }
        }
        std::cout << types[type] << " test with " << connectivity << "-connectivity:\n";
        std::cout << "My one component at a time algorithm:\naverage time = " <<
                  my_sum_time.count() / (20 * N_TIMES) << " milliseconds.\n";
        std::cout << "OpenCV algorithm:\naverage time = " <<
                  opencv_sum_time.count() / (20 * N_TIMES) << " milliseconds.\n\n";
        std::cout << cv::getNumThreads() << " " << cv::getNumberOfCPUs() << "\n\n";
    }

    void test_dim(int type = 0, int connectivity = 8) {
        auto p_range = choose_p(type);
        std::deque<std::string> types = {"Dense", "Mean", "Sparse", "Median"};
        std::chrono::milliseconds dim2_sum_time(0), dim1_sum_time(0);
        for (int i = 0; i < N_TIMES; ++i) {
            for (const float &p : p_range) {
                auto image1 = gen::square_comp_rand_size(p);
                auto image2 = image1.clone();
                std::deque<ComponentData> data1, data2;
                cv::Mat labels, centroids, stats;
                auto dim1_start = steady_clock::now();
                one_component_at_a_time1d(image1, data1, connectivity);
                dim1_sum_time += duration_cast<milliseconds>(steady_clock::now() - dim1_start);
                auto dim2_start = steady_clock::now();
                one_component_at_a_time2d(image2, data2, connectivity);
                //cv::imshow("Labelled", image1);
                //cv::waitKey(0);
                dim2_sum_time += duration_cast<milliseconds>(steady_clock::now() - dim2_start);
            }
        }
        std::cout << types[type] << " test with " << connectivity << "-connectivity:\n";
        std::cout << "My one component at a time algorithm (2-dimensional):\naverage time = " <<
                  dim2_sum_time.count() / (20 * N_TIMES) << " milliseconds.\n";
        std::cout << "My one component at a time algorithm (1-dimensional):\naverage time = " <<
                  dim1_sum_time.count() / (20 * N_TIMES) << " milliseconds.\n\n";
    }
}
#endif //SRC_TEST_H
