#include <vector>
#include <queue>
#include <cstdint>
#include "help_classes.h"

#ifndef SRC_ALGOS_H
#define SRC_ALGOS_H

void one_component_at_a_time(std::vector<std::vector<char>> &image,
                             std::vector<ComponentData> &data,
                             int connectivity = 4) {
    data.clear();
    data.reserve(10000);
    auto m = static_cast<int>(image.size());
    auto n = static_cast<int>(image[0].size());
    std::queue<Point> q;
    Point comp_start(0, 0);
    Point end_of_image(m, 0);
    char cur_number = 1;
    while (true) {
        while (comp_start != end_of_image &&
               image[comp_start.x()][comp_start.y()] != 1) {
            if (comp_start.y() != n - 1) {
                comp_start.setY(comp_start.y() + 1);
            } else {
                comp_start.setX(comp_start.x() + 1);
                comp_start.setY(0);
            }
        }
        if (comp_start != end_of_image) {
            q.push(comp_start);
            ++cur_number;
            image[comp_start.x()][comp_start.y()] = cur_number;
        } else {
            break;
        }
        ComponentData cur_data(Rectangle(Point(m - 1, n - 1), Point(0, 0)), cur_number);
        while (!q.empty()) {
            Point top = q.front();
            q.pop();
            Point temp_lt(std::min(cur_data.border().left_top().x(), top.x()),
                          std::min(cur_data.border().left_top().y(), top.y()));
            Point temp_rb(std::max(cur_data.border().right_bottom().x(), top.x()),
                          std::max(cur_data.border().right_bottom().y(), top.y()));
            cur_data.set_border(Rectangle(temp_lt, temp_rb));
            if (top.x() > 0 && image[top.x() - 1][top.y()] == 1) {
                q.push(Point(top.x() - 1, top.y()));
                image[top.x() - 1][top.y()] = cur_number;
            }
            if (top.y() > 0 && image[top.x()][top.y() - 1] == 1) {
                q.push(Point(top.x(), top.y() - 1));
                image[top.x()][top.y() - 1] = cur_number;
            }
            if (top.x() < m - 1 && image[top.x() + 1][top.y()] == 1) {
                q.push(Point(top.x() + 1, top.y()));
                image[top.x() + 1][top.y()] = cur_number;
            }
            if (top.y() < n - 1 && image[top.x()][top.y() + 1] == 1) {
                q.push(Point(top.x(), top.y() + 1));
                image[top.x()][top.y() + 1] = cur_number;
            }
            if (connectivity == 8) {
                if (top.x() > 0 && top.y() > 0 && image[top.x() - 1][top.y() - 1] == 1) {
                    q.push(Point(top.x() - 1, top.y() - 1));
                    image[top.x() - 1][top.y() - 1] = cur_number;
                }
                if (top.y() > 0 && top.x() < m - 1 && image[top.x() + 1][top.y() - 1] == 1) {
                    q.push(Point(top.x() + 1, top.y() - 1));
                    image[top.x() + 1][top.y() - 1] = cur_number;
                }
                if (top.x() < m - 1 && top.y() < n - 1 && image[top.x() + 1][top.y() + 1] == 1) {
                    q.push(Point(top.x() + 1, top.y() + 1));
                    image[top.x() + 1][top.y() + 1] = cur_number;
                }
                if (top.y() < n - 1 && top.x() > 0 && image[top.x() - 1][top.y() + 1] == 1) {
                    q.push(Point(top.x() - 1, top.y() + 1));
                    image[top.x() - 1][top.y() + 1] = cur_number;
                }
            }
        }
        data.emplace_back(cur_data);
    }
    data.back().set_number(1);
    for (auto &i : image) {
        for (char &j : i) {
            if (j == cur_number) {
                j = 1;
            }
        }
    }
}

#endif //SRC_ALGOS_H
