#include "distributedsquaregenerator.h"
#include "util.h"

using namespace monad;

DistributedSquareGenerator::DistributedSquareGenerator(ivec2 size) : size(size) {
    cells = new bool*[size.y];
    for (auto y = 0; y < size.y; y++) {
        cells[y] = new bool[size.x];
    }

    maxSize = size.x > size.y ? size.x : size.y;
    maxSize /= 2;
}

bool DistributedSquareGenerator::inBounds(ivec2 p) {
    return p.x >= 0 && p.x < size.x && p.y >= 0 && p.y < size.y;
}

bool DistributedSquareGenerator::isUsed(ivec2 p) {
    if (!inBounds(p)) return true;
    return cells[p.y][p.x];
}

void DistributedSquareGenerator::spawnRay(ivec2 p, ivec2 d) {
    auto nextP = p + d;
    if (isUsed(nextP + ivec2(d.y, d.x)) || isUsed(nextP - ivec2(d.y, d.x))) return;

    if (inBounds(nextP) && !cells[nextP.y][nextP.x]) {
        auto newRayDist = (int)random(5, maxSize * distMultiplier);
        newRays.push_back({p, d, newRayDist});
    }
}

void DistributedSquareGenerator::updateRay(Ray &ray) {
    ray.p = ray.p + ray.d;
    if (!inBounds(ray.p)) return;

    cells[ray.p.y][ray.p.x] = true;
    ray.distance--;

    if (ray.distance <= 0) {
        spawnRay(ray.p, ivec2(ray.d.y, ray.d.x));
        spawnRay(ray.p, -ivec2(ray.d.y, ray.d.x));

        if (random(0, 1) < 0.5) spawnRay(ray.p, ray.d);
    } else {
        auto nextP = ray.p + ray.d;
        if (inBounds(nextP) && !cells[nextP.y][nextP.x]) newRays.push_back(ray);
    }
}

void DistributedSquareGenerator::fixCorners() {
    for (auto y = 0; y < size.y; y++) {
        for (auto x = 0; x < size.x; x++) {
            if (!cells[y][x]) continue;

            auto usedLeft = isUsed({x - 1, y});
            auto usedRight = isUsed({x + 1, y});
            auto usedAbove = isUsed({x, y - 1});
            auto usedBelow = isUsed({x, y + 1});

            auto hFlag = usedLeft ^ usedRight;
            auto vFlag = usedAbove ^ usedBelow;
            if (hFlag && vFlag) {
                ivec2 direction;
                if (random(0, 1) < 0.5) direction = {usedLeft ? 1 : -1, 0};
                else direction = {0, usedAbove ? 1 : -1};

                auto startPoint = ivec2(x, y) + direction;
                while (!isUsed(startPoint)) {
                    cells[startPoint.y][startPoint.x] = true;
                    startPoint = startPoint + direction;
                }
            }
        }
    }
}

void DistributedSquareGenerator::floodfill(List<ivec4> &boxes) {
    for (auto y = 0; y < size.y; y++) {
        for (auto x = 0; x < size.x; x++) {
            if (cells[y][x]) continue;

            auto usedLeft = isUsed({x - 1, y});
            auto usedTop = isUsed({x, y - 1});
            if (!usedLeft || !usedTop) continue;

            auto boundX = x + 1;
            while (!isUsed({boundX, y})) boundX++;
            if (boundX == x) continue;

            auto boundY = y + 1;
            while (!isUsed({x, boundY})) boundY++;
            if (boundY == y) continue;

            boxes.push_back({x, y, boundX - x, boundY - y});
        }
    }
}

void DistributedSquareGenerator::generate(List<ivec4> &boxes) {
    distMultiplier = 1;

    for (auto y = 0; y < size.y; y++) {
        for (auto x = 0; x < size.x; x++) {
            cells[y][x] = false;
        }
    }

    ivec2 startRayP = {(int)random(0, size.x), (int)random(0, size.y)};
    ivec2 rayDirection;
    int startDist1, startDist2;

    if (random(0, 1) < 0.5) {
        rayDirection = {1, 0};
        startDist1 = (int)random(5, size.x - startRayP.x - 5);
        startDist2 = (int)random(5, size.x - startRayP.x - 5);
    } else {
        rayDirection = {0, 1};
        startDist1 = (int)random(5, size.y - startRayP.y - 5);
        startDist2 = (int)random(5, size.y - startRayP.y - 5);
    }

    rays = List<Ray>(2);
    rays.push_back({startRayP, rayDirection, startDist1});
    rays.push_back({startRayP, -rayDirection, startDist2});

    while (rays.size()) {
        for (auto &ray : rays) {
            updateRay(ray);
        }

        rays = newRays;
        newRays = List<Ray>();
        distMultiplier *= 0.99;
    }

    fixCorners();
    floodfill(boxes);
}
