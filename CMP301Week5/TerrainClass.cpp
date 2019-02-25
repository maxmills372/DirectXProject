////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "terrainclass.h"
#include <cmath>


TerrainClass::TerrainClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_heightMap = 0;
	m_terrainGeneratedToggle = false;
	perlin_x = 0.5f;
	perlin_y = 0.5f;
	/*
	m_GrassTexture = 0;
	m_SlopeTexture = 0;
	m_RockTexture = 0;
	m_WaterTexture = 0;*/
}


TerrainClass::TerrainClass(const TerrainClass& other)
{
}


TerrainClass::~TerrainClass()
{
}

bool TerrainClass::InitializeTerrain(ID3D11Device* device, int terrainWidth, int terrainHeight, WCHAR* Texture1Filename, WCHAR* Texture2Filename,
	WCHAR* Texture3Filename, WCHAR* waterTextureFilename, WCHAR* grainTextureFilename)
{
	int index;
	float height = 0.0;
	bool result;

	float positions[9];

	// Save the dimensions of the terrain.
	m_terrainWidth = terrainWidth;
	m_terrainHeight = terrainHeight;

	// Create the structure to hold the terrain data.
	m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	if (!m_heightMap)
	{
		return false;
	}

	// Initialise the data in the height map (flat).
	for (int j = 0; j<m_terrainHeight; j++)
	{
		for (int i = 0; i<m_terrainWidth; i++)
		{
			index = (m_terrainHeight * j) + i;

			m_heightMap[index].x = (float)i;
			m_heightMap[index].y = (float)height;
			m_heightMap[index].z = (float)j;

		}
	}


	//even though we are generating a flat terrain, we still need to normalise it. 
	// Calculate the normals for the terrain data.
	result = CalculateNormals();
	if (!result)
	{
		return false;
	}

	// Calculate texture coordinates
	CalculateTextureCoordinates();

	// Load the textures.
	result = LoadTextures(device, Texture1Filename, Texture2Filename, Texture3Filename, waterTextureFilename, grainTextureFilename);
	if (!result)
	{
		return false;
	}


	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}


	// Start //
	srand(time(NULL));


	//GenerateHeightMap(device, true);


	//LoadHeightMap("../Engine/data/test.bmp");

	//IterateFaultLines(device, 20);
	/*
	ParticleDeposition(device, true, 0);// ((128 * 128) / 2) + 20);
	ParticleDeposition(device, true, ((128 * 128) / 2) + 80);



	ActualSmooth(device, true);
	ActualSmooth(device, true);

	GenerateHeightMapSmall(device, true);
	GenerateHeightMapSmall(device, true);
	GenerateHeightMapSmall(device, true);

	//ActualSmooth(device, true);
	ActualSmooth(device, true);

	*/

	//CapHeights(device, true,0.19f,false);


	//RandomPoint(device, true);



	//Octaves(device,true);
	//CapHeights(device, true, 0.0f,false);
	//CapHeights(device, true, 3.5f,true);

	//RemoveTerrain(device, true);

	//RandomPoint(device, true);
	//RandomPoint(device, true);

	//	ActualSmooth(device, true);
	//ActualSmooth(device, true);
	//RandomPoint(device, true);

	//GenerateHeightMapTest(device, true);


	return true;
}

