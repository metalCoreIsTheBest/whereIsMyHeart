#pragma once

#include <fmt/core.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>

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
            void toVideo(int frame_rate);

        private:
            static const int COLOR_MAX_VAL = 255;
            static const size_t ORG_WIDTH = 1280;
            static const size_t ORG_HEIGHT = 720;

            enum class color {BLACK, RED, PINK, VIOLET};

            // canvas is to class to represent each frame
            class Canvas {
                private:
                    size_t width, height;
                    std::vector<Heart::color> canv;
                public:
                    Canvas(size_t width, size_t height): width(width), height(height), canv(width * height) {};
                    ~Canvas() = default;

                    // print the pixels arround a point with a specific color
                    void print_region(size_t x, size_t y, color c, size_t size);

                    // load the image to a ppm file
                    void toPPM(const std::string& output_file);
            };

            // ======================================================================================================
            size_t width, height;     // real size of the video
            std::string output_dir;   // output directory for ppm frames
            double k;                 // used by the heart curve
            size_t a, b;              // used by the heart curve

            size_t frame_cnt;         // internal managed counter of frames
            // ======================================================================================================


            static constexpr std::string_view c2s(color c) {
                switch (c) {
                    case color::RED:
                        return "255 0 0";
                    case color::PINK:
                        return "255 192 203";
                    case color::VIOLET:
                        return "238 130 238";
                    case color::BLACK:
                        return "0 0 0";
                    default:
                        return "255 255 255"; // color white for error
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

            // inline drawing utility function

            // F(x, y) = (k(x -a)^2 + k(y - b)^2 - 1)^3 - (k(x - a))^2 * (k(y - b)^3
            // it is a heart if the output is 0
            // domain (approximate):
            // -1.15/|k| + a < x < 1.15/|k| + a
            //  -1/|k| + b < y < 1.25/|k| + b
            double heart_curve(double x, double y) {
                return std::pow(std::pow(k * (x - a), 2) + std::pow(k * (y - b), 2) - 1, 3)
                       - std::pow(k * (x - a), 2) * std::pow(k * (y - b), 3);
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
