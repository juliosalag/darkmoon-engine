#include "entity_model.hpp"
#include <set>

namespace DarkMoon {
    void Model::load(const char* filePath, ResourceManager& rm) {
        // Read file
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_FlipWindingOrder);
        if (!scene || !scene->mRootNode) {
            std::cerr << "[ERROR ASSIMP] : " << importer.GetErrorString() << std::endl;
            return;
        }

        m_loaded = true;
        m_name = filePath;

        std::cout << " - Load a model -> " << m_name << std::endl;

        scene->mRootNode->mTransformation = aiMatrix4x4();
        processNode(scene->mRootNode, scene, rm);
    };

    void Model::unload() {
        //ResourceManager rm = ResourceManager::getInstance();
        //std::cout << "Unload -> " << this->m_name << std::endl; 
        //for(int i=0; i<static_cast<int>(m_meshes.size()); i++)
        //    rm.unloadResource(m_meshes[i]->getID());
    }

    void Model::draw(glm::mat4 transMatrix) {

        // Draw meshes
        if (drawModel)
            for (const auto& mesh : m_meshes) {
                if (hasBones && animID != maxID)
                    mesh->animID = animID;

                mesh->draw(transMatrix, color);

                if (hasBones && animID != maxID)
                    mesh->animID = maxID;
            }

        // Draw wires
        if (drawWires)
            for (const auto& mesh : m_meshes)
                mesh->drawLines(transMatrix);

    };

    // PRIVATE

    void Model::processNode(aiNode* node, const aiScene* scene, ResourceManager& rm) {
        // Find the directory of the obj file
        std::string directory = m_name;
        size_t last_slash_idx = directory.find_last_of('/');
        if (std::string::npos != last_slash_idx)
            directory = directory.substr(0, last_slash_idx);

        //mesh_base_vertex.resize(scene->mNumMeshes);
        // Process all the node's meshes
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            //auto name = scene->mMeshes[0]->mName;
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex];
            //std::cout << "Material: " << aiMaterial->GetName().C_Str() << std::endl;
           // mesh_base_vertex[i] = 0;
            processMesh(mesh, aiMaterial, scene, rm);
        }

        // Recursively process each of the node's children
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            // aiMesh* mesh = scene->mMeshes[];
            // mesh_base_vertex[i] = mesh->mNumVertices;
            processNode(node->mChildren[i], scene, rm);
        }
    }

    void Model::processMesh(aiMesh* mesh, aiMaterial* mat, const aiScene* scene, ResourceManager& rm) {
        // Process the vertices
        std::vector<Vertex> vertices(mesh->mNumVertices);
        std::vector<glm::ivec4> boneIDs(mesh->mNumVertices);
        std::vector<glm::vec4> weights(mesh->mNumVertices);
        //m_Bones.resize(mesh->mNumVertices);

        // Loop all vertices in loaded mesh
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            glm::ivec4 boneID;
            glm::vec4 weight;

            // Set default values
            for (int i = 0; i < 4; i++)
            {
                boneID[i] = -1;
                weight[i] = 0.0f;
            }

            boneIDs[i] = boneID;
            weights[i] = weight;

            glm::vec3 vector;
            // Set positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertices[i].position = vector;

            if (mesh->HasNormals())
            {
                // Set normals
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertices[i].normal = vector;
            }

            if (mesh->mTextureCoords[0])
            {
                // Set texture coords
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertices[i].textCoords = vec;
                if (mesh->HasTangentsAndBitangents()) {
                    // Set tangent
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertices[i].Tangent = vector;
                    // Set bitangent
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertices[i].BiTangent = vector;
                }
            }
        }

        // Process the indices
        std::vector<uint16_t> indices;

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(static_cast<unsigned short>(face.mIndices[j]));
            }
        }

        //Process bones
        bool meshHasBones = mesh->HasBones();
        if (meshHasBones)
        {
            processBone(boneIDs, weights, mesh, scene);
            hasBones = true;

            for (std::size_t i = 0; i < vertices.size(); i++) {
                vertices[i].boneIDs = boneIDs[i];
                vertices[i].weights = weights[i];
            }
        }

        //std::cout << mat->GetName().C_Str() << "\n";
        //std::cout << mesh->mName.C_Str() << "\n";

        // Process material
        auto material = processMaterial(mat, rm);
        processTextures(mat, material, rm);

        std::string name = std::string(m_name) + mesh->mName.C_Str();
        std::string meshName = mesh->mName.C_Str();
        auto currentMesh = rm.loadResource<Mesh>(name.c_str(), vertices, indices, material, meshName, meshHasBones);

        m_meshes.push_back(currentMesh);
    }

    Material* Model::processMaterial(aiMaterial* aiMaterial, ResourceManager& rm) {
        aiColor4D ambient(0.0f, 0.0f, 0.0f, 1.0f);
        aiColor4D diffuse(0.0f, 0.0f, 0.0f, 1.0f);
        aiColor4D specular(0.0f, 0.0f, 0.0f, 1.0f);
        float shininess = 0.0f;

        // Retrieve material properties
        aiGetMaterialColor(aiMaterial, AI_MATKEY_COLOR_AMBIENT, &ambient);
        aiGetMaterialColor(aiMaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
        aiGetMaterialColor(aiMaterial, AI_MATKEY_COLOR_SPECULAR, &specular);
        aiGetMaterialFloat(aiMaterial, AI_MATKEY_SHININESS, &shininess);

        ambient.r = ambient.r == 0 ? 1.0f : ambient.r;
        ambient.g = ambient.g == 0 ? 1.0f : ambient.g;
        ambient.b = ambient.b == 0 ? 1.0f : ambient.b;

        //std::cout << ambient.r << " | " << ambient.g << " | " << ambient.b << "\n";

        // Create and return Material object
        std::string name = std::string(m_name) + aiMaterial->GetName().C_Str();
        //auto material = rm.loadResource<Material>(aiMaterial->GetName().C_Str());
        auto material = rm.loadResource<Material>(name.c_str(),
            glm::vec3(ambient.r, ambient.g, ambient.b),
            glm::vec3(diffuse.r, diffuse.g, diffuse.b),
            glm::vec3(specular.r, specular.g, specular.b),
            shininess);

        return material;
    }

    void Model::processTextures(aiMaterial* aiMaterial, Material* material, ResourceManager& rm) {
        aiString aiTexturePath;
        aiReturn texFound = aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexturePath);
        if (texFound == AI_SUCCESS) {
            std::string texturePath = aiTexturePath.C_Str();
            std::replace(texturePath.begin(), texturePath.end(), '\\', '/');
            std::string modelDirectory = m_name;
            size_t lastSlashIndex = modelDirectory.find_last_of('/');
            if (lastSlashIndex != std::string::npos) {
                modelDirectory = modelDirectory.substr(0, lastSlashIndex);
            }
            texturePath = modelDirectory + "/" + texturePath;

            auto texture = rm.loadResource<Texture>(texturePath.c_str());
            //texture->load(texturePath.c_str());

            material->texture = texture;
        }
        else
            material->texture = nullptr;
    }

    glm::mat4 Model::getBoneTransform(const std::string& name) {
        for (auto& bone : boneVector) {
            if (bone.name == name)
                return bone.offset;
        }
        return glm::mat4(1.0f);
    }

    void Model::processBone(std::vector<glm::ivec4>& boneIDs_all, std::vector<glm::vec4>& weights_all, aiMesh* mesh, const aiScene*)
    {
        // Set the max bones to 100
        unsigned int numBones = mesh->mNumBones > 100 ? 100 : mesh->mNumBones;
        // For each bone
        for (unsigned int boneIndex = 0; boneIndex < numBones; ++boneIndex)
        {
            int boneID = -1;
            std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
            if (boneIndex >= boneVector.size()) {
                glm::mat4 offset = aiMatrix4x4ToGlm(&mesh->mBones[boneIndex]->mOffsetMatrix);
                //std::cout << "\n";
                boneVector.push_back({ boneName, offset });
                boneID = static_cast<int>(boneIndex);
                m_BoneCounter++;
            }
            else {
                for (unsigned int i = 0; i < boneVector.size(); i++) {
                    if (boneVector[i].name == boneName) {
                        boneID = static_cast<int>(i);
                        break;
                    }
                }
            }
            assert(boneID != -1);

            // Get all vertex weights for current bone
            aiVertexWeight* weights = mesh->mBones[boneIndex]->mWeights;
            unsigned int numWeights = mesh->mBones[boneIndex]->mNumWeights;

            // For each weight at vertex x for current bone
            for (unsigned int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
            {
                unsigned int vertexId = weights[weightIndex].mVertexId;
                float weight = weights[weightIndex].mWeight;
                assert(vertexId <= boneIDs_all.size());

                // Update four most influential bones
                for (int i = 0; i < 4; ++i)
                {
                    if (boneIDs_all[vertexId][i] < 0)
                    {
                        weights_all[vertexId][i] = weight;
                        boneIDs_all[vertexId][i] = boneID;
                        break;
                    }
                }
            }
        }
    }
}