#pragma once

class ExtrudedTriangle : public TriSurface {
private:
    Point3 a, b, c;
    Vector3 extrusion; // direction and length

public:
    ExtrudedTriangle(Point3 a, Point3 b, Point3 c, Vector3 extrusion,
        const int position_loc, const int normal_loc, const int tex_coord_loc)
        : a(a), b(b), c(c), extrusion(extrusion) {
        Add(a, c, b);
        Point3 ap = a + extrusion, bp = b + extrusion, cp = c + extrusion;
        Add(a, b, ap);
        Add(b, bp, ap);
        Add(b, c, bp);
        Add(c, cp, bp);
        Add(c, a, cp);
        Add(a, ap, cp);
        Add(ap, bp, cp);
        End(position_loc, normal_loc, tex_coord_loc);
    }
};