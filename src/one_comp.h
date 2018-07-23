#ifndef SRC_ONE_COMP_H
#define SRC_ONE_COMP_H

#include <deque>
#include <queue>
#include <cstdint>
#include <opencv2/opencv.hpp>
#include "component_data.h"

inline void find_start(cv::Mat &image, int &comp_start, const int &out) {
    while (comp_start != out && image.at<uchar>(comp_start) != 1) {
        ++comp_start;
    }
}

inline void push_and_update(cv::Mat &image, std::queue<cv::Point2i> &q,
                            const cv::Point &p, const uchar &value) {
    q.push(p);
    image.at<uchar>(p.x) = value;
}

inline void push_and_update_left(cv::Mat &image, std::queue<cv::Point2i> &q,
                                 ComponentData &data, const cv::Point &p, const uchar &value) {
    push_and_update(image, q, p, value);
    if (p.y < data.left) {
        data.left = p.y;
    }
}

inline void push_and_update_right(cv::Mat &image, std::queue<cv::Point2i> &q,
                                  ComponentData &data, const cv::Point2i &p, const uchar &value) {
    push_and_update(image, q, p, value);
    if (p.y > data.right) {
        data.right = p.y;
    }

}

inline void push_and_update_up(cv::Mat &image, std::queue<cv::Point2i> &q,
                               ComponentData &data, const cv::Point2i &p, const uchar &value) {
    push_and_update(image, q, p, value);
    if (p.x < data.top) {
        data.top = p.x;
    }
}

inline void push_and_update_down(cv::Mat &image, std::queue<cv::Point2i> &q,
                                 ComponentData &data, const cv::Point2i &p, const uchar &value) {
    push_and_update(image, q, p, value);
    if (p.x > data.bottom) {
        data.bottom = p.x;
    }
}

void bfs_step4(cv::Mat &image, std::queue<cv::Point2i> &q,
                      ComponentData &data, const uchar &value, const int &out) {
    auto top = q.front();
    q.pop();
    if (top.x > image.size().width - 1) {
        if (image.at<uchar>(top.x - image.size().width) == 1) {
            push_and_update_up(image, q, data, cv::Point2i(top.x - image.size().width, top.y), value);
        }
        if (top.y > 0 && image.at<uchar>(top.x - 1) == 1) {
            push_and_update_left(image, q, data, cv::Point2i(top.x - 1, top.y - 1), value);
        }
    } else {
        if (top.y > 0 && image.at<uchar>(top.x - 1) == 1) {
            push_and_update_left(image, q, data, cv::Point2i(top.x - 1, top.y - 1), value);
        }
    }
    if (top.x + image.size().width < out) {
        if (image.at<uchar>(top.x + image.size().width) == 1) {
            push_and_update_down(image, q, data, cv::Point2i(top.x + image.size().width, top.y), value);
        }
        if (top.y < image.size().width - 1 && image.at<uchar>(top.x + 1) == 1) {
            push_and_update_right(image, q, data, cv::Point2i(top.x + 1, top.y + 1), value);
        }
    } else {
        if (top.y < image.size().width - 1 && image.at<uchar>(top.x + 1) == 1) {
            push_and_update_right(image, q, data, cv::Point2i(top.x + 1, top.y + 1), value);
        }
    }
}

