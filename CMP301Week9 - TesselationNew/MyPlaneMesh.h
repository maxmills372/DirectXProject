// MyPlaneMesh.h
// Generates a plane mesh based on resolution.
// Resolution is the width and depth of the generated plane, built from unit quads.

#ifndef _MYPLANEMESH_H_
#define _MYPLANEMESH_H_

#include "../DXFramework/BaseMesh.h"
#include <vector>
class MyPlaneMesh : public BaseMesh
{
private:
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	unsigned long* indices;
public:
	MyPlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution, VertexType vertices[]);
	MyPlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution);
	~MyPlaneMesh();
	void sendData(ID3D11DeviceContext*);

	//VertexType my_vertices[5000];

	VertexType* my_vertices;
	void Triangulate(ID3D11Device* device, VertexType vertices[]);
	void AddVert(VertexType vertex);

protected:
	void initBuffers(ID3D11Device* device);
	void Init_UsingVerts(ID3D11Device* device, VertexType vertices[]);
	int resolution;
};

#endif