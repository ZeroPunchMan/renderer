#pragma once

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

	MyVector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

};


struct MyVector4
{
	float x, y, z, w;

	MyVector4(){}

	MyVector4(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	MyVector4 operator*(float a) {
		return MyVector4(this->x * a,
			this->y * a,
			this->z * a,
			this->w * a);
	}

	MyVector4 operator+(MyVector4 rightV4) {
		return MyVector4(this->x + rightV4.x,
			this->y + rightV4.y,
			this->z + rightV4.z,
			this->w + rightV4.w);
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

	MyColor operator+(MyColor rightColor) {
		return MyColor(this->r + rightColor.r,
						this->g + rightColor.g,
						this->b + rightColor.b,
						this->a + rightColor.a);
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
