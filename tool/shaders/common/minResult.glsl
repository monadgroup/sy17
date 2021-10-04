RaymarchResult minResult(RaymarchResult a, RaymarchResult b) {
    float q = step(a.dist, b.dist);
    return RaymarchResult(
        mix(b.localP, a.localP, q),
        mix(b.dist, a.dist, q),
        mix(b.material, a.material, q)
    );
}
