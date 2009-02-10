/****************************************************************************
 *
 * $Id: vpFeatureBuilderPoint.cpp,v 1.13 2008-02-01 15:11:39 fspindle Exp $
 *
 * Copyright (C) 1998-2006 Inria. All rights reserved.
 *
 * This software was developed at:
 * IRISA/INRIA Rennes
 * Projet Lagadic
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * http://www.irisa.fr/lagadic
 *
 * This file is part of the ViSP toolkit.
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE included in the packaging of this file.
 *
 * Licensees holding valid ViSP Professional Edition licenses may
 * use this file in accordance with the ViSP Commercial License
 * Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Contact visp@irisa.fr if any conditions of this licensing are
 * not clear to you.
 *
 * Description:
 * Conversion between tracker and visual feature point with
 * polar coordinates.
 *
 * Authors:
 * Fabien Spindler
 *
 *****************************************************************************/


/*!
  \file vpFeatureBuilderPointPolar.cpp

  \brief Conversion between tracker and visual feature point with
  polar coordinates.
*/
#include <visp/vpFeatureBuilder.h>
#include <visp/vpFeatureException.h>
#include <visp/vpException.h>

/*!

  Initialize a point feature with polar coordinates
  \f$(\rho,\theta)\f$ using the coordinates of the point in pixels
  \f$(u,v)\f$ obtained by image processing. This point is the center
  of gravity of a dot tracked using vpDot. Using the camera
  parameters, the pixels coordinates of the dot \f$(u,v)\f$ are first
  converted in cartesian \f$(x,y)\f$ coordinates in meter in the
  camera frame and than in polar coordinates by:

  \f[\rho = \sqrt{x^2+y^2}  \hbox{,}\; \; \theta = \arctan \frac{y}{x}\f]

  \warning This function does not initialize \f$Z\f$ which is
  requested to compute the interaction matrix by
  vpfeaturePointPolar::interaction().
  
  \param s : Visual feature \f$(\rho,\theta)\f$ to initialize. Be
  aware, the 3D depth \f$Z\f$ requested to compute the interaction
  matrix is not initialized by this function.
  
  \param cam : Camera parameters.

  \param dot : Tracked dot. The center of gravity corresponds to the
  coordinates of the point in the image plane.

  The code below shows how to initialize a vpFeaturePointPolar visual
  feature. First, we initialize the \f$\rho,\theta\f$, and lastly we
  set the 3D depth \f$Z\f$ of the point which is generally the result
  of a pose estimation.

  \code
  vpImage<unsigned char> I; // Image container
  vpCameraParameters cam;   // Default intrinsic camera parameters
  vpDot2 dot;               // Dot tracker

  vpFeaturePointPolar s;    // Point feature with polar coordinates
  ...
  // Tracking on the dot
  dot.track(I);

  // Initialize rho,theta visual feature
  vpFeatureBuilder::create(s, cam, dot);
  
  // A pose estimation is requested to initialize Z, the depth of the
  // point in the camera frame.
  double Z = 1; // Depth of the point in meters
  ....
  s.set_Z(Z);
  \endcode

*/
void vpFeatureBuilder::create(vpFeaturePointPolar &s,
			      const vpCameraParameters &cam,
			      const vpDot &dot)
{
  try {
    double x=0, y=0;

    double u = dot.get_u() ;
    double v = dot.get_v() ;

    vpPixelMeterConversion::convertPoint(cam,u,v,x,y) ;

    double rho   = sqrt(x*x + y*y);
    double theta = atan2(y, x);
 
    s.set_rho(rho) ;
    s.set_theta(theta) ;
  }
  catch(...) {
    vpERROR_TRACE("Error caught") ;
    throw ;
  }
}


/*!

  Initialize a point feature with polar coordinates
  \f$(\rho,\theta)\f$ using the coordinates of the point in pixels
  \f$(u,v)\f$ obtained by image processing. This point is the center
  of gravity of a dot tracked using vpDot2. Using the camera
  parameters, the pixels coordinates of the dot \f$(u,v)\f$ are first
  converted in cartesian \f$(x,y)\f$ coordinates in meter in the
  camera frame and than in polar coordinates by:

  \f[\rho = \sqrt{x^2+y^2}  \hbox{,}\; \; \theta = \arctan \frac{y}{x}\f]

  \warning This function does not initialize \f$Z\f$ which is
  requested to compute the interaction matrix by
  vpfeaturePointPolar::interaction().
  
  \param s : Visual feature \f$(\rho,\theta)\f$ to initialize. Be
  aware, the 3D depth \f$Z\f$ requested to compute the interaction
  matrix is not initialized by this function.
  
  \param cam : Camera parameters.

  \param dot : Tracked dot. The center of gravity corresponds to the
  coordinates of the point in the image plane.

  The code below shows how to initialize a vpFeaturePointPolar visual
  feature. First, we initialize the \f$\rho,\theta\f$, and lastly we
  set the 3D depth \f$Z\f$ of the point which is generally the result
  of a pose estimation.

  \code
  vpImage<unsigned char> I; // Image container
  vpCameraParameters cam;   // Default intrinsic camera parameters
  vpDot2 dot;               // Dot tracker

  vpFeaturePointPolar s;    // Point feature with polar coordinates
  ...
  // Tracking on the dot
  dot.track(I);

  // Initialize rho,theta visual feature
  vpFeatureBuilder::create(s, cam, dot);
  
  // A pose estimation is requested to initialize Z, the depth of the
  // point in the camera frame.
  double Z = 1; // Depth of the point in meters
  ....
  s.set_Z(Z);
  \endcode

*/
void vpFeatureBuilder::create(vpFeaturePointPolar &s,
			      const vpCameraParameters &cam,
			      const vpDot2 &dot)
{
  try {
    double x=0, y=0;

    double u = dot.get_u() ;
    double v = dot.get_v() ;

    vpPixelMeterConversion::convertPoint(cam,u,v,x,y) ;

    double rho   = sqrt(x*x + y*y);
    double theta = atan2(y, x);
 
    s.set_rho(rho) ;
    s.set_theta(theta) ;
  }
  catch(...) {
    vpERROR_TRACE("Error caught") ;
    throw ;
  }
}

