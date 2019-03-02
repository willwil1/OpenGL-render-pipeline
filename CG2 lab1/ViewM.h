#pragma once
//view transform
#include "Vec3.h"
#include "Matrix.h"
class ViewM {
public:
	vec3 C; //camera position
	vec3 P; //object center
	vec3 V1; //up vector
	matrix_4x4 viewM;

	void init(vec3 c,vec3 p,vec3 v){
		this->C = c;
		this->P = p;
		this->V1 = v;
	}

	void cal_viewM() {
		vec3 N=P.operator-(C);
		N = N.normalize();
		vec3 U = N.corssProduct(V1);
		U = U.normalize();
		vec3 V = U.corssProduct(N);
		matrix_4x4 R,T;
		R.setM(U.x, V.x, N.x, 0.0, U.y, V.y, N.y, 0.0, U.z, V.z, N.z, 0.0, 0.0, 0.0, 0.0, 1.0);
		T.setM(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -C.x, -C.y, -C.z, 1.0);//negtive?!!!!!
		this->viewM = R.dot(T);
	}
};