/******************************************************************************
  Copyright (C) 2012-2013  TANGUY Arnaud arn.tanguy@gmail.com
 *                                                                             *
 * This program is free software; you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by        *
 * the Free Software Foundation; either version 2 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * This program is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                *
 * GNU General Public License for more details.                                *
 *                                                                             *
 * You should have received a copy of the GNU General Public License along     *
 * with this program; if not, write to the Free Software Foundation, Inc.,     *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
 ******************************************************************************/

// See
// http://ogldev.atspace.co.uk/www/tutorial22/tutorial22.html
// for original code and explanations

#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <vector>
#include <map>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>

struct aiScene;
struct aiMesh;
struct Vertex;
class Texture;

class AssimpMeshEntity : public Entity
{
    public:
        AssimpMeshEntity();
        ~AssimpMeshEntity();

        bool LoadMesh(const std::string& Filename);

        virtual bool render();
        virtual bool generate() { return true; }

    private:
        bool InitFromScene(const aiScene* pScene, const std::string& Filename);
        void InitMesh(unsigned int Index, const aiMesh* paiMesh);
        bool InitMaterials(const aiScene* pScene, const std::string& Filename);
        void Clear();

#define INVALID_MATERIAL 0xFFFFFFF0
#define INVALID_OGL_VALUE 0xFFFFFFF1

        struct MeshEntry {
            MeshEntry();
            ~MeshEntry();

            bool Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices);

            GLuint VB;
            GLuint IB;
            unsigned int NumIndices;
            unsigned int MaterialIndex;
        };

        std::vector<MeshEntry> m_Entries;
        std::vector<Texture *> m_Textures;
};
