
#include <cmath>
#include <ostream>
#include <iostream>
using namespace std;


class Vec3f {
public:
	float v[3];
public:
	Vec3f();
	Vec3f(float x, float y, float z);
	float &operator[](int index);
	float operator[](int index) const;
	Vec3f operator*(float scale) const;
	Vec3f operator/(float scale) const;
	Vec3f operator+(const Vec3f &other) const;
	Vec3f operator-(const Vec3f &other) const;
	Vec3f operator-() const;
	const Vec3f &operator*=(float scale);
	const Vec3f &operator/=(float scale);
	const Vec3f &operator+=(const Vec3f &other);
	const Vec3f &operator-=(const Vec3f &other);
	float magnitude() const;
	float magnitudeSquared() const;
	Vec3f normalize() const;
	float dot(const Vec3f &other) const;
	Vec3f cross(const Vec3f &other) const;
};
Vec3f::Vec3f() {
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
}
Vec3f::Vec3f(float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}
float &Vec3f::operator[](int index) {
	return v[index];
}
float Vec3f::operator[](int index) const {
	return v[index];
}
Vec3f Vec3f::operator*(float scale) const {
	return Vec3f(v[0] * scale, v[1] * scale, v[2] * scale);
}
Vec3f Vec3f::operator/(float scale) const {
	return Vec3f(v[0] / scale, v[1] / scale, v[2] / scale);
}
Vec3f Vec3f::operator+(const Vec3f &other)  const {
	return Vec3f(v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]);
}
Vec3f Vec3f::operator-(const Vec3f &other) const {
	return Vec3f(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]);
}
Vec3f Vec3f::operator-() const {
	return Vec3f(-v[0], -v[1], -v[2]);
}
const Vec3f &Vec3f::operator*=(float scale) {
	v[0] *= scale;
	v[1] *= scale;
	v[2] *= scale;
	return *this;
}
const Vec3f &Vec3f::operator/=(float scale) {
	v[0] /= scale;
	v[1] /= scale;
	v[2] /= scale;
	return *this;
}
const Vec3f &Vec3f::operator+=(const Vec3f &other) {
	v[0] += other.v[0];
	v[1] += other.v[1];
	v[2] += other.v[2];
	return *this;
}
const Vec3f &Vec3f::operator-=(const Vec3f &other) {
	v[0] -= other.v[0];
	v[1] -= other.v[1];
	v[2] -= other.v[2];
	return *this;
}
float Vec3f::magnitude() const {
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
float Vec3f::magnitudeSquared() const {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}
Vec3f Vec3f::normalize() const {
	float m = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	return Vec3f(v[0] / m, v[1] / m, v[2] / m);
}
float Vec3f::dot(const Vec3f &other) const {
	return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
}
Vec3f Vec3f::cross(const Vec3f &other) const {
	return Vec3f(v[1] * other.v[2] - v[2] * other.v[1],
		v[2] * other.v[0] - v[0] * other.v[2],
		v[0] * other.v[1] - v[1] * other.v[0]);
}
ostream &operator<<(ostream &output, const Vec3f &v) {
	cout << '(' << v[0] << ", " << v[1] << ", " << v[2] << ')';
	return output;
}
int main()
{
	Vec3f v1(2, 3, -1);
	Vec3f v2(2, 5, 6);
	cout << "v1 = " << v1 << endl;
	cout << "v2 = " << v2 << endl;
	cout << "v1 * 2 = " << v1 * 2 << " and v1 / 2 = " << v1 / 2 << endl;
	cout << "v1 + v2 = " << v1 + v2 << endl;
	cout << "v1 - v2 = " << v1 - v2 << endl;
	cout << "-v1 = " << -v1 << endl;
	cout << "|v1| = " << v1.magnitude() << endl;
	cout << "Normalized v1 = " << v1.normalize() << endl;
	cout << "v1.v2 = " << v1.dot(v2) << endl;
	cout << "v1xv2 = " << v1.cross(v2) << endl;
}