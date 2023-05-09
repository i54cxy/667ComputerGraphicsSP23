//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Module6_10.cpp
//	Purpose: OpenGL and GLUT program to draw a simple 3-D scene. It starts
//           with some simple object modeling and representation, adds camera
//           and projection controls, adds lighting and shading, then adds
//           texture mapping.
//
//============================================================================

#include <iostream>
#include <vector>

#include <GL/gl3w.h>
#include <GL/freeglut.h>
#include "geometry/geometry.h"
#include "shader_support/glsl_shader.h"
#include "scene/scene.h"

#include "lighting_shader_node.h"
#include "unitsquare.h"
#include "conic.h"
#include "meshteapot.h"
#include "surface_of_revolution.h"
#include "spheresection.h"
#include "torus.h"
#include "light_node.h"
#include "textured_unit_square.h"
#include "texured_sphere.h"
#include "textured_conic.h"
#include "texture_node.h"

#include "IL/il.h"

#include "Particle.hpp"
#include "SpringDamper.hpp"
#include "Cloth.hpp"
#include "ExtrudedTriangle.hpp"

// Root of the scene graph and scene state
SceneNode* SceneRoot;
SceneState MySceneState;

// Global camera node (so we can change view)
CameraNode* MyCamera;

Cloth* cloth;

int RenderWidth = 800;
int RenderHeight = 600;
bool wireframe = false;

// Simple logging function
void logmsg(const char* message, ...)
{
    // Open file if not already opened
    static FILE* lfile = NULL;
    if (lfile == NULL)
        lfile = fopen("Module6_10.log", "w");

    va_list arg;
    va_start(arg, message);
    vfprintf(lfile, message, arg);
    putc('\n', lfile);
    fflush(lfile);
    va_end(arg);
}

/**
 * Display callback. Clears the prior scene and draws a new one.
 */
void display() {
    // Clear the framebuffer and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    //auto lap = MyCamera->GetLookAtPt();
    //std::cout << "Lookat: " << lap.x <<", " << lap.y << ", " << lap.z << std::endl;
    //auto pos = MyCamera->GetPosition();
    //std::cout << "Pos: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;

    // Init scene state and draw the scene graph
    MySceneState.Init();
    SceneRoot->Draw(MySceneState);

    // Swap buffers
    glutSwapBuffers();
}

Vector3 windDirection = Vector3(0.0f, -1.0f, 0.0f);
float windVelocity = 0.0f;
/**
 * Use a timer method to try to do a consistent update rate or 72Hz.
 * Without using a timer, the speed of movement will depend on how fast
 * the program runs (fast movement on a fast PC and slow movement on a
 * slower PC)
 */
void updateScene(int value) {
    // update scene graph
    SceneRoot->Update(MySceneState);

    cloth->addForce(Vector3(0.0f, 0.0f, -9.8f)); // gravity
    cloth->computeAerodynamicForces(windDirection * windVelocity); //wind
    cloth->checkCollision(0.0f);

    glutPostRedisplay();
    glutTimerFunc(1 / MySceneState.FPS, updateScene, 0);
}

/**
 * Mouse callback (called when a mouse button state changes)
 */
void mouse(int button, int state, int x, int y) {
}

/**
 * Mouse motion callback (called when mouse button is depressed)
 */
void mouseMotion(int x, int y) {
}

