
in vec2 texCoords;
out vec3 outColor;

uniform sampler2D sColor;
uniform vec2 viewportSize;
uniform vec2 glowDirection;

void main() {
    vec3 color = vec3(0);
    vec2 off1 = vec2(1.3846153846) * glowDirection;
    vec2 off2 = vec2(3.2307692308) * glowDirection;
    color += texture(sColor, texCoords).rgb * 0.2270270270;
    color += texture(sColor, texCoords + (off1 / viewportSize)).rgb * 0.3162162162;
    color += texture(sColor, texCoords - (off1 / viewportSize)).rgb * 0.3162162162;
    color += texture(sColor, texCoords + (off2 / viewportSize)).rgb * 0.0702702703;
    color += texture(sColor, texCoords - (off2 / viewportSize)).rgb * 0.0702702703;
    outColor = color;
}
