#include "Model.h"
#include <iostream>
#include <chrono>


Model::Model()
{
}

void Model::LoadModel(const std::string& fileName, char alpha)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if(!scene)
	{
		std::cout<<"[ERROR:GLCustom] Model failed to load. Error: " <<importer.GetErrorString();
		return;
	}

	LoadNode(scene->mRootNode, scene);

	if(alpha == 'A' || alpha == 'a')
	{
		auto start = std::chrono::steady_clock::now();
		std::cout<<"[P6:GLCustom] Loading Alpha channel models. ";
		LoadMaterialsAlpha(scene);
		auto end = std::chrono::steady_clock::now();
		std::cout<<"Execution Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() <<" milliseconds \n";
	}
	else if (alpha == 'n' || alpha == 'N')
	{
		auto start = std::chrono::steady_clock::now();
		std::cout<<"[P7:GLCustom] Loading Non-Alpha channel models. ";
		LoadMaterials(scene);
		auto end = std::chrono::steady_clock::now();
		std::cout<<"Execution Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() <<" milliseconds \n";
	}
	
}


void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for(size_t i = 0; i< node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i =0; i < node->mNumChildren;i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* Mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for(size_t i = 0; i < Mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { Mesh->mVertices[i].x , Mesh->mVertices[i].y , Mesh->mVertices[i].z });

		if(Mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { Mesh->mTextureCoords[0][i].x , Mesh->mTextureCoords[0][i].y});
		} 
		else
		{
			vertices.insert(vertices.end(), { 0.0 , 0.0});
		}

		vertices.insert(vertices.end(), { -Mesh->mNormals[i].x , -Mesh->mNormals[i].y , -Mesh->mNormals[i].z });
	}

	for(size_t i = 0; i < Mesh->mNumFaces; i++)
	{
		aiFace face = Mesh->mFaces[i];

		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	mesh* newMesh = new mesh();
	newMesh->createMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(newMesh);
	meshToTex.push_back(Mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if(material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("Textures/") + filename;

				textureList[i] = new Texture(texPath.c_str());

				if(!textureList[i]->loadTexture())
				{
					std::cout<<"[ERROR:GLCustom] Failed to load texture at: " <<texPath <<"\n";
					delete(textureList[i]);
					textureList[i] = nullptr;
				}
			}
		}

		if(!textureList[i])
		{
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->loadTextureA();
		}
	}
}

void Model::LoadMaterialsAlpha(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if(material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("Textures/") + filename;

				textureList[i] = new Texture(texPath.c_str());

				if(!textureList[i]->loadTextureA())
				{
					std::cout<<"[ERROR:GLCustom] Failed to load texture at: " <<texPath <<"\n";
					delete(textureList[i]);
					textureList[i] = nullptr;
				}
			}
		}

		if(!textureList[i])
		{
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->loadTextureA();
		}
	}
}


void Model::ClearModel()
{
	for (size_t i = 0 ; i < meshList.size(); i++)
	{
		if(meshList[i])
		{
			delete(meshList[i]);
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0 ; i < textureList.size(); i++)
	{
		if(textureList[i])
		{
			delete(textureList[i]);
			textureList[i] = nullptr;
		}
	}
}

void Model::RenderModel()
{
	for(size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];

		if(materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->useTexture();
		}

		meshList[i]->renderMesh();
	}
}

Model::~Model()
{
}
