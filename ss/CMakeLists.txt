if(NOT TARGET algebra)
    ensure_submodule_initialized(algebra)
    add_subdirectory(algebra)
endif()
