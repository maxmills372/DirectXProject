// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	//BaseApplication::BaseApplication();
	//mesh = nullptr;
	mesh_2 = nullptr;
	boxBlurShader = nullptr;
	lightShader = nullptr;
	manipShader = nullptr;
	my_light = nullptr;
	light_2 = nullptr;
	textureMgr = nullptr;
	renderTexture = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);
	
	// Create Mesh object
	mesh_2 = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	plane_mesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(),screenWidth, screenHeight,0,0);
	orthoMeshDown = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight, 0, 0);


	textureMgr->loadTexture("my_texture", L"../res/greg2.jpg");

	// RENDER TEXTURE
	// RenderTexture, OrthoMesh and shader set for different renderTarget
	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	renderTextureDown = new RenderTexture(renderer->getDevice(), screenWidth/2, screenHeight/2, SCREEN_NEAR, SCREEN_DEPTH);
	renderTextureHorizontal = new RenderTexture(renderer->getDevice(), screenWidth , screenHeight , SCREEN_NEAR, SCREEN_DEPTH);
	renderTextureVertical = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight , SCREEN_NEAR, SCREEN_DEPTH);
	renderTextureUp = new RenderTexture(renderer->getDevice(), screenWidth , screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	//Shaders 
	manipShader = new ManipulationShader(renderer->getDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	boxBlurShader = new BoxBlur(renderer->getDevice(), hwnd);
	hBlurShader = new HorizontalBlurShader(renderer->getDevice(), hwnd);
	vBlurShader = new VerticalBlurShader(renderer->getDevice(), hwnd);

	// Create light object and set diffuse colour and direction
	my_light = new Light;
	my_light->setLightType(0); // 0 = DIRECTIONAL
	my_light->setDiffuseColour(0.0f, 0.0f, 0.0f, 1.0f);
	my_light->setAmbientColour(0.8f, 0.8f, 0.8f, 1.0f);
	my_light->setDirection(1.8f, 1.8f, 0.0f);

	// Create second light 
	light_2 = new Light;
	light_2->setLightType(1); // 1 = POINT, 2 = SPOT
	light_2->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	light_2->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	light_2->setPosition(3.0f, 3.0f, 5.0f);

	counter = 0.0f;
	height = 0.05f;
	length = 0.12f;
	speed = 2.0f;
	rotate = 0.0f;
	sW = screenWidth;
	sH = screenHeight;

//	blurDirection = XMFLOAT3(30.0f, 30.0f, 1.0f);

	directionalBlur = false;
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (lightShader)
	{
		delete lightShader;
		lightShader = 0;
	}
	if (my_light)
	{
		delete my_light;
		my_light = 0;
	}
	if (light_2)
	{
		delete light_2;
		light_2 = 0;
	}
}

bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

	//keyboard input
	//height
	if (input->isKeyDown(79)) // o key
	{
		/*int sum;
		sum = 0.5f - (weights[0] + weights[1] + weights[2] + weights[3] + weights[4]);

		
		if ((sum + (weights[0] + weights[1] + weights[2] + weights[3] + weights[4])) != 0.5f)
		{
			weights[0] += 0.001f;
		}*/
		height += timer->getTime();
	}
	if (input->isKeyDown(80)) // p key
	{
		height -= timer->getTime();
	}
	//length
	if (input->isKeyDown(85)) // u key
	{
		length += timer->getTime();
	}
	if (input->isKeyDown(73)) // i key
	{
		length -= timer->getTime();
	}

	//counter - speed
	if (input->isKeyDown(75)) // k key
	{
		speed += timer->getTime();
	}
	if (input->isKeyDown(76)) // l key
	{
		speed -= timer->getTime();
	}



	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{	
	// VEREX MANIPULATION varibles
	counter += speed * timer->getTime();

	//rotation variable
	rotate += 0.01;

	if (directionalBlur)
	{
		//Render to texture first
		RenderToTexture();

		//Down scale
		DownSampleTexture();

		//Horizontal Blur
		RenderHorizontalBlur();

		//Vertical Blur
		//RenderVerticalBlur();

		//Up scale
		UpSampleTexture();

		//then render the actual scene
		RenderFinalBlur();
	}
	else
	{
		RenderScene();
	}

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}

void App1::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Set the render target to be the render to texture.
	renderTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();


	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	mesh_2->sendData(renderer->getDeviceContext());

	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("my_texture"), my_light, light_2,camera);// my_light, light_2, sW, sH);// , camera, counter, height, length);

	// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), mesh_2->getIndexCount());

	// Do this before you render the actual scene 
	//renderer->setBackBufferRenderTarget();
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

	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,
		renderTexture->getShaderResourceView(), my_light, light_2,camera);// sW, sH);

	lightShader->render(renderer->getDeviceContext(), orthoMeshDown->getIndexCount());

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

	blurDirection = XMFLOAT2(camera->getPosition().x * 5000.0f, camera->getPosition().y * 5000.0f);


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
		renderTextureDown->getShaderResourceView(), sW, blurDirection);

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

	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,
		renderTextureHorizontal->getShaderResourceView(), my_light, light_2,camera);// sW, sH);

	lightShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

	renderer->setZBuffer(true);
}


