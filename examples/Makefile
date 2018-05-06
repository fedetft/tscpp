
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall -I..

all:
	$(CXX) $(CXXFLAGS) 1_stream_known.cpp   ../tscpp.cpp -o 1_stream_known
	$(CXX) $(CXXFLAGS) 3_buffer_known.cpp   ../tscpp.cpp -o 3_buffer_known
	$(CXX) $(CXXFLAGS) 4_buffer_unknown.cpp ../tscpp.cpp -o 4_buffer_unknown
	./1_stream_known
	./3_buffer_known
	./4_buffer_unknown

clean:
	rm -f 1_stream_known 3_buffer_known 4_buffer_unknown
