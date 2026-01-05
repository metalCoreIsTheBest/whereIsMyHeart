#include <fmt/core.h>
#include <cstdlib>
#include <fstream>
#include <string>

#include "Heart.hpp"

int main(int argc, char *argv[]) {
    using std::string;
    /*
    // Test
    using namespace heart;
    Heart h("test");
    h.anime_block(Heart::animation::SHOW_DOWN2UP, 60);
    h.anime_block(Heart::animation::STILL, 30);
    h.anime_block(Heart::animation::DIS_UP2DOWN, 60);
    h.anime_block(Heart::animation::SHOW_UP2DOWN, 60);
    h.anime_block(Heart::animation::STILL, 30);
    h.anime_block(Heart::animation::DIS_DOWN2UP, 60);
    h.toVideo(30);
    */

    if (argc != 3) {
        fmt::print(stderr, "Usage: whereIsMyHeart [path_to_configuration_file] [path_to_output_dir]\n");
        exit(1);
    }

    std::ifstream fin(argv[1]);
    if (!fin.is_open()) {
        fmt::print(stderr, "Cannot open {}!\n", argv[1]);
        exit(1);
    }

    size_t frame_rate{};
    double wfactor{};
    double hfactor{};

    size_t n{};
    string ani_buf;
    size_t frames_buf{};

    if (!(fin >> frame_rate)) {
        fmt::print(stderr, "Failed to read frame_rate\n");
        exit(1);
    }

    if (!(fin >> wfactor)) {
        fmt::print(stderr, "Failed to read scaling factor of width\n");
        exit(1);
    }

    if (!(fin >> hfactor)) {
        fmt::print(stderr, "Failed to read scaling factor of height\n");
        exit(1);
    }

    auto h = heart::Heart(wfactor, hfactor, string(argv[2]));

    while (fin >> n >> ani_buf >> frames_buf) {
        // I need pattern mathcing in C++
        using ani_t = heart::Heart::animation;
        if (ani_buf == "SHOW_UP2DOWN") {
            h.anime_block(ani_t::SHOW_UP2DOWN, frames_buf);
        } else if (ani_buf == "SHOW_DOWN2UP") {
            h.anime_block(ani_t::SHOW_UP2DOWN, frames_buf);
        } else if (ani_buf == "DIS_UP2DOWN") {
            h.anime_block(ani_t::DIS_UP2DOWN, frames_buf);
        } else if (ani_buf == "DIS_DOWN2UP") {
            h.anime_block(ani_t::DIS_DOWN2UP, frames_buf);
        } else if (ani_buf == "STILL") {
            h.anime_block(ani_t::STILL, frames_buf);
        } else if (ani_buf == "BEAT") {
            h.anime_block(ani_t::BEAT, frames_buf);
        } else if (ani_buf == "SLIDE") {
            h.anime_block(ani_t::SLIDE, frames_buf);
        } else if (ani_buf == "SLIDE") {
            h.anime_block(ani_t::SLIDE, frames_buf);
        } else if (ani_buf == "EMPTY") {
            h.anime_block(ani_t::EMPTY, frames_buf);
        } else if (ani_buf == "COLOR_CHANGE") {
            h.anime_block(ani_t::COLOR_CHANGE, frames_buf);
        } else {
            fmt::print(stderr, "Wrong animation name {}\n", ani_buf);
            exit(1);
        }
    }

    if (!fin.eof()) {
        fmt::print(stderr, "Failed to read the file\n");
        exit(1);
    }

    fin.close();

    h.toVideo(frame_rate);

    fmt::print("Done!\n");

    return 0;
}
