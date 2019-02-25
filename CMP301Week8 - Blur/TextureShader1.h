// texture shader.h
#ifndef _TEXTURESHADER_H_
#define _TEXTURESHADER_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"


using namespace std;
using namespace DirectX;


class TextureShader : public BaseShader
{

public:

	TextureShader(ID3D11Device* device, HWND hwnd);
	~TextureShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light, Light* light_2);//, ID3D11ShaderResourceView* texture2);
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

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
};

#endif