#pragma once
#include "../DXFramework/BaseMesh.h"

using namespace DirectX;

class SquareMesh :
	public BaseMesh
{

public:
	SquareMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~SquareMesh();


protected:
	void initBuffers(ID3D11Device* device);
};
