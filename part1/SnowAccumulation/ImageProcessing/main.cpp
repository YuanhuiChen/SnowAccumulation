//The start code is CIS565 Image Processing Project

#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "Utility.h"
#include "SOIL.h"
#include "objloader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <glm/gtc/random.hpp>

using namespace glm;
bool displayDepth = false;
bool toggleFog = true;

GLuint meshFogLocation;
GLuint passFogLocation;
GLuint snowFogLocation;

int width = 1280;
int height = 680;

// vbos
GLuint meshVertexBufferObjID[4];
GLuint squareVertexBufferObjID[3];

//attributes
GLuint meshPositionLocation = 0;
GLuint meshUVLocation = 2;
GLuint meshNormalLocation = 1;


//mesh
GLuint modelMatrixLocationMesh;
GLuint persMatrixLocationMesh;
GLuint viewMatrixLocationMesh;
GLuint depthMapLocationMesh;
GLuint textureMapLocationMesh;
GLuint toMapMatrixLocationMesh;
GLuint lightPosLocationMesh;
GLuint timeLocationMesh;
GLuint growMapLocationMesh; 

//depth map
GLuint projMatrixLocationDepth;
GLuint modelMatrixLocationDepth;
//background render
GLuint imageLocationSquare;
//init shaders
const char *meshAttributeLocations[] = {"vs_position", "vs_normal", "vs_uv"};
const char *depthAttributeLocations[] = {"vs_position"};
const char *squareAttributeLocations[] = {"Position", "TexCoords"};
const char *snowAttributionLocations[] = { "vs_position", "pointSize", "fallSpeed"};

//render programs
GLuint passThroughProgram;
GLuint renderMeshProgram;
GLuint depthMapProgram;

//images
GLuint depthMap;
GLuint textureMap;
GLuint depthBuffer;
GLuint backImage;
//frame buffer
GLuint depthRenderFrame;
mat4 worldToMapMatrix;
int meshElementSize;


//depth map 
float size = 10.0;	
glm::vec3 lightPosition = vec3(0,10,0);
glm::vec3 center = vec3(0,0,0);
glm::vec3 up = vec3(0,0,1);

//render mesh
glm::vec3 eyePosition = glm::vec3(-10, 10,10);
//Animation/transformation stuff
clock_t old;
float rotation = 0.0f;
float globalTime = 0.0;




//snow fall
GLuint snowFallProgram;
int particlesNumber = 2000;
GLuint snowVertexBufferObjID[3];
GLuint snowPositionLocation = 0;
GLuint pointSizeLocation = 1;
GLuint fallSpeedLocation = 2;
GLuint snowTimeLocation;
GLuint snowImage;
GLuint snowProjMatrixLocation;
GLuint snowViewMatrixLocation;



//standard glut-based program functions
void init(void);
void resize(int, int);
void display(void);
void keypress(unsigned char, int, int);
void cleanup(void);

void createRedSquare(mat4);
void createBlueSquare(mat4);
void initMesh();
GLuint initMeshShader(const char *vertexShaderPath, const char *fragmentShaderPath);
GLuint initDepthShader(const char *vertexShaderPath, const char *fragmentShaderPath);
GLuint initPassShader(const char *vertexShaderPath, const char *fragmentShaderPath);
GLuint initSnowShader(const char *vertexShaderPath, const char *fragmentShaderPath);
void initDepthFrameBuffer();
void drawSquare();
void initBackgroundTexture();
void drawBackground();
void initSquare();
void drawMesh(mat4 modelView);
void drawDepthMap(mat4 modelView);
void initPoints();
void drawPoints();

