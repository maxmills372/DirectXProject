// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
//#include "ColourShader.h"
#include "GeometryShader.h"
#include "../DXFramework/Light.h"
#include "MySinglePointMesh.h"
#include "ColourShader.h"
#include "MyPlaneMesh.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	void gui();

private:
	
	GeometryShader* geoShader;

	TriangleMesh* triMesh;
	MySinglePointMesh* pointMesh;
	MyPlaneMesh* myPlaneMesh;
	SphereMesh* sphereMesh;

	ColourShader* colourShader;

	Model* model;

	bool wireframe, alpha;

	Light* my_light;
	Light* light_2;
	
	int i;
};

#endif