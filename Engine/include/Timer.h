#ifndef TIMER_H
#define TIMER_H


class Timer
{
    public:
        Timer();
        virtual ~Timer();

        void Start();
        void Reset();

        long getElapsedMilliseconds();
    protected:
    private:
};

#endif // TIMER_H
