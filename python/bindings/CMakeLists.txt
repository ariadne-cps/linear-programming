include_directories(${Python3_INCLUDE_DIRS})

get_target_property(PYTHON_COMMON_SOURCE_DIR pyariadne-python-common SOURCE_DIR)

add_library(pyariadne-linear_programming-submodule-obj OBJECT
    linear_programming_submodule.cpp
)
target_include_directories(pyariadne-linear_programming-submodule-obj PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${PYTHON_COMMON_SOURCE_DIR}/include"
    $<TARGET_PROPERTY:pyariadne-algebra,INTERFACE_INCLUDE_DIRECTORIES>
)
target_link_libraries(pyariadne-linear_programming-submodule-obj PRIVATE
    pyariadne-python-common
    pybind11::module
    linear_programming
)
set_target_properties(pyariadne-linear_programming-submodule-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
if(MSVC)
    target_compile_options(pyariadne-linear_programming-submodule-obj PRIVATE
        /bigobj
        /wd4702
    )
endif()

add_library(pyariadne-linear_programming INTERFACE)
target_sources(pyariadne-linear_programming INTERFACE
    $<TARGET_OBJECTS:pyariadne-linear_programming-submodule-obj>
)
target_include_directories(pyariadne-linear_programming INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<TARGET_PROPERTY:pyariadne-algebra,INTERFACE_INCLUDE_DIRECTORIES>
)
target_link_libraries(pyariadne-linear_programming INTERFACE
    pyariadne-algebra
)

ariadne_add_python_module(
    NAME linear_programming
    MODULE_SOURCE linear_programming_module.cpp
)
