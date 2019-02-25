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
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, 0, 0);
	orthoMeshDown = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), SHADOWMAP_WIDTH/2, SHADOWMAP_HEIGHT/2, 0, 0);


	spherePos = new XMFLOAT3(-5.0f, 2.0f, 0.0f);
	movingLightPos = new XMFLOAT3(-5.0f, 8.0f, -5.0f);

	renderTexture = new RenderTexture(renderer->getDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);
	renderTextureDown = new RenderTexture(renderer->getDevice(), SHADOWMAP_WIDTH / 2, SHADOWMAP_HEIGHT / 2, SCREEN_NEAR, SCREEN_DEPTH);
	renderTextureHorizontal = new RenderTexture(renderer->getDevice(), SHADOWMAP_WIDTH/2, SHADOWMAP_HEIGHT/2, SCREEN_NEAR, SCREEN_DEPTH);
	renderTextureVertical = new RenderTexture(renderer->getDevice(), SHADOWMAP_WIDTH/2, SHADOWMAP_HEIGHT/2, SCREEN_NEAR, SCREEN_DEPTH);
	renderTextureUp = new RenderTexture(renderer->getDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);

	blackWhiteRenderTexture = new RenderTexture(renderer->getDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);

	colourShader = new ColourShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);
	softShadowShader = new SoftShadowShader(renderer->getDevice(), hwnd);
	hBlurShader = new HorizontalBlurShader(renderer->getDevice(), hwnd);
	vBlurShader = new VerticalBlurShader(renderer->getDevice(), hwnd);
	textureShader = new TextureShader(renderer->getDevice(), hwnd);

	light = new Light;
	light->setLightType(1);
	light->setDiffuseColour(0.9f, 0.1f, 0.1f, 1.0f);
	light->setAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	//light->setDirection(1.8f, -1.0f, 0.0f);

	//light->setPosition(camera->getPosition().x, camera->getPosition().y, -camera->getPosition().z);
	light->setPosition(0.0f, 8.0f, -10.0f);
	light->setLookAt(0.0f, 0.0f, 0.0f);

	light->generateViewMatrix();
	light->generateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);

	sW = screenWidth;
	sH = screenHeight;


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

