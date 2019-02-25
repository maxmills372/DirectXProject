// texture shader.h
#ifndef _BoxBlur_H_
#define _BoxBlur_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"


using namespace std;
using namespace DirectX;


class BoxBlur : public BaseShader
{

public:

	BoxBlur(ID3D11Device* device, HWND hwnd);
	~BoxBlur();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light, Light* light_2, int screenWidth, int screenHeight);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR*, WCHAR*);

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

	struct ScreenSizeBufferType
	{
		FLOAT screenWidth;
		FLOAT screenHeight;
		XMFLOAT2 padding;

	};

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* ScreenSizeBuffer;
};

#endif