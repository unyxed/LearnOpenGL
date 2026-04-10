#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <ios>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
  public:
    unsigned int shader_program;

    // Constructor
    Shader(const char* vertex_shader_file_path, const char* fragment_shader_file_path) {

      // vertex/fragment shader source code (read from file)
      std::string   vertex_shader_code;
      std::string   fragment_shader_code;
      std::ifstream vertex_shader_ifs;
      std::ifstream fragment_shader_ifs;

      vertex_shader_ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      fragment_shader_ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

      try {
        vertex_shader_ifs.open(vertex_shader_file_path, std::ios_base::in);
        fragment_shader_ifs.open(fragment_shader_file_path, std::ios_base::in);

        std::stringstream vertex_shader_sstream, fragment_shader_sstream;
        vertex_shader_sstream << vertex_shader_ifs.rdbuf();
        fragment_shader_sstream << fragment_shader_ifs.rdbuf();

        vertex_shader_code   = vertex_shader_sstream.str();
        fragment_shader_code = fragment_shader_sstream.str(); 

        std::cout << "SUCCESS::SHADER::FILE_READ_SUCCESSFULLY" << std::endl;

      } catch (std::ifstream::failure& exception) {
        std::cerr << "ERROR::SHADER::FILE_NOT_READ_SUCCESSFULLY " << exception.what() << std::endl;
      }

      const char* vertex_shader_csource_code   = vertex_shader_code.c_str();
      const char* fragment_shader_csource_code = fragment_shader_code.c_str(); 

      unsigned int vertex_shader, fragment_shader;

      // vertex shader 
      vertex_shader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex_shader, 1, &vertex_shader_csource_code, NULL);
      glCompileShader(vertex_shader); 
      checkCompileErrors(vertex_shader, "VERTEX");

      // fragment shader
      fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment_shader, 1, &fragment_shader_csource_code, NULL);
      glCompileShader(fragment_shader);
      checkCompileErrors(fragment_shader, "FRAGMENT");

      // shader program
      shader_program = glCreateProgram();
      glAttachShader(shader_program, vertex_shader);
      glAttachShader(shader_program, fragment_shader);
      glLinkProgram(shader_program);
      checkCompileErrors(shader_program, "PROGRAM");

      // delete vertex and fragment shader 
      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
    }

    void use() {
      glUseProgram(shader_program);
    }

  private:
    void checkCompileErrors(unsigned int shader_or_program, std::string type) {
      int success {0};
      char infoLog[1024];

      if (type != "PROGRAM") {
        glGetShaderiv(shader_or_program, GL_COMPILE_STATUS, &success);
        if (!success) {
          glGetShaderInfoLog(shader_or_program, 1024, NULL, infoLog);
          std::cerr << "ERROR::SHADER_COMPILATION_ERROR - Type: " << type << "\n" << infoLog << std::endl;
        } else {
          std::cout << "SUCCESS::SHADER::COMPILED" << std::endl;
        }
      } else {
        glGetProgramiv(shader_or_program, GL_LINK_STATUS, &success);
        if (!success) {
          glGetProgramInfoLog(shader_or_program, 1024, NULL, infoLog);
          std::cerr << "ERROR::PROGRAM::LINKING_ERROR - Type: " << type << "\n" << infoLog << std::endl;
        } else {
          std::cout << "SUCCESS::PROGRAM::LINKED" << std::endl;
        }
      }
    }
};
#endif
