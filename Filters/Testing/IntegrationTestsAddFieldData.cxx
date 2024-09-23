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

SCENARIO("vtkAddFieldData can append an array of double to a dataset")
{

  GIVEN("A vtkPolyData with some field data to the vtkAddFieldData")
  {
    vtkNew<vtkPolyData> polyData;

    vtkNew<vtkIntArray> timestep;
    timestep->SetName("TimeStep");
    timestep->SetNumberOfValues(1);
    timestep->SetValue(0, 1);
    polyData->GetFieldData()->AddArray(timestep);

    vtkNew<vtkDoubleArray> timevalue;
    timevalue->SetName("TimeValue");
    timevalue->SetNumberOfValues(1);
    timevalue->SetValue(0, 0.5);
    polyData->GetFieldData()->AddArray(timevalue);

    vtkNew<vtkAddFieldData> addFieldData;
    addFieldData->SetInputData(polyData);

    WHEN("the given array name already exist")
    {
      std::string name = "TimeValue";
      std::vector<double> newTimeValue = {0.75};

      addFieldData->SetArrayName(name);
      addFieldData->SetArray(newTimeValue);

      THEN("the array will be override.")
      {
        addFieldData->Update();
        auto *array = addFieldData->GetOutput()->GetFieldData()->GetArray(name.c_str());

        REQUIRE(array->GetComponent(0, 0) == 0.75);
      }
    }

    WHEN("the given array name already exist for a non double array")
    {
      std::string name = "TimeStep";
      std::vector<double> newTimeStep = {1.5};

      addFieldData->SetArrayName(name);
      addFieldData->SetArray(newTimeStep);

      THEN("the array will also be override.")
      {
        addFieldData->Update();
        auto *array = addFieldData->GetOutput()->GetFieldData()->GetArray(name.c_str());

        REQUIRE(array->GetComponent(0, 0) == 1.5);
      }
    }

    WHEN("the given array is a 2D array")
    {
      std::string name = "Metrics";
      std::vector<std::vector<double>> metrics = {{0, 1}, {1.5, 2}, {7.8, 15000}};

      addFieldData->SetArrayName(name);
      addFieldData->SetArray(metrics);

      THEN("this array will be correctly set to the polydata.")
      {
        addFieldData->Update();
        auto *array = addFieldData->GetOutput()->GetFieldData()->GetArray(name.c_str());

        REQUIRE(array->GetComponent(2, 0) == 7.8);
        REQUIRE(array->GetComponent(2, 1) == 15000);
      }
    }
  }
}

//------------------------------------------------------------------------------
int IntegrationTestsAddFieldData(int argc, char *argv[])
{
  int result = Catch::Session().run(argc, argv);

  return result;
}
