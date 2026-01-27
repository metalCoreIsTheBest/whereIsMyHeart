#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>
#include <stdexcept>
#include <memory>


namespace heart {
class SimpleThreadPool {
  private:
    // for synchronization
    std::mutex mtx;
    std::condition_variable cv;
    bool is_stopped_;

    // the threads
    std::vector<std::thread> threads;
    // the tasks
    std::queue<std::function<void(void)>> tasks;

    // management
    const unsigned int num_threads_;

  public:
    explicit SimpleThreadPool(unsigned int num_threads);
    ~SimpleThreadPool();

    SimpleThreadPool &operator=(const SimpleThreadPool &) = delete;
    SimpleThreadPool(const SimpleThreadPool &) = delete;

    template <typename Func, typename... Args,
              typename RtrnType = typename std::invoke_result_t<Func, Args...>>
    auto enqueue(Func &&f, Args &&...args) -> std::future<RtrnType> {
        {
            std::lock_guard lock_guard(mtx);

            if (is_stopped_) {
                throw std::runtime_error("SimpleThreadPool is stopped, cannot "
                                        "enqueue more tasks into it!");
            }
        }

        using PackagedTaskType = std::packaged_task<RtrnType(void)>;

        auto bound_task = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);

        // Create packaged_task - this works for both void and non-void return types
        PackagedTaskType task(std::move(bound_task));

        // Get the future before moving the task
        auto future_ret = task.get_future();

        // Create a shared_ptr to manage the task lifetime
        auto task_ptr = std::make_shared<PackagedTaskType>(std::move(task));

        // Lambda that captures the shared_ptr by value (not by reference!)
        auto wrapper = [task_ptr]() {
            (*task_ptr)();
        };

        {
            std::lock_guard lock_guard(mtx);
            tasks.emplace(std::move(wrapper));
        }
        cv.notify_one();

        return future_ret;
    }
};
} // namespace heart
