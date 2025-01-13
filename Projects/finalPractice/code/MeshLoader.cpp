
// Public Domain Code
// Author: Xavier Canals

#include "MeshLoader.hpp"



#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <vector>



namespace finalPractice
{
    const std::string MeshLoader::vertexShaderCode =

        "#version 330\n"
        ""
        "struct Light"
        "{"
        "    vec4 position;"
        "    vec3 color;"
        "};"
        ""
        "uniform Light light;"
        "uniform float ambient_intensity;"
        "uniform float diffuse_intensity;"
        ""
        "uniform vec3 material_color;"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        "uniform mat4 normal_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 2) in vec3 vertex_normal;"
        ""
        "out vec3 front_color;"
        ""
        "void main()"
        "{"
        "    vec4 normal   = normal_matrix * vec4(vertex_normal, 0.0);"
        "    vec4 position = model_view_matrix * vec4(vertex_coordinates, 1.0);"
        ""
        "    vec4  light_direction = light.position - position;"
        "    float light_intensity = diffuse_intensity * max(dot(normalize(normal.xyz), normalize(light_direction.xyz)), 0.0);"
        ""
        "    gl_Position = projection_matrix * position;"
        "    front_color = material_color * ambient_intensity + diffuse_intensity * light_intensity * light.color * material_color;"
        "}";

    const std::string MeshLoader::fragmentShaderCode =

        "#version 330\n"
        ""
        "in  vec3    front_color;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "    fragment_color = vec4(front_color, 1.0);"
        "}";
    
    const std::string MeshLoader::vertexShaderCodeTexture =

        "#version 330\n"
        ""
        "struct Light"
        "{"
        "    vec4 position;"
        "    vec3 color;"
        "};"
        ""
        "uniform Light light;"
        "uniform float ambient_intensity;"
        "uniform float diffuse_intensity;"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        "uniform mat4 normal_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec2 vertex_texture_uv;"
        "layout (location = 2) in vec3 vertex_normal;"
        ""
        "out vec2 texture_uv;"
        ""
        "void main()"
        "{"
        "    vec4 normal   = normal_matrix * vec4(vertex_normal, 0.0);"
        "    vec4 position = model_view_matrix * vec4(vertex_coordinates, 1.0);"
        ""
        "    vec4  light_direction = light.position - position;"
        "    float light_intensity = diffuse_intensity * max(dot(normalize(normal.xyz), normalize(light_direction.xyz)), 0.0);"
        ""
        "   gl_Position = projection_matrix * position;"
        "   texture_uv  = vertex_texture_uv;"
        "}";

    const std::string MeshLoader::fragmentShaderCodeTexture =

        "#version 330\n"
        ""
        "uniform sampler2D sampler;"
        ""
        "in  vec2     texture_uv;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "    fragment_color = texture(sampler, texture_uv);"
        "}";



    MeshLoader::MeshLoader(const std::string& meshFilePath) :
        shader(vertexShaderCode, fragmentShaderCode),
        angle(0),
        posY (0),
        moveDown(false)
    {
        shader.Use();

        modelViewMatrixID  = glGetUniformLocation(shader.getID(), "model_view_matrix");
        projectionMatrixID = glGetUniformLocation(shader.getID(), "projection_matrix");
        normalMatrixID     = glGetUniformLocation(shader.getID(), "normal_matrix"    );

        needTexture = false;
        loadMesh(meshFilePath);

        lighting.configureLight(shader.getID());
    }

    MeshLoader::MeshLoader(const std::string& meshFilePath, const std::string& texturePath) :
        shader(vertexShaderCodeTexture, fragmentShaderCodeTexture),
        angle(0),
        posY (0),
        moveDown(false)
    {
        modelViewMatrixID  = glGetUniformLocation(shader.getID(), "model_view_matrix");
        projectionMatrixID = glGetUniformLocation(shader.getID(), "projection_matrix");
        normalMatrixID     = glGetUniformLocation(shader.getID(), "normal_matrix"    );

        needTexture = true;
        loadMesh(meshFilePath);

        texture.setID(texture.createTexture2D< Rgba8888 >(texturePath, Texture::TypeTexture2D::ALBEDO));
        assert(texture.isOk());

        lighting.configureLight(shader.getID());
    }

    MeshLoader::~MeshLoader()
    {
        glDeleteVertexArrays(1, &vaoID);
        glDeleteBuffers(VBO_COUNT, vboIDs);
    }



