// Light shader.h
// Basic single light shader setup
#ifndef _TessellationQuadShader_H_
#define _TessellationQuadShader_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Camera.h"

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

public:
	TessellationQuadShader(ID3D11Device* device, HWND hwnd);
	~TessellationQuadShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* heightMap, XMFLOAT4 edges, XMFLOAT2 insides, Camera* camera);
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
};

#endif