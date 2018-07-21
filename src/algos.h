#include <deque>
#include <queue>
#include <cstdint>
#include "component_data.h"

#ifndef SRC_ALGOS_H
#define SRC_ALGOS_H

void one_component_at_a_time8(cv::Mat &image, std::deque<ComponentData> &data) {
    data.clear();
    auto m = image.size().height;
    auto n = image.size().width;
    std::queue<cv::Point2i> q;
    cv::Point2i cur_comp(0, 0);
    uchar cur_number = 1;
    while (true) {
        while (cur_comp.y != m && image.at<uchar>(cur_comp.y, cur_comp.x) != 1) {
            if (cur_comp.x != n - 1) {
                ++cur_comp.x;
            } else {
                ++cur_comp.y;
                cur_comp.x = 0;
            }
        }
        if (cur_comp.y != m) {
            q.push(cur_comp);
            ++cur_number;
            image.at<uchar>(cur_comp.y, cur_comp.x) = cur_number;
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
            if (top.y > 0 && image.at<uchar>(top.y - 1, top.x) == 1) {
                q.push(cv::Point2i(top.x, top.y - 1));
                image.at<uchar>(top.y - 1, top.x) = cur_number;
            }
            if (top.x > 0 && image.at<uchar>(top.y, top.x - 1) == 1) {
                q.push(cv::Point2i(top.x - 1, top.y));
                image.at<uchar>(top.y, top.x - 1) = cur_number;
            }
            if (top.y < m - 1 && image.at<uchar>(top.y + 1, top.x) == 1) {
                q.push(cv::Point2i(top.x, top.y + 1));
                image.at<uchar>(top.y + 1, top.x) = cur_number;
            }
            if (top.x < n - 1 && image.at<uchar>(top.y, top.x + 1) == 1) {
                q.push(cv::Point2i(top.x + 1, top.y));
                image.at<uchar>(top.y, top.x + 1) = cur_number;
            }
            if (top.y > 0 && top.x > 0 && image.at<uchar>(top.y - 1, top.x - 1) == 1) {
                q.push(cv::Point2i(top.x - 1, top.y - 1));
                image.at<uchar>(top.y - 1, top.x - 1) = cur_number;
            }
            if (top.x > 0 && top.y < m - 1 && image.at<uchar>(top.y + 1, top.x - 1) == 1) {
                q.push(cv::Point2i(top.x - 1, top.y + 1));
                image.at<uchar>(top.y + 1, top.x - 1) = cur_number;
            }
            if (top.y < m - 1 && top.x < n - 1 && image.at<uchar>(top.y + 1, top.x + 1) == 1) {
                q.push(cv::Point2i(top.x + 1, top.y + 1));
                image.at<uchar>(top.y + 1, top.x + 1) = cur_number;
            }
            if (top.x < n - 1 && top.y > 0 && image.at<uchar>(top.y - 1, top.x + 1) == 1) {
                q.push(cv::Point2i(top.x + 1, top.y - 1));
                image.at<uchar>(top.y - 1, top.x + 1) = cur_number;
            }
        }
        data.emplace_back(cur_data);
    }
    if (data.empty()) {
        return;
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

void one_component_at_a_time4(cv::Mat &image,
                              std::deque<ComponentData> &data) {
    data.clear();
    auto m = image.size().height;
    auto n = image.size().width;
    std::queue<cv::Point2i> q;
    cv::Point2i cur_comp(0, 0);
    uchar cur_number = 1;
    while (true) {
        while (cur_comp.y != m && image.at<uchar>(cur_comp.y, cur_comp.x) != 1) {
            if (cur_comp.x != n - 1) {
                ++cur_comp.x;
            } else {
                ++cur_comp.y;
                cur_comp.x = 0;
            }
        }
        if (cur_comp.y != m) {
            q.push(cur_comp);
            ++cur_number;
            image.at<uchar>(cur_comp.y, cur_comp.x) = cur_number;
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
            if (top.y > 0 && image.at<uchar>(top.y - 1, top.x) == 1) {
                q.push(cv::Point2i(top.x, top.y - 1));
                image.at<uchar>(top.y - 1, top.x) = cur_number;
            }
            if (top.x > 0 && image.at<uchar>(top.y, top.x - 1) == 1) {
                q.push(cv::Point2i(top.x - 1, top.y));
                image.at<uchar>(top.y, top.x - 1) = cur_number;
            }
            if (top.y < m - 1 && image.at<uchar>(top.y + 1, top.x) == 1) {
                q.push(cv::Point2i(top.x, top.y + 1));
                image.at<uchar>(top.y + 1, top.x) = cur_number;
            }
            if (top.x < n - 1 && image.at<uchar>(top.y, top.x + 1) == 1) {
                q.push(cv::Point2i(top.x + 1, top.y));
                image.at<uchar>(top.y, top.x + 1) = cur_number;
            }
        }
        data.emplace_back(cur_data);
    }
    if (data.empty()) {
        return;
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

void one_component_at_a_time(cv::Mat &image,
                             std::deque<ComponentData> &data,
                             int connectivity=8) {
    if (connectivity == 8) {
        one_component_at_a_time8(image, data);
    } else if (connectivity == 4) {
        one_component_at_a_time4(image, data);
    } else {
        throw std::runtime_error("Wrong value of connectivity!");
    }
}

#endif //SRC_ALGOS_H
