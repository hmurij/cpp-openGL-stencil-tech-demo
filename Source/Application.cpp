#include <glad/glad.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <iomanip>
#include <bitset>
#include <math.h>

#include "ShaderProgram.h"
#include "Camera.h"
#include "stb_image.h"
#include "KTX.h"
#include "Object.h"

// Include GLM core features
#include <glm/glm.hpp>																																																																																																													
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


// window properties
unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;

// set up transformation matricies
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

// set up camera
glm::vec3 cameraPos = glm::vec3(0.00f, 0.0f, 3.0f);

Camera camera(cameraPos);

// Camera camera(cameraPos, glm::vec3(0.0, 1.0, 0.0), -90.0, -30.0);

// (glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)

float lastX = (float)SCR_WIDTH / 2, lastY = (float)SCR_HEIGHT / 2;
bool firstMouse = true;
bool freeView = false;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// declare utility functions
void processInput(GLFWwindow* window);
void renderGUI();
std::string matrixToString(std::string matrixName, glm::mat4& matrix);
std::string vec3ToString(std::string vectorName, glm::vec3 vector);
void generate_texture(float* data, int width, int height);
unsigned int loadTexture(const char* path);
static inline float random_float();

void setTextureParamters(GLenum pname, GLint param);
int getGlintParam(const char* param);


// glfw utility functions
void window_size_callback(GLFWwindow* window, int width, int height);
void window_iconify_callback(GLFWwindow* window, int iconified);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
int setUpGLFW(GLFWwindow* window);

bool minized = false;

// const global values
const float PI = 3.1415927f;

// Random number generator
static unsigned int seed = 0x13371337;

// Application global varialbes
bool rotation = false; // leave for ratation pause
bool stencil_test = true;

