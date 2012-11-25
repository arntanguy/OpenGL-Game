#ifndef MESH_H
#define MESH_H

#include <GL/glut.h>
//#include <GL\glfw.h> //Use GLFW to open a window cross-platform and render with OpenGL

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
//#include <IL/il.h>
#include <map>
#include <iostream>
#include "Debug.h"

class Texture;

class Mesh
{
	public:
		Mesh();
		Mesh(std::string modelName);
		~Mesh(void);
		void Init(std::string modelName);
		void Render(float scale);
		void ApplyMaterial(const aiMaterial* mat);
		int LoadGLTextures();
	private:
		void Color4f(const aiColor4D* color);
		void Render(const aiNode* node, float scale);
		bool configured;
		const aiScene *scene;
		std::map<std::string, Texture*> textureMap;
		GLuint* textureIds;
		std::string basePath,
					modelName;
		Assimp::Importer modelImporter;
};

#endif
