CX = g++
CXFLAGS = -g -Wall `pkg-config opencv4 --cflags`
LDFLAGS = `pkg-config opencv4 --libs`
TARGET = linetracer
OBJS = main.o vision.o

$(TARGET) : $(OBJS)
	$(CX) -o $(TARGET) $(OBJS) $(LDFLAGS)

main.o : main.cpp 
	$(CX) $(CXFLAGS) -c main.cpp

vision.o : vision.hpp vision.cpp 
	$(CX) $(CXFLAGS) -c vision.cpp

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)