
in vec2 texCoords;
out vec4 blurredTex;

uniform sampler2D inTex;

uniform vec2 direction;
uniform vec2 viewportSize;

/*float coefficients[21] = {0.000272337, 0.00089296, 0.002583865, 0.00659813, 0.014869116,
                          0.029570767, 0.051898313, 0.080381679, 0.109868729, 0.132526984,
                          0.14107424,
                          0.132526984, 0.109868729, 0.080381679, 0.051898313, 0.029570767,
                          0.014869116, 0.00659813, 0.002583865, 0.00089296, 0.000272337};*/
float coefficients[11] = {0.000257, 0.003279, 0.023688, 0.097049, 0.225985, 0.299485, 0.225985, 0.097049, 0.023688, 0.003279, 0.000257};

void main() {
    vec2 texScale = 1 / viewportSize;

    vec4 val = vec4(0);
    for (int i = 0; i < coefficients.length(); i++) {
        val += texture(inTex, texCoords + vec2(i - 5) * direction * texScale) * coefficients[i];
    }
    blurredTex = val;
}
