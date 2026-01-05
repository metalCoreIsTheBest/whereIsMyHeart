#include <fmt/os.h>
#include <fmt/core.h>

#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <algorithm>
#include <string>

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
        /*
        auto fout = fmt::output_file(output_file);
        fout.print("P3\n{} {}\n{}\n", width, height, COLOR_MAX_VAL);
        for (auto pixel: canv) {
            fout.print("{} ", Heart::c2s(pixel));
        }
        */
        std::ofstream fout;
        fout.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        fout.open(output_file, std::ios::out | std::ios::binary);
        fout << "P6\n" << width << " " << height << "\n" << COLOR_MAX_VAL << "\n";

        for (auto pixel: canv) {
            fout.write(reinterpret_cast<const char *>(Heart::c2v(pixel).data()), Heart::c2v(pixel).size());
        }

        fout.close();
    }

    Heart::Heart(const std::string& outout_dir) {
        this->width = ORG_WIDTH;
        this->height = ORG_HEIGHT;
        this->a = this->width / 2;
        this->b = this->height / 2;
        this->k = -1 * 2.25 / (0.75 * this->height); // a lot of magic numbers
        this->output_dir = outout_dir;
        this->frame_cnt = 0;

        fmt::print("Heart: a: {} | b: {} | k: {:.2e}\n", a, b, k);
    }

    Heart::Heart(double width_factor, double height_factor, const std::string& output_dir) {
        this->width = ORG_WIDTH * width_factor;
        this->height = ORG_HEIGHT * height_factor;
        this->a = this->width / 2;
        this->b = this->height / 2;
        this->k = -1 * 2.25 / (0.75 * this->height);
        this->output_dir = output_dir;
        this->frame_cnt = 0;

        fmt::print("Heart: a: {} | b: {} | k: {:.2e}\n", a, b, k);
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
            case animation::STILL:
                CALL_HEART_AB(STILL, frames);
                break;
            case animation::BEAT:
                CALL_HEART_AB(BEAT, frames);
                break;
            case animation::SLIDE:
                CALL_HEART_AB(SLIDE, frames);
                break;

            // SPECIAL
            case animation::EMPTY:
                CALL_HEART_AB(EMPTY, frames);
                break;
            case animation::COLOR_CHANGE:
                CALL_HEART_AB(COLOR_CHANGE, frames);
                break;

            default:
                fmt::print(stderr, "Unexpected input for Heart::anime_block(): {}", ani);
        }
    }

    // animations
    DF_HEART_AB(SHOW_UP2DOWN) {
        Canvas canvas(width, height);

        fmt::print("Generating SHOW_UP2DOWN:");
        std::fflush(stdout);
        for (size_t f = 1; f <= frames; f++) {
            ITER_WHOLE_PLANE(width, static_cast<double>(height) / static_cast<double>(frames) * f) {
                if (heart_curve(curr_x + 0.5, curr_y) * heart_curve(curr_x + 0.5, curr_y + 1) < 0.0
                    || heart_curve(curr_x, curr_y + 0.5) * heart_curve(curr_x + 1, curr_y + 0.5) < 0.0)
                // it is not an accurate check, but good enough in this case
                {
                    canvas.print_region(curr_x, curr_y, color::RED, 3);
                }
            }
            canvas.toPPM(output_dir + "/frame" + std::to_string(frame_cnt) + ".ppm");
            if (f % 10 == 0 || f == frames) {
                fmt::print("\rGenerating SHOW_UP2DOWN: {:.2f}%", static_cast<double>(f) / static_cast<double>(frames) * 100.0);
                std::fflush(stdout);
            }
            frame_cnt++;
        }
        fmt::print("\n");
    }

    DF_HEART_AB(SHOW_DOWN2UP) {
        Canvas canvas(width, height);

        fmt::print("Generating SHOW_DOWN2UP:");
        std::fflush(stdout);
        for (size_t f = 1; f <= frames; f++) {
            for (size_t curr_y = -1.0 * height / frames * f + height; curr_y < height; curr_y++) {
                for (size_t curr_x = 0; curr_x < width; curr_x++) {
                    if (heart_curve(curr_x + 0.5, curr_y) * heart_curve(curr_x + 0.5, curr_y + 1) < 0.0
                        || heart_curve(curr_x, curr_y + 0.5) * heart_curve(curr_x + 1, curr_y + 0.5) < 0.0)
                    // it is not an accurate check, but good enough in this case
                    {
                        canvas.print_region(curr_x, curr_y, color::RED, 3);
                    }
                }
            }
            canvas.toPPM(output_dir + "/frame" + std::to_string(frame_cnt) + ".ppm");
            if (f % 10 == 0 || f == frames) {
                fmt::print("\rGenerating SHOW_DOWN2UP: {:.2f}%", static_cast<double>(f) / static_cast<double>(frames) * 100.0);
                std::fflush(stdout);
            }
            frame_cnt++;
        }
        fmt::print("\n");
    }

    DF_HEART_AB(DIS_UP2DOWN) {
        Canvas canvas(width, height);

        fmt::print("Generating DIS_UP2DOWN:");
        std::fflush(stdout);
        for (size_t f = 1; f <= frames; f++) {
            if (f == 1) {
                ITER_WHOLE_PLANE(width, height) {
                    if (heart_curve(curr_x + 0.5, curr_y) * heart_curve(curr_x + 0.5, curr_y + 1) < 0.0
                        || heart_curve(curr_x, curr_y + 0.5) * heart_curve(curr_x + 1, curr_y + 0.5) < 0.0)
                    {
                            canvas.print_region(curr_x, curr_y, color::RED, 3);
                    }
                }
            } else {
                ITER_WHOLE_PLANE(width, height / frames * f) {
                    if (canvas.get(curr_x, curr_y) == color::RED) {
                        canvas.print_region(curr_x, curr_y, color::BLACK, 3);
                    }
                }
            }
            canvas.toPPM(output_dir + "/frame" + std::to_string(frame_cnt) + ".ppm");
            if (f % 10 == 0 || f == frames) {
                fmt::print("\rGenerating DIS_UP2DOWN: {:.2f}%", static_cast<double>(f) / static_cast<double>(frames) * 100.0);
                std::fflush(stdout);
            }
            frame_cnt++;
        }
        fmt::print("\n");
    }

    DF_HEART_AB(DIS_DOWN2UP) {
        Canvas canvas(width, height);

        fmt::print("Generating DIS_DOWN2UP:");
        std::fflush(stdout);
        for (size_t f = 1; f <= frames; f++) {
            if (f == 1) {
                ITER_WHOLE_PLANE(width, height) {
                    if (heart_curve(curr_x + 0.5, curr_y) * heart_curve(curr_x + 0.5, curr_y + 1) < 0.0
                        || heart_curve(curr_x, curr_y + 0.5) * heart_curve(curr_x + 1, curr_y + 0.5) < 0.0)
                    {
                        canvas.print_region(curr_x, curr_y, color::RED, 3);
                    }
                }
            } else {
                for (size_t curr_y = -1.0 * height / frames * f + height; curr_y < height; curr_y++) {
                    for (size_t curr_x = 0; curr_x < width; curr_x++) {
                        if (canvas.get(curr_x, curr_y) == color::RED) {
                            canvas.print_region(curr_x, curr_y, color::BLACK, 3);
                        }
                    }
                }
            }
            canvas.toPPM(output_dir + "/frame" + std::to_string(frame_cnt) + ".ppm");
            if (f % 10 == 0 || f == frames) {
                fmt::print("\rGenerating DIS_DOWN2UP: {:.2f}%", static_cast<double>(f) / static_cast<double>(frames) * 100.0);
                std::fflush(stdout);
            }
            frame_cnt++;
        }
            fmt::print("\n");
    }

    DF_HEART_AB(STILL) {
        Canvas canvas(width, height);

        fmt::print("Generating STILL:");
        std::fflush(stdout);
        for (size_t f = 1; f <= frames; f++) {
            if (f == 1) {
                ITER_WHOLE_PLANE(width, height) {
                    if (heart_curve(curr_x + 0.5, curr_y) * heart_curve(curr_x + 0.5, curr_y + 1) < 0.0
                        || heart_curve(curr_x, curr_y + 0.5) * heart_curve(curr_x + 1, curr_y + 0.5) < 0.0)
                    // it is not an accurate check, but good enough in this case
                    {
                        canvas.print_region(curr_x, curr_y, color::RED, 3);
                    }
                }
            }
            canvas.toPPM(output_dir + "/frame" + std::to_string(frame_cnt) + ".ppm");
            if (f % 10 == 0 || f == frames) {
                fmt::print("\rGenerating STILL: {:.2f}%", static_cast<double>(f) / static_cast<double>(frames) * 100.0);
                std::fflush(stdout);
            }
            frame_cnt++;
        }
        fmt::print("\n");
    }

    DF_HEART_AB(BEAT) {
        fmt::print("Generating BEAT:");
        std::fflush(stdout);
        auto org_k = k;
        for (size_t f = 1; f <= frames; f++) {
            Canvas canvas(width, height);
            if (f <= frames / 2) {
                k = org_k - 0.4 * org_k / frames * f; // adapt k
                ITER_WHOLE_PLANE(width, height) {
                    if (heart_curve(curr_x + 0.5, curr_y) * heart_curve(curr_x + 0.5, curr_y + 1) < 0.0
                        || heart_curve(curr_x, curr_y + 0.5) * heart_curve(curr_x + 1, curr_y + 0.5) < 0.0)
                    {
                        canvas.print_region(curr_x, curr_y, color::RED, 3);
                    }
                }
            } else {
                k = 0.6 * org_k + 0.4 * org_k / frames * f;
                ITER_WHOLE_PLANE(width, height) {
                    if (heart_curve(curr_x + 0.5, curr_y) * heart_curve(curr_x + 0.5, curr_y + 1) < 0.0
                        || heart_curve(curr_x, curr_y + 0.5) * heart_curve(curr_x + 1, curr_y + 0.5) < 0.0)
                    {
                        canvas.print_region(curr_x, curr_y, color::RED, 3);
                    }
                }
            }

            canvas.toPPM(fmt::format("{0}/frame{1}.ppm", output_dir, std::to_string(frame_cnt)));
            if (f % 10 == 0 || f == frames) {
                fmt::print("\rGenerating BEAT: {:.2f}%", static_cast<double>(f) / static_cast<double>(frames) * 100.0);
                std::fflush(stdout);
            }
            frame_cnt++;
        }
        k = org_k;
        fmt::print("\n");
    }

    DF_HEART_AB(SLIDE) {
        fmt::print("Generating SLIDE:");
        std::fflush(stdout);
        auto org_a = a;
        for (size_t f = 1; f <= frames; f++) {
            Canvas canvas(width, height);
            if (f <= frames / 2) {
                a = org_a - 4 * org_a / frames * f;
                ITER_WHOLE_PLANE(width, height) {
                    if (heart_curve(curr_x + 0.5, curr_y) * heart_curve(curr_x + 0.5, curr_y + 1) < 0.0
                        || heart_curve(curr_x, curr_y + 0.5) * heart_curve(curr_x + 1, curr_y + 0.5) < 0.0)
                    {
                        canvas.print_region(curr_x, curr_y, color::RED, 3);
                    }
                }
            } else {
                a = 5 * org_a - 4 * org_a / frames * f;
                ITER_WHOLE_PLANE(width, height) {
                    if (heart_curve(curr_x + 0.5, curr_y) * heart_curve(curr_x + 0.5, curr_y + 1) < 0.0
                        || heart_curve(curr_x, curr_y + 0.5) * heart_curve(curr_x + 1, curr_y + 0.5) < 0.0)
                    {
                        canvas.print_region(curr_x, curr_y, color::RED, 3);
                    }
                }
            }

            canvas.toPPM(fmt::format("{0}/frame{1}.ppm", output_dir, std::to_string(frame_cnt)));
            if (f % 10 == 0 || f == frames) {
                fmt::print("\rGenerating SLIDE: {:.2f}%", static_cast<double>(f) / static_cast<double>(frames) * 100.0);
                std::fflush(stdout);
            }
            frame_cnt++;
        }
        a = org_a;
        fmt::print("\n");
    }

    DF_HEART_AB(EMPTY) {
        Canvas canvas(width, height);

        fmt::print("Generating EMPTY:");
        std::fflush(stdout);
        for (size_t f = 1; f <= frames; f++) {
            canvas.toPPM(output_dir + "/frame" + std::to_string(frame_cnt) + ".ppm");
            if (f % 10 == 0 || f == frames) {
                fmt::print("\rGenerating EMPTY: {:.2f}%", static_cast<double>(f) / static_cast<double>(frames) * 100.0);
                std::fflush(stdout);
            }
            frame_cnt++;
        }
        fmt::print("\n");
    }

    DF_HEART_AB(COLOR_CHANGE) {
        Canvas canvas(width, height);

        fmt::print("Generating COLOR_CHANGE:");
        std::fflush(stdout);
        for (size_t f = 1; f <= frames; f++) {
            if (f == 1) {
                ITER_WHOLE_PLANE(width, height) {
                    if (heart_curve(curr_x + 0.5, curr_y) * heart_curve(curr_x + 0.5, curr_y + 1) < 0.0
                        || heart_curve(curr_x, curr_y + 0.5) * heart_curve(curr_x + 1, curr_y + 0.5) < 0.0)
                    // it is not an accurate check, but good enough in this case
                    {
                        canvas.print_region(curr_x, curr_y, color::PINK, 3);
                    }
                }
            } else if (f == frames / 2) {
                ITER_WHOLE_PLANE(width, height) {
                    if (heart_curve(curr_x + 0.5, curr_y) * heart_curve(curr_x + 0.5, curr_y + 1) < 0.0
                        || heart_curve(curr_x, curr_y + 0.5) * heart_curve(curr_x + 1, curr_y + 0.5) < 0.0)
                    // it is not an accurate check, but good enough in this case
                    {
                        canvas.print_region(curr_x, curr_y, color::VIOLET, 3);
                    }
                }
            }

            canvas.toPPM(output_dir + "/frame" + std::to_string(frame_cnt) + ".ppm");
            if (f % 10 == 0 || f == frames) {
                fmt::print("\rGenerating COLOR_CHANGE: {:.2f}%", static_cast<double>(f) / static_cast<double>(frames) * 100.0);
                std::fflush(stdout);
            }
            frame_cnt++;
        }
        fmt::print("\n");
    }

    void Heart::toVideo(int frame_rate) {
        std::string inputPattern = output_dir + "/frame%d.ppm";
        std::string outputFile = output_dir + "/output.mp4";
        std::string codec = "libx264";
        std::string pixelFormat = "yuv420p";

        std::string cmd = fmt::format("ffmpeg -framerate {0} -i {1} -c:v {2} -v 0 -pix_fmt {3} {4}",
            std::to_string(frame_rate), inputPattern, codec, pixelFormat, outputFile);

        fmt::print("Execut ffmpeg command: {}\n", cmd);

        int result = std::system(cmd.c_str());

        if (result != 0) {
           fmt::print(stderr, "Failed to generate the video!");
        }
    };
}
