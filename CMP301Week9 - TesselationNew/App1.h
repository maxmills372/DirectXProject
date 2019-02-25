// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "TessellationShader.h"
#include "TessellationQuadShader.h"
#include "MyTessMesh.h"
#include "MyTessQuad.h"
#include "MyPlaneMesh.h"
#include "MyTessSphereMesh.h"
#include "../DXFramework/BaseMesh.h"

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
	ColourShader* colourShader;
	//TessellationShader* tessShader;
	TessellationQuadShader* tessQuadShader;

	TriangleMesh* mesh;
	TessellationMesh* tessMesh;
	MyTessMesh* myTessMesh;
	MyTessQuad* myTessQuad;
	MyPlaneMesh* myTessPlaneMesh;
	MyPlaneMesh* myPlaneMesh2;
	MyTessSphereMesh* myTessSphereMesh;

	bool reverse;
	XMFLOAT4 edges;
	XMFLOAT2 insides;

	bool wireframe;
	int current;
	
};

#endif