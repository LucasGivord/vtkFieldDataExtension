#ifndef vtkAddFieldData_h
#define vtkAddFieldData_h

#include "vtkDataSetAlgorithm.h"
#include "vtkFieldDataExtensionModule.h" // For export macro

#include <string>
#include <vector>

/**
 * @class   vtkAddFieldData
 * @brief   Add field data array to the vtkDataSet.
 *
 * Add to the given dataset a new field data based on a given array and array name.
 * A valid array is:
 * - an array of double
 * - a 1D array if there is one component
 * - a 2D array if there is a constant number of components
 *
 * A valid array name should never start with "vtk" which is used by VTK for internal
 * treatment.
 *
 * @warning if the dataset already contains an array with this valid name, this one will
 * be erased by the new one.
 */
class VTKFIELDDATAEXTENSION_EXPORT vtkAddFieldData : public vtkDataSetAlgorithm
{
public:
  static vtkAddFieldData *New();
  vtkTypeMacro(vtkAddFieldData, vtkDataSetAlgorithm);
  void PrintSelf(ostream &os, vtkIndent indent) override;

  ///@{
  /**
   * Set/Get the array name used to create the new vtkDataArray added as field data.
   *
   * @note the setter will check if the array name is valid or not (start with 'vtk').
   */
  void SetArrayName(const std::string &arrayName);
  std::string GetArrayName();
  ///@}

  ///@{
  /**
   * Set/Get the array used to create the new vtkDataArray added as field data.
   *
   * @note the setter will check that the second dimension is constant.
   */
  void SetArray(const std::vector<std::vector<double>> &array);
  void SetArray(const std::vector<double> &array);
  std::vector<std::vector<double>> GetArray();
  ///@}

protected:
  vtkAddFieldData();
  ~vtkAddFieldData() override;

  /**
   * This method contains the core of this filter which generate a valid vtkDataArray and
   * add it to the output vtkDataSet.
   */
  int RequestData(vtkInformation *, vtkInformationVector **,
                  vtkInformationVector *) override;

private:
  vtkAddFieldData(const vtkAddFieldData &) = delete;
  void operator=(const vtkAddFieldData &) = delete;

  std::string ArrayName;

  std::vector<std::vector<double>> Array;

  std::size_t NumberOfComponents = 1;
};

#endif
