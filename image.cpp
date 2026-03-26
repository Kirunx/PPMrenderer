#include "image.hpp"
#include <algorithm>
void Image::set_pixel(int x, int y, int z, Pixel color) {
    // Развертка координат в 1D массив
    int idx = (y * width) + x;
    // Проверка на наличие пикселя ближе к камере
    if (z <= depth_buffer[idx]) {
        depth_buffer[idx] = z;
        color_buffer[idx] = color;
    }
}
void Image::save_to_ppm(const std::string& filename) {
    FILE* f = fopen(filename.c_str(), "wb");

    fprintf(f, "P6\n%d %d\n255\n", width, height);

    fwrite(color_buffer.data(), sizeof(Pixel), color_buffer.size(), f);

    fclose(f);
}

void Image::clear(Pixel clear_color) {
    std::fill(color_buffer.begin(), color_buffer.end(), clear_color);
    std::fill(depth_buffer.begin(), depth_buffer.end(), std::numeric_limits<float>::max());
}