float unlinearizeDepth(float depth, vec2 clipRange) {
    float nonLinear = (clipRange.y + clipRange.x - 2 * clipRange.x * clipRange.y / depth) / (clipRange.y - clipRange.x);
    return nonLinear;
}
