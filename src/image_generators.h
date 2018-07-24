#ifndef SRC_IMAGE_GENERATORS_H
#define SRC_IMAGE_GENERATORS_H

#include <random>

constexpr uchar VALUE = 1;
constexpr int HEIGHT = 1024, WIDTH = 576, MAX_COMPONENTS = 20;
//constexpr int HEIGHT = 576, WIDTH = 1024, MAX_COMPONENTS = 20;

void draw_figure(cv::Mat &image, int type, const int &fig_size,
                 const cv::Point2i &start, uchar value) {
    std::deque<cv::Point2i> q_old, q_new;
    q_old.emplace_back(start);
    image.at<uchar>(start.y, start.x) = 1;
    for (int k = 0; k < fig_size; ++k) {
        while (!q_old.empty()) {
            auto top = q_old.front();
            q_old.pop_front();
            if (type != 2 && top.x > 0 && image.at<uchar>(top.y, top.x - 1) != value) {
                image.at<uchar>(top.y, top.x - 1) = value;
                q_new.emplace_back(cv::Point2i(top.x - 1, top.y));
            }
            if (type == 0 && top.y > 0 && image.at<uchar>(top.y - 1, top.x) != value) {
                image.at<uchar>(top.y - 1, top.x) = value;
                q_new.emplace_back(cv::Point2i(top.x, top.y - 1));
            }
            if (top.x < image.size().width - 1 && image.at<uchar>(top.y, top.x + 1) != value) {
                image.at<uchar>(top.y, top.x + 1) = value;
                q_new.emplace_back(cv::Point2i(top.x + 1, top.y));
            }
            if (top.y < image.size().height - 1 && image.at<uchar>(top.y + 1, top.x) != value) {
                image.at<uchar>(top.y + 1, top.x) = value;
                q_new.emplace_back(cv::Point2i(top.x, top.y + 1));
            }
        }
        q_old = q_new;
        q_new = {};
    }
}

void draw_rectangle(cv::Mat &image, const int &width, const int &height,
                    const cv::Point2i &start, uchar value) {
    for (int i = start.y; i < std::min(start.y + height, image.size().height); ++i) {
        for (int j = start.x; j < std::min(start.x + width, image.size().width); ++j) {
            image.at<uchar>(i, j) = value;
        }
    }
}


namespace gen {
    /* Function divides image into grid_param.x * grid_param.y equal(almost) parts
     * and generates a rectangle of some size (depends on max_size value) inside them with probability p.
     */
    cv::Mat rect_components_grid(double p = 0.5, cv::Point2i grid_param = cv::Point2i(5, 4),
                                 int height = HEIGHT, int width = WIDTH) {
        cv::Mat image(cv::Mat::zeros(height, width, CV_8UC1));
        std::random_device device;
        std::default_random_engine gen(device());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        const int HEIGHT_SM = height / grid_param.x, WIDTH_SM = width / grid_param.y;
        auto width_lb = static_cast<int>(static_cast<double>(WIDTH_SM) / std::sqrt(2));
        auto height_lb = static_cast<int>(static_cast<double>(HEIGHT_SM) / std::sqrt(2));
        int top = 0;
        for (int i = 0; i < grid_param.x; ++i) {
            int left = 0;
            for (int j = 0; j < grid_param.y; ++j) {
                if (distr(gen) < p) {
                    int comp_width{}, comp_height{};
                    bool max_size = distr(gen) < 0.5;
                    if (!max_size) {
                        comp_width = width_lb - 40 + static_cast<int>(41 * distr(gen));
                        comp_height = height_lb - 40 + static_cast<int>(41 * distr(gen));
                    } else {
                        comp_width = width_lb;
                        comp_height = height_lb;
                    }
                    draw_rectangle(image, comp_width, comp_height, cv::Point2i(left, top), VALUE);
                }
                left += WIDTH_SM;
            }
            top += HEIGHT_SM;
        }
        return image;
    }

