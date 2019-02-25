// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	//BaseApplication::BaseApplication();
	mesh = nullptr;
	mesh_2 = nullptr;
	colourShader = nullptr;
	lightShader = nullptr;
	manipShader = nullptr;
	my_light = nullptr;
	light_2 = nullptr;
	textureMgr = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create Mesh object
	mesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture("height", L"../res/height.png");

	mesh_2 = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext(),200);
	textureMgr->loadTexture("my_texture", L"../res/test.jpg");


	colourShader = new ColourShader(renderer->getDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);
	manipShader = new ManipulationShader(renderer->getDevice(), hwnd);
	

	// Create light object and set diffuse colour and direction
	my_light = new Light;
	my_light->setLightType(0); // 0 = DIRECTIONAL

	my_light->setDiffuseColour(0.0f, 0.0f, 0.0f, 1.0f);
	my_light->setAmbientColour(0.8f, 0.8f, 0.8f, 1.0f);
	my_light->setDirection(1.8f, -1.0f, 0.0f);
	//my_light->setPosition(5.0f, 0.0f, -1.0f);

	
	// Create second light 
	//light_2 = new Light;
	//light_2->setLightType(2); // 1 = POINT, 2 = SPOT

	//light_2->setDiffuseColour(1.0f, 1.0f, 0.0f, 1.0f);
	//light_2->setAmbientColour(0.0f, 0.0f, 0.4f, 1.0f);
	//light_2->setDirection(0.0f, 0.0f, 1.0f);
	////light_2->setPosition(15.0f, -10.0f, 25.0f);
	//

	// Create second light 
	light_2 = new Light;
	//light_2->setLightType(1); // 1 = POINT, 2 = SPOT

	//light_2->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	//light_2->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	////light_2->setDirection(0.0f, 0.0f, 1.0f);
	//light_2->setPosition(0.0f, 10.0f, 25.0f);
	
	counter = 0.0f;
	height = 0.05f;
	length = 0.12f;
	speed = 2.0f;
	heightMapSize = 0.49f;
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

	//keyboard input
	//height
	if (input->isKeyDown(79)) // o key
	{
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
	 
	if (input->isKeyDown(49)) // 1 key
	{
		heightMapSize += timer->getTime() * 10;
	}
	if (input->isKeyDown(50)) // 2 key
	{
		heightMapSize -= timer->getTime() * 10;
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


	//move spotlight with the camera
	//light_2->setPosition(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

	//light_2->setDirection(light_2->getPosition().x, light_2->getPosition().y, light_2->getPosition().z);
	

	//translate by x
	//worldMatrix = XMMatrixTranslation(-50.0f, -5.0f, 0.0f);


	// rotate world by x
	// matrix2 = XMMatrixScaling(5.0f, 5.0f, 5.0f);

	//worldMatrix = XMMatrixRotationRollPitchYaw(counter,0.0f,0.0f);

	// multiply both matrices
	// worldMatrix = XMMatrixMultiply(matrix2, matrix1);

	// VEREX MANIPULATION varibles
	counter += speed * timer->getTime();
	
	float vec[2];
	vec[0] = 1.0f;
	vec[1] = 1.0f;

	//// Send geometry data (from mesh)
	mesh_2->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	//colourShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	//colourShader->render(renderer->getDeviceContext(), mesh->getIndexCount());
	manipShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("height"), my_light, light_2, camera,counter,height, length, heightMapSize, perlin.noise2(vec) * 50);
	//// Render object (combination of mesh geometry and shader process
	manipShader->render(renderer->getDeviceContext(), mesh_2->getIndexCount());

	
	//translate by x
	matrix1 = XMMatrixTranslation(0.0f, -20.0f, 20.0f);
	//increase scale
	matrix2 = XMMatrixScaling(10.0f, 10.0f, 10.0f);

	//multiply both matrices
	worldMatrix = XMMatrixMultiply(matrix2, matrix1);



	//// Send geometry data (from mesh)
	//mesh_2->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	//manipShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("my_texture"), my_light, light_2, camera,counter, height, length);
	//// Render object (combination of mesh geometry and shader process
	//manipShader->render(renderer->getDeviceContext(), mesh_2->getIndexCount());



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
	
	//display height and length
	ImGui::Text("Height: %.2f", height);
	ImGui::Text("Length: %.2f", length);
	ImGui::Text("Speed: %.2f", speed);

	ImGui::Text("Size: %.2f", heightMapSize);


	// Render UI
	ImGui::Render();
}

