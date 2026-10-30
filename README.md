
# OpenGL Mini Renderer

A lightweight OpenGL rendering project built with **C++20**, **OpenGL 3.3 Core Profile**, and **CMake**.

This project was developed as part of my C++ learning journey to explore graphics programming beyond high-level game engines such as Unity.

The goal is to understand how vertex data, shaders, textures, and transformation matrices work together in a graphics rendering pipeline.

## Features

- OpenGL 3.3 Core Profile initialization using GLFW and GLAD
- Triangle rendering with VAO and VBO
- Vertex attributes for positions and texture coordinates
- External GLSL vertex and fragment shaders
- Shader compilation, linking, and error reporting
- Uniform-based animated triangle colors
- Procedurally generated 2×2 RGBA checkerboard texture
- Texture sampling using `sampler2D`
- Animated rotation and scaling using GLM transformation matrices
- Texture and color rendering modes configured in C++
- Window resizing and ESC key handling
- Explicit cleanup of OpenGL resources
- Debug and Release build validation

## Technologies

| Technology | Purpose |
|---|---|
| C++20 | Application development |
| OpenGL 3.3 | Graphics rendering |
| GLFW 3.4 | Window creation, context management, and input |
| GLAD | OpenGL function loading |
| GLSL | Vertex and fragment shaders |
| GLM 1.0.3 | Vector and matrix mathematics |
| CMake | Build configuration |
| Ninja / Visual Studio 2022 | Build tools |
| Git / GitHub | Version control |

## Project Structure

```text
OpenGLMiniRenderer/
├── CMakeLists.txt
├── CMakePresets.json
├── README.md
├── docs/
│   └── development-log.md
├── external/
│   └── glad/
├── shaders/
│   ├── triangle.vert
│   └── triangle.frag
└── src/
    └── main.cpp
```

Generated build directories are excluded from version control.

## Getting Started

### Prerequisites

- Windows
- Visual Studio 2022 with C++ development tools
- CMake 3.20 or newer
- Ninja
- Git

The project uses CMake to manage its build configuration and dependencies.

### Configure

Open a terminal in the project root directory.

For the existing manual Debug build workflow:

```bash
cmake -S . -B build-manual -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

If the existing `build-manual` directory was configured with a different generator, continue using its original configuration instead of switching generators in the same directory.

### Build

Build the renderer using:

```bash
cmake --build build-manual --target opengl_mini_renderer
```

### Release Build

Use a separate build directory for Release:

```bash
cmake -S . -B build-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --target opengl_mini_renderer
```

### Run

Launch the generated executable from the appropriate build directory.

For example:

```powershell
.\build-manual\opengl_mini_renderer.exe
```

The application opens an OpenGL window and renders the configured scene.

Press **ESC** to close the application.

**Note:** The shader files must be accessible at the paths expected by the application. Run the executable with the appropriate working directory.

## Rendering Pipeline

The renderer follows this general process:

```text
C++ Application
       |
       v
GLFW Window & OpenGL Context
       |
       v
GLAD Function Loading
       |
       v
Vertex Data (VBO)
       |
       v
Vertex Attribute Configuration (VAO)
       |
       v
Vertex Shader
       |
       v
Primitive Assembly & Rasterization
       |
       v
Fragment Shader
       |
       v
Framebuffer
       |
       v
Display
```

### Vertex Data

The Vertex Buffer Object (VBO) stores vertex data in GPU-managed memory.

The Vertex Array Object (VAO) records the vertex attribute configuration, including how the vertex data should be interpreted.

### Shaders

The renderer uses custom GLSL shaders.

The vertex shader processes vertex attributes and can apply transformation matrices.

The fragment shader determines fragment colors and can sample texture data.

### Textures

Texture coordinates (UVs) specify which parts of a texture correspond to the vertices.

A `sampler2D` uniform allows the fragment shader to access the bound texture.

### Transformations

GLM is used to construct transformation matrices.

These matrices can be passed to the vertex shader through uniforms to modify vertex positions without recreating the vertex buffer every frame.

## Debugging and Validation

The project includes practice with OpenGL debugging and build validation.

### Build Validation

| Configuration | Result |
|---|---|
| Debug | Passed |
| Release | Passed |

Successful compilation and linking do not guarantee correct rendering, so runtime verification is also important.

### OpenGL Error Handling

The following functions were studied during development:

- `glGetError()` for detecting OpenGL API errors
- `glfwSetErrorCallback()` for GLFW error reporting
- `glfwGetError()` for retrieving GLFW errors
- `glGetShaderiv()` for shader compilation status
- `glGetShaderInfoLog()` for shader compilation diagnostics

A black screen does not necessarily indicate an OpenGL API error. Shader logic, vertex attributes, uniforms, transformations, and draw calls may also need to be inspected.

## Troubleshooting

### CMake Configuration Failure with GLM

During development, CMake Configure terminated unexpectedly with exit code:

```text
-1073740791 (0xC0000409)
```

Diagnostic messages were added to `CMakeLists.txt` using:

```cmake
message(STATUS "Checking GLM configuration...")
```

This helped narrow down the failure to the following stage:

```cmake
FetchContent_MakeAvailable(glm)
```

As a workaround, GLM 1.0.3 was downloaded locally using Git, and `FETCHCONTENT_SOURCE_DIR_GLM` was configured to point to the local source directory.

After applying the workaround, CMake configuration, compilation, and application execution completed successfully.

The exact underlying cause of the abnormal termination was not determined.

This experience demonstrated how incremental diagnostic messages can help isolate CMake configuration problems.

## What I Learned

### Understanding the Graphics Pipeline

Working directly with OpenGL helped me understand how a graphics application manages vertex buffers, shaders, textures, and rendering state.

### Connecting OpenGL with Unity

My previous experience with Unity involved higher-level systems for meshes, materials, shaders, and object transformations.

Building a renderer using OpenGL provided a clearer understanding of the lower-level operations behind these systems.

### GPU Resource Management

OpenGL objects such as vertex buffers, vertex arrays, textures, and shader programs require explicit resource management.

This reinforces important C++ concepts such as resource ownership, lifetime management, and RAII.

### Debugging and Build Systems

I practiced troubleshooting CMake dependency configuration, validating Debug and Release builds, and investigating graphics-related errors.

## Development Log

Development notes, debugging experiences, and weekly learning reflections are available in:

[Development Log](docs/development-log.md)

## Future Improvements

- Refactor shader compilation into a reusable Shader class
- Introduce RAII wrappers for OpenGL resources
- Add Camera, View, and Projection matrices
- Implement reusable Mesh and Texture classes
- Improve shader and asset path management
- Add automated build validation
- Explore framebuffer rendering and post-processing

## Project Status

**Week 12 — C++ Career Quest**

Core learning project completed, with further refactoring and graphics features planned as future improvements.

## References

- [LearnOpenGL](https://learnopengl.com/)
- [OpenGL Reference](https://docs.gl/)
- [GLFW Documentation](https://www.glfw.org/documentation.html)
- [GLM](https://github.com/g-truc/glm)
- [CMake Documentation](https://cmake.org/documentation/)
