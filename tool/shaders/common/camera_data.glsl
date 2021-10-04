layout(std140, binding=0) uniform camera_data {
    uniform mat4 viewMatrix;
    uniform mat4 projectionMatrix;
    uniform mat4 lastFrameMatrix;
    uniform vec4 cameraPos;
    uniform vec2 clipRange;
};
