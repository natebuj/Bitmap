#include "bmp/include.h"

#define INPUT "bpt\\teapotCGAtall.bpt"
#define OUTPUT "output\\output.bmp"

int main() {
    Bitmap bitmap(640, 480);
    Camera cam; cam.zoom = 1, cam.focus = 10000, cam.height = 100;
    BezObj teapot(INPUT);
    teapot.move(P3(0, -100, -1000));
    teapot.rotateZ(-45);
    teapot.rotateX(-45);
    teapot.split(2);
    cam.shot(teapot, bitmap);
    ImageProcessor ip(&bitmap);
    ip.rerange(
        P2(-320, -240),
        P2(0, 0)
    );
    ip.GaussianBlur(2);
    bitmap.save(OUTPUT);
    system(OUTPUT);
    return 0;
}