bool TerrainClass::Initialize(ID3D11Device* device, char* heightMapFilename, WCHAR* textureFilename)
{
	bool result;


	// Load in the height map for the terrain.
	result = LoadHeightMap(heightMapFilename);
	if (!result)
	{
		return false;
	}

	// Normalize the height of the height map.
	NormalizeHeightMap();

	// Calculate the normals for the terrain data.
	result = CalculateNormals();
	if (!result)
	{
		return false;
	}
	// Calculate the texture coordinates.
	CalculateTextureCoordinates();

	// Load the texture.
	//result = LoadTextures(device, textureFilename);
	//if (!result)
	//{
	//	return false;
	//}

	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void TerrainClass::Shutdown()
{
	// Release the texture.
	ReleaseTextures();

	// Release the vertex and index buffer.
	ShutdownBuffers();

	// Release the height map data.
	ShutdownHeightMap();

	return;
}

void TerrainClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

ID3D11ShaderResourceView* TerrainClass::GetGrassTexture()
{
	//return m_GrassTexture->GetTexture();
}
ID3D11ShaderResourceView* TerrainClass::GetSlopeTexture()
{
	//return m_SlopeTexture->GetTexture();
}
ID3D11ShaderResourceView* TerrainClass::GetRockTexture()
{
	//return m_RockTexture->GetTexture();
}
ID3D11ShaderResourceView* TerrainClass::GetWaterTexture()
{
	//return m_WaterTexture->GetTexture();
}
ID3D11ShaderResourceView* TerrainClass::GetGrainTexture()
{
	//return m_GrainTexture->GetTexture();
}

int TerrainClass::GetIndexCount()
{
	return m_indexCount;
}

bool TerrainClass::GetCanMove()
{
	return can_move;
}

bool TerrainClass::GenerateHeightMapWater(ID3D11Device* device)
{
	bool result;

	if (!m_terrainGeneratedToggle)
	{
		int index;
		float height = 0.0;
		float vec[2];

		perlin_x += 1.0f;
		perlin_y += 1.0f;


		//loop through the terrain and set the hieghts how we want. This is where we generate the terrain
		//in this case I will run a sin-wave through the terrain in one axis.

		for (int j = 0; j < m_terrainHeight; j++)
		{
			for (int i = 0; i < m_terrainWidth; i++)
			{
				index = (m_terrainHeight * j) + i;

				vec[0] = (i + perlin_x) / 2;
				vec[1] = (j + perlin_y) / 2;

				m_heightMap[index].x = (float)i;
				m_heightMap[index].y -= perlin.noise2(vec) * 2.0f;
				m_heightMap[index].z = (float)j;

				//magic numbers ahoy, just to ramp up the height of the sin function so its visible.
				//(float)(cos((float)i/(m_terrainWidth/12))*3.0) + (float)(cos((float)j / (m_terrainWidth / 12))*3.0); 
			}
		}

		ActualSmooth(device, true);
		ActualSmooth(device, true);


		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;

	}
	return true;
}
void TerrainClass::CalculateTextureCoordinates()
{
	int incrementCount, i, j, tuCount, tvCount;
	float incrementValue, tuCoordinate, tvCoordinate;


	// Calculate how much to increment the texture coordinates by.
	incrementValue = (float)texture_repeat / (float)m_terrainWidth;

	// Calculate how many times to repeat the texture.
	incrementCount = m_terrainWidth / texture_repeat;

	// Initialize the tu and tv coordinate values.
	tuCoordinate = 0.0f;
	tvCoordinate = 1.0f;

	// Initialize the tu and tv coordinate indexes.
	tuCount = 0;
	tvCount = 0;

	// Loop through the entire height map and calculate the tu and tv texture coordinates for each vertex.
	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			// Store the texture coordinate in the height map.
			m_heightMap[(m_terrainHeight * j) + i].tu = tuCoordinate;
			m_heightMap[(m_terrainHeight * j) + i].tv = tvCoordinate;

			// Increment the tu texture coordinate by the increment value and increment the index by one.
			tuCoordinate += incrementValue;
			tuCount++;

			// Check if at the far right end of the texture and if so then start at the beginning again.
			if (tuCount == incrementCount)
			{
				tuCoordinate = 0.0f;
				tuCount = 0;
			}
		}

		// Increment the tv texture coordinate by the increment value and increment the index by one.
		tvCoordinate -= incrementValue;
		tvCount++;

		// Check if at the top of the texture and if so then start at the bottom again.
		if (tvCount == incrementCount)
		{
			tvCoordinate = 1.0f;
			tvCount = 0;
		}
	}

	return;
}
bool TerrainClass::LoadTextures(ID3D11Device* device, WCHAR* Texture1Filename, WCHAR* Texture2Filename, WCHAR* Texture3Filename, WCHAR* waterTextureFilename, WCHAR* grainTextureFilename)
{
	bool result;
	/*
	// Create the grass texture object.
	m_GrassTexture = new TextureClass;
	if (!m_GrassTexture)
	{
		return false;
	}

	// Initialize the grass texture object.
	result = m_GrassTexture->Initialize(device, Texture1Filename);
	if (!result)
	{
		return false;
	}

	// Create the slope texture object.
	m_SlopeTexture = new TextureClass;
	if (!m_SlopeTexture)
	{
		return false;
	}

	// Initialize the slope texture object.
	result = m_SlopeTexture->Initialize(device, Texture2Filename);
	if (!result)
	{
		return false;
	}

	// Create the rock texture object.
	m_RockTexture = new TextureClass;
	if (!m_RockTexture)
	{
		return false;
	}

	// Initialize the rock texture object.
	result = m_RockTexture->Initialize(device, Texture3Filename);
	if (!result)
	{
		return false;
	}

	// Create the rock texture object.
	m_WaterTexture = new TextureClass;
	if (!m_WaterTexture)
	{
		return false;
	}

	// Initialize the rock texture object.
	result = m_WaterTexture->Initialize(device, waterTextureFilename);
	if (!result)
	{
		return false;
	}

	// Create the rock texture object.
	m_GrainTexture = new TextureClass;
	if (!m_GrainTexture)
	{
		return false;
	}

	// Initialize the rock texture object.
	result = m_GrainTexture->Initialize(device, grainTextureFilename);
	if (!result)
	{
		return false;
	}
	*/


	return true;
}

void TerrainClass::ReleaseTextures()
{/*
	// Release the texture objects.
	if (m_GrassTexture)
	{
		m_GrassTexture->Shutdown();
		delete m_GrassTexture;
		m_GrassTexture = 0;
	}

	if (m_SlopeTexture)
	{
		m_SlopeTexture->Shutdown();
		delete m_SlopeTexture;
		m_SlopeTexture = 0;
	}

	if (m_RockTexture)
	{
		m_RockTexture->Shutdown();
		delete m_RockTexture;
		m_RockTexture = 0;
	}*/

	return;
}
bool TerrainClass::RemoveTerrain(ID3D11Device* device, bool keydown)
{
	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 
	if (keydown && (!m_terrainGeneratedToggle))
	{

		int index;

		int point_1_x = rand() % 128;
		int point_1_z = rand() % 128;

		int point_2_x = rand() % 128;
		int point_2_z = rand() % 128;

		int a = (point_2_z - point_1_z);
		int	b = (point_2_x - point_1_x);


		int m = (b) / (a + 0.01f);
		int c = point_2_x - (m*point_2_z);

		int width = 10;
		int y;
		float max_height = 5.0f;
		//loop through the terrain and set the hieghts how we want. This is where we generate the terrain
		//in this case I will run a sin-wave through the terrain in one axis.
		for (int w = 0; w < width; w++)
		{
			for (int j = 0; j < m_terrainHeight; j++)
			{
				for (int i = 0; i < m_terrainWidth; i++)
				{
					index = (m_terrainHeight * j) + i;

					y = (m*i) + c;
					if (j == y)
					{
						//m_heightMap[index].y -= 5.0f;
						m_heightMap[index].y -= max_height;// -(m_heightMap[index].y - max_height);

					}

					/*if ((a*i) + (b*j) - crs > 0)
					{
					m_heightMap[index].y = 5.0f;
					}
					else if((a*i) + (b*j) + crs < 0)
					{
					m_heightMap[index].y = 5.0f;
					}
					else
					{
					m_heightMap[index].y = 0.0f;

					}*/


					//magic numbers ahoy, just to ramp up the height of the sin function so its visible.
					//(float)(cos((float)i/(m_terrainWidth/12))*3.0) + (float)(cos((float)j / (m_terrainWidth / 12))*3.0); 
				}
			}

			point_1_z++;
			point_1_x--;
			point_2_z++;
			point_2_x--;

			a = (point_2_z - point_1_z);
			b = (point_2_x - point_1_x);

			m = (b) / (a + 0.1f);
			c = point_2_x - (m*point_2_z);

		}

		//ActualSmooth(device,keydown);
		//ActualSmooth(device, keydown);
		//ActualSmooth(device, keydown);


		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}




	return true;


}

