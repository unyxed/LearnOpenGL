#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "../../include/shader.h"

// callbacks & handlers
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window);

// lighting
glm::vec3 lightPos(1.2f, 1.0, 2.0f);

// window settings
const unsigned int WINDOW_WIDTH  = 800;
const unsigned int WINDOW_HEIGHT = 600;

// timing
float deltaTime = 0.0f; // measures the elapsed time since the previous frame
float lastFrame = 0.0f; 
float currentFrame;	

// camera
const float cameraSpeed = 0.5f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// mouse
float lastX = WINDOW_WIDTH / 2;
float lastY = WINDOW_HEIGHT /2;
float yaw   = -90.0f;
float pitch = 0.0f;
float fov   = 45.0f;

int main() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWART_COMPAT, GL_TRUE);
#endif
	
	// glfw: window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL - Colors", NULL, NULL);
	if (window == NULL) {
		std::cerr << "ERROR::GLFW::FAILED_WINDOW_CREATION" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // TODO: FUCK AROUND
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "ERROR::GLAD::FAILED_INITIALIZING_GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	
	// build and compile our shader programs
	Shader lightingShader(
		"../resources/shaders/2_Lighting/colors_vertex.vert", 
		"../resources/shaders/2_Lighting/colors_fragment.frag"
	);

	/*
		Shader lightCubeShader(
		"../resources/shaders/2_Lighting/light_cube_vertex.vert",
		"../resources/shaders/2_Lighting/light_cube_fragment.frag"
		);
	*/
	
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
			-0.5f, -0.5f, -0.5f, 
			 0.5f, -0.5f, -0.5f,  
			 0.5f,  0.5f, -0.5f,  
			 0.5f,  0.5f, -0.5f,  
			-0.5f,  0.5f, -0.5f, 
			-0.5f, -0.5f, -0.5f, 

			-0.5f, -0.5f,  0.5f, 
			 0.5f, -0.5f,  0.5f,  
			 0.5f,  0.5f,  0.5f,  
			 0.5f,  0.5f,  0.5f,  
			-0.5f,  0.5f,  0.5f, 
			-0.5f, -0.5f,  0.5f, 

			-0.5f,  0.5f,  0.5f, 
			-0.5f,  0.5f, -0.5f, 
			-0.5f, -0.5f, -0.5f, 
			-0.5f, -0.5f, -0.5f, 
			-0.5f, -0.5f,  0.5f, 
			-0.5f,  0.5f,  0.5f, 

			 0.5f,  0.5f,  0.5f,  
			 0.5f,  0.5f, -0.5f,  
			 0.5f, -0.5f, -0.5f,  
			 0.5f, -0.5f, -0.5f,  
			 0.5f, -0.5f,  0.5f,  
			 0.5f,  0.5f,  0.5f,  

			-0.5f, -0.5f, -0.5f, 
			 0.5f, -0.5f, -0.5f,  
			 0.5f, -0.5f,  0.5f,  
			 0.5f, -0.5f,  0.5f,  
			-0.5f, -0.5f,  0.5f, 
			-0.5f, -0.5f, -0.5f, 

			-0.5f,  0.5f, -0.5f, 
			 0.5f,  0.5f, -0.5f,  
			 0.5f,  0.5f,  0.5f,  
			 0.5f,  0.5f,  0.5f,  
			-0.5f,  0.5f,  0.5f, 
			-0.5f,  0.5f, -0.5f, 
	};

	// cube: configuring the cube's VAO and VBO
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(cubeVAO);
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// light cube: configure the light cube's VAO (VBO is the same as the one used for "cube")
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // redundant

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		// --------------------	
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// input
		// -----
		processInput(window);
		
		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
		// make sure to activate shaders when setting uniforms/drawing objects
		float objectColor[] {1.0f, 0.5f, 0.31f};
		float lightColor[] {1.0f, 1.0f, 1.0f};	
		lightingShader.use();
		glUniform3fv(glGetUniformLocation(lightingShader.shader_program, "objectColor"), 1, objectColor);
		glUniform3fv(glGetUniformLocation(lightingShader.shader_program, "lightColor"),  1, lightColor);			
		
		// view/projection transformation	
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.shader_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		
		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.shader_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		
		// draw the cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		// glfw:: swap buffers and pull IO events (key pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);		
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraFront * cameraSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraFront * cameraSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += cameraUp * cameraSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) 
		cameraPos -= cameraUp * cameraSpeed * deltaTime;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;
	
	const float mouse_sensitivity = 0.1f;
	xoffset *= mouse_sensitivity;
	yoffset *= mouse_sensitivity;
	
	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) 
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	
	glm::vec3 front_direction;
	front_direction.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
	front_direction.y = sin(glm::radians(pitch));
	front_direction.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));
	cameraFront = glm::normalize(front_direction); 
}