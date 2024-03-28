#include <GLFW/glfw3.h>
#include "timer.h"

namespace game {

    Timer::Timer(void) 
    {
        end_time_ = 0;
    }

    Timer::~Timer(void)
    {
    }

    void Timer::Start(float duration)
    {
        // Get the current time and add the duration to it
        end_time_ = glfwGetTime() + duration;
    }

    bool Timer::Finished(void) 
    {
        if (glfwGetTime() >= end_time_ && end_time_ != 0) {
            end_time_ = 0;
            return true;
    }
        return false;
        
    }

    void Timer::Reset() {
        end_time_ = 0;
    }

} // namespace game
