// texture shader.cpp
#include "shadowshader.h"


ShadowShader::ShadowShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"shadow_vs.cso", L"shadow_ps.cso");
}


ShadowShader::~ShadowShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	// Release the light constant buffer.

	// Release the light constant buffer.
	if (lightBuffer2)
	{
		lightBuffer2->Release();
		lightBuffer2 = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void ShadowShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	
	D3D11_BUFFER_DESC lightBufferDesc2;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Required a CLAMPED sampler for sampling the depth map
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleStateClamp);


	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	//lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	//lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//lightBufferDesc.MiscFlags = 0;
	//lightBufferDesc.StructureByteStride = 0;

	//// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	//renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	lightBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc2.ByteWidth = sizeof(LightBufferType2);
	lightBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc2.MiscFlags = 0;
	lightBufferDesc2.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&lightBufferDesc2, NULL, &lightBuffer2);

}


void ShadowShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView*depthMap,  Light* light)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	//LightBufferType* lightPtr;
	LightBufferType2* lightPtr2;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj, tLightViewMatrix, tLightProjectionMatrix, tLightViewMatrix2, tLightProjectionMatrix2;


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);
	tLightViewMatrix = XMMatrixTranspose(light->getViewMatrix());
	tLightProjectionMatrix = XMMatrixTranspose(light->getProjectionMatrix());
	//tLightProjectionMatrix = XMMatrixTranspose(light->getOrthoMatrix());
	//Second Light
	//tLightViewMatrix2 = XMMatrixTranspose(light2->getViewMatrix());
	//tLightProjectionMatrix2 = XMMatrixTranspose(light2->getProjectionMatrix());

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	dataPtr->lightView = tLightViewMatrix;
	dataPtr->lightProjection = tLightProjectionMatrix;
	//Second light
	//dataPtr->lightView2 = tLightViewMatrix2;
	//dataPtr->lightProjection2 = tLightProjectionMatrix2;


	// Unlock the constant buffer.
	deviceContext->Unmap(matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

	////Additional
	//// Send light data to pixel shader
	//deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//lightPtr = (LightBufferType*)mappedResource.pData;
	//lightPtr->ambient = light->getAmbientColour();
	//lightPtr->diffuse = light->getDiffuseColour();
	//lightPtr->ambient2 = light2->getAmbientColour();
	//lightPtr->diffuse2 = light2->getDiffuseColour();
	//deviceContext->Unmap(lightBuffer, 0);
	//bufferNumber = 0;
	//deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer);

	// Send light data to vertex shader
	deviceContext->Map(lightBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr2 = (LightBufferType2*)mappedResource.pData;
	lightPtr2->position = light->getPosition();
	lightPtr2->padding = 1.0f;
	//Second Light
	//->position2 = light2->getPosition();
	//lightPtr2->padding2 = 1.0f;
	deviceContext->Unmap(lightBuffer2, 0);
	bufferNumber = 1;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &lightBuffer2);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	// Set shader depth map texture resource.
	deviceContext->PSSetShaderResources(1, 1, &depthMap);

	//deviceContext->PSSetShaderResources(2, 1, &depthMap2);
}

void ShadowShader::render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetSamplers(1, 1, &sampleStateClamp);

	// Base render function.
	BaseShader::render(deviceContext, indexCount);
}



