#pragma once

class TexturedSphere : public TriSurface {
public:
    TexturedSphere(const float minlat, const float maxlat, const uint32_t nlat,
        const float minlng, const float maxlng, const uint32_t nlng,
        const float radius, const int position_loc, const int normal_loc, const int tex_coord_loc) {
        // Convert to radians
        float minlat_rad = DegreesToRadians(minlat);
        float maxlat_rad = DegreesToRadians(maxlat);
        float minlng_rad = DegreesToRadians(minlng);
        float maxlng_rad = DegreesToRadians(maxlng);

        // Create a vertex list with unit length normals
        float lng = minlng_rad;
        float coslat, coslng, sinlng;
        float dLat = (maxlat_rad - minlat_rad) / static_cast<float>(nlat);
        float dLng = (maxlng_rad - minlng_rad) / static_cast<float>(nlng);
        for (float lng = minlng_rad; lng < maxlng_rad + kEpsilon; lng += dLng) {
            coslng = cosf(lng);
            sinlng = sinf(lng);
            for (float lat = maxlat_rad; lat >= minlat_rad - kEpsilon; lat -= dLat) {
                coslat = cosf(lat);
                Vector3 normal(coslng * coslat, sinlng * coslat, sinf(lat));
                Point3 vertex(radius * normal.x, radius * normal.y, radius * normal.z);
                auto normalize = [](float val, float min, float max) { return (val - min) / (max - min); };
                Vector2 texCoord(normalize(lng, minlng_rad, maxlng_rad), normalize(lat, minlat_rad, maxlat_rad));
                VertexNormalTexcoord vnt(vertex, normal, texCoord);
                vnts.push_back(vnt);
            }
        }

        // Copy the first column of vertices
        for (uint32_t i = 0; i <= nlat + 1; i++) {
            vnts.push_back(vnts[i]);
        }

        // Construct face list.  There are nlat+1 rows and nlng+1 columns. Create VBOs
        ConstructRowColFaceList(nlng + 1, nlat + 1);
        createVertexBuffersWithTexture(position_loc, normal_loc, tex_coord_loc);
    };

private:
    // Make default constructor private to force use of the constructor
    // with number of subdivisions.
    TexturedSphere() { };
};