bool TerrainClass::Octaves(ID3D11Device* device, bool keydown)
{
	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 
	if (keydown && (!m_terrainGeneratedToggle))
	{

		int index;
		float height = 0.0f;
		float vec[2], vec_2[2], vec_4[2];

		perlin_x += 1.0f;
		perlin_y += 1.0f;


		//loop through the terrain and set the hieghts how we want. This is where we generate the terrain
		//in this case I will run a sin-wave through the terrain in one axis.

		for (int j = 0; j<m_terrainHeight; j++)
		{
			for (int i = 0; i<m_terrainWidth; i++)
			{
				index = (m_terrainHeight * j) + i;

				vec[0] = (i + perlin_x) / 20 - 0.5f;
				vec[1] = (j + perlin_y) / 20 - 0.5f;

				vec_2[0] = vec[0] * 2;
				vec_2[1] = vec[1] * 2;

				vec_4[0] = vec[0] * 4;
				vec_4[1] = vec[1] * 4;

				/*float octaves = 1 * perlin.noise2(vec)
				+ 0.5f * perlin.noise2(vec_2);
				+0.25f * perlin.noise2(vec_4);
				*/

				float octaves = OctavePerlin(j, i, 8, 0.35f);// +(1.0f - abs(perlin.noise2(vec))) / 2.0f;

				m_heightMap[index].x = (float)i;
				m_heightMap[index].y += octaves;// pow(octaves * 5.0f, 2.0f);

				m_heightMap[index].z = (float)j;




				//magic numbers ahoy, just to ramp up the height of the sin function so its visible.
				//(float)(cos((float)i/(m_terrainWidth/12))*3.0) + (float)(cos((float)j / (m_terrainWidth / 12))*3.0); 
			}
		}

		//ActualSmooth(device,keydown);
		//ActualSmooth(device, keydown);
		//ActualSmooth(device, keydown);


		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}




	return true;


}
bool TerrainClass::IterateFaultLines(ID3D11Device* device, bool keydown, int iterations)
{

	float disp0 = 2.0f;
	float dispi = 4.0f;
	float dispn = 2.5f;
	float n = 5.0f;
	float min = 0.5f;

	if (keydown && (!m_terrainGeneratedToggle))
	{
		for (int i = 0; i < iterations; i++)
		{
			dispi -= 0.2f;
			if (dispi <= 0.0f)
			{
				dispi = min;
			}
			/*if (i < n)
			{
			dispi = disp0 - (i / n) * (dispn - disp0);
			}
			else
			{
			dispi = dispn;
			}*/

			FaultLines(device, true, dispi);
		}
		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}

	return true;

}
bool TerrainClass::FaultLines(ID3D11Device* device, bool keydown, float disp)
{
	bool result;

	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 
	if (keydown && (!m_terrainGeneratedToggle))
	{

		int point_1_x = rand() % 128;
		int point_1_z = rand() % 128;
		int point_2_x = rand() % 128;
		int point_2_z = rand() % 128;

		int a = (point_2_z - point_1_z);
		int	b = (point_2_x - point_1_x);
		int	c = point_1_x*(point_2_z - point_1_z) + point_1_z*(point_2_x - point_1_x);

		int displacement = disp;

		int index;
		for (int j = 0; j < m_terrainHeight; j++)
		{
			for (int i = 0; i < m_terrainWidth; i++)
			{
				index = (m_terrainHeight * j) + i;

				if ((a*i) + (b*j) - c > 0)
				{
					m_heightMap[index].y += displacement;
				}
				else
				{
					m_heightMap[index].y -= displacement;
				}
			}
		}

		//ActualSmooth(device, true);
		//ActualSmooth(device, true);

		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}

	return true;

}
bool TerrainClass::CapHeights(ID3D11Device* device, bool keydown, float min_height, bool test)
{
	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 

	//if (keydown && (!m_terrainGeneratedToggle))
	//{


	int index;

	for (int j = 0; j < m_terrainHeight; j++)
	{
		for (int i = 0; i < m_terrainWidth; i++)
		{
			index = (m_terrainHeight * j) + i;
			if (test)
			{
				if (/*m_heightMap[index].y <= 8.0f &&*/ m_heightMap[index].y >= 4.5f)
				{
					m_heightMap[index].y = 4.5f;
				}
			}
			else
			{
				if (m_heightMap[index].y <= min_height)
				{
					m_heightMap[index].y = min_height;
				}
			}
		}
	}


	result = CalculateNormals();
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	m_terrainGeneratedToggle = true;
	//}
	//else
	//{
	//	m_terrainGeneratedToggle = false;
	//}

	return true;

}

