#pragma once
class LightNode : public SceneNode {
public:
    LightNode(const uint32_t n, const HPoint3& position, const Color4& ambient,
        const Color4& diffuse, const Color4& specular) :
        n(n), position(position), ambient(ambient), diffuse(diffuse), specular(specular),
        spotDirection(Vector3()), spotCutoff(0.0f), spotExponent(0.0f) {};

    LightNode(const uint32_t n, const HPoint3& position, const Vector3& spotDirection,
        const Color4& diffuse, const Color4& specular, float spotCutoff, float spotExponent) :
        n(n), position(position), ambient(Color4()), diffuse(diffuse), specular(specular),
        spotDirection(spotDirection), spotCutoff(spotCutoff), spotExponent(spotExponent) {
        std::cout << "location: " << position.x << ", " << position.y << ", " << position.z << std::endl;
        std::cout << "spotCutoff: " << spotCutoff << std::endl;
    };

    void Draw(SceneState& scene_state) {
        glUniform1i(scene_state.lights[n].enabled, 1);
        glUniform4fv(scene_state.lights[n].position, 1, &position.x);
        glUniform4fv(scene_state.lights[n].ambient, 1, &ambient.r);
        glUniform4fv(scene_state.lights[n].diffuse, 1, &diffuse.r);
        glUniform4fv(scene_state.lights[n].specular, 1, &specular.r);

        glUniform3fv(scene_state.lights[n].spotDirection, 1, &spotDirection.x);
        glUniform1f(scene_state.lights[n].spotCutoff, DegreesToRadians(spotCutoff));
        glUniform1f(scene_state.lights[n].spotExponent, spotExponent);
    }

    void SetPosition(Point3 pos) {
        position = HPoint3(pos.x, pos.y, pos.z, 1.0f);
    }

    void SetSpotDirection(Vector3 dir) {
        spotDirection = dir;
    }

private:
    uint32_t n;
    HPoint3 position;
    Color4 ambient;
    Color4 diffuse;
    Color4 specular;
    Vector3 spotDirection;
    float spotCutoff;
    float spotExponent;
};