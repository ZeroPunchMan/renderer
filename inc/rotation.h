#pragma once
#include "matrix.h"


class Rotation {

private:
	MyMath::MyMat4 mat;



public:
	Rotation() {}
	Rotation(MyMath::Vector3 axis, double angle) {
		Quaternion(axis, angle);
	}

	Rotation(double x, double y, double z) {
		EulerAngles(x, y, z);
	}

	//ÉèÖÃÐý×ª¾ØÕó
	void Quaternion(MyMath::Vector3 axis, double angle) {
		axis.Normalize();
		double sinHalfAngle = sin(angle * PI / 360);
		double qw = cos(angle * PI / 360);
		double qx = sinHalfAngle * axis.x;
		double qy = sinHalfAngle * axis.y;
		double qz = sinHalfAngle * axis.z;
		mat.data[0][0] = 1 - 2 * (qy*qy + qz*qz);
		mat.data[0][1] = 2 * (qx*qy - qw*qz);
		mat.data[0][2] = 2 * (qx*qz + qw*qy);
		mat.data[0][3] = 0;

		mat.data[1][0] = 2 * (qx*qy + qw*qz);
		mat.data[1][1] = 1 - 2 * (qx*qx + qz*qz);
		mat.data[1][2] = 2 * (qy*qz - qw*qx);
		mat.data[1][3] = 0;

		mat.data[2][0] = 2 * (qx*qz - qw*qy);
		mat.data[2][1] = 2 * (qy*qz + qw*qx);
		mat.data[2][2] = 1 - 2 * (qx*qx + qy*qy);
		mat.data[2][3] = 0;

		mat.data[3][0] = 0;
		mat.data[3][1] = 0;
		mat.data[3][2] = 0;
		mat.data[3][3] = 1;
	}

	//ÓÃÅ·À­½ÇÉèÖÃÐý×ª¾ØÕó
	void EulerAngles(double x, double y, double z) {
		double sinx = sin(x * PI / 180);
		double cosx = cos(x * PI / 180);
		double siny = sin(y * PI / 180);
		double cosy = cos(y * PI / 180);
		double sinz = sin(z * PI / 180);
		double cosz = cos(z * PI / 180);

		this->mat.data[0][0] = cosy * cosz + sinx * siny * sinz;
		this->mat.data[0][1] = -cosy * sinz + sinx * siny * cosz;
		this->mat.data[0][2] = cosx * siny;
		this->mat.data[0][3] = 0;

		this->mat.data[1][0] = cosx * sinz;
		this->mat.data[1][1] = cosx * cosz;
		this->mat.data[1][2] = -sinx;
		this->mat.data[1][3] = 0;

		this->mat.data[2][0] = -siny * cosz + sinx * cosy * sinz;
		this->mat.data[2][1] = siny * sinz + sinx * cosy * cosz;
		this->mat.data[2][2] = cosx * cosy;
		this->mat.data[2][3] = 0;

		this->mat.data[3][0] = 0;
		this->mat.data[3][1] = 0;
		this->mat.data[3][2] = 0;
		this->mat.data[3][3] = 1;

	}

	void EulerAngles(MyMath::Vector3 angles) {
		this->EulerAngles(angles.x, angles.y, angles.z);
	}

	MyMath::HomoVector4 Rotate(MyMath::HomoVector4 point) {
		MyMath::HomoVector4 newPoint;
		newPoint = this->mat * point;
		return newPoint;
	}

	Rotation operator*(Rotation other) {
		Rotation newRot;
		newRot.mat = this->mat * other.mat;
		return newRot;
	}

	Rotation operator*=(Rotation other) {
		this->mat = this->mat * other.mat;
		return *this;
	}

	MyMath::HomoVector4 operator*(MyMath::HomoVector4 point) {
		return this->Rotate(point);
	}

	Rotation GetInverse() {
		Rotation inv;
		inv.mat = this->mat.GetTranspose();
		return inv;
	}

	void Inverse() {
		this->mat.Transpose();
	}

	string ToString() {
		return this->mat.ToString();
	}
};