/**
 * Keyboard callback.
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // Escape key
    case 27:
        exit(0);
        break;
        // Reset the view
    case 't':
        MyCamera->SetPosition(Point3(0.0f, -56.0f, 106.0f));
        MyCamera->SetLookAtPt(Point3(0.0f, 22.0f, 32.0f));
        MyCamera->SetViewUp(Vector3(0.0, 0.0, 1.0));
        glutPostRedisplay();
        break;
        // Roll the camera by 5 degrees (counter-closewise)
    case 'q':
        MyCamera->Roll(5);
        glutPostRedisplay();
        break;
        // Roll the camera by 5 degrees (clockwise)
    case 'e':
        MyCamera->Roll(-5);
        glutPostRedisplay();
        break;
        // Change the pitch of the camera by 5 degrees
    case 'r':
        MyCamera->Pitch(5);
        glutPostRedisplay();
        break;
        // Change the pitch of the camera by 5 degrees (clockwise)
    case 'f':
        MyCamera->Pitch(-5);
        glutPostRedisplay();
        break;
        // Change the heading of the camera by 5 degrees
    case 'z':
        MyCamera->Heading(5);
        glutPostRedisplay();
        break;
        // Change the heading of the camera by 5 degrees (clockwise)
    case 'c':
        MyCamera->Heading(-5);
        glutPostRedisplay();
        break;
        // Slide right
    case 'd':
        MyCamera->Slide(5.0f, 0.0f, 0.0f);
        glutPostRedisplay();
        break;
        // Slide left
    case 'a':
        MyCamera->Slide(-5.0f, 0.0f, 0.0f);
        glutPostRedisplay();
        break;
        // Slide up
    case 'R':
        MyCamera->Slide(0.0f, 5.0f, 0.0f);
        glutPostRedisplay();
        break;
        // Slide down
    case 'F':
        MyCamera->Slide(0.0f, -5.0f, 0.0f);
        glutPostRedisplay();
        break;
        // Move forward
    case 'w':
        MyCamera->Slide(0.0f, 0.0f, -5.0f);
        glutPostRedisplay();
        break;
        // Move backward
    case 's':
        MyCamera->Slide(0.0f, 0.0f, 5.0f);
        glutPostRedisplay();
        break;
    case 'g':
        // Toggle wireframe
        wireframe = !wireframe;
        glutPostRedisplay();
        break;

    case 'U':
        windVelocity += 5.0f;
        std::cout << "Current wind velocity: " << windVelocity << std::endl;
        glutPostRedisplay();
        break;
    case 'u':
        windVelocity -= 5.0f;
        std::cout << "Current wind velocity: " << windVelocity << std::endl;
        glutPostRedisplay();
        break;
    case 'I':
        cloth->increaseSpring(500.0f);
        glutPostRedisplay();
        break;
    case 'i':
        cloth->increaseSpring(-500.0f);
        glutPostRedisplay();
        break;
    case 'O':
        cloth->increaseDamper(2.0f);
        glutPostRedisplay();
        break;
    case 'o':
        cloth->increaseDamper(-2.0f);
        glutPostRedisplay();
        break;
    case 'P':
        cloth->increaseDrag(0.001f);
        glutPostRedisplay();
        break;
    case 'p':
        cloth->increaseDrag(-0.001f);
        glutPostRedisplay();
        break;
    case 'J':
        cloth->rotate(1.0f);
        glutPostRedisplay();
        break;
    case 'j':
        cloth->rotate(-1.0f);
        glutPostRedisplay();
        break;
    case 'K':
        cloth->move(Vector3(0.0f, 0.0f, 5.0f));
        glutPostRedisplay();
        break;
    case 'k':
        cloth->move(Vector3(0.0f, 0.0f, -5.0));
        glutPostRedisplay();
        break;
    case 'T':
        cloth->reset();
        glutPostRedisplay();
        break;

    default:
        break;
    }
}

/**
 * Reshape callback. Update projection to reflect new aspect ratio.
 * @param  width  Window width
 * @param  height Window height
 */
