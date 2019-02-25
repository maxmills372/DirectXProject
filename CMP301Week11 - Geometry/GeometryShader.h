// Light shader.h
// Geometry shader example.
#ifndef _GEOMETRYSHADER_H_
#define _GEOMETRYSHADER_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"
#include "../DXFramework/Camera.h"

using namespace std;
using namespace DirectX;


class GeometryShader : public BaseShader
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

public:

	GeometryShader(ID3D11Device* device, HWND hwnd);
	~GeometryShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* my_light, Light* light_2, bool alpha, Camera* camera, float time);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;

	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* cameraBuffer;

	ID3D11BlendState* d3dBlendState;
};

#endif