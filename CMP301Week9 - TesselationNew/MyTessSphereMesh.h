// Sphere Mesh
// Generated sphere mesh with texture coordinates and normals.
// Uses the cube sphere normalisation method. First a cube is generated,
// then the vertices are normalised creating a sphere.
// Resolution specifies the number of segments in the sphere (top and bottom, matches equator).

#ifndef _MYTESSSPHEREMESH_H_
#define _MYTESSSPHEREMES_H_

#include "../DXFramework/BaseMesh.h"

using namespace DirectX;

class MyTessSphereMesh : public BaseMesh
{

public:
	MyTessSphereMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 20);
	~MyTessSphereMesh();
	void sendData(ID3D11DeviceContext*);

protected:
	void initBuffers(ID3D11Device* device);
	int resolution;
};

#endif