#include "ioutils.h"

#include "itkOpenBunImageFilter.h"

int main(int argc, char * argv[])
{
  const unsigned dim = 2;
  typedef itk::Image<unsigned char, dim> ImageType;
  ImageType::Pointer raw = readIm<ImageType>(argv[3]);

  // r is the line length, count the number of lines.
  int r = atoi(argv[1]);
  int count = atoi(argv[2]);

  typedef itk::FlatStructuringElement<dim> FlatKernelType;

  typedef itk::OpenBunImageFilter<ImageType, FlatKernelType> OpenBunType;

  FlatKernelType::RadiusType rad;
  rad.Fill(r);

  // create a polygon flat structuring element, which is a collection
  // of oriented lines. A box also works, but will only have lines
  // parallel to axes. 
  FlatKernelType kernel = FlatKernelType::Polygon(rad, count);

  OpenBunType::Pointer cb = OpenBunType::New();
  cb->SetInput(raw);
  cb->SetKernel(kernel);

  writeIm<ImageType>(cb->GetOutput(), argv[4]);
  return EXIT_SUCCESS;
}
