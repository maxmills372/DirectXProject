// Light shader.h
// Basic single light shader setup
#ifndef _TessellationQuadShader_H_
#define _TessellationQuadShader_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Camera.h"
#include "../DXFramework/Light.h"

using namespace std;
using namespace DirectX;


class TessellationQuadShader : public BaseShader
{
private:
	struct TessBufferType
	{
		XMFLOAT4 edges;
		XMFLOAT2 insides;
		XMFLOAT2 padding;
	};
	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding2;
	};
	struct HeightBufferType
	{
		float heightMapSize;
		XMFLOAT3 padding3;
	};
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

public:
	TessellationQuadShader(ID3D11Device* device, HWND hwnd);
	~TessellationQuadShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, 
							 ID3D11ShaderResourceView* heightMap, /*XMFLOAT4 edges, XMFLOAT2 insides,*/ Camera* camera, Light* light, Light* light_2, XMFLOAT3 camera_offset, float planeHeightMapSize);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11RasterizerState* rastState;
	ID3D11Buffer* tessBuffer;
	ID3D11Buffer* cameraBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* heightBuffer;

};

#endif