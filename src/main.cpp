#include "Heart.hpp"

int main() {
    // Test
    using namespace heart;
    Heart h("test");
    h.anime_block(Heart::animation::SHOW_UP2DOWN, 30);
    h.anime_block(Heart::animation::SHOW_DOWN2UP, 30);
    h.anime_block(Heart::animation::STILL, 120);
    h.toVideo(30);
    return 0;
}
