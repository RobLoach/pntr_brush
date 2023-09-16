cmake_minimum_required(VERSION 3.11)
project(pntr_brush
    DESCRIPTION "pntr_brush: Brush API for pntr"
    HOMEPAGE_URL "https://github.com/robloach/pntr_brush"
    VERSION 0.0.1
    LANGUAGES C
)

# pntr_build
add_library(pntr_brush INTERFACE)
target_include_directories(pntr_brush INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(PNTR_BRUSH_IS_MAIN TRUE)
else()
    set(PNTR_BRUSH_IS_MAIN FALSE)
endif()

option(PNTR_BRUSH_BUILD_TESTS "Build Tests" ${PNTR_BRUSH_IS_MAIN})

# Tests
if (PNTR_BRUSH_BUILD_TESTS)
    include(CTest)
    enable_testing()
    if (BUILD_TESTING)
        set(CTEST_CUSTOM_TESTS_IGNORE
            pkg-config--static
        )
        # Always print verbose output when tests fail if run using `make test`.
        list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
        add_subdirectory(test)
    endif()
endif()
