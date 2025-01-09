
// Public Domain Code
// Author: Xavier Canals

#include "MeshLoader.hpp"



#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>



namespace finalPractice
{
    const std::string MeshLoader::vertexShaderCode =

        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec3 vertex_color;"
        ""
        "out vec3 front_color;"
        ""
        "void main()"
        "{"
        "   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "   front_color = vertex_color;"
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



    MeshLoader::MeshLoader(const std::string& meshFilePath) :
        shader(vertexShaderCode, fragmentShaderCode)
    {
        modelViewMatrixId  = glGetUniformLocation(shader.getID(), "model_view_matrix");
        projectionMatrixID = glGetUniformLocation(shader.getID(), "projection_matrix");

        loadMesh(meshFilePath);
    }

    MeshLoader::~MeshLoader()
    {
        glDeleteVertexArrays(1, &vaoID);
        glDeleteBuffers(VBO_COUNT, vboIDs);
    }



    void MeshLoader::render(const Camera & camera)
    {
        shader.Use();

        glm::mat4 modelViewMatrix(1);

        modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0.f, -3.5f, -4.f));
        modelViewMatrix = glm::scale    (modelViewMatrix, glm::vec3(1.f,  1.f,   1.f));

        modelViewMatrix = camera.getTransformMatrixInverse() * modelViewMatrix;

        glUniformMatrix4fv(modelViewMatrixId, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

        glBindVertexArray(vaoID);
        glDrawElements(GL_TRIANGLES, numIndex, GL_UNSIGNED_SHORT, 0);
    }

    void MeshLoader::resize(int width, int height)
    {
        glm::mat4 projectionMatrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 5000.f);

        glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
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

            std::vector< glm::vec3 > vertexColor(numVertex);

            for (auto& color : vertexColor)
                color = glm::vec3(.3f, .3f, .3f);

            glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COLORS]);
            glBufferData(GL_ARRAY_BUFFER, vertexColor.size() * sizeof(glm::vec3), vertexColor.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

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
}
