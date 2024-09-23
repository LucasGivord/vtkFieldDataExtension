#include "vtkAddFieldData.h"

#include <vtkDataSet.h>
#include <vtkDoubleArray.h>
#include <vtkFieldData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>

//------------------------------------------------------------------------------
vtkStandardNewMacro(vtkAddFieldData);

//------------------------------------------------------------------------------
vtkAddFieldData::vtkAddFieldData() = default;

//------------------------------------------------------------------------------
vtkAddFieldData::~vtkAddFieldData()
{
  this->ArrayName = "";
  this->Array.clear();
  this->NumberOfComponents = 1;
}

//------------------------------------------------------------------------------
void vtkAddFieldData::PrintSelf(ostream &os, vtkIndent indent) {}

//------------------------------------------------------------------------------
void vtkAddFieldData::SetArrayName(const std::string &arrayName)
{
  if (arrayName.size() > 3 && arrayName.substr(0, 3) == "vtk")
  {
    vtkWarningMacro("The array name '"
                    << arrayName << "' is invalid as it start with the prefix 'vtk'.");
    return;
  }

  this->ArrayName = arrayName;
}

//------------------------------------------------------------------------------
std::string vtkAddFieldData::GetArrayName() { return this->ArrayName; }

//------------------------------------------------------------------------------
void vtkAddFieldData::SetArray(const std::vector<std::vector<double>> &array)
{
  if (array.empty())
  {
    return;
  }

  bool shouldHasConstantComponentSize = true;
  std::size_t componentSize = array[0].size();

  for (std::size_t tuple = 1; tuple < array.size(); tuple++)
  {
    if (componentSize != array[tuple].size())
    {
      shouldHasConstantComponentSize = false;
      break;
    }
  }

  if (!shouldHasConstantComponentSize)
  {
    vtkWarningMacro("The given array doesn't have a constant number of component which "
                    "is required, aborting.");
    return;
  }

  this->NumberOfComponents = componentSize;

  this->Array.clear();
  this->Array = array;
}

//------------------------------------------------------------------------------
void vtkAddFieldData::SetArray(const std::vector<double> &array)
{
  this->Array.clear();
  this->Array.resize(array.size());
  this->NumberOfComponents = 1;

  std::fill(this->Array.begin(), this->Array.end(), array);
}

//------------------------------------------------------------------------------
std::vector<std::vector<double>> vtkAddFieldData::GetArray() { return this->Array; }

//------------------------------------------------------------------------------
int vtkAddFieldData::RequestData(vtkInformation *vtkNotUsed(request),
                                 vtkInformationVector **inputVector,
                                 vtkInformationVector *outputVector)
{
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  vtkDataSet *input = vtkDataSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkDataSet *output =
      vtkDataSet::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  if (!input || !output)
  {
    vtkErrorMacro("Input or output of this filter isn't a dataset, aborting.");
    return 0;
  }

  output->DeepCopy(input);

  if (output->GetFieldData()->HasArray(this->ArrayName.c_str()))
  {
    output->GetFieldData()->RemoveArray(this->ArrayName.c_str());
  }

  vtkNew<vtkDoubleArray> newArray;
  newArray->SetName(this->ArrayName.c_str());
  newArray->SetNumberOfTuples(this->Array.size());
  newArray->SetNumberOfValues(this->Array.size() * this->NumberOfComponents);
  newArray->SetNumberOfComponents(this->NumberOfComponents);

  for (std::size_t i = 0; i < this->Array.size(); i++)
  {
    newArray->SetTuple(i, this->Array[i].data());
  }

  output->GetFieldData()->AddArray(newArray);

  return 1;
}
