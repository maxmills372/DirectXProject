// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "LightShader.h"
#include "ManipulationShader.h"
#include "../DXFramework/Input.h"
#include "TextureShader1.h"


class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	void RenderToTexture();
	void RenderScene();
	void gui();

private:
	ColourShader* colourShader;
	LightShader* lightShader;
	TextureShader* textureShader;
	ManipulationShader* manipShader;

	RenderTexture* renderTexture;
	
	OrthoMesh* orthoMesh;
	CubeMesh* mesh_2;

	Light* my_light;
	Light* light_2;

	float counter, height, length,speed;
	float rotate;

	
};

#endif