//draw snowflakes
void drawPoints()
{
	glUseProgram(snowFallProgram);
	glEnableVertexAttribArray(fallSpeedLocation);
	glEnableVertexAttribArray(pointSizeLocation);
	glEnableVertexAttribArray(snowPositionLocation);

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, snowImage);

	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, 1);

	glBindBuffer(GL_ARRAY_BUFFER, snowVertexBufferObjID[0]);
	glVertexAttribPointer((GLuint)snowPositionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, snowVertexBufferObjID[1]);
	glVertexAttribPointer((GLuint)pointSizeLocation, 1, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, snowVertexBufferObjID[2]);
	glVertexAttribPointer((GLuint)fallSpeedLocation, 1, GL_FLOAT, GL_FALSE, 0, 0); 

	glUniform1f(snowTimeLocation, globalTime);
	glUniform1i(snowFogLocation, toggleFog);
	glm::mat4 pers = glm::perspective(70.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(eyePosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glUniformMatrix4fv(snowProjMatrixLocation, 1, GL_FALSE, &pers[0][0]);
	glUniformMatrix4fv(snowViewMatrixLocation, 1, GL_FALSE, &view[0][0]);

	glDrawArrays(GL_POINTS, 0,  particlesNumber);


	glDisable(GL_BLEND);
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);

	glDisableVertexAttribArray(fallSpeedLocation);
	glDisableVertexAttribArray(pointSizeLocation);
	glDisableVertexAttribArray(snowPositionLocation);

}


//initialize snowflakes positions, sizes, and speeds
void initPoints()
{
	glm::vec3 * vertices = new glm::vec3[particlesNumber];

	for(int i = 0; i < particlesNumber ; i++)
	{
		vertices[i].x = glm::linearRand(-20.0f, 20.0f);
		vertices[i].y = glm::linearRand(-20.0f, 20.0f);
		vertices[i].z = glm::linearRand(-20.0f, 20.0f);;
	}

   
	float * sizes = new float [particlesNumber];

	for(int i = 0; i < particlesNumber; i++)
	{
		sizes[i] = glm::linearRand(2.0f, 20.0f);
	}

	float * speeds  = new float[particlesNumber];

	for(int i = 0; i < particlesNumber; i++)
	{
		speeds[i] = glm::linearRand(2.0f, 5.0f);
	}
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	glGenBuffers(3, snowVertexBufferObjID);

	glBindBuffer(GL_ARRAY_BUFFER, snowVertexBufferObjID[0]);
	glBufferData(GL_ARRAY_BUFFER, particlesNumber * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);



	glBindBuffer(GL_ARRAY_BUFFER, snowVertexBufferObjID[1]);
	glBufferData(GL_ARRAY_BUFFER, particlesNumber * sizeof(float), sizes, GL_STATIC_DRAW);


    
	glBindBuffer(GL_ARRAY_BUFFER, snowVertexBufferObjID[2]);
	glBufferData(GL_ARRAY_BUFFER, particlesNumber * sizeof(float), speeds, GL_STATIC_DRAW);



	delete [] vertices;
	delete [] sizes;
	delete [] speeds;
}

//snowfall
GLuint initSnowShader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
	GLuint program = Utility::createProgram(vertexShaderPath, fragmentShaderPath, snowAttributionLocations, 3);
	GLint location;

	glUseProgram(program);

	if ((location = glGetUniformLocation(program, "u_image")) != -1)
	{
		glUniform1i(location, 0);
	}
	
	snowProjMatrixLocation = glGetUniformLocation(program, "u_projMatrix");
	snowViewMatrixLocation = glGetUniformLocation(program, "u_modelViewMatrix");
	snowFogLocation  = glGetUniformLocation(program, "u_fog");
	snowTimeLocation = glGetUniformLocation(program, "globalTime");
	return program;
}



int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("Starter Code");

	glewInit();
		renderMeshProgram = initMeshShader("renderMeshVS.glsl", "renderMeshFS.glsl");
		depthMapProgram = initDepthShader("depthVS.glsl", "depthFS.glsl");
	passThroughProgram = initPassShader("passthroughVS.glsl", "passthroughFS.glsl");
	snowFallProgram = initSnowShader("snowfallVS.glsl","snowfallFS.glsl");

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keypress);
	glutIdleFunc(display);

	glutMainLoop();
	return 0;
}

