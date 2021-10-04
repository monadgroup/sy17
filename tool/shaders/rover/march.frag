#include "../common/structs.glsl"
#include "../common/math.glsl"
#include "../common/noise.glsl"
#include "../common/camera_data.glsl"
#include "../common/rover_data.glsl"
#include "../common/scene_data.glsl"
#include "../common/getRay.glsl"
#include "../common/linearizeDepth.glsl"
#include "../common/unlinearizeDepth.glsl"
#include "../common/sdf.glsl"
#include "../common/materials.glsl"
#include "../common/mapNormal.glsl"
#include "../common/rasterFrag.glsl"
#include "../common/minResult.glsl"
#include "../marsHeightmap.glsl"

float roverHeight;
float roverAngle;
vec3 wheelHeights[2];
vec2 roverTilt;

const float marchSizeDiff = 40;

float fHolyBox(vec3 p, vec3 size) {
    float boxDist = fBox(p, size);

    float circleSpacing = 2 * size.y / 5;
    float circleRadius = size.y - circleSpacing;
    float sectionSize = 2 * circleRadius + 2 * circleSpacing;

    pMod1(p.z, sectionSize);
    pR(p.xy, radians(90));
    float cylinderDist = fCylinder(p, circleRadius, size.x+0.1);

    return max(boxDist, -cylinderDist);
}

float fWheel(vec3 p) {
    pR(p.xz, -time/10);

    // axle connection
    float axleDist = fCylinder(p - vec3(0, 0.5, 0), 0.3, 0.2);

    p.xz *= 1.2;

    pMirror(p.y, 0);
    p.y -= 0.23;

    //float cylinderDist = fCylinder(p, 1, 1);
    float coneDist = fConeSection(p, 0.4, 0.2, 0.15);

    // add curved edges to outer ring
    float ringY = p.y - 0.05;
    float ringSize = 0.03 * sin(max(0.1, ringY)*20-3.5);

    float circleDist = fCircle(vec3(p.x, 0, p.z), 0.8 - ringSize) - 0.02;
    circleDist = max(circleDist, abs(ringY) - 0.2);

    // and center cylinder
    float centerCylinder = fCylinder(p + vec3(0, 0.19, 0), 0.82 - ringSize, 0.05);
    circleDist = min(circleDist, centerCylinder);

    // add grips
    const float gripAmount = 40;
    pModPolar(p.xz, gripAmount);
    p -= vec3(0.857, -0.05, 0);
    float gripPlate = fBox(p, vec3(0.01, 0.15, 0.02));
    pModInterval1(p.y, 0.04, -3, 3);
    //float gripBox = fBox(p - vec3(0.02, 0, 0), vec3(0.01));
    pR(p.xy, radians(-90));
    float gripCone = fCone(p - vec3(0, 0.009, 0), 0.015, 0.04);
    gripPlate = min(gripPlate, gripCone);

    coneDist = min(coneDist, gripPlate);

    //return RaymarchResult(p, min(coneDist, min(circleDist, axleDist)), MATERIAL_METAL);
    return min(coneDist, min(circleDist, axleDist));
}

RaymarchResult fBody(vec3 p) {
    p -= vec3(0, 1.8, -0.25);

    vec3 antannaP1 = vec3(2.04, 0.6, 3);
    vec3 antannaP2 = vec3(2.04, 1.2, 3);
    vec3 antannaP3 = vec3(2.3,  1.5, 3);
    vec3 antannaP4 = vec3(2.3,  3,   3);

    float antanna1 = fLineSegment(p, antannaP1, antannaP2) - 0.05;
    float antanna2 = fLineSegment(p, antannaP2, antannaP3) - 0.05;
    float antanna3 = fLineSegment(p, antannaP3, antannaP4) - 0.05;
    RaymarchResult antanna = RaymarchResult(p, min(min(antanna1, antanna2), antanna3), MATERIAL_WHITE);

    p.y *= 1.2;

    float solarBox = fBox(p - vec3(0, 0.845, -0.6), vec3(2.3, 0.05, 2.6));
    solarBox = min(solarBox, fBox(p - vec3(0, 0.845, 2.5), vec3(2, 0.05, 1)));
    RaymarchResult solarPanels = RaymarchResult(p, solarBox, MATERIAL_SOLAR);

    float mainBox = fBox(p, vec3(2, 0.8, 2.2));
    p -= vec3(0, -0.4, 2.6);
    mainBox = min(fBox(p, vec3(2, 0.4, 0.4)), mainBox);
    RaymarchResult box = RaymarchResult(p, mainBox, MATERIAL_FOIL);

    return minResult(antanna, minResult(solarPanels, box));
}

