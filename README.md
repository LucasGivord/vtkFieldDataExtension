# VTK Field Data module

An external module useful to add field data to a given dataset.

## Build steps

### Dependencies

- CMake 3.26.0 or higher
- VTK 9.2.6 or higher

If you want to build the tests too, enable the `BUILD_TESTING`, note that it will fetch automatically Catch2 (v3.6.0).

### Configure

On linux:

```bash
mkdir -p build
cd build
cmake -GNinja -DCMAKE_PREFIX_PATH=/path/to/vtk -DBUILD_TESTING=ON ..
ninja
```

## Example

How to append an array of double as field data array?

```c++

#include <vtkAddFieldData.h>

// ...

// your dataset
vtkNew<vtkPolyData> polyData;

// your array of double which represents the timestep
std::vector<double> myArrayOfTimeStep = {0,1,2,3,4,5,6,7,8,9};

// Create a new vtkAddFieldData and set each entry points.
vtkNew<vtkAddFieldData> addFieldData;
addFieldData->SetInputData(polyData);
addFieldData->SetArrayName("Timesteps");
addFieldData->SetArray(myArrayOfTimeStep);
addFieldData->Update();

```

## License

This repository is under the Apache 2.0 license, see NOTICE and LICENSE file.
