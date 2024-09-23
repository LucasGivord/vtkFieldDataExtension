set(tests
    UnitTestsAddFieldData.cxx
    IntegrationTestsAddFieldData.cxx
)

vtk_add_test_cxx(vtkAddFieldDataTests no_data_tests
  NO_DATA NO_VALID NO_OUTPUT
  ${tests}
)

vtk_test_cxx_executable(vtkAddFieldDataTests no_data_tests)

# Link the test executable to our test suite framework
target_link_libraries(vtkAddFieldDataTests
    PRIVATE
        Catch2::Catch2)
