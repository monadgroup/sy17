layout(vertices=4) out;

in float vScaleFactor[];
in mat3 vModelMatrix[];
in float vRockId[];

patch out mat3 tModelMatrix;
patch out float tRockId;

void main() {
    tModelMatrix = vModelMatrix[0];
    tRockId = vRockId[0];

    gl_TessLevelOuter[0]
        = gl_TessLevelOuter[1]
        = gl_TessLevelOuter[2]
        = gl_TessLevelOuter[3]
        = gl_TessLevelInner[0]
        = gl_TessLevelInner[1]
        = pow(vScaleFactor[0]*2, 3) * 100 + 2;
}
