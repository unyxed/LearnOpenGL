#include "shader.h"
#include <math.h>
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

/* -- CONSTANS & GLOBALS ---------------------------------------------------- */

/* window setting */
const unsigned int WIN_WIDTH  = 800;
const unsigned int WIN_HEIGHT = 600;
const char* WIN_TITLE = "Razer Triangles";

/* vertex data */
float vertices[] = {
    // t1
    -0.44f, -0.2f, 0.0f,
    -0.24f,  0.2f, 0.0f,
    -0.64f,  0.2f, 0.0f,

    // t2
    -0.22f,  0.2f, 0.0f,
    -0.42f, -0.2f, 0.0f,
    -0.02f, -0.2f, 0.0f,

    // t3
     0.0f, -0.2f, 0.0f,
    -0.2f,  0.2f, 0.0f,
     0.2f,  0.2f, 0.0f,

     // t4
     0.22f,  0.2f, 0.0f,
     0.42f, -0.2f, 0.0f,
     0.02f, -0.2f, 0.0f,

     // t5
     0.44f, -0.2f, 0.0f,
     0.24f,  0.2f, 0.0f,
     0.64f,  0.2f, 0.0f
};

/* timing and frames */
float delta_time = 0.0f;
float last_frame = 0.0f;

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

    // shader
    Shader razer_triangles_shader(
        RESOURCE_DIR "shaders/1-getting-started/razer_triangles.vert",
        RESOURCE_DIR "shaders/1-getting-started/razer_triangles.frag"
    );

    // activating our shader
    razer_triangles_shader.use();

    // getting uniform: u_time index (location)
    int u_time_loc = glGetUniformLocation(razer_triangles_shader.shader_program, "u_time");
    if (u_time_loc == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM::GET_LOCATION: Failed" << std::endl;
        return -1;
    }

    // setting up vao, vbo
    unsigned int vbo, vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        // processing the last polled inputs/events
        process_input(window);

        // render
        glClearColor(.2f, .2f, .2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // idk yet
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        float random_value = current_frame;
        glUniform1f(u_time_loc, current_frame);

        // render the triangle
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 3, 3);
        glDrawArrays(GL_TRIANGLES, 6, 3);
        glDrawArrays(GL_TRIANGLES, 9, 3);
        glDrawArrays(GL_TRIANGLES, 12, 3);

        // swap buffer (double bufffering)
        glfwSwapBuffers(window);

        // listen for events
        glfwPollEvents();
    }

    // TODO: cleaning up

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
