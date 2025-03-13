#include "draw_scene.hpp"

/// Camera parameters
float angle_theta {45.0};    // Angle between x axis and viewpoint
float angle_phy {30.0};      // Angle between z axis and viewpoint
float dist_zoom {30.0};      // Distance between origin and viewpoint

bool flag_anim_rot_arm{false};
bool flag_anim_rot_scale{false};
bool flag_anim_light{false};

GLBI_Engine myEngine;
GLBI_Convex_2D_Shape ground{3};
GLBI_Convex_2D_Shape disque{3};
GLBI_Set_Of_Points frame(3);
GLBI_Set_Of_Points string_pan(3);
IndexedMesh* sphere;
StandardMesh* cone;
float rot_arm{0.0};
int dir_rot_arm{1};
float rot_scale{0.0};
float rot_light{0.0};

void initScene() {

	std::vector<float> baseCarre{-10.0,-10.0,0.0,
								 10.0,-10.0,0.0,
								 10.0,10.0,0.0,
								 -10.0,10.0,0.0};
	ground.initShape(baseCarre);
	ground.changeNature(GL_TRIANGLE_FAN);

	float lg = 10.0;
	std::vector<float> pts_frame{0.0,0.0,0.0,
								lg,0.0,0.0,
								0.0,0.0,0.0,
								0.0,lg,0.0,
								0.0,0.0,0.0,
								0.0,0.0,lg};
	std::vector<float> col_frame{1.0,0.0,0.0,
								1.0,0.0,0.0,
								0.0,1.0,0.0,
								0.0,1.0,0.0,
								0.0,0.0,1.0,
								0.0,0.0,1.0};
	frame.initSet(pts_frame,col_frame);
	frame.changeNature(GL_LINES);

	sphere = basicSphere();
	sphere->createVAO();

	std::vector<float> pt_cercle;
	std::vector<float> nm_cercle;
	float angle {0.0};
	for(int i=0;i<50;i++) {
		angle = i*2.*M_PI/(float)50;
		pt_cercle.push_back(cos(angle));
		pt_cercle.push_back(sin(angle));
		pt_cercle.push_back(0.0f);
	}
	disque.initShape(pt_cercle);
	disque.changeNature(GL_TRIANGLE_FAN);

	cone = basicCone(1.0,1.0);
	cone->createVAO();

	std::vector<float> pt_pan{0.0,0.0,0.0,
								3.0,0.0,-5.0,
								0.0,0.0,0.0,
								3.0*cosf(2.0*M_PI/3.0),3.0*sinf(2.0*M_PI/3.0),-5.0,
								0.0,0.0,0.0,
								3.0*cosf(4.0*M_PI/3.0),3.0*sinf(4.0*M_PI/3.0),-5.0};
	string_pan.initSet(pt_pan,1.0,0.0,0.0);
	string_pan.changeNature(GL_LINES);

	myEngine.switchToPhongShading();
	myEngine.setLightPosition({5.0f,0.0f,15.0,1.0});
	myEngine.setLightIntensity({100.0,100.0,100.0});
	// myEngine.addALight({0.0,1.0,1.0,0.0},{0.0,0.0,1.0});
	// myEngine.addALight({1.0,0.0,1.0,0.0},{0.5,0.0,0.0});
	// myEngine.setAttenuationFactor({1.0,0.0,1.0});
	myEngine.switchToFlatShading();
}

void drawFrame() {
	frame.drawSet();
}

void drawBase() {
	// TO DO
	myEngine.setFlatColor(0.921568627f,0.811764706f,0.203921569f);
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({0.0,0.0,0.1});
		myEngine.mvMatrixStack.addHomothety(3.0);
		myEngine.updateMvMatrix();
		myEngine.setNormalForConvex2DShape({0.0,0.0,1.0});
		disque.drawShape();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({0.0,0.0,0.1});
		myEngine.mvMatrixStack.addRotation(M_PI/2.0,{1.0,0.0,0.0});
		myEngine.mvMatrixStack.addHomothety({2.0,10.0,2.0});
		myEngine.updateMvMatrix();
		cone->draw();
	myEngine.mvMatrixStack.popMatrix();

}

void drawArm() {
	myEngine.setFlatColor(0.960784314f,0.643137255,0.258823529);
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety(1.6);
		myEngine.updateMvMatrix();
		sphere->draw();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety({1.0,10.0,1.0});
		myEngine.updateMvMatrix();
		cone->draw();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addRotation(M_PI,{1.0,0.0,0.0});
		myEngine.mvMatrixStack.addHomothety({1.0,10.0,1.0});
		myEngine.updateMvMatrix();
		cone->draw();
	myEngine.mvMatrixStack.popMatrix();
}

void drawPan() {
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.updateMvMatrix();
		string_pan.drawSet();
	
		myEngine.mvMatrixStack.addTranslation({0.0,0.0,-5.0});
		myEngine.mvMatrixStack.addHomothety(3.0);
		myEngine.setFlatColor(0.349019608,0.450980392,0.0);
		myEngine.updateMvMatrix();
		myEngine.setNormalForConvex2DShape({0.0,0.0,1.0});
		disque.drawShape();
	myEngine.mvMatrixStack.popMatrix();
}

void drawScene() {
	glPointSize(10.0);

	if (flag_anim_light) {
		rot_light += 1.0;
	}
	if (flag_anim_rot_arm) {
		rot_arm += dir_rot_arm;
		if ((rot_arm>=20)||(rot_arm <= -20)) dir_rot_arm *= -1;
	}
	if (flag_anim_rot_scale) {
		rot_scale += 1.0;
	}
	

	myEngine.setFlatColor(0.2,0.0,0.0);
	ground.drawShape();

	drawFrame();

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({5.0f*cosf(rot_light/10.0f),5.0f*sinf(rot_light/10.0f),15.0});
		myEngine.mvMatrixStack.addHomothety(0.5);
		myEngine.updateMvMatrix();
		myEngine.setFlatColor(1.0,1.0,0.0);
		sphere->draw();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.switchToPhongShading();
	myEngine.setLightPosition({5.0f*cosf(rot_light/10.0f),5.0f*sinf(rot_light/10.0f),15.0,1.0});
	// myEngine.setLightPosition({cosf(M_PI+rot_light/10.0f),sinf(M_PI+rot_light/10.0f),1.0,0.0},1);
	// myEngine.setLightPosition({cosf(rot_light/10.0f),sinf(rot_light/10.0f),1.0,0.0},2);
	// myEngine.setAttenuationFactor({1.0,0.0,1.0});
	myEngine.setShininess(32.0);
	myEngine.setSpecularColor({1.0,0.0,0.0});

	myEngine.mvMatrixStack.addRotation(rot_scale*M_PI/100.0,{0.0,0.0,1.0});
	myEngine.updateMvMatrix();
	drawBase();

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({0.0,0.0,10.1});
		myEngine.mvMatrixStack.addRotation(deg2rad(rot_arm),{1.0,0.0,0.0});
		drawArm();

		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addTranslation({0.0,10.0,0.0});
			myEngine.mvMatrixStack.addRotation(deg2rad(-rot_arm),{1.0,0.0,0.0});
			drawPan();
		myEngine.mvMatrixStack.popMatrix();
		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addTranslation({0.0,-10.0,0.0});
			myEngine.mvMatrixStack.addRotation(deg2rad(-rot_arm),{1.0,0.0,0.0});
			drawPan();
		myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.switchToFlatShading();

}



