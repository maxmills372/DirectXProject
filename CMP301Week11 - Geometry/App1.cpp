// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	//BaseApplication::BaseApplication();
	pointMesh = nullptr;
	
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	//textureMgr->loadTexture("tex1", L"../res/lava.jpg");

	// Create Mesh object
	pointMesh = new MySinglePointMesh(renderer->getDevice(), renderer->getDeviceContext());
	triMesh = new TriangleMesh(renderer->getDevice(), renderer->getDeviceContext());
	myPlaneMesh = new MyPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 20);
	model = new Model(renderer->getDevice(), renderer->getDeviceContext(), "../res/Sphere.obj");
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());

	geoShader = new GeometryShader(renderer->getDevice(), hwnd);
	colourShader = new ColourShader(renderer->getDevice(), hwnd);

	wireframe = false;
	alpha = false;

	// Create light object and set diffuse colour and direction
	my_light = new Light;
	my_light->setLightType(0); // 0 = DIRECTIONAL

	my_light->setDiffuseColour(0.0f, 0.0f, 0.0f, 1.0f);
	my_light->setAmbientColour(0.8f, 0.8f, 0.8f, 1.0f);
	my_light->setDirection(1.8f, -1.0f, 0.0f);
	//my_light->setPosition(5.0f, 0.0f, -1.0f);


	// Create second light 
	light_2 = new Light;
	light_2->setLightType(1); // 1 = POINT, 2 = SPOT
	
	light_2->setDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	light_2->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	//light_2->setDirection(0.0f, 0.0f, 1.0f);
	light_2->setPosition(0.0f, 1.0f, -5.0f);


	i = 0;

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (pointMesh)
	{
		delete pointMesh;
		pointMesh = 0;
	}

	if (geoShader)
	{
		delete geoShader;
		geoShader = 0;
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
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix;
	XMFLOAT3 cameraPosition, modelPosition;
	double angle;
	float rotation;

	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	renderer->setWireframeMode(wireframe);

	// Get the position of the camera.
	cameraPosition = camera->getPosition();

	// Set the position of the billboard model.
	modelPosition.x = 2.0f;
	modelPosition.y = 11.5f;
	modelPosition.z = 0.0f;

	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
//	angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / XM_PI);

	// Convert rotation into radians.
	//rotation = (float)angle * 0.0174532925f;

	// Setup the rotation the billboard at the origin using the world matrix.
	//worldMatrix = XMMatrixRotationRollPitchYaw(0,rotation,0);

	// Setup the translation matrix from the billboard model.
	//translateMatrix = XMMatrixTranslation(modelPosition.x, modelPosition.y, modelPosition.z);

	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	//XMMatrixMultiply(worldMatrix, translateMatrix);

	// Setup the translation matrix from the billboard model.
	//worldMatrix = XMMatrixTranslation(modelPosition.x, modelPosition.y, modelPosition.z);

	/*//// Send geometry data (from mesh)
	myPlaneMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	geoShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("tex1"), my_light, light_2, alpha, camera, timer->getActualTime());
	//// Render object (combination of mesh geometry and shader process
	geoShader->render(renderer->getDeviceContext(), myPlaneMesh->getIndexCount());
	*/
	worldMatrix = XMMatrixTranslation(-10.f, 0,0);

	//// Send geometry data (from mesh)
	model->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	geoShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("tex1") , my_light, light_2, alpha, camera, timer->getActualTime());
	//// Render object (combination of mesh geometry and shader process
	geoShader->render(renderer->getDeviceContext(), model->getIndexCount());

	/*i++;
	if (i == 50)
	{
		pointMesh->RandomisePosition(renderer->getDevice());
		i = 0;
	}*/
	worldMatrix = XMMatrixTranslation(10.f, 0, 0);

	//// Send geometry data (from mesh)
	pointMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	geoShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,textureMgr->getTexture("tex1"), my_light,  light_2, alpha, camera, 0.f);
	//// Render object (combination of mesh geometry and shader process
	geoShader->render(renderer->getDeviceContext(), pointMesh->getIndexCount());

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
	ImGui::Text("FPS: %.2f", timer->getActualTime());
	ImGui::Checkbox("Wireframe Mode: ", &wireframe);
	ImGui::Checkbox("Aplha Blending: ", &alpha);
	
	// Render UI
	ImGui::Render();
}

