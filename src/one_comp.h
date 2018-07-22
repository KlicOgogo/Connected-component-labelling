#ifndef SRC_ONE_COMP_H
#define SRC_ONE_COMP_H

#include "c_data.h"

inline void find_unlabeled_component(cv::Mat &image, int &comp_start, const int &out) {
    while (comp_start != out && image.at<uchar>(comp_start) != 1) {
        ++comp_start;
    }
}

inline void push_and_update(cv::Mat &image, std::queue<int> &q,
                            const int &p, const uchar &value) {
    q.push(p);
    image.at<uchar>(p) = value;
}

inline void push_and_update_left(cv::Mat &image, std::queue<int> &q,
                                 CompData &data, const int &p, const uchar &value) {
    push_and_update(image, q, p, value);
    int l = p % image.size().width;
    if (l < data.left) {
        data.left = l;
    }
}

inline void push_and_update_right(cv::Mat &image, std::queue<int> &q,
                                  CompData &data, const int &p, const uchar &value) {
    push_and_update(image, q, p, value);
    int l = p % image.size().width;
    if (l > data.right) {
        data.right = l;
    }

}

inline void push_and_update_up(cv::Mat &image, std::queue<int> &q,
                               CompData &data, const int &p, const uchar &value) {
    push_and_update(image, q, p, value);
    if (p < data.top) {
        data.top = p;
    }
}

inline void push_and_update_down(cv::Mat &image, std::queue<int> &q,
                                 CompData &data, const int &p, const uchar &value) {
    push_and_update(image, q, p, value);
    if (p > data.bottom) {
        data.bottom = p;
    }
}

inline void bfs_step4(cv::Mat &image, std::queue<int> &q,
                      CompData &data, const uchar &value, const int &out) {
    int top = q.front();
    q.pop();
    if (top - image.size().width > -1) {
        if (image.at<uchar>(top - image.size().width) == 1) {
            push_and_update_up(image, q, data, top - image.size().width, value);
        }
        if (image.at<uchar>(top - 1) == 1) {
            push_and_update_left(image, q, data, top - 1, value);
        }
    } else {
        if (top - 1 > -1 && image.at<uchar>(top - 1) == 1) {
            push_and_update_left(image, q, data, top - 1, value);
        }
    }
    if (top + image.size().width < out) {
        if (image.at<uchar>(top + image.size().width) == 1) {
            push_and_update_down(image, q, data, top + image.size().width, value);
        }
        if (image.at<uchar>(top + 1) == 1) {
            push_and_update_right(image, q, data, top + 1, value);
        }
    } else {
        if (top + 1 < out && image.at<uchar>(top + 1) == 1) {
            push_and_update_right(image, q, data, top + 1, value);
        }
    }
}

inline void bfs_step4(cv::Mat &image, std::queue<int> &q,
                      const uchar &old_value, const uchar &new_value, const int &out) {
    int n = image.size().width;
    int top = q.front();
    q.pop();
    if (top - n > -1) {
        if (image.at<uchar>(top - n) == old_value) {
            push_and_update(image, q, top - n, new_value);
        }
        if (image.at<uchar>(top - 1) == old_value) {
            push_and_update(image, q, top - 1, new_value);
        }
    } else {
        if (top > 0 && image.at<uchar>(top - 1) == old_value) {
            push_and_update(image, q, top - 1, new_value);
        }
    }
    if (top + n < out) {
        if (image.at<uchar>(top + n) == old_value) {
            push_and_update(image, q, top + n, new_value);
        }
        if (image.at<uchar>(top + 1) == old_value) {
            push_and_update(image, q, top + 1, new_value);
        }
    } else {
        if (top + 1 < out && image.at<uchar>(top + 1) == old_value) {
            push_and_update(image, q, top + 1, new_value);
        }
    }
}

