#ifndef __POINT_H__
#define __POINT_H__

#include <cmath>


class P3 {
public:
    double x, y, z;

    P3() {}
    P3(int x, int y, int z);
    P3(double x, double y, double z);
    double len();               // length
    double len2();              // length square
    P3 unit();                  // unit vector
    double operator*(P3 point); // dot product
    double dot(P3 p);           // dot product
    P3 operator%(P3 point);     // cross product, % looks like X
    P3 cross(P3 p);             // cross product
    P3 operator+(P3 point);     // sum
    P3 operator+=(P3 point);    // sum
    P3 operator-();             // negative
    P3 operator-(P3 point);     // difference
    P3 operator-=(P3 point);    // difference
    P3 operator*(double n);     // constant product
    P3 operator*(int n);        // constant product
    P3 operator/(double n);     // constant quotient
    P3 operator/(int n);        // constant quotient
};

class P2 {
public:
    double x, y;

    P2() {}
    P2(int x, int y);
    P2(double x, double y);
    P2(P3 p);
    double operator*(P2 point);     // dot product
    double len();                   // length
    double len2();                  // length square
    P2 unit();                      // unit vector
    P2 operator+(P2 point);         // sum
    P2 operator-(P2 point);         // difference
    P2 operator-();                 // negative
    P2 operator+=(P2 point);        // sum
    P2 operator-=(P2 point);        // difference
    P2 operator*(double n);         // constant product double
    P2 operator*(int n);            // constant product int
    P2 operator/(double n);         // constant quotient double
    P2 operator/(int n);            // constant quotient int
};

class P2P {
public:
    P2 p1, p2;

    P2P();
    P2P(P2 a, P2 b);
    P2 vector();    // vector
    double len();   // length of vector
    double len2();  // length square of vector
};

class P3P {
public:
    P3 p1, p2;

    P3P();
    P3P(P3 a, P3 b);
    P3 vector();    // vector
    double len();   // length of vector
    double len2();  // length square of vector
};

class P2T {
public:
    P2 p1, p2, p3;

    P2T();
    P2T(P2 a, P2 b, P2 c);
    double area();  // area
};

class P3T {
public:
    P3 p1, p2, p3;

    P3T();
    P3T(P3 a, P3 b, P3 c);
    double area();  // area
    P3 norm();      // normal vector
};

P3 mult(P3 p1, P3 p2);      // multiply 3D pairs
P2 mult(P2 p1, P2 p2);      // multiply 2D pairs
P2 div(P2 p1, P2 p2);       // divide 2D pairs
P3 div(P3 p1, P3 p2);       // divide 3D pairs
double det(P2 p1, P2 p2);   // delta 2D points
double det(P3 p1, P3 p2);   // delta 3D points
double det(P2P pair);       // delta 2D pair
double det(P3P pair);       // delta 3D pair
void rotateX(P3 &p, double theta);  // rotate by X axis
void rotateY(P3 &p, double theta);  // rotate by Y axis
void rotateZ(P3 &p, double theta);  // rotate by Z axis








/*
P2 Member Functions
*/

//P2::P2() {}

P2::P2(int x, int y) {
    this->x = double(x);
    this->y = double(y);
}

P2::P2(double x, double y) {
    this->x = x;
    this->y = y;
}

P2::P2(P3 p) {
    this->x = p.x;
    this->y = p.y;
}

// dot product
double P2::operator*(P2 point) {
    return this->x * point.x  + this->y * point.y;
}

double P2::len() {
// length
    return sqrt(x * x + y * y);
}

// length square
double P2::len2() {
    return x * x + y * y;
}

// unit vector
P2 P2::unit() {
    return P2(x / len(), y / len());
}

// sum
P2 P2::operator+(P2 point) {
    return P2(this->x + point.x, this->y + point.y);
}

// difference
P2 P2::operator-(P2 point) {
    return P2(this->x - point.x, this->y - point.y);
}

// negative
P2 P2::operator-() {
    return P2(-this->x, -this->y);
}

P2 P2::operator+=(P2 point) {
// sum
    this->x += point.x;
    this->y += point.y;
}

P2 P2::operator-=(P2 point) {
// difference
    this->x -= point.x;
    this->y -= point.y;
}

P2 P2::operator*(double n) {
// constant product double
    return P2(this->x * n, this->y * n);
}

// constant product int
P2 P2::operator*(int n) {
    return P2(this->x * double(n), this->y * double(n));
}

// constant quotient double
P2 P2::operator/(double n) {
    if(n == 0.0)
        return P2(0, 0);
    return P2(this->x / n, this->y / n);
}

// constant quotient int
P2 P2::operator/(int n) {
    if(n == 0)
        return P2(0, 0);
    return P2(this->x / double(n), this->y / double(n));
}








/*
P3 Member Functions
*/

//P3::P3() {}

P3::P3(int x, int y, int z) {
    this->x = double(x);
    this->y = double(y);
    this->z = double(z);
}

