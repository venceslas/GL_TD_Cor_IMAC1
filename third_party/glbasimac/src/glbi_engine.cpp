#include "glbasimac/glbi_engine.hpp"
#include "tools/shaders.hpp"
using namespace glbasimac;
using namespace STP3D;

namespace glbasimac {

	void GLBI_Engine::initGL() {
		std::cout<<"Initialisation of GL Engine"<<std::endl;

		if (mode2D) {
			std::cerr<<"Flat 2D"<<std::endl;
			idShader[0] = ShaderManager::loadShader("../assets/shaders/flat_shading_2D.vert","../assets/shaders/flat_shading.frag",true);
		}

		glUseProgram(idShader[0]);
	}

	void GLBI_Engine::setFlatColor(float r,float g,float b,int ids) {
		glVertexAttrib3f(glGetAttribLocation(idShader[ids],"vx_col"),r,g,b);
	}

	void GLBI_Engine::updateMvMatrix(int ids) {
		glUniformMatrix4fv(glGetUniformLocation(idShader[ids],"modelviewMat"),1,GL_FALSE,mvMatrixStack.getTopGLMatrix());
		if (!mode2D) {
			Matrix4D nmlMatrix = mvMatrixStack.getTopGLMatrix();
			nmlMatrix.invert();
			nmlMatrix.transpose();
			glUniformMatrix4fv(glGetUniformLocation(idShader[ids],"normalMat"),1,GL_FALSE,nmlMatrix);
		}
	}

	void GLBI_Engine::set2DProjection(float xmin,float xmax,float ymin,float ymax,int ids) {
		Matrix4D proj = Matrix4D::ortho2D(xmin,xmax,ymin,ymax);
		glUniformMatrix4fv(glGetUniformLocation(idShader[ids],"projectionMat"),1,GL_FALSE,proj);
	}


}
