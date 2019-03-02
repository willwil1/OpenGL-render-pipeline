#pragma once
#include"Model.h"

//structure for ET & AET
typedef struct ET
{
	float x;
	float dx, ymax;
	float ymin_z;
	float dz;

	ET* next;
}AET, NET;


#define width 512
#define height 512
//create refresh buffer to store the closest color 
struct RefBuffer {
	float R[width][height];
	float G[width][height];
	float B[width][height];
};
//create depth buffer to store the closest depth
RefBuffer RBuffer;
float DepBuffer[width][height];



void PolyScan(Model &m) {
	// rescale the vertices based on the window size
	for (unsigned i = 0; i < m.numVerts; i++) {
		m.verts[i].x = width/2.0 + m.verts[i].x*width/2.0;
		m.verts[i].y = height/2.0 + m.verts[i].y*height/2.0;
		m.verts[i].x = ceil(m.verts[i].x);
		m.verts[i].y = ceil(m.verts[i].y);

		//		printf("%f,%f,%f\n", m.verts[i].x, m.verts[i].y, m.verts[i].z);
	}
	//init the refresh buffer & depth buffer
	for (unsigned i = 0; i < width; i++) {
		for (unsigned j = 0; j < height; j++) {
			RBuffer.R[i][j] = 0.0;
			RBuffer.G[i][j] = 0.0;
			RBuffer.B[i][j] = 0.0;
			DepBuffer[i][j] = 1000000;
		}
	}
	/*	for (unsigned j = 0; j < m.faces.size(); j++) {
	for (unsigned k = 0; k < m.faces[j].indices.size(); k++) {
	m.faces[j].indices[k] = m.faces[j].indices[k] - 1;
	}
	}*/

	//calculate the max y axis of the scan line
	int MaxY = 0;
	int i;
	for (i = 0; i < m.verts.size(); i++)
		if (m.verts[i].y > MaxY)
			MaxY = m.verts[i].y;

	// scan for each face
	for (unsigned int n = 0; n < m.faces.size(); n++) {
		if (m.backface[n] == 0) {
			//create new AET
			AET *pAET = new AET;
			pAET->next = NULL;

			//crete new ET
			NET *pNET[1024];
			for (i = 0; i <= MaxY; i++)
			{
				pNET[i] = new NET;
				pNET[i]->next = NULL;
			}

			//init color of each face
			float r = (char)rand() % 256 / 256.0, g = (char)rand() % 256 / 256.0, b = (char)rand() % 256 / 256.0;
			glColor3f(r, g, b);

			//for each scan line
			for (unsigned int i = 0; i <= MaxY; i++)
			{
				for (unsigned int j = 0; j < m.faces[n].indices.size(); j++)
				{
					if (m.verts[m.faces[n].indices[j] - 1].y == i)
					{ 
						// if this vertex is the lower y of the adjancent vertex, store in ET
						if (m.verts[m.faces[n].indices[(j - 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].y > m.verts[m.faces[n].indices[j] - 1].y)
						{
							NET *p = new NET;
							p->x = m.verts[m.faces[n].indices[j] - 1].x;
							p->ymax = m.verts[m.faces[n].indices[(j - 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].y;
							p->dx = (m.verts[m.faces[n].indices[(j - 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].x - m.verts[m.faces[n].indices[j] - 1].x) / (m.verts[m.faces[n].indices[(j - 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].y - m.verts[m.faces[n].indices[j] - 1].y);
							p->ymin_z = m.verts[m.faces[n].indices[j]-1].z;
							p->dz = (m.verts[m.faces[n].indices[(j - 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()]-1].z - m.verts[m.faces[n].indices[j]-1].z) / (m.verts[m.faces[n].indices[(j - 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()]-1].y - m.verts[m.faces[n].indices[j]-1].y);

							p->next = pNET[i]->next;
							pNET[i]->next = p;

						}
						if (m.verts[m.faces[n].indices[(j + 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].y > m.verts[m.faces[n].indices[j] - 1].y)
						{
							NET *p = new NET;
							p->x = m.verts[m.faces[n].indices[j] - 1].x;
							p->ymax = m.verts[m.faces[n].indices[(j + 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].y;
							p->dx = (m.verts[m.faces[n].indices[(j + 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].x - m.verts[m.faces[n].indices[j] - 1].x) / (m.verts[m.faces[n].indices[(j + 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].y - m.verts[m.faces[n].indices[j] - 1].y);
							p->ymin_z = m.verts[m.faces[n].indices[j]-1].z;
							p->dz = (m.verts[m.faces[n].indices[(j + 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()]-1].z - m.verts[m.faces[n].indices[j]-1].z) / (m.verts[m.faces[n].indices[(j + 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] -1].y - m.verts[m.faces[n].indices[j]-1].y);

							p->next = pNET[i]->next;
							pNET[i]->next = p;
						}
					}
				}
			}
			// update AET with scan 
			for (unsigned int i = 0; i <= MaxY; i++)
			{
				// update x & z
				NET *p = pAET->next;
				while (p)
				{
					p->x = p->x + p->dx;
					p->ymin_z = p->ymin_z + p->dz;
					p = p->next;
				}
				// sort AET according to x
				AET *tq = pAET;
				p = pAET->next;
				tq->next = NULL;
				while (p)
				{
					while (tq->next && p->x >= tq->next->x)
						tq = tq->next;
					NET *s = p->next;
					p->next = tq->next;
					tq->next = p;
					p = s;
					tq = pAET;
				}
				//delete from AET where ymax=i 
				AET *q = pAET;
				p = q->next;
				while (p)
				{
					if (p->ymax == i)
					{
						q->next = p->next;
						delete p;
						p = q->next;
					}
					else
					{
						q = q->next;
						p = q->next;
					}
				}
				//add ET to AET and sort
				p = pNET[i]->next;
				q = pAET;
				while (p)
				{
					while (q->next && p->x >= q->next->x)
						q = q->next;
					NET *s = p->next;
					p->next = q->next;
					q->next = p;
					p = s;
					q = pAET;
				}
				// update the color and depth buffer
				p = pAET->next;
				while (p && p->next) {
					for (int j = p->x; j < p->next->x; j++) {
						float z = p->ymin_z + (p->next->ymin_z - p->ymin_z) / ((int)p->next->x - (int)p->x)*(j-(int)p->x);
						if (DepBuffer[j][i] > z) {
							DepBuffer[j][i] = z;
							RBuffer.R[j][i] = r;
							RBuffer.G[j][i] = g;
							RBuffer.B[j][i] = b;
						}
					
					}
					p = p->next->next;
				}
			}
		}
	}

	// draw points using refresh buffer
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			glColor3f(RBuffer.R[i][j], RBuffer.G[i][j], RBuffer.B[i][j]);
			float x = (i - 256.0) / 256.0;
			float y = (j - 256.0) / 256.0;
	//		printf("%f,%f,%f\n", x, y, RBuffer.R[i][j]);
			glVertex2f(x, y);
		}
	}
	glEnd();

	// restore the vertices
	for (unsigned int i = 0; i < m.numVerts; i++) {
		m.verts[i].x = (m.verts[i].x - width / 2.0) / width / 2.0;
		m.verts[i].y = (m.verts[i].y - height/2.0) / height / 2.0;

		//printf("%f,%f,%f\n", m.verts[i].x, m.verts[i].y, m.verts[i].z);
	}
}