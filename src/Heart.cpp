#include <fmt/os.h>
#include <fmt/core.h>
#include <cstdlib>
#include <string.h>

#include "Heart.hpp"

namespace heart {
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
