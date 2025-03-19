
/*
    Public Domain Code

    Author: Xavier Canals
*/

#include "Postprocess.hpp"



#include <cassert>
#include <glad/glad.h>



namespace finalPractice
{
    const std::string Postprocess::postprocessVertexShaderCode =

        "#version 330\n"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec2 vertex_texture_uv;"
        ""
        "out vec2 texture_uv;"
        ""
        "void main()"
        "{"
        "   gl_Position = vec4(vertex_coordinates, 1.0);"
        "   texture_uv  = vertex_texture_uv;"
        "}";

    const std::string Postprocess::postprocessFragmentShaderCode =

        "#version 330\n"
        ""
        "uniform sampler2D sampler2d;"
        ""
        "in  vec2 texture_uv;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "    vec3 color = texture (sampler2d, texture_uv.st).rgb;"
        "    float i = (color.r + color.g + color.b) * 0.3333333333;"
        "    fragment_color = vec4(vec3(i, i, i) * vec3(1.0, 0.75, 0.5), 1.0);"
        "}";

    

    Postprocess::Postprocess(int _windowWidth, int _windowHeight) :
        shader(postprocessVertexShaderCode, postprocessFragmentShaderCode),
        windowWidth (_windowWidth),
        windowHeight(_windowHeight)
    {
        buildFramebuffer();
    }

    Postprocess::~Postprocess()
    {
        glDeleteVertexArrays(1, &framebufferQuadVAO);
        glDeleteBuffers     (2, framebufferQuadVBOs);
    }



    void Postprocess::buildFramebuffer()
    {
        // Framebuffer creation
        glGenFramebuffers(1, &framebufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

        //Frambuffer's texture creation
        glGenTextures(1, &outTextureID);
        glBindTexture(GL_TEXTURE_2D, outTextureID);

        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            framebufferWidth,
            framebufferHeight,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            0
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Z-buffer creation
        glGenRenderbuffers(1, &depthbufferID);
        glBindRenderbuffer(GL_RENDERBUFFER, depthbufferID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, framebufferWidth, framebufferHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbufferID);

        // Framebuffer configuration
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, outTextureID, 0);

        const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;

        glDrawBuffers(1, &draw_buffer);

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Quad's mesh creation for rendering
        static const GLfloat quadPositions[] =
        {
            +1.0f, -1.0f, 0.0f,
            +1.0f, +1.0f, 0.0f,
            -1.0f, +1.0f, 0.0f,
            -1.0f, +1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            +1.0f, -1.0f, 0.0f,
        };

        static const GLfloat quadTextureUVs[] =
        {
            +1.0f,  0.0f,
            +1.0f, +1.0f,
             0.0f, +1.0f,
             0.0f, +1.0f,
             0.0f,  0.0f,
             1.0f,  0.0f,
        };

        glGenVertexArrays(1, &framebufferQuadVAO);
        glGenBuffers(2, framebufferQuadVBOs);

        glBindVertexArray(framebufferQuadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, framebufferQuadVBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadPositions), quadPositions, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, framebufferQuadVBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadTextureUVs), quadTextureUVs, GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    void Postprocess::renderFramebuffer()
    {
        glViewport(0, 0, windowWidth, windowHeight);

        // Activates window's framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glUseProgram(shader.getID());

        // Activates texture's framebuffer and renders the window
        glBindTexture(GL_TEXTURE_2D, outTextureID);

        glBindVertexArray(framebufferQuadVAO);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
