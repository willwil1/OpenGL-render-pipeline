#pragma once


class matrix_4x4{
public:
	float M[16];

	void setM(float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, float z1, float z2, float z3, float z4, float w1, float w2, float w3, float w4) {
		this->M[0] = x1;
		this-> M[1] = x2;
		this->M[2] = x3;
		this->M[3] = x4;
		this->M[4] = y1;
		this->M[5] = y2;
		this->M[6] = y3;
		this->M[7] = y4;
		this->M[8] = z1;
		this->M[9] = z2;
		this->M[10] = z3;
		this->M[11] = z4;
		this->M[12] = w1;
		this->M[13] = w2;
		this->M[14] = w3;
		this->M[15] = w4;
	}

	matrix_4x4 dot(matrix_4x4 m) {
		matrix_4x4 result;
		result.M[0] = this->M[0] * m.M[0] + this->M[4] * m.M[1] + this->M[8] * m.M[2] + this->M[12] * m.M[3];
		result.M[1] = this->M[1] * m.M[0] + this->M[5] * m.M[1] + this->M[9] * m.M[2] + this->M[13] * m.M[3];
		result.M[2] = this->M[2] * m.M[0] + this->M[6] * m.M[1] + this->M[10] * m.M[2] + this->M[14] * m.M[3];
		result.M[3] = this->M[3] * m.M[0] + this->M[7] * m.M[1] + this->M[11] * m.M[2] + this->M[15] * m.M[3];
		result.M[4] = this->M[0] * m.M[4] + this->M[4] * m.M[5] + this->M[8] * m.M[6] + this->M[12] * m.M[7];
		result.M[5] = this->M[1] * m.M[4] + this->M[5] * m.M[5] + this->M[9] * m.M[6] + this->M[13] * m.M[7];
		result.M[6] = this->M[2] * m.M[4] + this->M[6] * m.M[5] + this->M[10] * m.M[6] + this->M[14] * m.M[7];
		result.M[7] = this->M[3] * m.M[4] + this->M[7] * m.M[5] + this->M[11] * m.M[6] + this->M[15] * m.M[7];
		result.M[8] = this->M[0] * m.M[8] + this->M[4] * m.M[9] + this->M[8] * m.M[10] + this->M[12] * m.M[11];
		result.M[9] = this->M[1] * m.M[8] + this->M[5] * m.M[9] + this->M[9] * m.M[10] + this->M[13] * m.M[11];
		result.M[10] = this->M[2] * m.M[8] + this->M[6] * m.M[9] + this->M[10] * m.M[10] + this->M[14] * m.M[11];
		result.M[11] = this->M[3] * m.M[8] + this->M[7] * m.M[9] + this->M[11] * m.M[10] + this->M[15] * m.M[11];
		result.M[12] = this->M[0] * m.M[12] + this->M[4] * m.M[13] + this->M[8] * m.M[14] + this->M[12] * m.M[15];
		result.M[13] = this->M[1] * m.M[12] + this->M[5] * m.M[13] + this->M[9] * m.M[14] + this->M[13] * m.M[15];
		result.M[14] = this->M[2] * m.M[12] + this->M[6] * m.M[13] + this->M[10] * m.M[14] + this->M[14] * m.M[15];
		result.M[15] = this->M[3] * m.M[12] + this->M[7] * m.M[13] + this->M[11] * m.M[14] + this->M[15] * m.M[15];
		return result;
	}

};

class matrix_4x1 {
public:
	float M[4];

	void setM(float x1, float x2, float x3, float x4) {
		this->M[0] = x1;
		this->M[1] = x2;
		this->M[2] = x3;
		this->M[3] = x4;
	}
	matrix_4x1 dot_M4x4(matrix_4x4 m) {
		matrix_4x1 result;
		result.M[0] = m.M[0] * this->M[0] + m.M[4] * this->M[1] + m.M[8] * this->M[2] + m.M[12] * this->M[3];
		result.M[1] = m.M[1] * this->M[0] + m.M[5] * this->M[1] + m.M[9] * this->M[2] + m.M[13] * this->M[3];
		result.M[2] = m.M[2] * this->M[0] + m.M[6] * this->M[1] + m.M[10] * this->M[2] + m.M[14] * this->M[3];
		result.M[3] = m.M[3] * this->M[0] + m.M[7] * this->M[1] + m.M[11] * this->M[2] + m.M[15] * this->M[3];
		return result;
	}
};