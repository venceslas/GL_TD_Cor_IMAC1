#pragma once

#include <iostream>
#include "tools/matrix4d.hpp"
#include "tools/matrix_stack.hpp"

using namespace STP3D;

namespace glbasimac {

struct GLBI_Engine {
	GLBI_Engine():mode2D(false) {
		std::cout<<"Engine initialization"<<std::endl;
	}

	~GLBI_Engine() {


		std::cerr<<"Fin de l'engine"<<std::endl;
	}

	// Set the OpenGL Engine
	void initGL();
	// Set 2D orthographic projection. Resulting virtual screen size is [xmin,ymin][xmax,ymax]
	/// \param diag diagonal component
	void set2DProjection(float xmin,float xmax,float ymin,float ymax,int ids=0);
	// Set the current flat color to r,g,b. This color will remains until changed
	void setFlatColor(float r,float g,float b,int ids=0);
	// Send current transformation to GL Engine. ids is the id of the shader to set.
	void updateMvMatrix(int ids=0);

	void test();

	// GL parameters
	unsigned int idShader[3];
	Matrix4D projMatrix;
	MatrixStack mvMatrixStack;
	bool mode2D;

};

}