inline void bfs_step8(cv::Mat &image, std::queue<int> &q,
                      const uchar &old_value, const uchar &new_value, const int &out) {
    auto n = image.size().width;
    int top = q.front();
    q.pop();
    if (top > n) {
        if (image.at<uchar>(top - n - 1) == old_value) {
            push_and_update(image, q, top - n - 1, new_value);
        }
        if (image.at<uchar>(top - n) == old_value) {
            push_and_update(image, q, top - n, new_value);
        }
        if (image.at<uchar>(top - n + 1) == old_value) {
            push_and_update(image, q, top - n + 1, new_value);
        }
        if (image.at<uchar>(top - 1) == old_value) {
            push_and_update(image, q, top - 1, new_value);
        }
    } else if (top > n - 1) {
        if (image.at<uchar>(top - n) == old_value) {
            push_and_update(image, q, top - n, new_value);
        }
        if (image.at<uchar>(top - n + 1) == old_value) {
            push_and_update(image, q, top - n + 1, new_value);
        }
        if (image.at<uchar>(top - 1) == old_value) {
            push_and_update(image, q, top - 1, new_value);
        }
    } else if (top > n - 2) {
        if (image.at<uchar>(top - n + 1) == old_value) {
            push_and_update(image, q, top - n + 1, new_value);
        }
        if (image.at<uchar>(top - 1) == old_value) {
            push_and_update(image, q, top - 1, new_value);
        }
    } else if (top > 0 && image.at<uchar>(top - 1) == old_value) {
        push_and_update(image, q, top - 1, new_value);
    }

    if (top + n + 1 < out) {
        if (image.at<uchar>(top + n + 1) == old_value) {
            push_and_update(image, q, top + n + 1, new_value);
        }
        if (image.at<uchar>(top + n) == old_value) {
            push_and_update(image, q, top + n, new_value);
        }
        if (image.at<uchar>(top + n - 1) == old_value) {
            push_and_update(image, q, top + n - 1, new_value);
        }
        if (image.at<uchar>(top + 1) == old_value) {
            push_and_update(image, q, top + 1, new_value);
        }
    } else if (top + n < out) {
        if (image.at<uchar>(top + n) == old_value) {
            push_and_update(image, q, top + n, new_value);
        }
        if (image.at<uchar>(top + n - 1) == old_value) {
            push_and_update(image, q, top + n - 1, new_value);
        }
        if (image.at<uchar>(top + 1) == old_value) {
            push_and_update(image, q, top + 1, new_value);
        }
    } else if (top + n - 1 < out) {
        if (image.at<uchar>(top + n - 1) == old_value) {
            push_and_update(image, q, top + n - 1, new_value);
        }
        if (image.at<uchar>(top + 1) == old_value) {
            push_and_update(image, q, top + 1, new_value);
        }
    } else if (top + 1 < out && image.at<uchar>(top + 1) == old_value) {
        push_and_update(image, q, top + 1, new_value);
    }
}

inline void bfs_step8(cv::Mat &image, std::queue<int> &q,
                      CompData &data, const uchar &value, const int &out) {
    auto n = image.size().width;
    int top = q.front();
    q.pop();
    if (top > n) {
        if (image.at<uchar>(top - n - 1) == 1) {
            push_and_update_up(image, q, data, top - n - 1, value);
            int l = (top - 1) % n;
            if (l < data.left) {
                data.left = l;
            }
        }
        if (image.at<uchar>(top - n) == 1) {
            push_and_update_up(image, q, data, top - n, value);
        }
        if (image.at<uchar>(top - n + 1) == 1) {
            push_and_update_up(image, q, data, top - n + 1, value);
            int l = (top + 1) % n;
            if (l > data.right) {
                data.right = l;
            }
        }
        if (image.at<uchar>(top - 1) == 1) {
            push_and_update_left(image, q, data, top - 1, value);
        }
    } else if (top > n - 1) {
        if (image.at<uchar>(top - n) == 1) {
            push_and_update_up(image, q, data, top - n, value);
        }
        if (image.at<uchar>(top - n + 1) == 1) {
            push_and_update_up(image, q, data, top - n + 1, value);
            int l = (top + 1) % n;
            if (l > data.right) {
                data.right = l;
            }
        }
        if (image.at<uchar>(top - 1) == 1) {
            push_and_update_left(image, q, data, top - 1, value);
        }
    } else if (top > n - 2) {
        if (image.at<uchar>(top - n + 1) == 1) {
            push_and_update_up(image, q, data, top - n + 1, value);
            int l = (top + 1) % n;
            if (l > data.right) {
                data.right = l;
            }
        }
        if (image.at<uchar>(top - 1) == 1) {
            push_and_update_left(image, q, data, top - 1, value);
        }
    } else if (top > 0 && image.at<uchar>(top - 1) == 1) {
        push_and_update_left(image, q, data, top - 1, value);
    }

    if (top + n + 1 < out) {
        if (image.at<uchar>(top + n + 1) == 1) {
            push_and_update_down(image, q, data, top + n + 1, value);
            int l = (top + 1) % n;
            if (l > data.right) {
                data.right = l;
            }
        }
        if (image.at<uchar>(top + n) == 1) {
            push_and_update_down(image, q, data, top + n, value);
        }
        if (image.at<uchar>(top + n - 1) == 1) {
            push_and_update_down(image, q, data, top + n - 1, value);
            int l = (top - 1) % n;
            if (l < data.left) {
                data.left = l;
            }

        }
        if (image.at<uchar>(top + 1) == 1) {
            push_and_update_right(image, q, data, top + 1, value);
        }
    } else if (top + n < out) {
        if (image.at<uchar>(top + n) == 1) {
            push_and_update_down(image, q, data, top + n, value);
        }
        if (image.at<uchar>(top + n - 1) == 1) {
            push_and_update_down(image, q, data, top + n - 1, value);
            int l = (top - 1) % n;
            if (l < data.left) {
                data.left = l;
            }

        }
        if (image.at<uchar>(top + 1) == 1) {
            push_and_update_right(image, q, data, top + 1, value);
        }
    } else if (top + n - 1 < out) {
        if (image.at<uchar>(top + n - 1) == 1) {
            push_and_update_down(image, q, data, top + n - 1, value);
            int l = (top - 1) % n;
            if (l < data.left) {
                data.left = l;
            }

        }
        if (image.at<uchar>(top + 1) == 1) {
            push_and_update_right(image, q, data, top + 1, value);
        }
    } else if (top + 1 < out && image.at<uchar>(top + 1) == 1) {
        push_and_update_right(image, q, data, top + 1, value);
    }
}

