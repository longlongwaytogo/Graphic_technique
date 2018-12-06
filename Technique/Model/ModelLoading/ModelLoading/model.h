#pragma once
#ifdef USE_MY

#include <vector>
#include <string>
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>




class Model 
{
    public:
        /*  函数   */
     Model(std::string const & path, bool gamma = false) : gammaCorrection(gamma)
    {
        this->loadModel(path);
    }

        
        void Draw(Shader shader);   
    private:
        /*  模型数据  */
		std::vector<Texture> _textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
  
        std::vector<Mesh> _meshes;
        std::string _directory;
		 bool gammaCorrection;
public:
        /*  函数   */
        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             std::string typeName);
};
 /*  Model Data */
  
    
#endif