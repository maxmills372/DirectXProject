// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	//BaseApplication::BaseApplication();
	mesh = nullptr;
	colourShader = nullptr;
	depthShader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	textureMgr->loadTexture("brick", L"../res/brick1.dds");

	model = new Model(renderer->getDevice(), renderer->getDeviceContext(), "../assets/TieFighter2.obj");

	// Create Mesh object
	mesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());

	spherePos = new XMFLOAT3(-5.0f, 2.0f, 0.0f);
	movingLightPos = new XMFLOAT3(-5.0f, 8.0f, -5.0f);

	renderTexture = new RenderTexture(renderer->getDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);
	renderTexture2 = new RenderTexture(renderer->getDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);

	colourShader = new ColourShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);

	light = new Light;
	light->setLightType(2);
	light->setDiffuseColour(0.9f, 0.1f, 0.1f, 1.0f);
	light->setAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	//light->setDirection(1.8f, -1.0f, 0.0f);

	//light->setPosition(camera->getPosition().x, camera->getPosition().y, -camera->getPosition().z);
	light->setPosition(0.0f, 8.0f, -10.0f);
	light->setLookAt(0.0f, 0.0f, 0.0f);

	light->generateViewMatrix();
	light->generateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);

	light_2 = new Light;
	light_2->setLightType(2);
	light_2->setDiffuseColour(0.0f, 0.9f, 0.1f, 1.0f);
	light_2->setAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	//light->setDirection(1.8f, -1.0f, 0.0f);

	//light->setPosition(camera->getPosition().x, camera->getPosition().y, -camera->getPosition().z);
	light_2->setPosition(movingLightPos->x, movingLightPos->y, movingLightPos->z);
	light_2->setLookAt(0.0f, 0.0f, 0.0f);

	light_2->generateViewMatrix();
	light_2->generateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);

	light_3 = new Light;
	light_3->setLightType(0);
	light_3->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light_3->setAmbientColour(0.8f, 0.8f, 0.8f, 1.0f);
	light_3->setDirection(1.8f, 1.8f, 0.0f);

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (mesh)
	{
		delete mesh;
		mesh = 0;
	}

	if (colourShader)
	{
		delete colourShader;
		colourShader = 0;
	}

	if (depthShader)
	{
		delete depthShader;
		depthShader = 0;
	}
}


bool App1::frame()
{
	bool result;

	if (input->isKeyDown(54)) // 6 key
	{
		light_2->setPosition(light_2->getPosition().x + 0.1f, light_2->getPosition().y, light_2->getPosition().z);
	}

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

void App1::renderToTexture1()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix;

	//// Clear the scene. (default blue colour)
	//renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
	
	// Set the render target to be the render to texture.
	renderTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();
	//light->setPosition(camera->getPosition().x, camera->getPosition().y, -camera->getPosition().z);

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
//	light->generateViewMatrix();
	//light->generateProjectionMatrix(1.0f, 100.0f);

	lightViewMatrix = light->getViewMatrix();
	lightProjectionMatrix = light->getProjectionMatrix();
	worldMatrix = renderer->getWorldMatrix();

	//// Send geometry data (from mesh)
	model->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), model->getIndexCount());

	worldMatrix = XMMatrixTranslation(spherePos->x, spherePos->y, spherePos->z);

	//// Send geometry data (from mesh)
	sphereMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());


	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(-50.0f, -1.0f, -25.0f);

	//// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());


	//// Present the rendered scene to the screen.
	//renderer->endScene();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}
void App1::renderToTexture2()
{
	XMMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix, translateMatrix;
	float posX, posY, posZ;
	bool result;


	// Set the render target to be the render to texture.
	renderTexture2->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTexture2->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

	light_2->setPosition(movingLightPos->x, movingLightPos->y, movingLightPos->z);


	// Generate the light view matrix based on the light's position.
	light_2->generateViewMatrix();

	// Get the view and orthographic matrices from the light object.
	lightViewMatrix = light_2->getViewMatrix();
	lightProjectionMatrix = light_2->getProjectionMatrix();
	worldMatrix = renderer->getWorldMatrix();


	// Setup the translation matrix for the cube model.
	//m_CubeModel->GetPosition(posX, posY, posZ);
	//D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	//// Send geometry data (from mesh)
	model->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(),  worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), model->getIndexCount());

	worldMatrix = XMMatrixTranslation(spherePos->x, spherePos->y, spherePos->z);

	//// Send geometry data (from mesh)
	sphereMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());


	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(-50.0f, -1.0f, -25.0f);

	//// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());


	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();

	// Reset the viewport back to the original.
	renderer->resetViewport();

	
}
void App1::renderToTexture(Light* light, RenderTexture* renderTexture)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix;

	//// Clear the scene. (default blue colour)
	//renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Set the render target to be the render to texture.
	renderTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();
	//light->setPosition(camera->getPosition().x, camera->getPosition().y, -camera->getPosition().z);

	light_2->setPosition(movingLightPos->x, movingLightPos->y, movingLightPos->z);

	light_2->generateViewMatrix();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	//	light->generateViewMatrix();
	//light->generateProjectionMatrix(1.0f, 100.0f);

	lightViewMatrix = light->getViewMatrix();
	lightProjectionMatrix = light->getProjectionMatrix();
	worldMatrix = renderer->getWorldMatrix();

	//// Send geometry data (from mesh)
	model->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), model->getIndexCount());

	worldMatrix = XMMatrixTranslation(spherePos->x, spherePos->y, spherePos->z);

	//// Send geometry data (from mesh)
	sphereMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());


	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(-50.0f, -1.0f, -25.0f);

	//// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());


	//// Present the rendered scene to the screen.
	//renderer->endScene();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::renderScene()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, viewMatrix2, projectionMatrix2;

	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();
	viewMatrix2 = light_2->getViewMatrix();
	projectionMatrix2 = light_2->getProjectionMatrix();

	//// Send geometry data (from mesh)
	model->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), renderTexture->getShaderResourceView(), renderTexture2->getShaderResourceView(), light, light_2);
	//// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), model->getIndexCount());

	worldMatrix = XMMatrixTranslation(spherePos->x, spherePos->y, spherePos->z);

	//// Send geometry data (from mesh)
	sphereMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), renderTexture->getShaderResourceView(), renderTexture2->getShaderResourceView(), light, light_2);
	//// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(-50.0f, -1.0f, -25.0f);

	//// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,textureMgr->getTexture("brick"), renderTexture->getShaderResourceView(), renderTexture2->getShaderResourceView(), light, light_2);
	//// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	
	
}

bool App1::render()
{
	camera->update();

	//RenderToTexture
	renderToTexture(light,renderTexture);
	renderToTexture(light_2, renderTexture2);

	//RenderToTexture
	//renderToTexture1();

	//RenderToTexture
	//renderToTexture2();

	//RenderScene
	renderScene();

	// Render GUI
	gui();

	//// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off on Geometry shader
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	// Build UI
	ImGui::SliderFloat("Pos x:", &movingLightPos->x,-100,100);
	ImGui::SliderFloat("Pos y:", &movingLightPos->y, -100, 100);
	ImGui::SliderFloat("Pos z:", &movingLightPos->z, -100, 100);

	// Render UI
	ImGui::Render();
}

