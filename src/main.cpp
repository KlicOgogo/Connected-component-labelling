//#include <fstream>
#include "test.h"
#include "image_generators.h"

int main() {
    for (int i = 0; i < 6; ++i) {
        auto imm = gen::figure_components(0.5, 2, 576, 1024);
        cv::imshow("Generated", imm);
        cv::waitKey(0);
    }
    /*
    std::ifstream test_image("test.txt");
    cv::Mat imm(cv::Mat::zeros(10, 10, CV_8UC1));
    //auto im = gen::square_comp_rand_size(0.5);
    std::string str;
    int i = 0;
    while(test_image >> str) {
        for (int j = 0; j < str.size(); ++j) {
            imm.at<uchar>(i, j) = static_cast<uchar>(str[j] - '0');
        }
        ++i;
    }
    std::deque<ComponentData> comp_data;
    one_component_at_a_time1d(imm, comp_data, 4);
    */
    /*
    bmark::test_dim(0);
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
    */
    return 0;
}