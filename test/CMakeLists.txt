
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/test/bin)

MESSAGE(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")


add_executable(log_test log_test.cpp)
target_link_libraries(log_test mp4)

add_executable(utils_test utils_test.cpp)
target_link_libraries(utils_test mp4)

add_executable(parser parser.cpp)
target_link_libraries(parser mp4)
