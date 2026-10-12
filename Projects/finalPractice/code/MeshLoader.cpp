
/*
    Public Domain Code

    Author: Xavier Canals
*/

#include "MeshLoader.hpp"



#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>



namespace finalPractice
{
    // Vertex shader for non-textured meshes. Positions and normals are passed in
    // view space so the Blinn-Phong calculation can be performed per fragment.
    const std::string MeshLoader::vertexShaderCode =

        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        "uniform mat3 normal_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 2) in vec3 vertex_normal;"
        ""
        "out vec3 view_position;"
        "out vec3 view_normal;"
        ""
        "void main()"
        "{"
        "    vec4 position = model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "    gl_Position = projection_matrix * position;"
        "    view_position = position.xyz;"
        "    view_normal = normalize(normal_matrix * vertex_normal);"
        "}";

    // Blinn-Phong fragment shader for meshes without an albedo texture.
    const std::string MeshLoader::fragmentShaderCode =

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
        "uniform float specular_intensity;"
        "uniform float attenuation_constant;"
        "uniform float attenuation_linear;"
        "uniform float attenuation_quadratic;"
        ""
        "uniform vec3 material_color;"
        "uniform float material_ambient;"
        "uniform float material_diffuse;"
        "uniform float material_specular;"
        "uniform float material_shininess;"
        "uniform float transparency;"
        ""
        "in vec3 view_position;"
        "in vec3 view_normal;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "    vec3 normal = normalize(view_normal);"
        "    vec3 to_light = light.position.xyz - view_position;"
        "    float light_distance = length(to_light);"
        "    vec3 light_direction = normalize(to_light);"
        "    vec3 view_direction = normalize(-view_position);"
        "    vec3 halfway_direction = normalize(light_direction + view_direction);"
        ""
        "    float diffuse_factor = max(dot(normal, light_direction), 0.0);"
        "    float specular_factor = 0.0;"
        "    if (diffuse_factor > 0.0)"
        "        specular_factor = pow(max(dot(normal, halfway_direction), 0.0), material_shininess);"
        ""
        "    float attenuation = 1.0 / (attenuation_constant"
        "        + attenuation_linear * light_distance"
        "        + attenuation_quadratic * light_distance * light_distance);"
        ""
        "    vec3 ambient = ambient_intensity * material_ambient * material_color;"
        "    vec3 diffuse = diffuse_intensity * material_diffuse * diffuse_factor * light.color * material_color;"
        "    vec3 specular = specular_intensity * material_specular * specular_factor * light.color;"
        "    fragment_color = vec4(ambient + attenuation * (diffuse + specular), transparency);"
        "}";
    
    // Vertex shader code for rendering with lighting and textures
    const std::string MeshLoader::vertexShaderCodeTexture =

        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        "uniform mat3 normal_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec2 vertex_texture_uv;"
        "layout (location = 2) in vec3 vertex_normal;"
        ""
        "out vec2 texture_uv;"
        "out vec3 view_position;"
        "out vec3 view_normal;"
        ""
        "void main()"
        "{"
        "    vec4 position = model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "    gl_Position = projection_matrix * position;"
        "    texture_uv  = vertex_texture_uv;"
        "    view_position = position.xyz;"
        "    view_normal = normalize(normal_matrix * vertex_normal);"
        "}";

    // Blinn-Phong fragment shader using the texture as the material albedo.
    const std::string MeshLoader::fragmentShaderCodeTexture =

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
        "uniform float specular_intensity;"
        "uniform float attenuation_constant;"
        "uniform float attenuation_linear;"
        "uniform float attenuation_quadratic;"
        ""
        "uniform sampler2D sampler;"
        "uniform float material_ambient;"
        "uniform float material_diffuse;"
        "uniform float material_specular;"
        "uniform float material_shininess;"
        "uniform float transparency;"
        ""
        "in vec2 texture_uv;"
        "in vec3 view_position;"
        "in vec3 view_normal;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "    vec4 tex_color = texture(sampler, texture_uv);"
        "    vec3 normal = normalize(view_normal);"
        "    vec3 to_light = light.position.xyz - view_position;"
        "    float light_distance = length(to_light);"
        "    vec3 light_direction = normalize(to_light);"
        "    vec3 view_direction = normalize(-view_position);"
        "    vec3 halfway_direction = normalize(light_direction + view_direction);"
        ""
        "    float diffuse_factor = max(dot(normal, light_direction), 0.0);"
        "    float specular_factor = 0.0;"
        "    if (diffuse_factor > 0.0)"
        "        specular_factor = pow(max(dot(normal, halfway_direction), 0.0), material_shininess);"
        ""
        "    float attenuation = 1.0 / (attenuation_constant"
        "        + attenuation_linear * light_distance"
        "        + attenuation_quadratic * light_distance * light_distance);"
        ""
        "    vec3 ambient = ambient_intensity * material_ambient * tex_color.rgb;"
        "    vec3 diffuse = diffuse_intensity * material_diffuse * diffuse_factor * light.color * tex_color.rgb;"
        "    vec3 specular = specular_intensity * material_specular * specular_factor * light.color;"
        "    vec3 lit_color = ambient + attenuation * (diffuse + specular);"
        "    fragment_color = vec4(lit_color, tex_color.a * transparency);"
        "}";



