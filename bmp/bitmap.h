#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <fstream>
#include <vector>
#include <cmath>
#include "color.h"
#include "point.h"

using namespace std;

#pragma pack(1)
struct BitmapHeader {
	char identity[2]; // 'B' 'M'
	unsigned int file_size;
	unsigned short reserved[2];
	unsigned int data_offset; // 54
	unsigned int info_size; // 0x28
	int width;
	int height;
	unsigned short planes; // 1
	unsigned short bits_per_pixel;
	unsigned int compression; // 0
	unsigned int data_size;
	int hresolution;
	int vresolution;
	unsigned int used_colors;
	unsigned int important_colors;
};
#pragma pack()

void bitmapHeaderInit(struct BitmapHeader& header, int w, int h) {
	header.identity[0] = 'B';
	header.identity[1] = 'M';
	header.file_size = (w * 3) * h + 54;
	header.reserved[0] = 0;
	header.reserved[1] = 0;
	header.data_offset = 0x36;
	header.info_size = 0x28;
	header.width = w;
	header.height = h;
	header.planes = 1;
	header.bits_per_pixel = 24;
	header.compression = 0;
	header.data_size = (w * 3) * h;
	header.hresolution = 0;
	header.vresolution = 0;
	header.used_colors = 0;
	header.important_colors = 0;
}


class Bitmap {
public:
	Bitmap();
	Bitmap(int width, int height);
	Bitmap(const char* name);
	Bitmap(const char* name, int width, int height);
	~Bitmap();
	void line(P2, P2, RGB);
	void line(P2 p1, P2 p2);
	void line(P2P pp);
	void line(P2P pp, RGB c);
	void triangle(P2, P2, P2);
	void triangle(P2T pt);
	void sTriangle(P2, P2, P2);
	void sTriangle(P2T pt);
	void sTriangle(P2T pt, RGB rgb);
	void p4(P2, P2, P2, P2);
	void ball(P2, double);
	void circle(P2, double);
	void square(P2, P2);
	void sSquare(P2, P2);
	void frame(const vector<P2>&);
	void connect(const vector<P2>&);
	void set(P2 point);
	void set(P2 point, const RGB color);
	void set(const vector<P2>&);
	void set(const vector<P2>&, const RGB color);
	void set(const RGB color);
	bool setSize(int width, int height);
	void setName(const char* name);
	void setColor(const RGB color);
	void setColor(unsigned char R, unsigned char G, unsigned char B);
	void setOrigin(P2 point);
	void setOriginCenter();
	RGB get(P2 point);
	int getWidth();
	int getHeight();
	P2 getOrigin();
	void save(const char* name);
	bool load(const char*);
	void setBuffer(RGB *buffer);
private:
	int width, height;
	const char* name;
	struct BitmapHeader header;
	RGB *buffer = NULL;
	P2 origin;
	RGB color;
	static const double PI = 3.1416;
	double deg2rad(double deg) { return (deg * 3.1416) / 180; }
	double rad2deg(double rad) { return (rad * 180) / 3.1416; }
};


// Constructors

Bitmap::Bitmap() {
	this->name = "";
	setColor(RGB(255, 255, 255));
}

Bitmap::Bitmap(int width, int height) {
	this->name = NULL;
	setSize(width, height);
	set(RGB(0, 0, 0));
	setColor(RGB(255, 255, 255));
}

Bitmap::Bitmap(const char* name) {
	load(name);
}

Bitmap::Bitmap(const char* name, int width, int height) {
	setName(name);
	setSize(width, height);
	set(RGB(0, 0, 0));
	setColor(RGB(255, 255, 255));
}

Bitmap::~Bitmap() {
	if(this->name)
		save(this->name);
	delete [] buffer;
}


// Drawing

void Bitmap::square(P2 m, P2 p) {
	P2 v1 = p - m;
	P2 v2 = P2(v1.y, -v1.x);
	vector<P2> vtx;
	vtx.push_back(m + v1); vtx.push_back(m + v2);
	vtx.push_back(m - v1); vtx.push_back(m - v2);
	connect(vtx);
}

