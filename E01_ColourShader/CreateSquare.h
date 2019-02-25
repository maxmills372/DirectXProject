//dgkjhbsjhgf

//#ifndef _TRIMESH_H_
//#define _TRIMESH_H_

//#include "M:\_GPwS_week1\ShadersFramework\Shaders\DXFramework\BaseMesh.h"
#include "../DXFramework/BaseMesh.h"

using namespace DirectX;

class CreateSquare :
	public BaseMesh
{

public:
	CreateSquare(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~CreateSquare();


protected:
	void initBuffers(ID3D11Device* device);
};