void reshape(int width, int height) {
    RenderWidth = width;
    RenderHeight = height;

    // Reset the viewport
    glViewport(0, 0, width, height);

    // Reset the perspective projection to reflect the change of aspect ratio 
    // Make sure we cast to float so we get a fractional aspect ratio.
    MyCamera->ChangeAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

/**
 * Construct room as a child of the specified node
 * @param  parent       Parent node
 * @param  unit_square  Geometry node to use
 * @return Returns a scene node that describes the room.
 */
SceneNode* ConstructRoom(UnitSquareSurface* unit_square) {
    // Contruct transform nodes for the walls. Perform rotations so the 
    // walls face inwards
    TransformNode* floor_transform = new TransformNode;
    floor_transform->Scale(200.0f, 200.0f, 1.0f);

    // Floor is orange for high contrast, mostly dull
    PresentationNode* floor_material = new PresentationNode(Color4(1.0f, 0.5f, 0.2f),
        Color4(1.0f, 0.5f, 0.2f), Color4(0.1f, 0.1f, 0.1f), Color4(0.0f, 0.0f, 0.0f), 5.0f);

    // Walls. We can group these all under a single presentation node.
    SceneNode* room = new SceneNode;

    // Add floor and ceiling to the parent. Use convenience method to add
    // presentation, then transform, then geometry.
    room->AddChild(floor_material);
    floor_material->AddChild(floor_transform);
    floor_transform->AddChild(unit_square);

    return room;
}

/**
 * Construct lighting for this scene. Note that it is hard coded
 * into the shader node for this exercise.
 * @param  lighting  Pointer to the lighting shader node.
 */
void ConstructLighting(LightingShaderNode* lighting) {
    // Set the global light ambient
    Color4 globalAmbient(0.4f, 0.4f, 0.4f, 1.0f);
    lighting->SetGlobalAmbient(globalAmbient);

    // Light 0 - a point light source located at the front of the room. Note the
    // w component is 1. This light is somewhat dim. No ambient - let the global
    // ambient control the ambient lighting
    HPoint3 position0(0.0f, -50.0f, 120.0f, 1.0f);
    Color4 ambient0(0.0f, 0.0f, 0.0f, 1.0f);
    Color4 diffuse0(0.5f, 0.5f, 0.5f, 1.0f);
    Color4 specular0(0.5f, 0.5f, 0.5f, 1.0f);
    lighting->AddChild(new LightNode(0, position0, ambient0, diffuse0, specular0));


    // Light 1 - a directional light from above. Note the w component is 0.
    // This light is somewhat bright. No ambient - let the global ambient 
    // control the ambient lighting
    HPoint3 position1(0.0f, 0.0f, 0.0f, 0.0f);
    Color4 ambient1(0.0f, 0.0f, 0.0f, 1.0f);
    Color4 diffuse1(0.7f, 0.7f, 0.7f, 1.0f);
    Color4 specular1(0.7f, 0.7f, 0.7f, 1.0f);
    lighting->AddChild(new LightNode(1, position1, ambient1, diffuse1, specular1));
}

/**
 * Construct the scene
 */
void ConstructScene() {
    // Shader node
    LightingShaderNode* shader = new LightingShaderNode;
    if (!shader->Create("vertex_lighting.vert", "vertex_lighting.frag") ||
        !shader->GetLocations())
        exit(-1);
    // Construct fixed scene lighting
    ConstructLighting(shader);

    // Get the position and normal locations to use when constructing VAOs
    int position_loc = shader->GetPositionLoc();
    int normal_loc = shader->GetNormalLoc();
    int tex_coord_loc = shader->GetTexCoordLoc();

    // Add the camera to the scene
    // Initialize the view and set a perspective projection
    MyCamera = new CameraNode();
    MyCamera->SetPosition(Point3(0.0f, -56.0f, 106.0f));
    MyCamera->SetLookAtPt(Point3(0.0f, 22.0f, 32.0f));
    MyCamera->SetViewUp(Vector3(0.0, 0.0, 1.0));
    MyCamera->SetPerspective(50.0, 1.0, 1.0, 1000.0);

    // Construct subdivided square - subdivided 10x in both x and y
    UnitSquareSurface* unit_square = new UnitSquareSurface(1, position_loc, normal_loc, tex_coord_loc);

    // Construct the room as a child of the root node
    SceneNode* room = ConstructRoom(unit_square);

    // Construct the scene layout
    SceneRoot = new SceneNode;
    SceneRoot->AddChild(shader);
    shader->AddChild(MyCamera);

    //// Add the room (walls, floor, ceiling)
    MyCamera->AddChild(room);

    TransformNode* cloth_transform = new TransformNode;
    MyCamera->AddChild(cloth_transform);
    PresentationNode* cloth_material = new PresentationNode(Color4(1.0f, 1.0f, 1.0f),
        Color4(1.0f, 1.0f, 1.0f), Color4(1.0f, 1.0f, 1.0f), Color4(0.0f, 0.0f, 0.0f), 5.0f);
    cloth_transform->AddChild(cloth_material);
    cloth = new Cloth(50, 60, 60, 43, 43);
    cloth_material->AddChild(cloth);

    //TransformNode* extruded_triangle_transform = new TransformNode;
    //MyCamera->AddChild(extruded_triangle_transform);
    //PresentationNode* extruded_triangle_material = new PresentationNode(Color4(1.0f, 1.0f, 1.0f),
    //    Color4(1.0f, 1.0f, 1.0f), Color4(1.0f, 1.0f, 1.0f), Color4(0.0f, 0.0f, 0.0f), 5.0f);
    //extruded_triangle_transform->AddChild(extruded_triangle_material);
    //ExtrudedTriangle* extruded_triangle = new ExtrudedTriangle(
    //    Point3(50.0f, 50.0f, 1.0f), Point3(60.0f, 60.0f, 1.0f), Point3(40.0f, 70.0f, 1.0f),
    //    Vector3(30.0f, 40.0f, 50.0f), position_loc, normal_loc, tex_coord_loc);
    //extruded_triangle_material->AddChild(extruded_triangle);
}

/**
 * Main
 */
int main(int argc, char** argv) {
    // Print the keyboard commands
    std::cout << "t - Reset to initial view" << std::endl;
    std::cout << "T - Reset to initial system" << std::endl;
    std::cout << "q - Roll    5 degrees clockwise   e - Counter-clockwise" << std::endl;
    std::cout << "r - Pitch   5 degrees clockwise   f - Counter-clockwise" << std::endl;
    std::cout << "z - Heading 5 degrees clockwise   c - Counter-clockwise" << std::endl;
    std::cout << "d - Slide camera right            a - Slide camera left" << std::endl;
    std::cout << "w - Move camera forward           s - Move camera backwards" << std::endl;
    std::cout << "R - Slide camera up               F - Slide camera down" << std::endl;
    std::cout << "U - Increase wind                 u - Decrease wind" << std::endl;
    std::cout << "I - Increase spring               i - Decrease spring" << std::endl;
    std::cout << "O - Increase damper               o - Decrease damper" << std::endl;
    std::cout << "P - Increase drag                 p - Decrease drag" << std::endl;
    std::cout << "J - Rotate cloth clockwise        j - Rotate cloth counter-clockwise" << std::endl;
    std::cout << "K - Transition cloth up           k - Transsition cloth down" << std::endl;
    std::cout << "g - Toggle wireframe mode" << std::endl;
    std::cout << "ESC - Exit Program" << std::endl;

    ilInit();

    // Initialize free GLUT
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    // Double buffer with depth buffer and MSAA
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Final Project by Kevin Chen");

    // Add GLUT callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    // Initialize Open 3.2 core profile
    if (gl3wInit()) {
        fprintf(stderr, "gl3wInit: failed to initialize OpenGL\n");
        return -1;
    }
    if (!gl3wIsSupported(3, 2)) {
        fprintf(stderr, "OpenGL 3.2 not supported\n");
        return -1;
    }
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Set the clear color to black. Any part of the window outside the
    // viewport should appear black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Construct scene.
    ConstructScene();

    // Enable multi-sample anti-aliasing
    glEnable(GL_MULTISAMPLE);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    glutTimerFunc(1000 / MySceneState.FPS, updateScene, 0);
    glutMainLoop();
    return 0;
}