void initBackgroundTexture()
{
	backImage = SOIL_load_OGL_texture("fog-night.jpg",4 , 0, 0);
	glBindTexture(GL_TEXTURE_2D, backImage);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	textureMap = SOIL_load_OGL_texture("outUV.dds",4 , 0, 0);
	glBindTexture(GL_TEXTURE_2D, textureMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	snowImage = SOIL_load_OGL_texture("snowflake.png", 4, 0, 0);
	glBindTexture(GL_TEXTURE_2D, snowImage);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void display() {

	globalTime += 0.2;
	if(globalTime > 10000.0)
		globalTime = 0.0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	clock_t newTime = clock();
	rotation += 150 * (static_cast<float>(newTime - old) / static_cast<float>(CLOCKS_PER_SEC));
	glm::mat4 modelView = glm::mat4(1.0);
	glUseProgram(renderMeshProgram);

	//drawBackground();
	//glUseProgram(renderMeshProgram);
	drawDepthMap(modelView);
	if(!displayDepth)
	{
	drawBackground();
	drawMesh(modelView);
	//
	//drawSquare();
	drawPoints();
	}
	glutSwapBuffers();
	old = newTime;
}

void init() {

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);

	glUseProgram(renderMeshProgram);
	initDepthFrameBuffer();
	initMesh();
	initSquare();
	initPoints();

	initBackgroundTexture();
	resize(width, height);
	old = clock();

}


GLuint initMeshShader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
	GLuint program = Utility::createProgram(vertexShaderPath, fragmentShaderPath, meshAttributeLocations, 3);
	modelMatrixLocationMesh = glGetUniformLocation(program, "u_modelMatrix");
	persMatrixLocationMesh = glGetUniformLocation(program, "u_persMatrix");
	viewMatrixLocationMesh = glGetUniformLocation(program, "u_viewMatrix");
	depthMapLocationMesh = glGetUniformLocation(program, "u_depthMap");
	toMapMatrixLocationMesh = glGetUniformLocation(program, "u_toMapMatrix");
	textureMapLocationMesh =  glGetUniformLocation(program, "u_textureMap");
	lightPosLocationMesh = glGetUniformLocation(program, "u_lightPos");
	timeLocationMesh = glGetUniformLocation(program, "u_time");
	growMapLocationMesh = glGetUniformLocation(program, "u_growMap");
	meshFogLocation = glGetUniformLocation(program, "u_fog");
	return program;
}

GLuint initDepthShader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
	GLuint program = Utility::createProgram(vertexShaderPath, fragmentShaderPath, depthAttributeLocations, 1);

	projMatrixLocationDepth = glGetUniformLocation(program, "u_projMatrix");	
	modelMatrixLocationDepth = glGetUniformLocation(program, "u_modelMatrix");
   
	return program;
}


