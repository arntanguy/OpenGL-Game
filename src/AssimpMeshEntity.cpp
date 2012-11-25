#include "AssimpMeshEntity.h"
#include "Texture.h"


AssimpMeshEntity::AssimpMeshEntity() : Entity()
{
}

AssimpMeshEntity::AssimpMeshEntity(std::string modelName) : Entity()
{
	init(modelName);
}


AssimpMeshEntity::~AssimpMeshEntity(void)
{
}

void AssimpMeshEntity::init(std::string modelName)
{
	basePath = "assets/models/";   //<--- Base path to where the models are included.
	this->modelName = modelName;
	configured = false;

	scene = modelImporter.ReadFile((basePath + modelName), aiProcessPreset_TargetRealtime_Quality | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs );// If the import failed, report it
	if( !scene)
	{
		std::cout << modelImporter.GetErrorString() << std::endl;
	}
	LoadGLTextures();

}

bool AssimpMeshEntity::generate()
{
    return true;
}

bool AssimpMeshEntity::render()
{
    return this->render(1.f);
}
bool AssimpMeshEntity::render(float scale)
{
	return this->render(scene->mRootNode, scale);
}

bool AssimpMeshEntity::render(const aiNode* node, float scale)
{
	unsigned int i = 0,
				 t = 0,
				 n = 0;
	aiMatrix4x4 aiM = node->mTransformation;
	aiM.Scaling(aiVector3D(scale, scale, scale), aiM);

	aiM.Transpose();
	glPushMatrix();
	glMultMatrixf((float*)&aiM);

	for(; n < node->mNumMeshes; ++n)
	{
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[n]];

		applyMaterial(scene->mMaterials[mesh->mMaterialIndex]);

		if(mesh->mNormals == NULL)
		{
			glDisable(GL_LIGHTING);
		}
		else
		{
			glEnable(GL_LIGHTING);
		}

		if(mesh->mColors[0] != NULL)
		{
			glEnable(GL_COLOR_MATERIAL);
		}
		else
		{
			glDisable(GL_COLOR_MATERIAL);
		}

		for(unsigned int t = 0; t < mesh->mNumFaces; ++t)
		{
			const aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices)
			{
				case 1:
					face_mode = GL_POINTS;
					break;
				case 2:
					face_mode = GL_LINES;
					break;
				case 3:
					face_mode = GL_TRIANGLES;
					break;
				case 4:
					face_mode = GL_POLYGON;
					break;
			}

			glBegin(face_mode);

			for(unsigned int i = 0; i < face->mNumIndices; ++i)
			{
				int vertexIndex = face->mIndices[i];

				if(mesh->mColors[0] != NULL)
				{
					Color4f(&mesh->mColors[0][vertexIndex]);
				}
				if(mesh->mNormals != NULL)
				{
					if(mesh->HasTextureCoords(0))
					{
						glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y);
					}
					glNormal3fv(&mesh->mNormals[vertexIndex].x);
				}
				glVertex3fv(&mesh->mVertices[vertexIndex].x);
			}

			glEnd();
		}
	}

	// draw all children
	for(unsigned int n = 0; n < node->mNumChildren; ++n)
	{
		this->render(node->mChildren[n], scale);
	}

	glPopMatrix();
    return true;
}

void AssimpMeshEntity::applyMaterial(const aiMaterial* mat)
{
	float c[4];

	GLenum fillMode;
	unsigned int max = 0;
	int ret1 = 0,
		ret2 = 0,
		twoSided = 0,
		wireframe = 0,
		texIndex = 0;
	aiColor4D diffuse,
			  specular,
			  ambient,
			  emission;
	float shininess = 0.0f,
		  strength = 0.0f;

	aiString texPath;

	if(AI_SUCCESS ==  mat->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
	{
        Texture *tex = textureMap[texPath.data];
        tex->bind();
	}

	c[0] = 0.8f;
        c[1] = 0.8f;
        c[2] = 0.8f;
        c[3] = 1.0f;

	if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
	{
		c[0] = diffuse.r;
		c[1] = diffuse.g;
		c[2] = diffuse.b;
		c[3] = diffuse.a;
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	c[0] = 0.0f;
        c[1] = 0.0f;
        c[2] = 0.0f;
        c[3] = 1.0f;

	if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular))
	{
		c[0] = specular.r;
		c[1] = specular.g;
		c[2] = specular.b;
		c[3] = specular.a;
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	c[0] = 0.2f;
        c[1] = 0.2f;
        c[2] = 0.2f;
        c[3] = 1.0f;

	if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &ambient))
	{
		c[0] = ambient.r;
		c[1] = ambient.g;
		c[2] = ambient.b;
		c[3] = ambient.a;
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	c[0] = 0.0f;
        c[1] = 0.0f;
        c[2] = 0.0f;
        c[3] = 1.0f;

	if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_EMISSIVE, &emission))
	{
		c[0] = emission.r;
		c[1] = emission.g;
		c[2] = emission.b;
		c[3] = emission.a;
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS, &shininess, &max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);

	if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
	{
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
	}
	else
	{
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		c[0] = 0.0f;
                c[1] = 0.0f;
                c[2] = 0.0f;
                c[3] = 0.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mat, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
	{
		fillMode = wireframe ? GL_LINE : GL_FILL;
	}
	else
	{
		fillMode = GL_FILL;
	}
	glPolygonMode(GL_FRONT_AND_BACK, fillMode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mat, AI_MATKEY_TWOSIDED, &twoSided, &max)) && twoSided)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

}

