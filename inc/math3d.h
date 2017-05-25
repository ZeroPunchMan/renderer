#pragma once

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

	MyVector2 operator+(MyVector2 other) {
		return MyVector2(this->x + other.x,
			this->y + other.y);
	}

	MyVector2 operator+=(MyVector2 other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	MyVector2 operator-(MyVector2 other) {
		return MyVector2(this->x - other.x,
			this->y - other.y);
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

	MyVector3 operator+(MyVector3 other) {
		return MyVector3(this->x + other.x,
			this->y + other.y,
			this->z + other.z);
	}

	MyVector3 operator+=(MyVector3 other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}

	MyVector3 operator-=(MyVector3 other) {
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

	MyColor operator+(MyColor rightColor) {
		return MyColor(this->r + rightColor.r,
			this->g + rightColor.g,
			this->b + rightColor.b,
			this->a + rightColor.a);
	}

	MyColor operator-(MyColor rightColor) {
		return MyColor(this->r - rightColor.r,
			this->g - rightColor.g,
			this->b - rightColor.b,
			this->a - rightColor.a);
	}

	MyColor operator+=(MyColor rightColor) {
		this->r += rightColor.r;
		this->g += rightColor.g;
		this->b += rightColor.b;
		this->a += rightColor.a;
		return *this;
	}

	MyColor operator*=(MyColor rightColor) {
		this->r *= rightColor.r;
		this->g *= rightColor.g;
		this->b *= rightColor.b;
		this->a *= rightColor.a;
		return *this;
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

	HomoPoint3(MyVector3 v) {
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

	HomoPoint3 operator+(HomoPoint3 other) {
		return HomoPoint3(this->pos.x + other.pos.x,
			this->pos.y + other.pos.y,
			this->pos.z + other.pos.z,
			this->w + other.w);
	}

	HomoPoint3 operator-(MyVector3 other) {
		return HomoPoint3(this->pos.x - other.x,
			this->pos.y - other.y,
			this->pos.z - other.z,
			this->w);
	}

	HomoPoint3 operator+=(MyVector3 vec) {
		this->pos += vec;
		return *this;
	}

	HomoPoint3 operator-=(MyVector3 vec) {
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

//struct MyVector4
//{
//	double x, y, z, w;
//
//	MyVector4(){}
//
//	MyVector4(double x, double y, double z, double w) {
//		this->x = x;
//		this->y = y;
//		this->z = z;
//		this->w = w;
//	}
//
//	MyVector4 operator*(double a) {
//		return MyVector4(this->x * a,
//			this->y * a,
//			this->z * a,
//			this->w * a);
//	}
//
//	MyVector4 operator+(MyVector4 rightV4) {
//		return MyVector4(this->x + rightV4.x,
//			this->y + rightV4.y,
//			this->z + rightV4.z,
//			this->w + rightV4.w);
//	}
//
//	wstring ToString() {
//		TCHAR buff[512];
//		swprintf(buff, _T("{%.2f, %.2f, %.2f, %.2f}\r\n"),
//			x, y, z, w);
//		return wstring(buff);
//	}
//
//};