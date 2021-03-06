// Copyright (c) 2015 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Draw a triangle. Created with the help of the tutorial on
// https://open.gl/drawing. Uses SDL and GLEW.
#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "common/error.h"
#include "common/io.h"
#include "common/other.h"


const float vertices[] = {
     0.0f,  0.5f,  1.0f, 0.0f, 0.0f, // vertex 1: Red
     0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // vertex 2: Green
    -0.5f, -0.5f,  0.0f, 0.0f, 1.0f  // vertex 3: Blue
};


/**
 * Return ID of the linked and activated shader.
 */
GLuint initShaders() {
    auto vshader = compileShader("vshader.glsl", ShaderType::vertex);
    auto fshader = compileShader("fshader.glsl", ShaderType::fragment);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vshader);
    glAttachShader(shaderProgram, fshader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    printGlErrors();
    return shaderProgram;
}

SDL_GLContext initContext(SDL_Window *window) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    printGlErrors();
    return context;
}

/**
 * Copy buffers to memory, set shader attributes, bind to VAO.
 * Return bound VAO ID.
 */
GLuint initBuffers(GLuint shaderProgram) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                          5*sizeof(float), 0);

    GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colorAttrib);
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE,
                          5*sizeof(float), (void*)(2*sizeof(float)));

    GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
    glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
    printGlErrors();
    return vao;
}

void paint() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    printGlErrors();
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Hello World",
                                           100, 100, 800, 600,
                                           SDL_WINDOW_OPENGL);
    auto context = initContext(window);
    initGlew();
    auto program = initShaders();
    initBuffers(program);

    paint();

    SDL_Event event;
    while (true) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) break;
            if (event.type == SDL_KEYDOWN) break;
        }
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