P3::P3(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

// length
double P3::len() {
    return sqrt(x * x + y * y + z * z);
}

// length square
double P3::len2() {
    return x * x + y * y + z * z;
}

// unit vector
P3 P3::unit() {
    return P3(x / len(), y / len(), z / len());
}

// dot product
double P3::operator*(P3 point) {
    return point.x * x + point.y * y + point.z * z;
}

// dot product
double P3::dot(P3 p) {
    return p.x * z + p.y * y + p.z * z;
}

// cross product              % looks like X
P3 P3::operator%(P3 point) {
    return P3(
        x * point.z - z * point.y,
        z * point.x - x * point.z,
        y * point.y - y * point.x
    );
}

// cross product
P3 P3::cross(P3 p) {
    return P3(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

// sum
P3 P3::operator+(P3 point) {
    return P3(this->x + point.x, this->y + point.y, this->z + point.z);
}

// sum
P3 P3::operator+=(P3 point) {
    this->x += point.x;
    this->y += point.y;
    this->z += point.z;
}

// negative
P3 P3::operator-() {
    return P3(-this->x, -this->y, -this->z);
}

// difference
P3 P3::operator-(P3 point) {
    return P3(this->x - point.x, this->y - point.y, this->z - point.z);
}

// difference
P3 P3::operator-=(P3 point) {
    this->x -= point.x;
    this->y -= point.y;
    this->z -= point.z;
}

// constant product
P3 P3::operator*(double n) {
    return P3(this->x * n, this->y * n, this->z * n);
}

// constant product
P3 P3::operator*(int n) {
    return P3(this->x * double(n), this->y * double(n), this->z * double(n));
}

// constant quotient
P3 P3::operator/(double n) {
    if(n == 0.0)
    return P3(0, 0, 0);
    return P3(this->x / n, this->y / n, this->z / n);
}

// constant quotient
P3 P3::operator/(int n) {
    if(n == 0)
        return P3(0, 0, 0);
    return P3(this->x / double(n), this->y / double(n), this->z / double(n));
}








/*
P2P Member Functions
*/

P2P::P2P() {
    p2 = p1 = P2(0, 0);
}

P2P::P2P(P2 a, P2 b) {
    p1 = a, p2 = b;
}

// vector
P2 P2P::vector() {
    return p2 - p1;
}

// length of vector
double P2P::len() {
    return vector().len();
}

// length square of vector
double P2P::len2() {
    return vector().len2();
}








/*
P3P Member Functions
*/

P3P::P3P() {
    p2 = p1 = P3(0, 0, 0);
}

P3P::P3P(P3 a, P3 b) {
    p1 = a, p2 = b;
}

// vector
P3 P3P::vector() {
    return p2 - p1;
}

// length of vector
double P3P::len() {
    return vector().len();
}

// length square of vector
double P3P::len2() {
    return vector().len2();
}








/*
P2T Member Functions
*/

P2T::P2T() {
    p3 = p2 = p1 = P2(0, 0);
}

P2T::P2T(P2 a, P2 b, P2 c) {
    p1 = a, p2 = b, p3 = c;
}

// area
double P2T::area() {
    return fabs(det(p2 - p1, p3 - p1)) / 2.0;
}








/*
P3T Member Functions
*/

P3T::P3T() {
    p3 = p2 = p1 = P3(0, 0, 0);
}

P3T::P3T(P3 a, P3 b, P3 c) {
    p1 = a, p2 = b, p3 = c;
}

// area
double P3T::area() {
    return fabs(det(p2 - p1, p3 - p1)) / 2.0;
}

// normal vector
P3 P3T::norm() {
    return (p2 - p1) % (p3 - p1);
}








/*
Extra Functions
*/

// multiply 3D pairs
P3 mult(P3 p1, P3 p2) {
    return P3(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z);
}

// multiply 2D pairs
P2 mult(P2 p1, P2 p2) {
    return P2(p1.x * p2.x, p1.y * p2.y);
}

// divide 2D pairs
P2 div(P2 p1, P2 p2) {
    if(p2.x * p2.y == 0)
    return p1;
    else
    return P2(p1.x / p2.x, p1.y / p2.y);
}

// divide 3D pairs
P3 div(P3 p1, P3 p2) {
    if(p2.x * p2.y * p2.z == 0)
    return p1;
    else
    return P3(p1.x / p2.x, p1.y / p2.y, p1.z / p2.z);
}

// delta 2D points
double det(P2 p1, P2 p2) {
    return p1.x * p2.y - p1.y * p2.x;
}

// delta 3D points
double det(P3 p1, P3 p2) {
    return p1.x * p2.y - p1.y * p2.x + p1.y * p2.z - p1.z * p2.y + p1.z * p2.x - p1.x * p2.z;
}

// delta 2D pair
double det(P2P pair) {
    P2 p1 = pair.p1, p2 = pair.p2;
    return p1.x * p2.y - p1.y * p2.x;
}

// delta 3D pair
double det(P3P pair) {
    P3 p1 = pair.p1, p2 = pair.p2;
    return p1.x * p2.y - p1.y * p2.x + p1.y * p2.z - p1.z * p2.y + p1.z * p2.x - p1.x * p2.z;
}

// rotate by X axis
void rotateX(P3 &p, double theta) {
    double rad = (theta * 3.1416) / 180.0;
    double y = p.y * cos(rad) - p.z * sin(rad);
    double z = p.y * sin(rad) + p.z * cos(rad);
    p = P3(p.x, y, z);
}

// rotate by Y axis
void rotateY(P3 &p, double theta) {
    double rad = (theta * 3.1416) / 180.0;
    double z = p.z * cos(rad) - p.x * sin(rad);
    double x = p.z * sin(rad) + p.x * cos(rad);
    p = P3(x, p.y, z);
}

// rotate by Z axis
void rotateZ(P3 &p, double theta) {
    double rad = (theta * 3.1416) / 180.0;
    double x = p.x * cos(rad) - p.y * sin(rad);
    double y = p.x * sin(rad) + p.y * cos(rad);
    p = P3(x, y, p.z);
}


#endif /* __POINT_H__ */
