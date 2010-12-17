/****************************************************************************
 *
 * $Id$
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2010 by INRIA. All rights reserved.
 * 
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.GPL at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional 
 * Edition License.
 *
 * See http://www.irisa.fr/lagadic/visp/visp.html for more information.
 * 
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * Description:
 * Class that implements the Fern classifier and the YAPE detector thanks
 * to the OpenCV library.
 *
 * Authors:
 * Romain Tallonneau
 *
 *****************************************************************************/

#ifndef vpFernClassifier_H
#define vpFernClassifier_H

#include <string>

#include <visp/vpConfig.h>

#if (VISP_HAVE_OPENCV_VERSION >= 0x020000) // Require opencv >= 2.0.0
#if (VISP_HAVE_OPENCV_VERSION >= 0x020101) // Require opencv >= 2.1.1
#  include <opencv2/imgproc/imgproc.hpp>
#  include <opencv2/features2d/features2d.hpp>
#elif (VISP_HAVE_OPENCV_VERSION >= 0x020000) // Require opencv >= 2.0.0
#  include <cv.h>
#  include <cvaux.hpp>
#endif

#include <visp/vpBasicKeyPoint.h>

// For template instantiation with Visual Studio
#if defined(VISP_BUILD_SHARED_LIBS) && defined(VISP_USE_MSVC)
#  ifdef VISP_HAVE_OPENCV
template class VISP_EXPORT std::allocator<cv::KeyPoint>;
template class VISP_EXPORT std::vector<cv::KeyPoint>;
template class VISP_EXPORT std::allocator<cv::Point2f>;
template class VISP_EXPORT std::vector<cv::Point2f>;
#  endif
#endif


/*!
  \class vpFernClassifier

  \brief Class that implements the Fern classifier and the YAPE detector thanks
  to the OpenCV library.
  
  
  This class provides a way to detect and match point using the YAPE and 
  a Fern Classifiers, thanks to the OpenCV library (version >= 2.0)

  This class provides a tool to match points between a model and the current 
  image. The points of interests belonging to the model and the points detected
  in the current image are given in pixels thanks to the vpImagePoint class.
  
  For more details about the Ferns Classifier and the point detector, see
   - Mustafa Özuysal, Michael Calonder, Vincent Lepetit, Pascal Fua, "Fast 
      KeyPoint Recognition Using Random Ferns", IEEE Transactions on Pattern 
      Analysis and Machine Intelligence, 15 Jan. 2009.

   - Vincent Lepetit, Pascal Fua, “Towards Recognizing Feature Points Using 
      Classification Trees”, Technical Report IC/2004/74, EPFL, 2004.
      
  To use this class, you first have to detect points in the model and train the 
  associated Fern classifier. Then, for each new grabbed image, You can detect
  points and try to match them with the model. 
  
  As training can requires up to several minutes, it is possible to save (in a 
  file) and load the classifier.
  
  The following small example shows how to use the class.

  \code
#include <visp/vpConfig.h>
#include <visp/vpImage.h>
#include <visp/vpFernClassifier.h>

#if VISP_HAVE_OPENCV_VERSION >= 0x020000 // Surf key-points only available since OpenCV-1.1.0
int main()
{
  vpImage<unsigned char> Irefrence;
  vpImage<unsigned char> Icurrent;
  vpFernClassifier fern;

  //First grab the reference image Irefrence

  //Build the reference SURF points.
  fern.buildReference(Irefrence);

  //Then grab another image which represents the current image Icurrent

  //Match points between the reference points and the SURF points computed in the current image.
  fern.matchPoint(Icurrent);

  //Display the matched points
  fern.display(Irefrence, Icurrent);

  return (0);
}
#else
int main() {}
#endif
  \endcode

  It is also possible to create the refernece thanks to only a part of the
  reference image (not the whole image) and find points to match in only a
  part of the current image. The small following example shows how to do this.

  \code
#include <visp/vpConfig.h>
#include <visp/vpImage.h>
#include <visp/vpDisplay.h>
#include <visp/vpFernClassifier.h>

#if VISP_HAVE_OPENCV_VERSION >= 0x020000 // Surf key-points only available since OpenCV-1.1.0
int main()
{
  vpImage<unsigned char> Ireference;
  vpImage<unsigned char> Icurrent;
  vpFernClassifier fern;

  //First grab the reference image Irefrence

  //Select a part of the image by clincking on two points which define a rectangle
  vpImagePoint corners[2];
  for (int i=0 ; i < 2 ; i++)
  {
    vpDisplay::getClick(Ireference, corners[i]);
  }

  //Build the reference SURF points.
  int nbrRef;
  unsigned int height, width;
  height = (unsigned int)(corners[1].get_i() - corners[0].get_i());
  width = (unsigned int)(corners[1].get_j() - corners[0].get_j());
  nbrRef = fern.buildReference(Ireference, corners[0], height, width);

  //Then grab another image which represents the current image Icurrent

  //Select a part of the image by clincking on two points which define a rectangle
  for (int i=0 ; i < 2 ; i++)
  {
    vpDisplay::getClick(Icurrent, corners[i]);
  }

  //Match points between the reference points and the SURF points computed in the current image.
  int nbrMatched;
  height = (unsigned int)(corners[1].get_i() - corners[0].get_i());
  width = (unsigned int)(corners[1].get_j() - corners[0].get_j());
  nbrMatched = fern.matchPoint(Icurrent, corners[0], height, width);

  //Display the matched points
  fern.display(Ireference, Icurrent);

  return(0);
}
#else
int main() {}
#endif
  \endcode
*/
class VISP_EXPORT vpFernClassifier: public vpBasicKeyPoint
{
protected:
  //! The points of interest detector.
  cv::LDetector ldetector;
  
