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
	mesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext(),2);
	cut_plane = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 20);


	colourShader = new ColourShader(renderer->getDevice(), hwnd);

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
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, tranform_Matrix, rotate_Matrix;

	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();
	renderer->setWireframeMode(true);
	

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	//// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	colourShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	colourShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	tranform_Matrix = XMMatrixTranslation(0, 0, 10.f);
	rotate_Matrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(-90.f),0.0f,0.0f);
	worldMatrix = XMMatrixMultiply(rotate_Matrix, tranform_Matrix);

	//// Send geometry data (from mesh)
	cut_plane->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	colourShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	colourShader->render(renderer->getDeviceContext(), cut_plane->getIndexCount());


	//mesh
	


	renderer->setWireframeMode(false);

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
	//ImGui::Checkbox("Test ", test_);

	// Render UI
	ImGui::Render();
}

