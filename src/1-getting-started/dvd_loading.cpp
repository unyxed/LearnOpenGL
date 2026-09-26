#include "shader.h"
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
const char* WIN_TITLE = "DvD Loading";

/* vertex data */
float vertices[] = {
    // positions          // texture coords
     0.3f,  0.2f, 0.0f,   1.0f, 1.0f, // top right
     0.3f, -0.2f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.3f, -0.2f, 0.0f,   0.0f, 0.0f, // bottom left
    -0.3f,  0.2f, 0.0f,   0.0f, 1.0f  // top left
};

/* indicies data */
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

/* texture variables */
float scale = 0.2f;
int texture_width, texture_height, nr_channels;
unsigned char* data;

/* time */
float delta_time = 0;
float last_frame = 0;

/* logo speed & position */
float speed = 0.01f;
float half_width = 0.3f * scale;
float half_height = 0.2f * scale;
glm::vec2 pos = glm::vec2(0.0f, 0.0f);
glm::vec2 velocity = glm::vec2(0.65f, 0.5f);

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
void setup_dvd_texture(unsigned int texture_id);

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
    Shader dvd_loading_shader(
        RESOURCE_DIR "shaders/1-getting-started/dvd_loading.vert",
        RESOURCE_DIR "shaders/1-getting-started/dvd_loading.frag"
    );

    // activating our shader
    dvd_loading_shader.use();

    // setting up vao, vbo, ebo
    unsigned int vbo, ebo, vao;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    // bind vao to save configurations
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // setting up textures
    stbi_set_flip_vertically_on_load(true);

    // get and set texture uniforms
    unsigned int u_texture1 = glGetUniformLocation(dvd_loading_shader.shader_program, "texture1");
    glUniform1i(u_texture1, 0);

    // texture1
    unsigned int texture1;
    glGenTextures(1, &texture1);
    setup_dvd_texture(texture1);

    // get transformation uniforms
    unsigned int u_transform = glGetUniformLocation(dvd_loading_shader.shader_program, "transform");

    while (!glfwWindowShouldClose(window)) {
        // processing the last polled inputs/events
        process_input(window);

        // calculate time/frame
        float current_frame = (float)glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // position and physics logic
        pos += velocity * delta_time;

        if (pos.x + half_width >= 1.0f) {
            pos.x = 1.0f - half_width; // clamp
            velocity.x = -velocity.x;  // reverse velocity direction
        } else if (pos.x - half_width <= -1.0f) {
            pos.x = half_width - 1.0f; // clamp
            velocity.x = -velocity.x;  // reverse velocity direction
        }

        if (pos.y + half_height >= 1.0f) {
            pos.y = 1.0f - half_height;
            velocity.y = -velocity.y;
        } else if (pos.y - half_height <= -1.0f) {
            pos.y = half_height - 1.0f;
            velocity.y = -velocity.y;
        }

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0.0));
        transform = glm::scale(transform, glm::vec3(scale, scale, 1.0f));
        glUniformMatrix4fv(u_transform, 1, GL_FALSE, glm::value_ptr(transform));

        // render
        glClearColor(.0f, .0f, .0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // render the triangle
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // swap buffer (double bufffering)
        glfwSwapBuffers(window);

        // listen for events
        glfwPollEvents();
    }

    // cleaning up
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

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

void setup_dvd_texture(unsigned int texture_id) {
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    data = stbi_load(RESOURCE_DIR "textures/dvd.jpg", &texture_width, &texture_height, &nr_channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
