#pragma once
#include <vector>
#include "Particle.hpp"
#include "SpringDamper.hpp"

class Cloth : public SceneNode {
private:
    std::vector<Particle> particles;
    std::vector<SpringDamper> spring_damps;
    int num_row, num_col;
    float mass;
    float width;
    float height;
    float drag_density_coefficient = 0.0000f;

    GLuint vao;
    GLuint vbo;

    Particle* getParticle(int x, int y) {
        return &particles[y * (num_col + 1) + x];
    }

    void addSpringDamper(Particle* p1, Particle* p2) {
        spring_damps.push_back(SpringDamper(1000, 20, p1, p2));
    }

    Vector3 calcTriangleNormal(Particle* p1, Particle* p2, Particle* p3) {
        Vector3 v1 = p2->getPos() - p1->getPos();
        Vector3 v2 = p3->getPos() - p1->getPos();

        return v1.Cross(v2);
    }

    void computeDragAndWind(Particle* p1, Particle* p2, Particle* p3, const Vector3 air) {
        Vector3 v = (p1->getV() + p2->getV() + p3->getV()) * (1.0f / 3.0f) - air;
        //std::cout << v.x << ", " << v.y << ", " << v.z << ", " << std::endl;
        Vector3 normal = calcTriangleNormal(p1, p2, p3).Normalize();

        float area = (p2->getPos() - p1->getPos()).Cross(p3->getPos() - p1->getPos()).Norm() / 2.0f;
        if (v.Norm() > kEpsilon) {
            float cross_area = area * v.Dot(normal) / v.Norm();
            // the drag equation
            Vector3 f = -0.5 * v.Norm() * v.Norm() * drag_density_coefficient * cross_area * normal;

            p1->addForce(f * (1.0f / 3.0f));
            p2->addForce(f * (1.0f / 3.0f));
            p3->addForce(f * (1.0f / 3.0f));
        }

    }

    void drawTriangle(Particle* p1, Particle* p2, Particle* p3, std::vector<VertexAndNormal>& vertexData) {
        VertexAndNormal v1 = { p1->getPos(), p1->normal };
        vertexData.push_back(v1);
        VertexAndNormal v2 = { p2->getPos(), p2->normal };
        vertexData.push_back(v2);
        VertexAndNormal v3 = { p3->getPos(), p3->normal };
        vertexData.push_back(v3);
    }

public:
    Cloth(float mass, float width, float height, int col, int row) :
        num_row(row), num_col(col), mass(mass), width(width), height(height) {
        reset();

        glGenVertexArrays(1, &vao);

        glGenBuffers(1, &vbo);
    }

    void reset() {
        particles.resize((num_row + 1) * (num_col + 1));
        spring_damps.clear();
        // generate particles
        for (int x = 0; x <= num_col; x++) {
            for (int y = 0; y <= num_row; y++) {
                Point3 pos = Point3(width * ((float)x / num_col) - width / 2.0f,
                    -height * ((float)y / num_row) + height / 2.0f,
                    75.0f);
                //std::cout << "Particle " << x << ", " << y
                //    << " position: " << pos.x << ", " << pos.y << ", " << pos.z << "." << std::endl;
                particles[y * (num_col + 1) + x] = Particle(pos);
                particles[y * (num_col + 1) + x].setMass(mass / ((num_row + 1) * (num_col + 1)));
            }
        }
        /*
        * (x,y)
            p0--p1
            | \/ |
            | /\ |
            p2--p3
        */
        for (int x = 0; x <= num_col; x++) {
            for (int y = 0; y <= num_row; y++) {
                if (x < num_col) addSpringDamper(getParticle(x, y), getParticle(x + 1, y));
                if (y < num_row) addSpringDamper(getParticle(x, y), getParticle(x, y + 1));
                if (x < num_col && y < num_row)
                {
                    addSpringDamper(getParticle(x, y), getParticle(x + 1, y + 1));
                    addSpringDamper(getParticle(x + 1, y), getParticle(x, y + 1));
                }
            }
        }

        getParticle(0, 0)->setUnmovable();
        getParticle(num_col, 0)->setUnmovable();
    }

