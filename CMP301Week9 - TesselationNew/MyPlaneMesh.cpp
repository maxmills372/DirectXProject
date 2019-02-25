// plane mesh
// Quad mesh made of many quads. Default is 100x100
#include "MyPlaneMesh.h"

// Initialise buffer and load texture.
MyPlaneMesh::MyPlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int lresolution, VertexType vertices[])
{
	resolution = lresolution;
	
	Init_UsingVerts(device, vertices);
	
}
MyPlaneMesh::MyPlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int lresolution)
{
	resolution = lresolution;
	
		initBuffers(device);
}

// Release resources.
MyPlaneMesh::~MyPlaneMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

// Generate plane (including texture coordinates and normals).
void MyPlaneMesh::initBuffers(ID3D11Device* device)
{
	//VertexType* vertices;
	//unsigned long* indices;
	int index, i, j;
	float positionX, positionZ, u, v, increment;
	/*D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;*/

	// Calculate the number of vertices in the terrain mesh.
	vertexCount = (resolution - 1) * (resolution - 1) * 8;
	indexCount = vertexCount;

	my_vertices = new VertexType[vertexCount];			
	indices = new unsigned long[indexCount];

	index = 0;
	// UV coords.
	u = 0;
	v = 0;
	increment = 1.0f / resolution;

	for (j = 0; j<(resolution - 1); j++)
	{
		for (i = 0; i<(resolution - 1); i++)
		{

			// Upper right.
			//positionX = (float)(i + 1);
			//positionZ = (float)(j + 1);

			positionX = (float)(rand() & resolution) + 1;
			positionZ = (float)(rand() & resolution) + 1;

			my_vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			my_vertices[index].texture = XMFLOAT2(u + increment, v + increment); //MIGHT BE WRONG
			my_vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// Upper left.
			//positionX = (float)(i);
			//positionZ = (float)(j+1);
			positionX = (float)(rand() & resolution) + 1;
			positionZ = (float)(rand() & resolution) + 1;

			my_vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			my_vertices[index].texture = XMFLOAT2(u, v + increment);
			my_vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// Bottom right
			//positionX = (float)(i + 1);
			//positionZ = (float)(j);
			positionX = (float)(rand() & resolution) + 1;
			positionZ = (float)(rand() & resolution) + 1;

			my_vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			my_vertices[index].texture = XMFLOAT2(u + increment, v);
			my_vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// lower left
			//positionX = (float)(i);
			//positionZ = (float)(j);
			positionX = (float)(rand() & resolution) + 1;
			positionZ = (float)(rand() & resolution) + 1;


			my_vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			my_vertices[index].texture = XMFLOAT2(u, v);
			my_vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			//// Upper left
			//positionX = (float)(i);
			//positionZ = (float)(j);

			//vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			//vertices[index].texture = XMFLOAT2(u, v);
			//vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			//indices[index] = index;
			//index++;

		

			//// Upper right.
			//positionX = (float)(i + 1);
			//positionZ = (float)(j + 1);

			//vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			//vertices[index].texture = XMFLOAT2(u + increment, v + increment);
			//vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			//indices[index] = index;
			//index++;

			u += increment;

		}

		u = 0;
		v += increment;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.

	vertexData.pSysMem = my_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the buffers have been created and loaded.
	//delete[] vertices;
	//vertices = 0;
	delete[] indices;
	indices = 0;
}

void MyPlaneMesh::AddVert(VertexType vertex)
{
	vertexCount++;
	my_vertices[vertexCount] = vertex;

}
void MyPlaneMesh::Triangulate(ID3D11Device* device, VertexType vertices[])
{

	indices = new unsigned long[indexCount];

	for (int i = 0; i < vertexCount; i+=2)
	{
		indices[i] = i;
		indices[i + 1] = i + 1;
		indices[i + 2] = i + 2;

	}


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	/*VertexType* verts = new VertexType[vertexCount];

	for (size_t i = 0; i < my_vertices.size(); i++)
	{
	verts[i] = *my_vertices[i];
	}*/
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);


}

void MyPlaneMesh::Init_UsingVerts(ID3D11Device* device, VertexType vertices[])
{
	
	unsigned long* indices;
	int index, i, j;
	float positionX, positionZ, u, v, increment;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Calculate the number of vertices in the terrain mesh.
	vertexCount = (resolution - 1) * (resolution - 1) * 8;


	indexCount = vertexCount;
	//vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];


	index = 0;
	// UV coords.
	u = 0;
	v = 0;
	increment = 1.0f / resolution;

	for (j = 0; j < (resolution - 1); j++)
	{
		for (i = 0; i < (resolution - 1); i++)
		{

			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			//vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			//vertices[index].texture = XMFLOAT2(u + increment, v + increment); //MIGHT BE WRONG
			//vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// Upper left.
			positionX = (float)(i);
			positionZ = (float)(j + 1);

			//vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			//vertices[index].texture = XMFLOAT2(u, v + increment);
			//vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// Bottom right
			positionX = (float)(i + 1);
			positionZ = (float)(j);

			//vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			//vertices[index].texture = XMFLOAT2(u + increment, v);
			//vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// lower left
			positionX = (float)(i);
			positionZ = (float)(j);


			//vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			//vertices[index].texture = XMFLOAT2(u, v);
			//vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			//// Upper left
			//positionX = (float)(i);
			//positionZ = (float)(j);

			//vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			//vertices[index].texture = XMFLOAT2(u, v);
			//vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			//indices[index] = index;
			//index++;



			//// Upper right.
			//positionX = (float)(i + 1);
			//positionZ = (float)(j + 1);

			//vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			//vertices[index].texture = XMFLOAT2(u + increment, v + increment);
			//vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			//indices[index] = index;
			//index++;

			u += increment;

		}

		u = 0;
		v += increment;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	/*VertexType* verts = new VertexType[vertexCount];

	for (size_t i = 0; i < my_vertices.size(); i++)
	{
		verts[i] = *my_vertices[i];
	}*/
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
 	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the buffers have been created and loaded.
	//delete[] vertices;
	//vertices = 0;
	delete[] indices;
	indices = 0;
}

// Override sendData() to change topology type. Control point patch list is required for tessellation.
void MyPlaneMesh::sendData(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the type of primitive that should be rendered from this vertex buffer, in this case control patch for tessellation.
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	//deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);
	
}
