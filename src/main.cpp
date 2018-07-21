#include <fstream>
#include "test.h"

int main() {
    /*
    std::chrono::milliseconds a(10), b(100), c(1000);
    std::cout << ((a + b) + c).count() << '\n';
    std::ifstream test_image("test.txt");
    std::vector<std::vector<char>> data;
    cv::Mat chmo(10, 10, CV_8UC1);
    data.reserve(100);
    auto im = generate(0.5);
    std::string str;
    int i = 0;
    while(test_image >> str) {
        data.emplace_back(std::vector<char>());
        data.back().resize(str.size());
        for (int j = 0; j < str.size(); ++j) {
            data[i][j] = str[j] - '0';
            chmo.at<uchar>(i, j) = static_cast<uchar>(data[i][j]);
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
    bmark::test(0);
    bmark::test(1);
    bmark::test(2);
    bmark::test(3);
    bmark::test(0, 4);
    bmark::test(1, 4);
    bmark::test(2, 4);
    bmark::test(3, 4);
}