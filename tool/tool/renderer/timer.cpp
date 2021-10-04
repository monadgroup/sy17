#include "timer.h"
#include <utility>

using namespace monad;

Timer::Timer(const char *name) : name(name) {
    GLuint timers[2];
    glGenQueries(2, timers);

    startResource = timers[0];
    stopResource = timers[1];
}

void Timer::start() {
    glQueryCounter(startResource, GL_TIMESTAMP);
}

void Timer::stop() {
    glQueryCounter(stopResource, GL_TIMESTAMP);
}

void Timer::poll() {
    GLuint available = 0;
    while (!available) glGetQueryObjectuiv(stopResource, GL_QUERY_RESULT_AVAILABLE, &available);

    GLuint64 startTime, endTime;
    glGetQueryObjectui64v(startResource, GL_QUERY_RESULT, &startTime);
    glGetQueryObjectui64v(stopResource, GL_QUERY_RESULT, &endTime);

    this->startTime = (this->startTime + startTime) / 2;
    this->stopTime = (this->stopTime + endTime) / 2;
    this->duration = (this->duration + endTime - startTime) / 2;
}
