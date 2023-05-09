#pragma once

class TexturedUnitSquare : public TriSurface {
public:


    /**
     * Creates a unit length and width "flat surface".  The surface is composed of
     * triangles such that the unit length/width surface is divided into n
     * equal paritions in both x and y. Constructs a vertex list and face list
     * for the surface.
     * @param  n   Number of subdivisions in x and y
     */
    TexturedUnitSquare(uint32_t n, int scale, const int position_loc, const int normal_loc, const int tex_coord_loc) {
        // Student to define. Module 6. Create vertex list and face list for
        // subdivided unit square
        for (auto i = 0; i <= n; i++) {
            for (auto j = 0; j <= n; j++) {
                auto vertex = Point3(float(j) / n - 0.5f, float(i) / n - 0.5f, 0.0f);
                auto normal = Vector3(0.0f, 0.0f, 1.0f);
                auto texCoord = Vector2(float(i) / n * scale, float(j) / n * scale);
                VertexNormalTexcoord vnt(vertex, normal, texCoord);
                //std::cout << vnt.texCoord.x << ", " << vnt.texCoord.y << std::endl;
                vnts.push_back(vnt);
            }
        }
        for (auto i = 0; i < n; i++) {
            for (auto j = 0; j < n; j++) {
                int sz = n + 1;
                // upper left
                faces.push_back(i * sz + j);
                faces.push_back((i + 1) * sz + j + 1);
                faces.push_back((i + 1) * sz + j);
                // bottom right
                faces.push_back(i * sz + j);
                faces.push_back(i * sz + j + 1);
                faces.push_back((i + 1) * sz + j + 1);
            }
        }
        createVertexBuffersWithTexture(position_loc, normal_loc, tex_coord_loc);
    }

private:
    // Make default constructor private to force use of the constructor
    // with number of subdivisions.
    TexturedUnitSquare() { };
};