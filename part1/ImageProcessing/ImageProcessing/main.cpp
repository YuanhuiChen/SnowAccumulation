#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "Utility.h"
#include "SOIL.h"
#include "objloader.h"
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <iostream>
#include <sstream>
int width = 640;
int height = 480;

GLuint positionLocation = 0;
GLuint texcoordsLocation = 1;

GLuint meshPositionLocation = 0;
GLuint meshNormalLocation = 1;
GLuint meshColorLocation = 2;


GLuint vertexBufferObjID[3];
GLuint meshVertexBufferObjID[4];

const char *attributeLocations[] = { "Position", "Texcoords" };
const char *meshAttributeLocations[] = {"vs_position", "vs_normal", "vs_color"};

GLuint passthroughProgram;
GLuint frontViewProgram;
GLuint topViewProgram;
GLuint renderMeshProgram;

//uniform location
GLuint modelMatrixLocationDepth;
GLuint modelMatrixLocationMesh;
GLuint frontProjectMatrixLocation;
GLuint topProjectMatrixLocation;
GLuint persProjectMatrixLocation;
GLuint imageLocation;
GLuint lightLocation;
GLuint frontDepthLocation;
GLuint topDepthLocation;

//front View Frame
GLuint frontFrame;
GLuint frontTexture;
GLuint topTexture;
GLuint frontDepth;

//
GLuint image;
int meshElementSize;
void initSquareVAO(void)
{
	GLfloat vertices[] =
	{ 
        -1.0f, -1.0f, 
         1.0f, -1.0f, 
         1.0f,  1.0f, 
        -1.0f,  1.0f, 
    };

	GLfloat texcoords[] = 
    { 
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

	GLushort indices[] = { 0, 1, 3, 3, 1, 2 };

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	
	glGenBuffers(3, vertexBufferObjID);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);
	
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferObjID[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void initMeshVAO()
{
	obj * mesh =  new obj();

	objLoader* loader = new objLoader("suzanne.obj", mesh);
	//objLoader* loader = new objLoader("cow.obj", mesh);
	mesh->buildVBOs();
	delete loader;

	float * vertices = mesh->getVBO();
	int vboSize = mesh->getVBOsize();

	float * normals = mesh->getNBO();
	int nboSize = mesh->getNBOsize();

	float * colors = mesh->getCBO();
	int cboSize = mesh->getCBOsize();

	unsigned short * indices = mesh->getIBO();
	int iboSize = mesh->getIBOsize();

	meshElementSize = iboSize;

	glGenBuffers(4, meshVertexBufferObjID);

	glBindBuffer(GL_ARRAY_BUFFER, meshVertexBufferObjID[0]);
	glBufferData(GL_ARRAY_BUFFER, vboSize * sizeof(float), vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, meshVertexBufferObjID[1]);
	glBufferData(GL_ARRAY_BUFFER, nboSize * sizeof(float), normals, GL_STATIC_DRAW);



	glBindBuffer(GL_ARRAY_BUFFER, meshVertexBufferObjID[2]);
	glBufferData(GL_ARRAY_BUFFER,cboSize * sizeof(float), colors, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVertexBufferObjID[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,iboSize * sizeof(unsigned short), indices, GL_STATIC_DRAW);


}

void initVAO()
{
	initMeshVAO();
	initSquareVAO();
	

}

GLuint initDepthShader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
	GLuint program = Utility::createProgram(vertexShaderPath, fragmentShaderPath, meshAttributeLocations, 3);

	modelMatrixLocationDepth = glGetUniformLocation(program, "u_ModelMatrix");
	frontProjectMatrixLocation = glGetUniformLocation(program, "u_frontProjMatrix");
	topProjectMatrixLocation = glGetUniformLocation(program, "u_topProjMatrix");

	return program;
}
GLuint initMeshShader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
	GLuint program = Utility::createProgram(vertexShaderPath, fragmentShaderPath, meshAttributeLocations, 3);

	modelMatrixLocationMesh = glGetUniformLocation(program, "u_ModelMatrix");
	persProjectMatrixLocation = glGetUniformLocation(program, "u_ProjMatrix");
	
	lightLocation = glGetUniformLocation(program, "u_LightPosition");
	frontDepthLocation = glGetUniformLocation(program, "u_frontDepth");
	topDepthLocation = glGetUniformLocation(program, "u_topDepth");

	return program;
}

GLuint initShader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
	GLuint program = Utility::createProgram(vertexShaderPath, fragmentShaderPath, attributeLocations, 2);

	glUseProgram(program);
	
	imageLocation = glGetUniformLocation(program, "u_image");

	return program;
}


void initTextures()
{
	image = SOIL_load_OGL_texture("Valve.png", 3, 0, 0);
	glBindTexture(GL_TEXTURE_2D, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void drawSquare()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,width,height);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(passthroughProgram);
	//glBindTexture(GL_TEXTURE_2D, image);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frontTexture);
	//glBindTexture(GL_TEXTURE_2D,topTexture);
	glUniform1i(imageLocation, 0);
    //
	//glBindTexture(GL_TEXTURE_2D, frontTexture);

	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(texcoordsLocation);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[0]);
	glVertexAttribPointer((GLuint)positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[1]);
	glVertexAttribPointer((GLuint)texcoordsLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferObjID[2]);
	// VAO, shader program, and texture already bound
	glDrawElements(GL_TRIANGLES, 6,  GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(positionLocation);
}