void Bitmap::sSquare(P2 m, P2 p) {
	P2 v1 = p - m;
	P2 v2 = P2(v1.y, -v1.x);
	sTriangle(m + v1, m - v1, m + v2);
	sTriangle(m + v1, m - v1, m - v2);
}

void Bitmap::line(P2 p1, P2 p2) {
	line(p1, p2, color);
}

void Bitmap::line(P2P pp) {
	line(pp.p1, pp.p2, color);
}

void Bitmap::line(P2P pp, RGB c) {
	line(pp.p1, pp.p2, c);
}

void Bitmap::triangle(P2T pt) {
	triangle(pt.p1, pt.p2, pt.p3);
}

void Bitmap::sTriangle(P2T pt) {
	sTriangle(pt.p1, pt.p2, pt.p3);
}

void Bitmap::sTriangle(P2T pt, RGB rgb) {
	RGB temp = color;
	color = rgb;
	sTriangle(pt);
	color = temp;
}

void Bitmap::line(P2 p1, P2 p2, RGB c) {
	double dx = p2.x - p1.x, dy = p2.y - p1.y;
	if(dx == 0 || dy == 0) {
		if(dx != 0) {
			if(dx > 0)
				for(int i=p1.x; i<=p2.x; i++)
					set(P2(double(i), p1.y), c);
			else if(dx < 0)
				for(int i=p1.x; i>=p2.x; i--)
					set(P2(double(i), p1.y), c);
		} else if(dy != 0) {
			if(dy > 0)
				for(int j=p1.y; j<=p2.y; j++)
					set(P2(p1.x, double(j)), c);
			else if(dy < 0)
				for(int j=p1.y; j>=p2.y; j--)
					set(P2(p1.x, double(j)), c);
		} else
			set(p1, color);
	} else {
		if(abs(dx) >= abs(dy)){
			if(dx > 0)
				for(int i=p1.x; i<=p2.x; i++)
					set(P2(double(i), p1.y + (dy * (double(i) - p1.x)) / dx), c);
			else if(dx < 0)
				for(int i=p1.x; i>=p2.x; i--)
					set(P2(double(i), p1.y + (dy * (double(i) - p1.x)) / dx), c);
		} else {
			if(dy > 0)
				for(int j=p1.y; j<=p2.y; j++)
					set(P2(p1.x + (dx * (double(j) - p1.y)) / dy, double(j)), c);
			else if(dy < 0)
				for(int j=p1.y; j>=p2.y; j--)
					set(P2(p1.x + (dx * (double(j) - p1.y)) / dy, double(j)), c);
		}
	}
}

void Bitmap::triangle(P2 p1, P2 p2, P2 p3) {
	line(p1, p2);
	line(p1, p3);
	line(p2, p3);
}

void Bitmap::sTriangle(P2 p1, P2 p2, P2 p3) {
	double area2o = fabs(det(p3 - p1, p2 - p1));
	for(int i = 0 - int(origin.x); i < width - int(origin.x); i++) {
		for(int j = 0 - int(origin.y); j < height - int(origin.y); j++) {
			double area2n = 0.0;
			area2n += fabs(det(P2(i, j) - p1, P2(i, j) - p2));
			area2n += fabs(det(P2(i, j) - p2, P2(i, j) - p3));
			area2n += fabs(det(P2(i, j) - p3, P2(i, j) - p1));
			if(area2n <= area2o)
				set(P2(i, j));
		}
	}
}

void Bitmap::p4(P2 p1, P2 p2, P2 p3, P2 p4) {
	triangle(p1, p2, p3);
	triangle(p1, p2, p4);
}

void Bitmap::frame(const vector<P2>& pv) {
	for(int i = 0; i < pv.size(); i++)
		for(int j = 0; j < pv.size(); j++)
			if(i != j)
				line(pv[i], pv[j]);
}

