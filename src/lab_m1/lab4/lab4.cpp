#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 * To find out more about `FrameStart`, `Update`, `FrameEnd`
 * and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
}

void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Lab4::RenderScene() {
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    // --- Randarea principală ---
    RenderScene();
    DrawCoordinateSystem();


    // --- Randarea în viewport-ul mic ---

    // Golește buffer-ul de adâncime (depth buffer) pentru a nu
    // se suprapune testele de adâncime din cele două randări.
    glClear(GL_DEPTH_BUFFER_BIT);

    // Setează noul viewport
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    // TODO(student): render the scene again, in the new viewport
    // ✅ Randăm scena (cuburile)
    RenderScene();
    // ✅ Randăm axele de coordonate
    DrawCoordinateSystem();
}

void Lab4::FrameEnd()
{
}


/*
 * These are callback functions. To find more about callbacks and
 * how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // --- Logica de transformare adăugată anterior (rămâne neschimbată) ---
    float translationSpeed = 2.0f;
    float scaleSpeed = 1.0f;
    float rotationSpeed = glm::radians(60.0f);

    // Cubul 1: Translație
    if (window->KeyHold(GLFW_KEY_A)) translateX -= translationSpeed * deltaTime;
    if (window->KeyHold(GLFW_KEY_D)) translateX += translationSpeed * deltaTime;
    if (window->KeyHold(GLFW_KEY_W)) translateZ -= translationSpeed * deltaTime;
    if (window->KeyHold(GLFW_KEY_S)) translateZ += translationSpeed * deltaTime;
    if (window->KeyHold(GLFW_KEY_R)) translateY += translationSpeed * deltaTime;
    if (window->KeyHold(GLFW_KEY_F)) translateY -= translationSpeed * deltaTime;

    // Cubul 2: Scalare
    if (window->KeyHold(GLFW_KEY_1)) {
        float deltaScale = scaleSpeed * deltaTime;
        scaleX -= deltaScale;
        scaleY -= deltaScale;
        scaleZ -= deltaScale;
    }
    if (window->KeyHold(GLFW_KEY_2)) {
        float deltaScale = scaleSpeed * deltaTime;
        scaleX += deltaScale;
        scaleY += deltaScale;
        scaleZ += deltaScale;
    }

    // Cubul 3: Rotație
    if (window->KeyHold(GLFW_KEY_3)) angularStepOX += rotationSpeed * deltaTime;
    if (window->KeyHold(GLFW_KEY_4)) angularStepOX -= rotationSpeed * deltaTime;
    if (window->KeyHold(GLFW_KEY_5)) angularStepOY += rotationSpeed * deltaTime;
    if (window->KeyHold(GLFW_KEY_6)) angularStepOY -= rotationSpeed * deltaTime;
    if (window->KeyHold(GLFW_KEY_7)) angularStepOZ += rotationSpeed * deltaTime;
    if (window->KeyHold(GLFW_KEY_8)) angularStepOZ -= rotationSpeed * deltaTime;
}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }

    // TODO(student): Add viewport movement and scaling logic
    // ✅ Logica pentru viewport

    int moveStep = 10;   // Pixeli de mișcare
    int scaleStep = 10;  // Pixeli de scalare

    // Mișcare viewport (I, J, K, L)
    if (key == GLFW_KEY_I) {
        // Sus (crește Y)
        miniViewportArea.y += moveStep;
    }
    if (key == GLFW_KEY_K) {
        // Jos (scade Y)
        miniViewportArea.y -= moveStep;
    }
    if (key == GLFW_KEY_J) {
        // Stânga (scade X)
        miniViewportArea.x -= moveStep;
    }
    if (key == GLFW_KEY_L) {
        // Dreapta (crește X)
        miniViewportArea.x += moveStep;
    }

    // Scalare viewport (U, O)
    if (key == GLFW_KEY_U) {
        // Micșorare (Scale Out)
        miniViewportArea.width -= scaleStep;
        miniViewportArea.height -= scaleStep;
    }
    if (key == GLFW_KEY_O) {
        // Mărire (Scale In)
        miniViewportArea.width += scaleStep;
        miniViewportArea.height += scaleStep;
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}