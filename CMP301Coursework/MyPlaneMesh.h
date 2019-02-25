// MyPlaneMesh.h
// Generates a plane mesh based on resolution.
// Resolution is the width and depth of the generated plane, built from unit quads.

#ifndef _MYPLANEMESH_H_
#define _MYPLANEMESH_H_

#include "../DXFramework/BaseMesh.h"

class MyPlaneMesh : public BaseMesh
{

public:
	MyPlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 10);
	~MyPlaneMesh();
	void sendData(ID3D11DeviceContext*);

protected:
	void initBuffers(ID3D11Device* device);
	int resolution;
};

#endif