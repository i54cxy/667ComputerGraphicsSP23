#pragma once

class TexturedConic : public TriSurface {
public:
	TexturedConic(const float bottom_radius, const float top_radius,
		const uint32_t nsides, const uint32_t nstacks,
		const int position_loc, const int normal_loc, const int tex_coord_loc) {
		// Fail if top and bottom radius are both 0
		if (bottom_radius <= 0.0f && top_radius <= 0.0f)
			return;

		// Student to define. Module 6. Used to create the curved surface 
		// of a cylinder, cone, or truncated cone.
		for (auto j = 0; j <= nsides; j++) {
			// degree in radian
			float theta = float(j) / float(nsides) * 2.0f * kPi;
			Point3 bottom = Point3(cos(theta) * bottom_radius, sin(theta) * bottom_radius, 0.0f);
			Point3 top = Point3(cos(theta) * top_radius, sin(theta) * top_radius, 1.0f);
			Vector3 v1(bottom);
			Vector3 v2(bottom - top);
			Vector3 normal = (v1 - v1.Projection(v2)).Normalize();

			for (auto i = 0; i <= nstacks; i++) {
				float alpha = float(i) / float(nstacks);
				float radius = bottom_radius * (1.0f - alpha) + top_radius * alpha;
				Point3 vertex(cos(theta) * radius, sin(theta) * radius, alpha - 0.5f);
				auto normalize = [](float val, float min, float max) { return (val - min) / (max - min); };
				Vector2 texCoord(normalize(j, 0, nsides), normalize(i, 0, nstacks));
				VertexNormalTexcoord vnt(vertex, normal, texCoord);
				vnts.push_back(vnt);
			}
		}
		for (auto j = 0; j < nsides; j++) {
			for (auto i = 0; i < nstacks; i++) {
				int sz = nstacks + 1;
				// top left
				faces.push_back(j * sz + i);
				faces.push_back((j + 1) * sz + i + 1);
				faces.push_back(j * sz + i + 1);
				// bottom right
				faces.push_back(j * sz + i);
				faces.push_back((j + 1) * sz + i);
				faces.push_back((j + 1) * sz + i + 1);
			}
		}
		//for (auto i = 0; i < faces.size(); i += 3) {
		//	std::cout << faces[i] << ", " << faces[i + 1] << ", " << faces[i + 2] << std::endl;
		//}
		// Create VBOs and VAO
		createVertexBuffersWithTexture(position_loc, normal_loc, tex_coord_loc);
	}

private:
	// Make default constructor private to force use of the constructor
	// with number of subdivisions.
	TexturedConic() { };
};