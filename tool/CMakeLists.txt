cmake_minimum_required(VERSION 3.8)
project(tool)

set(CMAKE_CXX_STANDARD 11)

set(OMEDA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../synth")

message("Default debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")
message("Default reldeb flags: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message("Default release flags: ${CMAKE_CXX_FLAGS_RELEASE}")

set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "")
set(CMAKE_CXX_FLAGS_RELEASE "")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug")
    include_directories(./ ./tool ${OMEDA_DIR})
    set(DEV_FLAGS "-g -mconsole -DDEBUG")
    set(SMALL_FLAGS "")
    #set(OPTIMIZE_FLAGS "-ffast-math")
    set(EXTRA_SOURCE_FILES tool/dependency/shaderdependency.cpp tool/dependency/shadermanager.cpp tool/dependency/util.cpp
                           tool/renderer/implementations.cpp tool/renderer/program.cpp tool/renderer/shader.cpp tool/renderer/camera.cpp tool/renderer/timer.cpp
                           tool/ui/runtime.cpp tool/ui/toolwindow.cpp
                           tool/time/track_.cpp tool/time/bass_timemanager.cpp tool/time/device.cpp
                           tool/export/exporter.cpp
                           tool/debug.cpp
                           imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_gl.cpp)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CMAKE_CXX_COMPILER "${CMAKE_C_COMPILER}")
    set(CMAKE_AR "gcc-ar")
    set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_CXX_ARCHIVE_FINISH true)

    include_directories(./ ./player ${OMEDA_DIR})

    set(DEV_FLAGS "-mwindows -DNDEBUG -DSYNC_PLAYER")
    set(SMALL_FLAGS "-Os -nostdlib -fno-rtti -fno-stack-check -fno-stack-protector -mno-stack-arg-probe -fno-exceptions -fno-threadsafe-statics -e_MonadMain@0")
    set(OPTIMIZE_FLAGS "-ffast-math -ftree-vectorize -msse -msse2 -msse3")
    set(LTO_FLAGS "-flto -fuse-linker-plugin")
    set(EXTRA_SOURCE_FILES player/renderer/implementations.cpp player/renderer/program.cpp player/renderer/shader.cpp player/renderer/camera.cpp player/renderer/timer.cpp
                           player/ui/runtime.cpp
                           player/time/track_.cpp player/time/timemanager.cpp
                           player/export.cpp
                           player/debug.cpp)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    message("RelWithDebInfo")
    set(DEV_FLAGS "${DEV_FLAGS} -g")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Release")
    set(SMALL_FLAGS "${SMALL_FLAGS} -s")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STANDARD_FLAGS} ${DEV_FLAGS} ${SMALL_FLAGS} ${OPTIMIZE_FLAGS} ${LTO_FLAGS} -D__CRT__NO_INLINE")

set(SOURCE_FILES main.cpp util.cpp
                 env/math.cpp env/newdel.cpp env/hacks.cpp
                 geom/mesh.cpp geom/polygon.cpp geom/vertex.cpp geom/plane.cpp geom/generators.cpp
                 renderer/context.cpp renderer/ubo.cpp renderer/uniform.cpp renderer/uniformbuffer.cpp
                 renderer/rawobject.cpp renderer/object.cpp renderer/feedbackobject.cpp renderer/camera.cpp
                 renderer/texture.cpp renderer/framebuffer.cpp renderer/pingpong.cpp renderer/extfuncs.cpp
                 renderer/directionallight.cpp
                 units/directionallightunit.cpp units/sceneunit.cpp units/ssaounit.cpp units/motionblurunit.cpp units/dofunit.cpp units/bloomblurunit.cpp units/bloomunit.cpp units/compositeunit.cpp units/fxaaunit.cpp units/postunit.cpp units/flareunit.cpp
                 procedural/noisegenerator.cpp procedural/diamondsquaregenerator.cpp procedural/rockgenerator.cpp procedural/obeliskgenerator.cpp procedural/distributedsquaregenerator.cpp
                 time/track.cpp time/timemanager.cpp time/track_.cpp
                 demo/demowindow.cpp
                 scenes/marsTestScene.cpp)

set(OMEDA_COMMON_SOURCE_FILES ${OMEDA_DIR}/Add2.cpp ${OMEDA_DIR}/Add4.cpp ${OMEDA_DIR}/Amplitude.cpp ${OMEDA_DIR}/BeatTimer.cpp ${OMEDA_DIR}/BiquadFilter.cpp ${OMEDA_DIR}/BiquadHighPassFilter.cpp
                              ${OMEDA_DIR}/BiquadLowPassFilter.cpp ${OMEDA_DIR}/BiquadPeakFilter.cpp ${OMEDA_DIR}/Clamp.cpp ${OMEDA_DIR}/CommonBiquadFilter.cpp ${OMEDA_DIR}/CommonOscillator.cpp
                              ${OMEDA_DIR}/DbToLinear.cpp ${OMEDA_DIR}/Delay.cpp ${OMEDA_DIR}/Divide.cpp ${OMEDA_DIR}/Instrument.cpp ${OMEDA_DIR}/LinearToDb.cpp ${OMEDA_DIR}/Max.cpp ${OMEDA_DIR}/Min.cpp ${OMEDA_DIR}/Mix.cpp ${OMEDA_DIR}/Modulo.cpp
                              ${OMEDA_DIR}/Multiply.cpp ${OMEDA_DIR}/Node.cpp ${OMEDA_DIR}/Noise.cpp ${OMEDA_DIR}/NoteFreq.cpp ${OMEDA_DIR}/Pan.cpp ${OMEDA_DIR}/Power.cpp ${OMEDA_DIR}/RampOscillator.cpp ${OMEDA_DIR}/SawOscillator.cpp
                              ${OMEDA_DIR}/SineOscillator.cpp ${OMEDA_DIR}/SquareOscillator.cpp ${OMEDA_DIR}/StateVariableFilter.cpp ${OMEDA_DIR}/StepSequencer.cpp ${OMEDA_DIR}/Subtract.cpp
                              ${OMEDA_DIR}/ThresholdEnvelope.cpp ${OMEDA_DIR}/TriangleOscillator.cpp ${OMEDA_DIR}/TriggerEnvelope.cpp ${OMEDA_DIR}/Util.cpp)

set(OMEDA_STATIC_SOURCE_FILES ${OMEDA_DIR}/DirectSoundPlayer.cpp ${OMEDA_DIR}/Export.cpp ${OMEDA_DIR}/Parameter.cpp ${OMEDA_DIR}/SongData.cpp ${OMEDA_DIR}/SongPlayer.cpp ${OMEDA_DIR}/Track.cpp ${OMEDA_DIR}/Voice.cpp)

add_executable(tool ${SOURCE_FILES} ${EXTRA_SOURCE_FILES} ${OMEDA_COMMON_SOURCE_FILES} ${OMEDA_STATIC_SOURCE_FILES})

# load glslang
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(../glslang glslang)

    target_link_libraries(tool glslang ${CMAKE_SOURCE_DIR}/../bass.dll)
    add_custom_command(TARGET tool POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/../bass.dll"
            $<TARGET_FILE_DIR:tool>)
endif()


find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(tool ${OPENGL_LIBRARIES} Winmm Ws2_32 shlwapi dsound ${CMAKE_CURRENT_SOURCE_DIR}/player/Dialog1.o msvcrt kernel32 user32)