bool TerrainClass::ParticleDeposition(ID3D11Device* device, bool keydown, int point, bool inverse)
{
	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 

	if (keydown && (!m_terrainGeneratedToggle))
	{

		// Default value of point (0) is in the middle, otherwise use actual value of point
		if (point == 0)
		{
			point = ((m_terrainHeight * m_terrainWidth) / 2.0f) + 64.0f;
		}

		//int fall_points[25];
		std::vector<int> fall_points;
		int counter = 0;
		int count = 0;
		int n_index = 0;
		int fall_point;

		// Find neighbours
		for (int x = -2; x <= 2; x++)
		{
			for (int y = -2; y <= 2; y++)
			{
				n_index = (y * 128) + x;

				//fall_points[counter] = point - n_index;	

				fall_points.push_back(point - n_index);

				counter++;
			}
		}

		int random = rand() % 25;
		fall_point = fall_points[random];

		bool highest_point = false;
		int particles[8];
		int height_increase = 5.0f;
		int random_num;
		int index;

		if (inverse)
		{
			while (!highest_point)
			{
				count = 0;
				// Checks if any neighbours of the fallen point are lower than the point
				if (fall_point - 129 >= 0 && m_heightMap[fall_point - 129].y > m_heightMap[fall_point].y)
				{
					// Adds them to array
					particles[count] = (fall_point - 129);
					count++;
				}
				if (fall_point - 128 >= 0 && m_heightMap[fall_point - 128].y > m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point - 128);
					count++;
				}
				if (fall_point - 127 >= 0 && m_heightMap[fall_point - 127].y > m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point - 127);
					count++;
				}
				if (fall_point - 1 >= 0 && m_heightMap[fall_point - 1].y > m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point - 1);
					count++;
				}
				if (fall_point + 1 <= (m_terrainWidth * m_terrainHeight) && m_heightMap[fall_point + 1].y > m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point + 1);
					count++;
				}
				if (fall_point + 127 <= (m_terrainWidth * m_terrainHeight) && m_heightMap[fall_point + 127].y > m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point + 127);
					count++;
				}
				if (fall_point + 128 <= (m_terrainWidth * m_terrainHeight) && m_heightMap[fall_point + 128].y > m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point + 128);
					count++;
				}
				if (fall_point + 129 <= (m_terrainWidth * m_terrainHeight) && m_heightMap[fall_point + 129].y > m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point + 129);
					count++;
				}

				//Particle is stable
				if (count == 0)
				{
					m_heightMap[fall_point].y -= height_increase;

					if (m_heightMap[fall_point].y <= -50)
					{
						highest_point = true;
						break;
					}

					random_num = rand() % 25;

					fall_point = fall_points[random_num];

				}
				else
				{
					fall_point = particles[rand() % count];
				}
			}

			for (int j = 0; j < m_terrainHeight; j++)
			{
				for (int i = 0; i < m_terrainWidth; i++)
				{
					index = (m_terrainHeight * j) + i;
					m_heightMap[index].y += 10.0f;
				}
			}
		}
		else
		{
			while (!highest_point)
			{
				count = 0;
				// Checks if any neighbours of the fallen point are lower than the point
				if (fall_point - 129 >= 0 && m_heightMap[fall_point - 129].y < m_heightMap[fall_point].y)
				{
					// Adds them to array
					particles[count] = (fall_point - 129);
					count++;
				}
				if (fall_point - 128 >= 0 && m_heightMap[fall_point - 128].y < m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point - 128);
					count++;
				}
				if (fall_point - 127 >= 0 && m_heightMap[fall_point - 127].y < m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point - 127);
					count++;
				}
				if (fall_point - 1 >= 0 && m_heightMap[fall_point - 1].y < m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point - 1);
					count++;
				}
				if (fall_point + 1 <= (m_terrainWidth * m_terrainHeight) && m_heightMap[fall_point + 1].y < m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point + 1);
					count++;
				}
				if (fall_point + 127 <= (m_terrainWidth * m_terrainHeight) && m_heightMap[fall_point + 127].y < m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point + 127);
					count++;
				}
				if (fall_point + 128 <= (m_terrainWidth * m_terrainHeight) && m_heightMap[fall_point + 128].y < m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point + 128);
					count++;
				}
				if (fall_point + 129 <= (m_terrainWidth * m_terrainHeight) && m_heightMap[fall_point + 129].y < m_heightMap[fall_point].y)
				{
					particles[count] = (fall_point + 129);
					count++;
				}

				//Particle is stable
				if (count == 0)
				{
					m_heightMap[fall_point].y += height_increase;

					if (m_heightMap[fall_point].y >= 50)
					{
						highest_point = true;
						break;
					}

					random_num = rand() % 25;

					fall_point = fall_points[random_num];

				}
				else
				{
					fall_point = particles[rand() % count];
				}
			}
		}


		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}

	return true;

}

bool TerrainClass::MoveTerrain(ID3D11Device* device, bool keydown, float amount)
{
	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 

	int index;

	if (keydown && (!m_terrainGeneratedToggle))
	{

		for (int j = 0; j < m_terrainHeight; j++)
		{
			for (int i = 0; i < m_terrainWidth; i++)
			{
				index = (m_terrainHeight * j) + i;
				m_heightMap[index].y += amount;
			}
		}

		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}

	return true;

}

bool TerrainClass::ResetHeight(ID3D11Device* device, bool keydown)
{
	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 
	if (keydown && (!m_terrainGeneratedToggle))
	{
		int index;

		for (int j = 0; j < m_terrainHeight; j++)
		{
			for (int i = 0; i < m_terrainWidth; i++)
			{
				index = (m_terrainHeight * j) + i;
				m_heightMap[index].y = -0.01f;
			}
		}

		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}

	return true;

}
bool TerrainClass::Circle(ID3D11Device* device, bool keydown, int x, int y)
{
	bool result;

	if (keydown && (!m_terrainGeneratedToggle))
	{
		//create a circle at an x and y with radius between 1 and 20 using random numbers
		int random_x = x;            //randomly select x center point for circle
		int random_z = y; +1;                   //randomly generate radius integer between 1 - 20 to be the terrainCircleSize
		float disp = 2.0f;                         //randomly generate a value of disp between 1 - 12
		int random_radius = 5;                   //randomly generate radius integer between 1 - 20 to be the terrainCircleSize
		int index;

		for (int x = 0; x < 128; x++)
		{
			for (int z = 0; z < 128; z++)
			{
				index = ((x * 128) + z);
				int dx = x - random_x;                      //get the value for tx to determine distance from circle center
				int dz = z - random_z;                      //get the value for ty to determine distance from circle center
				float distance = sqrtf((dx*dx) + (dz*dz));  //get the distance from circle center
				float pd = (distance * 2) / random_radius;      //get the value for pd
				if (fabs(pd) <= 1.0)
				{
					//check if value is within the circle
					m_heightMap[index].y += (disp / 2.0) + (cos(pd*3.14)*(disp / 2.0)); //apply circle alorithm to get the new height
				}
			}
		}

		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}
	return true;
}
bool TerrainClass::RandomPoint(ID3D11Device* device, bool keydown, bool negative_effect)
{
	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 
	if (keydown && (!m_terrainGeneratedToggle))
	{

		//create a circle at an x and y with radius between 1 and 20 using random numbers
		int random_x = rand() % 128;                         //randomly select x center point for circle
		int random_z = rand() % 128;                         //randomly select z center point for circle
		int random_radius = (rand() % 10) + 1;                   //randomly generate radius integer between 1 - 20 to be the terrainCircleSize
		float disp = (rand() % 1) + 1;                         //randomly generate a value of disp between 1 - 12
		int index;


		for (int x = 0; x < 128; x++)
		{
			for (int z = 0; z < 128; z++)
			{
				index = ((x * 128) + z);
				int dx = x - random_x;                      //get the value for tx to determine distance from circle center
				int dz = z - random_z;                      //get the value for ty to determine distance from circle center
				float distance = sqrtf((dx*dx) + (dz*dz));  //get the distance from circle center
				float pd = (distance * 2) / random_radius;      //get the value for pd
				if (fabs(pd) <= 1.0)
				{
					if (negative_effect)
					{
						//check if value is within the circle
						m_heightMap[index].y -= (disp / 2.0) + (cos(pd*3.14)*(disp / 2.0)); //apply circle alorithm to get the new height
					}
					else
					{
						//check if value is within the circle
						m_heightMap[index].y += (disp / 2.0) + (cos(pd*3.14)*(disp / 2.0)); //apply circle alorithm to get the new height
					}
				}
			}
		}



		//ActualSmooth(device, keydown);
		//ActualSmooth(device, keydown);
		//CapHeights(device, true, -1.0f, false);
		//CapHeights(device, true, 0.0f, true );
		//ActualSmooth(device, keydown);
		//ActualSmooth(device, keydown);

		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}

	return true;

}

