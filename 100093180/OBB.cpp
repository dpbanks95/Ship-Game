#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLu.h>
#include <math.h>
#include <iostream>

#include "OBB.h"

OBB::OBB(){
	for (int i = 0; i<NUMVERTS; i++){
		vert[i].x = 0.0; vert[i].y = 0.0;
		vertOriginal[i].x = 0.0; vertOriginal[i].y = 0.0;
	}
}

void OBB::drawOBB(){
	glLineWidth(0.1);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i<NUMVERTS; i++)
		glVertex2f(vert[i].x, vert[i].y);
	glEnd();
	glLineWidth(1);
}

void OBB::transformPoints(float matrix[16]){
	for (int i = 0; i<NUMVERTS; i++)
	{
		vert[i].x = matrix[0] * vertOriginal[i].x + matrix[4] * vertOriginal[i].y + matrix[12];
		vert[i].y = matrix[1] * vertOriginal[i].x + matrix[5] * vertOriginal[i].y + matrix[13];
	}
}

bool OBB::SATtest(float proj[4], float len){
	float min = proj[0], max = proj[0];
	for (int i = 1; i <= 3; ++i){
		// Find minimum and maximum projections - the other two will lie in between and are of no consequence
		if (proj[i]<min){
			min = proj[i];
		}
		else if (proj[i]>max){
			max = proj[i];
		}
	}

	// comparison noting that the first point of first poly is at distance 0 and the second point at distance len
	// along the edge of the first poly acting as the axis
	if (min > len || max < 0.0){
		// found separating axis
		return true;
	}
	else{
		return false;
	}
}

bool OBB::SAT2D(OBB* p2){
	float axis[2], len, proj[4];
	// 2 tests for each edge of a polygon = 4 tests ( 4 axes )

	//// Axis 1 OBB 1-------------------------------------------------------------------------------------
	axis[0] = this->vert[1].x - this->vert[0].x;
	axis[1] = this->vert[1].y - this->vert[0].y;
	len = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
	// unit vector
	axis[0] /= len;
	axis[1] /= len;  

	// project second poly points on axis
	for (int i = 0; i < NUMVERTS; i++){
		// dot product
		proj[i] = (p2->vert[i].x - this->vert[0].x)*axis[0] + (p2->vert[i].y - this->vert[0].y)*axis[1];
	}
	if (SATtest(proj, len)){
		return false;
	}

	// Axis 2 OBB 1---------------------------------------------------------------------------------------
	axis[0] = this->vert[2].x - this->vert[1].x;
	axis[1] = this->vert[2].y - this->vert[1].y;
	len = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
	// unit vector
	axis[0] /= len;
	axis[1] /= len;  

	// project second poly points on axis
	for (int i = 0; i < NUMVERTS; i++){
		// dot product
		proj[i] = (p2->vert[i].x - this->vert[1].x)*axis[0] + (p2->vert[i].y - this->vert[1].y)*axis[1];
	}
	if (SATtest(proj, len)){
		return false;
	}

	// Axis 3 OBB 2---------------------------------------------------------------------------------------
	axis[0] = p2->vert[1].x - p2->vert[0].x;
	axis[1] = p2->vert[1].y - p2->vert[0].y;
	len = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
	// unit vector
	axis[0] /= len;
	axis[1] /= len;  

	// project first poly points on axis
	for (int i = 0; i < NUMVERTS; i++){
		// dot product
		proj[i] = (this->vert[i].x - p2->vert[0].x)*axis[0] + (this->vert[i].y - p2->vert[0].y)*axis[1];
	}
		 
	if (SATtest(proj, len)){
		return false;
	}

	// Axis 4 OBB 2---------------------------------------------------------------------------------------
	axis[0] = p2->vert[2].x - p2->vert[1].x;
	axis[1] = p2->vert[2].y - p2->vert[1].y;
	len = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
	// unit vector
	axis[0] /= len;
	axis[1] /= len;  

	// project first poly points on axis
	for (int i = 0; i < NUMVERTS; i++){
		// dot product
		proj[i] = (this->vert[i].x - p2->vert[1].x)*axis[0] + (this->vert[i].y - p2->vert[1].y)*axis[1];
	}
	if (SATtest(proj, len)){
		return false;
	}

	//no separating axis - must be in collision
	return true;
}