#pragma once

#include "math.h"
#include "mylog.h"

inline bool doubleEqual(double a, double b) {
	if (abs(a - b) < 0.0001f)
		return true;

	return false;
}

#define PI (3.1415926f)

struct MyVector2
{
	double x, y;
	MyVector2() {
		this->x = 0;
		this->y = 0;
	}
	MyVector2(double x, double y) {
		this->x = x;
		this->y = y;
	}

	MyVector2 operator*(double a) {
		return MyVector2(this->x * a,
			this->y * a);
	}
	MyVector2 operator/(double a) {
		return MyVector2(this->x / a,
			this->y / a);
	}

	MyVector2 operator+(const MyVector2& other) {
		return MyVector2(this->x + other.x,
			this->y + other.y);
	}

	MyVector2 operator+=(const MyVector2& other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	MyVector2 operator-(const MyVector2& other) {
		return MyVector2(this->x - other.x,
			this->y - other.y);
	}

	static void Interpolate(const MyVector2& v0, const MyVector2& v1, double p, double q, MyVector2 *out) {
		//p*v0 + (1-p)*v1;
		out->x = v0.x * p + v1.x * q;
		out->y = v0.y* p + v1.y * q;
	}
};

struct MyVector2Int {
	int x, y;
	MyVector2Int() {}
	MyVector2Int(int x, int y) {
		this->x = x;
		this->y = y;
	}
};


struct MyVector3
{
	double x, y, z;
	MyVector3() {
		x = y = z = 0;
	}

	MyVector3(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	static MyVector3 Cross(const MyVector3& a, const MyVector3& b) {
		MyVector3 res;
		res.x = a.y * b.z - a.z * b.y;
		res.y = a.z * b.x - a.x * b.z;
		res.z = a.x * b.y - a.y * b.x;
		return res;
	}

	static double Dot(const MyVector3& a, const MyVector3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	MyVector3 operator*(double a) {
		return MyVector3(this->x * a, 
			this->y * a,
			this->z * a);
	}

	MyVector3 operator*=(double a) {
		this->x *= a;
		this->y *= a;
		this->z *= a;
		return *this;
	}

	MyVector3 operator/=(double a) {
		this->x /= a;
		this->y /= a;
		this->z /= a;
		return *this;
	}

	MyVector3 operator+(const MyVector3& other) {
		return MyVector3(this->x + other.x,
			this->y + other.y,
			this->z + other.z);
	}

	MyVector3 operator-(const MyVector3& other) {
		return MyVector3(this->x - other.x,
			this->y - other.y,
			this->z - other.z);
	}

	MyVector3 operator+=(const MyVector3& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}

	MyVector3 operator-=(const MyVector3& other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}

	double Length() {
		return sqrtf(x*x + y*y + z*z);
	}

	void Normalize() {
		double len = this->Length();
		if (len == 0) {
			MyLog(_T("MyVector3.Normalize >> length is 0"));
			return;
		}
		this->x /= len;
		this->y /= len;
		this->z /= len;
	}

	string ToString() {
		TCHAR buff[128];
		sprintf(buff, _T("{%.2f, %.2f, %.2f}"),
			x, y, z);
		return string(buff);
	}
};

struct MyColor {
	double r, g, b, a;

	MyColor() {}

	MyColor(double r, double g, double b, double a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	MyColor(double r, double g, double b) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 1.0f;
	}

	MyColor operator*(double x) {
		return MyColor(this->r * x,
			this->g * x,
			this->b * x,
			this->a * x);
	}

	MyColor operator/(double x) {
		return MyColor(this->r / x,
			this->g / x,
			this->b / x,
			this->a / x);
	}

	MyColor operator+(const MyColor& rightColor) {
		return MyColor(this->r + rightColor.r,
			this->g + rightColor.g,
			this->b + rightColor.b,
			this->a + rightColor.a);
	}

	MyColor operator-(const MyColor& rightColor) {
		return MyColor(this->r - rightColor.r,
			this->g - rightColor.g,
			this->b - rightColor.b,
			this->a - rightColor.a);
	}

	MyColor operator+=(const MyColor& rightColor) {
		this->r += rightColor.r;
		this->g += rightColor.g;
		this->b += rightColor.b;
		this->a += rightColor.a;
		return *this;
	}

	MyColor operator*=(const MyColor& rightColor) {
		this->r *= rightColor.r;
		this->g *= rightColor.g;
		this->b *= rightColor.b;
		this->a *= rightColor.a;
		return *this;
	}

	static void Interpolate(const MyColor& c0, const MyColor& c1, double p, double q, MyColor *out) {
		//p*c0 + q*c1
		out->r = c0.r * p + c1.r * q;
		out->g = c0.g * p + c1.g * q;
		out->b = c0.b * p + c1.b * q;
		out->a = c0.a * p + c1.a * q;
	}

private:
	void correct() {
		if (this->r > 1.0f)
			this->r = 1.0f;
		if (this->g > 1.0f)
			this->g = 1.0f;
		if (this->b > 1.0f)
			this->b = 1.0f;
		if (this->a > 1.0f)
			this->a = 1.0f;

		if (this->r < 0)
			this->r = 0;
		if (this->g < 0)
			this->g = 0;
		if (this->b < 0)
			this->b = 0;
		if (this->a < 0)
			this->a = 0;
	}

};

struct HomoPoint3
{
	MyVector3 pos;
	double w;

	HomoPoint3() {}
	HomoPoint3(double x, double y, double z, double w) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
		this->w = w;
	}

	HomoPoint3(double x, double y, double z) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
		this->w = 1;
	}

	HomoPoint3(const MyVector3& v) {
		pos = v;
		w = 1;
	}

	void Homogenize() {
		if (w == 0) {
			MyLog(_T("HomoVector3.Homogenize >> w is 0"));
			return;
		}
		pos.x /= w;
		pos.y /= w;
		pos.z /= w;
		w = 1.0f;
	}

	HomoPoint3 operator*(double a) {
		return HomoPoint3(pos.x * a,
			pos.y * a,
			pos.z * a,
			this->w * a);
	}

	HomoPoint3 operator+(const HomoPoint3& other) {
		return HomoPoint3(this->pos.x + other.pos.x,
			this->pos.y + other.pos.y,
			this->pos.z + other.pos.z,
			this->w + other.w);
	}

	HomoPoint3 operator+(const MyVector3& other) {
		return HomoPoint3(this->pos.x + other.x,
			this->pos.y + other.y,
			this->pos.z + other.z,
			this->w);
	}

	HomoPoint3 operator-(const MyVector3& other) {
		return HomoPoint3(this->pos.x - other.x,
			this->pos.y - other.y,
			this->pos.z - other.z,
			this->w);
	}

	HomoPoint3 operator+=(const MyVector3& vec) {
		this->pos.x += vec.x;
		this->pos.y += vec.y;
		this->pos.z += vec.z;
		return *this;
	}

	HomoPoint3 operator-=(const MyVector3& vec) {
		this->pos -= vec;
		return *this;
	}

	operator MyVector3() {
		return this->pos;
	}

	string ToString() {
		TCHAR buff[128];
		sprintf(buff, _T("{%.2f, %.2f, %.2f, %.2f}"),
			pos.x, pos.y, pos.z, w);
		return string(buff);
	}
};
