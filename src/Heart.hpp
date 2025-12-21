#pragma once

#include <fmt/core.h>
#include <cmath>
#include <string>
#include <string_view>

#define DC_HEART_AB(NAME) \
    void anime_block_##NAME (size_t frames)

#define DF_HEART_AB(NAME) \
    void Heart::anime_block_##NAME (size_t frames)

#define CALL_HEART_AB(NAME, FRAMES) \
    anime_block_##NAME(FRAMES)

namespace heart {
    class Heart {
        public:
            enum class animation {
                // show up
                SHOW_UP2DOWN,
                SHOW_DOWN2UP,

                // disappear
                DIS_UP2DOWN,
                DIS_DOWN2UP,

                // moves
                STILL,
                SHAKE,
                SLIDE,

                // SPECIAL
                COLOR_CHANGE,
                ILOVEU,
            };

            Heart(const std::string& output_dir);
            Heart(double width_factor, double height_factor, const std::string& output_dir);
            ~Heart() = default;

            // modeling of animation:
            // [anime_block, anime_block, ...]
            // each animation only take effects inside the time interval of its own block,
            // and it should not interfere other [anime_block]

            // unified interface for different animations
            void anime_block(animation ani, size_t frames);

            // turn all the frames into a video
            void toVideo(int frame_rate, const std::string& format);

        private:
            static const int COLOR_MAX_VAL = 255;
            static const size_t ORG_WIDTH = 1280;
            static const size_t ORG_HEIGHT = 720;

            enum class color {RED, PINK, VIOLET, BLACK};

            size_t width, height;     // real size of the video
            std::string output_dir;   // output directory for ppm frames

            size_t frame_cnt;         // internal managed counter of frames

            std::string c2s(color c) const {
                switch (c) {
                    case color::RED:
                        return "255 0 0 ";
                    case color::PINK:
                        return "255 192 203 ";
                    case color::VIOLET:
                        return "238 130 238 ";
                    case color::BLACK:
                        return "255 255 255 ";
                    default:
                        return "255 255 255 "; // color black for error
                }
            }

            // impl of anime_blocks
            DC_HEART_AB(SHOW_UP2DOWN);
            DC_HEART_AB(SHOW_DOWN2UP);
            DC_HEART_AB(DIS_UP2DOWN);
            DC_HEART_AB(DIS_DOWN2UP);
            DC_HEART_AB(STILL);
            DC_HEART_AB(SHAKE);
            DC_HEART_AB(SLIDE);
            DC_HEART_AB(COLOR_CHANGE);
            DC_HEART_AB(ILOVEU);

            // drawing utils

            // F(x, y) = ((kx)^2 + (ky)^2 - 1)^3 - (kx)^2 * (ky)^3
            // it is a heart if the output is 0
            // domain (approximate):
            // -1.15/k < x < 1.15/k
            //   -1/k  < y < 1.25/k
            double heart_curve(double x, double y, double k) {
                return std::pow(std::pow(k * x, 2) + std::pow(k * y, 2) + 1, 3) - std::pow(k * x, 2) * std::pow(k * y, 3);
            }
    };

}

// for fmt
template<>
struct fmt::formatter<heart::Heart::animation>: public fmt::formatter<std::string_view> {
    using ani_t = heart::Heart::animation;
    auto format(ani_t ani, fmt::format_context& ctx) const {
        std::string_view name;
        switch (ani) {
            case ani_t::SHOW_UP2DOWN:
                name = "SHOW_UP2DOWN"; break;
            case ani_t::SHOW_DOWN2UP:
                name = "SHOW_DOWN2UP"; break;
            case ani_t::DIS_UP2DOWN:
                name = "DIS_UP2DOWN"; break;
            case ani_t::DIS_DOWN2UP:
                name = "DIS_DOWN2UP"; break;
            case ani_t::STILL:
                name = "STILL"; break;
            case ani_t::SHAKE:
                name = "SHAKE"; break;
            case ani_t::SLIDE:
                name = "SLIDE"; break;
            case ani_t::COLOR_CHANGE:
                name = "COLOR_CHANGE"; break;
            case ani_t::ILOVEU:
                name = "ILOVEU"; break;
        }
        return fmt::formatter<std::string_view>::format(name, ctx);
    }
};
