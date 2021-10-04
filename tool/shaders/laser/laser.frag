in vec3 VertPos;

out vec3 outColor;
out vec3 outHighlight;

void main() {
    vec3 color = vec3(1, 0.05, 0.05)*5;
	outColor = color;
	outHighlight = color;
}
