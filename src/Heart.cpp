#include <fmt/os.h>
#include <fmt/core.h>
#include <cstdlib>
#include <string.h>
#include <algorithm>

#include "Heart.hpp"
#include "utils.hpp"

namespace heart {
    void Heart::Canvas::print_region(size_t x, size_t y, color c, size_t size) {
        if (x >= width || y >= height) {
            return; // Center point is out of bounds, nothing to draw
        }

        // Calculate the starting and ending coordinates for the square
        // We need to ensure we don't go out of bounds
        size_t start_x = (x >= size) ? x - size : 0;
        size_t start_y = (y >= size) ? y - size : 0;
        size_t end_x = std::min(x + size, width - 1);
        size_t end_y = std::min(y + size, height - 1);

        // Iterate through the valid region and set the color
        for (size_t curr_y = start_y; curr_y <= end_y; ++curr_y) {
            for (size_t curr_x = start_x; curr_x <= end_x; ++curr_x) {
                // Calculate the index in the 1D vector using the utility function
                size_t index = map_xy(curr_x, curr_y, width);
                canv[index] = c;
            }
        }
    }

    void Heart::Canvas::toPPM(const std::string& output_file) {
        auto fout = fmt::output_file(output_file);
        fout.print("P3\n{} {}\n{}\n", width, height, COLOR_MAX_VAL);
        for (auto pixel: canv) {
            fout.print("{} ", Heart::c2s(pixel));
        }
    }

    Heart::Heart(const std::string& outout_dir) {
        this->width = ORG_WIDTH;
        this->height = ORG_HEIGHT;
        this->output_dir = outout_dir;
        this->frame_cnt = 0;
    }

    Heart::Heart(double width_factor, double height_factor, const std::string& output_dir) {
        this->width = ORG_WIDTH * width_factor;
        this->height = ORG_HEIGHT * height_factor;
        this->output_dir = output_dir;
        this->frame_cnt = 0;
    }

    void Heart::anime_block(Heart::animation ani, size_t frames) {
        switch (ani) {
            // show up
            case animation::SHOW_UP2DOWN:
                CALL_HEART_AB(SHOW_UP2DOWN, frames);
                break;
            case animation::SHOW_DOWN2UP:
                CALL_HEART_AB(SHOW_DOWN2UP, frames);
                break;

            // disappear
            case animation::DIS_UP2DOWN:
                CALL_HEART_AB(DIS_UP2DOWN, frames);
                break;
            case animation::DIS_DOWN2UP:
                CALL_HEART_AB(DIS_DOWN2UP, frames);
                break;

            // moves
            case animation::SHAKE:
                CALL_HEART_AB(SHAKE, frames);
                break;
            case animation::SLIDE:
                CALL_HEART_AB(SLIDE, frames);
                break;

            // SPECIAL
            case animation::COLOR_CHANGE:
                CALL_HEART_AB(COLOR_CHANGE, frames);
                break;
            case animation::ILOVEU:
                CALL_HEART_AB(ILOVEU, frames);
                break;

            default:
                fmt::print(stderr, "Unexpected input for Heart::anime_block(): {}", ani);
        }
    }

    // animations
    DF_HEART_AB(SHOW_UP2DOWN) {
        /*
        const std::string out_file_path = output_dir + "/frame" + std::to_string(frame_cnt) + ".ppm";
        fmt::output_file(out_file_path);

        frame_cnt++;
        */
    }
}
