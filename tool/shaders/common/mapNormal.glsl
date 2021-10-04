
#define MAP_NORMAL(func, diffAmt) \
    return normalize(vec3(\
        func(p - vec2(diffAmt, 0)) - func(p + vec2(diffAmt, 0)),\
        diffAmt * 2,\
        func(p - vec2(0, diffAmt)) - func(p + vec2(0, diffAmt))\
    ))
