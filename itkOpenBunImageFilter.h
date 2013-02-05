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
#ifndef __itkOpenBunImageFilter_h
#define __itkOpenBunImageFilter_h

#include "itkVanHerkGilWermanOpenCloseLineImageFilter.h"
#include "vnl/vnl_math.h"

namespace itk
{

/**
* \class OpenBunImageFilter
* \brief The pixel-wise maxima (opening by union - openbun)
* of many openings by line segments.
* This operation is useful for removing bright blobs while retaining
* bright lines or surfaces. This filter removes regions based on their
* length, rather than length and width, as with square or rectangular
* structuring elements.
*
* This class uses the FlatStructuringElement, although not in the
* way it was intended. When using the polygon structuring element, the
* radius ends up corresponding to the length of the line elements. The
* polygon structuring element is simply a convenient container for a
* set of oriented lines.
*
* \ingroup ITKMathematicalMorphology
* \author Richard Beare, Richard.Beare@ieee.org.
*/

template< class TImage, class TKernel >
class ITK_EXPORT OpenBunImageFilter:
    public VanHerkGilWermanOpenCloseLineImageFilter< TImage, TKernel, MinFunctorCBOB< typename TImage::PixelType >, MaxFunctorCBOB< typename TImage::PixelType >  >
{
public:
  typedef OpenBunImageFilter Self;
  typedef VanHerkGilWermanOpenCloseLineImageFilter< TImage, TKernel,
                                                  MinFunctorCBOB< typename TImage::PixelType >, MaxFunctorCBOB< typename TImage::PixelType > > Superclass;

  /** Runtime information support. */
  itkTypeMacro(OpenBunImageFilter,
               VanHerkGilWermanOpenCloseLineImageFilter);

  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;
  typedef typename TImage::PixelType PixelType;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

protected:

  OpenBunImageFilter()
  {
    this->m_Boundary = NumericTraits< PixelType >::max();
  }
  virtual ~OpenBunImageFilter() {}

private:

  OpenBunImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);                    //purposely not implemented
};
} // namespace itk

#endif
