
#include "shader.h"
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

/* -- CONSTANS ------------------------------------------------------------- */

/* window setting */
const unsigned int WIN_WIDTH  = 800;
const unsigned int WIN_HEIGHT = 600;
const char* WIN_TITLE = "Rainbow Triangle";

/* vertex data */
float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
};

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

    // TODO: shader
    Shader rainbow_triangle_shader(
        RESOURCE_DIR "shaders/1-getting-started/rainbow_triangle.vert",
        RESOURCE_DIR "shaders/1-getting-started/rainbow_triangle.frag"
    );

    // setting up vao, vbo
    unsigned int vbo, vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // activating our shader
    rainbow_triangle_shader.use();

    while (!glfwWindowShouldClose(window)) {
        // processing the last polled inputs/events
        process_input(window);

        // render
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
