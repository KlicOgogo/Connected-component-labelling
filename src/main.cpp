#include <fstream>
#include "test.h"
#include <opencv2/opencv.hpp>
#include "one_comp.h"

int main() {
    /*
    cv::Mat chmo(10, 20, CV_8UC1);
    uchar k = 0;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 20; ++j, ++k) {
            chmo.at<uchar>(i, j) = k;
        }
    }
    for (int i = 0; i < 200; ++i) {
        std::cout << int(chmo.at<uchar>(i)) << ' ';
        if (i % 20 == 19) {
            std::cout << '\n';
        }
    }
    std::cout << int(chmo.at<uchar>(201));
    */
    /*
    std::ifstream test_image("test.txt");
    std::vector<std::vector<char>> data;
    cv::Mat imm(10, 10, CV_8UC1);
    data.reserve(100);
    auto im = gen::square_comp_rand_size(0.5);
    std::string str;
    int i = 0;
    while(test_image >> str) {
        data.emplace_back(std::vector<char>());
        data.back().resize(str.size());
        for (int j = 0; j < str.size(); ++j) {
            data[i][j] = str[j] - '0';
            imm.at<uchar>(i, j) = static_cast<uchar>(data[i][j]);
        }
        ++i;
    }
    cv::Mat labels, centroids, stats;
    cv::connectedComponentsWithStats(chmo, labels, stats, centroids);
    std::deque<ComponentData> comp_data;
    one_component_at_a_time(im, comp_data);
    cv::imshow("components", im);
    cv::waitKey(0);
    return 0;
    */

    bmark::test_dim(0, 4);
    bmark::test_dim(1);
    bmark::test_dim(2);
    bmark::test_dim(3);
    bmark::test_dim(0, 4);
    bmark::test_dim(1, 4);
    bmark::test_dim(2, 4);
    bmark::test_dim(3, 4);
    bmark::test(0);
    bmark::test(1);
    bmark::test(2);
    bmark::test(3);
    bmark::test(0, 4);
    bmark::test(1, 4);
    bmark::test(2, 4);
    bmark::test(3, 4);
    return 0;

}