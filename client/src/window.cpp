#include "window.h"

#include "graphics/shader.h"
#include "file_system.h"
#include "input_manager.h"
#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "time/time.h"
#include <script_system.h>
#include "input_manager.h"
#include "file_loading.h"
#include "scene/scene_manager.h"
#include "logger.h"
#include "cxlua.h"
#include "sprite_renderer.h"
#include <graphics/ui_renderer.h>
extern "C" {
	void font_size(const char* str, int unicode, struct font_context* ctx) {}
	void font_glyph(const char* str, int unicode, void* buffer, struct font_context* ctx) {}
	void font_create(int font_size, struct font_context* ctx) {}
	void font_release(struct font_context* ctx) {}
}

#define GAME_SCREEN_WIDTH 800
#define GAME_SCREEN_HEIGHT 600

class GLFWwindow
{

};
static const float MS_PER_UPDATE = 1000 / 60.f / 1000;
int m_Width = 0;
int m_Height = 0;
int m_WindowWidth;
int m_WindowHeight;
float g_ScreenScale = 1.f;
float g_DeltaTime = MS_PER_UPDATE;
GLFWwindow* m_pWindow = nullptr;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


unsigned int m_Fbo;
unsigned int m_Rbo;
unsigned int m_TextureColor;
 
int Window::GetWidth() { return m_Width; };
int Window::GetHeight() { return m_Height; };
int Window::GetWindowWidth() { return m_WindowWidth; };
int Window::GetWindowHeight() { return m_WindowHeight; };
float Window::GetCenterX() { return GetWidth() / 2.f; }
float Window::GetCenterY() { return GetHeight() / 2.f; }
float Window::GetFPS() { return g_DeltaTime; }

int Window::GetRenderTexture() { return m_TextureColor; }
int Window::GetFrameBuffer() { return m_Fbo; }

float Window::GetScale()
{
	return g_ScreenScale;
}
void create_game_rendertexture() {
	glGenFramebuffers(1, &m_Fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);

	glGenTextures(1, &m_TextureColor);
	glBindTexture(GL_TEXTURE_2D, m_TextureColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColor, 0);

	glGenRenderbuffers(1, &m_Rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_Rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cxlog_err("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

static void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;

	SpriteRenderer::GetInstance()->UpdateProjection();
	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);

	create_game_rendertexture();


	glViewport(0, 0, m_WindowWidth, m_WindowHeight);
}

static void glfw_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	INPUT_MANAGER_INSTANCE->MouseButtonCallback(window, button, action, mods);
}

static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mode);
	INPUT_MANAGER_INSTANCE->KeyCallbackFunc(window, key, scancode, action, mode);
}
static void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	xoffset /= g_ScreenScale;
	yoffset /= g_ScreenScale;
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	INPUT_MANAGER_INSTANCE->ScrollCallbackFunc(window, (float)xoffset, (float)yoffset);
}

static void glfw_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	xpos/= g_ScreenScale;
	ypos/= g_ScreenScale;
	INPUT_MANAGER_INSTANCE->MouseCallbackFunc(window, (float)xpos, (float)ypos);
}

static void glfw_character_callback(GLFWwindow* window, unsigned int charcode)
{
	ImGui_ImplGlfw_CharCallback(window, charcode);
	INPUT_MANAGER_INSTANCE->CharacterInputCallback(window, charcode);
}

static bool s_WindowFocused = false;

static std::vector<CXString>  s_DropFiles;
static bool s_DropTriggered = false;
static void glfw_drog_function(GLFWwindow* window, int path_count, const char* paths[])
{
	s_DropTriggered = true;
	s_DropFiles.clear();
	//printf("%d \n", path_count);
	for (int i = 0; i < path_count; i++) {
		//printf("%s \n", paths[i]);
		s_DropFiles.push_back(paths[i]);
	}
	if (!s_WindowFocused) {
		//glfwFocusWindow(window);
	}
}

static void glfw_focus_function(GLFWwindow* window, int focused) {
	s_WindowFocused = focused != 0;
}


static void glfw_error_callback(int error, const char* description) {
	cxlog_err("Error: %d %s\n", error, description);
}

Window::Window()
{

}

Window::~Window() {

}

void Window::Init(int w, int h)
{
}

void Window::Destroy()
{

}

void Window::Show()
{

}

float Window::GetDeltaTime()
{
	return g_DeltaTime;
}


float Window::GetDeltaTimeMilliseconds()
{
	return g_DeltaTime * 1000.f;
}

