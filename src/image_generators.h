#ifndef SRC_IMAGE_GENERATORS_H
#define SRC_IMAGE_GENERATORS_H

#include <random>
#include <opencv2/opencv.hpp>

constexpr int HEIGHT = 1024, WIDTH = 576, MAX_COMPONENTS = 20;

namespace gen {
    /* Function divides image into grid_param.x * grid_param.y equal(almost) parts
     * and generates a rectangle of some size (depends on max_size value) inside them with probability p.
     */
    cv::Mat rect_components_grid(double p = 0.5, cv::Point2i grid_param = cv::Point2i(5, 4),
                                 int height = HEIGHT, int width = WIDTH);

    /* Same idea but generating rhombuses or triangles and sometimes they may intersect each other.
     * type == 0 - rhombus;
     * type == 1 - triangle;
     * type == 2 - rectangular triangle;
     */
    cv::Mat figure_components_grid(double p = 0.5, int type = 0, cv::Point2i grid_param = cv::Point2i(5, 4),
                                   int height = HEIGHT, int width = WIDTH);

    /* Same idea but generating random figure from {rhombus, triangle, rectangular rectangle, rectangle} each time. */
    cv::Mat all_types_components_grid(double p = 0.5, cv::Point2i grid_param = cv::Point2i(5, 4),
                                      int height = HEIGHT, int width = WIDTH);

    /* Function tries (with probability p) to generate a rectangle of some size
     *(depends on max_size value) at random place MAX_COMPONENTS times.
     */
    cv::Mat rect_components(double p = 0.5, int height = HEIGHT, int width = WIDTH);

    /* Same idea but generating rhombuses or triangles.
     * type == 0 - rhombus;
     * type == 1 - triangle;
     * type == 2 - rectangular triangle;
     */
    cv::Mat figure_components(double p = 0.5, int type = 0, int height = HEIGHT, int width = WIDTH);

    /* Same idea but generating random figure from {rhombus, triangle, rectangular rectangle, rectangle} each time. */
    cv::Mat all_types_components(double p = 0.5, int height = HEIGHT, int width = WIDTH);

    /* Function generates an image using mapping from coordinates to bool.
     * type == 0 => (x + y) % c < d, where (y,x) - image pixel;
     * type == 1 => ((x + y) * x * y / (1 + x * x + y * y )) % C > d, where (y,x) - image pixel;
     */
    cv::Mat func_components(int type = 0, int height = HEIGHT, int width = WIDTH);

    cv::Mat by_fig_type(const std::string &fig_type, float p);
}

#endif //SRC_IMAGE_GENERATORS_H
