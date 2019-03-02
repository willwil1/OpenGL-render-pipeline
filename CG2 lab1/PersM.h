#pragma once
//perspective matrix
#include"Matrix.h"
class PersM {
public:
	matrix_4x4 persM;
	float H;//projection plane
	float D;//near clipping plane
	float F;//far clipping plane

	void init(float h, float d, float f) {
		this->H = h;
		this->D = d;
		this->F = f;
	}
	void cal_persM(){
		this->persM.setM(D/H,0.0,0.0,0.0,0.0,D/H,0.0,0.0,0.0,0.0,F/(F-D),1.0,0.0,0.0,-D*F/(F-D),0.0);
		//this->persM.setM(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0/D, 0.0, 0.0, 0.0, 0.0);
	}
};