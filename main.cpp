#include "image.hpp"
#include "config.hpp"

int main(){
	Image img(config::ASPECT_RATIO[0]*config::RESOLUTION,config::ASPECT_RATIO[1]*config::RESOLUTION);
	img.clear({0,255,0});
	img.save_to_ppm("test.ppm");
}