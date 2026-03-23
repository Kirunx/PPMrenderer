#include "image.hpp"
#include "config.hpp"
#include "rasterizer.hpp"
int main(){
	Image img(config::ASPECT_RATIO[0]*config::RESOLUTION,config::ASPECT_RATIO[1]*config::RESOLUTION);
	Rasterizer rstr(img);
	img.clear({0,255,0});
	rstr.draw_line({0,0},{5,5},{255,0,255});



	img.save_to_ppm("test.ppm");
}