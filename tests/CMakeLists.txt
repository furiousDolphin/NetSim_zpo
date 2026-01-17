include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)



add_executable(unit_tests 
    main_gtest.cpp 
    test_factory.cpp
    test_factory_io.cpp
    test_nodes.cpp
    test_package.cpp
    test_reports.cpp
    test_simulate.cpp
    test_storage_types.cpp
)

target_compile_definitions(unit_tests PRIVATE EXERCISE_ID=3)

target_include_directories(unit_tests PRIVATE mocks)

target_link_libraries(unit_tests 
    GTest::gtest_main 
    GTest::gmock
    NetSimLib
)




include(GoogleTest)
gtest_discover_tests(unit_tests)