#include "SimpleThreadPool.hpp"
#include <algorithm>
#include <exception>
#include <mutex>
#include <queue>
#include <fmt/core.h>
#include <cstdio>

namespace heart {
SimpleThreadPool::SimpleThreadPool(unsigned int num_threads)
    : is_stopped_(false), num_threads_(num_threads) {

    // define each thread as a worker
    auto thread_work = [this]() {
        std::function<void(void)> task;
        while (true) {
            {
                std::unique_lock unique_lock(mtx);

                cv.wait(unique_lock,
                        [this]() { return is_stopped_ || !tasks.empty(); });

                if (is_stopped_)
                    return; // terminate the thread

                // transfer ownership
                task = std::move(tasks.front());
                tasks.pop();
            }

            // do the work
            try {
                task();
            } catch (std::exception& e) {
                fmt::print(stderr, "Fialed to run the task\n  Exeception occurred: {}\n", e.what());
            }
        }
    };

    // create the threads
    for (unsigned int i = 0; i < num_threads_; ++i) {
        threads.emplace_back(thread_work);
    }
}

SimpleThreadPool::~SimpleThreadPool() {
    {
        std::lock_guard lock_guard(mtx);

        is_stopped_ = true;
    }
    cv.notify_all(); // notify all threads to stop

    // join all the threads
    std::for_each(threads.begin(), threads.end(),
                  [](std::thread &t) { t.join(); });
}
} // namespace heart
