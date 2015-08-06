/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef EXAMPLEVRAPP_H_
#define EXAMPLEVRAPP_H_
#define GLM_FORCE_RADIANS

#define _USE_MATH_DEFINES 

#include "GL/glew.h"
#include <cmath>
#include "MVRCore/AbstractMVRApp.H"
#include "MVRCore/AbstractCamera.H"
#include "MVRCore/AbstractWindow.H"
#include "MVRCore/CameraOffAxis.H"

//#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include "MVRCore/Event.H"
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include "MVRCore/Thread.h"
#include "GeometryArray.h"
#include <utility>      // std::pair

class ShaderProgram;
class Shader;
class Uniform;
class Texture;
class Image;
class ExampleVrApp : public MinVR::AbstractMVRApp {
public:
	ExampleVrApp();
	virtual ~ExampleVrApp();

	void doUserInputAndPreDrawComputation(const std::vector<MinVR::EventRef> &events, double synchronizedTime);
	void initializeContextSpecificVars(int threadId, MinVR::WindowRef window);
	void postInitialization();
	void drawGraphics(int threadId, MinVR::WindowRef window, int viewportIndex);
    
private:
    void initData();

	void initGL();
	void initVBO(int threadId);
    void initShader(int threadId);

    std::map<std::string, std::map<int, GLuint> > _objectIds;


    MinVR::Mutex _mutex;
    std::map<std::string, Shader*> m_shaders;
    std::map<std::string, ShaderProgram*> m_shaderPrograms;
    std::map<std::string, Uniform*> m_uniforms;
    std::map<std::string, Texture*> m_textures;

    std::map<std::string, std::map<std::pair<GLuint, GLuint>, GLuint>> m_frameBufferTextures;

    std::map<std::string, std::map<std::pair<GLuint, GLuint>, GLuint>> m_frameBuffers;
    std::map<std::string, Image*> m_images;

    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;

    GeometryArray cube;
    GeometryArray quad;

    glm::mat4 _model;

};

#endif /* EXAMPLEVRAPP_H_ */
