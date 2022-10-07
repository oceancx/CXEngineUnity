#include "RenderAPI.h"
#include "PlatformBase.h"

#include <iostream>
#include <graphics/texture.h>
#include "sprite_renderer.h"

// OpenGL Core profile (desktop) or OpenGL ES (mobile) implementation of RenderAPI.
// Supports several flavors: Core, ES2, ES3


#if SUPPORT_OPENGL_UNIFIED


#include <assert.h>
#if UNITY_IOS || UNITY_TVOS
#	include <OpenGLES/ES2/gl.h>
#elif UNITY_ANDROID || UNITY_WEBGL
#	include <GLES2/gl2.h>
#elif UNITY_OSX
#	include <OpenGL/gl3.h>
#elif UNITY_WIN
// On Windows, use gl3w to initialize and load OpenGL Core functions. In principle any other
// library (like GLEW, GLFW etc.) can be used; here we use gl3w since it's simple and
// straightforward.
#	include "gl3w/gl3w.h"
#include <animation/sprite.h>
#include <resource_manager.h>
#include <file_system.h>
#include <cxlua.h>
//#elif UNITY_LINUX
//#	define GL_GLEXT_PROTOTYPES
//#	include <GL/gl.h>
//#elif UNITY_EMBEDDED_LINUX
//#	include <GLES2/gl2.h>
//#if SUPPORT_OPENGL_CORE
//#	define GL_GLEXT_PROTOTYPES
//#	include <GL/gl.h>
//#endif
//#else
//#	error Unknown platform
#endif


class RenderAPI_OpenGLCoreES : public RenderAPI
{
public:
	RenderAPI_OpenGLCoreES(UnityGfxRenderer apiType);
	virtual ~RenderAPI_OpenGLCoreES() { }

	virtual void ProcessDeviceEvent(UnityGfxDeviceEventType type, IUnityInterfaces* interfaces);

	virtual bool GetUsesReverseZ() { return false; }

	virtual void DrawSimpleTriangles(const float worldMatrix[16], int triangleCount, const void* verticesFloat3Byte4);

	virtual void* BeginModifyTexture(void* textureHandle, int textureWidth, int textureHeight, int* outRowPitch);
	virtual void EndModifyTexture(void* textureHandle, int textureWidth, int textureHeight, int rowPitch, void* dataPtr);

	virtual void* BeginModifyVertexBuffer(void* bufferHandle, size_t* outBufferSize);
	virtual void EndModifyVertexBuffer(void* bufferHandle);

private:
	void CreateResources();

private:
	UnityGfxRenderer m_APIType;
	GLuint m_VertexShader;
	GLuint m_FragmentShader;
	GLuint m_Program;
	GLuint m_VertexArray;
	GLuint m_VertexBuffer;
	int m_UniformWorldMatrix;
	int m_UniformProjMatrix;
	int m_UniformImage;
};


RenderAPI* CreateRenderAPI_OpenGLCoreES(UnityGfxRenderer apiType)
{
	return new RenderAPI_OpenGLCoreES(apiType);
}


enum VertexInputs
{
	kVertexInputPosition = 0,
	kVertexInputTexCoord ,
	kVertexInputColor 
};


// Simple vertex shader source
#define VERTEX_SHADER_SRC(ver, attr, varying)						\
	ver																\
	attr " vec3 pos;\n"										\
	attr " vec2 texcoord;\n"									\
	attr " vec4 color;\n"										\
	"\n"															\
	varying " vec4 ocolor;\n"									\
	varying " vec2 otexcoord;\n"								\
	"\n"															\
	"uniform mat4 worldMatrix;\n"								\
	"uniform mat4 projMatrix;\n"								\
	"\n"															\
	"void main()\n"													\
	"{\n"															\
	"	gl_Position = (projMatrix * worldMatrix) * vec4(pos,1);\n"	\
	"	ocolor = color;\n"											\
	"	otexcoord = texcoord;\n"											\
	"}\n"															\

static const char* kGlesVProgTextGLES2 = VERTEX_SHADER_SRC("\n", "attribute", "varying");
static const char* kGlesVProgTextGLES3 = VERTEX_SHADER_SRC("#version 300 es\n", "in", "out");
#if SUPPORT_OPENGL_CORE
static const char* kGlesVProgTextGLCore = VERTEX_SHADER_SRC("#version 100\nprecision lowp float;\n", "attribute", "varying");
#endif

#undef VERTEX_SHADER_SRC


// Simple fragment shader source
#define FRAGMENT_SHADER_SRC(ver, varying, outDecl, outVar)	\
	ver												\
	varying " vec4 ocolor;\n"					\
	varying " vec2 otexcoord;\n"					\
	"uniform sampler2D texture0;\n"					\
	"\n"											\
	"void main()\n"									\
	"{\n"											\
	"	 vec4 tmp = texture2D(texture0, otexcoord);\n"						\
	"	 gl_FragColor = tmp* ocolor;\n"						\
	"}\n"											\