void bfs_step8(cv::Mat &image, std::queue<cv::Point2i> &q,
                      ComponentData &data, const uchar &value, const int &out) {
    auto n = image.size().width;
    auto top = q.front();
    q.pop();
    if (top.x > n) {
        if (top.y > 0) {
            if (image.at<uchar>(top.x - n - 1) == 1) {
                push_and_update_up(image, q, data, cv::Point2i(top.x - n - 1, top.y - 1), value);
                if (top.y - 1 < data.left) {
                    data.left = top.y - 1;
                }
            }
            if (image.at<uchar>(top.x - 1) == 1) {
                push_and_update_left(image, q, data, cv::Point(top.x - 1, top.y - 1), value);
            }
        }
        if (image.at<uchar>(top.x - n) == 1) {
            push_and_update_up(image, q, data, cv::Point2i(top.x - n, top.y), value);
        }
        if (top.y < n - 1 && image.at<uchar>(top.x - n + 1) == 1) {
            push_and_update_up(image, q, data, cv::Point(top.x - n + 1, top.y + 1), value);
            if (top.y + 1 > data.right) {
                data.right = top.y + 1;
            }
        }
    } else if (top.x > n - 1) {
        if (image.at<uchar>(top.x - n) == 1) {
            push_and_update_up(image, q, data, cv::Point2i(top.x - n, top.y), value);
        }
        if (top.y < n - 1 && image.at<uchar>(top.x - n + 1) == 1) {
            push_and_update_up(image, q, data, cv::Point(top.x - n + 1, top.y + 1), value);
            if (top.y + 1 > data.right) {
                data.right = top.y + 1;
            }
        }
        if (top.y > 0 && image.at<uchar>(top.x - 1) == 1) {
            push_and_update_left(image, q, data, cv::Point(top.x - 1, top.y - 1), value);
        }
    } else if (top.x > n - 2) {
        if (top.y < n - 1 && image.at<uchar>(top.x - n + 1) == 1) {
            push_and_update_up(image, q, data, cv::Point(top.x - n + 1, top.y + 1), value);
            if (top.y + 1 > data.right) {
                data.right = top.y + 1;
            }
        }
        if (top.y > 0 && image.at<uchar>(top.x - 1) == 1) {
            push_and_update_left(image, q, data, cv::Point(top.x - 1, top.y - 1), value);
        }
    } else if (top.y > 0 && image.at<uchar>(top.x - 1) == 1) {
        push_and_update_left(image, q, data, cv::Point(top.x - 1, top.y - 1), value);
    }

    if (top.x + n + 1 < out) {
        if (top.y < n - 1) {
            if (image.at<uchar>(top.x + n + 1) == 1) {
                push_and_update_down(image, q, data, cv::Point2i(top.x + n + 1, top.y + 1), value);
                if (top.y + 1 > data.right) {
                    data.right = top.y + 1;
                }
            }
            if (image.at<uchar>(top.x + 1) == 1) {
                push_and_update_right(image, q, data, cv::Point2i(top.x + 1, top.y + 1), value);
            }
        }
        if (image.at<uchar>(top.x + n) == 1) {
            push_and_update_down(image, q, data, cv::Point(top.x + n, top.y), value);
        }
        if (top.y > 0 && image.at<uchar>(top.x + n - 1) == 1) {
            push_and_update_down(image, q, data, cv::Point(top.x + n - 1, top.y - 1), value);
            if (top.y - 1 < data.left) {
                data.left = top.y - 1;
            }
        }
    } else if (top.x + n < out) {
        if (image.at<uchar>(top.x + n) == 1) {
            push_and_update_down(image, q, data, cv::Point(top.x + n, top.y), value);
        }
        if (top.y > 0 && image.at<uchar>(top.x + n - 1) == 1) {
            push_and_update_down(image, q, data, cv::Point(top.x + n - 1, top.y - 1), value);
            if (top.y - 1 < data.left) {
                data.left = top.y - 1;
            }
        }
        if (top.y < n - 1  && image.at<uchar>(top.x + 1) == 1) {
            push_and_update_right(image, q, data, cv::Point2i(top.x + 1, top.y + 1), value);
        }
    } else if (top.x + n - 1 < out) {
        if (top.y > 0 && image.at<uchar>(top.x + n - 1) == 1) {
            push_and_update_down(image, q, data, cv::Point(top.x + n - 1, top.y - 1), value);
            if (top.y - 1 < data.left) {
                data.left = top.y - 1;
            }
        }
        if (top.y < n - 1  && image.at<uchar>(top.x + 1) == 1) {
            push_and_update_right(image, q, data, cv::Point2i(top.x + 1, top.y + 1), value);
        }
    } else if (top.y < n - 1  && image.at<uchar>(top.x + 1) == 1) {
        push_and_update_right(image, q, data, cv::Point2i(top.x + 1, top.y + 1), value);
    }
}

void one_component_at_a_time1d(cv::Mat &image, std::deque<ComponentData> &data,
                             int connectivity = 8) {
    int out = image.size().width * image.size().height, n = image.size().width;
    data = {};
    std::queue<cv::Point2i> q;
    int comp_start(0);
    uchar cur_number = 1;
    while (true) {
        find_start(image, comp_start, out);
        if (comp_start != out) {
            ++cur_number;
            push_and_update(image, q, cv::Point2i(comp_start, comp_start % n), cur_number);
        } else {
            break;
        }
        ComponentData cur_data(comp_start, comp_start, comp_start % n,
                               comp_start % n, cur_number);
        if (connectivity == 4) {
            while (!q.empty()) {
                bfs_step4(image, q, cur_data, cur_number, out);
            }
        } else if (connectivity == 8) {
            while (!q.empty()) {
                bfs_step8(image, q, cur_data, cur_number, out);
            }
        }
        data.emplace_back(cur_data);
    }
    if (data.empty()) { return; }
    for (auto &i : data) {
        i.bottom /= n;
        i.top /= n;
    }
}

#endif //SRC_ONE_COMP_H
