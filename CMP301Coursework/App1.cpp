// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	//BaseApplication::BaseApplication();
	planeMesh = nullptr;
	sphereMesh = nullptr;
	cubeBlurMesh = nullptr;	
	orthoMesh = nullptr;
	orthoMeshDown = nullptr;
	renderTexture = nullptr;
	renderTextureDown = nullptr;
	renderTextureHorizontal = nullptr;
	renderTextureVertical = nullptr;
	renderTextureUp = nullptr;
	lightMapTexture = nullptr;
	lightMapTexture2 = nullptr;
	myTessPlaneMesh = nullptr;
	cubeMesh = nullptr;

	textureShader = nullptr;
	lightShader = nullptr;
	hBlurShader = nullptr;
	vBlurShader = nullptr;
	depthShader = nullptr;
	shadowShader = nullptr;
	geometryShader = nullptr;
	tessQuadShader = nullptr;

	//manipShader = nullptr;
	my_light = nullptr;
	light_1 = nullptr;
	light_2 = nullptr;
	textureMgr = nullptr;
	singlePointMesh = nullptr;
	model = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);
	
	// Mesh objects
	planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 25);
	planeMesh2 = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 25);
	sphereMesh2 = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	manipSphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	singlePointMesh = new MySinglePointMesh(renderer->getDevice(), renderer->getDeviceContext());
	myTessPlaneMesh = new MyPlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 25);
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());

	// Blur 
	cubeBlurMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(),screenWidth, screenHeight,0,0);
	orthoMeshDown = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight, 0, 0);

	model = new Model(renderer->getDevice(), renderer->getDeviceContext(), "../assets/TieFighter2.obj");

	// Load textures
	textureMgr->loadTexture("my_texture", L"../res/greg2.jpg");
	textureMgr->loadTexture("brick", L"../res/brick1.dds");
	textureMgr->loadTexture("deathStar", L"../res/deathStart.jpg");
	textureMgr->loadTexture("white", L"../res/white.jpg");
	textureMgr->loadTexture("TieFighter", L"../res/silver.jpg");
	textureMgr->loadTexture("tree", L"../res/tree.png");
	textureMgr->loadTexture("height_map", L"../res/height.png");
	textureMgr->loadTexture("height_map2", L"../res/height2.png");
	textureMgr->loadTexture("test", L"../res/height.png");
	textureMgr->loadTexture("pattern1", L"../res/pattern1.jpg");
	textureMgr->loadTexture("pattern2", L"../res/pattern2.jpg"); 

	// RenderTexture, OrthoMesh and shader set for different renderTarget
	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	renderTextureDown = new RenderTexture(renderer->getDevice(), screenWidth/2, screenHeight/2, SCREEN_NEAR, SCREEN_DEPTH);
	renderTextureHorizontal = new RenderTexture(renderer->getDevice(), screenWidth , screenHeight , SCREEN_NEAR, SCREEN_DEPTH);
	renderTextureVertical = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight , SCREEN_NEAR, SCREEN_DEPTH);
	renderTextureUp = new RenderTexture(renderer->getDevice(), screenWidth , screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	lightMapTexture = new RenderTexture(renderer->getDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);
	lightMapTexture2 = new RenderTexture(renderer->getDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);


	// Init all Shaders 
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);
	
	hBlurShader = new HorizontalBlurShader(renderer->getDevice(), hwnd);
	vBlurShader = new VerticalBlurShader(renderer->getDevice(), hwnd);

	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);

	geometryShader = new GeometryShader(renderer->getDevice(), hwnd);
	tessQuadShader = new TessellationQuadShader(renderer->getDevice(), hwnd);
	manipShader = new ManipulationShader(renderer->getDevice(), hwnd);

	// Init light variables
	// Light1
	movingLightPos[0] = 0.0f;
	movingLightPos[1] = 8.0f;
	movingLightPos[2] = 10.0f;

	pointDiffuse[0] = 0.9f;
	pointDiffuse[1] = 0.1f;
	pointDiffuse[2] = 0.1f;

	//Light2
	movingLightPos2[0] = -5.0f;
	movingLightPos2[1] = 8.0f;
	movingLightPos2[2] = -5.0f;

	pointDiffuse2[0] = 0.0f;
	pointDiffuse2[1] = 0.9f;
	pointDiffuse2[2] = 0.1f;

	movingLightPos3[0] = -25.0f;
	movingLightPos3[1] = 5.0f;
	movingLightPos3[2] = 5.0f;

	tessPlanePosition = XMFLOAT3(15.0f, 0.0f, -12.5f);
	planePosition = XMFLOAT3(-12.5f, -1.0f, -12.5f);
	planePosition2 = XMFLOAT3(-40.0f, -1.1f, -5.0f);
	heightMapSpherePos = XMFLOAT3(0.0f, 10.0f, 40.0f);
	cubePosition = XMFLOAT3(-25.0f,1.0f,5.0f);

	// Create light objects and set variables depending on the type 
	// 0 = DIRECTIONAL, 1 = POINT, 2 = SPOT
	//Directional light
	my_light = new Light;
	//my_light->setLightType(0); //DIRECTIONAL
	my_light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	my_light->setAmbientColour(0.8, 0.8f, 0.8f, 1.0f);
	my_light->setDirection(1.37f, 1.37f, 0.0f);
	
	my_light->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	my_light->setSpecularPower(50.0f);


	// Point light 1 - LookAt is set for shadows 
	light_1 = new Light;
	//light_1->setLightType(1);
	light_1->setDiffuseColour(pointDiffuse[0], pointDiffuse[1], pointDiffuse[2], 1.0f);
	light_1->setAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	//light->setPosition(camera->getPosition().x, camera->getPosition().y, -camera->getPosition().z);
	light_1->setPosition(movingLightPos[0], movingLightPos[1], movingLightPos[2]);
	light_1->setLookAt(0.0f, 0.0f, 0.0f);

	light_1->generateViewMatrix();
	light_1->generateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);
	
	// Point light 2 - LookAt is set for shadows 
	light_2 = new Light;
	//light_2->setLightType(1); 
	light_2->setDiffuseColour(pointDiffuse2[0], pointDiffuse2[1], pointDiffuse2[2], 1.0f);
	light_2->setAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	light_2->setPosition(movingLightPos2[0], movingLightPos2[1], movingLightPos2[2]);
	light_2->setLookAt(0.0f, 0.0f, 0.0f);

	light_2->generateViewMatrix();
	light_2->generateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);
	

	//Spot light
	light_3 = new Light;
	light_3->setLightType(1); // 1 = POINT, 2 = SPOT
	light_3->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	light_3->setAmbientColour(0.0f, 0.0f, 0.4f, 1.0f);
	light_3->setDirection(0.0f, -1.0f, 0.0f);
	light_3->setPosition(movingLightPos3[0], movingLightPos3[1], movingLightPos3[2]);


	// Init variables
	counter = 0.0f;
	height = 0.05f;
	length = 0.12f;
	speed = 2.0f;
	rotate = 0.0f;

	sW = screenWidth;
	sH = screenHeight;

	directionalBlur = false;
	wireframe = false;
	moving = false;
	displayPositions = false;
	motionBlur = false;
	testRender = false;

	i = 0;
	storedX = 0;
	storedX = 0;
	velocityX = 0;
	velocityY = 0;

	blurAmount = 3000.0f;
	sphereHeightMapSize = 0.49f;
	planeHeightMapSize = 5.0f;
	modelRotationY = 0.0f;
	div = 0.5f;
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (planeMesh)
	{
		delete planeMesh;
		planeMesh = 0;
	}
	if (sphereMesh)
	{
		delete sphereMesh;
		sphereMesh = 0;
	}
	if (singlePointMesh)
	{
		delete singlePointMesh;
		singlePointMesh = 0;
	}
	if (cubeBlurMesh)
	{
		delete cubeBlurMesh;
		cubeBlurMesh = 0;
	}
	if (orthoMesh)
	{
		delete orthoMesh;
		orthoMesh = 0;
	}
	if (orthoMeshDown)
	{
		delete orthoMeshDown;
		orthoMeshDown = 0;
	}
	if (renderTextureDown)
	{
		delete renderTextureDown;
		renderTextureDown = 0;
	}
	if (renderTextureVertical)
	{
		delete renderTextureVertical;
		renderTextureVertical = 0;
	}
	if (renderTextureHorizontal)
	{
		delete renderTextureHorizontal;
		renderTextureHorizontal = 0;
	}
	if (renderTextureUp)
	{
		delete renderTextureUp;
		renderTextureUp = 0;
	}
	if (lightMapTexture)
	{
		delete lightMapTexture;
		lightMapTexture = 0;
	}
	if (lightMapTexture2)
	{
		delete lightMapTexture2;
		lightMapTexture2 = 0;
	}
	if (textureShader)
	{
		delete textureShader;
		textureShader = 0;
	}
	if (lightShader)
	{
		delete lightShader;
		lightShader = 0;
	}
	if (hBlurShader)
	{
		delete hBlurShader;
		hBlurShader = 0;
	}
	if (vBlurShader)
	{
		delete vBlurShader;
		vBlurShader = 0;
	}
	if (depthShader)
	{
		delete depthShader;
		depthShader = 0;
	}
	if (shadowShader)
	{
		delete shadowShader;
		shadowShader = 0;
	}
	if (renderTexture)
	{
		delete renderTexture;
		renderTexture = 0;
	}
	if (geometryShader)
	{
		delete geometryShader;
		geometryShader = 0;
	}
	if (my_light)
	{
		delete my_light;
		my_light = 0;
	}
	if (light_1)
	{
		delete light_1;
		light_1 = 0;
	}
	if (light_2)
	{
		delete light_2;
		light_2 = 0;
	}
	if (model)
	{
		delete model;
		model = 0;
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
	//Main render function
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix;

	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	renderer->setWireframeMode(wireframe);


	light_3->setPosition(movingLightPos3[0], movingLightPos3[1], movingLightPos3[2]);

	// VEREX MANIPULATION varibles
	counter += speed * timer->getTime();

	//rotation variable
	rotate += 0.01;


	//Direcional Blur ON
	if (directionalBlur || motionBlur)
	{
		//Render to texture first
		RenderToTexture(); // Should render the same as renderToShadowMap

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
		
		renderToShadowMap(light_1, lightMapTexture);
		renderToShadowMap(light_2, lightMapTexture2);
		renderShadows();

		//singlePointMesh->RandomisePosition(renderer->getDevice());

		renderer->setAlphaBlending(true);

		//// Send geometry data (from mesh)
		singlePointMesh->sendData(renderer->getDeviceContext());
		//// Set shader parameters (matrices and texture)
		geometryShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("tree"), light_1, light_2, camera);
		//// Render object (combination of mesh geometry and shader process
		geometryShader->render(renderer->getDeviceContext(), singlePointMesh->getIndexCount());

		renderer->setAlphaBlending(false);


		worldMatrix = XMMatrixTranslation(heightMapSpherePos.x,heightMapSpherePos.y,heightMapSpherePos.z);

		//// Send geometry data (from mesh)
		manipSphereMesh->sendData(renderer->getDeviceContext());
		//// Set shader parameters (matrices and texture)		
		manipShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("white"),textureMgr->getTexture("white"), my_light, light_2, camera, counter, height, length, sphereHeightMapSize);
		//// Render object (combination of mesh geometry and shader process
		manipShader->render(renderer->getDeviceContext(), manipSphereMesh->getIndexCount());

		RenderScene();
	}
	
	
	//if (testRender)
	//{
		
	//}


	renderer->setWireframeMode(false);
	
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
	/*cubeBlurMesh->sendData(renderer->getDeviceContext());

	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("my_texture"), my_light, light_2,camera);// my_light, light_2, sW, sH);// , camera, counter, height, length);

	// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), cubeBlurMesh->getIndexCount());*/

	worldMatrix = XMMatrixRotationRollPitchYaw(0.0f, modelRotationY, 0.0f);

	//// Send geometry data (from mesh)
	model->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("TieFighter"), lightMapTexture->getShaderResourceView(), lightMapTexture2->getShaderResourceView(), light_1, light_2);
	//// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), model->getIndexCount());

	worldMatrix = XMMatrixTranslation(-5.0f, 2.0f, 0.0f);

	//// Send geometry data (from mesh)
	sphereMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), lightMapTexture->getShaderResourceView(), lightMapTexture2->getShaderResourceView(), light_1, light_2);
	//// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(planePosition.x, planePosition.y, planePosition.z);

	//// Send geometry data (from mesh)
	planeMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), lightMapTexture->getShaderResourceView(), lightMapTexture2->getShaderResourceView(), light_1, light_2);
	//// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());


	worldMatrix = renderer->getWorldMatrix();
	
	//singlePointMesh->RandomisePosition(renderer->getDevice());
	renderer->setAlphaBlending(true);

	//// Send geometry data (from mesh)
	singlePointMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	geometryShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("tree"), light_1, light_2, camera);
	//// Render object (combination of mesh geometry and shader process
	geometryShader->render(renderer->getDeviceContext(), singlePointMesh->getIndexCount());

	renderer->setAlphaBlending(false);



	//worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(tessPlanePosition.x, tessPlanePosition.y, tessPlanePosition.z);

	//// Send geometry data (from mesh)
	myTessPlaneMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	tessQuadShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("test"), textureMgr->getTexture("height_map"),/* edges, insides,*/ camera, my_light, light_1, tessPlanePosition, planeHeightMapSize);
	//// Render object (combination of mesh geometry and shader process
	tessQuadShader->render(renderer->getDeviceContext(), myTessPlaneMesh->getIndexCount());


	worldMatrix = XMMatrixTranslation(heightMapSpherePos.x,heightMapSpherePos.y,heightMapSpherePos.z);

	//// Send geometry data (from mesh)
	manipSphereMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)		
	manipShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("white"), textureMgr->getTexture("height_map"), my_light, light_2, camera, counter, height, length, sphereHeightMapSize);
	//// Render object (combination of mesh geometry and shader process
	manipShader->render(renderer->getDeviceContext(), manipSphereMesh->getIndexCount());

	RenderScene();
	
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

	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,
		renderTexture->getShaderResourceView(), renderTexture->getShaderResourceView(), my_light, light_2, div);// camera);// sW, sH);

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


	if(motionBlur)
	{
		// Find mouse velocity
		// If current position is more than last frames position
		if (input->getMouseX() >= storedX)
		{
			velocityX = (input->getMouseX() - storedX) * blurAmount;
		}
		else if (input->getMouseX() <= storedX)
		{
			velocityX = (storedX - input->getMouseX()) * (1 - blurAmount);
		}

		if (input->getMouseY() >= storedY)
		{
			velocityY = (input->getMouseY() - storedY) * blurAmount;
		}
		else if (input->getMouseY() <= storedY)
		{
			velocityY = (storedY - input->getMouseY()) * (1 - blurAmount);
		}

		//saved current position to use in the next frame
		storedX = input->getMouseX();
		storedY = input->getMouseY();

		//set blur amount to velocty
		blurDirection = XMFLOAT2(velocityX, velocityY);
	}
	else if (directionalBlur)
	{
		blurDirection = XMFLOAT2(-blurDirection.x, -blurDirection.y);
	}
	

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

	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,
		renderTextureHorizontal->getShaderResourceView(),renderTextureHorizontal->getShaderResourceView(), my_light, light_2,div);// , camera);// sW, sH);

	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

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
	//cubeBlurMesh->sendData(renderer->getDeviceContext());

	//// Set shader parameters (matrices and texture)
	//textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix,
	//	projectionMatrix, renderTextureUp->getShaderResourceView(), my_light, light_2);//, camera, counter, height, length);
	//																			   // Render object (combination of mesh geometry and shader process
	//textureShader->render(renderer->getDeviceContext(), cubeBlurMesh->getIndexCount());


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
		renderTextureUp->getShaderResourceView(), my_light, light_1,camera);// sW, sH);

	lightShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

	renderer->setZBuffer(true);

	// Present the rendered scene to the screen.
	//renderer->endScene();
}

