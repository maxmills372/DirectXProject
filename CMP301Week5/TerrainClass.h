////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
//#include <d3dx10math.h>
#include <stdio.h>
#include "Perlin.h"
//#include "RayCollision.h"
//#include "quickVect.h"
#include <math.h>
#include <time.h>
//#include "textureclass.h"
#include <vector>
#include <random>
#include <DirectXMath.h>
using namespace DirectX;

const int texture_repeat = 8;

////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
public:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct HeightMapType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};



public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	bool InitializeTerrain(ID3D11Device*, int terrainWidth, int terrainHeight, WCHAR* Texture1Filename, WCHAR* Texture2Filename,
		WCHAR* Texture3Filename, WCHAR* waterTextureFilename, WCHAR* grainTextureFilename);

	void Shutdown();
	void Render(ID3D11DeviceContext*);

	bool GenerateHeightMap(ID3D11Device* device, bool keydown);
	int  GetIndexCount();

	bool ActualSmooth(ID3D11Device* device, bool keydown);
	bool RandomPoint(ID3D11Device* device, bool keydown, bool negative_effect);
	bool ResetHeight(ID3D11Device* device, bool keydown);
	bool GetCanMove();
	bool ParticleDeposition(ID3D11Device* device, bool keydown, int point, bool inverse);
	bool GenerateHeightMapSmall(ID3D11Device* device, bool keydown);
	bool GenerateHeightMapTest(ID3D11Device* device, bool keydown);
	bool CapHeights(ID3D11Device* device, bool keydown, float min_height, bool test);
	bool FaultLines(ID3D11Device* device, bool keydown, float disp);
	bool IterateFaultLines(ID3D11Device* device, bool keydown, int iterations);
	bool Octaves(ID3D11Device* device, bool keydown);
	bool RemoveTerrain(ID3D11Device* device, bool keydown);
	float OctavePerlin(float x, float y, int octaves, float persistence);
	float Island(float x, float y, float centerx, float centery, float radius);
	bool Circle(ID3D11Device* device, bool keydown, int x, int y);
	bool GenerateRidgedHeightMap(ID3D11Device* device, bool keydown, bool inverse_ridges, bool multiply_perlin);
	bool GrowIsland(ID3D11Device* device, bool keydown);
	bool MoveTerrain(ID3D11Device* device, bool keydown, float amount);
	bool GenerateHeightMapWater(ID3D11Device* device);
	float GetMaxHeight();


	void CalculateTextureCoordinates();
	bool LoadTextures(ID3D11Device* device, WCHAR* Texture1Filename, WCHAR* Texture2Filename, WCHAR* Texture3Filename, WCHAR* waterTextureFilename, WCHAR* grainTextureFilename);
	void ReleaseTextures();

	ID3D11ShaderResourceView* GetGrassTexture();
	ID3D11ShaderResourceView* GetSlopeTexture();
	ID3D11ShaderResourceView* GetRockTexture();
	ID3D11ShaderResourceView* GetWaterTexture();
	ID3D11ShaderResourceView* GetGrainTexture();

	float water_height;


private:
	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void ShutdownHeightMap();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	bool m_terrainGeneratedToggle;
	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	HeightMapType* m_heightMap;

	Perlin perlin;
	float perlin_x, perlin_y;

//RayCollision ray_collision;
	bool can_move;

	//TextureClass *m_GrassTexture, *m_SlopeTexture, *m_RockTexture, *m_WaterTexture, *m_GrainTexture;

};

#endif