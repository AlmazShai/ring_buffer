#include "ring_buffer.h"
#include "hr_ema.h"
#include "hr_gen.h"

#include <iostream>
#include <thread>
#include <exception>
#include <stddef.h>
#include <stdint.h>

namespace
{

void handleRetCode(rb_ret_t code)
{
    if (code != RB_OK)
    {
        throw std::runtime_error("Unexpected error: " + code);
    }
}

int runGenerate(ring_buffer_t* rb)
{
    while (1)
    {
        uint8_t val = hr_gen_random();
        if (rb_is_full(rb) == true)
        {
            handleRetCode(rb_remove(rb));
        }

        // Add new value to buffer and recalculate EMA
        handleRetCode(rb_add(rb, &val));
        uint8_t ema = hr_ema_calc(rb);

        std::cout << "EMA heart rate: " << std::to_string(ema) << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

} // namespace

int main(int argc, char* argv[])
{
    size_t bufferCapacity = 10;
    if (argc > 1)
    {
        bufferCapacity = std::stoul(argv[0]);
    }

    const size_t elSize     = sizeof(uint8_t);
    const size_t bufferSize = (bufferCapacity + 1) * elSize;
    char         buff[bufferSize];

    ring_buffer_t rb;
    rb_ret_t      ret = rb_init(&rb, buff, bufferSize, elSize);
    if (ret != RB_OK)
    {
        std::cout << "Failed to initialize ring buffer: " << ret << std::endl;
        return -1;
    }

    try
    {
        runGenerate(&rb);
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception thrown: " << e.what();
        return -1;
    }
    return 0;
}