/*!

  Initialize a point feature with polar coordinates
  \f$(\rho,\theta)\f$ using the coordinates of the point
  \f$(x,y,Z)\f$, where \f$(x,y)\f$ correspond to the perspective
  projection of the point in the image plane and \f$Z\f$ the 3D depth
  of the point in the camera frame. The values of \f$(x,y,Z)\f$ are
  expressed in meters. From the coordinates in the image plane, the
  polar coordinates are computed by:

  \f[\rho = \sqrt{x^2+y^2}  \hbox{,}\; \; \theta = \arctan \frac{y}{x}\f]

  \param s : Visual feature \f$(\rho,\theta)\f$ and \f$Z\f$ to initialize.

  \param p : A point with \f$(x,y)\f$ cartesian coordinates in the
  image plane corresponding to the camera perspective projection, and
  with 3D depth \f$Z\f$.
*/
void
vpFeatureBuilder::create(vpFeaturePointPolar &s, const vpPoint &p)
{
  try {

    double x = p.get_x();
    double y = p.get_y();

    double rho   = sqrt(x*x + y*y);
    double theta = atan2(y, x);

    s.set_rho(rho) ;
    s.set_theta(theta) ;

    s.set_Z( p.get_Z() )  ;


    if (s.get_Z() < 0) {
      vpERROR_TRACE("Point is behind the camera ") ;
      std::cout <<"Z = " << s.get_Z() << std::endl ;

      throw(vpFeatureException(vpFeatureException::badInitializationError,
			       "Point is behind the camera ")) ;
    }

    if (fabs(s.get_Z()) < 1e-6) {
      vpERROR_TRACE("Point Z coordinates is null ") ;
      std::cout <<"Z = " << s.get_Z() << std::endl ;

      throw(vpFeatureException(vpFeatureException::badInitializationError,
			       "Point Z coordinates is null")) ;
    }

  }
  catch(...) {
    vpERROR_TRACE("Error caught") ;
    throw ;
  }
}

/*!

  Initialize a point feature with polar coordinates
  \f$(\rho,\theta)\f$ using the coordinates of the point
  \f$(x,y,Z)\f$, where \f$(x,y)\f$ correspond to the perspective
  projection of the point in the image plane and \f$Z\f$ the 3D depth
  of the point in the camera frame. The values of \f$(x,y,Z)\f$ are
  expressed in meters.

  This function intends to introduce noise in the conversion from
  cartesian to polar coordinates. Cartesian \f$(x,y)\f$ coordinates
  are first converted in pixel coordinates in the image using \e
  goodCam camera parameters. Then, the pixels coordinates of the point
  are converted back to cartesian coordinates \f$(x^{'},y^{'})\f$ using
  the noisy camera parameters \e wrongCam. From these new coordinates
  in the image plane, the polar coordinates are computed by:

  \f[\rho = \sqrt{x^2+y^2}  \hbox{,}\; \; \theta = \arctan \frac{y}{x}\f]

  \param s : Visual feature \f$(\rho,\theta)\f$ and \f$Z\f$ to initialize.

  \param goodCam : Camera parameters used to introduce noise. These
  parameters are used to convert cartesian coordinates of the point \e
  p in the image plane in pixel coordinates.

  \param wrongCam : Camera parameters used to introduce noise. These
  parameters are used to convert pixel coordinates of the point in
  cartesian coordinates of the point in the image plane.

  \param p : A point with \f$(x,y)\f$ cartesian coordinates in the
  image plane corresponding to the camera perspective projection, and
  with 3D depth \f$Z\f$.
*/
void
vpFeatureBuilder::create(vpFeaturePointPolar &s,
			 const vpCameraParameters &goodCam,
			 const vpCameraParameters &wrongCam,
			 const vpPoint &p)
{
  try {
    double x = p.get_x();
    double y = p.get_y();

    s.set_Z( p.get_Z() );

    double u=0, v=0;
    vpMeterPixelConversion::convertPoint(goodCam, x, y, u, v);
    vpPixelMeterConversion::convertPoint(wrongCam, u, v, x, y);

    double rho   = sqrt(x*x + y*y);
    double theta = atan2(y, x);

    s.set_rho(rho) ;
    s.set_theta(theta) ;
  }
  catch(...) {
    vpERROR_TRACE("Error caught") ;
    throw ;
  }
}


/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
