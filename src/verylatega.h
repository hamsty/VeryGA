#include <Vveryga.h>
#include <verilated.h>

#if !defined(GDE_EXPORT)
#if defined(_WIN32)
#define GDE_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define GDE_EXPORT __attribute__((visibility("default")))
#else
#define GDE_EXPORT
#endif
#endif

class VerylateGA
{

public:
    GDE_EXPORT VerylateGA();
    GDE_EXPORT ~VerylateGA();
    int64_t GDE_EXPORT get_output();
    void GDE_EXPORT set_input(int64_t input);
    void GDE_EXPORT inc_time(int64_t time);
    void GDE_EXPORT eval();
    int64_t GDE_EXPORT get_time();
    void GDE_EXPORT stats_print_summary();

private:
    Vveryga *top;
    VerilatedContext *contextp;
};
