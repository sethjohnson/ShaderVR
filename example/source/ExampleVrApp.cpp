/*
 * Copyright Regents of the University of Minnesota, 2020.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Danseth Orbanjohnson (dstorbanjohnson)
 */

#include <example/include/ExampleVrApp.h>
#include <fstream>

#include "Image.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Uniform.h"
#include "Texture.h"
using std::ifstream;
using std::ios;
#define REPORT(thing) std::cout <<" REPORT AT " << __LINE__ << ": " << thing << std::endl;
#define DEBUG_ON 1



using namespace MinVR;
using glm::vec3;
using glm::vec2;



void ExampleVrApp::initShader(int threadId)
{
    
    for(auto const &shdr : m_shaders) {
        shdr.second->compileToContext(threadId);
    }
    
    for(auto const &shdr : m_shaderPrograms) {
        shdr.second->linkToContext(threadId);
    }
    

    
 }


ExampleVrApp::ExampleVrApp() : MinVR::AbstractMVRApp()
{

    initData();
}

ExampleVrApp::~ExampleVrApp()
{
	for(std::map<int, GLuint>::iterator iterator = _objectIds["vbo"].begin(); iterator != _objectIds["vbo"].end(); iterator++) {
		glDeleteBuffersARB(1, &iterator->second);
	}
}

void ExampleVrApp::doUserInputAndPreDrawComputation(
		const std::vector<MinVR::EventRef>& events, double synchronizedTime) {
    
    static int frameCount = 0;
    static int lastFrameCount = frameCount;
    static double lastTime = synchronizedTime;
    double fpsDelay = 3;
    
    if (synchronizedTime > lastTime + fpsDelay) {
        std::cout << "FPS: " << (frameCount - lastFrameCount)/(synchronizedTime - lastTime) << std::endl;
        lastTime = synchronizedTime;
        lastFrameCount = frameCount;
    }
    frameCount++;
   
    _model = glm::mat4(1);
    _model = glm::scale(_model, vec3(0.3,0.3,0.3));
    _model = glm::rotate(_model, (float)(M_PI_2*synchronizedTime*1.5), vec3(0,1,0));

    _model = glm::rotate(_model, (float)(M_PI_2*synchronizedTime), vec3(1,0,0));
    _model = glm::rotate(_model, (float)M_PI/4, vec3(0,1,0));
    
    m_uniforms["model"]->setData(glm::value_ptr(_model));

}