void initFrontViewFrame()
{

	glGenTextures(1, &frontTexture);
	glBindTexture(GL_TEXTURE_2D, frontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 1024, 768, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

	glGenTextures(1, &topTexture);
	glBindTexture(GL_TEXTURE_2D, topTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 1024, 768, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//create a renderbuffer object to store depth info
	glGenRenderbuffers(1, &frontDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, frontDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//create a framebuffer object
	glGenFramebuffers(1, &frontFrame);
	glBindFramebuffer(GL_FRAMEBUFFER, frontFrame);

	//attach texture object to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,frontTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,topTexture, 0);
	
	//attach depth render buffer to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,frontDepth);

	// Set the list of draw buffers.
	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		bool test = false;
		return;
	}
}

void drewMesh()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderMeshProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frontTexture);
	glUniform1i(frontDepthLocation, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, topTexture);
	glUniform1i(topDepthLocation, 1);


	glEnableVertexAttribArray(meshPositionLocation);
	glBindBuffer(GL_ARRAY_BUFFER, meshVertexBufferObjID[0]);
	glVertexAttribPointer(
		meshPositionLocation,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glEnableVertexAttribArray(meshNormalLocation);
	glBindBuffer(GL_ARRAY_BUFFER, meshVertexBufferObjID[1]);
	glVertexAttribPointer(
		meshNormalLocation,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glEnableVertexAttribArray(meshColorLocation);
	glBindBuffer(GL_ARRAY_BUFFER, meshVertexBufferObjID[2]);
	glVertexAttribPointer(
		meshColorLocation,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVertexBufferObjID[3]);	
	glm::mat4 projectMatrix =  glm::perspective(60.0f, static_cast<float>(width)/static_cast<float>(height), 0.1f, 10.0f);
	glm::mat4 camera = glm::lookAt(glm::vec3(0,0,5), glm::vec3(0,0,0), glm::vec3(0,1,0));
	glm::mat4 projection = projectMatrix * camera;

	glm::mat4 modelMatrix = glm::mat4(1.0);
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0, 2.0, 2.0));

	glUniformMatrix4fv(modelMatrixLocationMesh, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(persProjectMatrixLocation, 1, GL_FALSE, &projection[0][0]);

	glUniform3f(lightLocation, 0.0f, 1.0f, 10.0f);

	glDrawElements(
		GL_TRIANGLES,      
		meshElementSize,    
		GL_UNSIGNED_SHORT, 
		(void*)0          
		);
	
	glDisableVertexAttribArray(meshPositionLocation);
	glDisableVertexAttribArray(meshNormalLocation);
	glDisableVertexAttribArray(meshColorLocation);
}

void drawFrontView()
{

	glBindFramebuffer(GL_FRAMEBUFFER, frontFrame);
	glViewport(0, 0, width, height);
	//glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(frontViewProgram);


	glEnableVertexAttribArray(meshPositionLocation);
	glBindBuffer(GL_ARRAY_BUFFER, meshVertexBufferObjID[0]);
	glVertexAttribPointer(
		meshPositionLocation,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glEnableVertexAttribArray(meshNormalLocation);
	glBindBuffer(GL_ARRAY_BUFFER, meshVertexBufferObjID[1]);
	glVertexAttribPointer(
		meshNormalLocation,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glEnableVertexAttribArray(meshColorLocation);
	glBindBuffer(GL_ARRAY_BUFFER, meshVertexBufferObjID[2]);
	glVertexAttribPointer(
		meshColorLocation,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVertexBufferObjID[3]);
	
	//glm::mat4 projectMatrix =  glm::perspective(60.0f, static_cast<float>(width)/static_cast<float>(height), 0.1f, 10.0f);
	glm::mat4 projectMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 2.0f,12.0f);
	//glm::mat4 projectMatrix =  glm::perspective(60.0f, static_cast<float>(width)/static_cast<float>(height), 0.1f, 10.0f);
	glm::mat4 camera = glm::lookAt(glm::vec3(0,0,5), glm::vec3(0,0,0), glm::vec3(0,1,0));

	glm::mat4 projection = projectMatrix * camera;
	glm::mat4 modelMatrix = glm::mat4(1.0);

	//modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0, 2.0, 2.0));

	glUniformMatrix4fv(modelMatrixLocationDepth, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(frontProjectMatrixLocation, 1, GL_FALSE, &projection[0][0]);

	camera = glm::lookAt(glm::vec3(0,5,0), glm::vec3(0,0,0), glm::vec3(0,0,-1));
	projection = projectMatrix * camera;
	glUniformMatrix4fv(topProjectMatrixLocation, 1, GL_FALSE, &projection[0][0]);


	glDrawElements(
		GL_TRIANGLES,      
		meshElementSize,    
		GL_UNSIGNED_SHORT, 
		(void*)0          
		);
	


	glDisableVertexAttribArray(meshPositionLocation);
	glDisableVertexAttribArray(meshNormalLocation);
	glDisableVertexAttribArray(meshColorLocation);

}
void reshape(int w, int h)
{

}

void display(void)
{

	drawFrontView();
	//drawSquare();
	drewMesh();
	glutPostRedisplay();
	glutSwapBuffers();

}



void keyboard(unsigned char key, int x, int y)
{
    
}


void init()
{
	initVAO();
	initFrontViewFrame();
	initTextures();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("Image Processing");
	glewInit();
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "glewInit failed, aborting." << std::endl;
		exit (1);
	}
	std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "OpenGL version " << glGetString(GL_VERSION) << " supported" << std::endl;

	init();
	passthroughProgram = initShader("passthroughVS.glsl", "passthroughFS.glsl");
	frontViewProgram = initDepthShader("depthVS.glsl","depthFS.glsl");
	renderMeshProgram = initMeshShader("renderMeshVS.glsl", "renderMeshFS.glsl");


	glutDisplayFunc(display);
	glutReshapeFunc(reshape);	
    glutKeyboardFunc(keyboard);

    

	glutMainLoop();
	return 0;
}
