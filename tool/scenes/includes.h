#pragma once

#include "util.h"

#include "renderer/gl.h"

#include "renderer/defines.h"
#include "renderer/context.h"
#include "renderer/gl.h"
#include "renderer/ubo.h"
#include "renderer/object.h"
#include "renderer/rawobject.h"
#include "renderer/feedbackobject.h"
#include "renderer/scene.h"
#include "renderer/uniform.h"
#include "renderer/uniformbuffer.h"
#include "renderer/program.h"
#include "renderer/shader.h"
#include "renderer/camera.h"
#include "renderer/directionallight.h"
#include "renderer/texture.h"
#include "renderer/framebuffer.h"
#include "renderer/pingpong.h"
#include "renderer/timer.h"
#include "shaders/common/materials.glsl"

#include "procedural/noisegenerator.h"
#include "procedural/diamondsquaregenerator.h"
#include "procedural/rockgenerator.h"
#include "procedural/obeliskgenerator.h"

#include "units/sceneunit.h"
#include "units/postunit.h"

#include "geom/vertex.h"
#include "geom/polygon.h"
#include "geom/plane.h"
#include "geom/mesh.h"
#include "geom/generators.h"

#include "time/track_.h"
#include "demo/demowindow.h"

#include "debug.h"
