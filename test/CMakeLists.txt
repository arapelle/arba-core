add_cpp_library_tests(SHARED ${PROJECT_NAME}
                      STATIC ${PROJECT_NAME}-static
                      SOURCES
                      uuid_tests.cpp
                      hash_tests.cpp
                      )