void App1::renderToShadowMap(Light* light, RenderTexture* renderTexture)
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


	light_1->setPosition(movingLightPos[0], movingLightPos[1], movingLightPos[2]);
	light_2->setPosition(movingLightPos2[0], movingLightPos2[1], movingLightPos2[2]);
	//light->setDiffuseColour(pointDiffuse[0], pointDiffuse[1], pointDiffuse[2], 1.0f);

	light_1->generateViewMatrix();
	light_2->generateViewMatrix();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	//	light->generateViewMatrix();
	//light->generateProjectionMatrix(1.0f, 100.0f);

	lightViewMatrix = light->getViewMatrix();
	lightProjectionMatrix = light->getProjectionMatrix();
	worldMatrix = renderer->getWorldMatrix();


	worldMatrix = XMMatrixRotationRollPitchYaw(0.0f, rotate, 0.0f);
	modelRotationY = rotate;

	//// Send geometry data (from mesh)
	model->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), model->getIndexCount());

	worldMatrix = XMMatrixTranslation(-5.0f, 2.0f, 0.0f);

	//// Send geometry data (from mesh)
	sphereMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());


	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(planePosition.x, planePosition.y, planePosition.z);

	//// Send geometry data (from mesh)
	planeMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	//// Present the rendered scene to the screen.
	//renderer->endScene();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}
