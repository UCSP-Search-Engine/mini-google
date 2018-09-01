main:
	g++ -o engine core/console.cpp core/Suffix-Tree.hpp core/parse/tools.hpp -DENABLE_OPENSSL=0 -DENABLE_COMPRESSION=0 -std=c++11 -O3
exec:
	./engine