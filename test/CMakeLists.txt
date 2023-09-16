# CMAKE Modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

# Resources
# file(GLOB resources resources/*)
# set(examples_resources)
# list(APPEND examples_resources ${resources})
# file(COPY ${examples_resources} DESTINATION "resources/")

find_package(pntr)

add_executable(pntr_brush_test
    pntr_brush_test.c
)

target_compile_options(pntr_brush_test PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion)
target_link_libraries(pntr_brush_test PUBLIC
    pntr
)
set_property(TARGET pntr_brush_test PROPERTY C_STANDARD 99)

list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(NAME pntr_brush_test COMMAND pntr_brush_test)
