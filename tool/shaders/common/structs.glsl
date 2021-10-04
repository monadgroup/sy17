struct RaymarchResult {
    vec3 localP;
    float dist;
    float material;
};

struct MaterialInput {
    float depth;
    vec3 d;
    vec3 p;
    vec3 n;
};

struct EvaluatedMaterial {
    vec3 normal;
    vec3 albedo;
    vec3 emissive;
    float reflectivity;
    float roughness;
    float metallic;
    float ao;
};