    /* Same idea but generating rhombuses or triangles and sometimes they may intersect each other.
     * type == 0 - rhombus;
     * type == 1 - triangle;
     * type == 2 - rectangular triangle;
     */
    cv::Mat figure_components_grid(double p = 0.5, int type = 0, cv::Point2i grid_param = cv::Point2i(5, 4),
                                   int height = HEIGHT, int width = WIDTH) {
        cv::Mat image(cv::Mat::zeros(height, width, CV_8UC1));
        std::random_device device;
        std::default_random_engine gen(device());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        const int HEIGHT_SM = height / grid_param.x, WIDTH_SM = width / grid_param.y;
        int top = 0;
        for (int i = 0; i < grid_param.x; ++i) {
            int left = 0;
            for (int j = 0; j < grid_param.y; ++j) {
                if (distr(gen) < p) {
                    auto fig_size = static_cast<int>((std::min(HEIGHT_SM, WIDTH_SM) / 3 + 50 * distr(gen)));
                    cv::Point2i start(left + WIDTH_SM / 2, top + HEIGHT_SM / 2);
                    draw_figure(image, type, fig_size, start, VALUE);
                }
                left += WIDTH_SM;
            }
            top += HEIGHT_SM;
        }
        return image;
    }

    /* Same idea but generating random figure from {rhombus, triangle, rectangular rectangle, rectangle} each time. */
    cv::Mat all_types_components_grid(double p = 0.5, cv::Point2i grid_param = cv::Point2i(5, 4),
                                      int height = HEIGHT, int width = WIDTH) {
        cv::Mat image(cv::Mat::zeros(height, width, CV_8UC1));
        std::random_device device;
        std::default_random_engine gen(device());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        const int HEIGHT_SM = height / grid_param.x, WIDTH_SM = width / grid_param.y;
        auto width_lb = static_cast<int>(static_cast<double>(WIDTH_SM) / std::sqrt(2));
        auto height_lb = static_cast<int>(static_cast<double>(HEIGHT_SM) / std::sqrt(2));
        int top = 0;
        for (int i = 0; i < grid_param.x; ++i) {
            int left = 0;
            for (int j = 0; j < grid_param.y; ++j) {
                if (distr(gen) < p) {
                    auto cur_type = static_cast<int>(4 * distr(gen));
                    if (cur_type < 3) {
                        auto fig_size = static_cast<int>((std::min(HEIGHT_SM, WIDTH_SM) / 3 + 50 * distr(gen)));
                        cv::Point2i start(left + WIDTH_SM / 2, top + HEIGHT_SM / 2);
                        draw_figure(image, cur_type, fig_size, start, VALUE);
                    } else {
                        int comp_width{}, comp_height{};
                        bool max_size = distr(gen) < 0.5;
                        if (!max_size) {
                            comp_width = width_lb - 40 + static_cast<int>(41 * distr(gen));
                            comp_height = height_lb - 40 + static_cast<int>(41 * distr(gen));
                        } else {
                            comp_width = width_lb;
                            comp_height = height_lb;
                        }
                        draw_rectangle(image, comp_width, comp_height, cv::Point2i(left, top), VALUE);
                    }
                }
                left += WIDTH_SM;
            }
            top += HEIGHT_SM;
        }
        return image;
    }

    /* Function tries (with probability p) to generate a rectangle of some size
     *(depends on max_size value) at random place MAX_COMPONENTS times.
     */
    cv::Mat rect_components(double p = 0.5, int height = HEIGHT, int width = WIDTH) {
        cv::Mat image(cv::Mat::zeros(height, width, CV_8UC1));
        std::random_device device;
        std::default_random_engine gen(device());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        const int HEIGHT_SM = height / 5, WIDTH_SM = width / 4;
        auto width_lb = static_cast<int>(static_cast<double>(WIDTH_SM) / std::sqrt(2));
        auto height_lb = static_cast<int>(static_cast<double>(HEIGHT_SM) / std::sqrt(2));
        for (int i = 0; i < MAX_COMPONENTS; ++i) {
            if (distr(gen) < p) {
                cv::Point2i start(static_cast<int>(width * distr(gen)),
                                  static_cast<int>(height * distr(gen)));
                int comp_width{}, comp_height{};
                bool max_size = distr(gen) < 0.5;
                if (!max_size) {
                    comp_width = width_lb - 40 + static_cast<int>(41 * distr(gen));
                    comp_height = height_lb - 40 + static_cast<int>(41 * distr(gen));
                } else {
                    comp_width = width_lb;
                    comp_height = height_lb;
                }
                draw_rectangle(image, comp_width, comp_height, start, VALUE);
            }
        }
        return image;
    }

