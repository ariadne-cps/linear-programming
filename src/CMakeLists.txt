add_library(LINEAR_PROGRAMMING_SRC OBJECT
    linear_simplex_algorithm.cpp
    linear_interior_point_method.cpp
)

target_include_directories(LINEAR_PROGRAMMING_SRC PRIVATE
    "${PROJECT_SOURCE_DIR}/include/linear-programming"
)
