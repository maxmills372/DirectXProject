// NoisePlaneMesh.h
// Generates a plane mesh based on resolution.
// Resolution is the width and depth of the generated plane, built from unit quads.

#ifndef _NOISEPLANEMESH_H_
#define _NOISEPLANEMESH_H_

#include "../DXFramework/BaseMesh.h"

class NoisePlaneMesh : public BaseMesh
{

public:
	NoisePlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 100);
	~NoisePlaneMesh();

protected:
	void initBuffers(ID3D11Device* device);
	int resolution;
};

#endif