void initDepthFrameBuffer()
{
	//create texture buffer for frame
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, 1024, 768, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenFramebuffers(1, &depthRenderFrame);
	glBindFramebuffer(GL_FRAMEBUFFER,depthRenderFrame);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
	glDrawBuffer(GL_NONE);


	/*glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, 1024, 768, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

	//create a renderbuffer object to store depth info
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//create a framebuffer object
	glGenFramebuffers(1, &depthRenderFrame);
	glBindFramebuffer(GL_FRAMEBUFFER,depthRenderFrame);

	//attach texture object to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,depthMap, 0);

	
	//attach depth render buffer to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthBuffer);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers*/

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		bool test = false;
		return;
	}
}
void drawDepthMap(mat4 modelView)
{

	//draw depth map to framebuffer
	if(displayDepth)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	else
		glBindFramebuffer(GL_FRAMEBUFFER, depthRenderFrame);
	
	glUseProgram(depthMapProgram);
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glEnableVertexAttribArray(meshPositionLocation);
	glBindBuffer(GL_ARRAY_BUFFER,  meshVertexBufferObjID[0]);
	glVertexAttribPointer(
		meshPositionLocation,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

		glUniformMatrix4fv(modelMatrixLocationDepth, 1, GL_FALSE, &modelView[0][0]);   
		glm::mat4 orth = glm::ortho(-size, size, -size, size, -size, 2*size);
		glm::mat4 view = glm::lookAt(lightPosition, center,up);


		worldToMapMatrix = glm::mat4(0.5, 0.0, 0.0, 0.0, 
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0) * orth * view;
		glm::mat4 projectMatrix = orth * view * modelView;

		glUniformMatrix4fv(projMatrixLocationDepth, 1, GL_FALSE, &projectMatrix[0][0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  meshVertexBufferObjID[3]);
	glDrawElements(
		GL_TRIANGLES,      
		meshElementSize,    
		GL_UNSIGNED_SHORT, 
		(void*)0          
		);

	glDisableVertexAttribArray(meshPositionLocation);


}


void initMesh()
{
	//load mesh vertices
	glGenBuffers(1, & meshVertexBufferObjID[0]);
	glGenBuffers(1, & meshVertexBufferObjID[1]);
	glGenBuffers(1, & meshVertexBufferObjID[2]);
	glGenBuffers(1, & meshVertexBufferObjID[3]);

	obj * mesh =  new obj();

	objLoader* loader = new objLoader("scenes.obj", mesh);

	mesh->buildVBOs();
	delete loader;

	float * vertices = mesh->getVBO();
	int vboSize = mesh->getVBOsize();

	float * normals = mesh->getNBO();
	int nboSize = mesh->getNBOsize();

	/*float * colors = mesh->getCBO();
	int cboSize = mesh->getCBOsize();*/

	std::vector<glm::vec4> * textureUV = mesh->getTextureCoords();

	int iboSize = mesh->getIBOsize();
	unsigned short * indices = new unsigned short[iboSize];
	int * iboPtr = mesh->getIBO();

	for(int i = 0; i < iboSize; i++)
	{
		indices[i] = iboPtr[i];
	}


	meshElementSize = iboSize;


	glBindBuffer(GL_ARRAY_BUFFER, meshVertexBufferObjID[0]);
	glBufferData(GL_ARRAY_BUFFER, vboSize * sizeof(float), vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER,  meshVertexBufferObjID[1]);
	glBufferData(GL_ARRAY_BUFFER, nboSize * sizeof(float), normals, GL_STATIC_DRAW);



	glBindBuffer(GL_ARRAY_BUFFER,  meshVertexBufferObjID[2]);
	glBufferData(GL_ARRAY_BUFFER,textureUV->size() * sizeof(glm::vec4), textureUV, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  meshVertexBufferObjID[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,iboSize * sizeof(GLushort), indices, GL_STATIC_DRAW);
}


void cleanup() {
	glDeleteBuffers(1, &meshVertexBufferObjID[0]);
	glDeleteBuffers(1, & meshVertexBufferObjID[2]);
	glDeleteBuffers(1, & meshVertexBufferObjID[1]);
	glDeleteBuffers(1, & meshVertexBufferObjID[3]);


	glDeleteProgram(renderMeshProgram);
}

void keypress(unsigned char key, int x, int y) {
	switch(key) {
	case 'q':
		cleanup();
		exit(0);
		break;
	case 'd':
		displayDepth = !displayDepth;
		break;
	case 'f':
		toggleFog = !toggleFog;
		break;
	}

	glutPostRedisplay();
}

void drawMesh(mat4 modelView)
{

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glClear(GL_DEPTH_BUFFER_BIT);	
	glUseProgram(renderMeshProgram);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glUniform1i(depthMapLocationMesh, 0);

	glBindTexture(GL_TEXTURE_2D, depthMap);
	glUniform1i(growMapLocationMesh, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureMap);
	glUniform1i(textureMapLocationMesh, 1);

	glEnableVertexAttribArray(meshPositionLocation);
	glBindBuffer(GL_ARRAY_BUFFER,  meshVertexBufferObjID[0]);
	glVertexAttribPointer(
		meshPositionLocation,                
		3,                  
		GL_FLOAT,           
		GL_FALSE,           
		0,                  
		(void*)0            
		);


	glEnableVertexAttribArray(meshNormalLocation);
	glBindBuffer(GL_ARRAY_BUFFER,  meshVertexBufferObjID[1]);
	glVertexAttribPointer(
		meshNormalLocation,                             
		3,                                
		GL_FLOAT,                        
		GL_FALSE,                       
		0,                                
		(void*)0                          
		);

	glEnableVertexAttribArray(meshUVLocation);
	glBindBuffer(GL_ARRAY_BUFFER, meshVertexBufferObjID[2]);
	glVertexAttribPointer(
		meshUVLocation,                              
		4,                                
		GL_FLOAT,                         
		GL_FALSE,                        
		0,                               
		(void*)0                        
		);
	glUniform1i(meshFogLocation, toggleFog);
	glUniform1f(timeLocationMesh, globalTime);
	glUniformMatrix4fv(toMapMatrixLocationMesh, 1, GL_FALSE, &worldToMapMatrix[0][0]);
	glUniform4f(lightPosLocationMesh, lightPosition.x,lightPosition.y,lightPosition.z, 0.0f);   
		glUniformMatrix4fv(modelMatrixLocationMesh, 1, GL_FALSE, &modelView[0][0]);
		
		glm::mat4 pers = glm::perspective(70.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(eyePosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	//set the projection matrix here, it only needs to be changed if the screen is resized otherwise it can stay the same
	glUniformMatrix4fv(persMatrixLocationMesh, 1, GL_FALSE, &pers[0][0]);
	glUniformMatrix4fv(viewMatrixLocationMesh, 1, GL_FALSE, &view[0][0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  meshVertexBufferObjID[3]);
	glDrawElements(
		GL_TRIANGLES,      
		meshElementSize,    
		GL_UNSIGNED_SHORT, 
		(void*)0          
		);


	glDisableVertexAttribArray(meshPositionLocation);
	glDisableVertexAttribArray(meshUVLocation);
	glDisableVertexAttribArray(meshNormalLocation);
}




void resize(int width, int height) {

	glViewport(0, 0, width, height);
	
	glm::mat4 pers = glm::perspective(70.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(eyePosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(persMatrixLocationMesh, 1, GL_FALSE, &pers[0][0]);
	glUniformMatrix4fv(viewMatrixLocationMesh, 1, GL_FALSE, &view[0][0]);

	glutPostRedisplay();
}


GLuint initPassShader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
	GLuint program = Utility::createProgram(vertexShaderPath, fragmentShaderPath, squareAttributeLocations, 2);

	glUseProgram(program);

	imageLocationSquare = glGetUniformLocation(program, "u_image");
	passFogLocation= glGetUniformLocation(program, "u_fog");

	return program;
}

void initSquare()
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


	glGenBuffers(3, squareVertexBufferObjID);

	glBindBuffer(GL_ARRAY_BUFFER, squareVertexBufferObjID[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, squareVertexBufferObjID[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareVertexBufferObjID[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void drawBackground()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,width,height);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glUseProgram(passThroughProgram);
	//glBindTexture(GL_TEXTURE_2D, image);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, backImage);
	glUniform1i(imageLocationSquare, 0);
    //
	//glBindTexture(GL_TEXTURE_2D, frontDepthTexture);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, squareVertexBufferObjID[0]);
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glBindBuffer(GL_ARRAY_BUFFER, squareVertexBufferObjID[1]);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glUniform1i(passFogLocation, toggleFog);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareVertexBufferObjID[2]);
	// VAO, shader program, and texture already bound
	glDrawElements(GL_TRIANGLES, 6,  GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


void drawSquare()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,width,height);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(passThroughProgram);
	//glBindTexture(GL_TEXTURE_2D, image);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glUniform1i(imageLocationSquare, 0);
    //
	//glBindTexture(GL_TEXTURE_2D, frontDepthTexture);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, squareVertexBufferObjID[0]);
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glBindBuffer(GL_ARRAY_BUFFER, squareVertexBufferObjID[1]);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareVertexBufferObjID[2]);
	// VAO, shader program, and texture already bound
	glDrawElements(GL_TRIANGLES, 6,  GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}