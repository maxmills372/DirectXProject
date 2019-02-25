// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "../DXFramework/Input.h"

#include "TextureShader1.h"
#include "LightShader.h"
#include "ManipulationShader.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"
#include "ShadowShader.h"
#include "DepthShader.h"
#include "MySinglePointMesh.h"
#include "GeometryShader.h"
#include "MyPlaneMesh.h"
#include "TessellationQuadShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	// BLUR
	void RenderToTexture();
	void DownSampleTexture();
	void RenderHorizontalBlur();
	void RenderVerticalBlur();
	void UpSampleTexture();
	void RenderFinalBlur();

	void RenderScene();
	//SHADOW
	void renderToShadowMap(Light* light, RenderTexture* renderTexture);
	void renderShadows();

	void gui();

private:

	//Shaders
	LightShader* lightShader;
	TextureShader* textureShader;
	HorizontalBlurShader* hBlurShader;
	VerticalBlurShader* vBlurShader;

	ShadowShader* shadowShader;
	DepthShader* depthShader;
	GeometryShader* geometryShader;

	TessellationQuadShader* tessQuadShader;
	ManipulationShader* manipShader;

	//RenderTextures
	RenderTexture* renderTexture;
	RenderTexture* renderTextureDown;
	RenderTexture* renderTextureHorizontal;
	RenderTexture* renderTextureVertical;
	RenderTexture* renderTextureUp;
	
	RenderTexture* lightMapTexture;
	RenderTexture* lightMapTexture2;

	//Meshes
	OrthoMesh* orthoMesh;
	OrthoMesh* orthoMeshDown;
	CubeMesh* cubeBlurMesh;

	PlaneMesh* planeMesh;
	PlaneMesh* planeMesh2;
	CubeMesh* cubeMesh;
	SphereMesh* sphereMesh;
	SphereMesh* sphereMesh2;
	MySinglePointMesh* singlePointMesh;
	MyPlaneMesh* myTessPlaneMesh;
	SphereMesh* manipSphereMesh;
	Model* model;

	//Lights
	Light* my_light;
	Light* light_1;
	Light* light_2;
	Light* light_3;

	//Changable Variables
	float counter, height, length,speed;
	float rotate;
	int sW, sH;
	int weights[4];
	XMFLOAT2 blurDirection;	//XMVECTOR
	bool directionalBlur, wireframe, moving, motionBlur, testRender;
	XMFLOAT3 planePosition, tessPlanePosition, planePosition2, heightMapSpherePos, cubePosition;

	const int SHADOWMAP_WIDTH = 2048;
	const int SHADOWMAP_HEIGHT = 2048;
	//XMFLOAT3 movingLightPos;
	float movingLightPos[3];
	float movingLightPos2[3];
	float movingLightPos3[3];
	float pointDiffuse[3];
	float pointDiffuse2[3];

	float i;
	int storedX,storedY;
	float velocityX,velocityY;
	float blurAmount;
	float storedTime;
	bool displayPositions;
	float planeHeightMapSize, sphereHeightMapSize;
	float modelRotationY;
	float div;

};

#endif