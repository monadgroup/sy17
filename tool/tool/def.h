#pragma once

#include <memory>
#define SHARED_PTR(x) std::shared_ptr<x>
#define MAKE_SHARED(x, args...) (std::make_shared<x>(args))
