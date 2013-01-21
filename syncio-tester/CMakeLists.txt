enable_language(CXX)

# collect all the source files
add_executable(synciotest phastaiotest.cpp)
target_link_libraries(synciotest phastaIO)
