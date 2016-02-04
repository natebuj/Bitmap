#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "bitmap.h"
#include "object.h"
using namespace std;

/*
Camera

direction: (0, 0, -1) you cannot change
position: (0, 0, height)
focus: (0, 0, height + focus)
using geometric projection (diagram below)

    |-   `focus`   -|
                    |-   `height`  -|
    O---------------O---------------O
  focus       screen(camera)      object

*/
class Camera {
public:
    double focus;
    double height;
    double zoom;

    Camera();
    void shot(vector<P3>&, Bitmap&);    // shot P3 shape (connect P3 array)
    void shot(TriObj&, Bitmap&, bool);  // shot triangle object
    void shot(LinObj&, Bitmap&);        // shot line object
    void shot(BezObj&, Bitmap&);        // shot bezier object
    P2 proj(P3 p);      // geometric projection P3
    P2P proj(P3P pp);   // geometric projection P3 pair
    P2T proj(P3T pt);   // geometric projection P3 triangle
    vector<P2> proj(vector<P3>& p3v);   // geometric projection P3 array
};


Camera::Camera() {
    focus = 10000;
    height = 1000;
    zoom = 1;
}


// shot P3 shape (connect P3 array)
void Camera::shot(vector<P3>& p3v, Bitmap& bmp) {
    bmp.connect(proj(p3v));
}

// shot triangle object
void Camera::shot(TriObj& tri, Bitmap& bmp, bool solid = false) {
    for(int i = 0; i < tri.vs.size(); i++)
        if(solid)
            bmp.sTriangle(
                proj(
                    P3T(
                        (tri.vs[i].p1 * tri.scale) + tri.middle,
                        (tri.vs[i].p2 * tri.scale) + tri.middle,
                        (tri.vs[i].p3 * tri.scale) + tri.middle
                    )
                )
             );
        else
            bmp.triangle(
                proj(
                    P3T(
                        (tri.vs[i].p1 * tri.scale) + tri.middle,
                        (tri.vs[i].p2 * tri.scale) + tri.middle,
                        (tri.vs[i].p3 * tri.scale) + tri.middle
                    )
                )
            );
}

// shot line object
void Camera::shot(LinObj& line, Bitmap& bmp) {
    for(int i = 0; i < line.vs.size(); i++) {
        bmp.line(
            proj(
                P3P(
                    line.vs[i].p1 + line.middle,
                    line.vs[i].p2 + line.middle
                )
            )
        );
    }
}

// shot bezier object
void Camera::shot(BezObj& bpt, Bitmap& bmp) {
    int count = bpt.data.size();
    for(int i = 0; i < count; i++) {
        int h = bpt.data[i].size();
        for(int j = 0; j < h; j++) {
            int w = bpt.data[i][j].size();
            for(int k = 0; k < w; k++) {
                P3 scale(bpt.scale, bpt.scale, bpt.scale);
                P3 tempc = mult(bpt.data[i][j][k], scale) + bpt.middle;
                if(k < w - 1) {
                    P3 tempk = mult(bpt.data[i][j][k+1], scale) + bpt.middle;
                    bmp.line(proj(tempc), proj(tempk));
                }
                if(j < h - 1) {
                    P3 tempj = mult(bpt.data[i][j+1][k], scale) + bpt.middle;
                    bmp.line(proj(tempc), proj(tempj));
                }
            }
        }
    }
}

// geometric projection P3
P2 Camera::proj(P3 p) {
    double dist = height - p.z;
    double fd = dist + focus;
    return P2((zoom * p.x * focus) / fd, (zoom * p.y * focus) / fd);
}

// geometric projection P3 pair
P2P Camera::proj(P3P pp) {
    return P2P(proj(pp.p1), proj(pp.p2));
}

// geometric projection P3 triangle
P2T Camera::proj(P3T pt) {
    return P2T(proj(pt.p1), proj(pt.p2), proj(pt.p3));
}

// geometric projection P3 array
vector<P2> Camera::proj(vector<P3>& p3v) {
    vector<P2> p2v;
    for(int i = 0; i < p2v.size(); i++)
        p2v.push_back(proj(p3v[i]));
    return p2v;
}



#endif /* __CAMERA_H__ */
