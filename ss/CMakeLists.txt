if(NOT TARGET algebra)
    ariadne_add_submodule(algebra)
endif()

# Testing is used only by the test executables.
ariadne_setup_testing()