void relabel_last_component_bfs4(cv::Mat &image, CompData &data, const int &out) {
    uchar cur_number = data.number;
    data.number = 1;
    int comp_start(data.top - (data.top % image.size().width) + data.left);
    while (image.at<uchar>(comp_start) != cur_number) {
        ++comp_start;
    }
    std::queue<int> q;
    push_and_update(image, q, comp_start, data.number);
    while (!q.empty()) {
        bfs_step4(image, q, cur_number, data.number, out);
    }
}

void relabel_last_component_bfs8(cv::Mat &image, CompData &data, const int &out) {
    uchar cur_number = data.number;
    data.number = 1;
    int comp_start(data.top - (data.top % image.size().width) + data.left);
    while (image.at<uchar>(comp_start) != cur_number) {
        ++comp_start;
    }
    std::queue<int> q;
    push_and_update(image, q, comp_start, data.number);
    while (!q.empty()) {
        bfs_step8(image, q, cur_number, data.number, out);
    }
}

void relabel_last_component_bfs(cv::Mat &image, CompData &data,
                                const int &out, int connectivity = 8) {
    if (connectivity == 8) {
        relabel_last_component_bfs8(image, data, out);
    } else if (connectivity == 4) {
        relabel_last_component_bfs4(image, data, out);
    } else {
        return;
    }
}

inline void relabel_last_component_naive(cv::Mat &image, CompData &data) {
    uchar cur_number = data.number;
    data.number = 1;
    for (int i = data.top - (data.top % image.size().width) + data.left;
        i < data.bottom - (data.bottom % image.size().width) + data.right + 1; ++i) {
        if (image.at<uchar>(i) == cur_number) {
            image.at<uchar>(i) = 1;
        }
    }
}

void one_component_at_a_time8(cv::Mat &image, std::deque<CompData> &data) {
    int out = image.size().width * image.size().height;
    data = {};
    std::queue<int> q;
    int comp_start(0);
    uchar cur_number = 1;
    while (true) {
        find_unlabeled_component(image, comp_start, out);
        if (comp_start != out) {
            ++cur_number;
            push_and_update(image, q, comp_start, cur_number);
        } else {
            break;
        }
        CompData cur_data(comp_start, comp_start,
                               comp_start % image.size().width,
                               comp_start % image.size().width, cur_number);
        while (!q.empty()) {
            bfs_step8(image, q, cur_data, cur_number, out);
        }
        data.emplace_back(cur_data);
    }
    if (data.empty()) { return; }
    relabel_last_component_bfs(image, data.back(), out, 8);
}

void one_component_at_a_time4(cv::Mat &image, std::deque<CompData> &data) {
    int out = image.size().width * image.size().height;
    data = {};
    std::queue<int> q;
    int comp_start(0);
    uchar cur_number = 1;
    while (true) {
        find_unlabeled_component(image, comp_start, out);
        if (comp_start != out) {
            ++cur_number;
            push_and_update(image, q, comp_start, cur_number);
        } else {
            break;
        }
        CompData cur_data(comp_start, comp_start,
                          comp_start % image.size().width,
                          comp_start % image.size().width, cur_number);
        while (!q.empty()) {
            bfs_step4(image, q, cur_data, cur_number, out);
        }
        data.emplace_back(cur_data);
    }
    if (data.empty()) { return; }
    relabel_last_component_bfs(image, data.back(), out, 4);
}

void one_component_at_a_time(cv::Mat &image, std::deque<CompData> &data,
                             int connectivity = 8) {
    if (connectivity == 8) {
        one_component_at_a_time8(image, data);
    } else if (connectivity == 4) {
        one_component_at_a_time4(image, data);
    } else {
        return;
    }
}
#endif //SRC_ONE_COMP_H
