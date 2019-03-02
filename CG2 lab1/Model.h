#pragma once
//Read file and draw

#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include "Vec3.h"
#include <gl\glut.h>

#define PI 3.1415926

//================================
// ModelFace
//================================
class ModelFace {
public:
	std::vector< int > indices;

public:
	ModelFace() {
	}

	~ModelFace() {
	}
};

//================================
// Model
//================================
class Model {
public:
	std::vector< vec3 > verts;
	std::vector< ModelFace > faces;
	std::vector< bool > backface;
	unsigned int numVerts = 0;
	unsigned int numFaces = 0;

public:
	Model() {
	}

	~Model() {
	}

	//=============================================
	// Load Model
	//=============================================
	void Free(void) {
		verts.clear();
		faces.clear();
		backface.clear();
	}

	bool LoadModel(const char *path) {
		if (!path) {
			return false;
		}

		Free();

		// open file
		FILE *fp = fopen(path, "r");
		if (!fp) {
			return false;
		}

		// num of vertices and indices
		fscanf(fp, "data %d %d", &numVerts, &numFaces);
		// alloc vertex and index buffer
		verts.resize(numVerts);
		faces.resize(numFaces);
		backface.resize(numFaces);
		for (unsigned int i = 0; i < numFaces; i++) {
			backface[i] = 0;
		}

		// read vertices
		for (unsigned int i = 0; i < numVerts; i++) {
			fscanf(fp, "%f%f%f", &verts[i].x, &verts[i].y, &verts[i].z);
		}

		// read indices
		for (unsigned int i = 0; i < numFaces; i++) {
			int numSides = 0;
			fscanf(fp, "%i", &numSides);
			faces[i].indices.resize(numSides);

			for (unsigned int k = 0; k < faces[i].indices.size(); k++) {
				fscanf(fp, "%i", &faces[i].indices[k]);
		//		faces[i].indices[k] = faces[i].indices[k] - 1;
			}
		}


		// close file
		fclose(fp);

		ResizeModel();

		return true;
	}

	//=============================================
	// Render Model
	//=============================================
	void DrawEdges2D(void) {
		glBegin(GL_LINES);
		for (unsigned int i = 0; i < faces.size(); i++) {
			if (backface[i] == 0) {
				for (unsigned int k = 0; k < faces[i].indices.size(); k++) { 
					int p0 = faces[i].indices[k]-1; //indices starts from 1 so minus 1;
					int p1 = faces[i].indices[(k + 1) % faces[i].indices.size()]-1;
					glVertex2fv(verts[p0].ptr());
					glVertex2fv(verts[p1].ptr());
			//		printf("%f,%f,%f\n", verts[p0].x, verts[p0].y, verts[p0].z);
				}
			}	
		}
		glEnd();
	}

	void DrawEdges(void) {
		glShadeModel(GL_SMOOTH);
		glBegin(GL_POLYGON);
		for (unsigned int i = 0; i < faces.size(); i++) {
			glBegin(GL_POLYGON);
			for (unsigned int k = 0; k < faces[i].indices.size(); k++) {
				glVertex3fv(verts[faces[i].indices[k]].ptr());
			}
			glEnd();
		}
	}




	//=============================================
	// Resize Model
	//=============================================
	// scale the model into the range of [ -1.0, 1.0 ]
	void ResizeModel(void) {
		// bound
		vec3 min, max;
		if (!CalcBound(min, max)) {
			return;
		}

		// max side
		vec3 size = max - min;

		float r = size.x;
		if (size.y > r) {
			r = size.y;
		}
		if (size.z > r) {
			r = size.z;
		}

		if (r < 1e-6f) {
			r = 0;
		}
		else {
			r = 1.0f / r;
		}

		// scale
		for (unsigned int i = 0; i < verts.size(); i++) {
			// [0, 1]
			verts[i] = (verts[i] - min) * r;

			// [-1, 1]
			verts[i] = verts[i] * 2.0f - vec3(1.0f, 1.0f, 1.0f);

			// [-0.9, 0.9]
			verts[i] *= 1.0;
		}
	}

	bool CalcBound(vec3 &min, vec3 &max) {
		if (verts.size() <= 0) {
			return false;
		}

		min = verts[0];
		max = verts[0];

		for (unsigned int i = 1; i < verts.size(); i++) {
			vec3 v = verts[i];

			if (v.x < min.x) {
				min.x = v.x;
			}
			else if (v.x > max.x) {
				max.x = v.x;
			}

			if (v.y < min.y) {
				min.y = v.y;
			}
			else if (v.y > max.y) {
				max.y = v.y;
			}

			if (v.z < min.z) {
				min.z = v.z;
			}
			else if (v.z > max.z) {
				max.z = v.z;
			}
		}

		return true;
	}

	bool CalcFaceBound(vec3 &min, vec3 &max,int face) {
		
		min = verts[faces[face].indices[0]];
		max = verts[faces[face].indices[0]];

		for (unsigned int i = 1; i < faces[face].indices.size(); i++) {
			vec3 v = verts[faces[face].indices[i]];

			if (v.x < min.x) {
				min.x = v.x;
			}
			else if (v.x > max.x) {
				max.x = v.x;
			}

			if (v.y < min.y) {
				min.y = v.y;
			}
			else if (v.y > max.y) {
				max.y = v.y;
			}

			if (v.z < min.z) {
				min.z = v.z;
			}
			else if (v.z > max.z) {
				max.z = v.z;
			}
		}

		return true;
	}

};