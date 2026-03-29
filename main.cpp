#include "image.hpp"
#include "config.hpp"
#include "rasterizer.hpp"
int main(){
	Image img(WIDTH,HEIGHT);
	Rasterizer rstr(img);
	img.clear({255,255,255});
	vec2 center = {WIDTH/2,HEIGHT/2};
	Vertex v1 = {{center.x - 10, center.y + 10,0},{255,1,1}};
	Vertex v2 = {{center.x + 10, center.y - 10,0},{1,255,1}};
	Vertex v3 = {{center.x + 5, center.y + 5,0},{1,1,255}};
	rstr.draw_triangle(v1,v2,v3);
	
	// rstr.draw_dot(v1.get_2d_position(),{255,1,1});
	// rstr.draw_dot(v2.get_2d_position(),{1,255,1});
	// rstr.draw_dot(v3.get_2d_position(),{1,1,255});
	
	img.save_to_ppm("test.ppm");
}