static const char* kGlesFShaderTextGLES2 = FRAGMENT_SHADER_SRC("\n", "varying", "\n", "gl_FragColor");
static const char* kGlesFShaderTextGLES3 = FRAGMENT_SHADER_SRC("#version 300 es\n", "in", "out lowp vec4 fragColor;\n", "fragColor");
#if SUPPORT_OPENGL_CORE
static const char* kGlesFShaderTextGLCore = FRAGMENT_SHADER_SRC("#version 100\nprecision lowp float;\n", "varying", "out lowp vec4 fragColor;\n", "fragColor");
#endif

#undef FRAGMENT_SHADER_SRC


static GLuint CreateShader(GLenum type, const char* sourceText)
{
	GLuint ret = glCreateShader(type);
	glShaderSource(ret, 1, &sourceText, NULL);
	glCompileShader(ret);
	return ret;
}
Animation* anim;
void RenderAPI_OpenGLCoreES::CreateResources()
{
	// Make sure that there are no GL error flags set before creating resources
	//int err = glGetError();

	//while (glGetError() != GL_NO_ERROR) {}

	// Create shaders
//	if (m_APIType == kUnityGfxRendererOpenGLES20)
//	{
//		m_VertexShader = CreateShader(GL_VERTEX_SHADER, kGlesVProgTextGLES2);
//		m_FragmentShader = CreateShader(GL_FRAGMENT_SHADER, kGlesFShaderTextGLES2);
//	}
//	else if (m_APIType == kUnityGfxRendererOpenGLES30)
//	{
//		m_VertexShader = CreateShader(GL_VERTEX_SHADER, kGlesVProgTextGLES3);
//		m_FragmentShader = CreateShader(GL_FRAGMENT_SHADER, kGlesFShaderTextGLES3);
//	}
//#	if SUPPORT_OPENGL_CORE
//	else if (m_APIType == kUnityGfxRendererOpenGLCore)
//	{
//#		if UNITY_WIN
//		gl3wInit();
//#		endif
//
//		m_VertexShader = CreateShader(GL_VERTEX_SHADER, kGlesVProgTextGLCore);
//		m_FragmentShader = CreateShader(GL_FRAGMENT_SHADER, kGlesFShaderTextGLCore);
//	}
//#	endif // if SUPPORT_OPENGL_CORE
//
//
//	// Link shaders into a program and find uniform locations
//	m_Program = glCreateProgram();
//	glBindAttribLocation(m_Program, 0, "pos");
//	glBindAttribLocation(m_Program, 1, "texcoord");
//	glBindAttribLocation(m_Program, 2, "color");
//	glAttachShader(m_Program, m_VertexShader);
//	glAttachShader(m_Program, m_FragmentShader);
//#	if SUPPORT_OPENGL_CORE
//	/*if (m_APIType == kUnityGfxRendererOpenGLCore)
//		glBindFragDataLocation(m_Program, 0, "fragColor");*/
//#	endif // if SUPPORT_OPENGL_CORE
//	glLinkProgram(m_Program);
//
//	GLint status = 0;
//	glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
//	if (status == 0) {
//		char buf[1024];
//		GLint len;
//		glGetProgramInfoLog(m_Program, 1024, &len, buf);
//		std::cout << buf << std::endl;
//	}
//	assert(status == GL_TRUE);
//
//	m_UniformWorldMatrix = glGetUniformLocation(m_Program, "worldMatrix");
//	m_UniformProjMatrix = glGetUniformLocation(m_Program, "projMatrix");
//	m_UniformImage = glGetUniformLocation(m_Program, "texture0");
//	
//
//	// Create vertex buffer
//	glGenBuffers(1, &m_VertexBuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
//	glBufferData(GL_ARRAY_BUFFER, 1024, NULL, GL_STREAM_DRAW);
//
//	
//	testTex = new Texture(R"(C:\Users\oceancx\Pictures\WX_20220912204550.jpg)", true);
//
//	assert(glGetError() == GL_NO_ERROR);

	
	if (m_APIType == kUnityGfxRendererOpenGLCore)
	{
#if UNITY_WIN
		gl3wInit();
#endif
	}
}


RenderAPI_OpenGLCoreES::RenderAPI_OpenGLCoreES(UnityGfxRenderer apiType)
	: m_APIType(apiType)
{
}


