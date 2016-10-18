//
// Created by Raphaël Dantzer on 17/10/16.
//

#ifndef PARTICLE_SYSTEM_FRAMECOUNTER_H
#define PARTICLE_SYSTEM_FRAMECOUNTER_H


#include <chrono>

class FrameCounter {
public:
    FrameCounter(unsigned int _frameUpdate);
    ~FrameCounter();

    void start();
    void stop();

    const double getElapsedTime() const;
    const double getFps() const;

private:
    std::chrono::time_point<std::chrono::system_clock>  _start;
    std::chrono::time_point<std::chrono::system_clock>  _end;
    std::chrono::duration<double>                       _elapsedTime;
    double                                              _fps;
    unsigned                                            _frameCount;
    unsigned                                            _frameUpdate;

};


#endif //PARTICLE_SYSTEM_FRAMECOUNTER_H
