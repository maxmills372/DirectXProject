#pragma once
//#include "F:\_Uni Year 3 Semester 1\Graphics\ShadersWeek4\Shaders\DXFramework\BaseShader.h" // change to E for uni computers
#include "../DXFramework/BaseShader.h"

// Light shader.h
// Basic single light shader setup
#ifndef _ManipulationShader_H_
#define _ManipulationShader_H_

#include "../DXFramework/Light.h"
#include "../DXFramework/Camera.h"

using namespace std;
using namespace DirectX;


class ManipulationShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 diffuse[2];
		XMFLOAT4 ambient[2];
		XMFLOAT4 position[2];
		XMFLOAT4 direction[2];
		//FLOAT specularPower[2];
		//XMFLOAT4 specularColour[2];
		XMFLOAT4 type[2];

	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		FLOAT padding;
	};

	struct TimeBufferType
	{
		float time;
		//XMFLOAT3 padding2;
		float height;
		float length;
		float padding2;
	};

public:

	ManipulationShader(ID3D11Device* device, HWND hwnd);
	~ManipulationShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light, Light* light_2, Camera* camera, float counter, float height, float length);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* cameraBuffer;
	ID3D11Buffer* timeBuffer;

#endif
};




