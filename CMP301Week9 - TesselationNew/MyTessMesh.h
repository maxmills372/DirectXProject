#pragma once
#include "../DXFramework/BaseMesh.h"
// Tessellation Mesh
// A simple triangle mesh used for tessellation demonstration.
// Different output type, instead of Triangle list, a 3 control point patch list.
// The tessellator uses these points to generate new geometry but sub-division.
// Due to change in geometry type the sendData() function has been overridden.

#ifndef _MYTESSMESH_H_
#define _MYTESSMESH_H_

using namespace DirectX;

class MyTessMesh : public BaseMesh
{

public:
	MyTessMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~MyTessMesh();

	void sendData(ID3D11DeviceContext*);

protected:
	void initBuffers(ID3D11Device* device);

};

#endif