  //! The Fern classifier.
  cv::FernClassifier fernClassifier;

  //! The patch generator (OpenCV format).
  cv::PatchGenerator gen;
  
  
  //! Flag to indicate whether the classifier has been trained or not.
  bool hasLearn;
  
    /* parameters for the key-points detector and the classifier */
  //! Threshold to accept or reject points (usually around 20)
  int threshold;
  //! Number of view to generate for the training
  int nbView;
  //! Minimal distance between two points
  int dist;
  //! Number of classifier
  int nbClassfier;
  //! Size of the classifier
  int ClassifierSize;
  //! Number of octave for the multi scale
  int nbOctave;
  //! Size of the patch
  int patchSize;
  //! Radius for the detector
  int radius;
  //! Maximal number of points
  int nbPoints;

    /* parameters for the filter */
  //! Flag to specify whether the reference image have to be blurred or not in order to improve the recognition rate
  bool blurImage;
  //! Radius of the kernel used to blur the image 
  int radiusBlur;
  //! Sigma of the kernel used to blur the image
  int sigmaBlur;
  
  //! Number of minimum point below which the homography is not estimated (must be at least four)
  unsigned int nbMinPoint;
  
  //! The current image in the OpenCV format.
  IplImage* curIplImg; 
  
  //! keypoints detected in the reference image.
  std::vector<cv::KeyPoint> objKeypoints;
  //! the ROI in the reference image.
  cv::Rect modelROI_Ref; 
  //! the ROI for the reference image.
  cv::Rect modelROI;
  //! the vector containing the points in the model.
  std::vector<cv::KeyPoint> modelPoints;
  //! the vector containing the points in the current image.
  std::vector<cv::KeyPoint> imgKeypoints;
  //! vector in the OpenCV format to be used by the detector.
  std::vector<cv::Point2f> refPt, curPt;
  

public:
  vpFernClassifier();
  vpFernClassifier(const std::string& _dataFile, const std::string& _objectName);
  virtual ~vpFernClassifier();

    /* build reference */
  virtual unsigned int buildReference(const vpImage<unsigned char> &I);
  virtual unsigned int buildReference(const vpImage<unsigned char> &I, 
	        vpImagePoint &iP, 
	        const unsigned int height, const unsigned int width);
  virtual unsigned int buildReference(const vpImage<unsigned char> &I, 
	        const vpRect& rectangle);
    
    /* matrching */
  virtual unsigned int matchPoint(const vpImage<unsigned char> &I);
  virtual unsigned int matchPoint(const vpImage<unsigned char> &I, 
	    vpImagePoint &iP, 
	    const unsigned int height, const unsigned int width);
  virtual unsigned int matchPoint(const vpImage<unsigned char> &I, 
	    const vpRect& rectangle);

    /* display */
  virtual void display(const vpImage<unsigned char> &Iref, 
	  const vpImage<unsigned char> &Icurrent);
  virtual void display(const vpImage<unsigned char> &Icurrent);
  
    /* io methods */
  void load(const std::string& _dataFile, const std::string& /*_objectName*/);
  void record(const std::string& _objectName, const std::string& _dataFile);
  
  
  
    /* accessors */
  /*!
    The image is blurred before being processed. This solution can lead to a 
    better recognition rate.
    
    \param _blur : the new option for the blur
  */
  inline void setBlurSetting(const bool _blur){ blurImage = _blur;}
  inline void setBlurSettings(const bool _blur, int _sigma, int _size);
  
  /*!
    Return the blur option. 
    The Blur option is used to activate a filter used to blur the input image. 
    The blurring can improve the robustness of the detection.
    
    \return the value of the blur option.
  */
  inline bool getBlurSetting(){ return blurImage;}
  
  /*!
    Return the blur sigma (for the filter) option.
    
    \return The value of the sigma for the blur filter.
  */
  inline int getBlurSigma() { return this->sigmaBlur;}
  
  /*!
    return the blur size (for the filter) option
    
    \return the value of the radius for the blur filter
  */
  inline int getBlurSize() { return this->radiusBlur;}

  /*!
    Return a reference on the vector of reference points in the OpenCV format
    
    \return the list of reference points
  */
  const std::vector<cv::Point2f>& getRefPt() const {return refPt;}
  
  /*!
    Return a reference on the vector of current points in the OpenCV format
    
    \return the list of current points
  */
  const std::vector<cv::Point2f>& getCurPt() const {return curPt;}
  
  /*!
    Return the region of interest in the OpenCV format 
  */
  cv::Rect getModelROI() const { return modelROI;}

protected:
  void setImage(const vpImage<unsigned char>& _I);
  void train();
  virtual void init();
};

#endif /* opencv ver > 2.0.0 */

#endif

