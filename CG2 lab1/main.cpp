#include<stdio.h>
#include<iostream>
#include <vector>
#include"Model.h"
#include"Vec3.h"
#include"PersM.h"
#include"ViewM.h"
#include"Matrix.h"
#include"scan_depthbuffer.h"
//#include"scan.h"
//================================
// global variables
//================================
// screen size
int g_screenWidth = 0;
int g_screenHeight = 0;

// frame index
int g_frameIndex = 0;

// model
Model g_model;
//camera
ViewM V;
//perspective
PersM P;

//init camera and projection parameters
vec3 cameraP(1.0, 1.0,-2.0); //camera position
vec3 center(0.0, 0.0, 0.0); //look center
vec3 upV(0.0, 1.0, 0.0); //up vector
float h = 1.0, d = 1.0, f = 100.0; //project plane, near plane, far plane

//back face culling flag
bool back_face = 1;

void back_face_culling() {
	for (unsigned int i = 0; i < g_model.numFaces; i++) {
		vec3 edge1 = g_model.verts[g_model.faces[i].indices[1]-1].operator-(g_model.verts[g_model.faces[i].indices[0]-1]);
		vec3 edge2 = g_model.verts[g_model.faces[i].indices[2]-1].operator-(g_model.verts[g_model.faces[i].indices[1]-1]);	
		vec3 face_normal = edge1.corssProduct(edge2);
		face_normal.normalize();
		vec3 sight = V.C.operator-(g_model.verts[g_model.faces[i].indices[0]-1]);

		if (face_normal.dot(sight) > 0) {
			g_model.backface[i] = 0;
		}
		else {
			g_model.backface[i] = 1;
		}
	}
}
//================================
// init
//================================
void init(void) {
	// init something before main loop...

	// load model
	g_model.LoadModel("data/car.d");
	
	//set camera
	V.init(cameraP, center, upV);
	//set perspective projection
	P.init(h, d, f);

	//calculate PV matrix
	V.cal_viewM();
	P.cal_persM();

	std::vector< matrix_4x1 > view;
	view.resize(g_model.numVerts);

	//view transform
	for (unsigned int i = 0; i < g_model.numVerts; i++) {
		view[i].setM(g_model.verts[i].x, g_model.verts[i].y, g_model.verts[i].z, 1.0);
		view[i]=view[i].dot_M4x4(V.viewM);
		g_model.verts[i].x = view[i].M[0] / view[i].M[3];
		g_model.verts[i].y = view[i].M[1] / view[i].M[3];
		g_model.verts[i].z = view[i].M[2] / view[i].M[3];
	}

	//back face culling

	if (back_face == 1) {
		back_face_culling();
	}

	//perspective transform
	for (unsigned int i = 0; i < g_model.numVerts; i++) {
		view[i]=view[i].dot_M4x4(P.persM);
		g_model.verts[i].x = view[i].M[0] / view[i].M[3];
		g_model.verts[i].y = view[i].M[1] / view[i].M[3];
		g_model.verts[i].z = view[i].M[2] / view[i].M[3];
	}

	//scan convert
//	scan_convert(g_model);

}


//================================
// update
//================================
void update(void) {
	// do something before rendering...
}

//================================
// render
//================================

void render(void) {
	// clear color and depth buffer
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw model
//	glLineWidth(1);
//	glColor4f(1.0, 1.0, 1.0, 1.0);
//	g_model.DrawEdges2D();
	PolyScan(g_model);
	// swap back and front buffers
	glutSwapBuffers();
}

//================================
// keyboard input
//================================
void key_press(unsigned char key, int x, int y) {

}

void special_key(int key, int x, int y) {

}

//================================
// reshape : update viewport and projection matrix when the window is resized
//================================
void reshape(int w, int h) {
	// screen size
	g_screenWidth = w;
	g_screenHeight = h;
	// viewport
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

}


//================================
// timer : triggered every 16ms ( about 60 frames per second )
//================================
void timer(int value) {
	// increase frame index
	g_frameIndex++;

	update();

	// render
	glutPostRedisplay();

	// reset timer
	glutTimerFunc(16, timer, 0);
}

//================================
// main
//================================
int main(int argc, char** argv) {
	// create opengL window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

	// init
	init();

	// set callback functions
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key_press);
	glutSpecialFunc(special_key);
//	glutTimerFunc(16, timer, 0);

	// main loop
	glutMainLoop();

	return 0;
}