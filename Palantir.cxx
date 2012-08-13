#include <iostream>

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageToVTKImageFilter.h>
#include <itkRGBPixel.h>

#include <vtkActor2D.h>
#include <vtkImageData.h>
#include <vtkImageFlip.h>
#include <vtkImageMapper.h>
#include <vtkImageReslice.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

//VTK Code almost entirely hacked on:
//http://www.vtk.org/Wiki/VTK/Examples/Cxx/Visualization/ImageMapper

int main( int argc, char * argv[] )
{
  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef itk::Image<RGBPixelType, 2> ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  
  //Start Palantir image retrieval
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]); //ASSUME CORRECT PATH GIVEN IN ARGUMENT !!
  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject& err)
    {
    std::cerr<<"Could not read input file"<<std::endl;
    std::cerr<<err<<std::endl;
    return EXIT_FAILURE;
    }
    
  typedef itk::ImageToVTKImageFilter<ImageType> AdaptatorType;
  AdaptatorType::Pointer toVTKFilter = AdaptatorType::New();
  toVTKFilter->SetInput(reader->GetOutput());
  toVTKFilter->Update();

  //Flip image
  vtkSmartPointer<vtkImageFlip> theDolphin = vtkImageFlip::New();
  theDolphin->SetInput(toVTKFilter->GetOutput());
  theDolphin->SetFilteredAxis(1);//Y axis
  theDolphin->Update();

  vtkSmartPointer<vtkImageMapper> imageMapper =
    vtkSmartPointer<vtkImageMapper>::New();
  //imageMapper->SetInput(toVTKFilter->GetOutput());
  imageMapper->SetInput(theDolphin->GetOutput());
  imageMapper->SetColorWindow(255);
  imageMapper->SetColorLevel(127.5);

  vtkSmartPointer<vtkActor2D> imageActor = vtkSmartPointer<vtkActor2D>::New();
  imageActor->SetMapper(imageMapper);
  imageActor->SetPosition(0, 0);

  // Setup renderers
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  // Setup render window
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(toVTKFilter->GetOutput()->GetDimensions());
  renderWindow->AddRenderer(renderer);
 
  // Setup render window interactor
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  vtkSmartPointer<vtkInteractorStyleImage> style =
    vtkSmartPointer<vtkInteractorStyleImage>::New();
  renderWindowInteractor->SetInteractorStyle(style);
 
  // Render and start interaction
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderer->AddActor2D(imageActor);
  renderWindow->Render();
  renderWindowInteractor->Start();
 
  
  return EXIT_SUCCESS;
}