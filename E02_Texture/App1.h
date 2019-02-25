// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/quadmesh.h"
#include "TextureShader.h"
#include "SquareMesh.h"
#include "../DXFramework/PlaneMesh.h"

#include "NoisePlaneMesh.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);

	bool frame();

protected:
	bool render();
	void gui();

private:
	TextureShader* textureShader;
	SquareMesh* mesh;
	SquareMesh* mesh_2;
	NoisePlaneMesh *noise_plane_mesh;

	float rotation_value;

	float div;
};

#endif