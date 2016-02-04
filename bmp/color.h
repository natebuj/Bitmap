#ifndef __COLOR_H__
#define __COLOR_H__

class RGB {
public:
	unsigned char B, G, R;

	RGB(unsigned char R, unsigned char G, unsigned char B);
	RGB(const char *color); // initialize by color name
	RGB(int hex); // initailize by hex
	RGB(); // default color is black
	RGB operator+(RGB color);
	RGB operator-(RGB color);
	RGB avg(); // average R G B
};


RGB::RGB(unsigned char R, unsigned char G, unsigned char B) {
	this->R = R, this->G = G, this->B = B;
}

// initialize by color name
RGB::RGB(const char *color) {
	if(color == "black")
		R = G = B = 0;
	else if(color == "white")
		R = G = B = 255;
	else if(color == "gray")
		R = G = B = 127;
	else if(color == "red")
		R = 255, G = B = 0;
	else if(color == "green")
		G = 255, R = B = 0;
	else if(color == "blue")
		B = 255, R = G = 0;
	else if(color == "light_red")
		R = 255, G = B = 127;
	else if(color == "light_green")
		G = 255, R = B = 127;
	else if(color == "light_blue")
		B = 255, R = G = 127;
	else if(color == "dark_red")
		R = 127, G = B = 0;
	else if(color == "dark_green")
		G = 127, R = B = 0;
	else if(color == "navy")
		B = 127, R = G = 0;
	else if(color == "silver")
		R = G = B = 192;
	else if(color == "aqua")
		R = 0, G = 0xFF, B = 0xFF;
	else if(color == "purple")
		R = 128, G = 0, B = 128;
	else if(color == "violet")
		R = 238, G = 130, B = 238;
	else if(color == "sky_blue")
		R = 135, G = 206, B = 235;
	else if(color == "yellow")
		R = 255, G = 255, B = 0;
	else if(color == "orange")
		R = 255, G = 80, B = 0;
	else if(color == "olive")
		R = G = 128, B = 0;
}

// initailize by hex
RGB::RGB(int hex) {
	if(hex >= 0 && hex <= 0xFFFFFF) {
		R = (unsigned char)(hex / 0x10000);
		G = (unsigned char)((hex % 0x10000) / 0x100);
		B = (hex % 0x100);
	}
}

// default color is black
RGB::RGB() {
	this->R = this->G = this->B = 0;
}

RGB RGB::operator+(RGB color) {
	int rr = int(color.R) + int(R);
	int gg = int(color.G) + int(G);
	int bb = int(color.B) + int(B);
	if(rr > 255) rr = 255;
	if(gg > 255) gg = 255;
	if(bb > 255) bb = 255;
	return RGB((unsigned char)rr, (unsigned char)gg, (unsigned char)bb);
}

RGB RGB::operator-(RGB color) {
	int rr = -int(color.R) + int(R);
	int gg = -int(color.G) + int(G);
	int bb = -int(color.B) + int(B);
	if(rr < 0) rr = 0;
	if(gg < 0) gg = 0;
	if(bb < 0) bb = 0;
	return RGB((unsigned char)rr, (unsigned char)gg, (unsigned char)bb);
}

// average R G B
RGB RGB::avg() {
	unsigned char average = (unsigned char)((int(R) + int(G) + int(B)) / 3);
	return RGB(average, average, average);
}



#endif /* __COLOR_H__ */
