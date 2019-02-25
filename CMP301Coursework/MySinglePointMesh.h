// MySinglePointMesh.h
// A simple mesh for demostrating the geometry shader
// Instead producing a triangle list, produce a point list.
// This point list is three points of a triangle and can be used by the geometry shader to generate geometry.
// Therefore has a modified sendData() function.

#ifndef _MySinglePointMesh_H_
#define _MySinglePointMesh_H_

#include "../DXFramework/BaseMesh.h"

using namespace DirectX;

class MySinglePointMesh : public BaseMesh
{

private:
	VertexType* vertices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	unsigned long* indices;

public:
	MySinglePointMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~MySinglePointMesh();

	void sendData(ID3D11DeviceContext*);
	void RandomisePosition(ID3D11Device*);

protected:
	void initBuffers(ID3D11Device* device);

	XMFLOAT3 positions[100];
	float increase;
};

#endif