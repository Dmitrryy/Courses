#include <iostream>
#include <thread>
#include <string>
#include <functional>
#include <limits>
#include <map>
#include <vector>


#include "ThreadPool.hpp"
#include "Timer.hpp"


double Func(double x)
{
    return x * std::cos(1 / x);
}

double gA = 0.001;
double gB = 1.0;

// Simpson method
double Integrate(const std::function<double(double)> &func, double a, double b, double h)
{
    double I = 0;

    size_t N = (b - a) / h + 1;
    for (size_t i = 1; i < N - 1; ++i)
    {
        I += func(a + h * (i - 1)) + 4 * func(a + h * i) + func(a + h * (i + 1));
    }
    I *= h / 6.0;

    return I;
}

void threadTask(const std::function<double(double)> &func,
                double a, double b, double h, double eps,
                double &result, bool &norm)
{
    auto I1 = Integrate(func, a, b, h);
    auto I0 = Integrate(func, a, b, h * 2.0);

    norm = std::abs(I1 - I0) < eps;
    result = I1;
}

// одна задача это один отрезок с определенным шагом
struct Task
{
    double a = {};
    double b = {};
    double h = {};
};

struct ThreadResult
{
    double res = {};
    bool norm = false;
};


//=---------------------WARNING-------------------------
// this implementation is very bad 
// because threads are created and destroyed many times.
//
// Better look in main2.cpp
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("invalid arguments command line:\n argc = %d\n", argc);
        printf("Required 2: [num threads] [accuracy]");
        printf("We provide:\n");
        for (int i = 0; i < argc; i++)
        {
            printf("[%d] %s\n", i, argv[i]);
        }
        exit(1);
    }

    auto &&numThreads = std::stoll(argv[1]);
    auto &&epsilon = std::stod(argv[2]);

    // threads pool
    std::vector<std::thread> threads(numThreads);
    // tasks pool
    std::vector<Task> stackTasks;

    // рекомендуемый начальный шаг
    // (т.к. метод симпсона третьего порядка)
    double h = std::pow(epsilon, 1.0 / 3.0);
    stackTasks.push_back({gA, gB, h});

    Timer time;
    std::vector<ThreadResult> threadResults(numThreads);
    double result = 0;
    while (!stackTasks.empty())
    {
        auto curTask = stackTasks.back();
        stackTasks.pop_back();

        // запускаем каждый тред на свой кусок отрезка
        double offset = (curTask.b - curTask.a) / numThreads;
        for (size_t i = 0; i < numThreads; ++i)
        {
            double tmpA = curTask.a + offset * i;
            double tmpB = curTask.a + offset * (i + 1);
            threads.at(i) = std::thread(threadTask, Func, tmpA, tmpB, curTask.h, epsilon / 4,
                                        std::ref(threadResults.at(i).res), std::ref(threadResults.at(i).norm));
        }

        // собираем результаты вычислений
        bool prevIsNotNormal = false;
        for (size_t i = 0; i < numThreads; ++i)
        {
            threads.at(i).join();
            auto &&curRes = threadResults.at(i);

            if (curRes.norm)
            {
                result += curRes.res;
            }
            // интегрируем пока точность не станет требуемой
            else if (prevIsNotNormal)
            {
                // типо оптимизировал.
                // если предидущий отрезок тоже нужно пересчитывать, то просто расширяю его границы
                stackTasks.back().b += offset;
            }
            else
            {
                stackTasks.push_back({curTask.a + offset * i, curTask.a + offset * (i + 1), curTask.h / 2.0});
            }
            prevIsNotNormal = !curRes.norm;
        }
    }

    std::cout << "result = " << result << std::endl;
    std::cout << "Time = " << time.elapsed() << std::endl;

    return 0;
}