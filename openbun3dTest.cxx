#include "ioutils.h"

#include "itkOpenBunImageFilter.h"

int main(int argc, char * argv[])
{
  const unsigned dim = 3;
  typedef itk::Image<short, dim> ImageType;
  ImageType::Pointer raw = readIm<ImageType>(argv[1]);

  typedef itk::FlatStructuringElement<dim> FlatKernelType;

  typedef itk::OpenBunImageFilter<ImageType, FlatKernelType> OpenBunType;

  FlatKernelType::RadiusType rad;
  rad.Fill(31);

//  FlatKernelType kernel = FlatKernelType::Polygon(rad, 6);
  FlatKernelType kernel = FlatKernelType::Box(rad);

  OpenBunType::Pointer cb = OpenBunType::New();
  cb->SetInput(raw);
  cb->SetKernel(kernel);

  writeIm<ImageType>(cb->GetOutput(), argv[2]);
  return EXIT_SUCCESS;
}