    /* Same idea but generating rhombuses or triangles.
     * type == 0 - rhombus;
     * type == 1 - triangle;
     * type == 2 - rectangular triangle;
     */
    cv::Mat figure_components(double p = 0.5, int type = 0, int height = HEIGHT, int width = WIDTH) {
        cv::Mat image(cv::Mat::zeros(height, width, CV_8UC1));
        std::random_device device;
        std::default_random_engine gen(device());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        const int HEIGHT_SM = height / 5, WIDTH_SM = width / 4;
        for (int i = 0; i < MAX_COMPONENTS; ++i) {
            if (distr(gen) < p) {
                auto fig_size = static_cast<int>((std::min(HEIGHT_SM, WIDTH_SM) / 3 + 50 * distr(gen)));
                cv::Point2i start(static_cast<int>(width * distr(gen)),
                                  static_cast<int>(height * distr(gen)));
                draw_figure(image, type, fig_size, start, VALUE);
            }
        }
        return image;
    }

    /* Same idea but generating random figure from {rhombus, triangle, rectangular rectangle, rectangle} each time. */
    cv::Mat all_types_components(double p = 0.5, int height = HEIGHT, int width = WIDTH) {
        cv::Mat image(cv::Mat::zeros(height, width, CV_8UC1));
        std::random_device device;
        std::default_random_engine gen(device());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        const int HEIGHT_SM = height / 5, WIDTH_SM = width / 4;
        for (int i = 0; i < MAX_COMPONENTS; ++i) {
            if (distr(gen) < p) {
                auto fig_size = static_cast<int>((std::min(HEIGHT_SM, WIDTH_SM) / 3 + 50 * distr(gen)));
                cv::Point2i start(static_cast<int>(width * distr(gen)),
                                  static_cast<int>(height * distr(gen)));
                auto cur_type = static_cast<int>(4 * distr(gen));
                if (cur_type < 3) {
                    draw_figure(image, cur_type, fig_size, start, VALUE);
                } else {
                    draw_rectangle(image, fig_size * 2, fig_size * 2, start, VALUE);
                }
            }
        }
        return image;
    }

    /* Function generates an image using mapping from coordinates to bool.
     * type == 0 => (x + y) % c < d, where (y,x) - image pixel;
     * type == 1 => ((x + y) * x * y / (1 + x * x + y * y )) % C > d, where (y,x) - image pixel;

     */
    cv::Mat func_components(int type = 0, int height = HEIGHT, int width = WIDTH) {
        cv::Mat image(cv::Mat::zeros(height, width, CV_8UC1));
        const int C = 100;
        std::random_device device;
        std::default_random_engine gen(device());
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        auto lambda0 = [](int x, int y, int d) { return (x + y) % C < d; };
        auto lambda1 = [](int x, int y, int d) { return ((x + y) * x * y / (1 + x * x + y * y)) % C > d; };
        std::vector<bool (*)(int, int, int)> lambdas = {lambda0, lambda1};
        int d = 49 - static_cast<int>(20 * distr(gen));
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (lambdas[type](i, j, d)) {
                    image.at<uchar>(i, j) = VALUE;
                }
            }
        }
        return image;
    }

    cv::Mat by_fig_type(const std::string &fig_type, float p) {
        if (fig_type == "Grid rectangles") {
            return gen::rect_components_grid(p);
        } else if (fig_type == "Grid big rectangles") {
            return gen::rect_components_grid(p, cv::Point2i(2, 1));
        } else if (fig_type == "Random rectangles") {
            return gen::rect_components(p);
        } else if (fig_type == "Grid big rhombuses") {
            return gen::figure_components_grid(p, 0, cv::Point2i(2, 1));
        } else if (fig_type == "Grid rhombuses") {
            return gen::figure_components_grid(p, 0);
        } else if (fig_type == "Random rhombuses") {
            return gen::figure_components(p, 0);
        } else if (fig_type == "Grid big triangles") {
            return gen::figure_components_grid(p, 1, cv::Point2i(2, 1));
        } else if (fig_type == "Grid triangles") {
            return gen::figure_components_grid(p, 1);
        } else if (fig_type == "Random triangles") {
            return gen::figure_components(p, 1);
        } else if (fig_type == "Grid big rect. triangles") {
            return gen::figure_components_grid(p, 2, cv::Point2i(2, 1));
        } else if (fig_type == "Grid rect. triangles") {
            return gen::figure_components_grid(p, 2);
        } else if (fig_type == "Random rect. triangles") {
            return gen::figure_components(p, 2);
        } else if (fig_type == "Grid big something") {
            return gen::all_types_components_grid(p, cv::Point2i(2, 1));
        } else if (fig_type == "Grid something") {
            return gen::all_types_components_grid(p);
        } else if (fig_type == "Random something") {
            return gen::all_types_components(p);
        }
    }
}

#endif //SRC_IMAGE_GENERATORS_H
