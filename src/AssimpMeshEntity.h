#ifndef MESH_H
#define MESH_H

#include <GL/glut.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <map>
#include <iostream>
#include "Debug.h"
#include "Entity.h"

class Texture;

class AssimpMeshEntity : public Entity
{
	public:
		AssimpMeshEntity();
		AssimpMeshEntity(std::string modelName);
		~AssimpMeshEntity(void);
		void init(std::string modelName);
		bool render(float scale);
        // Virtual
        virtual bool render();
        virtual bool generate();

		void applyMaterial(const aiMaterial* mat);
		int LoadGLTextures();

	private:
		void Color4f(const aiColor4D* color);
		bool render(const aiNode* node, float scale);
		bool configured;
		const aiScene *scene;
		std::map<std::string, Texture*> textureMap;
		GLuint* textureIds;
		std::string basePath,
					modelName;
		Assimp::Importer modelImporter;
};

#endif