void App1::renderShadows()
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


	worldMatrix = XMMatrixRotationRollPitchYaw(0.0f, rotate, 0.0f);

	//// Send geometry data (from mesh)
	model->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("TieFighter"), lightMapTexture->getShaderResourceView(), lightMapTexture2->getShaderResourceView(), light_1, light_2);
	//// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), model->getIndexCount());

	worldMatrix = XMMatrixTranslation(-5.0f, 2.0f, 0.0f);

	//// Send geometry data (from mesh)
	sphereMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), lightMapTexture->getShaderResourceView(), lightMapTexture2->getShaderResourceView(), light_1, light_2);
	//// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(planePosition.x, planePosition.y, planePosition.z);


	//// Send geometry data (from mesh)
	planeMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), lightMapTexture->getShaderResourceView(), lightMapTexture2->getShaderResourceView(), light_1, light_2);
	//// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());
	

	//worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(tessPlanePosition.x, tessPlanePosition.y, tessPlanePosition.z);
	
	//// Send geometry data (from mesh)
	myTessPlaneMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	tessQuadShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("test"), textureMgr->getTexture("height_map"),/* edges, insides,*/ camera, my_light, light_1, tessPlanePosition, planeHeightMapSize);
	//// Render object (combination of mesh geometry and shader process
	tessQuadShader->render(renderer->getDeviceContext(), myTessPlaneMesh->getIndexCount());


	//i = 0;
	//}

	//worldMatrix = XMMatrixTranslation(5.0f, 0.0f, 0.0f);

	
}

