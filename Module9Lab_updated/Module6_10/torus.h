#pragma once

class Torus : public TriSurface {
public:
    Torus(float ring_radius, float tube_radius, int ring_division, int tube_division,
        const int position_loc, const int normal_loc, const int tex_coord_loc) {
        float delta_ring = 2 * kPi / ring_division;
        float delta_tube = 2 * kPi / tube_division;
        for (int i = 0; i <= ring_division; i++) {
            for (int j = 0; j <= tube_division; j++) {
                float u = delta_ring * i;
                float v = delta_tube * j;
                VertexAndNormal van;
                van.vertex.Set(
                    cos(u) * (ring_radius + tube_radius * cos(v)),
                    sin(u) * (ring_radius + tube_radius * cos(v)),
                    tube_radius * sin(v));
                van.normal.Set(
                    cos(v) * cos(u),
                    cos(v) * sin(u),
                    sin(v)
                );
                //std::cout << "XXX" << std::endl;
                //std::cout << van.vertex.x << ", " << van.vertex.y << ", " << van.vertex.z << std::endl;
                //std::cout << van.normal.x << ", " << van.normal.y << ", " << van.normal.z << std::endl;
                vertices.push_back(van);
            }
        }
        for (int i = 0; i < ring_division; i++) {
            for (int j = 0; j < tube_division; j++) {
                int sz = tube_division + 1;
                // top left
                faces.push_back(i * sz + j);
                faces.push_back((i + 1) * sz + j + 1);
                faces.push_back(i * sz + j + 1);
                // bottom right
                faces.push_back(i * sz + j);
                faces.push_back((i + 1) * sz + j);
                faces.push_back((i + 1) * sz + j + 1);
            }
        }
        // Create VBOs and VAO
        CreateVertexBuffers(position_loc, normal_loc, tex_coord_loc);
    }
};