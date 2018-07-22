#include <deque>
#include <queue>
#include <cstdint>
#include "component_data.h"

#ifndef SRC_ONE_COMPONENT_AT_A_TIME_H
#define SRC_ONE_COMPONENT_AT_A_TIME_H

inline void find_start(cv::Mat &image, cv::Point2i &comp_start) {
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
                            const int &x, const int &y, const uchar &value) {
    q.push(cv::Point2i(x, y));
    image.at<uchar>(y, x) = value;
}

inline void push_and_update_left(cv::Mat &image, std::queue<cv::Point2i> &q,
                            ComponentData &cur_data, const int &x,
                            const int &y, const uchar &value) {
    push_and_update(image, q, x, y, value);
    if (x < cur_data.left) {
        --cur_data.left;
    }
}

inline void push_and_update_right(cv::Mat &image, std::queue<cv::Point2i> &q,
                                 ComponentData &cur_data, const int &x,
                                 const int &y, const uchar &value) {
    push_and_update(image, q, x, y, value);
    if (x > cur_data.right) {
        ++cur_data.right;
    }
}

inline void push_and_update_up(cv::Mat &image, std::queue<cv::Point2i> &q,
                                 ComponentData &cur_data, const int &x,
                                 const int &y, const uchar &value) {
    push_and_update(image, q, x, y, value);
    if (y < cur_data.top) {
        --cur_data.top;
    }
}

inline void push_and_update_down(cv::Mat &image, std::queue<cv::Point2i> &q,
                               ComponentData &cur_data, const int &x,
                               const int &y, const uchar &value) {
    push_and_update(image, q, x, y, value);
    if (y > cur_data.bottom) {
        ++cur_data.bottom;
    }
}

inline void bfs_step4(cv::Mat &image, std::queue<cv::Point2i> &q,
                      ComponentData &data, const uchar &value) {
    cv::Point2i top = q.front();
    q.pop();
    if (top.x > 0 && image.at<uchar>(top.y, top.x - 1) == 1) {
        push_and_update_left(image, q, data, top.x - 1, top.y, value);
    }
    if (top.x < image.size().width - 1 && image.at<uchar>(top.y, top.x + 1) == 1) {
        push_and_update_right(image, q, data, top.x + 1, top.y, value);
    }
    if (top.y > 0 && image.at<uchar>(top.y - 1, top.x) == 1) {
        push_and_update_up(image, q, data, top.x, top.y - 1, value);
    }
    if (top.y < image.size().height - 1 && image.at<uchar>(top.y + 1, top.x) == 1) {
        push_and_update_down(image, q, data, top.x, top.y + 1, value);
    }
}

inline void bfs_step4(cv::Mat &image, std::queue<cv::Point2i> &q,
                      const uchar &old_value, const uchar &new_value) {
    cv::Point2i top = q.front();
    q.pop();
    if (top.x > 0 && image.at<uchar>(top.y, top.x - 1) == old_value) {
        push_and_update(image, q, top.x - 1, top.y, new_value);
    }
    if (top.x < image.size().width - 1 &&
        image.at<uchar>(top.y, top.x + 1) == old_value) {
        push_and_update(image, q, top.x + 1, top.y, new_value);
    }
    if (top.y > 0 && image.at<uchar>(top.y - 1, top.x) == old_value) {
        push_and_update(image, q, top.x, top.y - 1, new_value);
    }
    if (top.y < image.size().height - 1 &&
        image.at<uchar>(top.y + 1, top.x) == old_value) {
        push_and_update(image, q, top.x, top.y + 1, new_value);
    }
}

inline void bfs_step8(cv::Mat &image, std::queue<cv::Point2i> &q,
                      const uchar &old_value, const uchar &new_value) {
    auto m = image.size().height, n = image.size().width;
    cv::Point2i top = q.front();
    q.pop();
    if (top.x > 0) {
        auto temp_x = top.x - 1;
        if (image.at<uchar>(top.y, temp_x) == old_value) {
            push_and_update(image, q, temp_x, top.y, new_value);
        }
        if (top.y > 0 && image.at<uchar>(top.y - 1, temp_x) == old_value) {
            push_and_update(image, q, temp_x, top.y - 1, new_value);
        }
        if (top.y < m - 1 && image.at<uchar>(top.y + 1, temp_x) == old_value) {
            push_and_update(image, q, temp_x, top.y + 1, new_value);
        }
    }
    if (top.x < n - 1) {
        auto temp_x = top.x + 1;
        if (image.at<uchar>(top.y, temp_x) == old_value) {
            push_and_update(image, q, temp_x, top.y, new_value);
        }
        if (top.y > 0 && image.at<uchar>(top.y - 1, temp_x) == old_value) {
            push_and_update(image, q, temp_x, top.y - 1, new_value);
        }
        if (top.y < m - 1 && image.at<uchar>(top.y + 1, temp_x) == old_value) {
            push_and_update(image, q, temp_x, top.y + 1, new_value);
        }
    }
    if (top.y > 0 && image.at<uchar>(top.y - 1, top.x) == old_value) {
        push_and_update(image, q, top.x, top.y - 1, new_value);
    }
    if (top.y < m - 1 && image.at<uchar>(top.y + 1, top.x) == old_value) {
        push_and_update(image, q, top.x, top.y + 1, new_value);
    }
}

