/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkCloseBinImageFilter_h
#define __itkCloseBinImageFilter_h

#include "itkVanHerkGilWermanOpenCloseLineImageFilter.h"
#include "vnl/vnl_math.h"

namespace itk
{


/**
* \class CloseBinImageFilter
* \brief The pixel-wise minima (closing by intersection - closebin)
* of many closings by line segments.
* This operation is useful for removing dark blobs while retaining
* dark lines or surfaces. This filter removes regions based on their
* length, rather than length and width, as with square or rectangular
* structuring elements.
* \ingroup ITKMathematicalMorphology
* \author Richard Beare, Richard.Beare@ieee.org.
*/

template< class TImage, class TKernel >
class ITK_EXPORT CloseBinImageFilter:
    public VanHerkGilWermanOpenCloseLineImageFilter< TImage, TKernel, MaxFunctorCBOB< typename TImage::PixelType >, MinFunctorCBOB< typename TImage::PixelType >  >
{
public:
  typedef CloseBinImageFilter Self;
  typedef VanHerkGilWermanOpenCloseLineImageFilter< TImage, TKernel,
                                                  MaxFunctorCBOB< typename TImage::PixelType >, MinFunctorCBOB< typename TImage::PixelType > > Superclass;

  /** Runtime information support. */
  itkTypeMacro(CloseBinImageFilter,
               VanHerkGilWermanOpenCloseLineImageFilter);

  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;
  typedef typename TImage::PixelType PixelType;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

protected:

  CloseBinImageFilter()
  {
    this->m_Boundary = NumericTraits< PixelType >::NonpositiveMin();
  }
  virtual ~CloseBinImageFilter() {}

private:

  CloseBinImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);                    //purposely not implemented
};
} // namespace itk

#endif
