#include <cmath>
// This code only shows how to load an object. Input this into your
// program to also draw it.

#include <iostream>
#include <GL/glew.h>
#include <cmath>

#include <GLFW/glfw3.h>
#include <cstdlib>  
#include "readfile.hpp"
#include "lodepng.h"

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::value_ptr

#define TINYOBJLOADER_IMPLEMENTATION 


float g_rotation[2];
int width = 800, height = 600;
glm::mat4 model = glm::mat4(1.0f);
glm::vec4 normals[60];

void checkShaderCompileError(GLint shaderID)
{
    GLint isCompiled = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::string errorLog;
        errorLog.resize(maxLength);
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

        std::cout << "shader compilation failed:" << std::endl;
        std::cout << errorLog << std::endl;
        return;
    }
    else
        std::cout << "shader compilation success." << std::endl;

    return;
}


static void error_callback(int error, const char* description)
{
    std::cerr << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //-------------------------------------------------------------------------//
    // COPY YOUR CODE FROM 1.6 HERE
    // Update rotation angle here, for example
    //-------------------------------------------------------------------------//
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
    {
        //implement reloading of the shaders on the fly
        std::string vertex_shader_str = readFile("../lab2-1_vs.glsl");
        std::string fragment_shader_str = readFile("../lab2-1_fs.glsl");

        const char* vertex_shader_src = vertex_shader_str.c_str();
        const char* fragment_shader_src = fragment_shader_str.c_str();

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertex_shader_src, NULL);
        glCompileShader(vs);
        checkShaderCompileError(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragment_shader_src, NULL);
        glCompileShader(fs);
        checkShaderCompileError(fs);

        GLuint shader_program = glCreateProgram();
        glAttachShader(shader_program, fs);
        glAttachShader(shader_program, vs);
        glLinkProgram(shader_program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        glUseProgram(shader_program);
    }



    if ((key == GLFW_KEY_RIGHT) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
        g_rotation[0] += 10.0f;

    }
    if ((key == GLFW_KEY_LEFT) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
        g_rotation[0] -= 10.0f;

    }
    if ((key == GLFW_KEY_UP) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
        g_rotation[1] += 10.0f;

    }
    if ((key == GLFW_KEY_DOWN) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
        g_rotation[1] -= 10.0f;

    }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


glm::vec4 crossVec4(glm::vec4 _v1, glm::vec4 _v2) {
    glm::vec3 vec1 = glm::vec3(_v1[0], _v1[1], _v1[2]);
    glm::vec3 vec2 = glm::vec3(_v2[0], _v2[1], _v2[2]);
    glm::vec3 res = cross(vec1, vec2);
    return glm::vec4(res[0], res[1], res[2], 0);
}

glm::vec4 subtractV4(glm::vec4 _v1, glm::vec4 _v2) {
    glm::vec4 res;
    for (int c = 0; c < 3; c++) {
        res[c] = _v1[c] - _v2[c];
    }
    return res;
}

int main(int argc, char const *argv[])
{

    // start GL context and O/S window using the GLFW helper library

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);


    int w_height = 600;
    int w_width = 800;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Hello Icosahedron", NULL, NULL);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"


  float t = (1.0f + sqrtf(5.0f))*0.25f;
  int num_faces = 20;
  int num_verts = 60;

  float points[] = {
    // An icosahedron without shared vertices has 20*3 vertices
    -0.5,  t,  0, 
    -0.5,  t,  0, 
    -0.5,  t,  0,
    -0.5,  t,  0,
    -0.5,  t,  0,
    
    0.5,  t,  0,
    0.5,  t,  0,
    0.5,  t,  0, 
    0.5,  t,  0, 
    0.5,  t,  0,
    
    -0.5, -t,  0, 
    -0.5, -t,  0, 
    -0.5, -t,  0,
    -0.5, -t,  0,
    -0.5, -t,  0,
    
    0.5, -t,  0,  
    0.5, -t,  0,  
    0.5, -t,  0, 
    0.5, -t,  0, 
    0.5, -t,  0,
    
    0, -0.5,  t,  
    0, -0.5,  t,  
    0, -0.5,  t, 
    0, -0.5,  t, 
    0, -0.5,  t,
    
    0,  0.5,  t,  
    0,  0.5,  t,  
    0,  0.5,  t, 
    0,  0.5,  t, 
    0,  0.5,  t,
    
    0, -0.5, -t,  
    0, -0.5, -t,  
    0, -0.5, -t, 
    0, -0.5, -t, 
    0, -0.5, -t,
    
    0,  0.5, -t,  
    0,  0.5, -t,  
    0,  0.5, -t, 
    0,  0.5, -t, 
    0,  0.5, -t,
    
    t,  0, -0.5,  
    t,  0, -0.5,  
    t,  0, -0.5, 
    t,  0, -0.5, 
    t,  0, -0.5,
    
    t,  0,  0.5,  
    t,  0,  0.5,  
    t,  0,  0.5, 
    t,  0,  0.5, 
    t,  0,  0.5,
    
    -t,  0, -0.5, 
    -t,  0, -0.5, 
    -t,  0, -0.5,
    -t,  0, -0.5,
    -t,  0, -0.5,
    
    -t,  0,  0.5, 
    -t,  0,  0.5, 
    -t,  0,  0.5,
    -t,  0,  0.5,
    -t,  0,  0.5
  };

  float uv_coords[]=
    {  
      0.072405, 0.144686,
      0.27133,  0.10033,
      0.481007, 0.129901,
      0.681948, 0.115116,
      0.928587, 0.107723,
      0.29888,  0.70651,
      0.449421, 0.736084,
      0.108695, 0.129901,
      0.847270, 0.699121,
      0.868776, 0.728691,
      0.576437, 0.721299,
      0.319044, 0.750869,
      0.548883, 0.736084,
      0.388265, 0.115116,
      0.397674, 0.713906,
      0.149690, 0.122508,
      0.347942, 0.115116,
      0.583157, 0.107723,
      0.780738, 0.100331,
      0.229663, 0.107723,
      0.33436,  0.68882,   
      0.181276, 0.721299,
      0.379528, 0.721299,
      0.190684, 0.122508,
      0.358023, 0.736084,
      0.103319, 0.713906,
      0.24512,  0.70651,
      0.083157, 0.699121,
      0.31006,  0.10251,
      0.222942, 0.721299,
      0.772001, 0.728691,
      0.618776, 0.721299,
      0.745120, 0.721299,
      0.433292, 0.115116,
      0.598614, 0.713906,
      0.520657, 0.721299,
      0.650362, 0.736084,
      0.700765, 0.728691,
      0.878184, 0.107723,
      0.671867, 0.743476,
      0.914475, 0.743476,
      0.819717, 0.736084, 
      0.201437, 0.721299,
      0.633561, 0.122508,
      0.796195, 0.743476,
      0.140953, 0.728691,
      0.122136, 0.706514,
      0.262593, 0.721299,
      0.157754, 0.728691,
      0.825765, 0.115116,
      0.723614, 0.728691,
      0.897001, 0.721299,
      0.499152, 0.736084,
      0.731007, 0.115116,
      0.469582, 0.721299,
      0.045523, 0.691729,
      0.967566, 0.721299,
      0.28341,  0.67157,
      0.532754, 0.122508,
      0.418507, 0.706514,
    };

  int num_indices = num_faces*3;
  unsigned short faces[]= {
    // ... and 20 triangular faces, 
    // defined by the following vertex indices:
    0,  55, 25,
    1,  26,  5,
    2,  6, 35,
    3,  36, 50,
    4,  51, 56,
    7,  27, 45,
    28, 57, 20,
    58, 52, 10,
    53, 37, 30,
    38,  8, 40,    
    15, 46, 21,
    16, 22, 11,
    17, 12, 31,
    18, 32, 41,
    19, 42, 47,
    23, 48, 29,
    13, 24, 59,
    33, 14, 54,
    43, 34, 39,
    49, 44, 9
  };

  for (int crosses = 0; crosses < 60; crosses += 3) {
      glm::vec4 _vec1 = glm::vec4(points[faces[crosses] * 3], points[faces[crosses] * 3 + 1], points[faces[crosses] * 3 + 2], 0);
      glm::vec4 _vec2 = glm::vec4(points[faces[crosses + 1] * 3], points[faces[crosses + 1] * 3 + 1], points[faces[crosses + 1] * 3 + 2], 0);
      glm::vec4 _vec3 = glm::vec4(points[faces[crosses + 2] * 3], points[faces[crosses + 2] * 3 + 1], points[faces[crosses + 2] * 3 + 2], 0);

      glm::vec4 cross = crossVec4(_vec2 - _vec1, _vec3 - _vec1);

      normals[faces[crosses]] = cross;
      normals[faces[crosses + 1]] = cross;
      normals[faces[crosses + 2]] = cross;

      //printf("%f %f %f %f \n %f %f %f %f \n ---------\n", cube_vertices[cube_elements[crosses] * 4], cube_vertices[cube_elements[crosses] * 4 + 1], cube_vertices[cube_elements[crosses] * 4 + 2], 0,
          //cube_vertices[cube_elements[crosses + 1] * 4], cube_vertices[cube_elements[crosses + 1] * 4 + 1], cube_vertices[cube_elements[crosses + 1] * 4 + 2], 0);
  }
  for (int c = 0; c < 60; c++) {
      printf("%f %f %f %f\n", normals[c].x, normals[c].y, normals[c].z, normals[c].w);
  }

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);


  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


  unsigned int VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  unsigned int VBO3;
  glGenBuffers(1, &VBO3);
  glBindBuffer(GL_ARRAY_BUFFER, VBO3);
  glBufferData(GL_ARRAY_BUFFER, sizeof(uv_coords), uv_coords, GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);


  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

  //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  // load and compile shaders  "../lab1-7_vs.glsl" and "../lab1-7_fs.glsl"
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  std::string vertex_shader_str = readFile("../lab2-1_vs.glsl");
  std::string fragment_shader_str = readFile("../lab2-1_fs.glsl");

  const char* vertex_shader_src = vertex_shader_str.c_str();
  const char* fragment_shader_src = fragment_shader_str.c_str();

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader_src, NULL);
  glCompileShader(vs);
  checkShaderCompileError(vs);

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader_src, NULL);
  glCompileShader(fs);
  checkShaderCompileError(fs);

  GLuint shader_program = glCreateProgram();

  glAttachShader(shader_program, fs);
  glAttachShader(shader_program, vs);
  glLinkProgram(shader_program);
  glDeleteShader(vs);
  glDeleteShader(fs);
  glUseProgram(shader_program);

  glUseProgram(shader_program);

  GLuint texture_handle;
  glGenTextures(1, &texture_handle);
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(shader_program, "tex_sampler"), 0);
  glBindTexture(GL_TEXTURE_2D, texture_handle);
 
  unsigned char* image_data;
  unsigned image_w;
  unsigned image_h;
  unsigned err_code =
      lodepng_decode32_file(&image_data, &image_w, &image_h,
          "../../common/data/numberline_hires.png");
  if (!err_code)
      std::cout << "Read " << image_h << " x " << image_w << " image\n";

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  const float n = 1.0f;
  const float f = 100.0f;

  while (!glfwWindowShouldClose(window))
  {

      glfwGetFramebufferSize(window, &w_width, &w_height);

      glm::mat4 view = glm::mat4(1.0f);
      view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

      glm::mat4 projection;
      projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 100.0f);

      glm::mat4 model = glm::mat4(1.0f);
      model = glm::rotate(model, glm::radians(g_rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::rotate(model, glm::radians(g_rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));

      glm::mat4 mvp = projection * view * model;
      glUniformMatrix4fv(glGetUniformLocation(shader_program, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

      // update other events like input handling 
      glfwPollEvents();

      // clear the drawing surface
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      //-----------------------------------------------------------------------//
      // Issue an appropriate glDraw*() command.
      //-----------------------------------------------------------------------//
      glDrawElements(GL_TRIANGLES, sizeof(faces) / sizeof(unsigned short), GL_UNSIGNED_SHORT, 0);

      glfwSwapBuffers(window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

