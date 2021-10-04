#pragma once

#define SHARED_PTR(x) x*
#define MAKE_SHARED(x, args...) (new x(args))
