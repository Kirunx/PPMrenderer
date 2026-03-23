#include "image.hpp"
#include "config.hpp"
#include "rasterizer.hpp"
int main(){
	Image img(WIDTH,HEIGHT);
	Rasterizer rstr(img);
	img.clear({0,255,0});
	vec2 center = {WIDTH/2,HEIGHT/2};
	rstr.draw_line({5,5},center,{255,0,255});



	img.save_to_ppm("test.ppm");
}