void RenderAPI_OpenGLCoreES::ProcessDeviceEvent(UnityGfxDeviceEventType type, IUnityInterfaces* interfaces)
{
	if (type == kUnityGfxDeviceEventInitialize)
	{
		CreateResources();
	}
	else if (type == kUnityGfxDeviceEventShutdown)
	{
		//@TODO: release resources
	}
}


Texture* testTex = nullptr;
void RenderAPI_OpenGLCoreES::DrawSimpleTriangles(const float worldMatrix[16], int triangleCount, const void* verticesFloat3Byte4)
{
	// Set basic render state
	
//
//	// Tweak the projection matrix a bit to make it match what identity projection would do in D3D case.
//	float projectionMatrix[16] = {
//		1,0,0,0,
//		0,1,0,0,
//		0,0,2,0,
//		0,0,-1,1,
//	};
//
//	float texCoord[2] = {
//		0,1
//	};
//
//	// Setup shader program to use, and the matrices
//	glUseProgram(m_Program);
//	glUniformMatrix4fv(m_UniformWorldMatrix, 1, GL_FALSE, worldMatrix);
//	glUniformMatrix4fv(m_UniformProjMatrix, 1, GL_FALSE, projectionMatrix);
//	glUniform1i(m_UniformImage, 0);
//	
//	// Core profile needs VAOs, setup one
//#	if SUPPORT_OPENGL_CORE
//	if (m_APIType == kUnityGfxRendererOpenGLCore)
//	{
//		glGenVertexArrays(1, &m_VertexArray);
//		glBindVertexArray(m_VertexArray);
//	}
//#	endif // if SUPPORT_OPENGL_CORE
// 
//	// Bind a vertex buffer, and update data in it
//	const int kVertexSize = 12 + 8 + 4;
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, kVertexSize * triangleCount * 3, verticesFloat3Byte4);
//
//	// Setup vertex layout
//	glEnableVertexAttribArray(kVertexInputPosition);
//	glVertexAttribPointer(kVertexInputPosition, 3, GL_FLOAT, GL_FALSE, kVertexSize, (char*)NULL + 0);
//
//	glEnableVertexAttribArray(kVertexInputTexCoord);
//	glVertexAttribPointer(kVertexInputTexCoord, 2, GL_FLOAT, GL_FALSE, kVertexSize, (char*)NULL + 12);
//
//	glEnableVertexAttribArray(kVertexInputColor);
//	glVertexAttribPointer(kVertexInputColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, kVertexSize, (char*)NULL + 20);
//
//	if (testTex != nullptr) {
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, testTex->GetTextureID());
//	}
//	// Draw
//	glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);
//
//	// Cleanup VAO
//#	if SUPPORT_OPENGL_CORE
//	if (m_APIType == kUnityGfxRendererOpenGLCore)
//	{
//		glDeleteVertexArrays(1, &m_VertexArray);
//	}
//#	endif

	/*if (testTex == nullptr) {
		testTex = new Texture(R"(I:\Github\CXEngineUnity\UnityProject\Assets\Art\Avatar\xx0.tga)", true);
		SPRITE_RENDERER_INSTANCE;
	}
	SPRITE_RENDERER_INSTANCE->DrawTexture(testTex, { 0,0 } );*/
 
}


void* RenderAPI_OpenGLCoreES::BeginModifyTexture(void* textureHandle, int textureWidth, int textureHeight, int* outRowPitch)
{
	const int rowPitch = textureWidth * 4;
	// Just allocate a system memory buffer here for simplicity
	unsigned char* data = new unsigned char[rowPitch * textureHeight];
	*outRowPitch = rowPitch;
	return data;
}


void RenderAPI_OpenGLCoreES::EndModifyTexture(void* textureHandle, int textureWidth, int textureHeight, int rowPitch, void* dataPtr)
{
	GLuint gltex = (GLuint)(size_t)(textureHandle);
	// Update texture data, and free the memory buffer
	glBindTexture(GL_TEXTURE_2D, gltex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textureWidth, textureHeight, GL_RGBA, GL_UNSIGNED_BYTE, dataPtr);
	delete[](unsigned char*)dataPtr;
}

void* RenderAPI_OpenGLCoreES::BeginModifyVertexBuffer(void* bufferHandle, size_t* outBufferSize)
{
#	if SUPPORT_OPENGL_ES
	return 0;
#	else
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint)(size_t)bufferHandle);
	GLint size = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	*outBufferSize = size;
	void* mapped = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	return mapped;
#	endif
}


void RenderAPI_OpenGLCoreES::EndModifyVertexBuffer(void* bufferHandle)
{
#	if !SUPPORT_OPENGL_ES
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint)(size_t)bufferHandle);
	glUnmapBuffer(GL_ARRAY_BUFFER);
#	endif
}

#endif // #if SUPPORT_OPENGL_UNIFIED