void Bitmap::connect(const vector<P2>& pv) {
	for(int i = 1; i < pv.size(); i++)
		line(pv[i - 1], pv[i]);
	line(pv[0], pv[pv.size() - 1]);
	//for(int i = 0; i < pv.size(); i++) ball(pv[i], 3.0);
}

void Bitmap::circle(P2 p, double r) {
	P2 last = p + P2(r, 0.0);
	for(int n = 0; n <= 720; n++) {
		P2 now = P2(
			cos(deg2rad(double(n)/2.0)) * r + p.x,
			sin(deg2rad(double(n)/2.0)) * r + p.y
		);
		line(last, now);
		last = now;
	}
}

void Bitmap::ball(P2 p, double r) {
	for(int i = 0 - int(origin.x); i < width - int(origin.x); i++) {
		for(int j = 0 - int(origin.y); j < height - int(origin.y); j++) {
			P2 d = P2(i, j) - p;
			if(d.x * d.x + d.y * d.y <= r * r)
				set(P2(i, j));
		}
	}
}



// Pixel

void Bitmap::set(P2 point) {
	set(point, this->color);
}

void Bitmap::set(const RGB color) {
	for(int i = 0; i < width * height; i++)
		buffer[i] = color;
}

RGB Bitmap::get(P2 point) {
	P2 temp = point + origin;
	int x = int(temp.x), y = int(temp.y);
	if(x >= 0 && y >= 0 && x < width && y < height)
		return buffer[y * width + x];
	else
		return RGB(0, 0, 0);
}

void Bitmap::set(P2 point, const RGB color) {
	P2 temp = origin + point;
	int x = int(temp.x), y = int(temp.y);
	if(x < width && x >= 0 && y < height && y >= 0)
		buffer[y * width + x] = color;
}

void Bitmap::set(const vector<P2>& v, const RGB color) {
	for(int i = 0; i < v.size(); i++)
		set(v[i], color);
}
void Bitmap::set(const vector<P2>& v) {
	for(int i = 0; i < v.size(); i++)
		set(v[i]);
}


// Content

void Bitmap::setName(const char* name) {
	this->name = name;
}

void Bitmap::setOrigin(P2 point) {
	origin = point;
}

void Bitmap::setOriginCenter() {
	origin = P2(width / 2 - 1, height / 2 - 1);
}

int Bitmap::getWidth() {
	return width;
}

int Bitmap::getHeight() {
	return height;
}

P2 Bitmap::getOrigin() {
	return origin;
}

void Bitmap::setBuffer(RGB *buffer) {
	for(int i = 0; i < width * height; i++)
		this->buffer[i] = buffer[i];
}

void Bitmap::save(const char* name) {
	fstream file(name, ios::out | ios::binary);
	file.write((char*)&header, sizeof(header)); // write header
	for(int i = 0; i < width * height; i++) // write buffer
		file.write((char*)&buffer[i], sizeof(RGB));
	file.close();
}

bool Bitmap::load(const char* name) {
	fstream file(name, ios::in | ios::binary);
	if(!file)
		return false;
	struct BitmapHeader h;
	file.read((char*)&h, sizeof(h));
	if(!setSize(h.width, h.height))
		return false;
	for(int i = 0; i < h.width * h.height; i++)
		file.read((char*)&buffer[i], sizeof(RGB));
	file.close();
	return true;
}

bool Bitmap::setSize(int width, int height) {
	if(buffer)
		delete [] buffer;
	this->width = width;
	this->height = height;
	if(width * height > 2073600)
		return false;
	bitmapHeaderInit(header, width, height);
	buffer = new RGB[width * height];
	setOrigin(P2(width / 2 - 1, height / 2 - 1));
	return true;
}

void Bitmap::setColor(const RGB color) {
	this->color = color;
}

void Bitmap::setColor(unsigned char R, unsigned char G, unsigned char B) {
	this->color.R = R, this->color.G = G, this->color.B = B;
}


#endif /* __BITMAP_H__ */
