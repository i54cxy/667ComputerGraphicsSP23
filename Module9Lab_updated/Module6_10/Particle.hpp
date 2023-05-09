#pragma once

class Particle {
private:
    bool movable;
    float mass;
    Point3 position;
    Vector3 velocity;
    Vector3 force;

    float elasticity = 0.1f, friction = 1.00f;

public:
    Vector3 normal;

    Particle(Point3 pos) : movable(true), mass(1.0f), position(pos), velocity(Vector3(0, 0, 0)),
        force(Vector3(0, 0, 0)), normal(Vector3(0, 0, 0)) {}
    Particle() : movable(false), mass(0.0f) {}
    ~Particle() {}

    void addForce(Vector3 f) {
        force += f;
    }

    void move(Vector3 t) {
        position = position + t;
    }

    // calculate force before calling update
    void update(float deltaTime) {
        if (!movable) {
            return;
        }
        Vector3 accel = (1.0 / mass) * force;
        velocity += accel * deltaTime;
        position = position + velocity * deltaTime;

        //std::cout << "force is " << force.x <<", " << force.y << ", " << force.z << std::endl;
        //std::cout << "velocity is " << velocity.x << ", " << velocity.y << ", " << velocity.z << std::endl;

        force = Vector3(0, 0, 0);

    }

    void setMass(float m) {
        mass = m;
    }

    Point3 getPos() {
        return position;
    }

    Vector3 getV() {
        return velocity;
    }

    Vector3 getF() {
        return force;
    }

    void setMovable() {
        movable = true;
    }

    void setUnmovable() {
        movable = false;
    }

    void rotate(float angle, Vector3 axis) {
        Matrix4x4 transform;
        transform.SetIdentity();
        transform.Rotate(angle, axis.x, axis.y, axis.z);
        position = transform * position;
    }

    void checkCollision(float z0) {
        if (position.z < z0) {
            position.z = 2 * z0 - position.z;
            velocity.z = -elasticity * velocity.z;
            velocity.x = (1.0f - friction) * velocity.x;
            velocity.y = (1.0f - friction) * velocity.y;
        }
    }
};