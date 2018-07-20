#include <vector>
#include <queue>
#include "help_classes.h"

#ifndef SRC_ALGOS_H
#define SRC_ALGOS_H

void one_component_at_a_time(std::vector<std::vector<char>> &image,
                             std::vector<ComponentData> &data, int connectivity = 4) {
    auto m = static_cast<int>(image.size());
    auto n = static_cast<int>(image[0].size());
    data.clear();
    std::queue<Point> q;
    Point last_unmarked(0, 0);
    int cur_comp_number = 1;
    Point end_of_image(m, 0);
    while (true) {
        while (last_unmarked != end_of_image &&
               image[last_unmarked.x()][last_unmarked.y()] != 1) {
            if (last_unmarked.y() != n - 1) {
                last_unmarked.setY(last_unmarked.y() + 1);
            } else {
                last_unmarked.setX(last_unmarked.x() + 1);
                last_unmarked.setY(0);
            }
        }
        if (last_unmarked != end_of_image) {
            q.push(last_unmarked);
        } else {
            break;
        }
        ComponentData cur_data(Rectangle(Point(m-1, n-1), Point(0, 0)), cur_comp_number);
        ++cur_comp_number;
        while (!q.empty()) {
            Point top = q.front();
            q.pop();
            image[top.x()][top.y()] = static_cast<char>(cur_comp_number);
            cur_data.set_border(Rectangle(Point(std::min(cur_data.border().left_top().x(),
                                                         top.x()),
                                                std::min(cur_data.border().left_top().y(),
                                                         top.y())),
                                          Point(std::max(cur_data.border().right_bottom().x(),
                                                         top.x()),
                                                std::max(cur_data.border().right_bottom().y(),
                                                         top.y()))));
            if (top.x() > 0 && image[top.x() - 1][top.y()] == 1) {
                q.push(Point(top.x() - 1, top.y()));
            }
            if (top.y() > 0 && image[top.x()][top.y() - 1] == 1) {
                q.push(Point(top.x(), top.y() - 1));
            }
            if (top.x() < m - 1 && image[top.x() + 1][top.y()] == 1) {
                q.push(Point(top.x() + 1, top.y()));
            }
            if (top.y() < n - 1 && image[top.x()][top.y() + 1] == 1) {
                q.push(Point(top.x(), top.y() + 1));
            }
            if (connectivity == 8) {
                if (top.x() > 0 && top.y() > 0 && image[top.x() - 1][top.y() - 1] == 1) {
                    q.push(Point(top.x() - 1, top.y() - 1));
                }
                if (top.y() > 0 && top.x() < m - 1 && image[top.x() + 1][top.y() - 1] == 1) {
                    q.push(Point(top.x() + 1, top.y() - 1));
                }
                if (top.x() < m - 1 && top.y() < n - 1 && image[top.x() + 1][top.y() + 1] == 1) {
                    q.push(Point(top.x() + 1, top.y() + 1));
                }
                if (top.y() < n - 1 && top.x() > 0 && image[top.x() - 1][top.y() + 1] == 1) {
                    q.push(Point(top.x() - 1, top.y() + 1));
                }
            }
        }
    }
    for (auto &i : image) {
        for (char &j : i) {
            if (j == cur_comp_number) {
                j = 1;
            }
        }
    }
}

#endif //SRC_ALGOS_H
