#pragma once

// templated
#include "Mesh.h"
#include "Mesh.cpp"
#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"
#include "Shaders.h"
#include <string>

// assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class CObject
{
private:
	// mesh
	std::vector<CMesh<SVertexDataTextureNormal>> m_meshes;

	// consatnt buffer vertex shader data
	STransformsCB m_transformsCB;
	CConstantBuffer<STransformsCB> m_constantBufferVS;

	//world matrix of object
	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotation;

private:

	void LoadObject(ID3D11Device* device, const std::string& filePath);
	void ProcessNode(ID3D11Device* device, aiNode* node, const aiScene* scene);
	CMesh<SVertexDataTextureNormal> ProcessMesh(ID3D11Device* device, aiMesh* mesh, const aiScene* scene);

public:
	// constructor
	CObject();

	// getter
	const DirectX::XMFLOAT3& GetPosition();
	float* GetPositionAddress(); // for imgui
	float* GetRotationAddress(); // for imgui

	// setter
	void SetPosition(float x, float y, float z);


	// functions
	void CreateBuffersVIC(ID3D11Device* device, const std::string& objectFilePAth);
	void Update(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix);

	void UpdateWorldMatrix();
	void UpdateWorldMatrix(const DirectX::XMMATRIX& extraMatrixForWorld);
};