CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2
TARGET   := lms
SRCS     := main.cpp Book.cpp Student.cpp Library.cpp
OBJS     := $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: all
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) books.dat students.dat
