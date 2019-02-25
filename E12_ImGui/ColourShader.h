// Colour shader.h
// Simple shader example.
#ifndef _COLOURSHADER_H_
#define _COLOURSHADER_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/imgui.h"

using namespace std;
using namespace DirectX;


class ColourShader : public BaseShader
{

public:

	struct ColourBufferType
	{
		XMFLOAT4 colour;
	};

	ColourShader(ID3D11Device* device, HWND hwnd);
	~ColourShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ImVec4 colour);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* colourBuffer;
};

#endif