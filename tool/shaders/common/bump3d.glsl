
#define BUMP_3D(f, p, n) normalize(f((p).yz).yxz*abs((n).x) + f((p).xz).xyz*abs((n).y) + f((p).xy).xzy*abs((n).z))
