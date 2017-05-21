#pragma once

#define PI (3.1415926f)

struct MyVector2
{
	float x, y;

	MyVector2(float x, float y) {
		this->x = x;
		this->y = y;
	}
};

struct MyVector2Int {
	int x, y;
	MyVector2Int(){}
	MyVector2Int(int x, int y) {
		this->x = x;
		this->y = y;
	}
};


struct MyVector3
{
	float x, y, z;
	MyVector3(){
		x = y = z = 0;
	}

	MyVector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	MyVector3 operator*=(float a) {
		this->x *= a;
		this->y *= a;
		this->z *= a;
		return *this;
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

	float Length() {
		return sqrtf(x*x + y*y + z*z);
	}

	void Normalize() {
		float len = this->Length();
		if (len == 0) {
			MyLog(_T("MyVector3.Normalize >> length is 0"));
			return;
		}
		this->x /= len;
		this->y /= len;
		this->z /= len;
	}

	wstring ToString() {
		TCHAR buff[128];
		swprintf(buff, _T("{%.2f, %.2f, %.2f}"),
			x, y, z);
		return wstring(buff);
	}
};

struct MyColor {
	float r, g, b, a;

	MyColor(){}

	MyColor(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	MyColor(float r, float g, float b) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 1.0f;
	}

	MyColor operator*(float x) {
		return MyColor(this->r * x, 
						this->g * x, 
						this->b * x, 
						this->a * x);
	}

	MyColor operator/(float x) {
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

	//rate为靠近c2的程度
	static MyColor Interpolate(MyColor* c1, MyColor* c2, float rate) {
		MyColor newColor = *c1 * (1 - rate) + *c2 * rate;
		return newColor;
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
	float w;

	HomoPoint3(){}
	HomoPoint3(float x, float y, float z, float w) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
		this->w = w;
	}

	HomoPoint3(float x, float y, float z) {
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

	HomoPoint3 operator*(float a) {
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

	wstring ToString() {
		TCHAR buff[128];
		swprintf(buff, _T("{%.2f, %.2f, %.2f, %.2f}"),
			pos.x, pos.y, pos.z, w);
		return wstring(buff);
	}
};

//struct MyVector4
//{
//	float x, y, z, w;
//
//	MyVector4(){}
//
//	MyVector4(float x, float y, float z, float w) {
//		this->x = x;
//		this->y = y;
//		this->z = z;
//		this->w = w;
//	}
//
//	MyVector4 operator*(float a) {
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