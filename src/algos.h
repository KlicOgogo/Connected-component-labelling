#include <deque>
#include <queue>
#include <cstdint>
#include "component_data.h"

#ifndef SRC_ALGOS_H
#define SRC_ALGOS_H


inline void find_unlabeled_component(cv::Mat &image, cv::Point2i &comp_start, const int &m, const int &n) {
    while (comp_start.y != m && image.at<uchar>(comp_start.y, comp_start.x) != 1) {
        if (comp_start.x != n - 1) {
            ++comp_start.x;
        } else {
            ++comp_start.y;
            comp_start.x = 0;
        }
    }
}

inline void update_last_component(cv::Mat &image, std::deque<ComponentData> &data,
                                  const uchar &cur_number, const int &m, const int &n) {
    if (data.empty()) {
        return;
    }
    data.back().number = 1;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (image.at<uchar>(i, j) == cur_number) {
                image.at<uchar>(i, j) = 1;
            }
        }
    }
}

void one_component_at_a_time8(cv::Mat &image, std::deque<ComponentData> &data) {
    data.clear();
    auto m = image.size().height;
    auto n = image.size().width;
    std::queue<cv::Point2i> q;
    cv::Point2i comp_start(0, 0);
    uchar cur_number = 1;
    while (true) {
        find_unlabeled_component(image, comp_start, m, n);
        if (comp_start.y != m) {
            q.push(comp_start);
            ++cur_number;
            image.at<uchar>(comp_start.y, comp_start.x) = cur_number;
        } else {
            break;
        }
        ComponentData cur_data(cv::Rect2i(n, m, 0, 0), cur_number);
        while (!q.empty()) {
            cv::Point2i top = q.front();
            q.pop();
            cur_data.border.x = std::min(cur_data.border.x, top.x);
            cur_data.border.y = std::min(cur_data.border.y, top.y);
            cur_data.border.width = std::max(cur_data.border.width, top.x - cur_data.border.x);
            cur_data.border.height = std::max(cur_data.border.height, top.y - cur_data.border.y);
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
    update_last_component(image, data, cur_number, m, n);
}

void one_component_at_a_time4(cv::Mat &image,
                              std::deque<ComponentData> &data) {
    data.clear();
    auto m = image.size().height;
    auto n = image.size().width;
    std::queue<cv::Point2i> q;
    cv::Point2i comp_start(0, 0);
    uchar cur_number = 1;
    while (true) {
        find_unlabeled_component(image, comp_start, m, n);
        if (comp_start.y != m) {
            q.push(comp_start);
            ++cur_number;
            image.at<uchar>(comp_start.y, comp_start.x) = cur_number;
        } else {
            break;
        }
        ComponentData cur_data(cv::Rect2i(n, m, 0, 0), cur_number);
        while (!q.empty()) {
            cv::Point2i top = q.front();
            q.pop();
            cur_data.border.x = std::min(cur_data.border.x, top.x);
            cur_data.border.y = std::min(cur_data.border.y, top.y);
            cur_data.border.width = std::max(cur_data.border.width, top.x - cur_data.border.x);
            cur_data.border.height = std::max(cur_data.border.height, top.y - cur_data.border.y);
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
    update_last_component(image, data, cur_number, m, n);
}

void one_component_at_a_time(cv::Mat &image,
                             std::deque<ComponentData> &data,
                             int connectivity=8) {
    if (connectivity == 8) {
        one_component_at_a_time8(image, data);
    } else if (connectivity == 4) {
        one_component_at_a_time4(image, data);
    } else {
        return;
    }
}

#endif //SRC_ALGOS_H
