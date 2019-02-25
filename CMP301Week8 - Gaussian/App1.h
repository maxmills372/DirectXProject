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
#include "BoxBlur.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"

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
	void DownSampleTexture();
	void RenderHorizontalBlur();
	void RenderVerticalBlur();
	void UpSampleTexture();
	void RenderScene();

	void RenderFinalBlur();
	void gui();

private:
	//ColourShader* colourShader;
	LightShader* lightShader;
	TextureShader* textureShader;
	BoxBlur* boxBlurShader;
	ManipulationShader* manipShader;
	HorizontalBlurShader* hBlurShader;
	VerticalBlurShader* vBlurShader;
	
	RenderTexture* renderTexture;
	RenderTexture* renderTextureDown;
	RenderTexture* renderTextureHorizontal;
	RenderTexture* renderTextureVertical;
	RenderTexture* renderTextureUp;
	
	OrthoMesh* orthoMesh;
	OrthoMesh* orthoMeshDown;
	CubeMesh* mesh_2;
	PlaneMesh* plane_mesh;

	Light* my_light;
	Light* light_2;

	float counter, height, length,speed;
	float rotate;

	int sW, sH;
	int weights[4];

	XMFLOAT2 blurDirection;	//XMVECTOR
	bool directionalBlur;
	
};

#endif