int AssimpMeshEntity::LoadGLTextures()
{
    dinf << "AssimpMeshEntity::LoadGLTextures" << std::endl;
//	ILboolean success;
//
//	if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
//	{
//		ILint test = ilGetInteger(IL_VERSION_NUM);
//		//wrong DevIL version
//		std::string error = "Wrong DevIL version";
//		char* cError = (char*) error.c_str();
//		//glfwCloseWindow();
//		return -1;
//	}
//
//	ilInit();
//
	if(scene->HasTextures())
	{
        dinf << "Scene has textures" << std::endl;
		//glfwCloseWindow();
	}
	for(unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;
		aiString path; //filename
		while(texFound == AI_SUCCESS)
		{
			texFound = scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
            Texture *texture = new Texture(path.C_Str());
            dinf << "Loading texture " << path.C_Str() << std::endl;
			textureMap[path.data] = texture;
			texIndex++;
		}
    }
//
//	for(unsigned int i = 0; i < scene->mNumMaterials; ++i)
//	{
//		int texIndex = 0;
//		aiReturn texFound = AI_SUCCESS;
//		aiString path; //filename
//
//		while(texFound == AI_SUCCESS)
//		{
//			texFound = scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
//			textureIdMap[path.data] = NULL;
//			texIndex++;
//		}
//	}
//
//	int numTextures = textureIdMap.size();
//
//	ILuint* imageIds = NULL;
//	imageIds = new ILuint[numTextures];
//
//	/* generate DevIL Image IDs */
//	ilGenImages(numTextures, imageIds); /* Generation of numTextures image names */
//
//	/* create and fill array with GL texture ids */
//	textureIds = new GLuint[numTextures];
//	glGenTextures(numTextures, textureIds); /* Texture name generation */
//
//	/* define texture path */
//	//std::string texturepath = "../../../test/models/Obj/";
//
//	/* get iterator */
//	std::map<std::string, GLuint*>::iterator itr = textureIdMap.begin();
//
//	for (int i=0; i<numTextures; i++)
//	{
//
//		//save IL image ID
//		std::string filename = (*itr).first;  // get filename
//		(*itr).second =  &textureIds[i];	  // save texture id for filename in map
//		itr++;								  // next texture
//
//
//		ilBindImage(imageIds[i]); /* Binding of DevIL image name */
//		std::string fileloc = basePath + filename;	/* Loading of image */
//		success = ilLoadImage(fileloc.c_str());
//
//		if (success) /* If no error occured: */
//		{
//			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); /* Convert every colour component into
//			unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
//			if (!success)
//			{
//				/* Error occured */
//				std::cout << "Couldn't convert image." << std::endl;
//				//glfwCloseWindow();
//				return -1;
//			}
//			//glGenTextures(numTextures, &textureIds[i]); /* Texture name generation */
//			glBindTexture(GL_TEXTURE_2D, textureIds[i]); /* Binding of texture name */
//			//redefine standard texture values
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
//			interpolation for magnification filter */
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
//			interpolation for minifying filter */
//			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
//				ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
//				ilGetData()); /* Texture specification */
//		}
//		else
//		{
//			/* Error occured */
//			std::cout << "Couldn't load image:" << fileloc << std::endl;
//			//MessageBox(NULL, ("Couldn't load Image: " + fileloc).c_str() , "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		}
//
//
//	}
//
//	ilDeleteImages(numTextures, imageIds); /* Because we have already copied image data into texture data
//	we can release memory used by image. */
//
//	//Cleanup
//	delete [] imageIds;
//	imageIds = NULL;
//
//	//return success;
	return true;
}

void AssimpMeshEntity::Color4f(const aiColor4D* color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}
