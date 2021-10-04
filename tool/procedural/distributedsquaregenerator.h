#pragma once
#include "vector.h"
#include "list.h"

namespace monad {

    class DistributedSquareGenerator {
        struct Ray {
            ivec2 p;
            ivec2 d;
            int distance;

            Ray(ivec2 p, ivec2 d, int dist) : p(p), d(d), distance(dist) { }
        };

        ivec2 size;
        bool** cells;
        int maxSize;
        float distMultiplier;

        List<Ray> rays;
        List<Ray> newRays;

        bool inBounds(ivec2 p);
        bool isUsed(ivec2 p);
        void spawnRay(ivec2 p, ivec2 d);
        void updateRay(Ray &ray);
        void fixCorners();
        void floodfill(List<ivec4> &boxes);

    public:
        DistributedSquareGenerator(ivec2 size);

        void generate(List<ivec4> &boxes);
    };

}
