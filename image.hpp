#include <cstdint>
#include <vector>
#include <limits>
#include <string>
struct Pixel {
    uint8_t r, g, b;
};

class Image {
private:
	// Буфер цвета каждого пикселя
    std::vector<Pixel> color_buffer;
	// Буфер дальности пикселя от камеры 
	std::vector<float> depth_buffer;
	int width, height;
public:
	Image(int w, int h) : width(w), height(h){
		color_buffer.resize(w * h, {0,0,0});
		depth_buffer.resize(w * h, std::numeric_limits<float>::max());
	}
	void clear(Pixel clear_color);
	void set_pixel(int x, int y, int z, Pixel color);
	void save_to_ppm(const std::string& filename);
};