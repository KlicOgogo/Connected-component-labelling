#include <deque>
#include <queue>
#include <cstdint>
#include "component_data.h"

#ifndef SRC_ONE_COMPONENT_AT_A_TIME_H
#define SRC_ONE_COMPONENT_AT_A_TIME_H


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
    for (int i = data.back().border.y; i < data.back().border.y + data.back().border.height; ++i) {
        for (int j = data.back().border.x; j < data.back().border.x + data.back().border.width; ++j) {
            if (image.at<uchar>(i, j) == cur_number) {
                image.at<uchar>(i, j) = 1;
            }
        }
    }
}

inline void update_last_component_bfs4(cv::Mat &image, std::deque<ComponentData> &data,
                                  const uchar &cur_number, const int &m, const int &n) {
    if (data.empty()) {
        return;
    }
    data.back().number = 1;
    cv::Point2i comp_start(data.back().border.x, data.back().border.y);
    while (image.at<uchar>(comp_start.y, comp_start.x) != cur_number) {
        ++comp_start.x;
    }
    image.at<uchar>(comp_start.y, comp_start.x) = 1;
    std::queue<cv::Point2i> q;
    q.push(comp_start);
    while (!q.empty()) {
        cv::Point2i top = q.front();
        q.pop();
        if (top.x > 0 && image.at<uchar>(top.y, top.x - 1) == cur_number) {
            auto temp = top.x - 1;
            q.push(cv::Point2i(temp, top.y));
            image.at<uchar>(top.y, temp) = 1;
        }
        if (top.x < n - 1 && image.at<uchar>(top.y, top.x + 1) == cur_number) {
            auto temp = top.x + 1;
            q.push(cv::Point2i(temp, top.y));image.at<uchar>(top.y, temp) = 1;

        }
        if (top.y > 0 && image.at<uchar>(top.y - 1, top.x) == cur_number) {
            auto temp = top.y - 1;
            q.push(cv::Point2i(top.x, temp));
            image.at<uchar>(temp, top.x) = 1;
        }
        if (top.y < m - 1 && image.at<uchar>(top.y + 1, top.x) == cur_number) {
            auto temp = top.y + 1;
            q.push(cv::Point2i(top.x, temp));
            image.at<uchar>(temp, top.x) = 1;
        }
    }
}

inline void update_last_component_bfs8(cv::Mat &image, std::deque<ComponentData> &data,
                                       const uchar &cur_number, const int &m, const int &n) {
    if (data.empty()) {
        return;
    }
    data.back().number = 1;
    cv::Point2i comp_start(data.back().border.x, data.back().border.y);
    while (image.at<uchar>(comp_start.y, comp_start.x) != cur_number) {
        ++comp_start.x;
    }
    image.at<uchar>(comp_start.y, comp_start.x) = 1;
    std::queue<cv::Point2i> q;
    q.push(comp_start);
    while (!q.empty()) {
        cv::Point2i top = q.front();
        q.pop();
        if (top.x > 0) {
            auto temp_x = top.x - 1;
            if (image.at<uchar>(top.y, temp_x) == cur_number) {
                q.push(cv::Point2i(temp_x, top.y));
                image.at<uchar>(top.y, temp_x) = 1;
            }
            if (top.y > 0 && image.at<uchar>(top.y - 1, temp_x) == cur_number) {
                auto temp_y = top.y - 1;
                q.push(cv::Point2i(temp_x, temp_y));
                image.at<uchar>(temp_y, temp_x) = 1;
            }
            if (top.y < m - 1 && image.at<uchar>(top.y + 1, temp_x) == cur_number) {
                auto temp_y = top.y + 1;
                q.push(cv::Point2i(temp_x, temp_y));
                image.at<uchar>(temp_y, temp_x) = 1;
            }
        }
        if (top.x < n - 1) {
            auto temp_x = top.x + 1;
            if (image.at<uchar>(top.y, temp_x) == cur_number) {
                q.push(cv::Point2i(temp_x, top.y));
                image.at<uchar>(top.y, temp_x) = 1;
            }
            if (top.y > 0 && image.at<uchar>(top.y - 1, temp_x) == cur_number) {
                auto temp_y = top.y - 1;
                q.push(cv::Point2i(temp_x, temp_y));
                image.at<uchar>(temp_y, temp_x) = 1;
            }
            if (top.y < m - 1 && image.at<uchar>(top.y + 1, temp_x) == cur_number) {
                auto temp_y = top.y + 1;
                q.push(cv::Point2i(temp_x, temp_y));
                image.at<uchar>(temp_y, temp_x) = 1;
            }
        }
        if (top.y > 0 && image.at<uchar>(top.y - 1, top.x) == cur_number) {
            auto temp = top.y - 1;
            q.push(cv::Point2i(top.x, temp));
            image.at<uchar>(temp, top.x) = 1;
        }
        if (top.y < m - 1 && image.at<uchar>(top.y + 1, top.x) == cur_number) {
            auto temp = top.y + 1;
            q.push(cv::Point2i(top.x, temp));
            image.at<uchar>(temp, top.x) = 1;
        }
    }
}

inline void update_last_component_bfs(cv::Mat &image, std::deque<ComponentData> &data,
                                      const uchar &cur_number, const int &m,
                                      const int &n, int connectivity=8) {
    if (connectivity == 8) {
        update_last_component_bfs8(image, data, cur_number, m, n);
    } else if (connectivity == 4) {
        update_last_component_bfs4(image, data, cur_number, m, n);
    } else {
        return;
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
    //update_last_component(image, data, cur_number, m, n);
    update_last_component_bfs(image, data, cur_number, m, n, 8);
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
    //update_last_component(image, data, cur_number, m, n);
    update_last_component_bfs(image, data, cur_number, m, n, 4);
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
