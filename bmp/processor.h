#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__

#include <iostream>
#include "bitmap.h"
#include <math.h>

class ImageProcessor {
public:
    ImageProcessor(Bitmap *bmp);
    void rerange();
    void rerange(P2P);
    void rerange(P2, P2);
    void drawRange(RGB c);
    bool GaussianBlur(int r);
    bool GaussianBlur(P2P range, int r);
    void pixelate(int n);
private:
    Bitmap *bmp;
    P2P range;
    int _r, _l, _d, _u;
};

ImageProcessor::ImageProcessor(Bitmap *bmp) {
    this->bmp = bmp;
    rerange();
}

void ImageProcessor::drawRange(RGB c) {
    bmp->line(P2(_r-1, _u-1), P2(_r-1, _d), c);
    bmp->line(P2(_r-1, _u-1), P2(_l, _u-1), c);
    bmp->line(P2(_l, _u-1), P2(_l, _d), c);
    bmp->line(P2(_l, _d), P2(_r-1, _d), c);
}

void ImageProcessor::rerange() {
    rerange(P2P(
        P2(-bmp->getOrigin().x, -bmp->getOrigin().y),
        P2(bmp->getWidth() - bmp->getOrigin().x, bmp->getWidth() - bmp->getOrigin().y)
    ));
}

void ImageProcessor::rerange(P2 p1, P2 p2) {
    rerange(P2P(p1, p2));
}

void ImageProcessor::rerange(P2P p2p) {
    if(p2p.p1.x >= p2p.p2.x)
        _r = int(p2p.p1.x), _l = int(p2p.p2.x);
    else
        _l = int(p2p.p1.x), _r = int(p2p.p2.x);
    if(p2p.p1.y >= p2p.p2.y)
        _u = int(p2p.p1.y), _d = int(p2p.p2.y);
    else
        _d = int(p2p.p1.y), _u = int(p2p.p2.y);
}

bool ImageProcessor::GaussianBlur(P2P p2p, int r = 1) {
    rerange(p2p);
    return GaussianBlur(r);
}

bool ImageProcessor::GaussianBlur(int r = 1) {
    P2 o = bmp->getOrigin();
    int w = bmp->getWidth(), h = bmp->getHeight();
    if(w * h > 2073600)
        return false;
    RGB *buffer = new RGB[w * h];
    for(int x = 0; x < w; x++)
        for(int y = 0; y < h; y++)
            buffer[y * w + x] = bmp->get(P2(x - o.x, y - o.y));
    for(int x = _l; x < _r; x++) {
        for(int y = _d; y < _u; y++) {
            if(x < 0 - o.x || x >= w - o.x || y < 0 - o.y || y >= h - o.y)
                continue;
            int count = 0, R = 0, G = 0, B = 0;
            for(int i = x - r; i <= x + r; i++) {
                for(int j = y - r; j <= y + r; j++) {
                    int xd = i - x, yd = j - y;
                    if(xd * xd + yd * yd <= r * r && i >= 0 - o.x && j >= 0 - o.y && i < w - o.x && j < h - o.y) {
                        RGB temp = bmp->get(P2(i, j));
                        R += temp.R, G += temp.G, B += temp.B, count++;
                    }
                }
            }
            buffer[(y + int(o.y)) * w + x + int(o.x)] = RGB(
                (unsigned char)(R / count),
                (unsigned char)(G / count),
                (unsigned char)(B / count)
            );
        }
    }
    bmp->setBuffer(buffer);
    return true;
    delete [] buffer;
}


#endif /* __PROCESSOR_H__ */