RaymarchResult fGizmo(vec3 p) {


    vec3 connectorP = p;
    pR(connectorP.xy, radians(45));
    pModPolar(connectorP.yx, 4);
    float segment1 = fLineSegment(connectorP, vec3(0, 1, 0), vec3(0, 1, 1))-0.1;
    float segment2 = fLineSegment(connectorP, vec3(0, 1, 0), vec3(0, 0.4, -0.4))-0.1;
    float segments = min(segment1, segment2);

    p.z += 0.5;
    float box = fBox(p, vec3(0.4, 0.4, 0.2));
    pR(p.yz, radians(90));
    float cylinder = fCylinder(p + vec3(0, 0.5, 0), 0.4, 0.3);
    float wireRadius = 0.19+sin(p.y*80)*0.02;
    float wireTube = fCylinder(p - vec3(0, 1.2, 0), wireRadius, 1);

    box = min(box, cylinder);

    return RaymarchResult(p, min(min(segments, wireTube), box), MATERIAL_METAL);
}

RaymarchResult fWheelThingy(vec3 p) {
    float baseCylinderDist = fCylinder(p, 0.2, 0.05);
    p -= vec3(0, 0.05+0.03, 0.0);
    float baseBoxDist = fBox(p, vec3(0.2355, 0.03, 0.25));

    float tightenCylinder = fCylinder(p + vec3(0.04, -0.05, 0.02), 0.13, 0.1);
    baseBoxDist = min(baseBoxDist, tightenCylinder);

    //p.z -= 0.3 + 0.04;
    p -= vec3(-0.0555, 0.07, 0.25 + 0.015);
    float vBoxDist = fBox(p, vec3(0.18, 0.1, 0.03));

    pR(p.zy, radians(90));
    p.z -= 0.02;
    float bigPistonDist = fCylinder(p + vec3(0, 0.13, 0), 0.08, 0.15);
    p.y += 0.195;
    float smallPistonDist = fCylinder(p, 0.04, 0.33);
    float pistonHead = fCylinder(p - vec3(0, 0.33, 0), 0.07, 0.02);
    smallPistonDist = min(smallPistonDist, pistonHead);

    float pistonDist = min(bigPistonDist, smallPistonDist);

    baseBoxDist = min(baseBoxDist, vBoxDist);
    baseBoxDist = min(baseBoxDist, baseCylinderDist);

    return RaymarchResult(p, min(baseBoxDist, pistonDist), MATERIAL_METAL);
}

RaymarchResult fLegs(vec3 p, int side) {
    float middleHeight = roverHeight - wheelHeights[side].y;
    float backHeight = roverHeight - wheelHeights[side].z + 0.01;

    float tiltAmt = sin(roverTilt.x) / 20 * float(side * 2 - 1);
    backHeight += tiltAmt;
    middleHeight += tiltAmt;

    float backLeg1Angle = atan(backHeight-0.03, 0.03)+0.4;
    float backLeg2Angle = atan(middleHeight, 0.04)+0.4;

    vec3 originalP;
    p -= vec3(0, 1.9, -1.5);

    vec3 cylinderP = p - vec3(0, 0.05, 0.1);
    pR(cylinderP.xy, radians(90));
    float joinCylinderDist = fCylinder(cylinderP, 0.15, 0.1);

    vec3 longLegP = p;
    vec3 longBoxSize = vec3(0.05, 0.1, 1.4);
    float longPivotDist = distance(originalP, longLegP);
    float longAngle = mix(5, 28, saturate(longPivotDist/3));
    pR(longLegP.yz, radians(longAngle));
    longLegP -= vec3(0, longBoxSize.yz);
    float backBox1 = fHolyBox(longLegP, longBoxSize);
    vec3 thingyP = p + vec3(0, backHeight*marchSizeDiff + 0.7, -4.3);
    pR(thingyP.xz, radians(180));
    RaymarchResult wheelThingy1 = fWheelThingy(thingyP);

    vec3 backLegsP = p + vec3(0.1855, 1.25, -2.5);

    vec3 backCylinderP = backLegsP;
    pR(backCylinderP.xy, radians(90));
    backCylinderP -= vec3(0.1, -0.05, 0);
    float backCylinderDist = fCylinder(backCylinderP, 0.2, 0.2);

    vec3 backLeg1P = backLegsP;
    vec3 backLeg1Size = vec3(0.05, 0.1, 0.83);
    pR(backLeg1P.yz, backLeg1Angle);
    backLeg1P -= vec3(0, backLeg1Size.yz);
    float backBox2 = fHolyBox(backLeg1P, backLeg1Size);

    vec3 backLeg2P = backLegsP - vec3(0, 0.23, 0);
    vec3 backLeg2Size = vec3(0.05, 0.1, 0.7);
    float backLeg2Dist = distance(0, backLeg2P.z);
    pR(backLeg2P.yz, -backLeg2Angle);
    backLeg2P.x += pow(saturate(sin(radians(backLeg2Dist*8))), 0.3);
    backLeg2P += vec3(0, backLeg2Size.yz);
    float backBox3 = fHolyBox(backLeg2P, backLeg2Size);

    vec3 shortLegP = p - vec3(0, 0.1, 0);
    vec3 shortBoxSize = vec3(0.05, 0.1, 0.75);
    float shortPivotDist = distance(originalP, shortLegP);
    float shortAngle = mix(15, 0, saturate(shortPivotDist/10*3.5));
    pR(shortLegP.yz, radians(-45));
    pR(shortLegP.xz, radians(shortAngle));
    shortLegP += vec3(0, shortBoxSize.yz);
    float backBox4 = fHolyBox(shortLegP, shortBoxSize);
    RaymarchResult wheelThingy2 = fWheelThingy(p + vec3(0, 1.05, 1.25));

    RaymarchResult wheelThingies = minResult(wheelThingy1, wheelThingy2);

    float boxDist = min(backBox1, min(backBox2, backCylinderDist));
    boxDist = min(boxDist, min(backBox3, backBox4));

    return minResult(RaymarchResult(p, min(boxDist, joinCylinderDist), MATERIAL_METAL), wheelThingies);
}


