in vec2 texCoords;
out vec4 copyTex;
uniform sampler2D sIn;
void main() { copyTex = texture(sIn, texCoords); }
