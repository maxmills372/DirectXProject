// Lab1.cpp
// Lab 1 example, simple textured quad
#include "App1.h"
#include "SquareMesh.h"

App1::App1()
{
	mesh = nullptr;
	textureShader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create Mesh object, load required textures and load shaders.
	mesh = new SquareMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture("greg", L"../res/greg.jpg");
	
	// Second mesh, load texture and load shaders
	mesh_2 = new SquareMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture("tim", L"../res/greg2.jpg");

	noise_plane_mesh = new NoisePlaneMesh(renderer->getDevice(), renderer->getDeviceContext(),20);

	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	div = 0;
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
	if (mesh_2)
	{
		delete mesh_2;
		mesh_2 = 0;
	}

	if (textureShader)
	{
		delete textureShader;
		textureShader = 0;
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
	XMMATRIX worldMatrix, worldMatrix2, viewMatrix, projectionMatrix;
	rotation_value += 0.1f;

	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// Rotate every frame
	//worldMatrix = XMMatrixRotationRollPitchYaw(0.0, 0.0, rotation_value);

	//// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("greg"), textureMgr->getTexture("tim"),div);
	//// Render object (combination of mesh geometry and shader process
	textureShader->render(renderer->getDeviceContext(), mesh->getIndexCount());
	
	// Translate by 2
	worldMatrix = XMMatrixTranslation(1.0, 0.0, 0.0);

	//worldMatrix = worldMatrix2;

	//// Render second mesh
	plane_mesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("tim"), textureMgr->getTexture("greg"),div);
	//// Render object (combination of mesh geometry and shader process
	textureShader->render(renderer->getDeviceContext(), plane_mesh->getIndexCount());



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
	ImGui::SliderFloat("division", &div,0.0f,1.0f);

	// Render UI
	ImGui::Render();
}