bool TerrainClass::ActualSmooth(ID3D11Device* device, bool keydown)
{
	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 
	if (keydown && (!m_terrainGeneratedToggle))
	{


		int index;
		int count = 1;
		float height = 0.0;

		//loop through the terrain and set the hieghts how we want. This is where we generate the terrain
		//in this case I will run a sin-wave through the terrain in one axis.

		for (int j = 0; j<m_terrainHeight; j++)
		{
			for (int i = 0; i<m_terrainWidth; i++)
			{
				index = (m_terrainHeight * j) + i;

				if (index - 127 >= 0)
				{
					m_heightMap[index].y += (m_heightMap[index - 127].y);
					count++;
				}
				if (index - 128 >= 0)
				{
					m_heightMap[index].y += (m_heightMap[index - 128].y);
					count++;
				}
				if (index - 129 >= 0)
				{
					m_heightMap[index].y += (m_heightMap[index - 129].y);
					count++;
				}
				if (index - 1 >= 0)
				{
					m_heightMap[index].y += (m_heightMap[index - 1].y);
					count++;
				}
				if (index + 1 <= (m_terrainHeight*m_terrainHeight) - 1)
				{
					m_heightMap[index].y += (m_heightMap[index + 1].y);
					count++;
				}
				if (index + 127 <= (m_terrainHeight*m_terrainHeight) - 1)
				{
					m_heightMap[index].y += (m_heightMap[index + 127].y);
					count++;
				}
				if (index + 128 <= (m_terrainHeight*m_terrainHeight) - 1)
				{
					m_heightMap[index].y += (m_heightMap[index + 128].y);
					count++;
				}
				if (index + 129 <= (m_terrainHeight*m_terrainHeight) - 1)
				{
					m_heightMap[index].y += (m_heightMap[index + 129].y);
					count++;
				}

				m_heightMap[index].y /= count;
				count = 1;
			}
		}

		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}

	return true;
}


float* mult_array(float arr[2], float t)
{

	arr[0] * t;
	arr[1] * t;
	arr[2] * t;

	return arr;
}

bool TerrainClass::GenerateHeightMap(ID3D11Device* device, bool keydown)
{

	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 
	if (keydown && (!m_terrainGeneratedToggle))
	{
		int index;

		float vec[2], vec3[2], vec4[2];

		perlin_x += 1.0f;
		perlin_y += 1.0f;
		float perlinAmount, islandAmount, total;
		float weighting = 1.0f;
		float perlin_amount;
		float perlin_amount_2;
		float ridged_amount;
		float dist, dx, dy;

		//loop through the terrain and set the hieghts how we want. This is where we generate the terrain
		//in this case I will run a sin-wave through the terrain in one axis.

		for (int j = 0; j<m_terrainHeight; j++)
		{
			for (int i = 0; i<m_terrainWidth; i++)
			{
				index = (m_terrainHeight * j) + i;


				//vec[0] = (i + perlin_x)/20 - 0.5f;
				//vec[1] = (j + perlin_y)/20 - 0.5f;				
				//float e = 50.0f * pow(perlin.noise2(vec) + 0.5f /** perlin.noise2(mult_array(vec, 4.0f)) + 0.25f * perlin.noise2(mult_array(vec, 8.0f))*/, 2.5f);				
				//float d = 2 * sqrt(vec[0] * vec[0] + vec[1] * vec[1]);		
				//perlinAmount = perlin.noise2(vec) * 15.0f;
				vec[0] = ((i + 5.0f) / 100.0f);
				vec[1] = ((j + 5.0f) / 100.0f);
				vec3[0] = 6 * vec[0] / 20.0f;
				vec3[1] = 6 * vec[1] / 20.0f;

				vec4[0] = ((i + 5.0f) / 40.0f);
				vec4[1] = ((j + 5.0f) / 40.0f);
				perlin_amount = (perlin.noise2(vec4));
				perlin_amount_2 = (perlin.noise2(vec3));

				ridged_amount = (abs(perlin.noise2(vec)));

				dx = 250 - j;
				dy = 250 - i;
				dist = sqrt((dx*dx) + (dy*dy));

				//perlin_amount = pow(perlin_amount, 2.06f);

				float param = (dist * 0.38f);

				param = (param / 180.0f)*3.14f;

				float height = cos(sin(param));


				float total = (perlin_amount);// +perlin_amount_2 + (1.0f - ridged_amount * 4.0f)) * 2.0f;// *(1 - height * 2);

											  // terrace effect
											  // floor( perlin * 5)/10;
											  //total = floor( total * 5)/ 10;


				m_heightMap[index].x = (float)i;
				//m_heightMap[index].y = perlinAmount;
				m_heightMap[index].y += total;

				//float(rand() % 50 - 40) + 40;

				m_heightMap[index].z = (float)j;


				//magic numbers ahoy, just to ramp up the height of the sin function so its visible.
				//(float)(cos((float)i/(m_terrainWidth/12))*3.0) + (float)(cos((float)j / (m_terrainWidth / 12))*3.0); 
			}
		}

		//Smooth(device,keydown);
		//Smooth(device, keydown);
		//Smooth(device, keydown);
		//CapHeights(device, keydown, -1.0f, false);

		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}




	return true;
}
bool TerrainClass::GenerateHeightMapSmall(ID3D11Device* device, bool keydown)
{

	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 
	if (keydown && (!m_terrainGeneratedToggle))
	{
		int index;
		float height = 0.0;
		float vec[2];

		perlin_x += 1.0f;
		perlin_y += 1.0f;


		//loop through the terrain and set the hieghts how we want. This is where we generate the terrain
		//in this case I will run a sin-wave through the terrain in one axis.

		for (int j = 0; j<m_terrainHeight; j++)
		{
			for (int i = 0; i<m_terrainWidth; i++)
			{
				index = (m_terrainHeight * j) + i;

				vec[0] = (i + perlin_x) / 2;
				vec[1] = (j + perlin_y) / 2;

				m_heightMap[index].x = (float)i;
				m_heightMap[index].y -= perlin.noise2(vec) * 2.0f;
				m_heightMap[index].z = (float)j;

				//magic numbers ahoy, just to ramp up the height of the sin function so its visible.
				//(float)(cos((float)i/(m_terrainWidth/12))*3.0) + (float)(cos((float)j / (m_terrainWidth / 12))*3.0); 
			}
		}

		ActualSmooth(device, keydown);
		//Smooth(device, keydown);
		//Smooth(device, keydown);

		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}




	return true;
}