int main(void)
{
	GLFWwindow* window;
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL SuperBible Example 9.06 - Stencil Test Object outlining", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	setUpGLFW(window);

	// set up shader program
	ShaderProgram shader_program("Resource/Shaders/simple.shader");

	// set up object
	/*Object object;
	object.load("Resource/Objects/torus.sbm");*/

	// set up vertices

	// Set up textures
	/*unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	ktx::load("Resource/Textures/terragen1.ktx", texture);*/

	/*unsigned int kitty, puppy;
	stbi_set_flip_vertically_on_load(true);
	glActiveTexture(GL_TEXTURE0);
	kitty = loadTexture("Resource/Textures/kitten.png");
	glActiveTexture(GL_TEXTURE1);
	puppy = loadTexture("Resource/Textures/puppy.png");*/


	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	// set up vertex array and buffer objects
	GLfloat vertices[] = {
		-0.25f,  0.25f, -0.25f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.25f, -0.25f, -0.25f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.25f, -0.25f, -0.25f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.25f, -0.25f, -0.25f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.25f,  0.25f, -0.25f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.25f,  0.25f, -0.25f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		 0.25f, -0.25f, -0.25f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.25f, -0.25f,  0.25f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.25f,  0.25f, -0.25f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.25f, -0.25f,  0.25f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.25f,  0.25f,  0.25f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.25f,  0.25f, -0.25f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		 0.25f, -0.25f,  0.25f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.25f, -0.25f,  0.25f,	 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.25f,  0.25f,  0.25f,	 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.25f, -0.25f,  0.25f,	 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.25f,  0.25f,  0.25f,	 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.25f,  0.25f,  0.25f,	 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		 -0.25f, -0.25f,  0.25f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 -0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 -0.25f,  0.25f,  0.25f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 -0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 -0.25f,  0.25f, -0.25f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 -0.25f,  0.25f,  0.25f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		 -0.25f, -0.25f,  0.25f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		  0.25f, -0.25f,  0.25f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		  0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		  0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 -0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 -0.25f, -0.25f,  0.25f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		 -0.25f,  0.25f, -0.25f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		  0.25f,  0.25f, -0.25f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		  0.25f,  0.25f,  0.25f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		  0.25f,  0.25f,  0.25f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 -0.25f,  0.25f,  0.25f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 -0.25f,  0.25f, -0.25f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		 -0.5f,  -0.5f,   0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // square form 36 6 vertices
		  0.5f,  -0.5f,   0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		  0.5f,   0.5f,   0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		  0.5f,   0.5f,   0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 -0.5f,   0.5f,   0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 -0.5f,  -0.5f,   0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		 -1.00f, -1.00f,  0.00f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // red triangle 42 3 vertices
		  1.00f, -1.00f,  0.00f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		  0.00f,  1.00f,  0.00f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f,

		 -0.75f, -0.75f,  0.00f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // green square 45 6 vertices
		  0.75f, -0.75f,  0.00f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		  0.75f,  0.75f,  0.00f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 -0.75f,  0.75f,  0.00f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 -0.75f, -0.75f,  0.00f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		  0.75f,  0.75f,  0.00f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

		 -0.75f, -0.75f,  0.00f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // blue square 51 6 vertices
		  0.75f, -0.75f,  0.00f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		  0.75f,  0.75f,  0.00f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 -0.75f,  0.75f,  0.00f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 -0.75f, -0.75f,  0.00f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		  0.75f,  0.75f,  0.00f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		  0.00f,  0.00f,  0.00f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // single white dot center 57 1 vertice
		  
	};

	unsigned int vao, position_buffer;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	float angle;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 450";
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Our state
	// bool show_demo_window = true;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// set up variables
		angle = (float)glfwGetTime();
		angle = angle * 0.5f;
		float radius = 5.0f;

		// leave it for free view camera
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process input
		processInput(window);
		/* Render here */

		// leave it for object rotation
		static float total_time = 0;
		static float last_time = 0;

		if (rotation) {
			total_time += (angle - last_time);
		}
		last_time = angle;

		static const GLfloat color[] = { 0.33f, 0.33f, 0.33f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);

		static const GLfloat one = 1.0f;
		glClearBufferfv(GL_DEPTH, 0, &one);

		model = glm::mat4(1.0f);
		
		view = camera.GetViewMatrix();
		projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

		shader_program.use();
		shader_program.setProgramMat4("model", model);
		shader_program.setProgramMat4("view", view);
		shader_program.setProgramMat4("projection", projection);

		// shader_program.setVec4("override_color", glm::vec4(1.0, 0.0, 0.0, 1.0));

		if (stencil_test) {
			// specify the clear value for the stencil buffer. The initial value is 0.
			glClearStencil(0);

			// control the front and back writing of individual bits in the stencil planes
			// specifies a bit mask to enable and disable writing of individual bits in the stencil planes. Initially, the mask is all 1's.
			glStencilMask(1);

			// enable stencil test
			glEnable(GL_STENCIL_TEST);

			// clear the stencil buffer each iteration with bit value set by glClearStencil() function
			glClear(GL_STENCIL_BUFFER_BIT);

			glStencilFunc(GL_NOTEQUAL, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		}

		shader_program.setVec4("override_color", glm::vec4(1.0, 1.0, 1.0, 1.0));
		model = glm::rotate(model, total_time, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, total_time * 2.0f, glm::vec3(0.6, 0.8, 0.0));
		shader_program.setProgramMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// glPointSize(100.0f);
		// glDrawArrays(GL_POINTS, 57, 1);

		if (stencil_test) {
			glStencilFunc(GL_EQUAL, 0, 1);
			// glStencilMask(0x00);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		}
		
		model = glm::mat4(1.0f);
		
		model = glm::rotate(model, total_time, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
		model = glm::rotate(model, total_time * 2.0f, glm::vec3(0.6, 0.8, 0.0));
		shader_program.setMat4("model", model);
		shader_program.setVec4("override_color", glm::vec4(1.0, 0.0, 0.0, 1.0));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		if (stencil_test) {
			glClear(GL_STENCIL_BUFFER_BIT);

			glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF); // Write to stencil buffer
			glDepthMask(GL_FALSE); // Don't write to depth buffer
		}

		// draw black square
		model = glm::mat4(1.0f);
		model = glm::rotate(model, total_time, glm::vec3(0.0, 1.0, 0.0));
		model = glm::scale(model, glm::vec3(1.75f, 1.75f, 1.75f));
		shader_program.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 36, 6);

		if (stencil_test) {
			// Draw cube reflection
			glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			// glStencilMask(0x00); // Don't write anything to stencil buffer
			glDepthMask(GL_TRUE); // Write to depth buffer
		}

		model = glm::mat4(1.0f);
		// model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
		model = glm::rotate(model, total_time, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, total_time * 2.0f, glm::vec3(0.6, 0.8, 0.0));
		shader_program.setMat4("model", model);
		shader_program.setVec4("override_color", glm::vec4(1.0, 1.0, 1.0, 1.0));

		if (stencil_test) {
			if (std::fmod(total_time, (PI * 2.0f)) >= PI * 2.0f * (1.0f / 4.0f) && std::fmod(total_time, (PI * 2.0f)) <= PI * 2.0f * (3.0f / 4.0f)) {
				shader_program.setVec4("override_color", glm::vec4(0.0, 0.0, 0.0, 1.0));
			}
			else
			{
				shader_program.setVec4("override_color", glm::vec4(1.0, 1.0, 1.0, 1.0));
			}
		}
		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		if (stencil_test) {
			// draw reflection cube outlining
			glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		}

		model = glm::mat4(1.0f);
		model = glm::rotate(model, total_time, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
		model = glm::rotate(model, total_time * 2.0f, glm::vec3(0.6, 0.8, 0.0));
		shader_program.setMat4("model", model);
		shader_program.setVec4("override_color", glm::vec4(1.0, 0.0, 0.0, 1.0));

		if (stencil_test) {
			if (std::fmod(total_time, (PI * 2.0f)) >= PI * 2.0f * (1.0f / 4.0f) && std::fmod(total_time, (PI * 2.0f)) <= PI * 2.0f * (3.0f / 4.0f)) {
				shader_program.setVec4("override_color", glm::vec4(0.0, 0.0, 0.0, 1.0));
			}
			else
			{
				shader_program.setVec4("override_color", glm::vec4(1.0, 0.0, 0.0, 1.0));
			}
		}
		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		if (stencil_test) {
			glDisable(GL_STENCIL_TEST);
		}

		renderGUI();

		// Start the Dear ImGui frame
	   /*ImGui_ImplOpenGL3_NewFrame();
	   ImGui_ImplGlfw_NewFrame();
	   ImGui::NewFrame();*/

	   // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	   /*if (show_demo_window)
		   ImGui::ShowDemoWindow(&show_demo_window);*/

		   // Rendering
		   /*ImGui::Render();
		   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/


		   /* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	/*glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &buffer);*/
	// glDeleteTextures(2, tex_object);

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}


void renderGUI() {

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(" ");

	ImGui::SetWindowPos(ImVec2(0, 0));

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Separator();
	ImGui::Checkbox("Rotation", &rotation);
	ImGui::Separator();

	ImGui::Checkbox("Stencil test", &stencil_test);
	ImGui::Separator();

	ImGui::Text("Depth test");
	static int depth_test = 1;

	ImGui::RadioButton("Enable Depth test", &depth_test, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Disable Depth test", &depth_test, 0);

	if (depth_test)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	ImGui::Separator();



	ImGui::Text("Polygon mode");
	static int polygon_mode = 1;

	ImGui::RadioButton("Line", &polygon_mode, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Fill", &polygon_mode, 1);

	if (polygon_mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	ImGui::Separator();


	ImGui::Text("Culling");
	static int culling = 0;
	ImGui::RadioButton("Enable culling", &culling, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Disable culling", &culling, 0);

	if (culling)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	ImGui::Separator();

	ImGui::Text("Winding order");
	static int front_face = 0;
	ImGui::RadioButton("Clock wise", &front_face, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Counter clock wise", &front_face, 0);

	if (front_face)
		glFrontFace(GL_CW);
	else
		glFrontFace(GL_CCW);
	ImGui::Separator();

	ImGui::Text("Blend");
	static int blend = 0;
	ImGui::RadioButton("Enable blend", &blend, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Disable blend", &blend, 0);
	ImGui::Separator();

	if (blend) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else if (!blend) {
		glDisable(GL_BLEND);
	}

	ImGui::End();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

// glfw utility functions
int setUpGLFW(GLFWwindow* window) {

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	// set up window callbacks
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetWindowIconifyCallback(window, window_iconify_callback);
	// glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	/*int data;
	glGetIntegerv(GL_MAX_VIEWPORTS, &data);*/

	std::cout << "-- --------------------------------------------------------------------------------------------- -- \n"
		<< "openGL version: " << glGetString(GL_VERSION) << std::endl
		<< "Renderer : " << glGetString(GL_RENDERER) << std::endl
		<< "Vendor: " << glGetString(GL_VENDOR) << std::endl
		<< "Shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl
		<< "-- --------------------------------------------------------------------------------------------- -- " << std::endl;

	return 1;
} // end of setUpGLFW

void window_size_callback(GLFWwindow* window, int width, int height)
{
	// std::cout << "window size change" << std::endl;
	if (!minized) {
		SCR_WIDTH = width;
		SCR_HEIGHT = height;
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	}
}

void window_iconify_callback(GLFWwindow* window, int iconified)
{
	if (iconified)
		minized = true;
	else
		minized = false;
}

void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

} // end of processInput

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (mods == GLFW_MOD_CONTROL && (key == GLFW_KEY_C && action == GLFW_PRESS)) {


		freeView = !freeView;

		if (freeView) {
			// std::cout << "enable free camera" << std::endl;

			glfwSetCursorPosCallback(window, mouse_callback);
			// glfwSetScrollCallback(window, scroll_callback);

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		}
		else {
			// std::cout << "disable free camera" << std::endl;
			glfwSetCursorPosCallback(window, NULL);
			glfwSetScrollCallback(window, NULL);

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			firstMouse = true;

		}

	}

}

std::string matrixToString(std::string matrixName, glm::mat4& matrix)
{
	std::stringstream streamString;
	streamString << matrixName << std::endl;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			streamString << std::showpoint << std::setw(10) << std::fixed << matrix[j][i] << " ";
		}

		streamString << std::endl;
	}

	return streamString.str();
}

std::string vec3ToString(std::string vectorName, glm::vec3 vector)
{
	std::stringstream streamString;
	streamString << vectorName << std::endl
		<< std::showpoint << std::setw(10) << std::fixed << vector.x << " " << vector.y << " " << vector.z << std::endl;

	return streamString.str();
}

void generate_texture(float* data, int width, int height)
{
	int x, y;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			data[(y * width + x) * 4 + 0] = (float)((x & y) & 0xFF) / 255.0f;
			data[(y * width + x) * 4 + 1] = (float)((x | y) & 0xFF) / 255.0f;
			data[(y * width + x) * 4 + 2] = (float)((x ^ y) & 0xFF) / 255.0f;
			data[(y * width + x) * 4 + 3] = 1.0f;
		}
	}
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void setTextureParamters(GLenum pname, GLint param) {

}

static inline float random_float()
{
	float res;
	unsigned int tmp;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int*)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}

int getGlintParam(const char* param) {
	int parameter = 0;

	if (strcmp(param, "GL_NEAREST") == 0)
		parameter = GL_NEAREST;
	else if (strcmp(param, "GL_LINEAR") == 0)
		parameter = GL_LINEAR;
	else if (strcmp(param, "GL_NEAREST_MIPMAP_NEAREST") == 0)
		parameter = GL_NEAREST_MIPMAP_NEAREST;
	else if (strcmp(param, "GL_LINEAR_MIPMAP_NEAREST") == 0)
		parameter = GL_LINEAR_MIPMAP_NEAREST;
	else if (strcmp(param, "GL_NEAREST_MIPMAP_LINEAR") == 0)
		parameter = GL_NEAREST_MIPMAP_LINEAR;
	else if (strcmp(param, "GL_LINEAR_MIPMAP_LINEAR") == 0)
		parameter = GL_LINEAR_MIPMAP_LINEAR;

	return parameter;
}

