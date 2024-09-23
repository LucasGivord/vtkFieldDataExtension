#include "vtkAddFieldData.h"

#include <vtkDoubleArray.h>
#include <vtkFieldData.h>
#include <vtkIntArray.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkNew.h>
#include <vtkPolyData.h>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

#include <vector>

//------------------------------------------------------------------------------
TEST_CASE("AddFieldData can only take a dataset as input", "[Input - vtkDataObject]")
{
  vtkNew<vtkAddFieldData> addFieldData;

  SECTION("a vtkPolyData should be correct.")
  {
    vtkNew<vtkPolyData> polydata;
    addFieldData->SetInputData(polydata);
    addFieldData->Update();

    auto *outputOfFilter = vtkPolyData::SafeDownCast(addFieldData->GetOutput());
    REQUIRE(outputOfFilter != nullptr);
  }
  SECTION("a vtkMultiBlockDataSet should be incorrect.")
  {
    vtkNew<vtkMultiBlockDataSet> multiBlock;
    addFieldData->SetInputData(multiBlock);
    addFieldData->Update();

    auto *outputOfFilter = vtkMultiBlockDataSet::SafeDownCast(addFieldData->GetOutput());
    REQUIRE(outputOfFilter == nullptr);
  }
}

//------------------------------------------------------------------------------
TEST_CASE("AddFieldData can only take a valid array name", "[Input - ArrayName]")
{
  vtkNew<vtkAddFieldData> addFieldData;

  SECTION("the array name 'Normals' is valid.")
  {
    std::string arrayName = "Normals";
    addFieldData->SetArrayName(arrayName);

    REQUIRE(addFieldData->GetArrayName() == "Normals");
  }
  SECTION("a valid array name cannot be prefixed by 'vtk'.")
  {
    std::string arrayName = "vtkOriginalCellIds";
    addFieldData->SetArrayName(arrayName);

    REQUIRE(addFieldData->GetArrayName() != "vtkOriginalCellIds");
    REQUIRE(addFieldData->GetArrayName() == "");
  }
}

//------------------------------------------------------------------------------
TEST_CASE("AddFieldData take in input an array", "[Input - Array]")
{
  vtkNew<vtkAddFieldData> addFieldData;

  SECTION("the array can be an 1D array.")
  {
    std::vector<double> array = {1};
    addFieldData->SetArray(array);

    REQUIRE(addFieldData->GetArray() == std::vector<std::vector<double>>{{1}});
  }
  SECTION("the array can be a 2D array.")
  {
    std::vector<std::vector<double>> array = {{1, 1}};
    addFieldData->SetArray(array);

    REQUIRE(addFieldData->GetArray() == std::vector<std::vector<double>>{{1, 1}});
  }
  SECTION("an array with varying number of components is not allowed.")
  {
    std::vector<std::vector<double>> array = {{1, 1}, {1}};
    addFieldData->SetArray(array);

    REQUIRE(addFieldData->GetArray() == std::vector<std::vector<double>>{});
  }
}

//------------------------------------------------------------------------------
int UnitTestsAddFieldData(int argc, char *argv[])
{
  int result = Catch::Session().run(argc, argv);

  return result;
}
