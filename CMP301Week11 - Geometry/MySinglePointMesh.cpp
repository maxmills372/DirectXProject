// Point mesh.
// For geometry shader demonstration.
// Note sendData() override.
#include "MySinglePointMesh.h"

// Initialise buffers and load texture.
MySinglePointMesh::MySinglePointMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	initBuffers(device);
}

// Release resources.
MySinglePointMesh::~MySinglePointMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

// Generate point mesh. Simple triangle.
void MySinglePointMesh::initBuffers(ID3D11Device* device)
{
	//VertexType* vertices;
	//unsigned long* indices;
	//D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	//D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexCount = 14;
	indexCount = 14;


	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	for (int i = 0; i < vertexCount; i++)
	{
		// Load the vertex array with data.
		//vertices[i].position = XMFLOAT3(rand() % 20, rand() % 20, 0.0f);

		vertices[i].position = XMFLOAT3(i, 0.0f, 0.0f);
		vertices[i].texture = XMFLOAT2(0.0f, 0.0f);
		vertices[i].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);


		// Load the index array with data.
		indices[i] = i;  // Top/
	}
	

    // Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
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

	//// Release the arrays now that the vertex and index buffers have been created and loaded.
	//delete[] vertices;
	//vertices = 0;

	//delete[] indices;
	//indices = 0;
}

// Override sendData()
// Change in primitive topology (pointlist instead of trianglelist) for geometry shader use.
void MySinglePointMesh::sendData(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void MySinglePointMesh::RandomisePosition(ID3D11Device* device)
{
	/*for (int i = 0; i < vertexCount; i++)
	{
		// Load the vertex array with data.
		vertices[i].position = XMFLOAT3(i, 0.0f, 0.0f);// rand() % 20, rand() % 20, 0.0f);
	}*/
	float test = 0.2f;
	float curve = 1.0f;

	//curve = (rand() % 2) *(XM_PI * 2);

	for (int i = 0; i < vertexCount; i++)
	{

		// Load the vertex array with data.
		vertices[i].position = XMFLOAT3((cos(curve) * 5)+test,rand()% 2, (sin(curve)*5)+test);// rand() % 20, rand() % 20, 0.0f);
		test *= 0.2f;
		
		if (curve < 50)
		{
			curve++;
		}
		else
		{
			curve--;
		}
		
	}


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
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
