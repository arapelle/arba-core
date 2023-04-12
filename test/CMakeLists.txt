add_cpp_library_tests(SHARED ${PROJECT_NAME}
                      STATIC ${PROJECT_NAME}-static
                      SOURCES
                      uuid_tests.cpp
                      hash_tests.cpp
                      byte_swap_tests.cpp
                      htow_tests.cpp
                      intrusive_ptr_tests.cpp
                      string_tokenizer_tests.cpp
                      string_tests.cpp
                      type_helpers_tests.cpp
                      copy_stream_tests.cpp
                      check_file_tests.cpp
                      open_file_tests.cpp
                      )
