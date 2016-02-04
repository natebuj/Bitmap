#ifndef __OBJECT_H__
#define __OBJECT_H

#include "point.h"
#include <fstream>
#include <vector>
using namespace std;

class BezObj {
public:
    double scale;
    P3 middle;
    vector< vector< vector< P3 > > > data;
    BezObj();
    BezObj(const char* name);
    void load(const char* name); // load bpt file
    void clear(); // clear data
    void move(P3 d); // move position
    void rotateX(double theta); // X axis centered rotate
    void rotateY(double theta); // Y axis centered rotate
    void rotateZ(double theta); // Z axis centered rotate
    void split(); // bezier split
    void split(int n); // bezier split n times
private:
    void splitA();
    void splitB();
};

class LinObj {
public:
    double scale;
    P3 middle;
    vector<P3P> vs;
    LinObj();
    LinObj(const char* name);
    void load(const char* name);// load file
    void addLine(P3 p1, P3 p2); // add line
    void addLine(P3P);          // add line
    void clear();               // clear data
    void move(P3 p);
    void rotateX(double theta);
    void rotateY(double theta);
    void rotateZ(double theta);
};

class TriObj {
public:
    double scale;
    P3 middle;
    vector<P3T> vs;
    TriObj();
    TriObj(const char* name);
    void load(const char* name); // load file
    void addTriangle(P3 p1, P3 p2, P3 p3); // add triangle
    void addTriangle(P3T p3t); // add triangle P3T
    void clear(); // clear data
    void move(P3 p); // move
    void rotateX(double theta);
    void rotateY(double theta);
    void rotateZ(double theta);
};


/******************************************************************************/
//  BezObj Member Functions
/******************************************************************************/

BezObj::BezObj() {
    middle = P3(0, 0, 0);
    scale = 100;
}

BezObj::BezObj(const char* name) {
    load(name);
    middle = P3(0, 0, 0);
    scale = 100;
}

// load bpt file
void BezObj::load(const char* name) {
    clear();
    ifstream file(name);
    if(!file)
        return;
    int count, w, h;
    file >> count;
    for(int i = 0; i < count; i++) {
        file >> w >> h;
        vector< vector<P3> > temp1;
        for(int j = 0; j < h + 1; j++) {
            vector<P3> temp0;
            for(int k = 0; k < w + 1; k++) {
                P3 input;
                file >> input.x >> input.y >> input.z;
                temp0.push_back(input);
            }
            temp1.push_back(temp0);
        }
        data.push_back(temp1);
    }
    file.close();
}

// clear data
void BezObj::clear() {
    for(int i = 0; i < data.size(); i++) {
        for(int j = 0; j < data[i].size(); j++) {
            data[i][j].clear();
        }
        data[i].clear();
    }
    data.clear();
}

// move position
void BezObj::move(P3 d) {
    middle += d;
}

// X axis centered rotate
void BezObj::rotateX(double theta) {
    double rad = (theta * 3.1416) / 180.0;
    for(int i = 0; i < data.size(); i++) {
        for(int j = 0; j < data[i].size(); j++) {
            for(int k = 0; k < data[i][j].size(); k++) {
                double y1 = data[i][j][k].y * cos(rad) - data[i][j][k].z * sin(rad);
                double z1 = data[i][j][k].y * sin(rad) + data[i][j][k].z * cos(rad);
                data[i][j][k] = P3(data[i][j][k].x, y1, z1);
            }
        }
    }
}

// Y axis centered rotate
void BezObj::rotateY(double theta) {
    double rad = (theta * 3.1416) / 180.0;
    for(int i = 0; i < data.size(); i++) {
        for(int j = 0; j < data[i].size(); j++) {
            for(int k = 0; k < data[i][j].size(); k++) {
                double z1 = data[i][j][k].z * cos(rad) - data[i][j][k].x * sin(rad);
                double x1 = data[i][j][k].z * sin(rad) + data[i][j][k].x * cos(rad);
                data[i][j][k] = P3(x1, data[i][j][k].y, z1);
            }
        }
    }
}

// Z axis centered rotate
void BezObj::rotateZ(double theta) {
    double rad = (theta * 3.1416) / 180.0;
    for(int i = 0; i < data.size(); i++) {
        for(int j = 0; j < data[i].size(); j++) {
            for(int k = 0; k < data[i][j].size(); k++) {
                double x1 = data[i][j][k].x * cos(rad) - data[i][j][k].y * sin(rad);
                double y1 = data[i][j][k].x * sin(rad) + data[i][j][k].y * cos(rad);
                data[i][j][k] = P3(x1, y1, data[i][j][k].z);
            }
        }
    }
}

// bezier split
void BezObj::split() {
    splitA(); splitB();
}

// bezier split n times
void BezObj::split(int n) {
    if(n > 0 && n < 7) {
        splitA(); splitB();
        split(n - 1);
    }
}

