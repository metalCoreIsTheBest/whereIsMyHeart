#pragma once

#include <string>
#include <vector>
#include <utility>

#define DF_HEART_AB(NAME) \
    void anime_block_##NAME (size_t frames)

#define CALL_HEART_AB(NAME, FRAMES) \
    anime_block_##NAME(FRAMES)

namespace heart {
    class Heart {
        public:
            enum class color {RED, PINK, VIOLET};

            enum class animation {
                // show up
                SHOW_UP2DOWN,
                SHOW_DOWN2UP,

                // disappear
                DIS_UP2DOWN,
                DIS_DOWN2UP,

                // moves
                SHAKE,
                SLIDE,

                // SPECIAL
                COLOR_CHANGE,
                ILOVEU,
            };

            Heart(const std::string& output_dir);
            Heart(double width_factor, double height_factor, color c, const std::string& output_dir);
            ~Heart() = default;

            // modeling of animation:
            // [anime_block, anime_block, ...]
            // each animation only take effects inside the time interval of its own block,
            // and it should not interfere other [anime_block]

            // unified interface for different animations
            void anime_block(animation ani, size_t frames);

            // vector of anime_blocks
            void anime_series(std::vector<std::pair<animation, size_t>>&& anime_blocks);

            // turn all the frames into a video
            void toVideo(int frame_rate, const std::string& format);

        private:
            static const int COLOR_MAX_VAL = 255;
            static const size_t ORG_WIDTH = 1280;
            static const size_t ORG_HEIGHT = 720;

            size_t width, height;     // real size of the video
            std::string color_string; // RGB
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
                    default:
                        return "255 255 255 "; // color black for error
                }
            }

            // impl of anime_blocks
            DF_HEART_AB(SHOW_UP2DOWN);
            DF_HEART_AB(SHOW_DOWN2UP);
            DF_HEART_AB(DIS_UP2DOWN);
            DF_HEART_AB(DIS_DOWN2UP);
            DF_HEART_AB(SHAKE);
            DF_HEART_AB(SLIDE);
            DF_HEART_AB(COLOR_CHANGE);
            DF_HEART_AB(ILOVEU);
    };
}
