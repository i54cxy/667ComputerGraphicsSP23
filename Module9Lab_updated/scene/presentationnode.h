//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  	David W. Nesbitt
//	File:     presentationnode.h
//	Purpose:	Scene graph presentation node.
//
//============================================================================

#ifndef __PRESENTATIONNODE_H
#define __PRESENTATIONNODE_H

/**
* Presentation node. Holds material properties.
*/
class PresentationNode : public SceneNode {
public:
  /**
   * Constructor
   */
  PresentationNode() {
    node_type = SCENE_PRESENTATION;
    reference_count = 0;
    material_shininess = 1.0f;
    // Note: color constructors default rgb to 0 and alpha to 1
  }

  /**
   * Constructor given material properties.
   * @param  ma  Material ambient reflection coefficients (color).
   * @param  md  Material diffuse reflection coefficients (color).
   * @param  ms  Material specular reflection coefficients (color).
   * @param  me  Material emission (color).
   * @param  s   Material shininess.
   */
  PresentationNode(const Color4& ma, const Color4& md, const Color4& ms,
                   const Color4& me, const float s)
    : material_ambient(ma),
      material_diffuse(md),
      material_specular(ms),
      material_emission(me),
      material_shininess(s) {
  }

  /**
   * Set material ambient reflection coefficient.
   * @param  c  Ambient reflection coefficients (color).
   */
  void SetMaterialAmbient(const Color4& c) {
    material_ambient = c;
  }

  /**
   * Set material diffuse reflection coefficient.
   * @param  c  Diffuse reflection coefficients (color).
   */
  void SetMaterialDiffuse(const Color4& c) {
    material_diffuse = c;
  }

  /**
   * Set material ambient and diffuse reflection coefficient to the
   * same value.
   * @param  c  Ambient/diffuse reflection coefficients (color).
   */
  void SetMaterialAmbientAndDiffuse(const Color4& c) {
    material_ambient = c;
    material_diffuse = c;
  }

  /**
   * Set material specular reflection coefficient.
   * @param  c  Specular reflection coefficients (color).
   */
  void SetMaterialSpecular(const Color4& c) {
    material_specular = c;
  }

  /**
   * Set material emission.
   * @param  c  Emission (color).
   */
  void SetMaterialEmission(const Color4& c) {
    material_emission = c;
  }

  /**
   * Set material shininess.
   * @param  s  Shininess.
   */
  void SetMaterialShininess(const float s) {
    material_shininess = s;
  }

  /**
   * Draw. Sets the material properties.
   * @param  scene_state  Scene state (holds material uniform locations)
   */
  void Draw(SceneState& scene_state) {
    // Set the material uniform values
    glUniform4fv(scene_state.materialambient_loc, 1, &material_ambient.r);
    glUniform4fv(scene_state.materialdiffuse_loc, 1, &material_diffuse.r);
    glUniform4fv(scene_state.materialspecular_loc, 1, &material_specular.r);
    glUniform4fv(scene_state.materialemission_loc, 1, &material_emission.r);
    glUniform1f(scene_state.materialshininess_loc, material_shininess);
    glUniform1i(scene_state.use_texture_loc, scene_state.useTexture);

    // Draw children of this node
    SceneNode::Draw(scene_state);
  }

protected:
  Color4  material_ambient;
  Color4  material_diffuse;
  Color4  material_specular;
  Color4  material_emission;
  GLfloat material_shininess;
};

#endif