    // MeshLoader constructor for mesh without texture
    MeshLoader::MeshLoader(const std::string& meshFilePath, float _transparency) :
        shader(vertexShaderCode, fragmentShaderCode),
        angle(0),
        posY (0),
        moveDown(false),
        transparency(_transparency)
    {
        shader.use();

        modelViewMatrixID  = glGetUniformLocation(shader.getID(), "model_view_matrix");
        projectionMatrixID = glGetUniformLocation(shader.getID(), "projection_matrix");
        normalMatrixID     = glGetUniformLocation(shader.getID(), "normal_matrix"    );

        needTexture = false;                                                            // Indicates that the mesh doesn't need a texture
        
        loadMesh(meshFilePath);                                                         // Load the mesh
        configureMaterial(shader.getID());                                              // Sets its Blinn-Phong material
    }

    // MeshLoader constructor for mesh with texture
    MeshLoader::MeshLoader(const std::string& meshFilePath, const std::string& texturePath, float _transparency) :
        shader(vertexShaderCodeTexture, fragmentShaderCodeTexture),
        angle(0),
        posY (.1f),
        moveDown(false),
        transparency(_transparency)
    {
        shader.use();

        modelViewMatrixID  = glGetUniformLocation(shader.getID(), "model_view_matrix");
        projectionMatrixID = glGetUniformLocation(shader.getID(), "projection_matrix");
        normalMatrixID     = glGetUniformLocation(shader.getID(), "normal_matrix"    );

        needTexture = true;                                                             // Indicates that the mesh needs a texture

        loadMesh(meshFilePath);                                                         // Load the mesh

        // Sets the texture (albedo) for the mesh
        texture.setID(texture.createTexture2D< Rgba8888 >(texturePath, Texture::TypeTexture2D::ALBEDO));
        assert(texture.isOk());
        configureMaterial(shader.getID());                                              // Sets its Blinn-Phong material
    }

    MeshLoader::~MeshLoader()
    {
        glDeleteVertexArrays(1, &vaoID);
        glDeleteBuffers(VBO_COUNT, vboIDs);
    }



    void MeshLoader::update()
    {
        crystalAnimation(); // Crystal animation (Used in Scene.cpp by the crystal mesh)
    }
    
    void MeshLoader::render(const Camera & camera, glm::vec3 tanslateVector, float angle, glm::vec3 rotateVector, glm::vec3 scaleVector)
    {
        if (transparency < 1.f)
        {
            glDepthMask(GL_FALSE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        shader.use();

        glm::mat4 modelViewMatrix(1);

        // Sets the mesh's transform values
        modelViewMatrix = glm::translate(modelViewMatrix,      tanslateVector);
        modelViewMatrix = glm::rotate   (modelViewMatrix, angle, rotateVector);
        modelViewMatrix = glm::scale    (modelViewMatrix,         scaleVector);

        const glm::mat4 viewMatrix = camera.getTransformMatrixInverse();
        modelViewMatrix = viewMatrix * modelViewMatrix;
        const glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));

        glUniformMatrix4fv(modelViewMatrixID , 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
        glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        lighting.configureLight(shader.getID(), viewMatrix);

        if (needTexture)
            texture.bind();

        glUniform1f(glGetUniformLocation(shader.getID(), "transparency"), transparency);

        glBindVertexArray(vaoID);
        glDrawElements(GL_TRIANGLES, numIndex, GL_UNSIGNED_SHORT, 0);
        
        if(transparency < 1.f)
        {
            glDisable(GL_BLEND);
            glDepthMask(GL_TRUE);
        }
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

        shader.use();

        auto scene = importer.ReadFile
        (
            meshFilePath,
            aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
        );

        if (scene && scene->mNumMeshes > 0)
        {
            auto mesh = scene->mMeshes[0];

            glGenBuffers(VBO_COUNT, vboIDs);
            glGenVertexArrays(1, &vaoID);

            glBindVertexArray(vaoID);

            static_assert(sizeof(aiVector3D) == sizeof(glm::fvec3), "aiVector3D should composed of three floats");

            // MESH VERTEX COORDINATES
            size_t numVertex = mesh->mNumVertices;
            glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COORDINATES]);
            glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(aiVector3D), mesh->mVertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            // MESH TEXTURE COORDINATES
            if (needTexture)
            {
                if (not mesh->HasTextureCoords(0))
                {
                    std::cerr << "Mesh doesn't have UV coordinates" << std::endl;
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
            }

            // MESH VERTEX NORMALS
            if (not mesh->HasNormals()) // ERROR condition
                std::cerr << "Mesh doesn't have normals" << std::endl;
            else
            {
                // Sets the mesh normals
                std::vector< glm::vec3 > normals(numVertex);

                for (unsigned i = 0; i < numVertex; ++i)
                    normals[i] = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

                glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_NORMALS]);
                glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            }

            // MESH INDEXES
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
        const bool glossy = transparency < 1.f;

        glUniform3f(glGetUniformLocation(shaderID, "material_color"), 1.f, 1.f, 1.f);
        glUniform1f(glGetUniformLocation(shaderID, "material_ambient"), 1.f);
        glUniform1f(glGetUniformLocation(shaderID, "material_diffuse"), .9f);
        glUniform1f(glGetUniformLocation(shaderID, "material_specular"), glossy ? .85f : .35f);
        glUniform1f(glGetUniformLocation(shaderID, "material_shininess"), glossy ? 96.f : 32.f);
    }



    void MeshLoader::crystalAnimation()
    {
        // Rotation
        angle += 0.01f;

        // Floating effect
        if (moveDown)
        {
            posY -= 0.001f;

            if (posY <= -.1f)
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