void ExampleVrApp::initializeContextSpecificVars(int threadId,
		MinVR::WindowRef window) {
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    _mutex.lock();
    {
        
        _objectIds["FramebufferName"][threadId] = GLuint(0);

        quad.generateObjectsForContext(threadId);
        cube.generateObjectsForContext(threadId);
        
        // Initialize Shader Stages
        for(auto const &shader_i : m_shaders) {
            shader_i.second->generateObjectsForContext(threadId);
        }
        
        // Initialize Shader Programs
        for(auto const &shdrprg_i : m_shaderPrograms) {
            shdrprg_i.second->generateObjectsForContext(threadId);
        }
        
        // Initialize Shader Programs
        for(auto const &tex_i : m_textures) {
            tex_i.second->generateObjectsForContext(threadId);
        }

    }
    
    _mutex.unlock();
    
    initVBO(threadId);

    initShader(threadId);

    initGL();
    


    
    for(auto const &tex : m_textures) {
		std::cout << "Loading Texture " << tex.first << std::endl;
        tex.second->loadToContext(threadId);
    }
    
    for (int v = 0; v < window->getNumViewports(); v++) {
        _mutex.lock();
        glGenFramebuffers(1, &m_frameBuffers["FrameBufferName"][std::make_pair(threadId, v)]);
        m_frameBufferTextures["renderTexture"][std::make_pair(threadId, v)] = GLuint(0);
                _mutex.unlock();
        std::cout << "reporting first pass:\n";
        std::cout << std::make_pair(threadId, v).first << ", " << std::make_pair(threadId, v).second << std::endl;

        std::cout << (int)m_frameBuffers["FrameBufferName"][std::make_pair(threadId, v)] << std::endl;

        
        GLuint renderedTexture;
        glGenTextures(1, &m_frameBufferTextures["renderTexture"][std::make_pair(threadId, v)]);
        
        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, m_frameBufferTextures["renderTexture"][std::make_pair(threadId, v)]);
        
        // Give an empty image to OpenGL ( the last "0" )
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 400, 400, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
        
        // Poor filtering. Needed !
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffers["FrameBufferName"][std::make_pair(threadId, v)]);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_frameBufferTextures["renderTexture"][std::make_pair(threadId, v)], 0);
//        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_frameBufferTextures["depthTexture"][std::make_pair(threadId, v)]->getObjectForContext(threadId), 0);
        // Always check that our framebuffer is ok
        
        // The depth buffer
        GLuint depthrenderbuffer;
        glGenRenderbuffers(1, &depthrenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 400, 400);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
        std::cout << (int)m_frameBuffers["FrameBufferName"][std::make_pair(threadId, v)] << std::endl;
        GLenum error;
        if((error = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("BAD BUFFER\n");
            switch (error) {
                case GL_FRAMEBUFFER_UNDEFINED:
                    printf("GL_FRAMEBUFFER_UNDEFINED");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                    printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                    printf("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
                    break;
                case GL_FRAMEBUFFER_UNSUPPORTED:
                    printf("GL_FRAMEBUFFER_UNSUPPORTED");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                    printf("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
                    break;
                default:
                    break;
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    
    for (int v = 0; v < window->getNumViewports(); v++) {
        _mutex.lock();
               std::cout << "reporting second pass:\n";
        std::cout << std::make_pair(threadId, v).first << ", " << std::make_pair(threadId, v).second << std::endl;
        
        std::cout << (int)m_frameBuffers["FrameBufferName"][std::make_pair(threadId, v)] << std::endl;
       
        std::cout << (int)m_frameBuffers["FrameBufferName"][std::make_pair(threadId, v)] << std::endl;
        
        _mutex.unlock();
    }
    



    
	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "openGL ERROR in initializeContextSpecificVars: "<<err<<std::endl;
	}
}
void ExampleVrApp::postInitialization()
{
    
}
void ExampleVrApp::initGL()
{
    glClearColor(0.2, 0.2, 0.3, 1.0);
//    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

}
void ExampleVrApp::initVBO(int threadId)
{

    
    cube.loadIndicesToContext(threadId);
    cube.loadAttributesToContext(threadId);

    
    quad.loadAttributesToContext(threadId);
    quad.loadIndicesToContext(threadId);
        _mutex.lock();

//    _objectIds["texture"][threadId] = uploadTexture((unsigned char*)img->getData(), img->getImageDimensions().x, img->getImageDimensions().y);
    _mutex.unlock();
    
    
    GLenum err;
    if((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "GLERROR initGL: "<<err<<std::endl;
    }
}

void ExampleVrApp::initData()
{
    m_shaders["test.vsh"] = new Shader("../../resources/shaders/test.vsh", GL_VERTEX_SHADER);
    
    m_shaders["test.fsh"] = new Shader("../../resources/shaders/test.fsh", GL_FRAGMENT_SHADER);
    
    m_shaders["quad.vsh"] = new Shader("../../resources/shaders/quad.vsh", GL_VERTEX_SHADER);
    m_shaders["quad.fsh"] = new Shader("../../resources/shaders/quad.fsh", GL_FRAGMENT_SHADER);
    m_shaders["buffers.fsh"] = new Shader("../../resources/shaders/buffers.fsh", GL_FRAGMENT_SHADER);
    m_shaders["red.fsh"] = new Shader("../../resources/shaders/red.fsh", GL_FRAGMENT_SHADER);
    
    
    

    m_shaderPrograms["test"] = new ShaderProgram();
    m_shaderPrograms["test"]->addShader(m_shaders["test.vsh"]);
    m_shaderPrograms["test"]->addShader(m_shaders["test.fsh"]);

    m_shaderPrograms["quad"] = new ShaderProgram();
    m_shaderPrograms["quad"]->addShader(m_shaders["quad.vsh"]);
    m_shaderPrograms["quad"]->addShader(m_shaders["quad.fsh"]);
    
    m_shaderPrograms["buffers"] = new ShaderProgram();
    m_shaderPrograms["buffers"]->addShader(m_shaders["quad.vsh"]);
    m_shaderPrograms["buffers"]->addShader(m_shaders["buffers.fsh"]);
    
    m_shaderPrograms["red"] = new ShaderProgram();
    m_shaderPrograms["red"]->addShader(m_shaders["test.vsh"]);
    m_shaderPrograms["red"]->addShader(m_shaders["red.fsh"]);
    


    // cube /////////////////////////////////////////////////////
    //////////////////
    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3
    
    glm::vec3 v[] = {
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec3(-0.5f,-0.5f, 0.5f),
        glm::vec3(0.5f,-0.5f, 0.5f),
        glm::vec3(0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f, 0.5f,-0.5f),
        glm::vec3(-0.5f, 0.5f,-0.5f),
        glm::vec3(-0.5f,-0.5f,-0.5f)
    };
    glm::vec3 n[] = {
        glm::vec3(0,0,1),
        glm::vec3(1,0,0),
        glm::vec3(0,1,0),
        glm::vec3(-1,0,0),
        glm::vec3(0,-1,0),
        glm::vec3(0,0,-1)
    };
    glm::vec3 c[] = {
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(0.5,0.5,1),
        glm::vec3(0.5,0.6,0.5),
		glm::vec3(0.5, 0.75, 1),
		glm::vec3(1, 0.6, 0.5),
        glm::vec3(1,0.5,1),
		glm::vec3(1, 0.6, 0.5),
		glm::vec3(1, 0.6, 1)
    };
    
    glm::vec3 vertices_array[]  = {
        v[0], v[1], v[2], v[3],
        v[0], v[3], v[4], v[5],
        v[0], v[5], v[6], v[1],
        v[1], v[6], v[7], v[2],
        v[7], v[4], v[3], v[2],
        v[4], v[7], v[6], v[5]
    };
    
    glm::vec3 normals_array[] = {
        n[0], n[0], n[0], n[0],
        n[1], n[1], n[1], n[1],
        n[2], n[2], n[2], n[2],
        n[3], n[3], n[3], n[3],
        n[4], n[4], n[4], n[4],
        n[5], n[5], n[5], n[5]
    };
    
    glm::vec3 colors_array[] = {
#if 0
        c[0], c[1], c[2], c[3],
        c[0], c[3], c[4], c[5],
        c[0], c[5], c[6], c[1],
        c[1], c[6], c[7], c[2],
        c[7], c[4], c[3], c[2],
        c[4], c[7], c[6], c[5]
#elif 0
        c[0], c[0], c[0], c[0],
        c[1], c[1], c[1], c[1],
        c[2], c[2], c[2], c[2],
        c[3], c[3], c[3], c[3],
        c[4], c[4], c[4], c[4],
        c[5], c[5], c[5], c[5]
#else
        c[7], c[7], c[7], c[7],
        c[7], c[7], c[7], c[7],
        c[7], c[7], c[7], c[7],
        c[7], c[7], c[7], c[7],
        c[7], c[7], c[7], c[7],
        c[7], c[7], c[7], c[7]
        
#endif
    };
    
    GLuint indices_array[] = {
        0, 1, 2, // front
        2, 3, 0,
        4, 5, 6, // right
        6, 7, 4,
        8, 9, 10, // top
        10, 11, 8,
        12, 13, 14, // left
        14, 15, 12,
        16, 17, 18, // bottom
        18, 19, 16,
        20, 21, 22, // back
        22, 23, 20
    };

    
    glm::vec2 cornerVerticesArray[] = {
        glm::vec2(1, 1),
        glm::vec2(-1, 1),
        glm::vec2(-1,-1),
        glm::vec2(1,-1)

    };

    glm::vec2 cornerTexCoordArray[] = {
        glm::vec2(1, 1),
        glm::vec2(0, 1),
        glm::vec2(0,0),
        glm::vec2(1,0)
        
    };
    
    GLuint cornerIndicesArray[] = {
        0,1,2,
        2,3,0
    };

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;
    std::vector<GLuint> indicies;

    indicies.assign(indices_array,  indices_array+  sizeof(indices_array)/  sizeof(indices_array[0]));
    vertices.assign(vertices_array, vertices_array+ sizeof(vertices_array)/ sizeof(vertices_array[0]));
    normals.assign( normals_array,  normals_array+  sizeof(normals_array)/  sizeof(normals_array[0]));
    colors.assign(  colors_array,   colors_array+   sizeof(colors_array)/   sizeof(colors_array[0]));
    cube.addIndex(indicies, "cube");
    cube.addAttribute(vertices, ATTRIB_POSITION, 3, GL_FLOAT);
    cube.addAttribute(normals, ATTRIB_NORMAL, 3, GL_FLOAT);
    cube.addAttribute(colors, ATTRIB_COLOR, 3, GL_FLOAT);

    
    lightPositions.push_back(glm::vec3(0.1,1,0.1));
    lightColors.push_back(glm::vec3(1,1,1));

    lightPositions.push_back(glm::vec3(0.1,-1,0.1));
    lightColors.push_back(glm::vec3(1,0.5,0.5));

    
    std::vector<GLuint> cornerIndicies;
    std::vector<glm::vec2> cornerVertices;
    std::vector<glm::vec2> cornerTexCoords;

    cornerIndicies.assign(cornerIndicesArray, cornerIndicesArray + sizeof(cornerIndicesArray)/sizeof(cornerIndicesArray[0]));
    cornerVertices.assign(cornerVerticesArray, cornerVerticesArray + sizeof(cornerVerticesArray)/sizeof(cornerVerticesArray[0]));
    cornerTexCoords.assign(cornerTexCoordArray, cornerTexCoordArray + sizeof(cornerTexCoordArray)/sizeof(cornerTexCoordArray[0]));
    
    
    quad.addIndex(cornerIndicies, "quad");
    quad.addAttribute(cornerVertices, ATTRIB_POSITION, 2, GL_FLOAT);
    quad.addAttribute(cornerTexCoords, ATTRIB_TEXCOORD, 2, GL_FLOAT);
    
    
    
    m_uniforms["lightPositions"] = new Uniform3fv(lightPositions.data(), lightPositions.size());
    m_uniforms["lightPositions"]->tetherToShader(m_shaderPrograms["test"], "lightPositions");
    
    m_uniforms["lightColors"] = new Uniform3fv(lightColors.data(), lightColors.size());
    m_uniforms["lightColors"]->tetherToShader(m_shaderPrograms["test"], "lightColors");
    size_t lightCount = (lightPositions.size());
    m_uniforms["lightCount"] = new Uniform1iv(&lightCount, 1);
    m_uniforms["lightCount"]->tetherToShader(m_shaderPrograms["test"], "lightCount");

    
    m_uniforms["view"] = new UniformMatrix4fv(nullptr, 1, false);
    m_uniforms["model"] = new UniformMatrix4fv(nullptr, 1, false);
    m_uniforms["projection"] = new UniformMatrix4fv(nullptr, 1, false);
    
    m_uniforms["view"]->tetherToShader(m_shaderPrograms["test"], "view");
    m_uniforms["model"]->tetherToShader(m_shaderPrograms["test"], "model");
    m_uniforms["projection"]->tetherToShader(m_shaderPrograms["test"], "proj");
    m_uniforms["view"]->tetherToShader(m_shaderPrograms["red"], "view");
    m_uniforms["model"]->tetherToShader(m_shaderPrograms["red"], "model");
    m_uniforms["projection"]->tetherToShader(m_shaderPrograms["red"], "proj");
    


    m_uniforms["colorTexture"] = new Uniform1iv(0);
    m_uniforms["colorTexture"]->tetherToShader(m_shaderPrograms["buffers"], "colorTexture");
    m_uniforms["depthTexture"] = new Uniform1iv(1);
    m_uniforms["depthTexture"]->tetherToShader(m_shaderPrograms["buffers"], "depthTexture");

    m_images["gradient.bmp"] = Image::LoadFromBMP("../../resources/images/GRADIENT.bmp");
    m_uniforms["gradientTexture"] = new Uniform1iv(0);
    m_uniforms["gradientTexture"]->tetherToShader(m_shaderPrograms["quad"], "colorTexture");

    m_textures["gradientTexture"] = new ImageTexture(m_images["gradient.bmp"]);
    

    
    
    m_uniforms["thread"] = new Uniform1iv(0);
    m_uniforms["thread"]->tetherToShader(m_shaderPrograms["red"], "thread");
    m_uniforms["thread"]->tetherToShader(m_shaderPrograms["buffers"], "thread");

    m_uniforms["viewport"] = new Uniform1iv(0);
    m_uniforms["viewport"]->tetherToShader(m_shaderPrograms["red"], "viewport");
        m_uniforms["viewport"]->tetherToShader(m_shaderPrograms["buffers"], "viewport");
}


void ExampleVrApp::drawGraphics(int threadId,
		MinVR::WindowRef window, int viewportIndex) {

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    _mutex.lock();
    std::cout << "reporting third pass:\n";
    std::cout << std::make_pair(threadId, viewportIndex).first << ", " << std::make_pair(threadId, viewportIndex).second << std::endl;
    
    std::cout << (int)m_frameBuffers["FrameBufferName"][std::make_pair(threadId, viewportIndex)] << std::endl;
    
    std::cout << (int)m_frameBuffers["FrameBufferName"][std::make_pair(threadId, viewportIndex)] << std::endl;
    _mutex.unlock();

    
    MinVR::Rect2D  viewport = window->getViewport(viewportIndex);
    
    printf("%i, %i\n", window->getWidth(),window->getHeight());
    
    
    glm::mat4 proj;
    
    MinVR::CameraOffAxis* offAxisCamera = dynamic_cast<MinVR::CameraOffAxis*>(window->getCamera(viewportIndex).get());
    proj = offAxisCamera->getLastAppliedProjectionMatrix();
    
    glm::mat4 view = glm::mat4(offAxisCamera->getLastAppliedViewMatrix());
    
    
    _mutex.lock();
    m_uniforms["view"]->setDataForContext(glm::value_ptr(view), threadId);
    m_uniforms["projection"]->setDataForContext(glm::value_ptr(proj), threadId);
    m_uniforms["thread"]->setDataForContext(&threadId, 1);
    m_uniforms["viewport"]->setDataForContext(&viewportIndex, 1);

    for(auto const &uniform_i : m_uniforms) {
        uniform_i.second->uploadToContext(threadId);
    }
    _mutex.unlock();

    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(viewport.x0(), viewport.y0(), viewport.width(), viewport.height());
    glScissor(viewport.x0(), viewport.y0(), viewport.width(), viewport.height());
    
    m_shaderPrograms["red"]->useForContext(threadId);

    
    cube.bindToContext(threadId);
    glDrawElements(GL_TRIANGLES, cube.getCountForIndex("cube"), GL_UNSIGNED_INT, (void*)cube.getStartForIndex("cube"));


    std::cout << std::make_pair(threadId, viewportIndex).first << ", " << std::make_pair(threadId, viewportIndex).second << std::endl;
    std::cout << m_frameBuffers["FrameBufferName"][std::make_pair(threadId, viewportIndex)] << std::endl << std::endl;;
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffers["FrameBufferName"][std::make_pair(threadId, viewportIndex)]);
    glViewport(0, 0 , viewport.width(), viewport.height());
    glScissor(0, 0 , viewport.width(), viewport.height());

    m_shaderPrograms["quad"]->useForContext(threadId);
    glActiveTexture(GL_TEXTURE0);
    m_textures["gradientTexture"]->bindToContext(threadId);
    quad.bindToContext(threadId);
    glDrawElements(GL_TRIANGLES, quad.getCountForIndex("quad"), GL_UNSIGNED_INT, (void*)quad.getStartForIndex("quad"));
    glClear(GL_DEPTH_BUFFER_BIT);

    m_shaderPrograms["test"]->useForContext(threadId);
    cube.bindToContext(threadId);
    glDrawElements(GL_TRIANGLES, cube.getCountForIndex("cube"), GL_UNSIGNED_INT, (void*)cube.getStartForIndex("cube"));

glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(viewport.x0(), viewport.y0(), viewport.width(), viewport.height());
    glScissor(viewport.x0(), viewport.y0(), viewport.width(), viewport.height());


    m_shaderPrograms["buffers"]->useForContext(threadId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_frameBufferTextures["renderTexture"][std::make_pair(threadId, viewportIndex)]);
    
    _mutex.lock();
    std::cout << "reporting fourth pass:\n";
    std::cout << std::make_pair(threadId, viewportIndex).first << ", " << std::make_pair(threadId, viewportIndex).second << std::endl;
    
    std::cout << (int)m_frameBufferTextures["renderTexture"][std::make_pair(threadId, viewportIndex)]<< std::endl;
    
    std::cout << (int)m_frameBufferTextures["renderTexture"][std::make_pair(threadId, viewportIndex)] << std::endl;
    _mutex.unlock();
    
    
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(m_frameBufferTextures["depthTexture"][std::make_pair(threadId, viewportIndex)])
    
    
    quad.bindToContext(threadId);
    glDrawElements(GL_TRIANGLES, quad.getCountForIndex("quad"), GL_UNSIGNED_INT, (void*)quad.getStartForIndex("quad"));

    
   
}