void App1::RenderFinalBlur()
{
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoViewMatrix, orthoMatrix;
	
	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	//ROTATE
	//worldMatrix = XMMatrixRotationRollPitchYaw(0.0f, rotate, 0.0f);

	//// Send geometry data (from mesh)
	//mesh_2->sendData(renderer->getDeviceContext());

	//// Set shader parameters (matrices and texture)
	//textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix,
	//	projectionMatrix, renderTextureUp->getShaderResourceView(), my_light, light_2);//, camera, counter, height, length);
	//																			   // Render object (combination of mesh geometry and shader process
	//textureShader->render(renderer->getDeviceContext(), mesh_2->getIndexCount());


	// To render ortho mesh
	// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	// ortho matrix for 2D rendering
	orthoMatrix = renderer->getOrthoMatrix();
	orthoViewMatrix = camera->getOrthoViewMatrix();

	//Reset rotation to stop the little screen also rotate
	//worldMatrix = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);

	orthoMesh->sendData(renderer->getDeviceContext()); 

	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,
		renderTextureUp->getShaderResourceView(), my_light, light_2,camera);// sW, sH);

	lightShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

	renderer->setZBuffer(true);

	// Present the rendered scene to the screen.
	//renderer->endScene();
}

void App1::RenderScene()
{

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;


	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);


	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();


	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	mesh_2->sendData(renderer->getDeviceContext());

	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("my_texture"), my_light, light_2, camera);// my_light, light_2, sW, sH);// , camera, counter, height, length);

	// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), mesh_2->getIndexCount());


	plane_mesh->sendData(renderer->getDeviceContext());

	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("defualt"), my_light, light_2, camera);// my_light, light_2, sW, sH);// , camera, counter, height, length);

																		 // Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), plane_mesh->getIndexCount());

}
/*
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoViewMatrix, orthoMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	//ROTATE
	worldMatrix = XMMatrixRotationRollPitchYaw(0.0f, rotate, 0.0f);

	// Send geometry data (from mesh)
	mesh_2->sendData(renderer->getDeviceContext());
	// Set shader parameters (matrices and texture)

	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix,
		projectionMatrix, textureMgr->getTexture("my_texture"), my_light, light_2);//, camera, counter, height, length);
	// Render object (combination of mesh geometry and shader process
	textureShader->render(renderer->getDeviceContext(), mesh_2->getIndexCount());

	// To render ortho mesh
	// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	// ortho matrix for 2D rendering
	orthoMatrix = renderer->getOrthoMatrix();
	orthoViewMatrix = camera->getOrthoViewMatrix();

	//Reset rotation to stop the little screen also rotate
	worldMatrix = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);

	orthoMesh->sendData(renderer->getDeviceContext());

	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,
		renderTexture->getShaderResourceView(),my_light,light_2);

	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

	renderer->setZBuffer(true);
	// Present the rendered scene to the screen.
	renderer->endScene();
	*/

void App1::gui()
{
	// Force turn off on Geometry shader
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	
	//display height and length
	ImGui::Text("Height: %.2f", height);
	ImGui::Text("Length: %.2f", length);
	ImGui::Text("Speed: %.2f", speed);

	ImGui::Text("CameraPos X: %f Y: %f Z: %f",camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

	//ImGui::SliderFloat("x_dir", &blurDirection.x,-100000,100000);
	//ImGui::SliderFloat("y_dir", &blurDirection.y, -100000, 100000);

	ImGui::Checkbox("Directional Blur", &directionalBlur);


	// Render UI
	ImGui::Render();
}

