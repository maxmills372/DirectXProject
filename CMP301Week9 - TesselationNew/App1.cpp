// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	//BaseApplication::BaseApplication();
	mesh = nullptr;
	colourShader = nullptr;
	//tessShader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	textureMgr->loadTexture("height_map", L"../res/height.png");

	textureMgr->loadTexture("test", L"../res/checkerboard.png");



	// Create Mesh objects
	//tessMesh = new TessellationMesh(renderer->getDevice(), renderer->getDeviceContext());
	//myTessMesh = new MyTessMesh(renderer->getDevice(), renderer->getDeviceContext());
	myTessQuad = new MyTessQuad(renderer->getDevice(), renderer->getDeviceContext());
	myTessPlaneMesh = new MyPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(),4);
	myTessSphereMesh = new MyTessSphereMesh(renderer->getDevice(), renderer->getDeviceContext(), 50);

	colourShader = new ColourShader(renderer->getDevice(), hwnd);
	//tessShader = new TessellationShader(renderer->getDevice(), hwnd);
	tessQuadShader = new TessellationQuadShader(renderer->getDevice(), hwnd);

	edges = XMFLOAT4(1, 1, 1, 1);
	insides = XMFLOAT2(1, 1);
	reverse = false;
	wireframe = false;
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
	int min_clamp = 2;
	int max_clamp = 64;
	//CHANGE THIS
	if (input->isKeyDown(49)) // 1 key
	{
		//myPlaneMesh2->getIndexCount = current;
		BaseMesh::VertexType new_verts[5000];
		
		for (size_t i = 0; i < myTessPlaneMesh->vertexCount; i++)
		{
			new_verts[i] = myTessPlaneMesh->my_vertices[i];
		}
		//myPlaneMesh2 = new MyPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 20, new_verts);
		

		BaseMesh::VertexType v1;
		v1.position = XMFLOAT3(5.f, 0.0f, 0.f);
		v1.texture = XMFLOAT2(0, 1);
		v1.normal = XMFLOAT3(0.0, 1.0, 0.0);
		new_verts[myTessPlaneMesh->vertexCount + 1] = v1;

		BaseMesh::VertexType v2;
		v2.position = XMFLOAT3(5.f, 0.0f, -5.f);
		v2.texture = XMFLOAT2(0, 1);
		v2.normal = XMFLOAT3(0.0, 1.0, 0.0);
		new_verts[myTessPlaneMesh->vertexCount + 2] = v2;
		
		BaseMesh::VertexType v3;
		v3.position = XMFLOAT3(-5.f, 0.0f, -5.f);
		v3.texture = XMFLOAT2(0, 1);

		v3.normal = XMFLOAT3(0.0, 1.0, 0.0);
		new_verts[2] = v3;
		/*
		BaseMesh::VertexType v4;
		v4.position = XMFLOAT3(-5.f, 0.0f, 5.f);
		v4.texture = XMFLOAT2(0, 1);
		v4.normal = XMFLOAT3(0.0, 1.0, 0.0);
		new_verts[3] = v4;*/

		myPlaneMesh2 = new MyPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 20, new_verts);

		
		if (reverse)
		{
			if (edges.x >= min_clamp)
			{
				edges.x -= 0.5f;
			}
		}
		else
		{
			if (edges.x <= max_clamp)
			{
				edges.x += 0.5f;
			}
		}
	}
	if (input->isKeyDown(50)) // 2 key
	{
		if (reverse)
		{
			if (edges.y >= min_clamp)
			{
				edges.y -= 0.5f;
			}
		}
		else
		{
			if (edges.y <= max_clamp)
			{
				edges.y += 0.5f;
			}
		}
	}
	if (input->isKeyDown(51)) // 3 key
	{
		if (reverse)
		{
			if (edges.z >= min_clamp)
			{
				edges.z -= 0.5f;
			}
		}
		else
		{
			if (edges.z <= max_clamp)
			{
				edges.z += 0.5f;
			}
		}
	}
	if (input->isKeyDown(52)) // 4 key
	{
		if (reverse)
		{
			if (insides.x >= min_clamp)
			{
				insides.x -= 0.5f;
			}
		}
		else
		{
			if (insides.x <= max_clamp)
			{
				insides.x += 0.5f;
			}
		}
	}
	if (input->isKeyDown(79)) // 4 key
	{
		if (reverse)
		{
			if (insides.y >= min_clamp)
			{
				insides.y -= 0.5f;
			}
		}
		else
		{
			if (insides.y <= max_clamp)
			{
				insides.y += 0.5f;
			}
		}
	}
	if (input->isKeyDown(53)) // 5 key
	{
		if (reverse)
		{
			if (edges.x >= min_clamp || edges.y >= min_clamp || edges.z >= min_clamp )
			{
				edges.x -= 0.5f;

				edges.y -= 0.5f;

				edges.z -= 0.5f;

				edges.w += 0.5f;
			}
		}
		else
		{
			if (edges.x <= max_clamp || edges.y <= max_clamp || edges.z <= max_clamp)
			{
				edges.x += 0.5f;

				edges.y += 0.5f;

				edges.z += 0.5f;

				edges.w += 0.5f;
			}
		}
	}
	//reset
	if (input->isKeyDown(54)) // 6 key
	{
		edges = XMFLOAT4(1,1,1,1);
		insides = XMFLOAT2(1, 1);
	}
	//change direction
	if (input->isKeyDown(80))
	{
		reverse = !reverse;
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

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	//worldMatrix = XMMatrixRotationRollPitchYaw(45.0f, 0.0f, 0.0f);

	renderer->setWireframeMode(wireframe);

	//// Send geometry data (from mesh)
	myTessPlaneMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	tessQuadShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("test"), textureMgr->getTexture("height_map"), edges, insides, camera);
	//// Render object (combination of mesh geometry and shader process
	tessQuadShader->render(renderer->getDeviceContext(), myTessPlaneMesh->getIndexCount());

	worldMatrix = XMMatrixTranslation(0, 3, 0);

	if (myPlaneMesh2)
	{
		//// Send geometry data (from mesh)
		myPlaneMesh2->sendData(renderer->getDeviceContext());
		//// Set shader parameters (matrices and texture)
		tessQuadShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("test"), textureMgr->getTexture("height_map"), edges, insides, camera);
		//// Render object (combination of mesh geometry and shader process
		tessQuadShader->render(renderer->getDeviceContext(), myPlaneMesh2->getIndexCount());

	}

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
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Text("Vertices: %f", myTessPlaneMesh->indexCount);
	/*ImGui::SliderFloat("Edge1 :", &edges.x, 1, 64);
	ImGui::SliderFloat("Edge2 :", &edges.y, 1, 64);
	ImGui::SliderFloat("Edge3 :", &edges.z, 1, 64);
	ImGui::SliderFloat("Inside1 :", &insides.x, 1, 64);
	ImGui::SliderFloat("Insides2 :", &insides.y, 1, 64);*/
	ImGui::Checkbox("Wireframe", &wireframe);

	// Render UI
	ImGui::Render();
}

