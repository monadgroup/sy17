float linearizeDepth(float depth, vec2 clipRange) {
    float z = depth * 2 - 1;
    return (2 * clipRange.x * clipRange.y) / (clipRange.y + clipRange.x - z * (clipRange.y - clipRange.x));
}