float TerrainClass::OctavePerlin(float x, float y, int octaves, float persistence)
{
	float vec[2];
	float total = 0;
	float frequency = 10;
	float amplitude = 0.44f;
	float maxValue = 0;  // Used for normalizing result to 0.0 - 1.0

	for (int i = 0; i<octaves; i++)
	{
		vec[0] = x / 100.0f*frequency;
		vec[1] = y / 100.0f*frequency;

		total += perlin.noise2(vec) * amplitude;

		maxValue += amplitude;

		amplitude *= persistence;
		frequency *= 2.0f;
	}

	return total / maxValue;

}

bool TerrainClass::GenerateHeightMapTest(ID3D11Device* device, bool keydown)
{

	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 
	if (keydown && (!m_terrainGeneratedToggle))
	{
		int index;
		float height = 0.0;
		float vec[2];

		perlin_x += 1.0f;
		perlin_y += 1.0f;
		float weighting = 1.0f;
		float perlinMap;
		float islandAmount;
		float current_max = 1.0f, current_min = 0.0f;
		float perlin_array[128][128];


		for (int j = 0; j<m_terrainHeight; j++)
		{
			for (int i = 0; i<m_terrainWidth; i++)
			{
				index = (m_terrainHeight * j) + i;
				vec[0] = ((i + 5.0f) / 50.0f);
				vec[1] = ((j + 5.0f) / 50.0f);

				perlinMap = OctavePerlin(i, j, 2, 0.25f);
				//perlinMap = perlin.noise2(vec);

				islandAmount = Island((float)j, (float)i, (float)64, (float)64, 64.0f);


				perlin_array[j][i] = (((2 * islandAmount) - 1) - 0.0806051f) * perlinMap;


				m_heightMap[index].y += perlin_array[j][i];


				//magic numbers ahoy, just to ramp up the height of the sin function so its visible.
				//(float)(cos((float)i/(m_terrainWidth/12))*3.0) + (float)(cos((float)j / (m_terrainWidth / 12))*3.0); 

			}
		}

		//ActualSmooth(device, keydown);
		//Smooth(device, keydown);
		//Smooth(device, keydown);
		//CapHeights(device, keydown, 0.0f, false);


		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}




	return true;
}

bool TerrainClass::GrowIsland(ID3D11Device* device, bool keydown)
{
	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 
	if (keydown && (!m_terrainGeneratedToggle))
	{
		int index;
		float height = 0.0;
		float vec[2];

		perlin_x += 1.0f;
		perlin_y += 1.0f;
		float weighting = 1.0f;
		float perlinMap;
		float islandAmount;
		float current_max = 1.0f, current_min = 0.0f;
		float perlin_array[128][128];


		for (int j = 0; j<m_terrainHeight; j++)
		{
			for (int i = 0; i<m_terrainWidth; i++)
			{
				index = (m_terrainHeight * j) + i;
				vec[0] = ((i + 5.0f) / 50.0f);
				vec[1] = ((j + 5.0f) / 50.0f);

				//perlinMap = OctavePerlin(i, j, 2, 0.25f);
				//perlinMap = perlin.noise2(vec);

				islandAmount = Island((float)j, (float)i, (float)64, (float)64, 64.0f);


				perlin_array[j][i] = (((2 * islandAmount) - 1) - 0.0806051f);// *perlinMap;


				m_heightMap[index].y += perlin_array[j][i];


				//magic numbers ahoy, just to ramp up the height of the sin function so its visible.
				//(float)(cos((float)i/(m_terrainWidth/12))*3.0) + (float)(cos((float)j / (m_terrainWidth / 12))*3.0); 

			}
		}

		//ActualSmooth(device, keydown);
		//Smooth(device, keydown);
		//Smooth(device, keydown);
		//CapHeights(device, keydown, 0.0f, false);


		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}




	return true;
}
bool TerrainClass::GenerateRidgedHeightMap(ID3D11Device* device, bool keydown, bool inverse_ridges, bool multiply_perlin)
{

	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 
	if (keydown && (!m_terrainGeneratedToggle))
	{
		int index;
		float height = 0.0;
		float vec[2], vec2[2];

		perlin_x += 1.0f;
		perlin_y += 1.0f;
		float weighting = 1.0f;
		float perlinMap;
		float ridgedNoise = 0.0f;
		float current_max = 1.0f, current_min = 0.0f;
		float perlin_array[128][128];


		for (int j = 0; j<m_terrainHeight; j++)
		{
			for (int i = 0; i<m_terrainWidth; i++)
			{
				index = (m_terrainHeight * j) + i;

				vec[0] = ((i + 5.0f) / 40.0f);
				vec[1] = ((j + 5.0f) / 40.0f);

				vec2[0] = ((i + 5.0f) / 20.0f);
				vec2[1] = ((j + 5.0f) / 20.0f);

				//perlinMap = perlin.noise2(vec2);
				perlinMap = OctavePerlin(i, j, 4, 0.25f);

				ridgedNoise = (1.0f - abs(perlin.noise2(vec) * 2.0f));

				if (inverse_ridges)
				{
					if (multiply_perlin)
					{
						m_heightMap[index].y += (1.0f - (ridgedNoise*4.0f)) * perlinMap;// -0.0806051f;
					}
					else
					{
						m_heightMap[index].y += (1.0f - (ridgedNoise*4.0f)) + perlinMap;// -0.0806051f;
					}
				}
				else
				{
					if (multiply_perlin)
					{
						m_heightMap[index].y += ((ridgedNoise*4.0f)) * perlinMap;// -0.0806051f;
					}
					else
					{
						m_heightMap[index].y += ((ridgedNoise*2.0f)) + perlinMap - 1.0806051f;
					}
				}

				//magic numbers ahoy, just to ramp up the height of the sin function so its visible.
				//(float)(cos((float)i/(m_terrainWidth/12))*3.0) + (float)(cos((float)j / (m_terrainWidth / 12))*3.0); 

			}
		}

		////ActualSmooth(device, keydown);
		//Smooth(device, keydown);
		//Smooth(device, keydown);
		//CapHeights(device, keydown, -1.0f, false);

		result = CalculateNormals();
		if (!result)
		{
			return false;
		}
		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}
		m_terrainGeneratedToggle = true;
	}
	else
	{
		m_terrainGeneratedToggle = false;
	}

	return true;
}

