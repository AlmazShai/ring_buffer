# Random heart rate generator
This project implements a random heart rate generator which generate heart rate in diapason from 44 to 185 beats per second. Additionally this hear rate is filtered using Exponential Moving Average. This project consist of three main elements:
1. [Ring buffer](#ring-buffer)
2. [Heart rate generator](#heart-rate-generator)
3. [Heart rate EMA calculator](#heart-rate-exponential-moving-average-calculation)
### Ring buffer
The ring buffer uses a single fixed-size buffer that can be allocated on the stack or in the heap and passed during initialization.
The ring buffer is implemented using two pointers: head and tail. This ring buffer implementation does not allow overwriting data, if the buffer is full, the oldest element must be removed before adding a new element.  
This ring buffer implementation has the following features:
- A lightweight implementation that uses two pointers, so that none of the old element are copied on new element insertion.
- Can be used with any type of data structure, such that different data types may be combined into structure e.g [hearRate, temperature, timestamp]
- A read iterator is provided, allowing all values to be read one by one. This iterator provide safety and keep reader independent form the ring buffer implementation.   
#### Usage example
```c
    const size_t elSize   = sizeof(int);
    const size_t cap      = 5;
    const size_t buffSize = (cap + 1) * elSize;
    char         buff[buffSize];

    // initialize buffer
    ring_buffer_t rb;
    rb_init(&rb, buff, buffSize, elSize);

    // Add new value
    int val = 142;
    rb_add(&rb, &val);
 
    // Initialize iterator for reading
    rb_it_t it;
    rb_init_read_it(&rb, &it);

    // Read element
    int readVal;
    rb_get_next_val(&it, &readVal);

    // Remove oldest element
    rb_remove(&it);
```
### Heart rate generator
The heartbeat generator is a trivial random number generator that generates numbers from 44 to 185 using a `rand()` function from `stdlib.h`. This component is only responsible for generating random heartbeats and has nothing to do with the other components, so it is implemented in a separate file.
### Heart rate Exponential Moving Average(EMA) calculation
This component smooth the heart rate using EMA on given set of data using following formula:   
s(t) = αx(t) + (1-α)st-1   
where:
- s(t) is the smoothed value at time t,
- x(t) is the observed value at time t,
- st-1 is the previous smoothed statistic, and
- α is the smoothing parameter between 0 and 1.   
   
With each new function call, this algorithm retrieves all existing data from the ring buffer and performs the calculation. This algorithm is relatively slow because it iterate over the all elements every time, but it allows us to calculate the smoothed value for a certain window size.
## Repo structure
```
├── docs            # Project documentation        
├── include         # Public header files
├── src             # Library source files
├── test            # Test source files
└── third_party     # Third party sources
    └── googletest  # GoogleTest testing framework
```
## How to build
The build system is based on a Makefile that has two targets: 
- `main` - main application that performs random heartbeat generation and prints to console output
- `gtest` - a test application that runs a test for each component.   
To build all targets at once, simply run the command:
```
make all
```
To build a specific target, run the following:
```
make <target_name>
```
To clear all build results:
```
make clean
```
## Running
To start a heart beat generator run the following command:
```
./main [average window]
```
Average window is a size of window that is used for calculation of EMA, this parameter is optional and by default is 10.   
This command will start a random heart rate generation with period of 1 second and print filtered heart rate value to the consol.
## Testing
To run all google tests use the following command:
```
./gtest
```


