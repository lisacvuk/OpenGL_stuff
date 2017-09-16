#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "util/GLShader.hpp"
float rel2abs(float absolute){
  std::cout << (600 / absolute)/100 << std::endl;
  return (600 / absolute) / 100;
}
float rel2abs2(float absolute){
  std::cout << (800 / absolute)/100 << std::endl;
  return (800 / absolute) / 100;
}
int main(int argc, char const *argv[]) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
    std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);

  SDL_GLContext glcontext = SDL_GL_CreateContext(window);
  glewInit();
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
  glm::mat4 Model = glm::mat4(1.0f);
  float points[] = {
    rel2abs(-2.5), rel2abs2(2.5),
    rel2abs(-2.5), rel2abs2(-2.5),
    rel2abs(2.5), rel2abs2(-2.5),
    rel2abs(-2.5), rel2abs2(2.5),
    rel2abs(2.5), rel2abs2(-2.5),
    rel2abs(2.5),  rel2abs2(2.5)
  };
  float points_uv[] = {
    -1.0f, 1.0f,
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f
  };

  GLuint tex;
  glGenTextures(1, &tex);

  glBindTexture(GL_TEXTURE_2D, tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //glGenerateMipmap(GL_TEXTURE_2D);

  float pixels[] = {
    0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
  };
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  GLuint uvbuffer;
  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points_uv), points_uv, GL_STATIC_DRAW);

  GLuint program = LoadShader("shader.vs", "shader.fs");
  GLuint MatrixID = glGetUniformLocation(program, "MVP");
  bool running = true;
  int currX = 0;
  int currY = 0;
  while (running){
    SDL_Event sdlEvent;
    while(SDL_PollEvent(&sdlEvent) != 0)
    {
        // Esc button is pressed
        if(sdlEvent.type == SDL_QUIT)
        {
            running = false;
        }
        if(sdlEvent.type == SDL_KEYDOWN)
          if(sdlEvent.key.keysym.sym == SDLK_LEFT){
            currX++;
          }
          if(sdlEvent.key.keysym.sym == SDLK_RIGHT){
            currX--;
          }
          if(sdlEvent.key.keysym.sym == SDLK_UP){
            currY--;
          }
          if(sdlEvent.key.keysym.sym == SDLK_DOWN){
            currY++;
          }
    }

    glm::mat4 View = glm::lookAt(
      glm::vec3(currX,currY,1), // Camera is at (4,3,3), in World Space
      glm::vec3(currX,currY,0), // and looks at the origin
      glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    glm::mat4 mvp = Projection * View * Model;

    // Set background color as cornflower blue
    glClearColor(0.39f, 0.58f, 0.93f, 1.f);
    // Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glUseProgram(program);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Update window with OpenGL rendering
    SDL_GL_SwapWindow(window);
  }
  return 0;
}