float TerrainClass::GetMaxHeight()
{
	float max_height = 10.0f;
	float min_height = 0.0f;
	int index;
	float current_max = 0.0f;
	float current_min = 0.0f;
	float average_height = 0.0f;

	for (int j = 0; j < m_terrainHeight; j++)
	{
		for (int i = 0; i < m_terrainWidth; i++)
		{
			index = (m_terrainHeight * j) + i;
			current_max = m_heightMap[index].y;

			if (current_max > max_height)
			{
				max_height = current_max;
			}

			current_min = m_heightMap[index].y;
			if (current_min < min_height)
			{
				min_height = current_min;
			}

			average_height += m_heightMap[index].y;

		}
	}

	average_height /= (m_terrainHeight*m_terrainWidth);
	if (max_height < 1)
	{
		max_height = 1.0f;
	}


	return max_height - min_height;
}

float TerrainClass::Island(float x, float y, float centerx, float centery, float radius)
{

	//Calculate the parameter
	float dx = centerx - x;
	float dy = centery - y;
	float dist = sqrt((dx*dx) + (dy*dy));

	if (dist > radius)
	{
		dist = radius;
	}

	//float testx = x / (float)radius * 2 - 1;
	//float testy = y / (float)radius * 2 - 1;
	//float value = max(abs(x), abs(y));

	float param = (dist / radius)*90.0f;

	param = (param / 180.0f)*3.14159265359f;

	float height = cos(sin(param));

	return height;


}



bool TerrainClass::LoadHeightMap(char* filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;


	// Open the height map file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Save the dimensions of the terrain.
	m_terrainWidth = bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = m_terrainWidth * m_terrainHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Create the structure to hold the height map data.
	m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	if (!m_heightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (m_terrainHeight * j) + i;

			m_heightMap[index].x = (float)i;
			m_heightMap[index].y = (float)height / 2.0f;
			m_heightMap[index].z = (float)j;

			k += 3;
		}
	}

	// Release the bitmap image data.
	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}


void TerrainClass::NormalizeHeightMap()
{
	int i, j;


	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			m_heightMap[(m_terrainHeight * j) + i].y /= 15.0f;
		}
	}

	return;
}