    void MeshLoader::update()
    {
        crystalAnimation();
    }
    
    void MeshLoader::render(const Camera & camera, glm::vec3 tanslateVector, float angle, glm::vec3 rotateVector, glm::vec3 scaleVector)
    {
        shader.Use();

        glm::mat4 modelViewMatrix(1);

        modelViewMatrix = glm::translate(modelViewMatrix,      tanslateVector);
        modelViewMatrix = glm::rotate   (modelViewMatrix, angle, rotateVector);
        modelViewMatrix = glm::scale    (modelViewMatrix,         scaleVector);

        modelViewMatrix = camera.getTransformMatrixInverse() * modelViewMatrix;

        glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

        if (needTexture)
            texture.bind();

        glBindVertexArray(vaoID);
        glDrawElements(GL_TRIANGLES, numIndex, GL_UNSIGNED_SHORT, 0);
    }

    void MeshLoader::resize(int width, int height)
    {
        glm::mat4 projectionMatrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 5000.f);

        glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }

    float MeshLoader::getAngle()
    {
        return angle;
    }

    float MeshLoader::getPosY()
    {
        return posY;
    }



    void MeshLoader::loadMesh(const std::string & meshFilePath)
    {
        Assimp::Importer importer;

        shader.Use();

        auto scene = importer.ReadFile
        (
            meshFilePath,
            aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
        );

        if (scene && scene->mNumMeshes > 0)
        {
            auto mesh = scene->mMeshes[0];

            size_t numVertex = mesh->mNumVertices;

            glGenBuffers(VBO_COUNT, vboIDs);
            glGenVertexArrays(1, &vaoID);

            glBindVertexArray(vaoID);

            static_assert(sizeof(aiVector3D) == sizeof(glm::fvec3), "aiVector3D should composed of three floats");

            glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COORDINATES]);
            glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(aiVector3D), mesh->mVertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            if (not mesh->HasTextureCoords(0))
                std::cerr << "Mesh doesn't have UV coordinates" << std::endl;
            else if (not needTexture)
            {
                /*std::vector< glm::vec3 > vertex_colors(numVertex);

                for (auto& color : vertex_colors)
                {
                    color = glm::vec3(.3f, .3f, .3f);
                }

                glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COLORS]);
                glBufferData(GL_ARRAY_BUFFER, vertex_colors.size() * sizeof(glm::vec3), vertex_colors.data(), GL_STATIC_DRAW);

                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);*/
                configureMaterial(shader.getID());
            }
            else
            {
                std::vector< glm::vec2 > textureCoords(mesh->mNumVertices);

                for (unsigned i = 0; i < mesh->mNumVertices; ++i)
                {
                    textureCoords[i] = glm::vec2
                    (
                        mesh->mTextureCoords[0][i].x,
                        1.f - mesh->mTextureCoords[0][i].y
                    );
                }

                glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COLORS]);
                glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2), textureCoords.data(), GL_STATIC_DRAW);

                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            }

            if (not mesh->HasNormals())
                std::cerr << "Mesh doesn't have normals" << std::endl;
            else
            {
                std::vector< glm::vec3 > normals(numVertex);

                for (unsigned i = 0; i < numVertex; ++i)
                    normals[i] = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

                glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_NORMALS]);
                glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            }

            numIndex = mesh->mNumFaces * 3;

            std::vector< GLshort > index(numIndex);

            auto vertexIndex = index.begin();

            for (unsigned i = 0; i < mesh->mNumFaces; ++i)
            {
                auto& face = mesh->mFaces[i];

                assert(face.mNumIndices == 3);

                *vertexIndex++ = face.mIndices[0];
                *vertexIndex++ = face.mIndices[1];
                *vertexIndex++ = face.mIndices[2];
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[EBO_INDEX]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLshort), index.data(), GL_STATIC_DRAW);
        }
    }

    void MeshLoader::configureMaterial(GLuint shaderID)
    {
        GLint materialColor = glGetUniformLocation(shaderID, "material_color");
        
        glUniform3f(materialColor, 0.f, 1.f, 0.f);
    }



    void MeshLoader::crystalAnimation()
    {
        angle += 0.01f;

        if (moveDown)
        {
            posY -= 0.001f;

            if (posY <= -0.2f)
                moveDown = false;
        }
        else if (not moveDown)
        {
            posY += 0.001f;

            if (posY >= 0.2f)
                moveDown = true;
        }
    }
}
