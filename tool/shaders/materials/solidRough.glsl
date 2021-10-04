EvaluatedMaterial solidRoughMaterial(MaterialInput val, vec3 color) {
    return EvaluatedMaterial(
        val.n,
        color,
        vec3(0),
        0,
        1,
        0,
        1
    );
}
