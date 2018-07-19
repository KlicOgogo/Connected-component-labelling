#ifndef SRC_IMG_GEN_H
#define SRC_IMG_GEN_H

#include <vector>
#include <random>

constexpr size_t height = 1024, width = 576;
constexpr size_t height_sm = 205, width_sm = 144;


std::vector<std::vector<char>> generate_image(double p=0.5) {
    std::vector<std::vector<char>> result(height, std::vector<char>(width, 0));
    std::random_device device;
    std::default_random_engine gen(device());
    std::uniform_real_distribution<double> distr(0.0, 1.0);
    size_t left = 0;
    for (int i = 0; i < 5; ++i) {
        size_t up = 0;
        for (int j = 0; j < 4; ++j) {
            if (distr(gen) > p) {
                up += width_sm;
                continue;
            }
            size_t comp_width = 61 + static_cast<size_t>(41 * distr(gen));
            size_t comp_height = 104 + static_cast<size_t>(41 * distr(gen));
            for (size_t k = left; k < left + comp_height; ++k) {
                for (size_t l = up; l < up + comp_width; ++l) {
                    result[k][l] = 1;
                }
            }
            up += width_sm;
        }
        left += height_sm;
    }
    return result;
}

#endif //SRC_IMG_GEN_H
