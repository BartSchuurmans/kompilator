set(CMAKE_CXX_STANDARD 11)
list(APPEND CMAKE_CXX_FLAGS "-Wall -Wno-unused-private-field")
add_executable(kompilator main.cpp lexeme.cpp scanner.cpp)