void App1::renderToTexture()
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
	light->generateViewMatrix();
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
void App1::renderBlackAndWhiteShadows()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix;

	// Set the render target to be the render to texture.
	renderTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();
	//light->setPosition(camera->getPosition().x, camera->getPosition().y, -camera->getPosition().z);

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	light->generateViewMatrix();
	//light->generateProjectionMatrix(1.0f, 100.0f);

	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	lightViewMatrix = light->getViewMatrix();
	lightProjectionMatrix = light->getProjectionMatrix();
	worldMatrix = renderer->getWorldMatrix();

	//// Send geometry data (from mesh)
	model->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), renderTexture->getShaderResourceView(), light);
	//// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), model->getIndexCount());

	worldMatrix = XMMatrixTranslation(spherePos->x, spherePos->y, spherePos->z);

	//// Send geometry data (from mesh)
	sphereMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix,textureMgr->getTexture("brick"), renderTexture->getShaderResourceView(), light);
	//// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());


	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(-50.0f, -1.0f, -25.0f);

	//// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix,textureMgr->getTexture("brick"), renderTexture->getShaderResourceView(),  light);
	//// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), mesh->getIndexCount());


	//// Present the rendered scene to the screen.
	//renderer->endScene();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::DownSampleTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoViewMatrix, orthoMatrix;

	// Set the render target to be the render to texture.
	renderTextureDown->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTextureDown->clearRenderTarget(renderer->getDeviceContext(), 0.39f, 0.58f, 0.92f, 1.0f);

	camera->update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();

	// To render ortho mesh
	// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	// ortho matrix for 2D rendering
	orthoMatrix = renderer->getOrthoMatrix();
	orthoViewMatrix = camera->getOrthoViewMatrix();

	orthoMeshDown->sendData(renderer->getDeviceContext());

	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,
		blackWhiteRenderTexture->getShaderResourceView());// sW, sH);

	textureShader->render(renderer->getDeviceContext(), orthoMeshDown->getIndexCount());

	renderer->setZBuffer(true);

}
void App1::RenderHorizontalBlur()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoViewMatrix, orthoMatrix;;

	// Set the render target to be the render to texture.
	renderTextureHorizontal->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTextureHorizontal->clearRenderTarget(renderer->getDeviceContext(), 0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	//blurDirection = XMFLOAT2(camera->getPosition().x * 5000.0f, camera->getPosition().y * 5000.0f);


	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();

	// To render ortho mesh
	// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	// ortho matrix for 2D rendering
	orthoMatrix = renderer->getOrthoMatrix();
	orthoViewMatrix = camera->getOrthoViewMatrix();

	orthoMesh->sendData(renderer->getDeviceContext());

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	hBlurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,
		renderTextureDown->getShaderResourceView(), sW);

	// Render object (combination of mesh geometry and shader process
	hBlurShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

	renderer->setZBuffer(true);

}
void App1::RenderVerticalBlur()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoViewMatrix, orthoMatrix;;

	// Set the render target to be the render to texture.
	renderTextureVertical->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTextureVertical->clearRenderTarget(renderer->getDeviceContext(), 0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();

	// To render ortho mesh
	// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	// ortho matrix for 2D rendering
	orthoMatrix = renderer->getOrthoMatrix();
	orthoViewMatrix = camera->getOrthoViewMatrix();

	orthoMesh->sendData(renderer->getDeviceContext());

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	vBlurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,
		renderTextureHorizontal->getShaderResourceView(), sH);

	// Render object (combination of mesh geometry and shader process
	vBlurShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

	renderer->setZBuffer(true);

}
void App1::UpSampleTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoViewMatrix, orthoMatrix;

	// Set the render target to be the render to texture.
	renderTextureUp->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTextureUp->clearRenderTarget(renderer->getDeviceContext(), 0.39f, 0.58f, 0.92f, 1.0f);

	camera->update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// To render ortho mesh
	// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	// ortho matrix for 2D rendering
	orthoMatrix = renderer->getOrthoMatrix();
	orthoViewMatrix = camera->getOrthoViewMatrix();

	orthoMesh->sendData(renderer->getDeviceContext());

	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,
		renderTextureVertical->getShaderResourceView());// sW, sH);

	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

	renderer->setZBuffer(true);
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
	//viewMatrix2 = light_2->getViewMatrix();
	//projectionMatrix2 = light_2->getProjectionMatrix();

	//// Send geometry data (from mesh)
	model->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	softShadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), renderTextureUp->getShaderResourceView(), light);
	//// Render object (combination of mesh geometry and shader process
	softShadowShader->render(renderer->getDeviceContext(), model->getIndexCount());

	worldMatrix = XMMatrixTranslation(spherePos->x, spherePos->y, spherePos->z);

	//// Send geometry data (from mesh)
	sphereMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	softShadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), renderTextureUp->getShaderResourceView(), light);
	//// Render object (combination of mesh geometry and shader process
	softShadowShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(-50.0f, -1.0f, -25.0f);

	//// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	softShadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,textureMgr->getTexture("brick"), renderTextureUp->getShaderResourceView(), light);
	//// Render object (combination of mesh geometry and shader process
	softShadowShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	
	
}

bool App1::render()
{
	camera->update();

	//RenderToTexture
	renderToTexture();
	renderBlackAndWhiteShadows();
	DownSampleTexture();
	RenderHorizontalBlur();
	RenderVerticalBlur();
	UpSampleTexture();
	
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
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	// Build UI
	ImGui::SliderFloat("Pos x:", &movingLightPos->x,-100,100);
	ImGui::SliderFloat("Pos y:", &movingLightPos->y, -100, 100);
	ImGui::SliderFloat("Pos z:", &movingLightPos->z, -100, 100);

	// Render UI
	ImGui::Render();
}

