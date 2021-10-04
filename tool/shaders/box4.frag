
in vec2 texCoords;
out float blurredTex;

uniform sampler2D inTex;
uniform vec2 viewportSize;

const int blurSize = 4;

void main() {
    vec2 texelSize = 1 / viewportSize;
    float result = 0;
    vec2 hlim = vec2(-float(blurSize) * 0.5 + 0.5);
    for (int i = 0; i < blurSize; i++) {
        for (int j = 0; j < blurSize; j++) {
            vec2 offset = (hlim + vec2(float(i), float(j))) * texelSize;
            result += texture(inTex, texCoords + offset).r;
        }
    }

    blurredTex = result / float(blurSize * blurSize);
}
