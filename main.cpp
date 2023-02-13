#include <iostream>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <glm/glm.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}



int* pallette(int n) {
    int* color = new int[3];
    color[0] = 0;
    color[1] = (n % 5) * 50;
    color[2] = 0;
    return color;
}

double scale = 2.0f;
double centerx = 0.75f;
double centery = 0.0f;
double iter = 250;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W))
        centery -= scale * 0.01;
    if (glfwGetKey(window, GLFW_KEY_S))
        centery += scale * 0.01;
    if (glfwGetKey(window, GLFW_KEY_D))
        centerx -= scale * 0.01;
    if (glfwGetKey(window, GLFW_KEY_A))
        centerx += scale * 0.01;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP))
        iter *= 1.1;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN))
        iter *= 1/1.1f;
    if (glfwGetKey(window, GLFW_KEY_I))
        std::cout << centerx << ", " << centery << " @" << scale <<  std::endl;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    centerx =  (xpos / (double)width) * scale;
    centery =  (ypos / (double)height) * scale;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset == 1)
        scale *= 0.9;
    else
        scale *= 1.0 / 0.9;
}

GLFWwindow* glInit() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Window", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(-1);
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    return window;
}

float map (float s, float a1, float a2, float b1, float b2) {
    return b1 + (s - a1)*(b2 - b1) / (a2 - a1);
}

int main() {
    GLFWwindow* window = glInit();
    Shader ourShader(SOURCE_PATH"/shaders/shader.vs", SOURCE_PATH"/shaders/shader.fs");

//    unsigned int test = ourShader.generateTexture(PATH "/test.jpg");


//    unsigned int fracTexture;
//    int width, height;
//    //glfwGetFramebufferSize(window, &width, &height);
//    width = 1920;
//    height = 1080;
//    glGenTextures(1, &fracTexture);
//    glBindTexture(GL_TEXTURE_2D, fracTexture);
//
//    unsigned char* fracPixels = new unsigned char[width * height * 3];
//    for (int i = 0; i < width; i++){
//        for (int j = 0; j < height; j++) {
//            float x0 = map(i, 0, width - 1, -2.5, 1);
//            float y0 = map(j, 0, height - 1, -1, 1);
//            float x = 0.0f;
//            float y = 0.0f;
//            int iteration = 0;
//            int max_iteration = 100;
//            while (x*x + y*y <= 2*2 && iteration < max_iteration) {
//                float xtemp = x*x - y*y + x0;
//                y = 2*x*y + y0;
//                x = xtemp;
//                iteration++;
//            }
//            int* color = new int[3];
//            for (int k = 0; k < 3; k++)
//                color[k] = 0;
//            if (iteration <= max_iteration - 1){
//                color = pallette(iteration);
//            }
//            fracPixels[(j*width+i)*3] = color[0];
//            fracPixels[(j*width+i)*3 + 1] = color[1];
//            fracPixels[(j*width+i)*3 + 2] = color[2];
//        }
//    }
//    stbi_write_jpg(PATH "/meme2.jpg", width, height, 3, fracPixels, 90);

    unsigned int paletteTex;
    glGenTextures(1, &paletteTex);
    glBindTexture(GL_TEXTURE_1D, paletteTex);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(SOURCE_PATH"/pal.png", &width, &height, &nrChannels, 0);
    if (data)
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, width, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else
        std::cout << "Failed to load texture :)" << std::endl;
    stbi_image_free(data);
    float vertices[] = {
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,        // top right
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,      // bottom right
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,      // top left
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,       // top left
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,    // bottom left
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f
    };
//    float vertices[] = {
//            0.0f, 0.5f, 0.0f, 1.0f, 0.5f,
//            0.5f, -0.5f, 0.0f, -0.5f, 0.5f,
//            -0.5f, -0.5f, 0.0f, -0.5f, -0.5f
//    };
    //Create VBO2jj
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //Create VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // define vertex array data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glfwGetFramebufferSize(window, &width, &height);
    ourShader.use();
    glUniform2f(glGetUniformLocation(ourShader.ID, "center"), centerx, centery);
    ourShader.setDouble("scale", scale);
//    ourShader.setFloat("eps", 0.00001f);
    ourShader.setInt("iter", iter);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glUniform2d(glGetUniformLocation(ourShader.ID, "center"), centerx, centery);
        ourShader.setInt("iter", iter);
        ourShader.setDouble("scale", scale);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();

        glBindVertexArray(VAO);



        glBindTexture(GL_TEXTURE_1D, paletteTex);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}