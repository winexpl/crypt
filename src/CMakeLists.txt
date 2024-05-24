project(crypt)

add_executable(main main.cpp)
target_link_libraries(main crypt)
target_link_libraries(main m)