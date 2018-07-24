#ifndef SRC_ONE_COMP_H
#define SRC_ONE_COMP_H

#include <deque>
#include <queue>
#include <opencv2/opencv.hpp>
#include "component_data.h"

inline void push_and_update(cv::Mat &image, std::queue<cv::Point2i> &q,
                            const cv::Point &p, const uchar &value) noexcept {
    q.push(p);
    image.at<uchar>(p.x) = value;
}

inline void push_and_update_left_border(cv::Mat &image, std::queue<cv::Point2i> &q,
                                        ComponentData &data, const cv::Point &p, const uchar &value) noexcept {
    push_and_update(image, q, p, value);
    if (p.y < data.left) {
        data.left = p.y;
    }
}

inline void push_and_update_right_border(cv::Mat &image, std::queue<cv::Point2i> &q,
                                         ComponentData &data, const cv::Point2i &p, const uchar &value) noexcept {
    push_and_update(image, q, p, value);
    if (p.y > data.right) {
        data.right = p.y;
    }

}

inline void push_and_update_top_border(cv::Mat &image, std::queue<cv::Point2i> &q,
                                       ComponentData &data, const cv::Point2i &p, const uchar &value) noexcept {
    push_and_update(image, q, p, value);
    if (p.x < data.top) {
        data.top = p.x;
    }
}

inline void push_and_update_bottom_border(cv::Mat &image, std::queue<cv::Point2i> &q,
                                          ComponentData &data, const cv::Point2i &p, const uchar &value) noexcept {
    push_and_update(image, q, p, value);
    if (p.x > data.bottom) {
        data.bottom = p.x;
    }
}

inline void bfs_step1d4(cv::Mat &image, std::queue<cv::Point2i> &q,
                        ComponentData &data, const uchar &value, const int &out) noexcept {
    auto cur = q.front();
    q.pop();
    if (cur.x > image.size().width - 1) {
        if (image.at<uchar>(cur.x - image.size().width) == 1) {
            push_and_update_top_border(image, q, data,
                                       cv::Point2i(cur.x - image.size().width, cur.y), value);
        }
        if (cur.y > 0 && image.at<uchar>(cur.x - 1) == 1) {
            push_and_update_left_border(image, q, data, cv::Point2i(cur.x - 1, cur.y - 1), value);
        }
    } else {
        if (cur.y > 0 && image.at<uchar>(cur.x - 1) == 1) {
            push_and_update_left_border(image, q, data, cv::Point2i(cur.x - 1, cur.y - 1), value);
        }
    }
    if (cur.x + image.size().width < out) {
        if (image.at<uchar>(cur.x + image.size().width) == 1) {
            push_and_update_bottom_border(image, q, data,
                                          cv::Point2i(cur.x + image.size().width, cur.y), value);
        }
        if (cur.y < image.size().width - 1 && image.at<uchar>(cur.x + 1) == 1) {
            push_and_update_right_border(image, q, data, cv::Point2i(cur.x + 1, cur.y + 1), value);
        }
    } else {
        if (cur.y < image.size().width - 1 && image.at<uchar>(cur.x + 1) == 1) {
            push_and_update_right_border(image, q, data, cv::Point2i(cur.x + 1, cur.y + 1), value);
        }
    }
}

inline void bfs_step1d8(cv::Mat &image, std::queue<cv::Point2i> &q,
                        ComponentData &data, const uchar &value, const int &out) noexcept {
    auto cur = q.front();
    q.pop();
    auto n = image.size().width;
    if (cur.y > 0) {
        auto temp_y = cur.y - 1;
        if (image.at<uchar>(cur.x - 1) == 1) {
            push_and_update_left_border(image, q, data, cv::Point2i(cur.x - 1, temp_y), value);
        }
        if (cur.x > n && image.at<uchar>(cur.x - n - 1) == 1) {
            push_and_update_top_border(image, q, data, cv::Point2i(cur.x - 1 - n, temp_y), value);
            if (temp_y < data.left) {
                data.left = temp_y;
            }
        }
        if (cur.x + n <= out && image.at<uchar>(cur.x + n - 1) == 1) {
            push_and_update_bottom_border(image, q, data, cv::Point2i(cur.x - 1 + n, temp_y), value);
            if (temp_y < data.left) {
                data.left = temp_y;
            }
        }
    }
    if (cur.y < n - 1) {
        auto temp_y = cur.y + 1;
        if (image.at<uchar>(cur.x + 1) == 1) {
            push_and_update_right_border(image, q, data, cv::Point2i(cur.x + 1, temp_y), value);
        }
        if (cur.x > n - 2 && image.at<uchar>(cur.x + 1 - n) == 1) {
            push_and_update_top_border(image, q, data, cv::Point2i(cur.x + 1 - n, temp_y), value);
            if (temp_y > data.right) {
                data.right = temp_y;
            }
        }
        if (cur.x + n + 1 < out && image.at<uchar>(cur.x + n + 1) == 1) {
            push_and_update_bottom_border(image, q, data, cv::Point2i(cur.x + 1 + n, temp_y), value);
            if (temp_y > data.right) {
                data.right = temp_y;
            }
        }
    }
    if (cur.x >= n && image.at<uchar>(cur.x - n) == 1) {
        push_and_update_top_border(image, q, data, cv::Point2i(cur.x - n, cur.y), value);
    }
    if (cur.x + n < out && image.at<uchar>(cur.x + n) == 1) {
        push_and_update_bottom_border(image, q, data, cv::Point2i(cur.x + n, cur.y), value);
    }
}

void one_component_at_a_time1d(cv::Mat &image, std::deque<ComponentData> &data,
                               int connectivity = 8) noexcept {
    int out = image.size().width * image.size().height, n = image.size().width;
    data = {};
    std::queue<cv::Point2i> q;
    int comp_start(0);
    uchar cur_number = 1;
    while (comp_start != out) {
        while (comp_start != out && image.at<uchar>(comp_start) != 1) {
            ++comp_start;
        }
        if (comp_start != out) {
            ++cur_number;
            push_and_update(image, q, cv::Point2i(comp_start, comp_start % n), cur_number);
            ComponentData cur_data(comp_start, comp_start, comp_start % n,
                                   comp_start % n, cur_number);
            if (connectivity == 4) {
                while (!q.empty()) {
                    bfs_step1d4(image, q, cur_data, cur_number, out);
                }
            } else if (connectivity == 8) {
                while (!q.empty()) {
                    bfs_step1d8(image, q, cur_data, cur_number, out);
                }
            }
            data.emplace_back(cur_data);
        }
    }
    for (auto &i : data) {
        i.bottom /= n;
        i.top /= n;
    }
}

#endif //SRC_ONE_COMP_H
