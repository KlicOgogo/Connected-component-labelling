#include <deque>
#include <queue>
#include <cstdint>
#include "component_data.h"

#ifndef SRC_ONE_COMPONENT_AT_A_TIME_H
#define SRC_ONE_COMPONENT_AT_A_TIME_H


inline void find_unlabeled_component(cv::Mat &image, cv::Point2i &comp_start,
                                     const int &m, const int &n) {
    while (comp_start.y != m && image.at<uchar>(comp_start.y, comp_start.x) != 1) {
        if (comp_start.x != n - 1) {
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

void relabel_last_component_bfs4(cv::Mat &image, std::deque<ComponentData> &data) {
    if (data.empty()) {
        return;
    }
    uchar cur_number = data.back().number;
    data.back().number = 1;
    cv::Point2i comp_start(data.back().border.x, data.back().border.y);
    while (image.at<uchar>(comp_start.y, comp_start.x) != cur_number) {
        ++comp_start.x;
    }
    std::queue<cv::Point2i> q;
    push_and_update(image, q, comp_start.x, comp_start.y, data.back().number);
    while (!q.empty()) {
        cv::Point2i top = q.front();
        q.pop();
        if (top.x > 0 && image.at<uchar>(top.y, top.x - 1) == cur_number) {
            push_and_update(image, q, top.x - 1, top.y, data.back().number);
        }
        if (top.x < image.size().width - 1 &&
            image.at<uchar>(top.y, top.x + 1) == cur_number) {
            push_and_update(image, q, top.x + 1, top.y, data.back().number);
        }
        if (top.y > 0 && image.at<uchar>(top.y - 1, top.x) == cur_number) {
            push_and_update(image, q, top.x, top.y - 1, data.back().number);
        }
        if (top.y < image.size().height - 1 &&
            image.at<uchar>(top.y + 1, top.x) == cur_number) {
            push_and_update(image, q, top.x, top.y + 1, data.back().number);
        }
    }
}

void relabel_last_component_bfs8(cv::Mat &image, std::deque<ComponentData> &data) {
    if (data.empty()) {
        return;
    }
    auto m = image.size().height;
    auto n = image.size().width;
    uchar cur_number = data.back().number;
    data.back().number = 1;
    cv::Point2i comp_start(data.back().border.x, data.back().border.y);
    while (image.at<uchar>(comp_start.y, comp_start.x) != cur_number) {
        ++comp_start.x;
    }
    std::queue<cv::Point2i> q;
    push_and_update(image, q, comp_start.x, comp_start.y, data.back().number);
    while (!q.empty()) {
        cv::Point2i top = q.front();
        q.pop();
        if (top.x > 0) {
            auto temp_x = top.x - 1;
            if (image.at<uchar>(top.y, temp_x) == cur_number) {
                push_and_update(image, q, temp_x, top.y, data.back().number);
            }
            if (top.y > 0 && image.at<uchar>(top.y - 1, temp_x) == cur_number) {
                push_and_update(image, q, temp_x, top.y - 1, data.back().number);
            }
            if (top.y < m - 1 && image.at<uchar>(top.y + 1, temp_x) == cur_number) {
                push_and_update(image, q, temp_x, top.y + 1, data.back().number);
            }
        }
        if (top.x < n - 1) {
            auto temp_x = top.x + 1;
            if (image.at<uchar>(top.y, temp_x) == cur_number) {
                push_and_update(image, q, temp_x, top.y, data.back().number);
            }
            if (top.y > 0 && image.at<uchar>(top.y - 1, temp_x) == cur_number) {
                push_and_update(image, q, temp_x, top.y - 1, data.back().number);
            }
            if (top.y < m - 1 && image.at<uchar>(top.y + 1, temp_x) == cur_number) {
                push_and_update(image, q, temp_x, top.y + 1, data.back().number);
            }
        }
        if (top.y > 0 && image.at<uchar>(top.y - 1, top.x) == cur_number) {
            push_and_update(image, q, top.x, top.y - 1, data.back().number);
        }
        if (top.y < m - 1 && image.at<uchar>(top.y + 1, top.x) == cur_number) {
            push_and_update(image, q, top.x, top.y + 1, data.back().number);
        }
    }
}

void relabel_last_component_bfs(cv::Mat &image, std::deque<ComponentData> &data,
                                int connectivity = 8) {
    if (connectivity == 8) {
        relabel_last_component_bfs8(image, data);
    } else if (connectivity == 4) {
        relabel_last_component_bfs4(image, data);
    } else {
        return;
    }
}

inline void relabel_last_component_naive(cv::Mat &image, std::deque<ComponentData> &data) {
    if (data.empty()) {
        return;
    }
    uchar cur_number = data.back().number;
    data.back().number = 1;
    for (int i = data.back().border.y; i < data.back().border.y + data.back().border.height; ++i) {
        for (int j = data.back().border.x; j < data.back().border.x + data.back().border.width; ++j) {
            if (image.at<uchar>(i, j) == cur_number) {
                image.at<uchar>(i, j) = 1;
            }
        }
    }
}

void one_component_at_a_time8(cv::Mat &image, std::deque<ComponentData> &data) {
    data = {};
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
    //relabel_last_component_naive(image, data);
    relabel_last_component_bfs(image, data, 8);
}

void one_component_at_a_time4(cv::Mat &image,
                              std::deque<ComponentData> &data) {
    data = {};
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
        ComponentData cur_data(cv::Rect2i(comp_start.x, comp_start.y, 1, 1), cur_number);
        while (!q.empty()) {
            cv::Point2i top = q.front();
            q.pop();
            if (top.x > 0 && image.at<uchar>(top.y, top.x - 1) == 1) {
                auto temp = top.x - 1;
                q.push(cv::Point2i(temp, top.y));
                image.at<uchar>(top.y, temp) = cur_number;
                if (temp < cur_data.border.x) {
                    cur_data.border.x = temp;
                    ++cur_data.border.width;
                }
            }
            if (top.x < n - 1 && image.at<uchar>(top.y, top.x + 1) == 1) {
                auto temp = top.x + 1;
                q.push(cv::Point2i(temp, top.y));
                image.at<uchar>(top.y, temp) = cur_number;
                if (temp > cur_data.border.x + cur_data.border.width - 1) {
                    ++cur_data.border.width;
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
    //relabel_last_component_naive(image, data);
    relabel_last_component_bfs(image, data, 4);
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

#endif //SRC_ONE_COMPONENT_AT_A_TIME_H
