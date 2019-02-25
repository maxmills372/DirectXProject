// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "LightShader.h"

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
	LightShader* lightShader;
	PlaneMesh* mesh;
	SphereMesh* mesh_2;
	SphereMesh* mesh_3;
	SphereMesh* mesh_4;

	Light* my_light;
	Light* light_2;

	
};

#endif