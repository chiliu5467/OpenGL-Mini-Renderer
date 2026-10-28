#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

std::string ReadShaderFile(
    const std::filesystem::path& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "Failed to open shader file.\n";
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    if (file.bad())
    {
        std::cerr << "Failed to read shader file.\n";
        return "";
    }

    return buffer.str();
}

void framebuffer_size_callback(
    GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void buildVAOVBO(unsigned int& VAO, unsigned int& VBO)
{
    const float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GLuint CompileShader(
    GLenum shaderType,
    const std::string& source)
{
    GLuint shader = glCreateShader(shaderType);

    if (shader == 0)
    {
        std::cerr << "Failed to create shader.\n";
        return 0;
    }

    const char* shaderSource = source.c_str();

    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success != GL_TRUE)
    {
        char infoLog[1024] = {};

        glGetShaderInfoLog(
            shader, sizeof(infoLog), nullptr, infoLog
        );

        std::cerr << "Shader compilation failed:\n"
            << infoLog << '\n';

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint CreateShaderProgram(
    const std::string& vertexCode,
    const std::string& fragmentCode)
{
    // Step 1: Compile Vertex Shader
    GLuint vertexShader =
        CompileShader(GL_VERTEX_SHADER, vertexCode);

    if (vertexShader == 0)
    {
        return 0;
    }

    // Step 2: Compile Fragment Shader
    GLuint fragmentShader =
        CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return 0;
    }

    // Step 3: Create Shader Program
    GLuint shaderProgram = glCreateProgram();

    if (shaderProgram == 0)
    {
        std::cerr << "Failed to create shader program.\n";

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    // Step 4: Attach and Link
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    GLint success = GL_FALSE;
    glGetProgramiv(
        shaderProgram,
        GL_LINK_STATUS,
        &success
    );

    if (success != GL_TRUE)
    {
        char infoLog[1024] = {};

        glGetProgramInfoLog(
            shaderProgram,
            sizeof(infoLog),
            nullptr,
            infoLog
        );

        std::cerr << "Shader program linking failed:\n"
            << infoLog << '\n';

        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }

    // Step 5: Cleanup Individual Shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void RenderTriangle(
    GLuint shaderProgram,
    GLuint VAO,
    GLint colorLocation)
{
    // Clear Screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use Shader Program
    glUseProgram(shaderProgram);

    // Calculate Animated Color
    float timeValue =
        static_cast<float>(glfwGetTime());

    float greenValue =
        (std::sin(timeValue) + 1.0f) / 2.0f;

    // Update Uniform
    glUniform4f(
        colorLocation,
        0.0f,
        greenValue,
        0.0f,
        1.0f
    );

    // Draw Triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Cleanup(
    GLFWwindow* window,
    GLuint VAO,
    GLuint VBO,
    GLuint shaderProgram)
{
    // Delete OpenGL Resources
    if (shaderProgram != 0)
    {
        glDeleteProgram(shaderProgram);
    }

    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
    }

    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
    }

    // Destroy Window
    if (window != nullptr)
    {
        glfwDestroyWindow(window);
    }

    glfwTerminate();
}

int main()
{
    // Step 1: Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
    }

    // Step 2: Configure OpenGL Version & Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE);

    // Step 3: Create Window
    GLFWwindow* window = glfwCreateWindow(
        800, 600, "Hello Window", nullptr, nullptr
    );

    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
		return -1;
    }

    // Step 4: Make OpenGL Context Current
    glfwMakeContextCurrent(window);

    // Step 5: Initialize GLAD
    if (!gladLoadGLLoader(
        (GLADloadproc)glfwGetProcAddress))
    {
		std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Step 6: Set Viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Step 7: Build VAO, VBO, and Shader Program
    unsigned int VAO = 0;
    unsigned int VBO = 0;

    buildVAOVBO(VAO, VBO);

    // Step 8: Read Shader Files
    std::filesystem::path shaderDir =
        std::filesystem::u8path(SHADER_DIR);

    std::string vertexCode =
        ReadShaderFile(shaderDir / "triangle.vert");

    std::string fragmentCode =
        ReadShaderFile(shaderDir / "triangle.frag");

    if (vertexCode.empty() || fragmentCode.empty())
    {
        std::cerr << "Failed to load shader source.\n";
        Cleanup(window, VAO, VBO, 0);
        return -1;
    }

    // Step 9: Create Shader Program
    GLuint shaderProgram =
        CreateShaderProgram(vertexCode, fragmentCode);

    if (shaderProgram == 0)
    {
        std::cerr << "Failed to create shader program\n";
        Cleanup(window, VAO, VBO, 0);
        return -1;
    }

    // Step 10: Get Uniform Location
    GLint colorLocation =
        glGetUniformLocation(shaderProgram, "ourColor");

    if (colorLocation == -1)
    {
        std::cerr << "Uniform ourColor not found\n";
        Cleanup(window, VAO, VBO, shaderProgram);
        return -1;
    }

    // Step 11: Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // Process Input
        processInput(window);

        RenderTriangle(shaderProgram, VAO, colorLocation);

        // Swap Buffers
        glfwSwapBuffers(window);

        // Poll Events
        glfwPollEvents();
    }

    // Step 12: Cleanup
    Cleanup(window, VAO, VBO, shaderProgram);

    return 0;
}