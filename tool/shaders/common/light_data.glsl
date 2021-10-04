layout(std140, binding=2) uniform light_data {
    uniform mat4 lightMatrix;
    uniform vec4 lightDirection;
};
