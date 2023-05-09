//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:     scene_state.h
//	Purpose:	Class used to propogate state during traversal of the scene graph.
//
//============================================================================

#ifndef __SCENESTATE_H
#define __SCENESTATE_H

#include <list>

// Simple structure to hold light uniform locations
struct LightUniforms {
  GLint enabled;
  GLint spotlight;
  GLint position;
  GLint ambient;
  GLint diffuse;
  GLint specular;
  GLint spotDirection;
  GLint spotCutoff;
  GLint spotExponent;
};

/**
 * Scene state structure. Used to store OpenGL state - shader locations, 
 * matrices, etc.
 */
struct SceneState {
  // Vertex attribute locations
  GLint position_loc;       // Vertex position attribute location
  GLint vertexcolor_loc;    // Vertex color attribute location
  GLint use_texture_loc;
  GLint tex_coord_loc;     // texture coordinate attribute location
  GLint texture_loc;
  GLint normal_loc;        // Vertex normal

  // Uniform locations
  GLint orthomatrix_loc;    // Orthographic projection location (2-D)
  GLint color_loc;          // Constant color
  GLint pvm_loc;            // Composite project, view, model matrix location
  GLint modelmatrix_loc;    // Model matrix location
  GLint normalmatrix_loc;   // Normal matrix location
  GLint cameraposition_loc; // Camera position loc

  // Material uniform locations
  GLint materialambient_loc;   // Material ambient reflection location
  GLint materialdiffuse_loc;   // Material diffuse reflection location
  GLint materialspecular_loc;  // Material specular reflection location
  GLint materialemission_loc;  // Material emission location
  GLint materialshininess_loc; // Material shininess location

  // Lights
  LightUniforms lights[2];

  // Current matrices
  float ortho[16];          // Orthographic projection matrix (2-D)
  Matrix4x4 ortho_matrix;   // Orthographic projection matrix (2-D)
  Matrix4x4 pv;             // Current composite projection and view matrix
  Matrix4x4 model_matrix;   // Current model matrix

  int useTexture = 0;

  const float FPS = 72.0f;

  // Retained state to push/pop modeling matrix
  std::list<Matrix4x4> modelmatrix_stack;

  /**
  * Initialize scene state prior to drawing.
  */
  void Init() {
    model_matrix.SetIdentity();
    modelmatrix_stack.clear();
  }

  /**
  * Copy current matrix onto stack
  */
  void PushTransforms() {
    modelmatrix_stack.push_back(model_matrix);
  }

  /**
  * Remove the current matrix from the stack and revert to prior
  * (or 0 if none are set at this node)
  */
  void PopTransforms() {
    // If there are any matrices on the stack, retrieve the last one and
    // remove it from the stack
    if (modelmatrix_stack.size() > 0)
    {
      model_matrix = modelmatrix_stack.back();
      modelmatrix_stack.pop_back();
    }
    else
      model_matrix.SetIdentity();
  }
};

#endif
