#include "lab_m1/lab2/lab2.h"

#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;
using namespace m1;


Lab2::Lab2()
{
}


Lab2::~Lab2()
{
}


void Lab2::Init()
{
    // Asa nu arata fetele cubului din spate
    cullFace = GL_BACK;
    polygonMode = GL_FILL;

    {
        // Load box mesh
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        // creare cub manual
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3( 1, -1,  1), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(0, 0, 1)),
            VertexFormat(glm::vec3( 1,  1,  1), glm::vec3(1, 1, 0)),
            VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 0, 1)),
            VertexFormat(glm::vec3( 1, -1, -1), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3( 1,  1, -1), glm::vec3(0.5, 0.5, 0.5))
        };

        vector<unsigned int> indices =
        {
            // Ordinea in care se conecteaza varfurile
            0, 1, 2, 1, 3, 2, // fata din fata
            2, 3, 7, 2, 7, 6, // fata de sus
            1, 7, 3, 1, 5, 7, // fata din dreapta
            6, 7, 5, 6, 5, 4, // fata din spate
            0, 4, 1, 1, 4, 5, // fata din stanga
            2, 6, 4, 2, 4, 0  // fata de jos
        };

        // Functie de creare manuala a cubului folosind vertices si indices
        meshes["cube_A"] = new Mesh("generated cube 1");
        meshes["cube_A"]->InitFromData(vertices, indices);

        // Metoda 2 cu functia ajutatoare
        CreateMesh("cube_B", vertices, indices);
    }

    {
        // Acelasi lucru dar la tetraedru
        vector<VertexFormat> vertices_tetra
        {
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(-1, -1, 1), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(1, -1, 1), glm::vec3(0, 0, 1)),
            VertexFormat(glm::vec3(0, -1, -1), glm::vec3(1, 1, 0)),
        };

        vector<unsigned int> indices_tetra =
        {
            0, 1, 2,
            0, 2, 3,
            0, 3, 1,
            1, 3, 2
        };

        CreateMesh("tetrahedron", vertices_tetra, indices_tetra);
    }

    {
        vector<VertexFormat> vertices_square
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(1, 1, 0), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(1, 0, 0))
        };

        vector<unsigned int> indices_square =
        {
            0, 1, 2,
            0, 3, 2
        };

        CreateMesh("square", vertices_square, indices_square);
    }
}


void Lab2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices)
{
    // Creare de VAO
    // Setarile obiectului
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Creare VBO
    // Memoria de pe GPU unde stochez varfurile
    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Creare IBO
    // Indicii care definesc triunghiurile
    unsigned int IBO = 0;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    
    glBindVertexArray(0);

    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}


void Lab2::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab2::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    glEnable(GL_CULL_FACE);
    glCullFace(cullFace);

    RenderMesh(meshes["box"], shaders["VertexNormal"], glm::vec3(0, 0.5f, -1.5f), glm::vec3(0.75f));
    RenderMesh(meshes["cube_A"], shaders["VertexColor"], glm::vec3(-1.5f, 0.5f, 0), glm::vec3(0.25f));
    RenderMesh(meshes["cube_B"], shaders["VertexColor"], glm::vec3(1.5f, 0.5f, 0), glm::vec3(0.5f));
    RenderMesh(meshes["tetrahedron"], shaders["VertexColor"], glm::vec3(0, 0.4f, 0), glm::vec3(0.4f));
    RenderMesh(meshes["square"], shaders["VertexColor"], glm::vec3(-1.5f, 2.0f, 0), glm::vec3(1.0f));

    glDisable(GL_CULL_FACE);
}


void Lab2::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab2::OnInputUpdate(float deltaTime, int mods)
{
}


void Lab2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_F2)
    {
        if (cullFace == GL_BACK) {
            cullFace = GL_FRONT;
            cout << "Culling mode: GL_FRONT" << endl;
        } else {
            cullFace = GL_BACK;
            cout << "Culling mode: GL_BACK" << endl;
        }
    }

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
}


void Lab2::OnKeyRelease(int key, int mods)
{
}


void Lab2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void Lab2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Lab2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Lab2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab2::OnWindowResize(int width, int height)
{
}