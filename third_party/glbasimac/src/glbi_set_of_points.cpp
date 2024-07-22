#include "glbasimac/glbi_set_of_points.hpp"
#include <array>

namespace glbasimac {

	void GLBI_Set_Of_Points::initSet() {
		//glUseProgram(idShader[0]);
		std::cerr<<"Type of set points : "<<pts.gl_type_mesh<<std::endl;
		std::cerr<<"Nb max of points : "<<pts.nb_elts<<std::endl;
		std::cerr<<"Nb of points : "<<nb_pts<<std::endl;

		coord_pts.resize(2*MAX_NB_POINTS_SET_OF_POINTS,0.0f);
		pts.buffers.push_back(coord_pts.data());
		pts.copied.push_back(false);
		pts.attr_id.push_back(0);
		pts.size_one_elt.push_back(2);
		pts.attr_semantic.push_back("coordinates");

		color_pts.resize(3*MAX_NB_POINTS_SET_OF_POINTS,1.0f);
		//color_pts[0] = 1.0;
		pts.buffers.push_back(color_pts.data());
		pts.copied.push_back(false);
		pts.attr_id.push_back(3);
		pts.size_one_elt.push_back(3);
		pts.attr_semantic.push_back("Colors");

		pts.createVAO();
	}

	void GLBI_Set_Of_Points::drawSet() {
		//std::cerr<<"VAO : "<<pts.id_vao<<std::endl;
		glBindVertexArray(pts.id_vao);

		//std::cerr<<"N= "<<nb_pts<<std::endl;
		glDrawArrays(GL_POINTS,0,1);

		glBindVertexArray(0);
	}
}
