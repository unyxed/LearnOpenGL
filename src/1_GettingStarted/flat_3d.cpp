#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#define STB_IMAGE_IMPLEMENTATION

#include "../../include/shader.h"
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

const unsigned int WINDOW_WIDTH  = 800;
const unsigned int WINDWO_HEIGHT = 600;

int main() {

  // glfw: initialization and configuration
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__ 
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

  // glfw window creation 
  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDWO_HEIGHT, "Flat_3D", NULL, NULL);
  if (window == nullptr) {
    std::cerr << "Faild To Create GLFW Window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Faild To Initialize GLAD" << std::endl;
    return -1;
  }

  // Build and Compiler Our Shader Program
  // ----------------------------
  Shader ourShader("../resources/shaders/vertex.vert", "../resources/shaders/fragment.frag");
  // ----------------------------

  // vertex data 	
	float vertices[] = {
		// coordinates      // colors          // texture coords
		0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,         // top right
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0, 0.0,           // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,         // bottom left 
		-0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,         // top left
	};

	// indices
  unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3, // second triangle
  };

  // Vertex Buffer Object, Vertex Array Object, Element Buffer Object 
  // ----------------------------

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO); 

  glGenBuffers(1, &VBO); 
  glGenBuffers(1, &EBO); 

  glBindVertexArray(VAO); 

  glBindBuffer(GL_ARRAY_BUFFER, VBO); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // ----------------------------
  
  // Configuring Vertex Attributes 
  // ----------------------------
  
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // color attribute 
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // texture coords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
 
  // ----------------------------

  // Load and Create Texture 
  // ----------------------------

  unsigned int texture1, texture2;
  
  // texture 1
  glGenTextures(1, &texture1); 
  glBindTexture(GL_TEXTURE_2D, texture1); 

  // setting texture "wrapping" parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // setting texture "filtering" parameters 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // load image, create texture, generate mipmaps
  int width, height, nr_channels;
  stbi_set_flip_vertically_on_load(true);

  unsigned char *data = stbi_load("../resources/textures/container.jpg", &width, &height, &nr_channels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Failed To Load Texture" << std::endl;
    return -1;
  }
  stbi_image_free(data);

  // texture 2
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  // setting texture "wrapping" parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // setting texture "filtering" parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // load image, create texture, generate mipmaps
  data = stbi_load("../resources/textures/awesomeface.png", &width, &height, &nr_channels, 0);
  if (data) {
    // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Failed To Load Texture" << std::endl;
    return -1;
  }
  stbi_image_free(data);

  ourShader.use();  // must activate/use the shader before setting uniforms
  glUniform1i(glGetUniformLocation(ourShader.shader_program, "texture1"), 0);
  glUniform1i(glGetUniformLocation(ourShader.shader_program, "texture2"), 1);
		
  // Render Loop
  // ----------------------------
  while (!glfwWindowShouldClose(window)) {
    // process input
    process_input(window);

    // render 
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind and activate textures 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1); 

    // bind and activate texture 1 
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
		//glm::mat4 trans = glm::mat4(1.0f);
    //trans = glm::rotate(trans, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
    //trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

    ourShader.use();
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0f, 0.0f));
		
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 
		
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    //unsigned int transfomrLoc = glGetUniformLocation(ourShader.shader_program, "transform");
    //glUniformMatrix4fv(transfomrLoc, 1, GL_FALSE, glm::value_ptr(trans));
		
		unsigned int modelLoc = glGetUniformLocation(ourShader.shader_program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		unsigned int viewLoc = glGetUniformLocation(ourShader.shader_program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		unsigned int projectionLoc = glGetUniformLocation(ourShader.shader_program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // render container
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // ----------------------------

  // De-allocating Resources
  // ----------------------------

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  
  // ----------------------------

  glfwTerminate();
  return 0;
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
