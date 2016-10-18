//
// Created by Raphaël Dantzer on 17/10/16.
//
#include "FrameCounter.h"

FrameCounter::FrameCounter(unsigned int frameUpdate) : _fps(0), _frameCount(0U), _frameUpdate(frameUpdate) {
}

FrameCounter::~FrameCounter() {

}

void FrameCounter::start() {
    _start = std::chrono::system_clock::now();
}

void FrameCounter::stop() {
    _end = std::chrono::system_clock::now();
    _elapsedTime = _end - _start;
}

const double FrameCounter::getFps() const {
    return _fps;
}

const double FrameCounter::getElapsedTime() const {
    return _elapsedTime.count();
}