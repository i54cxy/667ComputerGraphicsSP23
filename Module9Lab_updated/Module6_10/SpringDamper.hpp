#pragma once
#include "Particle.hpp"

class SpringDamper {
public:
    float ks, kd; // spring constant, damping constant
    float l0; // rest length
    Particle* p1, * p2;

    SpringDamper(float ks, float kd, Particle* pp1, Particle* pp2)
        :ks(ks), kd(kd), p1(pp1), p2(pp2) {
        l0 = (p2->getPos() - p1->getPos()).Norm();
    }
    ~SpringDamper() {}

    void computeForce() {
        Point3 r2 = p2->getPos(), r1 = p1->getPos();
        float l = (r2 - r1).Norm(); // length
        Vector3 e = (r2 - r1).Normalize(); // unit vector
        float v2 = p2->getV().Dot(e);
        float v1 = p1->getV().Dot(e);
        float f = -ks * (l0 - l) - kd * (v1 - v2);
        Vector3 f1 = f * e, f2 = f1 * -1.0f;
        p1->addForce(f1);
        p2->addForce(f2);
        //std::cout << "f1 is " << f1.x << ", " << f1.y << ", " << f1.z << std::endl;
    }
};