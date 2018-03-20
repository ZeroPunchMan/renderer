#pragma once

namespace MyMath {
	#define PI (3.1415926f)

	inline bool doubleEqual(double a, double b) {
		if (abs(a - b) < 0.0001f)
			return true;

		return false;
	}

	struct Vector2
	{
		double x, y;
		Vector2() {
			this->x = 0;
			this->y = 0;
		}
		Vector2(double x, double y) {
			this->x = x;
			this->y = y;
		}

		Vector2 operator*(double a) {
			return Vector2(this->x * a,
				this->y * a);
		}
		Vector2 operator/(double a) {
			return Vector2(this->x / a,
				this->y / a);
		}

		Vector2 operator+(Vector2 other) {
			return Vector2(this->x + other.x,
				this->y + other.y);
		}

		Vector2 operator+=(Vector2 other) {
			this->x += other.x;
			this->y += other.y;
			return *this;
		}

		Vector2 operator-(Vector2 other) {
			return Vector2(this->x - other.x,
				this->y - other.y);
		}

		static void Interpolate(Vector2 *v0, Vector2 *v1, double p, double q, Vector2 *out) {
			//p*v0 + (1-p)*v1;
			out->x = v0->x * p + v1->x * q;
			out->y = v0->y* p + v1->y * q;
		}
	};

	struct Vector3
	{
		double x, y, z;
		Vector3() {
			x = y = z = 0;
		}

		Vector3(double x, double y, double z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		static Vector3 Cross(Vector3 a, Vector3 b) {
			Vector3 res;
			res.x = a.y * b.z - a.z * b.y;
			res.y = a.z * b.x - a.x * b.z;
			res.z = a.x * b.y - a.y * b.x;
			return res;
		}

		static double Dot(Vector3 a, Vector3 b) {
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		Vector3 operator*(double a) {
			return Vector3(this->x * a,
				this->y * a,
				this->z * a);
		}

		Vector3 operator*=(double a) {
			this->x *= a;
			this->y *= a;
			this->z *= a;
			return *this;
		}

		Vector3 operator/=(double a) {
			this->x /= a;
			this->y /= a;
			this->z /= a;
			return *this;
		}

		Vector3 operator+(Vector3 other) {
			return Vector3(this->x + other.x,
				this->y + other.y,
				this->z + other.z);
		}

		Vector3 operator-(Vector3 other) {
			return Vector3(this->x - other.x,
				this->y - other.y,
				this->z - other.z);
		}

		Vector3 operator+=(Vector3 other) {
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;
		}

		Vector3 operator-=(Vector3 other) {
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
				CLog(_T("Vector3.Normalize >> length is 0"));
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

	struct Color {
		double r, g, b, a;

		Color() {}

		Color(double r, double g, double b, double a) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		Color(double r, double g, double b) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = 1.0f;
		}

		Color operator*(double x) {
			return Color(this->r * x,
				this->g * x,
				this->b * x,
				this->a * x);
		}

		Color operator/(double x) {
			return Color(this->r / x,
				this->g / x,
				this->b / x,
				this->a / x);
		}

		Color operator+(Color rightColor) {
			return Color(this->r + rightColor.r,
				this->g + rightColor.g,
				this->b + rightColor.b,
				this->a + rightColor.a);
		}

		Color operator-(Color rightColor) {
			return Color(this->r - rightColor.r,
				this->g - rightColor.g,
				this->b - rightColor.b,
				this->a - rightColor.a);
		}

		Color operator+=(Color rightColor) {
			this->r += rightColor.r;
			this->g += rightColor.g;
			this->b += rightColor.b;
			this->a += rightColor.a;
			return *this;
		}

		Color operator*=(Color rightColor) {
			this->r *= rightColor.r;
			this->g *= rightColor.g;
			this->b *= rightColor.b;
			this->a *= rightColor.a;
			return *this;
		}

		static void Interpolate(Color *c0, Color *c1, double p, double q, Color *out) {
			//p*c0 + q*c1
			out->r = c0->r * p + c1->r * q;
			out->g = c0->g * p + c1->g * q;
			out->b = c0->b * p + c1->b * q;
			out->a = c0->a * p + c1->a * q;
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

	struct HomoVector4
	{
		Vector3 pos;
		double w;

		HomoVector4() {}
		HomoVector4(double x, double y, double z, double w) {
			pos.x = x;
			pos.y = y;
			pos.z = z;
			this->w = w;
		}

		HomoVector4(double x, double y, double z) {
			pos.x = x;
			pos.y = y;
			pos.z = z;
			this->w = 1;
		}

		HomoVector4(Vector3 v) {
			pos = v;
			w = 1;
		}

		void Homogenize() {
			if (w == 0) {
				CLog(_T("HomoVector3.Homogenize >> w is 0"));
				return;
			}
			pos.x /= w;
			pos.y /= w;
			pos.z /= w;
			w = 1.0f;
		}

		HomoVector4 operator*(double a) {
			return HomoVector4(pos.x * a,
				pos.y * a,
				pos.z * a,
				this->w * a);
		}

		HomoVector4 operator+(HomoVector4 other) {
			return HomoVector4(this->pos.x + other.pos.x,
				this->pos.y + other.pos.y,
				this->pos.z + other.pos.z,
				this->w + other.w);
		}

		HomoVector4 operator+(Vector3 other) {
			return HomoVector4(this->pos.x + other.x,
				this->pos.y + other.y,
				this->pos.z + other.z,
				this->w);
		}

		HomoVector4 operator-(Vector3 other) {
			return HomoVector4(this->pos.x - other.x,
				this->pos.y - other.y,
				this->pos.z - other.z,
				this->w);
		}

		HomoVector4 operator+=(Vector3 vec) {
			this->pos.x += vec.x;
			this->pos.y += vec.y;
			this->pos.z += vec.z;
			return *this;
		}

		HomoVector4 operator-=(Vector3 vec) {
			this->pos -= vec;
			return *this;
		}

		operator Vector3() {
			return this->pos;
		}

		string ToString() {
			TCHAR buff[128];
			sprintf(buff, _T("{%.2f, %.2f, %.2f, %.2f}"),
				pos.x, pos.y, pos.z, w);
			return string(buff);
		}
	};
}