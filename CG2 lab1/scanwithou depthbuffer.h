#pragma once
#pragma once
#include"Model.h"

/******定义结构体用于活性边表AET和新边表NET***********************************/
typedef struct XET
{
	float x;
	float dx, ymax;

	XET* next;
}AET, NET;

/******定义点结构体point******************************************************/
struct point
{
	float x;
	float y;
};

void PolyScan(Model &m) {

	for (unsigned i = 0; i < m.numVerts; i++) {
		m.verts[i].x = 256.0 + m.verts[i].x*256.0;
		m.verts[i].y = 256.0 + m.verts[i].y*256.0;
		m.verts[i].x = ceil(m.verts[i].x);
		m.verts[i].y = ceil(m.verts[i].y);

		//		printf("%f,%f,%f\n", m.verts[i].x, m.verts[i].y, m.verts[i].z);
	}
	/*	for (unsigned j = 0; j < m.faces.size(); j++) {
	for (unsigned k = 0; k < m.faces[j].indices.size(); k++) {
	m.faces[j].indices[k] = m.faces[j].indices[k] - 1;
	}
	}*/

	/******计算最高点的y坐标(扫描到此结束)****************************************/
	int MaxY = 0;
	int i;
	for (i = 0; i < m.verts.size(); i++)
		if (m.verts[i].y > MaxY)
			MaxY = m.verts[i].y;

	glBegin(GL_POINTS);
	//printf("%d\n", MaxY);  //314
	/******扫描并建立NET表*********************************************************/
	for (unsigned int n = 0; n < m.faces.size(); n++) {
		if (m.backface[n] == 0) {
			/*******初始化AET表***********************************************************/
			AET *pAET = new AET;
			pAET->next = NULL;

			/******初始化NET表************************************************************/
			NET *pNET[1024];
			for (i = 0; i <= MaxY; i++)
			{
				pNET[i] = new NET;
				pNET[i]->next = NULL;
			}
			float r = (char)rand() % 256 / 256.0, g = (char)rand() % 256 / 256.0, b = (char)rand() % 256 / 256.0;
			glColor3f(r, g, b);

			for (unsigned int i = 0; i <= MaxY; i++)
			{
				for (unsigned int j = 0; j < m.faces[n].indices.size(); j++)
				{
					if (m.verts[m.faces[n].indices[j] - 1].y == i)
					{  //一个点跟前面的一个点形成一条线段，跟后面的点也形成线段   
					   //printf("\n%d\n", i);
						if (m.verts[m.faces[n].indices[(j - 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].y > m.verts[m.faces[n].indices[j] - 1].y)
						{
							NET *p = new NET;
							p->x = m.verts[m.faces[n].indices[j] - 1].x;
							p->ymax = m.verts[m.faces[n].indices[(j - 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].y;
							p->dx = (m.verts[m.faces[n].indices[(j - 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].x - m.verts[m.faces[n].indices[j] - 1].x) / (m.verts[m.faces[n].indices[(j - 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].y - m.verts[m.faces[n].indices[j] - 1].y);
							p->next = pNET[i]->next;
							pNET[i]->next = p;

						}
						if (m.verts[m.faces[n].indices[(j + 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].y > m.verts[m.faces[n].indices[j] - 1].y)
						{
							NET *p = new NET;
							p->x = m.verts[m.faces[n].indices[j] - 1].x;
							p->ymax = m.verts[m.faces[n].indices[(j + 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].y;
							p->dx = (m.verts[m.faces[n].indices[(j + 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].x - m.verts[m.faces[n].indices[j] - 1].x) / (m.verts[m.faces[n].indices[(j + 1 + m.faces[n].indices.size()) % m.faces[n].indices.size()] - 1].y - m.verts[m.faces[n].indices[j] - 1].y);
							p->next = pNET[i]->next;
							pNET[i]->next = p;
						}
					}
				}
			}
			/******建立并更新活性边表AET*****************************************************/
			for (unsigned int i = 0; i <= MaxY; i++)
			{
				//计算新的交点x,更新AET
				NET *p = pAET->next;
				while (p)
				{
					p->x = p->x + p->dx;
					p = p->next;
				}
				//更新后新AET先排序*************************************************************/
				//断表排序,不再开辟空间
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
				//(改进算法)先从AET表中删除ymax==i的结点****************************************/
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
				//将NET中的新点加入AET,并用插入法按X值递增排序**********************************/
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
				/******配对填充颜色***************************************************************/

				p = pAET->next;
				while (p && p->next)
				{
					for (int j = p->x; j <= p->next->x; j++)
					{

						float tempj = (j - 256.0) / 256.0;
						float tempi = (i - 256.0) / 256.0;
						glVertex2f(tempj, tempi);
						//		printf("%f,%f\n", tempj, tempi);
						//glVertex2f(j, i);
						//	printf("%d,%d\n", j, i);
					}

					p = p->next->next;//考虑端点情况
				}

			}


		}
	}
	glEnd();

	for (unsigned int i = 0; i < m.numVerts; i++) {
		m.verts[i].x = (m.verts[i].x - 256.0) / 256.0;
		m.verts[i].y = (m.verts[i].y - 256.0) / 256.0;

		//printf("%f,%f,%f\n", m.verts[i].x, m.verts[i].y, m.verts[i].z);
	}
}