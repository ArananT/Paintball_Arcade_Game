#ifndef TIMER_H
#define TIMER_H

namespace game {

    class Timer {
    public:
        Timer();
        ~Timer();
        void Start(float duration);
        bool Finished();
        void Reset();

    private:
        float end_time_; // Store the end time
    };

} // namespace game

#endif // TIMER_H
