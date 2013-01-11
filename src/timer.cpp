#include "timer.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>

#include <set>
#include <assert.h>

typedef std::pair<boost::system_time, TimerJob*> TimerRef;

class Timer {
    typedef std::set<TimerRef> set_t;

    boost::mutex mutex;
    set_t jobs;
    boost::condition_variable condTimer;
    boost::condition_variable condExit;
    boost::condition_variable condJobDone;
    bool fExit;
    bool fRunning;

    // @pre: job is not running or scheduled
    // @pre: lock on mutex is acquired
    void Schedule_(TimerJob *job, const boost::system_time &time) {
        job->_scheduled = true;
        job->_time = time;
        std::pair<set_t::iterator,bool> ret = jobs.insert(std::make_pair(time, job));
        if (ret.first == jobs.begin())
            condTimer.notify_one();
    }

    // @pre: lock on mutex is acquired in lock
    // @return: whether the job needed unscheduling
    bool Unschedule_(TimerJob *job, boost::unique_lock<boost::mutex> &lock) {
        while (job->_running)
            condJobDone.wait(lock);

        if (job->_scheduled) {
            TimerRef ref(job->_time, job);
            jobs.erase(ref);
            return true;
        }
        return false;
    }

    // @pre: no job is running
    // @pre: lock on mutex is acquired in lock
    TimerJob *Wait_(boost::unique_lock<boost::mutex> &lock) {
        while (fExit || jobs.empty() || boost::get_system_time() < jobs.begin()->first) {
            if (fExit)
                return NULL;
            if (jobs.empty())
                condTimer.wait(lock);
            else
                condTimer.timed_wait(lock, jobs.begin()->first);
        }
        TimerJob *job = jobs.begin()->second;
        assert(job->_scheduled == true);
        assert(job->_running == false);
        job->_scheduled = false;
        job->_running = true;
        jobs.erase(jobs.begin());
        return job;
    }

    // @pre: lock on mutex is acquired
    // @pre: job is running
    void Done_(TimerJob *job) {
        assert(job->_running == true);
        job->_running = false;
        condJobDone.notify_one();
    }

public:
    void Schedule(TimerJob *job, const boost::system_time &time) {
        boost::unique_lock<boost::mutex> lock(mutex);
        Unschedule_(job, lock);
        Schedule_(job, time);
    }

    bool Unschedule(TimerJob *job) {
        boost::unique_lock<boost::mutex> lock(mutex);
        return Unschedule_(job, lock);
    }

    void operator()() {
        boost::unique_lock<boost::mutex> lock(mutex);
        if (fRunning)
            return;
        fRunning = true;
        while (TimerJob *job = Wait_(lock)) {
            lock.unlock();
            try {
                job->Run();
            } catch(...) {
            }
            lock.lock();
            Done_(job);
        } while(true);
        fRunning = false;
        condExit.notify_all();
    }

    void Exit() {
        boost::unique_lock<boost::mutex> lock(mutex);

        fExit = true;
        condTimer.notify_all();
        while (fRunning)
            condExit.wait(lock);
    }
};

static Timer timer;

void StartTimer() {
    boost::thread(timer);
}

void StopTimer() {
    timer.Exit();
}

void TimerJob::Schedule(const boost::system_time &time) {
    timer.Schedule(this, time);
}

bool TimerJob::Unschedule() {
    return timer.Unschedule(this);
}

TimerJob::~TimerJob() {
    Unschedule();
}

TimerJob::TimerJob() : _time(boost::date_time::not_a_date_time), _running(false), _scheduled(false) {}

TimerJob::TimerJob(const boost::system_time &time) : _time(boost::date_time::not_a_date_time), _running(false), _scheduled(false) {
    Schedule(time);
}
