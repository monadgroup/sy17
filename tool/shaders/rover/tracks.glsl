#include "../common/math.glsl"
#include "../common/rover_data.glsl"

in vec2 texCoords;
out float outTex;

uniform sampler2D sLast;
uniform vec2 worldSize;
uniform vec2 viewportSize;

void main() {
    if (abs(roverDelta.x) > 1 || abs(roverDelta.y) > 1) {
        outTex = 0;
    } else {
        vec2 worldSpace = texCoords * worldSize;
        vec2 worldCoords = worldSpace + roverPos;

        float lastAngle = atan(roverLastDir.y, roverLastDir.x);
        float thisAngle = atan(roverDir.y, roverDir.x);

        vec2 lastRotatedCoords = doR(worldSpace - worldSize/2, lastAngle) - roverDelta;
        vec2 rotatedCoords = doR(worldSpace - worldSize/2, thisAngle);

        float thisStep = step(-0.07, rotatedCoords.x);
        float lastStep = step(-0.07, lastRotatedCoords.x);

        float trackMul = 1 - step(abs(thisStep - lastStep), 0);

        float mirrorX = abs(rotatedCoords.y);
        float mul = step(0.048, mirrorX) * step(mirrorX, 0.066);
        float trackAmt = mul * trackMul * 2;

        vec2 unmappedCoords = doR(rotatedCoords, -thisAngle) + worldSize/2 - roverDelta;
        unmappedCoords /= worldSize;
        float readVal = texture(sLast, unmappedCoords).r;
        outTex = readVal + trackAmt;
    }
}
