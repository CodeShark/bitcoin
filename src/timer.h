#ifndef _TIMER_H_
#define _TIMER_H_ 1

#include <boost/thread/thread_time.hpp>

class Timer;

/** A base class for jobs to be scheduled by the timer. */
class TimerJob {
protected:
    // Method to be executed. Override with an actual implementation
    virtual void Run() = 0;

    // Destructor. This will automatically unschedule if necessary.
    virtual ~TimerJob();

    // Constructor for an unscheduled job
    TimerJob();

    // Constructor for an prescheduled job
    TimerJob(const boost::system_time &time);
public:
    // Unschedule this job.
    // This will block while the job is running.
    bool Unschedule();

    // Schedule this job at the specified time.
    // This will unschedule if necessary first
    void Schedule(const boost::system_time &time);

    friend class Timer;

private:
    // these internal fields are only modified by Timer itself
    boost::system_time _time;
    bool _running;
    bool _scheduled;
};

/** Start the global timer thread */
void StartTimer();

/** Stop the global timer thread */
void StopTimer();

#endif
