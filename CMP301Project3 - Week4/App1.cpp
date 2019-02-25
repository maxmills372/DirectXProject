// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	//BaseApplication::BaseApplication();
	mesh = nullptr;
	colourShader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create Mesh object
	mesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());

	colourShader = new ColourShader(renderer->getDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);

	// Create light object and set diffuse colour and direction
	my_light = new Light;
	my_light->setDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	my_light->setAmbientColour(1.0f, 1.0f, 1.0f, 1.0f);
	my_light->setPosition(-50.0f, 0.0f, 90.0f);

	// Create second light 
	light_2 = new Light;
	light_2->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	light_2->setAmbientColour(1.0f, 1.0f, 1.0f, 1.0f);
	light_2->setPosition(50.0f, 0.0f, 90.0f);

	// Create second light 
	light_3 = new Light;
	light_3->setDiffuseColour(1.0f, 1.0f, 0.0f, 1.0f);
	light_3->setAmbientColour(1.0f, 1.0f, 1.0f, 1.0f);
	light_3->setPosition(0.0f, 50.0f, 90.0f);
	// Create second light 
	light_4 = new Light;
	light_4->setDiffuseColour(1.0f, 0.0f, 1.0f, 1.0f);
	light_4->setAmbientColour(1.0f, 1.0f, 1.0f, 1.0f);
	light_4->setPosition(0.0f, -50.0f, 90.0f);

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
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, matrix1, matrix2;

	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	//translate by x
	matrix1 = XMMatrixTranslation(0.0f, 0.0f, 100.0f);

	//rotate world by x
	matrix2 = XMMatrixScaling(100.0f, 100.0f, 1.0f);
	
	//multiply both matrices
	worldMatrix = XMMatrixMultiply(matrix1, matrix2);


	//// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	//colourShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	//colourShader->render(renderer->getDeviceContext(), mesh->getIndexCount());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), my_light, light_2, light_3,light_4,camera);
	//// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), mesh->getIndexCount());


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

	// Render UI
	ImGui::Render();
}

