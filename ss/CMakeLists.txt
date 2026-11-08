if(NOT TARGET algebra)
    ariadne_add_submodule(algebra)
endif()

# Testing is used only by the test executables.
if(NOT TARGET logging)
    ariadne_add_submodule(logging)
endif()

ariadne_setup_testing()