void App1::RenderScene()
{

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;


	//// Clear the scene. (default blue colour)
	//renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);


	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();


	//// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//cubeBlurMesh->sendData(renderer->getDeviceContext());

	//lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
	//	textureMgr->getTexture("my_texture"), my_light, light_2, camera);// my_light, light_2, sW, sH);// , camera, counter, height, length);

	//// Render object (combination of mesh geometry and shader process
	//lightShader->render(renderer->getDeviceContext(), cubeBlurMesh->getIndexCount());



	worldMatrix = XMMatrixTranslation(-20.0f, 2.0f, 7.0f);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	sphereMesh2->sendData(renderer->getDeviceContext());

	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("brick"), light_2, light_3, camera);// my_light, light_2, sW, sH);// , camera, counter, height, length);

																		 // Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), sphereMesh2->getIndexCount());


	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(planePosition2.x,planePosition2.y,planePosition.z);

	planeMesh2->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("brick"), light_2, light_3, camera);// my_light, light_2, sW, sH);// , camera, counter, height, length);
	// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), planeMesh2->getIndexCount());


	worldMatrix = XMMatrixTranslation(cubePosition.x, cubePosition.y, cubePosition.z);
	//// Send geometry data (from mesh)
	cubeMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("pattern1"), textureMgr->getTexture("pattern2"),light_2,light_3, div);
	//// Render object (combination of mesh geometry and shader process
	textureShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
}

