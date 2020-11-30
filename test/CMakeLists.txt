add_cpp_library_tests(SHARED ${PROJECT_NAME}
                      STATIC ${PROJECT_NAME}-static
                      SOURCES
                      uuid_tests.cpp
                      hash_tests.cpp
                      byte_swap_tests.cpp
                      htow_tests.cpp
                      intrusive_ptr_tests.cpp
                      )
