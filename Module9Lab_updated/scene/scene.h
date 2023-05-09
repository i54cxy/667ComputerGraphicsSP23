//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    Scene.h
//	Purpose:	Scene graph support.
//
//============================================================================

#ifndef __SCENE_H
#define __SCENE_H

#include <vector>

enum SceneNodeType { SCENE_BASE, SCENE_PRESENTATION, 
                     SCENE_TRANSFORM, SCENE_GEOMETRY,
                     SCENE_SHADER, SCENE_CAMERA  };

// Include other scene files
#include <stdint.h>
#include "scene/color3.h"
#include "scene/color4.h"
#include "scene/scenestate.h"
#include "scene/scenenode.h"
#include "scene/transformnode.h"
#include "scene/presentationnode.h"
#include "scene/geometrynode.h"
#include "scene/shadernode.h"
#include "scene/cameranode.h"
#include "scene/trisurface.h"

inline void CheckError(const char* str) {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR)
    printf("%s: OpenGL Error: %d\n", str, err);
}

#endif
