#pragma once

#include <iostream>
#include "tools/mesh.hpp"

using namespace STP3D;

namespace glbasimac {

#define MAX_NB_POINTS_SET_OF_POINTS 100

struct GLBI_Set_Of_Points {
	GLBI_Set_Of_Points():nb_pts(0),pts(MAX_NB_POINTS_SET_OF_POINTS,GL_POINTS) {};

	~GLBI_Set_Of_Points() {
		coord_pts.clear();
		color_pts.clear();
	};

	void initSet();

	void addAPoint(float x,float y,float c_r,float c_v,float c_b);

	void drawSet();

	// Application and GL parameters
	unsigned int nb_pts;
	StandardMesh pts;
	std::vector<float> coord_pts;
	std::vector<float> color_pts;

};

}
