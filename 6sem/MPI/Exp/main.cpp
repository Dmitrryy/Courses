//
// Created by dadro on 14.03.2022.
//

#include "mpi.h"
#include <iostream>

#include <boost/multiprecision/mpfr.hpp>
#include <boost/multiprecision/cpp_int.hpp>

namespace mp = boost::multiprecision;
typedef mp::number<mp::cpp_int_backend<> >  my_float;

constexpr char ref_e[] = "2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274274663919320030599218174135966290435729003342952605956307381323286279434907632338298807531952510190115738341879307021540891499348841675092447614606680822648001684774118537423454424371075390777449920695517027618386062613313845830007520449338265602976067371132007093287091274437470472306969772093101416928368190255151086574637721112523897844250569536967707854499699679468644549059879316368892300987931277361782154249992295763514822082698951936680331825288693984964651058209392398294887933203625094431173012381970684161403970198376793206832823764648042953118023287825098194558153017567173613320698112509961818815930416903515988885193458072738667385894228792284998920868058257492796104841984443634632449684875602336248270419786232090021609902353043699418491463140934317381436405462531520961836908887070167683964243781405927145635490613031072085103837505101157477041718986106873969655212671546889570350354";


my_float Fact(uint32_t n) {
    static std::vector<my_float> results = {1, 1};

    for(uint32_t i = results.size(); i <= n; ++i)
        results.emplace_back(results.back() * i);

    return results.at(n);
}


void sum_mpi_string(char *invec, char *inoutvec, int *len, MPI_Datatype *datatype) {
    my_float a(invec);
    my_float b(inoutvec);
    my_float result = a + b;

    auto &&str = result.str();
    std::copy(str.begin(), str.end(), inoutvec);
}


int main(int argc, char *argv[])
{
    if (auto rc = MPI_Init(&argc, &argv)) {
        std::cout << "Ошибка запуска, выполнение остановлено " << std::endl;
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    if (argc != 3) {
        printf("invalid arguments command line:\n argc = %d\n", argc);
        printf("Required 2: [terms in a row] [desired accuracy]");
        printf("We provide:\n");
        for (int i = 0; i < argc; i++)
        {
            printf ("[%d] %s\n", i, argv[i]);
        }
        MPI_Abort(MPI_COMM_WORLD, 404);
    }

    char* cur_end1 = nullptr, *cur_end2 = nullptr;
    int n = (int) strtol(argv[1], &cur_end1, 10);
    int accuracy = (int) strtol(argv[2], &cur_end2, 10);
    if (errno || *cur_end1 != 0 || *cur_end2 != 0 || n <= 0) {
        std::cout << "incorrect numbers: \n[1] " << argv[1] << " (interpret as: " << n << " (int))\n";
        std::cout << "[2] " << argv[2] << " (interpret as: "<< accuracy << "(int))" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, errno);
    }

    int num_processes = 0;
    int proc_id = 0;
    MPI_Comm_size(MPI_COMM_WORLD,&num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD,&proc_id);

    double startTime = {};
    if (proc_id == 0) {
        startTime = MPI_Wtime();
    }

    const my_float zeros("1" + std::string(accuracy, '0'));
    my_float local_sum;
    for (auto i = proc_id; i <= n; i += num_processes) {
        my_float q, r;
        mp::divide_qr(zeros, Fact(i), q, r);
        if (q == 0)
            break;
        local_sum += q;
    }

    MPI_Op op = {};
    MPI_Op_create( (MPI_User_function *) sum_mpi_string, 1, &op );

    // magic 1.5 is chosen so as not to go out of buffer
    size_t buffer_size = accuracy * 1.5;
    std::string result;
    if(proc_id == 0)
        result.resize(buffer_size);
    MPI_Reduce(local_sum.str().data(), result.data(), buffer_size, MPI_CHAR, op, 0, MPI_COMM_WORLD);

    auto endTime = MPI_Wtime();
    // provide the result
    if (proc_id == 0) {
        my_float q, r;
        my_float res(result.data());
        mp::divide_qr(res, zeros, q, r);

        size_t matches = 1;
        while (result[matches] == ref_e[matches + 1])
            matches++;

        std::cout << "Result: "  << q << '.' << r << std::endl;
        std::cout << "Ref e:  "  << ref_e << std::endl;
        std::cout << "Matches: " << matches << std::endl;
        std::cout << "Time: "    << (endTime - startTime) * 1000 << std::endl;
    }

    MPI_Finalize();
    return 0;
}