inline void bfs_step8(cv::Mat &image, std::queue<cv::Point2i> &q,
                      ComponentData &data, const uchar &value) {
    auto m = image.size().height, n = image.size().width;
    cv::Point2i top = q.front();
    q.pop();
    if (top.x > 0) {
        auto temp_x = top.x - 1;
        if (image.at<uchar>(top.y, temp_x) == 1) {
            push_and_update_left(image, q, data, temp_x, top.y, value);
        }
        if (top.y > 0 && image.at<uchar>(top.y - 1, temp_x) == 1) {
            auto temp_y = top.y - 1;
            push_and_update_left(image, q, data, temp_x, temp_y, value);
            if (temp_y < data.top) {
                --data.top;
            }
        }
        if (top.y < m - 1 && image.at<uchar>(top.y + 1, temp_x) == 1) {
            push_and_update_left(image, q, data, temp_x, top.y + 1, value);
            if (top.y + 1 > data.bottom) {
                ++data.bottom;
            }
        }
    }
    if (top.x < n - 1) {
        auto temp_x = top.x + 1;
        if (image.at<uchar>(top.y, temp_x) == 1) {
            push_and_update_right(image, q, data, temp_x, top.y, value);
        }
        if (top.y > 0 && image.at<uchar>(top.y - 1, temp_x) == 1) {
            push_and_update_up(image, q, data, temp_x, top.y - 1, value);
            if (temp_x > data.right) {
                ++data.right;
            }
        }
        if (top.y < m - 1 && image.at<uchar>(top.y + 1, temp_x) == 1) {
            push_and_update_right(image, q, data, temp_x, top.y + 1, value);
            if (top.y + 1 > data.bottom) {
                ++data.bottom;
            }
        }
    }
    if (top.y > 0 && image.at<uchar>(top.y - 1, top.x) == 1) {
        push_and_update_up(image, q, data, top.x, top.y - 1, value);
    }
    if (top.y < image.size().height - 1 && image.at<uchar>(top.y + 1, top.x) == 1) {
        push_and_update_down(image, q, data, top.x, top.y + 1, value);
    }
}

void relabel_last_component_bfs(cv::Mat &image, ComponentData &data,
                                int connectivity = 8) {
    uchar cur_number = data.number;
    data.number = 1;
    cv::Point2i comp_start(data.left, data.top);
    while (image.at<uchar>(comp_start.y, comp_start.x) != cur_number) {
        ++comp_start.x;
    }
    std::queue<cv::Point2i> q;
    push_and_update(image, q, comp_start.x, comp_start.y, data.number);
    if (connectivity == 8) {
        while (!q.empty()) {
            bfs_step8(image, q, cur_number, data.number);
        }
    } else if (connectivity == 4) {
        while (!q.empty()) {
            bfs_step4(image, q, cur_number, data.number);
        }
    } else {
        return;
    }
}

inline void relabel_last_component_naive2d(cv::Mat &image, ComponentData &data) {
    uchar cur_number = data.number;
    data.number = 1;
    for (int i = data.top; i < data.bottom + 1; ++i) {
        for (int j = data.left; j < data.right + 1; ++j) {
            if (image.at<uchar>(i, j) == cur_number) {
                image.at<uchar>(i, j) = 1;
            }
        }
    }
}

void one_component_at_a_time2d8(cv::Mat &image, std::deque<ComponentData> &data) {
    data = {};
    std::queue<cv::Point2i> q;
    cv::Point2i comp_start(0, 0);
    uchar cur_number = 1;
    while (true) {
        find_start(image, comp_start);
        if (comp_start.y != image.size().height) {
            ++cur_number;
            push_and_update(image, q, comp_start.x, comp_start.y, cur_number);
        } else {
            break;
        }
        ComponentData cur_data(comp_start.y, comp_start.y, comp_start.x, comp_start.x, cur_number);
        while (!q.empty()) {
            bfs_step8(image, q, cur_data, cur_number);
        }
        data.emplace_back(cur_data);
    }
    if (data.empty()) { return; }
    relabel_last_component_bfs(image, data.back(), 8);
}

void one_component_at_a_time2d4(cv::Mat &image, std::deque<ComponentData> &data) {
    data = {};
    std::queue<cv::Point2i> q;
    cv::Point2i comp_start(0, 0);
    uchar cur_number = 1;
    while (true) {
        find_start(image, comp_start);
        if (comp_start.y != image.size().height) {
            ++cur_number;
            push_and_update(image, q, comp_start.x, comp_start.y, cur_number);
        } else {
            break;
        }
        ComponentData cur_data(comp_start.y, comp_start.y, comp_start.x, comp_start.x, cur_number);
        while (!q.empty()) {
            bfs_step4(image, q, cur_data, cur_number);
        }
        data.emplace_back(cur_data);
    }
    if (data.empty()) { return; }
    relabel_last_component_bfs(image, data.back(), 4);
}

void one_component_at_a_time2d(cv::Mat &image, std::deque<ComponentData> &data,
                             int connectivity=8) {
    if (connectivity == 8) {
        one_component_at_a_time2d8(image, data);
    } else if (connectivity == 4) {
        one_component_at_a_time2d4(image, data);
    } else {
        return;
    }
}

#endif //SRC_ONE_COMPONENT_AT_A_TIME_H
