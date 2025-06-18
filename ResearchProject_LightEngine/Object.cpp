#include "Object.h"

void CObject::LoadObject(ID3D11Device* device, const std::string& filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (scene == nullptr)
	{

		MessageBoxW(NULL, L"ERROR : Failed to create scene from file", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	ProcessNode(device, scene->mRootNode, scene);
}

void CObject::ProcessNode(ID3D11Device* device, aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(ProcessMesh(device, mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(device, node->mChildren[i], scene);
	}
}

CMesh<SVertexDataTextureNormal> CObject::ProcessMesh(ID3D11Device* device, aiMesh* mesh, const aiScene* scene)
{
	std::vector<SVertexDataTextureNormal> vertices;
	std::vector<unsigned short> indices;

	//Get vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		SVertexDataTextureNormal vertex;

		vertex.m_position.x = mesh->mVertices[i].x;
		vertex.m_position.y = mesh->mVertices[i].y;
		vertex.m_position.z = mesh->mVertices[i].z;

		vertex.m_normal.x = mesh->mNormals[i].x;
		vertex.m_normal.y = mesh->mNormals[i].y;
		vertex.m_normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.m_texureCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.m_texureCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	//Get indices
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]); // explicit int to short conversion
		}
	}
	return CMesh<SVertexDataTextureNormal>(device, vertices, indices);
}

CObject::CObject()
{
	m_transformsCB.m_worldTransform = DirectX::XMMatrixIdentity();
	m_transformsCB.m_worldViewProjectionTransform = DirectX::XMMatrixIdentity();

	m_worldMatrix = DirectX::XMMatrixIdentity();
	m_position = { 0.0f, 0.0f, 0.0f };
	m_rotation = { 0.0f, 0.0f, 0.0f };
}

float* CObject::GetPositionAddress()
{
	return &m_position.x;
}

const DirectX::XMFLOAT3& CObject::GetPosition()
{
	return m_position;
}

float* CObject::GetRotationAddress()
{
	return &m_rotation.x;
}

void CObject::SetPosition(float x, float y, float z)
{
	m_position = DirectX::XMFLOAT3(x, y, z);
}

void CObject::CreateBuffersVIC(ID3D11Device* device, const std::string& objectFilePAth)
{
	// create constant buffer Vertex
	HRESULT hr = m_constantBufferVS.CreateConstantBuffer(device);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create constant buffer for Vertex Shader", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// load object
	LoadObject(device, objectFilePAth);
}

void CObject::Update(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix)
{
	//Update Constant buffer with WVP Matrix
	m_transformsCB.m_worldViewProjectionTransform = DirectX::XMMatrixTranspose(m_worldMatrix * viewMatrix * projectionMatrix); // to convert to raw_major operation
	m_transformsCB.m_worldTransform = DirectX::XMMatrixTranspose(m_worldMatrix);

	m_constantBufferVS.UpdateAndMapData(deviceContext, m_transformsCB); // update constant buffer VS
	deviceContext->VSSetConstantBuffers(0, 1, m_constantBufferVS.GetAddressOfCB());

	for (auto& mesh : m_meshes)
	{
		mesh.Update(deviceContext);
	}
}

void CObject::UpdateWorldMatrix()
{
	// translation should be multiplied at last
	m_worldMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
		DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
}

void CObject::UpdateWorldMatrix(const DirectX::XMMATRIX& extraMatrixForWorld)
{
	// translation should be multiplied at last
	m_worldMatrix = extraMatrixForWorld *
		DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
		DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
}