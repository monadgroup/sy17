layout(location=0) in vec2 vertexPos;

out vec2 texCoords;

void main() {
    texCoords = vertexPos * 0.5 + 0.5;
    gl_Position = vec4(vertexPos, 0, 1);
}
