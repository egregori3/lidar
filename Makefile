RESULT = lidar
DRIVER_SRCS = CYdLidar.cpp ydlidar_driver.cpp serial.cpp unix_serial.cpp unix_timer.cpp
TEST_SRCS = main.cpp
OBJS = $(DRIVER_SRCS:.cpp=.o)

.PHONY: clean

test:
	g++ -I./include -std=c++11 -lrt -pthread $(TEST_SRCS) $(DRIVER_SRCS) -o $(RESULT)

driver:
	g++ -I./include -std=c++11 -c $(DRIVER_SRCS)
	ar -rc $(RESULT)_driver.a $(OBJS)
	rm *.o || true

clean:
	rm $(RESULT) || true
	rm *.o || true
	rm $(RESULT)_driver.a