RaymarchResult fScene(vec3 p) {
    float shiftAmount = roverHeight + 0.02;
    p -= vec3(roverPos.x, shiftAmount, roverPos.y);
    p *= marchSizeDiff;

    pR(p.zx, roverAngle);

    //vec3 shiftedXY = p - vec3(roverPos.x, 0, roverPos.y);
    //shiftedXY *= marchSizeDiff;
    //p -= vec3(roverPos.x, roverHeight+0.02, roverPos.y);
    //p = shiftedXY - vec3(0, roverHeight + 0.02, 0) * marchSizeDiff;

    //float markerDist = fSphere(p+vec3(0.0575, 0, 0.07), 0.03);

    vec3 bodyP = p;
    pR(bodyP.xy, roverTilt.x);

    vec3 justBodyP = bodyP;
    pR(justBodyP.zy, roverTilt.y);
    RaymarchResult bodyDist = fBody(justBodyP);
    bodyDist = minResult(bodyDist, fGizmo(justBodyP*1.3 - vec3(0, 2.3, -3.8)));

    float legSideIndex = step(0, pMirror(bodyP.x, 2.3));
    RaymarchResult legDist = fLegs(bodyP, int(legSideIndex));

    float sideIndex = step(0, pMirror(p.x, 2.3));
    float wheelIndex = pModInterval1(p.z, 2.8, -1, 1) + 1;
    float wheelHeight = wheelHeights[int(sideIndex)][int(wheelIndex)];

    vec3 wheelP = vec3(p.y + (shiftAmount - wheelHeight - 0.02)*marchSizeDiff, -p.x, p.z);
    RaymarchResult wheel = RaymarchResult(wheelP, fWheel(wheelP), MATERIAL_DIRTY_METAL);

    legDist = minResult(legDist, wheel);

    RaymarchResult r = minResult(bodyDist, legDist);
    r.dist *= 0.02;



    /*float sideIndex = step(0, pMirror(p.x, 0));
    p.x -= 2.3;

    RaymarchResult legDist = fLegs(p);
    float wheelIndex = pModInterval1(p.z, 2.8, -1, 1) + 1;
    //pR(p.xy, radians(90));

    float wheelHeight = wheelHeights[int(sideIndex)][int(wheelIndex)];
    legDist = minResult(legDist, fWheel(vec3(p.y + (shiftAmount - wheelHeight - 0.02)*marchSizeDiff, -p.x, p.z)));

    RaymarchResult r = minResult(bodyDist, legDist);
    r.dist *= 0.02;*/

    return r;
}

#include "../common/rasterRaymarch.glsl"

void main() {
    float axleDist = 0.0575;
    float wheelDist = 0.07;

    roverAngle = atan(roverDir.x, roverDir.y) + radians(180);

    // calculate rover positioning
    wheelHeights[0] = vec3(
        marsHeightmap(roverPos + doR(vec2(-axleDist, -wheelDist), roverAngle)),
        marsHeightmap(roverPos + doR(vec2(-axleDist, 0), roverAngle)),
        marsHeightmap(roverPos + doR(vec2(-axleDist, wheelDist), roverAngle))
    );
    //float avgLeftHeight = (wheelHeights[0].x + wheelHeights[0].y + wheelHeights[0].z) / 3;

    wheelHeights[1] = vec3(
        marsHeightmap(roverPos + doR(vec2(axleDist, -wheelDist), roverAngle)),
        marsHeightmap(roverPos + doR(vec2(axleDist, 0), roverAngle)),
        marsHeightmap(roverPos + doR(vec2(axleDist, wheelDist), roverAngle))
    );
    //float avgRightHeight = (wheelHeights[1].x + wheelHeights[1].y + wheelHeights[1].z) / 3;

    // rover height is based on height of front wheel (this is  a rigid connection)

    float frontHeight = (wheelHeights[0].x + wheelHeights[1].x) / 2;
    float backHeight = (wheelHeights[0].z + wheelHeights[1].z) / 2;
    float leftHeight = wheelHeights[0].x, rightHeight = wheelHeights[1].x;
    roverHeight = (leftHeight + rightHeight) / 2;
    roverTilt.x = atan(rightHeight - leftHeight, axleDist*2);
    roverTilt.y = atan(backHeight - frontHeight, wheelDist*2);

    // render
    rasterRaymarch(marchSizeDiff, vec3(roverDelta.x, 0, roverDelta.y));
}
