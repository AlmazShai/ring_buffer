# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = ./third_party/googletest/googletest

# Where to find user code.
INCLUDE_DIR = ./include
SRC_DIR = ./src
TEST_DIR = ./test

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

TARGET = gtest

# House-keeping build targets.

all : $(TARGET)


clean :
	rm -f gtest gtest.a gtest_main.a *.o

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

ring_buffer.o : $(INCLUDE_DIR)/ring_buffer.h $(SRC_DIR)/ring_buffer.c 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/ring_buffer.c

ringBufferTest.o : $(TEST_DIR)/ringBufferTest.cpp $(INCLUDE_DIR)/ring_buffer.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(TEST_DIR)/ringBufferTest.cpp

hr_gen.o : $(INCLUDE_DIR)/hr_gen.h $(SRC_DIR)/hr_gen.c 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/hr_gen.c

hrGenTest.o : $(TEST_DIR)/hrGenTest.cpp $(INCLUDE_DIR)/hr_gen.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(TEST_DIR)/hrGenTest.cpp

hr_ema.o : $(INCLUDE_DIR)/hr_ema.h $(SRC_DIR)/hr_ema.c 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/hr_ema.c

hrEmaTest.o : $(TEST_DIR)/hrEmaTest.cpp $(INCLUDE_DIR)/hr_ema.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(TEST_DIR)/hrEmaTest.cpp

gtest : ring_buffer.o ringBufferTest.o hr_gen.o hrGenTest.o hr_ema.o hrEmaTest.o gtest_main.a 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@