#include <deque>
#include <queue>
#include <cstdint>
#include "component_data.h"

#ifndef SRC_ALGOS_H
#define SRC_ALGOS_H

void one_component_at_a_time(cv::Mat &image,
                             std::deque<ComponentData> &data,
                             int connectivity = 4) {
    data.clear();
    auto m = image.size().height;
    auto n = image.size().width;
    std::queue<cv::Point2i> q;
    cv::Point2i cur_comp(0, 0);
    uchar cur_number = 1;
    while (true) {
        while (cur_comp.x != m && image.at<uchar>(cur_comp.x, cur_comp.y) != 1) {
            if (cur_comp.y != n - 1) {
                ++cur_comp.y;
            } else {
                ++cur_comp.x;
                cur_comp.y = 0;
            }
        }
        if (cur_comp.x != m) {
            q.push(cur_comp);
            ++cur_number;
            image.at<uchar>(cur_comp.x, cur_comp.y) = cur_number;
        } else {
            break;
        }
        ComponentData cur_data(cv::Rect2i(n, m, 0, 0), cur_number);
        while (!q.empty()) {
            cv::Point2i top = q.front();
            q.pop();
            cur_data.border_.x = std::min(cur_data.border_.x, top.x);
            cur_data.border_.y = std::min(cur_data.border_.y, top.y);
            cur_data.border_.width = std::max(cur_data.border_.width, top.x - cur_data.border_.x);
            cur_data.border_.height = std::max(cur_data.border_.height, top.y - cur_data.border_.y);
            if (top.x > 0 && image.at<uchar>(top.x - 1, top.y) == 1) {
                q.push(cv::Point2i(top.x - 1, top.y));
                image.at<uchar>(top.x - 1, top.y) = cur_number;
            }
            if (top.y > 0 && image.at<uchar>(top.x, top.y - 1) == 1) {
                q.push(cv::Point2i(top.x, top.y - 1));
                image.at<uchar>(top.x, top.y - 1) = cur_number;
            }
            if (top.x < m - 1 && image.at<uchar>(top.x + 1, top.y) == 1) {
                q.push(cv::Point2i(top.x + 1, top.y));
                image.at<uchar>(top.x + 1, top.y) = cur_number;
            }
            if (top.y < n - 1 && image.at<uchar>(top.x, top.y + 1) == 1) {
                q.push(cv::Point2i(top.x, top.y + 1));
                image.at<uchar>(top.x, top.y + 1) = cur_number;
            }
            if (connectivity == 8) {
                if (top.x > 0 && top.y > 0 && image.at<uchar>(top.x - 1, top.y - 1) == 1) {
                    q.push(cv::Point2i(top.x - 1, top.y - 1));
                    image.at<uchar>(top.x - 1, top.y - 1) = cur_number;
                }
                if (top.y > 0 && top.x < m - 1 && image.at<uchar>(top.x + 1, top.y - 1) == 1) {
                    q.push(cv::Point2i(top.x + 1, top.y - 1));
                    image.at<uchar>(top.x + 1, top.y - 1) = cur_number;
                }
                if (top.x < m - 1 && top.y < n - 1 && image.at<uchar>(top.x + 1, top.y + 1) == 1) {
                    q.push(cv::Point2i(top.x + 1, top.y + 1));
                    image.at<uchar>(top.x + 1, top.y + 1) = cur_number;
                }
                if (top.y < n - 1 && top.x > 0 && image.at<uchar>(top.x - 1, top.y + 1) == 1) {
                    q.push(cv::Point2i(top.x - 1, top.y + 1));
                    image.at<uchar>(top.x - 1, top.y + 1) = cur_number;
                }
            }
        }
        data.emplace_back(cur_data);
    }
    data.back().number_ = 1;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (image.at<uchar>(i, j) == cur_number) {
                image.at<uchar>(i, j) = 1;
            }
        }
    }
}

#endif //SRC_ALGOS_H
