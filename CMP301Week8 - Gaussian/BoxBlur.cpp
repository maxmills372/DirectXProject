// texture shader.cpp
#include "BoxBlur.h"


BoxBlur::BoxBlur(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"box_blur_vs.cso", L"box_blur_ps.cso");
}


BoxBlur::~BoxBlur()
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

	//Release base shader components
	BaseShader::~BaseShader();
}


void BoxBlur::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC screenSizeBufferDesc;


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


	//// Setup light buffer
	//// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	//// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	//lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	//lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//lightBufferDesc.MiscFlags = 0;
	//lightBufferDesc.StructureByteStride = 0;

	//// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	//renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);


	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
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



	//SCREEN SIZE BUFFER THINGY
	screenSizeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	screenSizeBufferDesc.ByteWidth = sizeof(ScreenSizeBufferType);
	screenSizeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	screenSizeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	screenSizeBufferDesc.MiscFlags = 0;
	screenSizeBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&screenSizeBufferDesc, NULL, &ScreenSizeBuffer);


}


void BoxBlur::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, Light* light, Light* light_2, int screenWidth, int screenHeight)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	//LightBufferType* lightPtr;
	ScreenSizeBufferType* screenSizePtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;

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
	//lightPtr->diffuse[0] = light->getDiffuseColour();
	//lightPtr->ambient[0] = light->getAmbientColour();
	//lightPtr->position[0] = XMFLOAT4(light->getPosition().x, light->getPosition().y, light->getPosition().z, 1.0f);
	//lightPtr->direction[0] = XMFLOAT4(light->getDirection().x, light->getDirection().y, light->getDirection().z, 1.0f);
	//lightPtr->type[0] = XMFLOAT4(light->getLightType(), 0.0f, 0.0f, 0.0f);

	////lightPtr->specularColour[0] = light->getSpecularColour();
	////lightPtr->specularPower[0] = light->getSpecularPower();

	//lightPtr->diffuse[1] = light_2->getDiffuseColour();
	//lightPtr->ambient[1] = light_2->getAmbientColour();
	//lightPtr->position[1] = XMFLOAT4(light_2->getPosition().x, light_2->getPosition().y, light_2->getPosition().z, 1.0f);
	//lightPtr->direction[1] = XMFLOAT4(light_2->getDirection().x, light_2->getDirection().y, light_2->getDirection().z, 1.0f);
	//lightPtr->type[1] = XMFLOAT4(light_2->getLightType(), 0.0f, 0.0f, 0.0f);
	////lightPtr->padding = 0.0f;

	//deviceContext->Unmap(lightBuffer, 0);
	//bufferNumber = 0;
	//deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer);


	// Send light data to pixel shader
	deviceContext->Map(ScreenSizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	screenSizePtr = (ScreenSizeBufferType*)mappedResource.pData;
	screenSizePtr->screenHeight = screenHeight;
	screenSizePtr->screenWidth = screenWidth;
	screenSizePtr->padding = XMFLOAT2(0.0f,0.0f);

	deviceContext->Unmap(ScreenSizeBuffer, 0);
	bufferNumber = 1;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &ScreenSizeBuffer);


	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

}

void BoxBlur::render(ID3D11DeviceContext* deviceContext, int indexCount)
{

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &sampleState);

	// Base render function.
	BaseShader::render(deviceContext, indexCount);
}



