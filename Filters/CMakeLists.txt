set(field_data_files
  vtkAddFieldData
)

vtk_module_add_module(vtkFieldDataExtension
  CLASSES ${field_data_files}
)