void Window::OnFrameBufferSizeCallback(int width, int height)
{

}

void iw_set_font(const char* path, int size) {
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF(path, size * g_ScreenScale, NULL, io.Fonts->GetGlyphRangesChineseFull());
}

const char* sprite_fs = R"(#version 100
precision lowp float;
varying vec2 v_texcoord;
varying vec4 v_color;
varying vec4 v_additive;
uniform sampler2D texture0;

void main() {
	gl_FragColor = v_color;
	/*vec4 tmp = texture2D(texture0, v_texcoord);
	gl_FragColor.xyz = tmp.xyz * v_color.xyz;
	gl_FragColor.w = tmp.w;
	gl_FragColor *= v_color.w;
	gl_FragColor.xyz += v_additive.xyz * tmp.w;*/
})";

const char* sprite_vs = R"(#version 100
precision lowp float;
attribute vec4 position;
attribute vec2 texcoord;
attribute vec4 color;
attribute vec4 additive;

varying vec2 v_texcoord;
varying vec4 v_color;
varying vec4 v_additive;

void main() {
	gl_Position = position; //+ vec4(-1.0, 1.0, 0, 0);
	v_texcoord = texcoord;
	v_color = color;
	v_additive = additive;
})";


struct material;
void iw_init(int w, int h)
{

 

}

void window_system_set_floating(int opt, int value)
{
	WINDOW_INSTANCE;
}

float window_system_get_dt()
{
	return WINDOW_INSTANCE->GetDeltaTimeMilliseconds();
}

float window_system_get_fps()
{
	return WINDOW_INSTANCE->GetFPS();
}

int game_get_width() {
	return WINDOW_INSTANCE->GetWidth();
}
int game_get_height() {
	return WINDOW_INSTANCE->GetHeight();
}

bool iw_should_close()
{
	return false;
}

void iw_deinit() {
	 

	auto* ne_thread = file_loading_thread();
	delete ne_thread;
	ne_thread = nullptr;
}

void iw_function_to_select_shader_or_blend_state(const ImDrawList* parent_list, const ImDrawCmd* cmd) {
	glDisable(GL_BLEND);
}

void iw_function_to_restore_shader_or_blend_state(const ImDrawList* parent_list, const ImDrawCmd* cmd) {
	glEnable(GL_BLEND);
}

//	shader_clear(0xff00ff00);
		// float x = 0;
		// float y = 0;
		// float w = 160;
		// float h = 160;
		// uint32_t color = 0xff0000ff;
		// struct vertex_pack vp[4];
		// vp[0].vx = x;
		// vp[0].vy = y;
		// vp[1].vx = x + w;
		// vp[1].vy = y;
		// vp[2].vx = x + w;
		// vp[2].vy = y + h;
		// vp[3].vx = x;
		// vp[3].vy = y + h;

		// int i;
		// for (i = 0; i < 4; i++) {
		// 	vp[i].tx = 0;
		// 	vp[i].ty = 0;
		// 	screen_trans(&vp[i].vx, &vp[i].vy);
		// }

		// shader_program(0, nullptr);
		// shader_draw(vp, color, 0);

		// shader_flush();

int iw_render(lua_State* L)
{
	lua_pushvalue(L, 1);
	 

	return 0;

}

bool iw_is_dropped() {
	return s_DropTriggered;
}

void iw_set_dropped(bool dropped) {
	s_DropTriggered = dropped;

}

int iw_get_drop_files(lua_State* L) {
	lua_newtable(L);
	for (int i = 0; i < s_DropFiles.size(); i++) {
		lua_pushlstring(L, s_DropFiles[i].c_str(), s_DropFiles[i].size());
		lua_seti(L, -2, i + 1);
	}
	return 1;
}


void luaopen_window(lua_State* L)
{
	script_system_register_function(L, window_system_set_floating);
	script_system_register_function(L, window_system_get_dt);
	script_system_register_function(L, window_system_get_fps);

	script_system_register_function(L, game_get_width);
	script_system_register_function(L, game_get_height);


	script_system_register_function(L, iw_deinit);
	script_system_register_function(L, iw_should_close);

	script_system_register_function(L, iw_set_font);

	script_system_register_function(L, iw_init);

	script_system_register_luac_function(L, iw_render);

	script_system_register_function(L, iw_is_dropped);
	script_system_register_function(L, iw_set_dropped);
	script_system_register_luac_function(L, iw_get_drop_files);



}
