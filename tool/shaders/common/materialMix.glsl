EvaluatedMaterial materialMix(EvaluatedMaterial a, EvaluatedMaterial b, float m) {
    return EvaluatedMaterial(
        mix(a.normal, b.normal, m),
        mix(a.albedo, b.albedo, m),
        mix(a.emissive, b.emissive, m),
        mix(a.reflectivity, b.reflectivity, m),
        mix(a.roughness, b.roughness, m),
        mix(a.metallic, b.metallic, m),
        mix(a.ao, b.ao, m)
    );
}