void App1::gui()
{
	// Force turn off on Geometry shader
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	
	//display height and length
	//ImGui::Text("Height: %.2f", height);
	//ImGui::Text("Length: %.2f", length);
	//ImGui::Text("Speed: %.2f", speed);

	//ImGui::Text("CameraPos X: %f Y: %f Z: %f",camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

	


	ImGui::Text("test: %f ", i);// singlePointMesh->RandomisePosition(renderer->getDevice()));

	ImGui::Checkbox("Directional Blur", &directionalBlur);
	//ImGui::Checkbox("TEst", &testRender);
	
	if (directionalBlur)
	{
		ImGui::SliderFloat("x_dir", &blurDirection.x,-10000,10000);
		ImGui::SliderFloat("y_dir", &blurDirection.y, -10000, 10000);
	}

	ImGui::Checkbox("Motion Blur", &motionBlur);

	ImGui::Checkbox("Wireframe", &wireframe);
	ImGui::Checkbox("Changeable variables", &displayPositions);

	if (displayPositions)
	{
		//ImGui::SliderFloat3("Point Light 1 Diffuse:", pointDiffuse, 0, 1);
		ImGui::SliderFloat3("Red Light 1 Position:", movingLightPos, -10, 10);
		/*ImGui::SliderFloat("x_pos", &movingLightPos.x, -10, 10);
		ImGui::SliderFloat("y_pos", &movingLightPos.y, -10, 10);
		ImGui::SliderFloat("Z_pos", &movingLightPos.z, -10, 10);*/

		//ImGui::SliderFloat3("Point Light 2 Diffuse:", pointDiffuse2, 0, 1);
		ImGui::SliderFloat3("Green Light 2 Position:", movingLightPos2, -10, 10);

		//ImGui::SliderFloat3("Spot Light Position:", movingLightPos3, -10, 10);


		ImGui::SliderFloat("Plane Height Map size", &planeHeightMapSize, -10.0f, 10.0f);
		ImGui::SliderFloat("Sphere Height Map size", &sphereHeightMapSize, -100.0f, 100.0f);

		ImGui::SliderFloat("Cube Texture Blend %", &div, 0.0f, 1.0f);
	}

	// Render UI
	ImGui::Render();
}