void BezObj::splitA() {
    int dataSize = data.size();
    for(int i = 0; i < dataSize; i++) {
        vector< vector<P3> > &temp = data[i];
        vector< vector<P3> > addA, addB;
        for(int j = 0; j < 4; j++) {
            vector<P3> partA, partB;
            P3 midA = (temp[j][0] + temp[j][1]) / 2;
            P3 midB = (temp[j][1] + temp[j][2]) / 2;
            P3 midC = (temp[j][2] + temp[j][3]) / 2;
            P3 midD = (midA + midB) / 2;
            P3 midE = (midB + midC) / 2;
            P3 midF = (midD + midE) / 2;
            partA.push_back(temp[j][0]); partA.push_back(midA); partA.push_back(midD); partA.push_back(midF);
            partB.push_back(midF); partB.push_back(midE); partB.push_back(midC); partB.push_back(temp[j][3]);
            addA.push_back(partA); addB.push_back(partB);
        }
        data[i] = addA; data.push_back(addB);
    }
}
void BezObj::splitB() {
    int dataSize = data.size();
    for(int i = 0; i < dataSize; i++) {
        vector< vector<P3> > &temp = data[i];
        vector< vector<P3> > addA, addB;
        for(int j = 0; j < 4; j++) {
            vector<P3> partA, partB;
            P3 midA = (temp[0][j] + temp[1][j]) / 2;
            P3 midB = (temp[1][j] + temp[2][j]) / 2;
            P3 midC = (temp[2][j] + temp[3][j]) / 2;
            P3 midD = (midA + midB) / 2;
            P3 midE = (midB + midC) / 2;
            P3 midF = (midD + midE) / 2;
            partA.push_back(temp[0][j]); partA.push_back(midA); partA.push_back(midD); partA.push_back(midF);
            partB.push_back(midF); partB.push_back(midE); partB.push_back(midC); partB.push_back(temp[3][j]);
            addA.push_back(partA); addB.push_back(partB);
        }
        data[i] = addA; data.push_back(addB);
    }
}


/******************************************************************************/
//  LinObj Member Functions
/******************************************************************************/

LinObj::LinObj() {
    middle = P3(0, 0, 0);
    scale = 1;
}

LinObj::LinObj(const char* name) {
    clear();
    load(name);
    middle = P3(0, 0, 0);
    scale = 1;
}

// load file
void LinObj::load(const char* name) {
    clear();
    ifstream file(name);
    P3P shape;
    while(file >>
        shape.p1.x >> shape.p1.y >> shape.p1.z >>
        shape.p2.x >> shape.p2.y >> shape.p2.z)
        vs.push_back(shape);
}

// add line
void LinObj::addLine(P3 p1, P3 p2) {
    vs.push_back(P3P(p1, p2));
}

// add line
void LinObj::addLine(P3P p3p) {
    vs.push_back(p3p);
}

// clear data
void LinObj::clear() {
    vs.clear();
}

// move
void LinObj::move(P3 p) {
    middle += p;
}

void LinObj::rotateX(double theta) {
    for(int i = 0; i < vs.size(); i++) {
        ::rotateX(vs[i].p1, theta);
        ::rotateX(vs[i].p2, theta);
    }
}
void LinObj::rotateY(double theta) {
    for(int i = 0; i < vs.size(); i++) {
        ::rotateY(vs[i].p1, theta);
        ::rotateY(vs[i].p2, theta);
    }
}
void LinObj::rotateZ(double theta) {
    for(int i = 0; i < vs.size(); i++) {
        ::rotateZ(vs[i].p1, theta);
        ::rotateZ(vs[i].p2, theta);
    }
}


/******************************************************************************/
//  TriObj Member Functions
/******************************************************************************/

TriObj::TriObj() {
    middle = P3(0, 0, 0);
    scale = 1;
}

TriObj::TriObj(const char* name) {
    clear();
    load(name);
    middle = P3(0, 0, 0);
    scale = 1;
}

// load file
void TriObj::load(const char* name) {
    clear();
    ifstream file(name);
    P3T shape;
    while(file >>
        shape.p1.x >> shape.p1.y >> shape.p1.z >>
        shape.p2.x >> shape.p2.y >> shape.p2.z >>
        shape.p3.x >> shape.p3.y >> shape.p3.z)
        vs.push_back(shape);
}

// add triangle
void TriObj::addTriangle(P3 p1, P3 p2, P3 p3) {
    vs.push_back(P3T(p1, p2, p3));
}

// add triangle P3T
void TriObj::addTriangle(P3T p3t) {
    vs.push_back(p3t);
}

// clear data
void TriObj::clear() {
    vs.clear();
}

// move
void TriObj::move(P3 p) {
    middle += p;
}

void TriObj::rotateX(double theta) {
    for(int i = 0; i < vs.size(); i++) {
        ::rotateX(vs[i].p1, theta);
        ::rotateX(vs[i].p2, theta);
        ::rotateX(vs[i].p3, theta);
    }
}
void TriObj::rotateY(double theta) {
    for(int i = 0; i < vs.size(); i++) {
        ::rotateY(vs[i].p1, theta);
        ::rotateY(vs[i].p2, theta);
        ::rotateY(vs[i].p3, theta);
    }
}
void TriObj::rotateZ(double theta) {
    for(int i = 0; i < vs.size(); i++) {
        ::rotateZ(vs[i].p1, theta);
        ::rotateZ(vs[i].p2, theta);
        ::rotateZ(vs[i].p3, theta);
    }
}

#endif /* __OBJECT_H__ */
