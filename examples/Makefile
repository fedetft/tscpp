
CXX = g++
CXXFLAGS = -std=c++11 -g -O0 -fsanitize=address -Wall -I../..

all:
	$(CXX) $(CXXFLAGS) 1_stream_known.cpp    ../stream.cpp -o 1_stream_known
	$(CXX) $(CXXFLAGS) 2_stream_unknown.cpp  ../stream.cpp -o 2_stream_unknown
	$(CXX) $(CXXFLAGS) 3_buffer_known.cpp    ../buffer.cpp -o 3_buffer_known
	$(CXX) $(CXXFLAGS) 4_buffer_unknown.cpp  ../buffer.cpp -o 4_buffer_unknown
	$(CXX) $(CXXFLAGS) 5_stream_failtest.cpp ../stream.cpp -o 5_stream_failtest
	$(CXX) $(CXXFLAGS) 6_buffer_failtest.cpp ../buffer.cpp -o 6_buffer_failtest
	./1_stream_known
	./2_stream_unknown
	./3_buffer_known
	./4_buffer_unknown
	./5_stream_failtest
	./6_buffer_failtest

clean:
	rm -f 1_stream_known 2_stream_unknown 3_buffer_known 4_buffer_unknown \
	      5_stream_failtest 6_buffer_failtest
