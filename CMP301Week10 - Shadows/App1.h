// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "DepthShader.h"
#include "ShadowShader.h"
#include "../DXFramework/Light.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	void renderToTexture1();
	void renderToTexture2();
	void renderToTexture(Light*, RenderTexture* );
	void renderScene();
	void gui();

private:
	ColourShader* colourShader;
	DepthShader* depthShader;
	ShadowShader* shadowShader;

	Light* light;
	Light* light_2;
	Light* light_3;

	RenderTexture* renderTexture;
	//For second light shadow
	RenderTexture* renderTexture2;

	PlaneMesh* mesh;
	CubeMesh* cubeMesh;
	SphereMesh* sphereMesh;

	XMFLOAT3* spherePos;
	XMFLOAT3* movingLightPos;

	const int SHADOWMAP_WIDTH = 2048;
	const int SHADOWMAP_HEIGHT = 2048;

	Model* model;
	
};

#endif