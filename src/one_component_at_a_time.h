#ifndef SRC_ONE_COMPONENT_AT_A_TIME_H
#define SRC_ONE_COMPONENT_AT_A_TIME_H

#include <deque>
#include <queue>
#include <opencv2/opencv.hpp>
#include "component_data.h"

inline void find_component(cv::Mat &image, cv::Point2i &comp_start) noexcept {
    while (comp_start.y != image.size().height &&
           image.at<uchar>(comp_start.y, comp_start.x) != 1) {
        if (comp_start.x != image.size().width - 1) {
            ++comp_start.x;
        } else {
            ++comp_start.y;
            comp_start.x = 0;
        }
    }
}

inline void push_and_update(cv::Mat &image, std::queue<cv::Point2i> &q,
                            const int &x, const int &y, const uchar &value) noexcept {
    q.push(cv::Point2i(x, y));
    image.at<uchar>(y, x) = value;
}

inline void push_and_update_left_border(cv::Mat &image, std::queue<cv::Point2i> &q,
                            ComponentData &data, const int &x,
                            const int &y, const uchar &value) noexcept {
    push_and_update(image, q, x, y, value);
    if (x < data.left) {
        --data.left;
    }
}

inline void push_and_update_right_border(cv::Mat &image, std::queue<cv::Point2i> &q,
                                 ComponentData &data, const int &x,
                                 const int &y, const uchar &value) noexcept {
    push_and_update(image, q, x, y, value);
    if (x > data.right) {
        ++data.right;
    }
}

inline void push_and_update_top_border(cv::Mat &image, std::queue<cv::Point2i> &q,
                                 ComponentData &data, const int &x,
                                 const int &y, const uchar &value) noexcept {
    push_and_update(image, q, x, y, value);
    if (y < data.top) {
        --data.top;
    }
}

inline void push_and_update_bottom_border(cv::Mat &image, std::queue<cv::Point2i> &q,
                               ComponentData &data, const int &x,
                               const int &y, const uchar &value) noexcept {
    push_and_update(image, q, x, y, value);
    if (y > data.bottom) {
        ++data.bottom;
    }
}

inline void bfs_step2d4(cv::Mat &image, std::queue<cv::Point2i> &q,
                      ComponentData &data, const uchar &value) noexcept {
    cv::Point2i cur = q.front();
    q.pop();
    if (cur.x > 0 && image.at<uchar>(cur.y, cur.x - 1) == 1) {
        push_and_update_left_border(image, q, data, cur.x - 1, cur.y, value);
    }
    if (cur.x < image.size().width - 1 && image.at<uchar>(cur.y, cur.x + 1) == 1) {
        push_and_update_right_border(image, q, data, cur.x + 1, cur.y, value);
    }
    if (cur.y > 0 && image.at<uchar>(cur.y - 1, cur.x) == 1) {
        push_and_update_top_border(image, q, data, cur.x, cur.y - 1, value);
    }
    if (cur.y < image.size().height - 1 && image.at<uchar>(cur.y + 1, cur.x) == 1) {
        push_and_update_bottom_border(image, q, data, cur.x, cur.y + 1, value);
    }
}

inline void bfs_step2d8(cv::Mat &image, std::queue<cv::Point2i> &q,
                      ComponentData &data, const uchar &value) noexcept {
    auto m = image.size().height, n = image.size().width;
    cv::Point2i cur = q.front();
    q.pop();
    if (cur.x > 0) {
        auto temp_x = cur.x - 1;
        if (image.at<uchar>(cur.y, temp_x) == 1) {
            push_and_update_left_border(image, q, data, temp_x, cur.y, value);
        }
        if (cur.y > 0 && image.at<uchar>(cur.y - 1, temp_x) == 1) {
            auto temp_y = cur.y - 1;
            push_and_update_left_border(image, q, data, temp_x, temp_y, value);
            if (temp_y < data.top) {
                --data.top;
            }
        }
        if (cur.y < m - 1 && image.at<uchar>(cur.y + 1, temp_x) == 1) {
            push_and_update_left_border(image, q, data, temp_x, cur.y + 1, value);
            if (cur.y + 1 > data.bottom) {
                ++data.bottom;
            }
        }
    }
    if (cur.x < n - 1) {
        auto temp_x = cur.x + 1;
        if (image.at<uchar>(cur.y, temp_x) == 1) {
            push_and_update_right_border(image, q, data, temp_x, cur.y, value);
        }
        if (cur.y > 0 && image.at<uchar>(cur.y - 1, temp_x) == 1) {
            push_and_update_top_border(image, q, data, temp_x, cur.y - 1, value);
            if (temp_x > data.right) {
                ++data.right;
            }
        }
        if (cur.y < m - 1 && image.at<uchar>(cur.y + 1, temp_x) == 1) {
            push_and_update_right_border(image, q, data, temp_x, cur.y + 1, value);
            if (cur.y + 1 > data.bottom) {
                ++data.bottom;
            }
        }
    }
    if (cur.y > 0 && image.at<uchar>(cur.y - 1, cur.x) == 1) {
        push_and_update_top_border(image, q, data, cur.x, cur.y - 1, value);
    }
    if (cur.y < image.size().height - 1 && image.at<uchar>(cur.y + 1, cur.x) == 1) {
        push_and_update_bottom_border(image, q, data, cur.x, cur.y + 1, value);
    }
}

void one_component_at_a_time2d(cv::Mat &image, std::deque<ComponentData> &data,
                             int connectivity=8) noexcept {
    data = {};
    std::queue<cv::Point2i> q;
    cv::Point2i comp_start(0, 0);
    uchar cur_number = 1;
    while (comp_start.y != image.size().height) {
        find_component(image, comp_start);
        if (comp_start.y != image.size().height) {
            ++cur_number;
            push_and_update(image, q, comp_start.x, comp_start.y, cur_number);
            ComponentData cur_data(comp_start.y, comp_start.y, comp_start.x, comp_start.x, cur_number);
            if (connectivity == 8) {
                while (!q.empty()) {
                    bfs_step2d8(image, q, cur_data, cur_number);
                }
            } else if (connectivity == 4) {
                while (!q.empty()) {
                    bfs_step2d4(image, q, cur_data, cur_number);
                }
            }
            data.emplace_back(cur_data);
        }
    }
}

#endif //SRC_ONE_COMPONENT_AT_A_TIME_H
