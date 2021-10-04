layout(std140, binding=5) uniform rover_data {
    vec2 roverPos;
    vec2 roverDelta;
    vec2 roverDir;
    vec2 roverLastDir;
};
