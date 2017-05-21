#pragma once
#include "math3d.h"

class MyMat4 {
public:
	float data[4][4] = {1,0,0,0,
						0,1,0,0,
						0,0,1,0,
						0,0,0,1};

	MyMat4(){}
	MyMat4(float d[4][4]) {
		for (int i = 0; i < 4; i++) {
			for (int k = 0; k < 4; k++)
				data[i][k] = d[i][k];
		}
	}

	///
	MyMat4 operator+(MyMat4 other) {
		MyMat4 newMat;
		for (int i = 0; i < 4; i++) {
			for (int k = 0; k < 4; k++)
				newMat.data[i][k] = data[i][k] + other.data[i][k];
		}
		return newMat;
	}

	MyMat4 operator+=(MyMat4 other) {
		for (int i = 0; i < 4; i++) {
			for (int k = 0; k < 4; k++)
				this->data[i][k] += other.data[i][k];
		}
		return *this;
	}

	MyMat4 operator-(MyMat4 other) {
		MyMat4 newMat;
		for (int i = 0; i < 4; i++) {
			for (int k = 0; k < 4; k++)
				newMat.data[i][k] = data[i][k] - other.data[i][k];
		}
		return newMat;
	}

	MyMat4 operator-=(MyMat4 other) {
		for (int i = 0; i < 4; i++) {
			for (int k = 0; k < 4; k++)
				this->data[i][k] -= other.data[i][k];
		}
		return *this;
	}

	MyMat4 operator*(MyMat4 other) {
		MyMat4 newMat;
		for (int i = 0; i < 4; i++) {
			for (int k = 0; k < 4; k++) {
				newMat.data[i][k] = data[i][0] * other.data[0][k] + 
									data[i][1] * other.data[1][k] + 
									data[i][2] * other.data[2][k] + 
									data[i][3] * other.data[3][k];
			}
		}
		return newMat;
	}

	HomoPoint3 operator*(HomoPoint3 vec) {
		HomoPoint3 res;
		res.pos.x = data[0][0] * vec.pos.x + data[0][1] * vec.pos.y + data[0][2] * vec.pos.z + data[0][3] * vec.w;
		res.pos.y = data[1][0] * vec.pos.x + data[1][1] * vec.pos.y + data[1][2] * vec.pos.z + data[1][3] * vec.w;
		res.pos.z = data[2][0] * vec.pos.x + data[2][1] * vec.pos.y + data[2][2] * vec.pos.z + data[2][3] * vec.w;
		res.w = data[3][0] * vec.pos.x + data[3][1] * vec.pos.y + data[3][2] * vec.pos.z + data[3][3] * vec.w;
		return res;
	}

	wstring ToString() {
		TCHAR buff[512];
		swprintf(buff, _T("{%.2f, %.2f, %.2f, %.2f}\r\n{%.2f, %.2f, %.2f, %.2f}\r\n{%.2f, %.2f, %.2f, %.2f}\r\n{%.2f, %.2f, %.2f, %.2f}\r\n"),
			data[0][0], data[0][1], data[0][2], data[0][3], 
			data[1][0], data[1][1], data[1][2], data[1][3], 
			data[2][0], data[2][1], data[2][2], data[2][3], 
			data[3][0], data[3][1], data[3][2], data[3][3]);
		return wstring(buff);
	}

	MyMat4 GetTranspose() {
		MyMat4 trans;
		for (int i = 0; i < 4; i++) {
			for (int k = 0; k < 4; k++) {
				trans.data[i][k] = this->data[k][i];
			}
		}
		return trans;
	}

	void Transpose() {
		for (int i = 0; i < 4; i++) {
			for (int k = i + 1; k < 4; k++) {
				data[i][k] = data[k][i];
			}
		}
	}
};
