// Light shader.h
// Basic single light shader setup
#ifndef _SOFTSHADOWSHADER_H_
#define _SOFTSHADOWSHADER_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"

using namespace std;
using namespace DirectX;


class SoftShadowShader : public BaseShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;

	};

	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		//XMFLOAT4 ambient2;
		//XMFLOAT4 diffuse2;
	};

	struct LightBufferType2
	{
		XMFLOAT3 position;
		float padding;
		//XMFLOAT3 position2;
		//float padding2;
	};

public:

	SoftShadowShader(ID3D11Device* device, HWND hwnd);
	~SoftShadowShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMap, Light* light);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateClamp;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* lightBuffer2;

};

#endif
