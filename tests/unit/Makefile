CXX ?= g++
CXXFLAGS ?= -std=c++14 -Wall -Wextra -I../.. -I.
TARGET := test_sensor_safety

.PHONY: all test clean
all: $(TARGET)

$(TARGET): test_sensor_safety.cpp ../../sensor_validation.cpp ../../sensor_validation.h ../../timing_utils.h ../../safe_string.h ../../modbus_context.h
	$(CXX) $(CXXFLAGS) -o $@ test_sensor_safety.cpp ../../sensor_validation.cpp

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