    ~Cloth() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    void Draw(SceneState& scene_state) {
        // resetting and recalculating normals
        for (auto p : particles)
        {
            p.normal = Vector3(0, 0, 0);
        }

        for (int x = 0; x < num_col; x++)
        {
            for (int y = 0; y < num_row; y++)
            {
                /*
                  (x,y)
                    p0--p1
                    |  / |
                    | /  |
                    p2--p3
                */
                Particle* p0 = getParticle(x, y), * p1 = getParticle(x + 1, y),
                    * p2 = getParticle(x, y + 1), * p3 = getParticle(x + 1, y + 1);
                Vector3 n = calcTriangleNormal(p1, p0, p2);
                p0->normal += n;
                p1->normal += n;
                p2->normal += n;

                n = calcTriangleNormal(p3, p1, p2);
                p1->normal += n;
                p2->normal += n;
                p3->normal += n;
            }
        }

        // normalizing
        for (int x = 0; x <= num_col; x++) {
            for (int y = 0; y <= num_row; y++)
            {
                auto p = getParticle(x, y);
                p->normal.Normalize();
            }
        }

        std::vector<VertexAndNormal> vertices;
        for (int x = 0; x < num_col; x++) {
            for (int y = 0; y < num_row; y++) {
                drawTriangle(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1), vertices);
                drawTriangle(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1), vertices);
                //std::cout << x + 1 << ", " << y << " " << x << ", " << y << " " << x << ", " << y + 1 << std::endl;
                //std::cout << x + 1 << ", " << y+1 << " " << x+1 << ", " << y << " " << x << ", " << y + 1 << std::endl;
            }
        }

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glEnableVertexAttribArray(scene_state.position_loc);
        glEnableVertexAttribArray(scene_state.normal_loc);
        glVertexAttribPointer(scene_state.position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAndNormal), (const GLvoid*)0);
        glVertexAttribPointer(scene_state.normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAndNormal), (const GLvoid*)sizeof(Point3));

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexAndNormal), (void*)&vertices[0], GL_DYNAMIC_DRAW);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        // restoring VAO, VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Update(SceneState& scene_state) {
        for (auto& spd : spring_damps) {
            spd.computeForce();
        }
        for (auto& p : particles) {
            p.update(1 / scene_state.FPS / 100);
        }
    }

    /* used to add gravity (or any other arbitrary vector) to all particles*/
    void addForce(const Vector3 direction) {
        for (auto& p : particles) {
            p.addForce(direction);
        }
    }

    void computeAerodynamicForces(const Vector3 air) {
        for (int x = 0; x < num_col - 1; x++) {
            for (int y = 0; y < num_row - 1; y++) {
                computeDragAndWind(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1), air);
                computeDragAndWind(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1), air);
            }
        }
    }

    void printForce(int x, int y) {
        auto f = particles[y * num_col + x].getF();
        std::cout << "force is " << f.x << "," << f.y << "," << f.z << std::endl;
    }

    void increaseSpring(float f) {
        for (auto& spd : spring_damps) {
            spd.ks += f;
        }
        std::cout << "current spring constant: " << spring_damps[0].ks << std::endl;
    }

    void increaseDamper(float f) {
        for (auto& spd : spring_damps) {
            spd.kd += f;
        }
        std::cout << "current damper constant: " << spring_damps[0].kd << std::endl;
    }

    void increaseDrag(float f) {
        drag_density_coefficient += f;
        std::cout << "current drag constant: " << drag_density_coefficient << std::endl;
    }

    void rotate(float angle) {
        getParticle(0, 0)->rotate(angle, Vector3(0, 0, 1));
        getParticle(num_col, 0)->rotate(angle, Vector3(0, 0, 1));
    }

    void move(Vector3 offset) {
        for (auto& p : particles) {
            p.move(offset);
        }
    }

    void checkCollision(float z0) {
        for (auto& p : particles) {
            p.checkCollision(z0);
        }
    }


};
