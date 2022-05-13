#pragma once

#include <iostream>
#include <thread>
#include <string>
#include <functional>
#include <limits>
#include <vector>
#include <queue>
#include <mutex>
#include <future>
#include <unordered_set>

class ThreadPool final
{
    // pool потоков, которые выполняют задачи из очереди
    std::vector<std::thread> m_threads;

    // очередь задач - хранит функцию(задачу), которую нужно исполнить и номер задачи
    std::queue<std::pair<std::future<void>, int64_t>> m_queue;
    std::mutex m_queueMutex;
    std::condition_variable m_queueCV;

    // помещаем в данный контейнер исполненные задачи
    std::unordered_set<int64_t> m_completedIDs;
    std::condition_variable m_cvCompleted;
    std::mutex m_mtxCompleted;

    // флаг завершения работы ThreadPool
    std::atomic<bool> m_quite{false};

    // переменная хранящая id который будет выдан следующей задаче
    std::atomic<int64_t> m_lastID = 0;

public:
    ThreadPool(size_t numThreads)
    {
        m_threads.reserve(numThreads);
        for (size_t i = 0; i < numThreads; ++i)
        {
            m_threads.emplace_back(&ThreadPool::run, this);
        }
    }

    ~ThreadPool()
    {
        // можно добавить wait_all() если нужно дождаться всех задачь перед удалением
        m_quite = true;
        for (uint32_t i = 0; i < m_threads.size(); ++i)
        {
            m_queueCV.notify_all();
            m_threads[i].join();
        }
    }

public:
    void run()
    {
        while (!m_quite)
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);

            m_queueCV.wait(lock, [this]() -> bool
                           { return !m_queue.empty() || m_quite; });

            if (!m_queue.empty())
            {
                auto elem = std::move(m_queue.front());
                m_queue.pop();
                lock.unlock();

                elem.first.get();

                std::lock_guard<std::mutex> lock(m_mtxCompleted);

                m_completedIDs.insert(elem.second);

                // делаем notify, чтобы разбудить потоки
                m_cvCompleted.notify_all();
            }
        }
    }

    template <typename Func, typename... Args>
    size_t addTask(const Func &task, Args &&...args)
    {
        auto taskID = m_lastID++;

        std::lock_guard<std::mutex> q_lock(m_queueMutex);
        m_queue.emplace(std::async(std::launch::deferred, task, args...), taskID);

        m_queueCV.notify_one();
        return taskID;
    }

    void wait(int64_t task_id)
    {
        std::unique_lock<std::mutex> lock(m_mtxCompleted);

        // ожидаем вызова notify в функции run (сработает после завершения задачи)
        m_cvCompleted.wait(lock, [this, task_id]() -> bool
                           { return m_completedIDs.find(task_id) != m_completedIDs.end(); });
    }

    void waitAll()
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);

        // ожидаем вызова notify в функции run (сработает после завершения задачи)
        m_cvCompleted.wait(lock, [this]() -> bool
                           {
        std::lock_guard<std::mutex> task_lock(m_mtxCompleted);
        return m_queue.empty() && m_lastID == m_completedIDs.size(); });
    }

    bool calculated(int64_t task_id)
    {
        std::lock_guard<std::mutex> lock(m_mtxCompleted);
        if (m_completedIDs.find(task_id) != m_completedIDs.end())
        {
            return true;
        }
        return false;
    }

    size_t numThreads() const { return m_threads.size(); }
    size_t lastTaskID() const { return m_lastID; }
};