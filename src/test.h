#ifndef SRC_TEST_H
#define SRC_TEST_H

#include <chrono>
#include "img_gen.h"
#include <opencv2/opencv.hpp>
#include "algos.h"
#include <iostream>
#include <vector>

using namespace std::chrono;

namespace bmark {
    constexpr int N_TIMES = 10;

    void test(int type = 0, int connectivity = 8) {
        std::vector<float> p_range;
        std::vector<std::string> types = {"Average", "Dense", "Sparse", "Median"};
        if (type == 0) { // different component number
            p_range = {0.05, 0.1, 0.15, 0.2, 0.25,
                       0.3, 0.35, 0.4, 0.45, 0.5,
                       0.55, 0.6, 0.65, 0.7, 0.75,
                       0.8, 0.85, 0.9, 0.95, 1.};
        } else if (type == 1) {
            p_range = std::vector<float>(20, 0.9);
        } else if (type == 2) {
            p_range = std::vector<float>(20, 0.1);
        } else if (type == 3) {
            p_range = std::vector<float>(20, 0.5);
        } else {
            throw std::runtime_error("Wrong value of type parameter");
        }
        std::chrono::milliseconds my_alg_sum_time(0), opencv_alg_sum_time(0);
        for (int i = 0; i < N_TIMES; ++i) {
            for (const float &p : p_range) {
                auto image = generate_image(p);
                std::deque<ComponentData> data;
                cv::Mat labels, centroids, stats;
                auto opencv_alg_start = steady_clock::now();
                cv::connectedComponentsWithStats(image, labels, stats, centroids, connectivity);
                opencv_alg_sum_time += duration_cast<milliseconds>(steady_clock::now() - opencv_alg_start);
                auto my_alg_start = steady_clock::now();
                one_component_at_a_time(image, data, connectivity);
                my_alg_sum_time += duration_cast<milliseconds>(steady_clock::now() - my_alg_start);
            }
        }
        std::cout << types[type] << " test with " << connectivity << "-connectivity:\n";
        std::cout << "My one component at a time algorithm:\naverage time = " <<
                  my_alg_sum_time.count() / (20 * N_TIMES) << " milliseconds\n";
        std::cout << "OpenCV algorithm:\naverage time = " <<
                  opencv_alg_sum_time.count() / (20 * N_TIMES) << " milliseconds\n\n";
    }
}
#endif //SRC_TEST_H
