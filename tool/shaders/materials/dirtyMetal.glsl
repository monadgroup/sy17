
EvaluatedMaterial dirtyMetalMaterial(MaterialInput val) {
    EvaluatedMaterial metal = metalMaterial(val);

    float dirtAmount = saturate(fbm(val.p * 100) * 2);
    vec3 terrainColor = mix(vec3(0.715, 0.089, 0.036)*2, vec3(0.5), 0.2);

    metal.albedo = mix(metal.albedo, terrainColor, dirtAmount);
    metal.roughness = 0.5 + dirtAmount * 0.4;
    metal.metallic = 0.8 + dirtAmount * 0.2;

    return metal;
}