bool TerrainClass::CalculateNormals()
{
	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	VectorType* normals;


	// Create a temporary array to hold the un-normalized normal vectors.
	normals = new VectorType[(m_terrainHeight - 1) * (m_terrainWidth - 1)];
	if (!normals)
	{
		return false;
	}

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j<(m_terrainHeight - 1); j++)
	{
		for (i = 0; i<(m_terrainWidth - 1); i++)
		{
			index1 = (j * m_terrainHeight) + i;
			index2 = (j * m_terrainHeight) + (i + 1);
			index3 = ((j + 1) * m_terrainHeight) + i;

			// Get three vertices from the face.
			vertex1[0] = m_heightMap[index1].x;
			vertex1[1] = m_heightMap[index1].y;
			vertex1[2] = m_heightMap[index1].z;

			vertex2[0] = m_heightMap[index2].x;
			vertex2[1] = m_heightMap[index2].y;
			vertex2[2] = m_heightMap[index2].z;

			vertex3[0] = m_heightMap[index3].x;
			vertex3[1] = m_heightMap[index3].y;
			vertex3[2] = m_heightMap[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (m_terrainHeight - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if ((i < (m_terrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if ((i < (m_terrainWidth - 1)) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * m_terrainHeight) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			m_heightMap[index].nx = (sum[0] / length);
			m_heightMap[index].ny = (sum[1] / length);
			m_heightMap[index].nz = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete[] normals;
	normals = 0;

	return true;
}


void TerrainClass::ShutdownHeightMap()
{
	if (m_heightMap)
	{
		delete[] m_heightMap;
		m_heightMap = 0;
	}

	return;
}


bool TerrainClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index1, index2, index3, index4;
	float tu, tv;

	// Calculate the number of vertices in the terrain mesh.
	m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize the index to the vertex buffer.
	index = 0;

	// Load the vertex and index array with the terrain data.
	for (j = 0; j<(m_terrainHeight - 1); j++)
	{
		for (i = 0; i<(m_terrainWidth - 1); i++)
		{

			index1 = (m_terrainHeight * j) + i;          // Bottom left.
			index2 = (m_terrainHeight * j) + (i + 1);      // Bottom right.
			index3 = (m_terrainHeight * (j + 1)) + i;      // Upper left.
			index4 = (m_terrainHeight * (j + 1)) + (i + 1);  // Upper right.

															 // Upper left.
			tv = m_heightMap[index3].tv;

			// Modify the texture coordinates to cover the top edge.
			if (tv == 1.0f) { tv = 0.0f; }

			vertices[index].position = XMFLOAT3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
			vertices[index].texture = XMFLOAT2(m_heightMap[index3].tu, tv);
			vertices[index].normal = XMFLOAT3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
			indices[index] = index;
			index++;

			// Upper right.
			tu = m_heightMap[index4].tu;
			tv = m_heightMap[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) { tu = 1.0f; }
			if (tv == 1.0f) { tv = 0.0f; }

			vertices[index].position = XMFLOAT3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
			vertices[index].texture = XMFLOAT2(tu, tv);
			vertices[index].normal = XMFLOAT3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
			vertices[index].texture = XMFLOAT2(m_heightMap[index1].tu, m_heightMap[index1].tv);
			vertices[index].normal = XMFLOAT3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
			vertices[index].texture = XMFLOAT2(m_heightMap[index1].tu, m_heightMap[index1].tv);
			vertices[index].normal = XMFLOAT3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
			indices[index] = index;
			index++;

			// Upper right.
			tu = m_heightMap[index4].tu;
			tv = m_heightMap[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) { tu = 1.0f; }
			if (tv == 1.0f) { tv = 0.0f; }

			vertices[index].position = XMFLOAT3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
			vertices[index].texture = XMFLOAT2(tu, tv);
			vertices[index].normal = XMFLOAT3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
			indices[index] = index;
			index++;

			// Bottom right.
			tu = m_heightMap[index2].tu;

			// Modify the texture coordinates to cover the right edge.
			if (tu == 0.0f) { tu = 1.0f; }

			vertices[index].position = XMFLOAT3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
			vertices[index].texture = XMFLOAT2(tu, m_heightMap[index2].tv);
			vertices[index].normal = XMFLOAT3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
			indices[index] = index;
			index++;


			/*
			if (i % 2 == 0)
			{

			if (j % 2 == 0)
			{
			index4 = (m_terrainHeight * j) + i;          // Bottom left.
			index3 = (m_terrainHeight * j) + (i + 1);      // Bottom right.
			index2 = (m_terrainHeight * (j + 1)) + i;      // Upper left.
			index1 = (m_terrainHeight * (j + 1)) + (i + 1);  // Upper right.
			}
			else
			{
			index3 = (m_terrainHeight * j) + i;          // Bottom left.
			index1 = (m_terrainHeight * j) + (i + 1);      // Bottom right.
			index4 = (m_terrainHeight * (j + 1)) + i;      // Upper left.
			index2 = (m_terrainHeight * (j + 1)) + (i + 1);  // Upper right.
			}
			}
			else
			{
			if (j % 2 == 0)
			{
			index2= (m_terrainHeight * j) + i;          // Bottom left.
			index4= (m_terrainHeight * j) + (i + 1);      // Bottom right.
			index1 = (m_terrainHeight * (j + 1)) + i;      // Upper left.
			index3 = (m_terrainHeight * (j + 1)) + (i + 1);  // Upper right.

			}
			else
			{	/*
			index4 = (m_terrainHeight * j) + i;          // Bottom left.
			index1 = (m_terrainHeight * (j + 1)) + (i + 1);  // Upper right.
			index2 = (m_terrainHeight * (j + 1)) + i;      // Upper left.
			index3 = (m_terrainHeight * j) + (i + 1);      // Bottom right.
			*/
			/*
			index1 = (m_terrainHeight * j) + i;          // Bottom left.
			index4 = (m_terrainHeight * (j + 1)) + (i + 1);  // Upper right.
			index3 = (m_terrainHeight * (j + 1)) + i;      // Upper left.
			index2 = (m_terrainHeight * j) + (i + 1);      // Bottom right.

			}
			}

			// Upper left.
			tv = m_heightMap[index3].tv;

			// Modify the texture coordinates to cover the top edge.
			if (tv == 1.0f) { tv = 0.0f; }

			vertices[index].position = D3DXVECTOR3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
			vertices[index].texture = D3DXVECTOR2(m_heightMap[index3].tu, tv);
			vertices[index].normal = D3DXVECTOR3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
			indices[index] = index;
			index++;


			// Upper right.
			tu = m_heightMap[index4].tu;
			tv = m_heightMap[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) { tu = 1.0f; }
			if (tv == 1.0f) { tv = 0.0f; }

			vertices[index].position = D3DXVECTOR3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
			vertices[index].texture = D3DXVECTOR2(tu, tv);
			vertices[index].normal = D3DXVECTOR3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
			indices[index] = index;
			index++;


			// Bottom left.
			vertices[index].position = D3DXVECTOR3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
			vertices[index].texture = D3DXVECTOR2(m_heightMap[index1].tu, m_heightMap[index1].tv);
			vertices[index].normal = D3DXVECTOR3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
			indices[index] = index;
			index++;


			// Bottom left.
			vertices[index].position = D3DXVECTOR3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
			vertices[index].texture = D3DXVECTOR2(m_heightMap[index1].tu, m_heightMap[index1].tv);
			vertices[index].normal = D3DXVECTOR3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
			indices[index] = index;
			index++;


			// Upper right.
			tu = m_heightMap[index4].tu;
			tv = m_heightMap[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) { tu = 1.0f; }
			if (tv == 1.0f) { tv = 0.0f; }

			vertices[index].position = D3DXVECTOR3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
			vertices[index].texture = D3DXVECTOR2(tu, tv);
			vertices[index].normal = D3DXVECTOR3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
			indices[index] = index;
			index++;


			// Bottom right.
			tu = m_heightMap[index2].tu;
			// Modify the texture coordinates to cover the right edge.
			if (tu == 0.0f) { tu = 1.0f; }

			vertices[index].position = D3DXVECTOR3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
			vertices[index].texture = D3DXVECTOR2(tu, m_heightMap[index2].tv);
			vertices[index].normal = D3DXVECTOR3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
			indices[index] = index;
			index++;
			*/
		}
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void TerrainClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void TerrainClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}