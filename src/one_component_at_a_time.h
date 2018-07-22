#include <deque>
#include <queue>
#include <cstdint>
#include "component_data.h"

#ifndef SRC_ONE_COMPONENT_AT_A_TIME_H
#define SRC_ONE_COMPONENT_AT_A_TIME_H


inline void find_unlabeled_component(cv::Mat &image, cv::Point2i &comp_start) {
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
    if (x < cur_data.border.x) {
        cur_data.border.x = x;
        ++cur_data.border.width;
    }
}

inline void push_and_update_right(cv::Mat &image, std::queue<cv::Point2i> &q,
                                 ComponentData &cur_data, const int &x,
                                 const int &y, const uchar &value) {
    push_and_update(image, q, x, y, value);
    if (x > cur_data.border.x + cur_data.border.width - 1) {
        ++cur_data.border.width;
    }
}

inline void push_and_update_up(cv::Mat &image, std::queue<cv::Point2i> &q,
                                 ComponentData &cur_data, const int &x,
                                 const int &y, const uchar &value) {
    push_and_update(image, q, x, y, value);
    if (y < cur_data.border.y) {
        cur_data.border.y = y;
        ++cur_data.border.height;
    }
}

inline void push_and_update_down(cv::Mat &image, std::queue<cv::Point2i> &q,
                               ComponentData &cur_data, const int &x,
                               const int &y, const uchar &value) {
    push_and_update(image, q, x, y, value);
    if (y > cur_data.border.y + cur_data.border.height - 1) {
        ++cur_data.border.height;
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

void relabel_last_component_bfs4(cv::Mat &image, ComponentData &data) {
    uchar cur_number = data.number;
    data.number = 1;
    cv::Point2i comp_start(data.border.x, data.border.y);
    while (image.at<uchar>(comp_start.y, comp_start.x) != cur_number) {
        ++comp_start.x;
    }
    std::queue<cv::Point2i> q;
    push_and_update(image, q, comp_start.x, comp_start.y, data.number);
    while (!q.empty()) {
        bfs_step4(image, q, cur_number, data.number);
    }
}

void relabel_last_component_bfs8(cv::Mat &image, ComponentData &data) {
    uchar cur_number = data.number;
    data.number = 1;
    cv::Point2i comp_start(data.border.x, data.border.y);
    while (image.at<uchar>(comp_start.y, comp_start.x) != cur_number) {
        ++comp_start.x;
    }
    std::queue<cv::Point2i> q;
    push_and_update(image, q, comp_start.x, comp_start.y, data.number);
    while (!q.empty()) {
        bfs_step8(image, q, cur_number, data.number);
    }
}

void relabel_last_component_bfs(cv::Mat &image, ComponentData &data,
                                int connectivity = 8) {
    if (connectivity == 8) {
        relabel_last_component_bfs8(image, data);
    } else if (connectivity == 4) {
        relabel_last_component_bfs4(image, data);
    } else {
        return;
    }
}

inline void relabel_last_component_naive(cv::Mat &image, ComponentData &data) {
    uchar cur_number = data.number;
    data.number = 1;
    for (int i = data.border.y; i < data.border.y + data.border.height; ++i) {
        for (int j = data.border.x; j < data.border.x + data.border.width; ++j) {
            if (image.at<uchar>(i, j) == cur_number) {
                image.at<uchar>(i, j) = 1;
            }
        }
    }
}

void one_component_at_a_time8(cv::Mat &image, std::deque<ComponentData> &data) {
    data = {};
    auto m = image.size().height, n = image.size().width;
    std::queue<cv::Point2i> q;
    cv::Point2i comp_start(0, 0);
    uchar cur_number = 1;
    while (true) {
        find_unlabeled_component(image, comp_start);
        if (comp_start.y != m) {
            q.push(comp_start);
            ++cur_number;
            image.at<uchar>(comp_start.y, comp_start.x) = cur_number;
        } else {
            break;
        }
        ComponentData cur_data(cv::Rect2i(comp_start.x, comp_start.y, 1, 1), cur_number);
        while (!q.empty()) {
            cv::Point2i top = q.front();
            q.pop();
            if (top.x > 0) {
                auto temp_x = top.x - 1;
                if (image.at<uchar>(top.y, temp_x) == 1) {
                    q.push(cv::Point2i(temp_x, top.y));
                    image.at<uchar>(top.y, temp_x) = cur_number;
                    if (temp_x < cur_data.border.x) {
                        cur_data.border.x = temp_x;
                        ++cur_data.border.width;
                    }
                }
                if (top.y > 0 && image.at<uchar>(top.y - 1, temp_x) == 1) {
                    auto temp_y = top.y - 1;
                    q.push(cv::Point2i(temp_x, temp_y));
                    image.at<uchar>(temp_y, temp_x) = cur_number;
                    if (temp_x < cur_data.border.x) {
                        cur_data.border.x = temp_x;
                        ++cur_data.border.width;
                    }
                    if (temp_y < cur_data.border.y) {
                        cur_data.border.y = temp_y;
                        ++cur_data.border.height;
                    }
                }
                if (top.y < m - 1 && image.at<uchar>(top.y + 1, temp_x) == 1) {
                    auto temp_y = top.y + 1;
                    q.push(cv::Point2i(temp_x, temp_y));
                    image.at<uchar>(temp_y, temp_x) = cur_number;
                    if (temp_x < cur_data.border.x) {
                        cur_data.border.x = temp_x;
                        ++cur_data.border.width;
                    }
                    if (temp_y > cur_data.border.y + cur_data.border.height - 1) {
                        ++cur_data.border.height;
                    }
                }
            }
            if (top.x < n - 1) {
                auto temp_x = top.x + 1;
                if (image.at<uchar>(top.y, temp_x) == 1) {
                    q.push(cv::Point2i(temp_x, top.y));
                    image.at<uchar>(top.y, temp_x) = cur_number;
                    if (temp_x > cur_data.border.x + cur_data.border.width - 1) {
                        ++cur_data.border.width;
                    }
                }
                if (top.y > 0 && image.at<uchar>(top.y - 1, temp_x) == 1) {
                    auto temp_y = top.y - 1;
                    q.push(cv::Point2i(temp_x, temp_y));
                    image.at<uchar>(temp_y, temp_x) = cur_number;
                    if (temp_x > cur_data.border.x + cur_data.border.width - 1) {
                        ++cur_data.border.width;
                    }
                    if (temp_y < cur_data.border.y) {
                        cur_data.border.y = temp_y;
                        ++cur_data.border.height;
                    }
                }
                if (top.y < m - 1 && image.at<uchar>(top.y + 1, temp_x) == 1) {
                    auto temp_y = top.y + 1;
                    q.push(cv::Point2i(temp_x, temp_y));
                    image.at<uchar>(temp_y, temp_x) = cur_number;
                    if (temp_x > cur_data.border.x + cur_data.border.width - 1) {
                        ++cur_data.border.width;
                    }
                    if (temp_y > cur_data.border.y + cur_data.border.height - 1) {
                        ++cur_data.border.height;
                    }
                }
            }
            if (top.y > 0 && image.at<uchar>(top.y - 1, top.x) == 1) {
                auto temp = top.y - 1;
                q.push(cv::Point2i(top.x, temp));
                image.at<uchar>(temp, top.x) = cur_number;
                if (temp < cur_data.border.y) {
                    cur_data.border.y = temp;
                    ++cur_data.border.height;
                }
            }
            if (top.y < m - 1 && image.at<uchar>(top.y + 1, top.x) == 1) {
                auto temp = top.y + 1;
                q.push(cv::Point2i(top.x, temp));
                image.at<uchar>(temp, top.x) = cur_number;
                if (temp > cur_data.border.y + cur_data.border.height - 1) {
                    ++cur_data.border.height;
                }
            }
        }
        data.emplace_back(cur_data);
    }
    if (data.empty()) { return; }
    //relabel_last_component_naive(image, data.back());
    relabel_last_component_bfs(image, data.back(), 8);
}

void one_component_at_a_time4(cv::Mat &image, std::deque<ComponentData> &data) {
    data = {};
    std::queue<cv::Point2i> q;
    cv::Point2i comp_start(0, 0);
    uchar cur_number = 1;
    while (true) {
        find_unlabeled_component(image, comp_start);
        if (comp_start.y != image.size().height) {
            ++cur_number;
            push_and_update(image, q, comp_start.x, comp_start.y, cur_number);
        } else {
            break;
        }
        ComponentData cur_data(cv::Rect2i(comp_start.x, comp_start.y, 1, 1), cur_number);
        while (!q.empty()) {
            bfs_step4(image, q, cur_data, cur_number);
        }
        data.emplace_back(cur_data);
    }
    if (data.empty()) { return; }
    relabel_last_component_bfs(image, data.back(), 4);
}

void one_component_at_a_time(cv::Mat &image, std::deque<ComponentData> &data,
                             int connectivity=8) {
    if (connectivity == 8) {
        one_component_at_a_time8(image, data);
    } else if (connectivity == 4) {
        one_component_at_a_time4(image, data);
    } else {
        return;
    }
}

#endif //SRC_ONE_COMPONENT_AT_A_TIME_H
