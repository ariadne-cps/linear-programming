find_library(ARIADNE_LINEAR_PROGRAMMING_LIBRARY NAMES ariadne-linear-programming)

find_package(Threads REQUIRED)

find_path(ARIADNE_LINEAR_PROGRAMMING_INCLUDE_DIR ariadne-linear-programming.hpp PATH_SUFFIXES ariadne-linear-programming)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AriadneLinearProgramming DEFAULT_MSG
  ARIADNE_LINEAR_PROGRAMMING_LIBRARY
  ARIADNE_LINEAR_PROGRAMMING_INCLUDE_DIR
)

if(AriadneLinearProgramming_FOUND)
  get_filename_component(ARIADNE_LINEAR_PROGRAMMING_INCLUDE_PARENT_DIR ${ARIADNE_LINEAR_PROGRAMMING_INCLUDE_DIR} DIRECTORY)
  set(ARIADNE_LINEAR_PROGRAMMING_INCLUDE_DIRS
    ${ARIADNE_LINEAR_PROGRAMMING_INCLUDE_PARENT_DIR}
    ${ARIADNE_LINEAR_PROGRAMMING_INCLUDE_DIR}
  )
  if(NOT TARGET AriadneLinearProgramming::ariadne-linear-programming)
    add_library(AriadneLinearProgramming::ariadne-linear-programming UNKNOWN IMPORTED)
    set_target_properties(AriadneLinearProgramming::ariadne-linear-programming PROPERTIES
      IMPORTED_LOCATION "${ARIADNE_LINEAR_PROGRAMMING_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${ARIADNE_LINEAR_PROGRAMMING_INCLUDE_DIRS}"
      INTERFACE_LINK_LIBRARIES "Threads::Threads"
    )
  endif()
endif()
