#include "shader.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

/* -- CONSTANS & GLOBALS ---------------------------------------------------- */

/* window setting */
const unsigned int WIN_WIDTH  = 800;
const unsigned int WIN_HEIGHT = 600;
const char* WIN_TITLE = "Many Cubes";

/* vertex data */
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

/* indicies data */
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

/* cube positions */
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

/* texture variables */
int texture_width, texture_height, nr_channels;
unsigned char* data;

/* time */
float delta_time = 0;
float last_frame = 0;

/* ------------------------------------------------------------------------- */

/* -- CALLBACKS ------------------------------------------------------------ */
void framebuffer_size_callback(GLFWwindow* window, int w, int h);
/* ------------------------------------------------------------------------- */

/* -- HELPER FUNCTIONS ----------------------------------------------------- */

/* helper: sets up glfw */
void setup_glfw();

/* helper: creates window */
GLFWwindow* create_glfw_window();

/* helper: sets up all the glfw callback functions */
void setup_glfw_callbacks(GLFWwindow* window);

/* helper: loads opengl function pointers */
bool load_opengl_funcptrs();

/* helper: load texture and generate mipmaps */
void setup_container_texture(unsigned int texture_id);
void setup_smiley_texture(unsigned int texture_id);

/* helper: processes user inputs */
void process_input(GLFWwindow* window);

/* ------------------------------------------------------------------------- */

int main() {

    // setting up glfw
    setup_glfw();

    // creating glfw window
    GLFWwindow* window = create_glfw_window();
    if (window == nullptr) return -1;

    // make context current
    glfwMakeContextCurrent(window);

    // setting glfw callbacks
    setup_glfw_callbacks(window);

    // loading opengl function pointers
    if (!load_opengl_funcptrs()) return -1;

    // configure opengl global state
    glEnable(GL_DEPTH_TEST);

    // shader
    Shader cube_shader(
        RESOURCE_DIR "shaders/1-getting-started/cube.vert",
        RESOURCE_DIR "shaders/1-getting-started/cube.frag"
    );

    // activating our shader
    cube_shader.use();

    // setting up vao, vbo
    unsigned int vbo, vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    // bind vao to save configurations
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // setting up textures
    stbi_set_flip_vertically_on_load(true);

    // get and set texture uniforms
    unsigned int u_texture1 = glGetUniformLocation(cube_shader.shader_program, "texture1");
    unsigned int u_texture2 = glGetUniformLocation(cube_shader.shader_program, "texture2");
    glUniform1i(u_texture1, 0);
    glUniform1i(u_texture2, 1);

    // texture1
    unsigned int texture1;
    glGenTextures(1, &texture1);
    setup_container_texture(texture1);

    // texture2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    setup_smiley_texture(texture2);

    // get transformation uniforms
    unsigned int u_model = glGetUniformLocation(cube_shader.shader_program, "model");
    unsigned int u_view = glGetUniformLocation(cube_shader.shader_program, "view");
    unsigned int u_projection = glGetUniformLocation(cube_shader.shader_program, "projection");

    // transformation logic: projection matrix (can be set once no need to set it every frame)
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);

    while (!glfwWindowShouldClose(window)) {
        // processing the last polled inputs/events
        process_input(window);

        // calculate time/frame
        float current_frame = (float)glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // transformation logic: view matrix
        glm::mat4 view  = glm::mat4(1.0f);
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glUniformMatrix4fv(u_view, 1, GL_FALSE, &view[0][0]);

        // clear color and depth buffers each frame before rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // render the triangle
        glBindVertexArray(vao);
        for (unsigned int i{0}; i < 10; i++) {
            // transformation logic: model matrix
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i + 1);
            model = glm::rotate(model, glm::radians(angle * (float)glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // swap buffer (double bufffering)
        glfwSwapBuffers(window);

        // listen for events
        glfwPollEvents();
    }

    // cleaning up
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
};

/* -- HELPER FUNCTIONS ----------------------------------------------------- */

void setup_glfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

GLFWwindow* create_glfw_window() {
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "ERROR::GLFW::WINDOW::CREATION: Failed" << std::endl;
        glfwTerminate();
    }
    return window;
}

void setup_glfw_callbacks(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

bool load_opengl_funcptrs() {
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cout << "ERROR::GLAD::INITIALIZATION:FAILED" << std::endl;
        return false;
    }
    return true;
}

void setup_container_texture(unsigned int texture_id) {
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    data = stbi_load(RESOURCE_DIR "textures/container.jpg", &texture_width, &texture_height, &nr_channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "ERROR::TEXTURE::LOADING: Failed" << std::endl;
    }
    stbi_image_free(data);
}

void setup_smiley_texture(unsigned int texture_id) {
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    data = stbi_load(RESOURCE_DIR "textures/awesomeface.png", &texture_width, &texture_height, &nr_channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "ERROR::TEXTURE::LOADING: Failed" << std::endl;
    }
    stbi_image_free(data);
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/* ------------------------------------------------------------------------- */

/* -- CALLBACKS ------------------------------------------------------------ */

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

/* ------------------------------------------------------------------------- */
