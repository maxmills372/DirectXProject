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

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* texture2, Light* light, Light* light_2, float div);//, ID3D11ShaderResourceView* texture2);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR*, WCHAR*);

	
	struct BlendTexBufferType
	{
		float divisionAmount;
		XMFLOAT3 padding;
	};

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* blendTexBuffer;
};

#endif