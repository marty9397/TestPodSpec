/**************************************************
 * Copyright (c)2008 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://martin@mtsrv.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/LabelingEngine/MtGeometry.h $
 *
 * Revision: $Revision: 82290 $
 *
 * Last modified on: $Date: 2016-04-06 11:09:31 -0400 (Wed, 06 Apr 2016) $, by: $Author: lwroczynski $
 *
 *************************************************/

#ifndef DTP_MTGEOMETRY_H
#define DTP_MTGEOMETRY_H

#include "LabelingEngineDefs.h"
#include "Feature.h"

#include "Collection.h"
#include "Error.h"

#include <math.h>

#include "Serialization.h"

// Provide the min/max macros for platforms where it's not present
#ifndef MIN
#define MIN( a, b ) ( (a) < (b) ? a : b )
#endif

#ifndef MAX
#define MAX( a, b ) ( (a) > (b) ? a : b )
#endif

#ifndef ABS
#define ABS( a ) ((a) < 0 ? -(a) : (a))
#endif

namespace MapText
{
	namespace Dtp
	{
		/**
		 * Forward declare pointer only dependencies
		 */
		class PointSymbol;
		class StrokeRectanglePolyline;
		class OverlapGeometries;
		class Polyline;
		class Point;
		class Vector;
		class Line;

        class LABELINGENGINE_API DoublePoint
		{
        public:
            /**
             * Possible error codes
             * @reqid 001.0063
             */
            enum ErrorCode
            {
                ErrorCode_AllocationFailure = 1
            };
            
            /**
             *	Default constructor.
             * @reqid 001.0063
             */
            DoublePoint()
            {
                m_x=0.0f;
                m_y=0.0f;
            }
            
            /**
             *	Constructor.
             *	@param	x		the x coordinate
             *	@param	y		the y coordinate
             * @reqid 001.0063
             */
            DoublePoint( double x, double y )
            {
                SetX( x );
                SetY( y );
            }
            
            /**
             * Get the x coordinate
             * @reqid 001.0063
             */
            double GetX() const
            {
                return m_x;
            }
            
            /**
             * Get the y coordinate
             * @reqid 001.0063
             */
            double GetY() const
            {
                return m_y;
            }
            
            /**
             * Set the x coordinate
             * @reqid 001.0063
             */
            void SetX( double x )
            {
                m_x = x;
            }
            
            /**
             * Set the y coordinate
             * @reqid 001.0063
             */
            void SetY( double y )
            {
                m_y = y;
            }
            
            /** 
             * Set both coordinates in one call
             * @reqid 001.0063
             */
            void SetXY( double x, double y )
            {
                m_x = x;
                m_y = y;
            }
            
            void Translate(double dx, double dy)
            {
                m_x += dx;
                m_y += dy;
            }
            
            /**
             * Assignment operator.
             * @param rhs right-hand side value
             * @return self
             * @reqid 001.0063
             */
            DoublePoint& operator= ( const DoublePoint& rhs )
            {
                if ( this == &rhs )
                {
                    return *this;
                }
                
                m_x = rhs.m_x;
                m_y = rhs.m_y;
                
                return *this;
            }            
            
#ifdef S11N
#	ifdef _MSC_VER
#		pragma warning(push)
#		pragma warning(disable : 4100)
#	endif
            /**
             * Serialization.
             * @param emitter - the emitter to serialize to.
             * @param error - error info.
             * @reqid N/A
             */
            void Serialize( ISerializer& emitter, Error& error ) const;
            
            /**
             * Deserializes a new object from the deserializer.
             * @param deserializer - The source.
             * @param output - Deserialized object if no error occured. NULL otherwise.
             * @param error - Error info.
             * @reqid N/A
             */
            static DoublePoint* DeserializeNew( IDeserializer& deserializer, Error& error );
            
            /**
             * Deserializes the object from a deserializer.
             * @param deserializer - Source for deserialization.
             * @param error - Error info.
             * @reqid N/A
             */
            void Deserialize( IDeserializer& deserializer, Error& error );
            
#	ifdef _MSC_VER
#		pragma warning(pop)
#	endif
#endif
            
            /****************************************************************************************
             THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
             TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
             ****************************************************************************************/
        private:
            
            double m_x;
            double m_y;
        };

		/**
		 *	A simple Point class that holds a two dimensional point.
		 *	@author Andrzej Ciereszko
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Point
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1
				};

				/**
				 *	Default constructor.
				 * @reqid 001.0063
				 */
				Point()
				{
					m_x=0.0f;
					m_y=0.0f;
				}

				/**
				 *	Constructor.
				 *	@param	x		the x coordinate
				 *	@param	y		the y coordinate
				 * @reqid 001.0063
				 */
				Point( float x, float y )
				{
					SetX( x );
					SetY( y );
				}

				/**
				 * Copy constructor.
				 * @param point point to copy
				 * @reqid 001.0063
				 */
				Point( const Point& point )
					: m_x(point.m_x), m_y(point.m_y)
				{				
				}

				/**
				 * Create a deep copy of this instance
				 * @return the copy
				 * @reqid 001.0063
				 */
				Point* Clone(Error& error) const
				{
					Point* clone = new Point( m_x, m_y );
					if (clone == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}

					return clone;
				}

				//GETTERS & SETTERS BEGIN

				/**
				 * Get the x coordinate
				 * @reqid 001.0063
				 */
				float GetX() const
				{
					return m_x;
				}

				/**
				 * Get the y coordinate
				 * @reqid 001.0063
				 */
				float GetY() const
				{
					return m_y;
				}

				/**
				 * Set the x coordinate
				 * @reqid 001.0063
				 */
				void SetX( float x ){
					m_x = x;
				}

				/**
				 * Set the y coordinate
				 * @reqid 001.0063
				 */
				void SetY( float y ){
					m_y = y;
				}

				/** 
				 * Set both coordinates in one call
				 * @reqid 001.0063
				 */
				void SetXY( float x, float y )
				{
					m_x = x;
					m_y = y;
				}

				//GETTERS & SETTERS END

				/**
				 * Equality operator
				 * @reqid 001.0063
				 */
				bool operator== ( const Point& point ) const;

				/**
				 * Equality operator
				 * @reqid 001.0063
				 */
				bool IsEqualTo(const Point &point,float toleranceSquared) const;

				/**
				 * Inequality operator
				 * @reqid 001.0063
				 */
				bool operator!= ( const Point& point ) const
				{
					return !operator== ( point );
				}

				/**
				* Assignment operator.
				* @param rhs right-hand side value
				* @return self
				* @reqid 001.0063
				*/
				Point& operator= ( const Point& rhs )
				{
					if ( this == &rhs )
					{
						return *this;
					}
					
					m_x = rhs.m_x;
					m_y = rhs.m_y;

					return *this;
				}

				/**
				* Rotate the point about the specified origin.
				* @param origin		the point about which rotation is requested
				* @param sin_t		the sine of the angle through which rotation is requested
				* @param cos_t		the cosine of the angle through which rotation is requested
				* @reqid 001.0063
				*/
				void Rotate( const Point& origin, float sin_t, float cos_t);

				/**
				* Rotate the set of coordinates about the specified origin.
				* @param origin		the point about which rotation is requested
				* @param x			[in,out] x coordinate of the rotated point
				* @param y			[in,out] y coordinate of the rotated point
				* @param sin_t		the sine of the angle through which rotation is requested
				* @param cos_t		the cosine of the angle through which rotation is requested
				* @reqid 001.0063
				*/
				void static Rotate( const Point& origin, float* x, float* y, float sin_t, float cos_t);

                void static Rotate( const Point& origin, double* x, double* y, float sin_t, float cos_t);

				/**
				 * Scale the coordinates by specified factors.
				 * @param horzScaleFactor		the factor by which to multiply the x coordinate
				 * @param vertScaleFactor		the factor by which to multiply the y coordinate
				 * @reqid 001.0063
				 */
				void Scale(float horzScaleFactor, float vertScaleFactor)
				{
					SetXY(m_x * horzScaleFactor, m_y * vertScaleFactor);
				}

				/**
				 * Shift by specified amounts in x and y directions.
				 * @param	dx		the x coordinate shift
				 * @param	dy		the y coordinate shift
				 * @reqid 001.0063
				 */
				void Translate(float dx, float dy)
				{
					m_x += dx;
					m_y += dy;
				}

				/**
				* Compute the square of the distance of this point from another point.
				* @param otherPoint	the other point from which to measure the distance
				* @return the square of the distance from the specified point.
				* @reqid 002.0039
				*/
				float SquaredDistanceFrom( const Point& otherPoint ) const;

				/**
				* Compute the distance of this point from another point.
				* @param otherPoint	the other point from which to measure the distance
				* @return the square of the distance from the specified point.
				* @reqid 002.0039
				*/
				float DistanceFrom( const Point& otherPoint ) const;

				/* 
				 * Determines if the three input points are colinear.
				 * @param p0 - first point
				 * @param p1 - second point
				 * @param p2 - third point
				 * @return - true if points are colinear, false if they are not.
				 * @reqid 003.0025
				 */
				static bool PointsAreCollinear(const Point *p0,const Point *p1,const Point *p2);

				/* 
				 * Forms vector from "this" point to the input "point" and stores in "vector"
				 * @param point - head of the vector
				 * @param vector - [out] receiver of formed vector
				 * @reqid 001.0063
				 */
				void GetVectorTo(const Point &point,Vector &vector) const;
			
#ifdef S11N
#	ifdef _MSC_VER
#		pragma warning(push)
#		pragma warning(disable : 4100)
#	endif
				/**
				 * Serialization.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				void Serialize( ISerializer& emitter, Error& error ) const;

				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static Point* DeserializeNew( IDeserializer& deserializer, Error& error );

				/**
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				void Deserialize( IDeserializer& deserializer, Error& error );

#	ifdef _MSC_VER
#		pragma warning(pop)
#	endif
#endif

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:

				float m_x;
				float m_y;
		};

		class UprightRectangle;
        
		/**
		 * A simple Rectangle class that holds for corners.
		 * @author Phil Grenetz
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Rectangle
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1
				};

				/**
				 * Default constructor.
				 * @reqid 001.0063
				 */
				Rectangle();

				/**
				 * Constructor.
				 * @param lowerLeft the lower left corner of the rectangle
				 * @param upperLeft the upper left corner of the rectangle
				 * @param upperRight the upper right corner of the rectangle
				 * @param lowerRight the lower right corner of the rectangle
				 * @reqid 001.0063
				 */
				Rectangle( const Point& lowerLeft, const Point& upperLeft, const Point& upperRight,
					const Point& lowerRight );

				/**
				* Constructor copying properties of an upright rectangle.
				* @param uprightRectangle Coordinates of this rectangle will be used.
				* @reqid 001.0063
				*/
				Rectangle( const UprightRectangle& uprightRectangle );

				/**
				 * Destructor.
				 * @reqid 001.0063
				 */
				~Rectangle()
				{
				}

				/**
				 * Create a deep copy of this instance
				 * @return the copy
				 * @reqid 001.0063
				 */
				Rectangle* Clone(Error& error) const
				{
					Rectangle* clone = new Rectangle();
					if (clone == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}

					clone->Copy( *this, *clone );

					return clone;
				}

				//GETTERS & SETTERS BEGIN

				/**
				 * @reqid 001.0063
				 */
				const Point* GetLowerLeft() const
				{
					return &m_lowerLeft;
				}

				/**
				 * @reqid 001.0063
				 */
				const Point* GetUpperLeft() const
				{
					return &m_upperLeft;
				}

				/**
				 * @reqid 001.0063
				 */
				const Point* GetUpperRight() const
				{
					return &m_upperRight;
				}

				/**
				 * @reqid 001.0063
				 */
				const Point* GetLowerRight() const
				{
					return &m_lowerRight;
				}
            
                /**
                 * Get the center of the rect
				 * @reqid 001.0063
                 */
                Point GetCenter()
                {
                    Point center;
                    center.SetX((m_lowerLeft.GetX()+m_upperRight.GetX())/2.0f);
                    center.SetY((m_lowerLeft.GetY()+m_upperRight.GetY())/2.0f);
                    
                    return center;
                }            

				/**
				 * @reqid 001.0063
				 */
				void SetLowerLeft( const float& x, const float& y )
				{
					m_lowerLeft.SetX( x );
					m_lowerLeft.SetY( y );
				}

				/**
				 * @reqid 001.0063
				 */
				void SetUpperLeft( const float& x, const float& y )
				{
					m_upperLeft.SetX( x );
					m_upperLeft.SetY( y );
				}

				/**
				 * @reqid 001.0063
				 */
				void SetUpperRight( const float& x, const float& y )
				{
					m_upperRight.SetX( x );
					m_upperRight.SetY( y );
				}

				/**
				 * @reqid 001.0063
				 */
				void SetLowerRight( const float& x, const float& y )
				{
					m_lowerRight.SetX( x );
					m_lowerRight.SetY( y );
				}

				/**
				 * Get the angle -- and its sine and cosine -- of the original
				 * orientation of the rectangle when instantiated.
				 * @param sine		address of the sine to be populated
				 * @param cosine	address of the cosine to be populated
				 * @return the original orientation angle in radians
				 */
				float GetAngleSineCosine( float* sine = NULL, float* cosine = NULL ) const;

				/**
				 * Rotate self about the rectangle's lower left corner.
				 * @param sin_t sine of angle by which to rotate
				 * @param cos_t cosine of angle by which to rotate
				 * @reqid 001.0063
				 */
				void Rotate( float sin_t, float cos_t );

				/**
				* Rotate self about the specified origin.
				* @param anchorPoint - point about which to rotate
				* @param sin_t - sine of angle by which to rotate
				* @param cos_t - cosine of angle by which to rotate
				* @reqid 001.0063
				*/
				void Rotate( const Point &anchorPoint, float sin_t, float cos_t );

				/**
				* Rotate self about the specified origin.
				* @param anchorPoint - point about which to rotate
				* @param angle - angle to which to rotate
				* @reqid 001.0063
				*/
				void RotateToAngle( const Point &anchorPoint, float angle );

				/**
				 * Rotate self to be upright.
				 * @reqid 001.0063
				 */
				void MakeUpright();

				/**
				 * Populate the minimum bounding upright rectangle.
				 * @param bbox		Reference to the upright rectangle to be populated
				 * @reqid 001.0063
				 */
				void GetBoundingBox( UprightRectangle& bbox ) const;

                /**
                 * Create polyline for the rectangle bounds
                 *
                 */
                Polyline* GetPolylineFromRectangle(Error& error);

				/**
				* Shift by specified amounts in x and y directions.
				* @param	dx		the x coordinate shift
				* @param	dy		the y coordinate shift
				* @reqid 001.0063
				*/
				void Translate(float dx, float dy);

                /**
                 * Scale the coordinates of the rectangle by specified factors.
                 * @param horzScaleFactor		the factor by which to multiply the x coordinate
                 * @param vertScaleFactor		the factor by which to multiply the y coordinate
                 * @reqid 001.0063
                 */
                void Scale(float horzScaleFactor, float vertScaleFactor);

                void Scale(float scaleFactor)
                {
                    Scale(scaleFactor, scaleFactor);
                }

				/**
				* Increase the size of the rectangle equally in all directions
				* @param	increaseAmount
				* @reqid 006.0002
				* @reqid 006.0003
				* @reqid 006.0004
				* @reqid 006.0006
				*/
				void IncreaseSize(float increaseAmount);

                /**
                 * Increase the width and height of the rectangle
                 * @param	increaseWidth
                 * @param	increaseHeight
                 * @reqid 006.0002
                 * @reqid 006.0003
                 * @reqid 006.0004
                 * @reqid 006.0006
                 */
                void IncreaseSize( float increaseWidth, float increaseHeight );

				/**
				* Increase the size of the rectangle to enclose the referenced Rectangle
				* @param	rect
				* @reqid 006.0002
				* @reqid 006.0003
				* @reqid 006.0004
				* @reqid 006.0006
				*/
				void IncreaseSize(Rectangle& rect, const Point &anchorPt );
				/**
				* Decrease the size of the rectangle equally in all directions
				* @param	decreaseAmount
				* @reqid 006.0002
				* @reqid 006.0003
				* @reqid 006.0004
				* @reqid 006.0006
				*/
				void DecreaseSize(float decreaseAmount);

				/**
				* Determine whether a specified point is in this rectangle.
				* @param testPoint	the point to be tested
				* @return true if inside; false if not
				* @reqid 002.0039
				*/
				bool IsPointInside( const Point& testPoint ) const;


                /**
                 * Determine whether a specified rectangle intersects this rectangle.
                 * @param testRectangle	the rectangle to be tested
                 * @return true if interstects; false if not
                 * @reqid 002.0039
                 */
                bool IntersectsRectangle( const Rectangle& testRectangle );
            
				/**
				* Get AnalysisFlag - a bread crumb of the last obstacle analysis done on the rectangle.
				* @return AnalysisFlag
				* @reqid 001.0064
				*/
				unsigned int GetAnalysisFlag() const 
				{ 
					return m_analysisFlag;
				}

				/**
				* Set AnalysisFlag
				* @see GetAnalysisFlag
				* @param val AnalysisFlag
				* @reqid 001.0064
				*/
				void SetAnalysisFlag(unsigned int val)
				{ 
					m_analysisFlag = val; 
				}


				/**
				* Recomputes the angle and resets the internal angle member.
				* @reqid 001.0064
				*/
				void RecomputeTheAngle()
				{
					m_angleComputed = false;

					ComputeTheAngle();
				}

#ifdef S11N
				/**
				 * Serialization.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				void Serialize( ISerializer& emitter, Error& error ) const;

				/**
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				void Deserialize( IDeserializer& deserializer, Error& error );
#endif

			protected:
				/**
				 * Make deep copy of one instance's members to another
				 * @param src
				 * @param dest
				 * @reqid 001.0063
				 */
				static void Copy( const Rectangle& src, Rectangle& dest )
				{
					dest.SetLowerLeft( src.GetLowerLeft()->GetX(), src.GetLowerLeft()->GetY() );
					dest.SetUpperLeft( src.GetUpperLeft()->GetX(), src.GetUpperLeft()->GetY() );
					dest.SetUpperRight( src.GetUpperRight()->GetX(), src.GetUpperRight()->GetY() );
					dest.SetLowerRight( src.GetLowerRight()->GetX(), src.GetLowerRight()->GetY() );

					dest.m_angleComputed = src.m_angleComputed;
					dest.m_angle = src.m_angle;
					dest.m_sine = src.m_sine;
					dest.m_cosine = src.m_cosine;

					dest.m_analysisFlag = src.m_analysisFlag;
				}

				/**
				* Computes the angle information (angle, sine, cosine) if 
				* was not computed before or rotation occurred invalidating previous
				* calculations.
				* @reqid 001.0063
				*/
				void ComputeTheAngle() const;

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				Point m_lowerLeft;
				Point m_upperLeft;
				Point m_upperRight;
				Point m_lowerRight;

				/**
				 * Flag indicating whether the rectangle's orientation info has been computed
				 * this enables a lazy initialization behavior -- some uses of the class
				 * will not need its orientation and the computations are expensive
				 * @reqid 001.0063
				 */
				mutable bool m_angleComputed;

				/**
				 * Angle of rectangle relative to horizontal,
				 * counter-clockwise being the positive direction of rotation.
				 * @reqid 001.0063
				 */
				mutable float m_angle;

				/**
				 * sin of orientation angle
				 * @reqid 001.0063
				 */
				mutable float m_sine;

				/**
				 * cosine of orientation angle
				 * @reqid 001.0063
				 */
				mutable float m_cosine;

				unsigned int m_analysisFlag;
		};

		/**
		* An upright rectangle - slimmed down version of regular rectangle.
		* Because it's upright there's no need to store 4 corners and angle information.
		* Useful for storing e.g. upright bounding boxes
		* @reqid 001.0063
		*/
		class LABELINGENGINE_API UprightRectangle
		{
		public:

			/**
			 * Collection of upright rectangles
			 */
			class LABELINGENGINE_API Collection : public LinkedList< UprightRectangle >
			{
			public:
				/**
				 * Constructor.
				 * @reqid 004.0032
				 */
				Collection() : LinkedList< UprightRectangle >(true)
				{}

				/**
				 * Destructor.
				 * @reqid 004.0032
				 */
				~Collection()
				{}
			};

			/**
			* Default constructor
			* @reqid 001.0063
			*/
			UprightRectangle() : m_lowerLeft(0,0),m_upperRight(0,0)
			{
			}

			/**
			* Constructor that initialize the rectangle with two points
			* @param lowerLeft lower left corner
			* @param upperRight upper right corner
			* @reqid 001.0063
			*/
			UprightRectangle( const Point& lowerLeft, const Point& upperRight )
				: m_lowerLeft(lowerLeft), m_upperRight(upperRight)
			{
			}

            UprightRectangle( const DoublePoint& lowerLeft, const DoublePoint& upperRight ) : m_lowerLeft((float)lowerLeft.GetX(), (float)lowerLeft.GetY()),
                                                                                              m_upperRight((float)upperRight.GetX(),(float)upperRight.GetY())
            {
            }

			/**
			* Constructor that initialize the rectangle with the coordinates of two points
			* @param llx lower left X
			* @param lly lower left Y
			* @param urx upper right X
			* @param ury upper right Y
			* @reqid 001.0063
			*/
			UprightRectangle( float llx, float lly, float urx, float ury )
				: m_lowerLeft(llx,lly),m_upperRight(urx,ury)
			{
			}

			/**
			 * Copy constructor.
			 * @param ur instance to copy
			 * @reqid 001.0063
			 */
			UprightRectangle( const UprightRectangle& ur )
				: m_lowerLeft( ur.m_lowerLeft ), m_upperRight( ur.m_upperRight )
			{			
			}

			/**
			 * Constructor that initialize the rectangle by computing the bounding upright rectangle of the 
			 * reference rectangle
			 * @param rect instance of a Rectangle
			 * @reqid 001.0063
			 */
			UprightRectangle(Rectangle &rect);

			/**
			 * Assignment operator.
			 * @pa�growram rhs right-hand side
			 * @return self
			 * @reqid 001.0063
			 */
			UprightRectangle& operator= ( const UprightRectangle& rhs )
			{
				if ( this == &rhs )
				{
					return *this;
				}
				m_lowerLeft = rhs.m_lowerLeft;
				m_upperRight = rhs.m_upperRight;

				return *this;
			}

			/**
			* Get the upper right point of the rect
			* @reqid 001.0063
			*/
			const Point* GetUpperRight() const
			{
				return &m_upperRight;
			}

			/**
			* Get the lower left point of the rect
			* @reqid 001.0063
			*/
			const Point* GetLowerLeft() const
			{
				return &m_lowerLeft;
			}
            
			/**
             * Get the center of the rect
             * @reqid 
             */
            Point GetCenter() const
            {
                Point center;
                center.SetX((m_lowerLeft.GetX()+m_upperRight.GetX())/2.0f);
                center.SetY((m_lowerLeft.GetY()+m_upperRight.GetY())/2.0f);
                
                return center;
            }

			/**
			* Set the upper right point of the rect
			* @reqid 001.0063
			*/
			void SetUpperRight( float x, float y )
			{
				m_upperRight.SetXY( x, y );
			}

			/**
			* Set the lower left point of the rect
			* @reqid 001.0063
			*/
			void SetLowerLeft( float x, float y )
			{
				m_lowerLeft.SetXY( x, y );
			}

			/**
			* Grow the uprightrectangle with the given point.
			* @param point if any coordinate of this point is outside the upright rectangle
			*        it will grow to accommodate the new point
			* @reqid 001.0063
			*/
			void Grow( const Point &point );

            /**
             * Grow the uprightrectangle with the given uprightrectangle.
             * @param bbox if either the lowerleft or upperright of this uprightrectangle is outside the upright rectangle
             *        it will grow to accommodate this point
             * @reqid 001.0063
             */
            void Grow( UprightRectangle& bbox );
            
			/**
             */
			void Scale( float horzScaleFactor, float vertScaleFactor );

            void Scale(float scaleFactor)
            {
                Scale(scaleFactor, scaleFactor);
            }
            
			/**
			* Shift by specified amounts in x and y directions.
			* @param	dx		the x coordinate shift
			* @param	dy		the y coordinate shift
			* @reqid 001.0063
			*/
			void Translate(float dx,float dy);

			/**
			 * Performs spatial test to check if the point is within the rectangle (border 
			 * is considered to be outside the rectangle).
			 * @param x coordinate of the point
			 * @param y coordinate of the point
			 * @return true if point inside the rectangle, false otherwise
			 * @reqid 002.0039
			 */
			bool IsPointInside( float x, float y ) const
			{
				return ( x > m_lowerLeft.GetX() 
					&& x < m_upperRight.GetX()
					&& y > m_lowerLeft.GetY()
					&& y < m_upperRight.GetY() );
			}
            
			/**
			 * Performs spatial test to check if the uprightrectnagle is partially within the rectangle (border
			 * is considered to be outside the rectangle).
			 * @param uprightRectangle
			 * @return true if uprightRectangle at least partially inside the rectangle, false otherwise
			 */
            bool IsUprightRectangleInside(const UprightRectangle& uprightRectangle) const;

            /**
			 * Performs spatial test to check if the uprightrectnagle is completely within the rectangle (border
			 * is considered to be outside the rectangle).
			 * @param uprightRectangle
			 * @return true if uprightRectangle is completely inside the rectangle, false otherwise
			 */
            bool IsUprightRectangleCompletelyInside(const UprightRectangle& uprightRectangle) const;

			/**
			* Determine whether a specified point is on the boundary of this rectangle.
			* @param testPoint	the point to be tested
			* @return true if on the boundary; false if not
			* @reqid 002.0039
			*/
			bool IsPointOnBoundary( const Point& testPoint ) const;

			void ClipToUprightRectangle( const UprightRectangle& otherRectangle );

			/**
			 * Get the rectangle width
			 * @return width
			 * @reqid 004.0032
			 */
			float GetWidth() const
			{
				return m_upperRight.GetX() - m_lowerLeft.GetX();
			}

			/**
			* Get the rectangle height
			* @return heigth
			* @reqid 004.0032
			*/
			float GetHeight() const
			{
				return m_upperRight.GetY() - m_lowerLeft.GetY();
			}

            /**
             * Create polyline for the rectangle bounds
             *
             */
			Polyline* GetPolylineFromRectangle(Error& error) const;
            
			/**
			* Compare two UprightRectangles
			* @param uprightRectangle - the UprightRectangle to compare current UprightRectangle with
			* @return - true if vectors are identical, false otherwise.
			* @reqid 
			*/
			bool operator == (const UprightRectangle &uprightRectangle ) const;

#ifdef S11N
			/**
			 * Serialization.
			 * @param emitter - emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			void Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the object from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			void Deserialize( IDeserializer& deserializer, Error& error );

#endif

			/**
			 * Checks wheter this upright rectangle has zero coordinates (is uninitialized).
			 * @return true if both lower left and upper right points are set to (0,0), false otherwise.
			 */
			bool IsZero() const;

		private:
			Point m_lowerLeft;
			Point m_upperRight;
		};

		/**
		 * A circle as defined by it's center and radius. Also provides sampling functionality.
		 */
		class LABELINGENGINE_API Circle
		{
		public:
			/**
			* Possible error codes
			*/
			enum ErrorCode
			{
				ErrorCode_AllocationFailure = 1
			};

			enum SamplingDirection
			{
				SamplingDirection_Clockwise,
				SamplingDirection_CounterClockwise,
			};

			/**
			 * Constructor.
			 * @param center the center of the circle
			 * @param radius radius of the circle
			 * @return 
			 * @reqid 003.0030
			 */
			Circle( const Point& center, float radius )
				: m_center( center ), m_radius( radius )
			{}

			/**
			 * Destructor (compiler generated code avoidance)
			 * @reqid 003.0030
			 */
			~Circle()
			{}

			/**
			 * Create a straight-segment approximation of the circular arc.
			 * @param startAngle - start angle of the arc. It will be normalized to (0; 2*PI] range.
			 * @param endAngle - end angle of the arc. It will be normalized to (0; 2*PI] range.
			 * @param direction - in which direction should the vertices be progressing
			 * @param segmentMaxLength - max length of the sampled segment
			 *													(some segments may be shorter because of the arc length).
			 * @param sampledArc [out] - the resulting polyline
			 * @param error - error info
			 * @reqid 003.0030
			 */
			void Sample( float startAngle, float endAngle, SamplingDirection direction, 
				float segmentMaxLength, Polyline** sampledArc, Error& error );

			/**
			 * Create a straight-segment approximation of the circular arc.
			 * @param startAngle - start angle of the arc. It will be normalized to (0; 2*PI] range.
			 * @param endAngle - end angle of the arc. It will be normalized to (0; 2*PI] range.
			 * @param direction - in which direction should the vertices be progressing
			 * @param numSegments - the number of segments to create whilte sampling
			 * @param sampledArc [out] - the resulting polyline
			 * @param error - error info
			 * @reqid 003.0030
			 */
			void Sample( float startAngle, float endAngle, int numSegments, SamplingDirection direction,
                        Polyline** sampledArc, Error& error );
            
			/**
			 * Getter for center of circle
			 * @return center of circle
			 * @reqid 003.0030
			 */
			const Point *GetCenter() const
			{
				return &m_center;
			}

			/**
			 * Getter of radius of circle
			 * @return radius of cicle
			 * @reqid 003.0030
			 */
			float GetRadius() const
			{
				return m_radius;
			}

		private:
			Point m_center;
			float m_radius;
		};

		/**
		 * A simple Line Segment class that holds two vertices.
		 * @author Phil Grenetz
		 * @reqid 001.0065
		 */
		class LABELINGENGINE_API LineSegment
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1
				};

				enum WhereInSegment
				{
					WhereInSegment_BeforePoint0,
					WhereInSegment_AfterPoint1,
					WhereInSegment_BetweenPoints
				};

				/**
				 * Default constructor.
				 * @reqid 001.0065
				 */
				LineSegment()
				{
				}

				/**
				 * Constructor.
				 * @param point0 first endpoint
				 * @param point1 second endpoint
				 * @reqid 001.0065
				 */
				LineSegment( const Point& point0, const Point& point1 )
				{
					m_point0 = point0;
					m_point1 = point1;
				}

				/**
				 * Destructor.
				 * @reqid 001.0065
				 */
				~LineSegment()
				{
				}

				/**
				 * Equals operator.
				 * @param lineSegment - the one to compare to
				 * @return true->the line segments are equal, false->they are not
				 * @reqid Simplification
				 */
				bool operator == (const LineSegment &lineSegment ) const;

				/**
				 * Create a deep copy of this instance
				 * @return the copy
				 * @reqid 001.0063
				 */
				LineSegment* Clone(Error& error) const
				{
					LineSegment* clone = new LineSegment();
					if (clone == NULL)
					{
						SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
					}

					clone->Copy( *this, *clone );

					return clone;
				}

				//GETTERS & SETTERS BEGIN

				/**
				 * Get the start point of the segment.
				 * @return the start point of the segment
				 * @reqid 001.0063
				 */
				const Point* GetPoint0() const
				{
					return &m_point0;
				}

				/**
				 * Get the end point of the segment.
				 * @return the end point of the segment
				 * @reqid 001.0063
				 */
				const Point* GetPoint1() const
				{
					return &m_point1;
				}

				/**
				* Calculate the length of the segment.
				* @return the length of the segment
				* @reqid 002.0039
				*/
				float GetLength()
				{
					return m_point1.DistanceFrom( m_point0 );
				}

				/**
				* Get point along segment
				* param point point being returned
				* param t	  percentage distance along segment
				* @reqid 003.0021
				*/
				void GetPointAlongSegment(Point &point, float t) const
				{
					float dx, dy;

					dx = m_point1.GetX() - m_point0.GetX();
					dy = m_point1.GetY() - m_point0.GetY();

					point.SetXY(m_point0.GetX()+dx*t, m_point0.GetY()+dy*t);					
				}

				/**
				 * Set starting point of the segment
				 * @reqid 001.0063
				 */
				void SetPoint0( float x, float y ){
					m_point0.SetX( x );
					m_point0.SetY( y );
				}

				/**
				 * TODO
				 * @reqid Simplification
				 */
				void SetPoint0(const Point &point)
				{
					m_point0=point;
				}

				/**
				 * Set starting point of the segment
				 * @reqid 001.0063
				 */
				void SetPoint1( float x, float y ){
					m_point1.SetX( x );
					m_point1.SetY( y );
				}

				/**
				 * TODO
				 * @reqid Simplification
				 */
				void SetPoint1(const Point &point)
				{
					m_point1=point;
				}

				/**
				 * Rotate specified endpoints about the specified origin.
				 * @param point0 the first endpoint
				 * @param point1 the second endpoint
				 * @param origin point about which to rotate
				 * @param sin_t sine of angle by which to rotate
				 * @param cos_t cosine of angle by which to rotate
				 * @reqid 001.0065
				 */
				static void Rotate( Point* point0, Point* point1,
					const Point& origin, float sin_t, float cos_t );

				/**
				* Shift by specified amounts in x and y directions.
				* @param	dx		the x coordinate shift
				* @param	dy		the y coordinate shift
				* @reqid 001.0063
				*/
				void Translate( float dx, float dy );

				/**
				 * Clip the LineSegment to a rectangle that has been 
				 * rotated to be upright.
				 * This static method allows caller to construct a
				 * LineSegment instance if clipped.
				 * @param point0 the first endpoint
				 * @param point1 the second endpoint
				 * @param rect the clipping window
				 * @param sine Sine of the angle by which the clipping rectangle
				 *             has been rotated to become upright. Needed to rotate
				 *             the obstacle accordingly.
				 * @param cosine Cosine of the angle by which the clipping rectangle
				 *             has been rotated to become upright.
				 * @param rotatedPoint0 [out] returns the point0 rotated to be in the
				 *        upright rectangle coordinate system; set to NULL if not needed
				 * @param rotatedPoint1 [out] @see rotatedPoint0
				 * @return true if the line segment is clipped by the window; false if not
				 * @reqid 001.0065
				 */
				static bool ClipToRectangle( Point* point0, Point* point1,
					Rectangle& rect, float sine, float cosine, 
					Point* rotatedPoint0, Point* rotatedPoint1 );

				/**
				* Clip the LineSegment to an upright rectangle. This version
				* assumes the rectangle has been originally upright and no internal
				* rotation of the segment needs to be done.
				* This static method allows caller to construct a
				* LineSegment instance if clipped.
				* @param point0 the first endpoint
				* @param point1 the second endpoint
				* @param rect the clipping window
				* @return true if the line segment is clipped by the window; false if not
				* @reqid 001.0065
				*/
				static bool ClipToUprightRectangle( Point* point0, Point* point1, UprightRectangle& rect );

				/**
				 * Perform perpendicular projection of specified point.
				 * @param x								horizontal coordinate of point
				 * @param y								vertical coordinate of point
				 * @param projectedPoint	[out] intersection point
				 * @reqid 001.0063
				 */
				void ProjectPointPerpendicular(
					float x, float y, Point* projectedPoint );

				/**
				 * Find point's relative location to segment, given it is
				 * coincident with extended line.
				 * @param point		Horizontal coordinate of point
				 * @param tparam	The error object to be populated
				 * @reqid 001.0063
				 */
				WhereInSegment GetWhereInSegment( const Point& point, float* tparam ) const;

				/**
				* Does the given circle intersect this line segment, given it is
				* coincident with extended line.
				* @param center center of circle
				* @param radius radius of circle
				* @param doesIntersect [out] does the circle intersect the segment
				* @reqid 003.0021
				*/
				void DoesLineSegmentIntersectCircle(
					const Point &center, float radius, bool &doesIntersect ) const;

				/**
				* Does the given point lie on the segment
				* @param point check if this point is on the segment
				* @reqid 003.0021
				*/
				bool DoesPointLieOnSegment( const Point &point) const;

				/** 
				* Static version of the CalculateDistanceFromSegment method.
				*
				* @param segmentPoint1 one end point of the line segment
				* @param segmentPoint2 the other end point of the line segment
				* @param pointOffSegment the point for which the distance is to be calculated
				*
				* @return the distance from the pointOffSegment to the line segment
				*
				* @reqid 003.0021
				*/
				static float CalculateDistanceFromSegment(const Point &segmentPoint1, const Point &segmentPoint2, const Point &pointOffSegment);

				/** 
				* Calculates the distance from the point to the line segment.
				*
				* @param segmentPoint1 one end point of the line segment
				* @param segmentPoint2 the other end point of the line segment
				* @param pointOffSegment the point for which the distance is to be calculated
				*
				* @return the distance from the pointOffSegment to the line segment
				*
				* @reqid 003.0021
				*/
				float CalculateDistanceFromSegment(const Point &pointOffSegment)
				{
					return CalculateDistanceFromSegment(m_point0, m_point1, pointOffSegment);
				}

				/**
				 * Computes intersection of input circle with the line segment.
				 * @param circle - circle to intersect with line segment
				 * @param earlierIntersectionPoint -[out] first intersection with segment if it exists.
				 * @param laterIntersectionPoint - [out] second intersection with segment if it exists.
				 * @return number of intersections with polyline.
				 * @reqid 001.0063
				 */
				int ComputeIntersectionWithCircle(const Circle &circle,Point *earlierIntersectionPoint,Point *laterIntersectionPoint) const;

				/**
				 * Computes the intersection of the input line with the line segment.
				 * @param line - line to intersect with line segment.
				 * @param intersectionPoint [out] filled with intersection point
				 * @return true if intersection found, false if no intersection found
				 * @reqid 001.0063
				 */
				bool ComputeIntersectionWithLine(const Line &line,Point *intersectionPoint) const;

				/**
				 * Determines whether this line segment is intersected by the input line.
				 * @param line - line to test for intersection with this segment.
				 * @return true if intersection found, false if no intersection found
				 * @reqid 001.0063
				 */
				bool IntersectedByLine(const Line &line) const;

                /**
                 * Determines whether this line segment is intersected by the input line segment.
                 * @param lineSegment - lineSegment to test for intersection with this segment.
                 * @return true if intersection found, false if no intersection found
                 * @reqid 001.0063
                 */
                bool IntersectedByLineSegment(const LineSegment &lineSegment) const;
            
				/**
					TODO
				 * @reqid Simplification
				*/
				bool Point1InFrontOfPoint2(const Point &point1,const Point &point2,bool inclusive=true) const;

				/**
					TODO
				 * @reqid Simplification
				*/
				bool Point1InBackOfPoint2(const Point &point1,const Point &point2,bool inclusive=true) const;

			protected:
				/**
				 * Make deep copy of one instance's members to another
				 * @param src
				 * @param dest
				 * @reqid 001.0063
				 */
				static void Copy( const LineSegment& src, LineSegment& dest )
				{
					dest.SetPoint0( src.GetPoint0()->GetX(), src.GetPoint0()->GetY() );
					dest.SetPoint1( src.GetPoint1()->GetX(), src.GetPoint1()->GetY() );
				}

			private:
				/** Clip the line segment represented (x0,y0), (x1,y1) to the upright rectangle.
				 * See Liang and Barsky,The Optimal Tree Algorithm for Line Clipping,
				 * University of California, Berkeley,
				 * Original function name is subclip
				 * @param x0         x coordinate of the first endpoint of the line segment
				 * @param y0         y coordinate of the first endpoint of the line segment
				 * @param x1         x coordinate of the second endpoint of the line segment
				 * @param y1         y coordinate of the second endpoint of the line segment
				 * @param left       x coordinate of the left side of the rectangle
				 * @param right      x coordinate of the right side of the rectangle
				 * @param top        y coordinate of the top edge of the rectangle
				 * @param bottom     y coordinate of the bottom edge of the rectangle
				 * @param reject     address of flag to be populated with true if no intersection.
				 * @reqid 001.0065
				 */
				static void SubClip( float& x0, float& y0, float& x1, float& y1,
					float left, float right, float top, float bottom, bool* reject );

				/** Clip segment where one endpoint is interior to the clipping rectangle
				 *  and the other endpoint above or below the clipping rectangle.
				 * See Liang and Barsky,The Optimal Tree Algorithm for Line Clipping,
				 * University of California, Berkeley,
				 * Page 20
				 * Original function name is clip_window
				 * @param x_window   x coordinate of the interior endpoint
				 * @param y_window   y coordinate of the interior endpoint
				 * @param x_strip    x coordinate of the exterior endpoint
				 * @param y_strip    y coordinate of the exterior endpoint
				 * @param b1         y coordinate of the crossed horizontal edge (bottom or top)
				 * @param left       x coordinate of the left side of the rectangle
				 * @param right      x coordinate of the right side of the rectangle
				 * @reqid 001.0065
				 */
				static void ClipWindow( float x_window, float y_window,
					float& x_strip, float& y_strip, float b1, float left, float right );

				/** Clip segment where endpoints form a strip down or across the middle,
				 * at least one being exterior of the clipping rectangle.
				 * See Liang and Barsky,The Optimal Tree Algorithm for Line Clipping,
				 * University of California, Berkeley,
				 * Page 27
				 * Original function name is clip_strip
				 * @param b1         x coordinate of the left or right edge
				 * @param b2         x coordinate of the right left edge (opposite from b1)
				 * @param flag       true if other endpoint is exterior
				 * @param x0         x coordinate of the first endpoint of the line segment
				 * @param y0         y coordinate of the first endpoint of the line segment
				 * @param x1         x coordinate of the second endpoint of the line segment
				 * @param y1         y coordinate of the second endpoint of the line segment
				 * @reqid 001.0065
				 */
				static void ClipStrip(  float b1, float b2, bool flag,
					float& x0, float& y0, float& x1, float& y1 );

				/** Clip segment where endpoints are both exterior to the clipping rectangle
				 * and one endpoint is in central exterior region opposite the strip
				 * occupied by the other endpoint.
				 * See Liang and Barsky,The Optimal Tree Algorithm for Line Clipping,
				 * University of California, Berkeley,
				 * Page 27
				 * Original function name is clip_opposite
				 * @param b1         x coordinate of the left or right edge
				 * @param b2         x coordinate of the right or left edge (opposite from b1)
				 * @param x0         x coordinate of the first endpoint of the line segment
				 * @param y0         y coordinate of the first endpoint of the line segment
				 * @param x1         x coordinate of the second endpoint of the line segment
				 * @param y1         y coordinate of the second endpoint of the line segment
				 * @param left       x coordinate of the left side of the rectangle
				 * @param right      x coordinate of the right side of the rectangle
				 * @param reject     address of flag to be populated with true if no intersection.
				 * @reqid 001.0065
				 */
				static void ClipOpposite( float b1, float b2, float& x0, float& x1,
					float& y0, float& y1, float left, float right, bool* reject );

				/** Clip segment where endpoints are both exterior to the clipping rectangle,
				 * one endpoint is in the left or right central exterior region and
				 * the other is in a horizontally aligned adjacent pair of regions
				 * neither of which is directly above or below region occupied by the the first endpoint.
				 * See Liang and Barsky,The Optimal Tree Algorithm for Line Clipping,
				 * University of California, Berkeley,
				 * Page 24
				 * Original function name is clip_unbalanced_pivotal
				 * @param b2         x coordinate of the left or right edge
				 * @param b3         x coordinate of the right left edge (opposite from b2)
				 * @param b4         y coordinate of top or bottom edge (top if y1 above top, bottom if below bottom)
				 * @param flag       true if other endpoint is exterior
				 * @param x0         x coordinate of the first endpoint of the line segment
				 * @param x1         x coordinate of the second endpoint of the line segment
				 * @param y0         y coordinate of the first endpoint of the line segment
				 * @param y1         y coordinate of the second endpoint of the line segment
				 * @param reject     address of flag to be populated with true if no intersection.
				 * @reqid 001.0065
				 */
				static void ClipUnbalancedPivotal( float b2, float b3, float b4,
					bool flag, float& x0, float& x1, float& y0, float& y1, bool* reject );

				/** Clip segment where endpoints are both exterior to the clipping rectangle,
				 * one endpoint is in a horizontally aligned adjacent pair of regions,
				 * the other is in a vertically aligned adjacent pair of regions,
				 * and neither endpoint occupies the adjacent corner region.
				 * See Liang and Barsky,The Optimal Tree Algorithm for Line Clipping,
				 * University of California, Berkeley,
				 * Page 23
				 * Original function name is clip_balanced_pivotal
				 * @param b1         y coordinate of the bottom or top edge
				 * @param b2         x coordinate of the left or right edge
				 * @param b3         y coordinate of the top or bottom edge (opposite from b1)
				 * @param b4         x coordinate of the right or left edge (opposite from b2)
				 * @param flag0      true if first endpoint is horizontally outside rectangle
				 * @param flag1      true if second endpoint is vertically outside rectangle
				 * @param x0         x coordinate of the first endpoint of the line segment
				 * @param y0         y coordinate of the first endpoint of the line segment
				 * @param x1         x coordinate of the second endpoint of the line segment
				 * @param y1         y coordinate of the second endpoint of the line segment
				 * @param reject     address of flag to be populated with true if no intersection.
				 * @reqid 001.0065
				 */
				static void ClipBalancedPivotal( float b1, float b2, float b3, float b4, bool flag0, bool flag1,
					float& x0, float& y0, float& x1, float& y1, bool* reject );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				Point m_point0;
				Point m_point1;
		};

		/**
		 * A simple Vector class that holds .
		 * @author Phil Grenetz
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Vector
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_ZeroMagnitude = 1
				};

				/**
				 * Points of the compass reflecting a vector's direction
				 * @reqid 001.0063
				 */
				enum Direction
				{
					Direction_Unknown = 0,
					Direction_East,
					Direction_NorthEast,
					Direction_North,
					Direction_NorthWest,
					Direction_West,
					Direction_SouthWest,
					Direction_South,
					Direction_SouthEast,
				};

				/**
				 * Enumerates the side of the vector, the left side or the right side
				 */
				enum VectorSide
				{
					VectorSide_Unknown = 0,
					VectorSide_Left,
					VectorSide_Right
				};

				/**
				* Default constructor.
				* @reqid 001.0063
				*/
				Vector();

				/**
				 * Constructor.
				 * @param point0	first endpoint
				 * @param point1	second endpoint
				 * @reqid 001.0063
				 */
				Vector( const Point& point0, const Point& point1 );

				/*
				 * Constructor for Position Vector
				 * @param point - vector formed to input point from origin.
				 * @reqid 001.0063
			     */
				Vector(const Point& point);

				/**
				 * Constructor.
				 * @param lineSegment		a line segment from which to create the vector
				 * @reqid 001.0063
				 */
				Vector( const LineSegment& lineSegment );

				/**
				 * Constructor.
				 * @param deltax		projection of magnitude in x dimension
				 * @param deltay		projection of magnitude in y dimension
				 * @reqid 001.0063
				 */
				Vector( float deltax, float deltay );

				/**
				 * Copying constructor.
				 * @param anotherVector vector to make copy of.
				 * @reqid 003.0030
				 */
				Vector( const Vector& anotherVector )
					:	m_deltaX( anotherVector.m_deltaX ), m_deltaY( anotherVector.m_deltaY ),
						m_initialized( anotherVector.m_initialized ), m_magnitude( anotherVector.m_magnitude )
				{}

				/**
				 * Multiplies the vector components by a scalar
				 * @param scalar - value to multiply each component
				 * @reqid 001.0063
				 */
				void Scale(float scalar)
				{
					m_deltaX *= scalar;
					m_deltaY *= scalar;

					if (m_initialized)
						m_magnitude *= scalar;
				}

				/**
				 * Get the horizontal projection of the vector's magnitude.
				 * @return the vector's magnitude projected in the x dimension
				 * @reqid 001.0063
				 */
				float GetDeltaX() const
				{
					return m_deltaX;
				}

				/**
				* Sets Deltax
				* @see GetDeltaX
				* @param val			the new deltax value
				* @reqid 001.0063
				*/
				void SetDeltaX( float val );

				/**
				 * Get the vertical projection of the vector's magnitude.
				 * @return the vector's magnitude projected in the y dimension
				 * @reqid 001.0063
				 */
				float GetDeltaY() const
				{
					return m_deltaY;
				}

				/**
				* Sets DeltaY
				* @see GetDeltaY
				* @param val			the new deltay value
				* @reqid 001.0063
				*/
				void SetDeltaY( float val );

				/**
				* Sets both deltas
				* @see GetDeltaX
				* @see GetDeltaY
				* @param dx delta X
				* @param dy delta Y
				* @reqid 001.0063
				*/
				void SetDeltas( float dx, float dy );

				/**
				* Report whether the magnitude of the vector has been computed yet.
				* It is computed by non-default constructors and SetXxx methods.
				* @return true if magnitude was computed; false if not
				* @reqid 001.0063
				*/
				bool IsInitialized() const
				{
					return m_initialized;
				}

				/**
				 * Get the magnitude of the vector.
				 * @return the vector's magnitude
				 * @reqid 001.0063
				 */
				float GetMagnitude() const
				{
					if ( !m_initialized )
					{
						ComputeMagnitude();
					}
					return m_magnitude;
				}

				/**
				 * Get the magnitude squared of the vector.
				 * @return the vector's magnitude squared
				 * @reqid 001.0063
				 */
				float GetMagnitudeSquared() const
				{
					return m_deltaX*m_deltaX+m_deltaY*m_deltaY;
				}

				/**
				* Get the angle of the vector relative to the positive x axis.
				* @return the vector's angle in radians
				* @reqid 001.0063
				*/
				float GetAngle() const;

				/**
				* Get the compass direction of the vector.
				* @return the vector's direction or Direction_Unknown if both deltas are effectively zero
				* @reqid 001.0063
				*/
				Direction GetDirection() const;

				/**
				* Normalize the vector to one unit magnitude.
				* If magnitude is effectively zero, leave as zero.
				* @reqid 001.0063
				*/
				void Unitize()
				{
					if ( !m_initialized )
					{
						ComputeMagnitude();
					}

					if (	( m_magnitude > 0.0f ) &&
								( ( m_magnitude < 0.99999f ) || ( m_magnitude > 1.00001f ) ) )
					{
						m_deltaX /= m_magnitude;
						m_deltaY /= m_magnitude;
						m_magnitude = 1.0f;
					}
				}

				/**
				 * Rotate the vector by the given angle
				 * @param angle
				 * @reqid 001.0063
				 */
				void Rotate( float angle );

				/**
				* Rotate the vector.
				* @param sin_t		the sine of the angle through which rotation is requested
				* @param cos_t		the cosine of the angle through which rotation is requested
				* @reqid 001.0063
				*/
				void Rotate( float sin_t, float cos_t );

				/**
				* Reverse the direction of the vector.
				* @reqid 002.0039
				*/
				void Invert()
				{
					m_deltaX = -m_deltaX;
					m_deltaY = -m_deltaY;
				}

				/**
				* Reverse the direction of the vector.
				* This method does not change the vector on which it is called.
				* @param outVector	[out] a vector with the reverse direction
				* @reqid 002.0039
				*/
				void AssignInverted( Vector& outVector )
				{
					assert( &outVector != this );

					outVector = *this;
					outVector.Invert();
				}

				/**
				 * Calculate the dot product of two vector instances.
				 * @param vector0		first vector
				 * @param vector1		second vector
				 * @reqid 001.0063
				 */
				static float DotProduct(const Vector& vector0, const Vector& vector1);

				/**
				* Calculate the z coordinate of a cross product of two vectors.
				* Since we operate in 2-d space z coordinate is not very useful,
				* but it may allow us to check the relative position of two vectors.
				* The z coordinate sign follows the sine of angle between the two
				* vectors.
				* @param testVector The other vector
				* @return the z coordinate of the cross product
				* @reqid 001.0063
				*/
				float CrossProduct(const Vector& testVector) const;

				/**
				 * Tests if two vector are parallel with some tolerance.
				 * @param v1 vector no 1
				 * @param v2 vector no 2
				 * @param cosineTolerance cosing of the angle defining the tolerance
				 * @return true if vectors are parallel
				 * @reqid 001.0063
				 */
				static bool AreVectorsParallel( Vector& v1, Vector& v2, float cosineTolerance = 0.99999f );

				/**
				  * Determines in which quadrant in the cartesian plane the input vector (dx,dy) is in
				  * @param dx,dy - components of vector
				  * @return - quadrant 1, 2, 3, or 4.
				  * @reqid 002.0039
				  */
				static int GetCartesianQuadrant(float dx,float dy);

				/**
				* Add two vectors.
				* @param vector1 - the first addend
				* @param vector2 - the second addend
				* @param sumVector - [out] the sum of the two input vectors
				* @reqid 003.0030
				*/
				static void AddVectors( const Vector& vector1, const Vector& vector2, Vector& sumVector );

				/**
				 * Determines in which quadrant in the cartesian plane the vector (dx,dy) is in
				 * @return - quadrant 1, 2, 3, or 4.
				 * @reqid 003.0030
				 */
				int GetCartesianQuadrant() const
				{
					return GetCartesianQuadrant( m_deltaX, m_deltaY );
				}

				/* 
				 * Determines if the slope of the vector is in the range of [-slope,slope].
				 * @param slope - positive end of the slope range.
				 * @return - true if vector's slope is in range, false if it is not
				 * @reqid 003.0025
				 */
				bool SlopeInRange(float slope) const;

				/* 
				 * Determines if the left or right side of the vector is the above side.
				 * @return - Left or Right side
				 * @reqid 003.0025
				 */
				VectorSide WhichSideIsAbove() const;

				/* 
				 * Determines if the left or right side of the vector is the below side.
				 * @return - Left or Right side
				 * @reqid 003.0025
				 */
				VectorSide WhichSideIsBelow() const;

				/**
				 * Gets the vector perpendicular to "this" one to the left.
				 * @return perpendicular vector to the left
				 * @reqid 001.0063
				 */
				Vector GetPerpendicularVectorToTheLeft() const
				{
					return Vector(-m_deltaY,m_deltaX);
				}

				/**
				 * Gets the vector perpendicular to "this" one to the right.
				 * @return perpendicular vector to the right
				 * @reqid 001.0063
				 */
				Vector GetPerpendicularVectorToTheRight() const
				{
					return Vector(m_deltaY,-m_deltaX);
				}

				/**
				 * Assignment operator
				 * @reqid 001.0063
				 */
				Vector& operator = (const Vector& v );

				/**
				 * Compare two vectors
				 * @param otherVector - the vector to compare current vector with
				 * @return - true if vectors are identical, false otherwise.
				 * @reqid 001.0063
				 */
				bool operator == (const Vector& otherVector ) const
				{
					return m_deltaX == otherVector.m_deltaX
						&& m_deltaY == otherVector.m_deltaY;
				}

			private:
				float m_deltaX;
				float m_deltaY;
				mutable float m_magnitude;
				mutable bool m_initialized;

				/**
				 * Computes the vector magnitude
				 * @reqid 001.0063
				 */
				void ComputeMagnitude() const;
		};

		/**
		 * Abstract base class for the polyline geometry. Provides read only access to its
		 * points collection.
		 */
		class LABELINGENGINE_API IPolylineView
		{
			public:

				class Collection : public LinkedList<IPolylineView>
				{
				public:

					/**
					 * Constructor
					 * @param ownsElements if true elements are owned by the collection and destroyed along with it.
					 *        if false, they are not deleted with the collection
					 * @see LinkedList::LinkedList
					 * @reqid 001.0063
					 */
					Collection( bool ownsElements )
						: LinkedList<IPolylineView>( ownsElements )
					{
					}

					/**
					 * Destructor declared explicitly to avoid compiler generated code.
					 * @reqid 001.0063
					 */
					~Collection()
					{
					}
				};

				/**
				 * Our own RTTI - all the derived classes have to identify themselves
				 * by implementing the GetType method returning the proper type.
				 */
				enum Type
				{
					Type_Polyline,
					Type_PolylinePart,
					Type_ClippedPolyline,
					Type_RectanglePolyline,
					Type_StrokeRectanglePolyline,
				};

				/**
				 * Get the i-th vertex of the polyline
				 * @param i				index of the vertex requested
				 * @param vertex	[out] the requested vertex
				 * @param error		an error object
				 * @reqid 001.0063
				 */
				virtual void GetVertex( int i, const Point** vertex, Error& error ) const = 0;

				/**
				 * Get the number of vertices in this polyline.
				 * @return the number of vertices allocated
				 * @reqid 001.0063
				 */
				virtual int GetNumVertices() const = 0;

				/**
				 * Make sure the destructors are virtual
				 * @reqid 001.0063
				 */
				virtual ~IPolylineView()
				{}

				/**
				 * Create a deep copy of the object
				 * @param error		an Error object
				 * @reqid 001.0063
				 */
				virtual IPolylineView* Clone(Error& error) const = 0;

				/**
				 * Clip the segments of the polyline to an upright rectangle.
				 * @param rect							the clipping window
				 * @param sine							Sine of the angle by which the clipping rectangle
				 *													has been rotated to become upright. Needed to rotate
				 *													the obstacle accordingly.
				 * @param cosine						Cosine of the angle by which the clipping rectangle
				 *													has been rotated to become upright.
				 * @param overlaps					[out] the overlap geometries
				 * @param errorInfo					[out] an Error object
				 * @param analysisFlag			optional number passed into the clipper to avoid inclusion
				 *													of the same segment in more than one overlap tests.
				 *													If called twice with the same input flag segments already analyzed,
				 *													they will not be analyzed again.
				 * @reqid 001.0065
				 */
				virtual void ClipToRectangle( Rectangle& rect, float sine, float cosine, 
					OverlapGeometries& overlaps, Error& errorInfo, unsigned int analysisFlag = 0 ) = 0;

				/**
				 * Get a segment surrounding the polyline segment box built to reflect
				 * the thickness of the polyline symbolization. Those boxes are cashed by the polyline
				 * for the given stroke width. It means, that if the stroke width does not change
				 * the given segment box will not be re-generated. It will be recomputed discarding
				 * the previous instance if a different stroke width is supplied.
				 * @param segmentNumber number of the segment which box is needed
				 * @param strokeWidth width of the stroke to apply
				 * @param box [out] the segment box. A pointer to the internal cashe is returned. 
				 *                  Client should NOT attempt to deallocate the object. It will be 
				 *                  deallocated together with the polyline.
				 * @param error error information
				 * @reqid 001.0064
				 */
				virtual void GetStrokeBox( int segmentNumber, float strokeWidth, StrokeRectanglePolyline** box, Error& error ) = 0;

				/**
				 * Poor man's RTTI. Each child class should identify itself by returning
				 * the proper type.
				 * @reqid 001.0065
				 */
				virtual Type GetType( ) const = 0;
		};

		/**
		 * A collection of rectangles
		 * @reqid 001.0065
		 */
		class LABELINGENGINE_API Rectangles : public LinkedList<Rectangle>
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0065
				 */
				Rectangles()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0065
				 */
				~Rectangles()
				{
				}
		};

		/**
		 * Defined by an anchor point and a vector giving its direction.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Ray
		{
		public:
			/**
			* Default Constructor.
			* @reqid 001.0063
			*/	
			Ray()
			{
			}

			/**
			 * Constructor
			 * @param anchorPoint - where the ray starts from
			 * @param direction - the direction in which the ray emanates from the anchor point.
			 * @reqid 001.0063
			 */	
			Ray(const Point &anchorPoint,const Vector &direction)
			{
				m_anchorPoint = anchorPoint;
				m_direction = direction;
			}

			/**
			* Constructor
			* @param segment	A line segment whose point0 represents the starting point of the ray
			*									and whose direction is from point0 to point1
			* @param error		An Error object reference to be popualted
			* @reqid 001.0063
			*/	
			Ray( const LineSegment& segment )
			{
				m_anchorPoint.SetXY( segment.GetPoint0()->GetX(), segment.GetPoint0()->GetY() );
				m_direction.SetDeltas(
					( segment.GetPoint1()->GetX() - segment.GetPoint0()->GetX() ),
					( segment.GetPoint1()->GetY() - segment.GetPoint0()->GetY() ) );
			}

			/**
			 * Get the anchor point.
			 * @return - anchor point of ray
			 * @reqid 001.0063
			 */	
			const Point *GetAnchorPoint() const
			{
				return &m_anchorPoint;
			}

			/**
			* Set the anchor point.
			* @param anchorPoint	the anchor point of the ray
			* @reqid 001.0063
			*/	
			void SetAnchorPoint( const Point& anchorPoint )
			{
				m_anchorPoint = anchorPoint;
			}

			/**
			 * Get the direction.
			 * @return - direction of ray
			 * @reqid 001.0063
			 */	
			const Vector *GetDirection() const
			{
				return &m_direction;
			}

			/**
			* Set the direction.
			* @param direction	the direction of the ray
			* @reqid 001.0063
			*/	
			void SetDirection( const Vector& direction )
			{
				m_direction = direction;
			}

			/**
			 * Intersects the input ray with this ray and returns the intersection times. T1 and T2 may be negative.
			 * @param ray		the other ray to test for intersection
			 * @param t1		[out] time at which this ray intersects the input ray, can be NULL, will not be calculated if NULL
			 * @param t2		[out] time at which the input ray intersects this ray. can be NULL, will not be calculated if NULL
			 * @return true if the rays intersect, false if not
			 * @reqid 001.0063
			 */	
			bool ComputeIntersectionWithRay(const Ray &ray, float *t1, float *t2) const;

			/**
			 * Fills the input pointOnRay with the coordinates of the Point on the ray at time 'time'.
			 * @param time - time at which to compute the point on the ray.
			 * @param pointOnRay - [out] container to receive computed point on ray.
			 * @reqid 001.0063
			 */	
			void ComputePointAtTimeT(float time, Point &pointOnRay) const;

			/**
			 * Tests whether the input line intersected this ray or not.
			 * @param line - line to test for intersection with this ray.
			 * @return - true if intersection occurs, false if it does not.
			 * @reqid 001.0063
			 */	
			bool IntersectedByLine(const Line &line) const;

			/**
			 * Compare two rays.
			 * @param otherRay - the ray to compare current ray with
			 * @return - true if rays are identical, false otherwise.
			 * @reqid 001.0063
			 */
			bool operator == ( const Ray& otherRay ) const;

		private:
			Point m_anchorPoint;
			Vector m_direction;
		};

		class LABELINGENGINE_API Line
		{
		public:

			/**
			 * Represents a line geometry
			 * @param ray - defines the line
			 * @reqid 001.0063
			 */
			Line(const Ray &ray)
			{
				m_ray=ray;
			}

			/**
			 * Getter for ray that defines the line
			 * @return internal ray
			 * @reqid 001.0063
			 */
			const Ray *GetRay() const
			{
				return &m_ray;
			}

		private:

			Ray m_ray;
		};

	/**
		 * Polyline simplifier that implements original Ramer�Douglas�Peucker algorithm.
		 * It has O(nlogn) complexity.
		 * To use: instantiate this class passing a polyline to simplify and call Simplify method with
		 * a tolerance.
		 * @reqid simplification
		 */
		class PolylineSimplifier
		{
		public:
			/**
			 * Possible error codes
			 * @reqid simplification
			 */
			enum ErrorCode
			{
				ErrorCode_AllocationFailure = 1
			};

			/**
			 * Creates instance of line simplifier.
			 * @param polyline polyline to simplify
			 * @param tolerance simplification precision (maximum distance between the original and simplified polyline)
			 * @reqid simplification
			 */
			PolylineSimplifier(Polyline* polyline, float tolerance);

			/**
			 * Destructor.
			 * @reqid simplification
			 */
			~PolylineSimplifier();

			/**
			 * Simplify polyline using Ramer-Douglas-Peucker alogorithm.
			 * @param error error object
			 * @reqid simplification
			 */
			void Simplify(Error& error);

		private:
			/**
			 * Segment to simplify defined by index of start vertex and end vertex.
			 * @reqid simplification
			 */
			class Segment
			{
			public:
				/**
				 * Constructs new segment
				 * @param startVertex index of a start vertex
				 * @param endVertex index of an end vertex
				 * @reqid simplification
				 */
				Segment(int startVertex, int endVertex)
					: startVertex(startVertex), endVertex(endVertex) {};

				int startVertex;
				int endVertex;
			};

			/**
			 * Find vertex which is farthest from given segment defined by start and end vertex
			 * and return orthogonal distance of this vertex to the segment.
			 * @param startVertex index of a start vertex
			 * @param endVertex index of an end vertex
			 * @param farthestVertex [out] index of found farthest vertex
			 * @param error error object
			 * @return distance of farthest vertex
			 * @reqid simplification
			 */
			float FindFarthestVertex( int startVertex, int endVertex, int* farthestVertex, Error& error );

			/**
			 * Moves given vertices to the list of vertices to keep.
			 * @param startVertex first vertex
			 * @param endVertex end vertex
			 * @param error error object
			 * @reqid simplification
			 */
			void KeepSegment( int startVertex, int endVertex, Error& error );

			/**
			 * Simplifies part of the polyline.
			 * @param startVertex index of a start vertex to simplify
			 * @param endVertex index of an end vertex to simplify
			 * @param error error object
			 * @reqid simplification
			 */
			void SimplifyChain(int startVertex, int endVertex, Error& error);

			float m_tolerance;
			Polyline* m_polyline;
			LinkedList<Point> m_keptVertices;
		};

		/**
		 *	A simple Polyline class that holds an array of vertices.
		 *	@author Andrzej Ciereszko
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Polyline : public IPolylineView
#ifdef S11N
			, ISerializable
#endif
		{
			public:

				/**
				 * A factory class to instantiate a new Polyline or its derived class.
				 * @reqid N/A.
				 */
				class Factory
				{
				public:
					/**
					 * Constructor.
					 * @param numVertices		the number of vertices that constitute the polyline
					 * @param error					an error object to be populated
					 * @reqid 001.0063
					 */
					virtual Polyline* Create( int numVertices, Error& error ) = 0;

					/**
					 * Needs to be cloneable
					 * @param error[out] - Error info.
					 * @return - The clone or NULL on error.
					 * @reqid N/A.
					 */
					virtual Factory* Clone( Error& error ) = 0;

                    /**
                     * Virtual destructor needed.
                     * @reqid N/A.
                     */
                    virtual ~Factory()
                    {}
                };
            
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_IndexOutOfRange,
					ErrorCode_ArgumentError,
					ErrorCode_TooSmallNumVertices,
					ErrorCode_ZeroLengthSegment,
				};

				/**
				* Possible sides of a polyline
				* @reqid 001.0063
				*/
				enum PolylineSide
				{
					PolylineSide_Unknown = 0,
					PolylineSide_Left,
					PolylineSide_Right
				};

				/**
				* Possible intersections with a a polyline
				* @reqid 001.0063
				*/
				enum WhichIntersection
				{
					WhichIntersection_First = 0,
					WhichIntersection_Nearest,
					WhichIntersection_Furthest
				};

				struct LABELINGENGINE_API TimeTaggedIntersection
				{
					Point intersection;
					float time;
				};

				/**
				 * A point cursor capable of sliding along the polyline while ensuring
				 * it's still on it.
				 */
				class LABELINGENGINE_API Cursor
				{
				public:

					enum ErrorCode
					{
						ErrorCode_ZeroLengthPolyline = 1,
						ErrorCode_ShiftImpossible,
						ErrorCode_DifferentOwner,
					};

					/**
					* Default constructor.
					* @reqid 003.0030
					*/
					Cursor()
						:	m_owner(NULL),
							m_segmentNumber(-1),
							m_dxAlongSegment(0.0f),
							m_dyAlongSegment(0.0f),
							m_distanceFromSegmentStart(0.0f),
							m_distanceFromSegmentEnd(0.0f),
							m_distancesCalculated( false ),
							m_segmentDx(0),
							m_segmentDy(0)
					{
					};

					/**
					 * Constructor.
					 * @param owner - the polyline on which the cursor exists
					 * @param segmentNumber - number of the segment on which the cursor
					 *                      is initially positioned
					 * @param dxAlongSegment - x delta from the segment beginning
					 * @param dyAlongSegment - y delta from the segment beginning
					 * @param error - [out] possible data validation errors are reported here
					 * @reqid 003.0030
					 */
					Cursor( const Polyline& owner, int segmentNumber, float dxAlongSegment,
						float dyAlongSegment, Error& error );

					/**
					 * Constructor.
					 * @param owner - the polyline on which the cursor exists
					 * @param segmentNumber - number of the segment on which the cursor
					 *                      is initially positioned
					 * @param location - the location of the cursor
					 * @param error - [out] error info
					 * @reqid 003.0030
					 */
					Cursor( const Polyline& owner, int segmentNumber, const Point& location, Error& error );

					/**
					 * Assignment operator. Compiler generated code avoidance.
					 * @param otherCursor - the cursor to be copied over this instance
					 * @return - this instance
					 * @reqid 003.0030
					 */
					Cursor& operator = ( const Cursor& otherCursor )
					{
						Copy( otherCursor, *this );
						return *this;
					}

					/**
					 * Copying constructor.
					 * @param otherCursor - the cursor to copy properties from.
					 * @reqid 003.0030
					 */
					Cursor( const Cursor& otherCursor )
					{
						Copy( otherCursor, *this );
					}

					/**
					 * Copy one cursor over another.
					 * @param src - source cursor
					 * @param dest - destination cursor
					 * @reqid 003.0030
					 */
					static void Copy( const Cursor& src, Cursor& dest )
					{
						if ( &src == &dest )
						{
							return;
						}

						dest.m_currentLocation = src.m_currentLocation;
						dest.m_dxAlongSegment = src.m_dxAlongSegment;
						dest.m_dyAlongSegment = src.m_dyAlongSegment;
						dest.m_segmentNumber = src.m_segmentNumber;
						dest.m_owner = src.m_owner;
						dest.m_distanceFromSegmentStart = src.m_distanceFromSegmentStart;
						dest.m_distanceFromSegmentEnd = src.m_distanceFromSegmentEnd;
						dest.m_distancesCalculated = src.m_distancesCalculated;
						dest.m_segmentDx = src.m_segmentDx;
						dest.m_segmentDy = src.m_segmentDy;
					}

					/**
					 * Shift the cursor along the related polyline by an
					 * absolute amount measured along the polyline. 
					 * @param shiftAmount - The desired shift. Positive number
					 *        shifts the anchor towards the n-1 vertex, negative value 
					 *        towards the 0th vertex. Direction flag is updated 
					 *        accordingly. If shift is impossible given the constraints
					 *        an error code of ErrorCode_ShiftImpossible value is returned
					 * @param error - Error info
					 * @param treatAsClosed - If true than shifting will be done in a closed loop
					 * @reqid 003.0030
					 */
					void Shift( float shiftAmount, Error& error, bool treatAsClosed = false, bool allowedToShiftBeyondLimits = true );

					/**
					 * Calculate the distance along a polyline between two cursors on it.
					 * @param otherCursor - the other cursor to measure the distance to
					 * @param distance - [out] the distance
					 * @param error - [out] error info
					 * @reqid 003.0030
					 */
					void DistanceAlongPolyline( const Cursor& otherCursor, float* distance, Error& error ) const;

					/**
					 * Compare two cursors using the distance from polyline start as the metric.
					 * @param otherCursor - cursor to compare the current cursor to
					 * @return - true if the current cursor is closer to the polyline start, false otherwise
					 * @reqid 003.0030
					 */
					bool operator < ( const Cursor& otherCursor ) const;

					/**
					* Gets CurrentLocation
					* @return CurrentLocation
					* @reqid 003.0030
					*/
					const MapText::Dtp::Point* GetCurrentLocation() const 
					{ 
						return &m_currentLocation; 
					}

					/**
					* Sets CurrentLocation
					* @param val - new location
					* @param segmentNumber - number of segment no which the point lies
					* @param error error information
					* @reqid 003.0030
					*/
					void SetCurrentLocation(const MapText::Dtp::Point& val, int segmentNumber, Error& error );

					/**
					* Resets cursor properties
					* @param owner - the polyline on which the cursor exists
					* @param segmentNumber - number of the segment on which the cursor
					*                      is initially positioned
					* @param dxAlongSegment - the x location of the cursor relative to the segment start
					* @param dyAlongSegment - the y location of the cursor relative to the segment start
					* @param error - [out] error info
					* @reqid 003.0030
					*/
					void Reset( const Polyline& owner, int segmentNumber, float dxAlongSegment,
						float dyAlongSegment, Error& error );
				
					/**
					* Resets cursor properties
					* @param owner - the polyline on which the cursor exists
					* @param segmentNumber - number of the segment on which the cursor
					*                      is initially positioned
					* @param location - the location of the cursor
					* @param error - [out] error info
					* @reqid 003.0030
					*/
					void Reset( const Polyline& owner, int segmentNumber, const Point& location, Error& error );
					/**
					* Gets x delta from the segment beginning
					* @return delta
					* @reqid 003.0030
					*/
					float GetDxAlongSegment() const 
					{ 
						return m_dxAlongSegment; 
					}

					/**
					* Shifts the cursor along the segment by dx. Delta y is calculated accordingly.
					* @param dx - the new dx from segment start
					* @param error - error info; it's possible the new dx is more than dx of 
					*                the whole segment
					* @reqid 003.0030
					*/
					void ShiftXAlongSegment( float dx, Error& error );

					/**
					* Shifts the cursor along the segment by dy. Delta x is calculated accordingly.
					* @param dy - the dy to shift by along the segment
					* @param error - error info; it's possible the new dy is more than dy of
					*                the whole segment
					* @reqid 003.0030
					*/
					void ShiftYAlongSegment( float dy, Error& error );

					/**
					* Gets y delta from the segment beginning
					* @return delta
					* @reqid 003.0030
					*/
					float GetDyAlongSegment() const 
					{ 
						return m_dyAlongSegment; 
					}

					/**
					* Gets number of the segment on which the cursor currently exists
					* @return segment number
					* @reqid 003.0030
					*/
					int GetSegmentNumber() const 
					{ 
						return m_segmentNumber; 
					}

					/**
					* Gets the polyline this cursor refers to
					* @return - owner polyline
					* @reqid 003.0030
					*/
					const Polyline* GetOwner() const 
					{ 
						return m_owner; 
					}

					/**
					* Sets the polyline this cursor refers to
					* @param owner - the polyline on which the cursor exists
					* @reqid 003.0030
					*/
					void SetOwner( const Polyline& owner )
					{
						m_owner = &owner;
					}

					/**
					* Sets the polyline this cursor refers to
					* @param owner - the polyline on which the cursor exists
					* @reqid 003.0030
					*/
					void SetOwner( const Polyline *owner )
					{
						m_owner = owner;
					}

					/**
					* Gets DistanceFromSegmentStart
					* @return DistanceFromSegmentStart
					* @reqid 003.0030
					*/
					float GetDistanceFromSegmentStart() const 
					{ 
						if ( !m_distancesCalculated )
						{
							CalculateDistances();
						}
						return m_distanceFromSegmentStart; 
					}

					/**
					* Gets DistanceFromSegmentEnd
					* @return DistanceFromSegmentEnd
					* @reqid 003.0030
					*/
					float GetDistanceFromSegmentEnd() const 
					{ 
						if ( !m_distancesCalculated )
						{
							CalculateDistances();
						}
						return m_distanceFromSegmentEnd; 
					}

					/**
					* Indicates whether the current location on the polyline is at a vertex that is neither of
					  the end points.
					* @return true -> location is at a vertex, false -> location is not at a vertex.
					* @reqid 003.0025
					*/
					bool IsAtNonEndPointVertex() const;

					/**
					* Determines if the current location is sitting on the first end point.
					* @return true -> location is on first end point, false -> location is not
					* @reqid 003.0025
					*/
					bool IsAtFirstEndPoint() const;

					/**
					* Determines if the current location is sitting on the last end point.
					* @return true -> location is on last end point, false -> location is not
					* @reqid 003.0025
					*/
					bool IsAtLastEndPoint() const;

					/**
					* Indicates whether the current location on the polyline is at an end point
					* @return true -> location is at an end point, false -> location is not at an end point.
					* @reqid 003.0025
					*/
					bool IsAtEndPoint() const;

					/**
					* Indicates whether the current location on the polyline is outside the extent of the polyline.
					* @return true -> location is outside extent, false -> location is within extent
					* @reqid 003.0025
					*/
					bool IsOutsideExtent() const;

					/**
					* Indicates whether the current location on the polyline is outside the extent along the 
					  extension away from the first vertex.
					* @return true -> location is in back of first vertex, false -> location is not
					* @reqid 003.0025
					*/
					bool IsInBackOfFirstVertex() const;

					/**
					* Indicates whether the current location on the polyline is outside the extent along the 
					  extension away from the last vertex.
					* @return true -> location is in front of last vertex, false -> location is not
					* @reqid 003.0025
					*/
					bool IsInFrontOfLastVertex() const;

					/**
					* Returns the end point that is near the polyline location. If this method is called on a location
					  that is not on the first or last segment's extensions, then NULL is returned.
					* @return a point if the location resides on the first segment's extension or on the last
							  segment's extension, otherwise NULL.
					* @reqid 003.0025
					*/
					const Point *GetNearEndPoint() const;

					/**
					* Determines which of the two input locations is closer to the location being called from
					* @param firstLocation - location to compare
					* @param secondLocation - location to compare
					* @param error - standard error object
					* @return true -> firstLocation closer than secondLocation to "this" Location, false-> it is not.
					* @reqid 003.0025
					*/
					bool FirstLocationCloserThanSecond(const Cursor *firstLocation,const Cursor *secondLocation,Error &error) const;

				private:
					void CalculateDistances() const;

				private:
					const Polyline* m_owner;
					int m_segmentNumber;
					float m_dxAlongSegment;
					float m_dyAlongSegment;
					mutable float m_distanceFromSegmentStart;
					mutable float m_distanceFromSegmentEnd;
					mutable bool m_distancesCalculated;
					Point m_currentLocation;
					float m_segmentDx;
					float m_segmentDy;
				};

				/**
				 * Used to give a cursor a direction in which it can move.
				 */
				class LABELINGENGINE_API DirectionalCursor : public Cursor
				{
				public:

					enum MovementDirection
					{
						MovementDirection_Forward,
						MovementDirection_Backward
					};

					/**
					 * Stores a location and a direction it can move.
					 * @param anchor - location on polyline
					 * @param movementDirection - direction it can move.
					 * @reqid 001.0063
					 */
					DirectionalCursor(const Cursor &cursor,MovementDirection movementDirection)
					{
						*((Polyline::Cursor *)this)=cursor;
						m_movementDirection=movementDirection;
					}

					/**
					 * &see DirectionalCursor(Cursor &cursor,MovementDirection movementDirection)
					 * @param movementDirection - direction it can move.
					 * @reqid 001.0063
					 */
					DirectionalCursor(MovementDirection movementDirection)
					{
						m_movementDirection=movementDirection;
					}

					/* 
					 * Default Constructor
					 * @reqid 001.0063
					 */
					DirectionalCursor()
					{
						m_movementDirection=MovementDirection_Forward;
					}

					/**
					 * Getter for movement direction
					 * @return direction in which location can move
					 * @reqid 001.0063
					 */
					MovementDirection GetMovementDirection() const
					{
						return m_movementDirection;
					}

					/**
					 * Setter for movement direction
					 * @param movementDirection - direction location can move.
					 * @reqid 001.0063
					 */
					void SetMovementDirection(MovementDirection movementDirection)
					{
						m_movementDirection=movementDirection;
					}

					/**
					 * Assignment operator
					 * @param cursor - directionless cursor
					 * @reqid 001.0063
					 */
					DirectionalCursor &operator =(const Cursor &cursor)
					{
						*((Polyline::Cursor *)this)=cursor;
						return *this;
					}

				private:

					MovementDirection m_movementDirection;
				};

				/**
				 * Used to represent a segment whose end points reside on a polyline and thus this segment
				   spans the polyline. The end points of the spanning segment are represented as cursors.
				 */
				class LABELINGENGINE_API SpanningSegment
				{
				public:

					/**
					 * Stores two nodes on a polyline defining a segment that spans the polyline
					 * @param fromNode - first endpoint of segment
					 * @param toNode - second endpoint of segment
					 * @param lengthBeforeFromNode - length of spanning segment that extends off the polyline in back of the from Node.
					 * @param lengthAfterToNode - length of spanning segment that extends off the polyline in front of the to Node.
					 * @reqid 001.0063
					 */
					SpanningSegment(Cursor &fromNode,Cursor &toNode,float lengthBeforeFromNode,float lengthAfterToNode)
					{
						m_fromNode=fromNode;
						m_toNode=toNode;
						m_lengthBeforeFromNode=lengthBeforeFromNode;
						m_lengthAfterToNode=lengthAfterToNode;

						m_segmentVector.SetDeltas(m_toNode.GetCurrentLocation()->GetX()-m_fromNode.GetCurrentLocation()->GetX(),m_toNode.GetCurrentLocation()->GetY()-m_fromNode.GetCurrentLocation()->GetY());
						m_segmentVector.Unitize();
					}

					/**
					 * Getter for from Node
					 * @return from Node
					 * @reqid 001.0063
					 */
					const Cursor &GetFromNode() const
					{
						return m_fromNode;
					}

					/**
					 * Getter for to Node
					 * @return to Node
					 * @reqid 001.0063
					 */
					const Cursor &GetToNode() const
					{
						return m_toNode;
					}

					/**
					 * Getter for length before from Node
					 * @return length before from node
					 * @reqid 001.0063
					 */
					float GetLengthBeforeFromNode() const
					{
						return m_lengthBeforeFromNode;
					}

					/**
					 * Getter for length after to Node
					 * @return length after to node
					 * @reqid 001.0063
					 */
					float GetLengthAfterToNode() const
					{
						return m_lengthAfterToNode;
					}

					/**
					 * Getter for unit vector along spanning segment.
					 * @return unit vector along spanning segment.
					 * @reqid 001.0063
					 */
					const Vector *GetSegmentVector() const
					{
						return &m_segmentVector;
					}

					/**
					 * Determines if the segment has zero length or not
					 * @return true->segment has zero length, false->it does not.
					 * @reqid 001.0063
					 */
					bool IsDegenerate() const;

				private:

					Cursor m_fromNode,m_toNode;
					float m_lengthBeforeFromNode;
					float m_lengthAfterToNode;
					Vector m_segmentVector;
				};

				/**
				 * Constructor.
				 * @param numVertices		the number of vertices that constitute the polyline
				 * @param error					an error object to be populated
				 * @reqid 001.0063
				 */
				Polyline( int numVertices, Error& error );

				/**
				 * Destructor.
				 * @reqid 001.0063
				 */
				virtual ~Polyline();

				Type GetType( ) const
				{
					return Type_Polyline;
				}

				/**
				 * Cleans the polyline by removing sections where the polyline goes back on itself. Does not
				   handle the case when this condition is satisfied on the first and last segments and the polyline
				   is interpreted as a polygon boundary
				   @return true->After cleaning, polyline no longer represents a non-degenerate polygon, false->polyline representing a polygon boundary is still a valid polygon
				 * @reqid simplification
				 */
				bool Clean();

				/**
				 * Examines each pair of vertices (i,i+1) and if the coordinates of the two vertices are both within 
				   Configuration::GetComparisonTolerance() of each other, the (i+1)th vertex is removed.
				 * @reqid TODO
				 */
				void RemoveCoincidentVertices();

				/**
				 * Resets the length of the polyline. All previous vertices are
				 * deleted and a new array of vertices is allocated.
				 * @param numVertices - New length of the polyline. Has to be > 1.
				 * @param error - [out] Possible error details.
				 * @reqid 001.0063
				 */
				void SetNumVertices( int numVertices, Error& error );

				/**
				 * Get the number of vertices in this polyline.
				 * @return the number of vertices allocated
				 * @reqid 001.0063
				 */
				int GetNumVertices() const
				{
					return m_numVertices;
				}

				/**
				 * @see IPolylineView::GetVertex
				 * @reqid 001.0063
				 */
				virtual void GetVertex( int i, const Point** vertex, Error& error ) const
				{
					if ( i >= m_numVertices )
					{
						SET_ERROR_NOMSG( error, ErrorCode_IndexOutOfRange );
					}
					else
					{
						*vertex = &( m_points[i] );
					}
				}

				/**
				 * Set coordinates of i-th vertex
				 * @param i index of the vertex to modify
				 * @param point set of new coordinates
				 * @param error error information
				 * @reqid 001.0063
				 */
				void SetVertex( int i, const Point& point, Error& error )
				{
					if ( i >= m_numVertices )
					{
						SET_ERROR_NOMSG( error, ErrorCode_IndexOutOfRange );
						return;
					}

					m_lengthCalculated = false;	// Require recalculation of length on next request.

					m_points[i] = point;

					UpdateBoundingBox( point.GetX(), point.GetY() );
				}

				/**
				 * Set coordinates of i-th vertex
				 * @param i index of the vertex to modify
				 * @param x new x coordinate
				 * @param y new y coordinate
				 * @param error error information
				 * @return 0 for success, error code otherwise
				 * @reqid 001.0063
				 */
				void SetVertex( int i, float x, float y, Error& error )
				{
					if ( i >= m_numVertices )
					{
						SET_ERROR_NOMSG( error, ErrorCode_IndexOutOfRange );
						return;
					}

					m_lengthCalculated = false;	// Require recalculation of length on next request.

					m_points[i].SetX( x );
					m_points[i].SetY( y );

					UpdateBoundingBox( x, y );
				}

				/**
				 * Copy vertices over from another polyline. 
				 * Caution: UpdateBoundingBox method should be called afterwards 
				 * if deferUpdatingOfTheBoundingBox is set to true since 
				 * the bounding box is not updated automatically in such case.
				 * @param inputPolyline - where to get the coordinates from
				 * @param startIndex - at which vertex start overwriting the vertices
				 * @param startSourceIndex - at which vertex start reading the source
				 * @param numVertices - how many vertices to copy?
				 * @param error - error info, can return ErrorCode_IndexOutOfRange if either 
				 *        polyline does not have enough vertices
				 * @param deferUpdatingOfTheBoundingBox - if true the bounding box is not 
				 *                                      updated after copying
				 * @return 
				 * @reqid 003.0030
				 */
				void SetVertices( const Polyline& inputPolyline, int startIndex, int startSourceIndex, 
					int numVertices, Error& error, bool deferUpdatingOfTheBoundingBox = false );

				/**
				 * Scale the coordinates of all vertices by specified factors.
				 * This operation assumes the vertices of the polyline are
				 * relative to the origin.
				 * @param	horzScaleFactor		the factor by which to multiply the x coordinate
				 * @param	vertScaleFactor		the factor by which to multiply the y coordinate
				 * @reqid 001.0063
				 */
				void Scale(float horzScaleFactor, float vertScaleFactor)
				{
					for (int i = 0; i < m_numVertices; i++)
					{
						m_points[i].Scale(horzScaleFactor, vertScaleFactor);
					}

                    UpdateBoundingBox();

					m_lengthCalculated = false;	// Require recalculation of length on next request.
				}

				/**
				* Rotate all the vertices about the specified origin.
				* @param origin		the point about which rotation is requested
				* @param sin_t		the sine of the angle through which rotation is requested
				* @param cos_t		the cosine of the angle through which rotation is requested
				* @reqid 001.0063
				*/
				void Rotate( const Point& origin, float sin_t, float cos_t)
				{
					if ( sin_t == 0 && cos_t == 1 )
					{
						return;
					}

					m_vertexAssigned = false;
					for ( int iVertex = 0; iVertex < m_numVertices; iVertex++ )
					{
						m_points[iVertex].Rotate( origin, sin_t, cos_t );
						UpdateBoundingBox( m_points[iVertex].GetX(), m_points[iVertex].GetY() );
					}
				}

				/**
				 * Create a deep copy of this instance
				 * @return the copy
				 * @reqid 001.0063
				 */
				virtual Polyline* Clone(Error& error) const
				{
					Polyline* clone = new Polyline();
					if (clone == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
                        return NULL;
					}

					Copy( *this, *clone, error );
					return clone;
				}

				/**
				 * Determine whether the polyline is directed interior to the right.
				 * @return true if interior to the right; false if not
				 * @reqid 001.0063
				 */
				bool FollowsInteriorToRightDirection() const;

				/**
				 * Computes polyline area.
				 * @return polyline area
				 * @reqid 001.0063
				 */
				double ComputeArea() const;

				/**
				 * Reverse the vertices so the polyline is directed interior to the right.
				 * Used in processing a glyph outline when a hole is orphaned,
				 * i.e., it cannot be matched with a parent boundary polyline.
				 * In this case, it is turned into the boundary of a new polygon.
				 * @reqid 001.0063
				 */
				void ReverseDirection();

				/**
				 * Given a horizontal line as defined by the input y coordinate, this method determines if this line
				 * intersects the polyline. If it does, it computes the right most and left most intersections. This
				 * method returns the indices of the segments that are intersected.
				 * @param ycoordinate - defines the horizontal line
				 * @param leftSegment - [out] index of segment containing the left most intersection
				 * @param leftXCoordinate - [out] X Coordinate of left most intersection
				 * @param rightSegment - [out] index of segment containing the right most intersection
				 * @param rightXCoordinate - [out] X Coordinate of right most intersection
				 * @reqid 002.0039
				 */
				void FindExtremeSegmentsIntersectedByHorizontalLine(float ycoordinate,int *leftSegment,float *leftXCoordinate,int *rightSegment,float *rightXCoordinate) const;

				/**
				 * Given a horizontal line as defined by the input y coordinate, this method determines if this line
				 * intersects the polyline. If it does, it computes intersection.
				 * @param ycoordinate - defines the horizontal line
				 * @param segmentIndex - [out] index of segment containing the intersection
				 * @param xCoordinate - [out] X Coordinate of the intersection
				 * @param startIndex - index to start searching polyline for intersection
				 * @param forwardDirection - search forward or backward along polyline
				 * @reqid 003.0025
				 */
				void FindIntersectionWithHorizontalLine(float yCoordinate, int &segmentIndex, 
																	float &xCoordinate, int startIndex, 
																	bool forwardDirection);

				/**
				 * Given a vertical line as defined by the input x coordinate, this method determines if this line
				 * intersects the polyline. If it does, it computes intersection.
				 * @param xCoordinate - defines the horizontal line
				 * @param segmentIndex - [out] index of segment containing the intersection
				 * @param yCoordinate - [out] Y Coordinate of the intersection
				 * @param startIndex - index to start searching polyline for intersection
				 * @param forwardDirection - search forward or backward along polyline
				 * @reqid 003.0025
				 */
				void FindIntersectionWithVerticalLine(float xCoordinate, int &segmentIndex, 
																float &yCoordinate, int startIndex, 
																bool forwardDirection);

				/**
				 * Get the polyine bounding box.
				 * @param llx		lower left corner x coordinate to be populated
				 * @param lly		lower left corner y coordinate to be populated
				 * @param urx		upper right corner x coordinate to be populated
				 * @param ury		upper right corner y coordinate to be populated
				 * @return the coordinates of the minimum enclosing upright rectangle
				 * @reqid 001.0063
				 */
				void GetBoundingBox(float* llx, float* lly, float* urx, float* ury) const;

				/**
				 * Get the polyine bounding box.
				 * @return the polyline's bounding box as an upright rectangle reference
				 * @reqid 001.0063
				 */
				const UprightRectangle* GetBoundingBox() const;

				/**
				 * Shift by specified amounts in x and y directions.
				 * @param	dx		the x coordinate shift
				 * @param	dy		the y coordinate shift
				 * @reqid 001.0063
				 */
				void Translate(float dx, float dy);

				/**
				 * @see IPolylineView::ClipToRectangle
				 * @param rect							the clipping window
				 * @param sine							Sine of the angle by which the clipping rectangle
				 *													has been rotated to become upright. Needed to rotate
				 *													the obstacle accordingly.
				 * @param cosine						Cosine of the angle by which the clipping rectangle
				 *													has been rotated to become upright.
				 * @param overlaps	[out]		the overlap geometries
				 * @param startVertex				the start vertex defining a sub-polyline to be clipped
				 * @param numberOfVertices	the number of vertices following the startVertex
				 *													defining a sub-polyline to be clipped
				 * @param error							an Error object
				 * @param analysisFlag			optional number passed into the clipper to avoid inclusion
				 *													of the same segment in more than one overlap tests.
				 *													If called twice with the same input flag segments already analyzed,
				 *													they will not be analyzed again.
				 * @reqid 001.0065
				 */
				void ClipToRectangle( Rectangle& rect, float sine, float cosine, 
					OverlapGeometries& overlaps,  int startVertex, 
					int numberOfVertices, Error& error, unsigned int analysisFlag = 0  );

				/**
				* @see IPolylineView::ClipToRectangle
				* @reqid 001.0065
				*/
				void ClipToRectangle( Rectangle& rect, float sine, float cosine, 
					OverlapGeometries& overlaps, Error& error, unsigned int analysisFlag = 0  );

				/**
				* Determine whether the specified point is within the polyline.
				* This polyline must be a closed polyline.
				* @param	point		the point to test
				* @return true if inside; false otherwise
				* @reqid 001.0063
				*/
				bool IsPointInClosedPolyline(const Point& point) const;

				/**
				* Computes the point on the feature closest to the chosen point.
				* @param projectedPoint			The point from which to project to the feature
				* @param treatAsClosedLoop	If true, do not project off polyline for segment n
				* @param distance						Computed distance from chosen point to closest point on feature
				* @param featurePoint				Point of intersection with feature, to be populated
				* @param vertexIndex				Computed vertex index, to be populated
				*							If it is negative, then the actual feature vertex indicated 
				*							by abs(feature index) is closest to the chosen point.
				*							If numvertices - 1 >= i > 0, then the closest point
				*							lies on the segment between
				*							vertex(feature index - 1) and vertex(feature index).
				*							If feature index == 0, then the closest point does not lie
				*							on the feature; it lies on the line starting at feature[0]
				*							and going in the direction from feature[1] to feature[0].
				*							If featureindex == numvertices, then the closest point lies
				*							on the line starting at feature[numvertices-1] and going in the
				*							direction from feature[numvertices-2] to feature[numvertices-1].
				* @param minRangeIndex the minimum vertex index to start search from
				* @param maxRangeIndex the maximum vertex index to search until
				*/
				void ProjectPointOntoPolyline(
					const Point* projectedPoint,
					bool treatAsClosedLoop,
					float *distance,
					Point* featurePoint,
					int *vertexIndex, int minRangeIndex=-1, int maxRangeIndex=-1 ) const;

				/**
				 * Builds a box around the given segment of the polyline
				 *
				 *  +-----------------------------------------------+  \
				 *  |                                               |  |
				 *  |    +-------------------------------------+    |  > box width
				 *  |    vn                                  vn+1   |  |
				 *  +-----------------------------------------------+  /
				 *                                              \___/
				 *  -segment 'n', between vertices                |
				 *   n and n+1                                   length extension
				 *
				 * @param segmentNumber number of the segment to buid the box around
				 * @param bufferWidth total width of the box (dimension parpendicular to the segment)
				 * @param extensionLength length of the box extension lengthwise
				 * @param box [out] the box
				 * @param error error info
				 * @reqid 001.0063
				 */
				void BuildSegmentBox( int segmentNumber, float bufferWidth, float extensionLength, Rectangle& box, Error& error );

				/**
				 * @see IPolylineView::GetStrokeBox
				 * @reqid 001.0064
				 */
				void GetStrokeBox( int segmentNumber, float strokeWidth, StrokeRectanglePolyline** box, Error& error );

				/**
				* Compute the length of the polyline
				* @return length of the polyline
				* @reqid 003.0019
				*/
				float GetLength() const;

				/**
				* Compute point along polyline by a certain distance
				* @param distance - distance along polyline to compute point
				* @return - the cursor positioned at the desired distance.
				* @reqid 003.0019
				*/
				Polyline::Cursor GetPointAtDistanceAlongPolyline(
					float distance ) const;

				/**
				* Compute distance of point along polyline
				* @param distance - distance along polyline of point
				* @param point - point along polyline to compute distance
				* @param index - index of segment of polyline
				* @reqid 003.0021
				*/
				void GetDistanceOfPointAlongPolyline(float &distance, const Point *point, int index) const;

				/**
				* Compute distance of point along polyline
				* @param distance - distance along polyline of point
				* @param point - point along polyline to compute distance
				* @reqid 003.0021
				*/

				void GetDistanceOfPointAlongPolyline(float &distance, const Point &point) const;

				/**
				* Compute point along polyline intersected by the given circle
				* @param center							center of circle
				* @param radius							radius of circle
				* @param searchForward			true to search forward (false for backward) along polyline
				* @param intersectionPoint	[out] point where line segment intersects circle
				* @param index							[out] the index of the start vertex of the intersected segment
				* @reqid 003.0021
				*/
				void FindSegmentIntersectedByCircle(
					const Point &center, float radius, bool searchForward, 
					Point &intersectionPoint, int &index ) const;

				/* Recomputes the bounding box based on all vertices of the polyline
				* @reqid 002.0036
				*/
				void UpdateBoundingBox();

				/**
				 * Searches for sections along a polyline on the input side for concavities. When a concavity is found,
				 * the distance across this concavity is compared against the gap tolerance. If the distance is smaller 
				 * than the gap tolerance, the concavity is closed and output polyline includes only the first and last
				 * vertices of the concavity resulting in a single line segment across the gap. If the distance is 
				 * larger than the gap tolerance, the concavity is kept and the output polyline includes all of the 
				 * vertices within the concavity. This method traverses the polyline once and closes off gaps where
				 * necessary. Then a second pass is made on the polyline produced by the first pass and the polyline
				 * is processed in the same way. This continues until a final pass makes no changes to the polyline
				 * from the previous pass.
				 * @param polylineSide - side of polyline on which to examine the concavities
				 * @param gapTolerance - minimum distance required across a concavity gap
				 *                       to keep the concavity in the output polyline
				 * @param filteredPolyline - [out] newly allocated polyline containing the filtered vertices.
				 *                           It must be freed by client when no longer needed
				 * @param error - an Error object
				 * @reqid 003.0023
				 */
				void Filter(PolylineSide polylineSide,float gapTolerance,Polyline **filteredPolyline,Error &error);

				/**
				 * Takes each segment pair in the polyline, shifts each of the segments away from the polyline
				 * perpendicularly on the input side by the input offset amount, computes the intersection of the shifted
				 * segments, and stores this intersection in the polyline replacing the original segment pair corner
				 * vertex. The first and last vertices of the polyline are simply shifted away from the polyline
				 * perpendicularly on the input side by the input offset amount and these replace the original polyline
				 * vertices. This produces an overall offsetting of the polyline by the offset amount on the input side.
				 * The method changes the Polyline's internal vertices instead of producing a new Polyline.
				 * @param polylineSide - polyline side on which to shift the segments.
				 * @param offsetAmount - the distance to shift the segments from the original polyline.
				 * @param treatAsClosedLoop	If true, turn the corner ffrom the last vertex back to the first
				 * @param error - standard error object
				 * @reqid 003.0023
				 */
				void Offset(PolylineSide polylineSide,float offsetAmount,bool treatAsClosedLoop,Error &error); 

				/**
				 * Attempts to change a possibly jagged polyline into a smooth one using circular arcs. It examines
				 * each segment pair in the polyline.  If the angular change of the segment pair is small or the length
				 * of either segment is small relative to the input average character width, then the segment pair's corner
				 * vertex is placed into the output polyline. Otherwise, the vertex is replaced with an appropriately 
				 * chosen circular arc that is tangent to both segments. The radius of the arc is chosen by dividing the
				 * segment pair's angular change by the desired inter-character angular change. This yields the ideal number
				 * of characters to curve within the segment pair concavity. If it is assumed that all of these characters
				 * have a width equal to the average character width and since these letters would sit on the circular arc,
				 * their total length approximate the length of the desired circular arc. Hence the length of the desired
				 * circular arc is approximated to be the number of desired character times the average character width.
				 * This length plus the segment pair's angular change produces the radius of the circular arc. 
				 * @param polylineSide - side of the polyline on which placement will take place.
				 * @param offsetAmount - the amount by which the polyline that is to be smoothened has been offsetted from
										 the original polyline.
				 * @param averageCharacterWidth - the average width of all characters in the string to curved.
				 * @param error - exception handling object.
				 * @reqid 003.0023
				 */
				void Smoothen(Polyline::PolylineSide polylineSide,float offsetAmount,float averageCharacterWidth,Error &error);

				/**
				* Searches for the segment in this polyline that is intersected by the input ray. 
				* @param ray - ray that potentially intersects the polyline
				* @param segment - [out] the index of the vertex on the polyline such that segment v[segment]-v[segment+1] is
				*				   intersected by the ray. If no intersection exists value of -1 is returned.
				* @param intersectionOnSegment - [out] container to receive the intersection point on the segment.
				* @reqid 001.0063
				*/
				void FindSegmentIntersectedByRay( const Ray &ray,int &segment,Point &intersectionOnSegment ) const;

				/**
				* Searches for the segment in this polyline that is intersected by the input ray. 
				* @param ray - ray that potentially intersects the polyline
				* @param startSegment - start the search at this vertex
				* @param treatPolylineAsClosed -	if set to true, the search continues around the polyline
				*																	in a loop-like way until the same segment is reached
				* @param whichIntersection -	the first encountered, the one furthest from the ray's anchor point,
				*															or the one nearest to the ray's anchor point.
				* @param segment - [out] the index of the vertex on the polyline such that segment v[segment]-v[segment+1] is
				*                  intersected by the ray. If no intersection exists value of -1 is returned.
				* @param intersectionOnSegment - [out] container to receive the intersection point on the segment.
				* @reqid 001.0063
				*/
				void FindSegmentIntersectedByRay( const Ray &ray, int startSegment,
					bool treatPolylineAsClosed, WhichIntersection whichIntersection,
					int &segment,Point &intersectionOnSegment ) const;

				/**
				* Searches for all points of intersection of the input ray with this polyline and
				* sorts them by distance from the ray anchor point in ascending order.
				* @param ray - ray that potentially intersects the polyline
				* @param treatPolylineAsClosed -	if set to true, the search continues around the polyline
				*																	in a loop-like way until the same segment is reached
				* @param sortedIntersections - [out] an array to be populated with all the intersection points
				*																		in ascending order by distance from the ray anchor point
				* @param numIntersectionsFound - [out] the number of intersections found
				* @reqid 001.0063
				*/
				void FindSortedIntersectionsWithRay( const Ray &ray, bool treatPolylineAsClosed,
					TimeTaggedIntersection* sortedIntersections, int* numIntersectionsFound ) const;

				/**
				 * Get the length of n-th segment.
				 * @param startVertex - index of the segment start vertex
				 * @param length - [out] length of the segment given
				 * @param error - error info
				 * @reqid 003.0030
				 */
				void GetSegmentLength( int startVertex, float* length, Error& error ) const
				{
					if ( startVertex < 0 || startVertex > m_numVertices-2 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_IndexOutOfRange );
						return;
					}

					*length = m_points[startVertex+1].DistanceFrom( m_points[startVertex] );
				}

				/**
				 * Retrieve a vector describing a segment of the polyline.
				 * @param startVertex - start vertex of the segment
				 * @param segment - [out] vector to be set with the segment dimensions
				 * @param error - error info
				 * @reqid 003.0030
				 */
				void GetSegmentAsVector( int startVertex, Vector& segment, Error& error ) const
				{
					if ( startVertex < 0 || startVertex > m_numVertices-2 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_IndexOutOfRange );
						return;
					}

					segment.SetDeltas( m_points[startVertex+1].GetX()-m_points[startVertex].GetX(),
						m_points[startVertex+1].GetY()-m_points[startVertex].GetY() );
				}

				/**
				 * Retrieve a segment of the polyline.
				 * @param startVertex - start vertex of the segment
				 * @param segment - [out] segment to be set 
				 * @param error - error info
				 * @reqid 003.0030
				 */
				void GetSegment( int startVertex, LineSegment& segment, Error& error ) const
				{
					if ( startVertex < 0 || startVertex > m_numVertices-2 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_IndexOutOfRange );
						return;
					}

					segment.SetPoint0(m_points[startVertex].GetX(), m_points[startVertex].GetY());
					segment.SetPoint1(m_points[startVertex+1].GetX(), m_points[startVertex+1].GetY());
				}

				/* Determines whether this is a closed polyline.
				 * @return true if closed; false if not
				 * @reqid 002.0039
				 */
				bool IsClosed() const;

				/* Determine the closer end point of a segment given to a given point.
				 * @param segtmentIndex - index of segment
				 * @param point - point on the segment
				 * @param vertexIndex - [out] index of closer endpoint of segment
				 * @param vertex - the closer vertex
				 * @reqid 003.0021
				 */
				void GetCloserEndPointofSegment(int segmentIndex, Point &point, int &vertexIndex, Point &vertex) const;



				/**
				* Finds first segment that is intersected by the input circle when searching from "startSearchHere"
				  in the movementDirection and returns the vertex of this segment that is farther along the polyline
				  from "startSearchHere".
				* @param startSearchHere - search is begun at this location
				* @param searchDirection - direction (Forward or Backward) in which to search
				* @param circle - circle intersecting with the polyline
				* @param intersectedSegment - [out] index of vertex forming intersected segment.
				* @return true if intersection found, false if no intersection found
				* @reqid 001.0063
				*/
				bool FindSegmentIntersectedByCircle(const Cursor &startSearchHere,DirectionalCursor::MovementDirection searchDirection,const Circle &circle,int *intersectedSegment) const;

				/**
				* Finds first segment that is intersected by the input line when searching from "startSearchHere"
				  in the movementDirection direction and returns the vertex on this segment that is farther from along
				  the polyline from "startSearchHere". If startSearchHere is outside the extent of the polyline at vertex 0 and 
				  the input line intersects the polyline extension at vertex 0, then intersectedSegment is filled
				  with -1 and true is returned. If startSearchHere is outside the extent at vertex n-1 and the 
				  input line intersects the polyline extension at vertex n-1, then intersectedSegment is filled 
				  with n and true is returned.
				* @param startSearchHere - search is begun at this location
				* @param searchDirection - direction (Forward or Backward) in which to search
				* @param line - line intersecting with the polyline
				* @param intersectedSegment - [out] index of vertex forming intersected segment.
				* @return true if intersection found, false if no intersection found
				* @reqid 001.0063
				*/
				bool FindSegmentIntersectedByLine(const Cursor &startSearchHere,DirectionalCursor::MovementDirection searchDirection,const Line &line,int *intersectedSegment) const;

                /**
                 * Finds first segment that is intersected by the input linesegment when searching from "startSearchHere"
                 in the movementDirection direction and returns the vertex on this segment that is farther from along
                 the polyline from "startSearchHere".
                 * @param startSearchHere - search is begun at this location
                 * @param searchDirection - direction (Forward or Backward) in which to search
                 * @param lineSegment - linesegment intersecting with the polyline
                 * @param intersectedSegment - [out] index of vertex forming intersected segment.
                 * @return true if intersection found, false if no intersection found
                 * @reqid 001.0063
                 */
                bool FindSegmentIntersectedByLineSegment(const Cursor &startSearchHere,DirectionalCursor::MovementDirection searchDirection,const LineSegment &lineSegment,int *intersectedSegment) const;
            
				/**
				* Starting at the input startLocation and moving in the direction specified in startLocation (Forward
				  or Backward), this method searches for the segment intersected by a circle centered at startLocation
				  with radius equal to the input segmentLength. The segment from the startLocation to this intersection
				  is stored in the spanning segment. If the spanning segment cannot be of length segmentLength due
				  to insufficient polyline length, then the amount of this missing length is stored in the spanning
				  segment as well.
				* @param startLocation - location on polyline to serve as From node of the spanning segment.
				* @param segmentLength - desired length of the spanning segment.
				* @return a spanning segment starting at startLocation.
				* @reqid 001.0063
				*/
				SpanningSegment GenerateSpanningSegment(DirectionalCursor startLocation,float segmentLength) const;

				/**
				 * Performs a test on the polyline geometry and reports true if at least one segment
				 * of zero length is discovered.
				 * @return - True if there are zero-length segments. False otherwise.
				 * @reqid 001.0086
				 */
				bool HasZeroLengthSegments() const;

				friend class PolylineSimplifier;

				/**
				 * Simplifies polyline using Ramer�Douglas�Peucker algorithm.
				 * Note: in extreme case simplification can reduce polyline to a point (if polyline is closed
				 * and smaller than given tolerance)
				 * @param tolerance simplification precision (maximum distance between the original and simplified polyline)
				 * @param error error object
				 * @reqid simplification
				 */
				void Simplify(float tolerance, Error &error);

			private:
				bool IgnoreSegment( int* ignoreSegments, int numIgnoreSegments, int testSegment ) const;
				static int CompareTimeTaggedIntersections( const void* test1, const void* test2 );

			protected:
				/**
				 * Make deep copy of one instance's members to another
				 * @param src
				 * @param dest
				 * @reqid 001.0063
				 */
				static void Copy( const Polyline& src, Polyline& dest, Error& error );

			private:
				/* Grows the bounding box of the polyline, as needed, to accommodate the additional vertex.
				 * @param x	The X coordinate of the additional vertex
				 * @param y	The Y coordinate of the additional vertex
				 * @reqid 001.0063
				 */
				void UpdateBoundingBox( float x, float y );
			
#ifdef S11N
			public:
				/**
				* @see ISerializable::Serialize
				 */
				virtual void Serialize( ISerializer& emitter, Error& error ) const;
			
				/**
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				void Deserialize( IDeserializer& deserializer, Error& error );


				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static Polyline* DeserializeNew( IDeserializer& deserializer, Error& error );
#endif

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				UprightRectangle m_boundingBox;
				Rectangle* m_segmentBoxes;
				float* m_segmentBoxesWidths;
				bool m_vertexAssigned;
				mutable float m_length;

				unsigned int* m_analysisFlags;

			protected:
                mutable bool m_lengthCalculated;

				/**
                 * Default constructor. Only for internal use of the class and derivatives.
                 * Created object is NOT usable for the client.
				 * @reqid 001.0063
                 */
                Polyline() : m_points(NULL), m_numVertices(0), m_segmentBoxes(NULL), m_segmentBoxesWidths(NULL),
                    m_lengthCalculated(false), m_vertexAssigned(false), m_analysisFlags(NULL)
                {}

				/**
				* array of the vertices of the polyline
				* @reqid 001.0063
				*/
				Point* m_points;
				int m_numVertices;

		};

		/**
		 * A 'virtual' polyline - a polyline parasiting on another polyline.
		 * Needed to save memory on operations like clipping, which basically
		 * uses part of the original feature geometry and introduces a start and/or
		 * end vertex where the feature is crossing the cropping rectangle.
		 * An example of polyline and a cropping box:
		 * <code>
		 *    0         1
		 *    +---------+
		 * a            |(s)                    b
		 * +-----------(+)---------------------+
		 * |            |      3               | cropping box
		 * |            +------+               |
		 * |            2      |               |
		 * |                   |               |
		 * |            5+-----+4              |
		 * |             |    original         |(e)
		 * |             +--------------------(+)------+
		 * |             6    feature          |       7
		 * +-----------------------------------+
		 * c                                   d
		 *
		 * </code>
		 *
		 * When the clipping of a feature is done agains the rectangle a,b,c,d
		 * the output is a new polyline composed of points (s),2, 3, 4, 5, 6, (e).
		 * As we can see a continuous overlapping part of a feature can introduce
		 * at most two new vertives which were not present in the original feature.
		 * Leveraging this assumption the VPolyline stores only locations of
		 * (s) and (e) points, index of point no. 2, number of points to include
		 * from original polyline and a reference to the polyline itself.
		 * In the boundary cases, such as: whole polyline fits within the cropping
		 * rectangle or it starts or ands inside of the rectangle ((s) and/or (e) points
		 * are not there) special flags are set to use the original polyline points instead
		 * as the first/last point of the VPolyline.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API ClippedPolyline : public IPolylineView
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0065
				 */
				enum ErrorCode
				{
					ErrorCode_IndexOutOfRange = 1,
					ErrorCode_NoParentPolyline,
					ErrorCode_NotEnoughVerticesToInherit,
					ErrorCode_NotEnoughVertices,
				};

				/**
				 * Construct the virtual polyline given:
				 * @param parentPolyline	the polyline on which the virtual one will be based
				 * @param startIndex			index at which the string of inherited points starts
				 * @param error						an Error object
				 * @reqid 001.0065
				 */
				ClippedPolyline( Polyline *parentPolyline, int startIndex,
					Error& error );

				/**
				 * Construct the virtual polyline given:
				 * @param parentPolyline	the polyline on which the virtual one will be based
				 * @param startXingX			the X coordinate of the first point of this polyline - ownership of point taken
				 * @param startXingY			the Y coordinate of the first point of this polyline - ownership of point taken
				 * @param startXingSet		flag indicating whether the first owned vertex has been set
				 * @param error						an Error object
				 * @reqid 001.0065
				 */
				ClippedPolyline( Polyline *parentPolyline, float startXingX, float startXingY, bool startXingSet,
					Error& error );

				/**
				 * Construct the virtual polyline given:
				 * @param parentPolyline the polyline on which the virtual one will be based
				 * @param startIndex index at which the string of inherited points starts
				 * @param numberOfInheritedVertices number of common vertices
				 * @param startXingX			the X coordinate of the first point of this polyline - ownership of point taken
				 * @param startXingX			the Y coordinate of the first point of this polyline - ownership of point taken
				 * @param endXingX				the X coordinate of the last point of this polyline - ownership of point taken
				 * @param endXingY				the Y coordinate of the last point of this polyline - ownership of point taken
				 * @param endXingSet			flag indicating whether the last owned vertex has been set
				 * @param error						an Error object
				 * @reqid 001.0065
				 */
				ClippedPolyline( Polyline *parentPolyline, int startIndex,
					int numberOfInheritedVertices, float startXingX, float startXingY, bool startXingSet,
					float endXingX, float endXingY, bool endXingSet,
					Error& error );

				/**
				 * @see IPolylineView::GetType
				 * @reqid 001.0065
				 */
				Type GetType( ) const
				{
					return Type_ClippedPolyline;
				}

				/**
				 * Add a vertex inherited from the original polyline.
				 * @param iVertex	index of the vertex of the original polyline
				 * @param error		an Error object
				 * @reqid 001.0065
				 */
				void AddInheritedVertex( int iVertex, Error& error );

				/**
				 * Get the i-th vertex of the polyline
				 * @param i				index of the vertex to retrieve
				 * @param vertex	the retrieved point, the address of the internal instance
				 *								the client should NOT deallocate it
				 * @param error		an Error object
				 * @reqid 001.0065
				 */
				virtual void GetVertex( int i, const Point** vertex, Error& error ) const;

				/**
				* @see IPolylineView::ClipToRectangle
				* @reqid 001.0064
				*/
				void ClipToRectangle( Rectangle& rect, 
					float sine, float cosine, OverlapGeometries& overlaps, 
					Error& errorInfo, unsigned int analysisFlag = 0  );


				/**
				 * Destructor.
				 * @reqid 001.0065
				 */
				~ClippedPolyline()
				{
				}

				/**
				 * @see IPolylineView::Clone
				 * @reqid 001.0065
				 */
				virtual ClippedPolyline* Clone(Error& error) const
				{
					return new ClippedPolyline(
						m_parentPolyline, m_startIndex,
						m_numberOfInheritedVertices,
						m_startXing.GetX(), m_startXing.GetY(), m_startXingSet, m_endXing.GetX(), 
						m_endXing.GetY(), m_endXingSet, error );
				}

				// GETTERS AND SETTERS
				/**
				 * Get the total number of vertices, included inherited ones and those
				 * on the clipping rectangle boundary, resulting from clipping.
				 * @see IPolylineView::GetNumVertices
				 * @reqid 001.0065
				 */
				virtual int GetNumVertices() const
				{
					return m_numVertices;
				}

				/*
				 * Assign the first boundary point encountered in this section of the
				 * clipped polyline that results from clipping.
				 * This can be viewed as an entry crossing point.
				 * @param startXing the intersection point.
				 * @reqid 001.0065
				 */
				/* Not used for now, commented out to avoid dead code.
				 *  Bring back if turns out to be needed.
				 *  void SetStartXing(Point* startXing)
				 *  {
				 *       if (m_startXing != NULL)
				 *       {
				 *         delete m_startXing;
				 *         --m_numVertices;
				 *       }
				 *       m_startXing = startXing;
				 *
				 *       if (startXing != NULL)
				 *         ++m_numVertices;
				 *  }
				 */

				/**
				 * Assign the second boundary point encountered in this section of the
				 * clipped polyline that results from clipping.
				 * This can be viewed as an exit crossing point.
				 * @param endXingSet	a flag requesting either setting or unsetting the exit crossing point.
				 * @param endXingX		if set, the X coordinate of the last point of this polyline - ownership of point taken
				 * @param endXingY		if set, the Y coordinate of the last point of this polyline - ownership of point taken
				 * @reqid 001.0065
				 */
				void SetEndXing( bool endXingSet, float endXingX = 0.0f, float endXingY = 0.0f );

				/**
				* Gets StartIndex
				* @return StartIndex
				* @reqid 001.0065
				*/
				int GetStartIndex() const 
				{ 
					return m_startIndex; 
				}

				/**
				* Gets NumberOfInheritedVertices, the number of vertices inherited from the parent polyline.
				* @return NumberOfInheritedVertices
				* @reqid 001.0065
				*/
				int GetNumberOfInheritedVertices() const 
				{ 
					return m_numberOfInheritedVertices;
				}

				/**
				* Gets ParentPolyline, the polyline being clipped.
				* @return ParentPolyline
				* @reqid 001.0065
				*/
				const Polyline* GetParentPolyline() const 
				{ 
					return m_parentPolyline; 
				}

				/**
				 * @see IPolylineView::GetStrokeBox
				 * @reqid 001.0064
				 */
				void GetStrokeBox( int segmentNumber, float strokeWidth, StrokeRectanglePolyline** box, Error& error );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				Polyline* m_parentPolyline;

				/**
				 * Index of the first vertex in the parent polyline
				 * included in this clipped section.
				 * @reqid 001.0065
				 */
				int m_startIndex;
				int m_numberOfInheritedVertices;
				int m_numVertices;

				/**
				 * The first boundary point in this section of clipped polyline
				 * that resulted from clipping rather than being inherited from
				 * the parent polyline if m_startXingSet is true.
				 * @reqid 001.0065
				 */
				Point m_startXing;

				/**
				 * @see m_startXing
				 */
				bool m_startXingSet;

				/**
				 * The last boundary point in this section of clipped polyline
				 * that resulted from clipping rather than being inherited from
				 * the parent polyline if m_endXingSet is true.
				 * @reqid 001.0065
				 */
				Point m_endXing;

				/**
				* @see m_endXing
				*/
				bool m_endXingSet;
		};

		/**
		 * Wraps a part of Polyline and exposes interface to behave like a regular polyline.
		 * @reqid 001.0064
		 */
		class LABELINGENGINE_API PolylinePart : public IPolylineView
		{
		public:

			/**
			* Possible error codes
			* @reqid 001.0064
			*/
			enum ErrorCode
			{
				ErrorCode_IndexOutOfRange = 1,
				ErrorCode_NoParentPolyline,
				ErrorCode_NotEnoughVerticesToInherit,
				ErrorCode_NotEnoughVertices,
				ErrorCode_AllocationError,
			};

			/**
			 * Parameterized constructor
			 * @param parentPolyline the polyline which will be used as a base
			 * @param startVertex number of the first vertex to be included in the constructed part
			 * @param numberOfVertices number of vertices used by this part (min 1)
			 * @param errorInfo error information
			 * @return 0 on success, non-zero error code otherwise.
			 * @reqid 001.0064
			 */
			PolylinePart( Polyline& parentPolyline, int startVertex, int numberOfVertices, Error &errorInfo );

			~PolylinePart()
			{
			}

			/**
			* @see IPolylineView::GetType
			* @reqid 001.0065
			*/
			Type GetType( ) const
			{
				return Type_PolylinePart;
			}

			/**
			* @see IPolylineView::GetVertex
			* @reqid 001.0064
			*/
			virtual void GetVertex( int i, const Point **vertex, Error& error ) const;

			/**
			* Get the number of vertices in this polyline.
			* @return the number of vertices allocated
			* @reqid 001.0064
			*/
			int GetNumVertices() const
			{
				return m_numberOfVertices;
			}

			/**
			* @see IPolylineView::ClipToRectangle
			* @reqid 001.0064
			*/
			void ClipToRectangle( Rectangle& rect, float sine, float cosine, 
				OverlapGeometries& overlaps, Error& errorInfo, unsigned int analysisFlag = 0  )
			{
				m_parentPolyline->ClipToRectangle( rect, sine, cosine, overlaps, m_startVertex, m_numberOfVertices, 
					errorInfo, analysisFlag );
			}

			/**
			* Gets StartVertex
			* @return StartVertex
			* @reqid 001.0064
			*/
			int GetStartVertex() const 
			{ 
				return m_startVertex; 
			}

			/**
			* Sets StartVertex
			* @param val StartVertex
			* @reqid 001.0064
			*/
			void SetStartVertex(int val) 
			{ 
				m_startVertex = val; 
			}

			/**
			* Sets NumberOfVertices
			* @param val NumberOfVertices
			* @reqid 001.0064
			*/
			void SetNumVertices(int val) 
			{ 
				m_numberOfVertices = val; 
			}

			PolylinePart* Clone(Error& error) const
			{
				PolylinePart* clone = new PolylinePart( *m_parentPolyline, m_startVertex, m_numberOfVertices, error );
				if ( clone == NULL )
				{
					SET_ERROR_NOMSG( error, ErrorCode_AllocationError );
				}
				return clone;
			}

			/**
			* @see IPolylineView::GetStrokeBox
			* @reqid 001.0064
			*/
			void GetStrokeBox( int segmentNumber, float strokeWidth, StrokeRectanglePolyline** box, Error& error );

			/****************************************************************************************
			THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
			TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
			****************************************************************************************/
		private:
			Polyline *m_parentPolyline;
			int m_startVertex;
			int m_numberOfVertices;
		};

		/**
		 * Polyline based on a rectangle - iterates over rectangle vertices starting
		 * in lower left and continuing in a counter-clockwise manner until the lower left
		 * corner is reached again.
		 * Always has 5 vertices.
		 * @reqid 001.0065
		 */
		class LABELINGENGINE_API RectanglePolyline : public IPolylineView
		{
		public:
			enum ErrorCode
			{
				ErrorCode_AllocationError = 1,
				ErrorCode_IndexOutOfRange,
				ErrorCode_NotImplemented,
			};

			/**
			 * Constructor.
			 * @param parent the rectangle which should be adapted as polyline.
			 * Copy is made. Resulting object is independent of the parent rectangle.
			 * @reqid 001.0065
			 */
			RectanglePolyline( const Rectangle& parent );

			/**
			* @see IPolylineView::GetType
			* @reqid 001.0065
			*/
			Type GetType( ) const
			{
				return Type_RectanglePolyline;
			}

			/**
			* @see IPolylineView::GetVertex
			* @reqid 001.0065
			*/
			void GetVertex( int i, const Point** vertex, Error& error ) const;

			/**
			* @see IPolylineView::GetNumVertices
			* @reqid 001.0065
			*/
			int GetNumVertices() const;

			/**
			* Dectructor
			* @reqid 001.0065
			*/
			~RectanglePolyline()
			{}

			/**
			* Create a deep copy of the object
			* @reqid 001.0065
			*/
			IPolylineView* Clone(Error& error) const;

			/**
			* @see IPolylineView::GetStrokeBox
			* @reqid 001.0064
			*/
			void GetStrokeBox( int segmentNumber, float strokeWidth, StrokeRectanglePolyline** box, Error& error );

			/**
			* @see IPolylineView::ClipToRectangle
			* @reqid 001.0064
			*/
			void ClipToRectangle( Rectangle& rect, float sine, float cosine, 
				OverlapGeometries& overlaps, Error& error, unsigned int analysisFlag = 0  );

			/**
			 * Get the parent rectangle. Address of the member is returned.
			 * It shall not be deallocated by the client.
			 * @param rect [out] the parent rectangle.
			 * @reqid 001.0064
			 */
			void GetParentRectangle( Rectangle** rect );

			/**
			 * Set the wrapped rectangle. Copy is made.
			 * @param rect the rectangle to be a new parent for the polyline.
			 * @reqid 001.0064
			 */
			void SetParentRectangle( const Rectangle& rect );

			/****************************************************************************************
			THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
			TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
			****************************************************************************************/
		private:
			Rectangle m_parentRectangle;
		};

		class StrokeRectanglePolyline : public RectanglePolyline
		{
		public:
			/**
			 * Constructor.
			 * @param parentRectangle
			 * @param parentPolyline
			 * @param segmentNumber
			 * @reqid 002.0039
			 */
			StrokeRectanglePolyline( const Rectangle& parentRectangle, Polyline* parentPolyline, int segmentNumber )
				: RectanglePolyline( parentRectangle ), m_parentPolyline( parentPolyline ),
				m_segmentNumber( segmentNumber )
			{

			}

			/**
			 * Destructor.
			 * @reqid 002.0039
			 */
			~StrokeRectanglePolyline()
			{}

			/**
			* @see IPolylineView::GetType
			* @reqid 001.0065
			*/
			Type GetType( ) const
			{
				return Type_StrokeRectanglePolyline;
			}

			/**
			* Gets ParentPolyline - the polyline which stroked segment is
			* encapsulated.
			* @return ParentPolyline
			* @reqid 002.0039
			*/
			Polyline* GetParentPolyline() const 
			{ 
				return m_parentPolyline; 
			}

			/**
			* Sets ParentPolyline
			* @see GetParentPolyline
			* @return ParentPolyline
			* @reqid 002.0039
			*/
			void SetParentPolyline(Polyline* polyline)
			{ 
				assert( polyline != NULL );

				m_parentPolyline = polyline;
			}

			/**
			* Gets SegmentNumber - number of the segment which is encapsulated.
			* @return SegmentNumber
			* @reqid 002.0039
			*/
			int GetSegmentNumber() const 
			{ 
				return m_segmentNumber; 
			}

			/**
			* Sets SegmentNumber
			* @see GetSegmentNumber
			* @return SegmentNumber
			* @reqid 002.0039
			*/
			void SetSegmentNumber( int segmentNumber )
			{ 
				m_segmentNumber = segmentNumber; 
			}
		private:
			Polyline* m_parentPolyline;
			int m_segmentNumber;

		};

		/**
		 * A collection of polylines
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Polylines : public ClonableLinkedList < Polyline, Polylines >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				Polylines()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~Polylines()
				{
				}
		};

		/**
		* Holds indexes into the owning polyline representing hit points from the 
		* left and right sides at the min and max y coordinate values.
		* @author Phil Grenetz
		* @reqid 001.0063
		*/
		class LABELINGENGINE_API PolylineHitPointGroup
		{
		public:
			/**
			* Default constructor.
			* @reqid 001.0063
			*/
			PolylineHitPointGroup()
				:	m_parentPolyline( NULL ),
					m_hitPointIndexFromRightAtMaxY(-1),
					m_hitPointIndexFromLeftAtMaxY(-1),
					m_hitPointIndexFromRightAtMinY(-1),
					m_hitPointIndexFromLeftAtMinY(-1)
			{
			}

			/**
			* Constructor.
			* @reqid 001.0063
			*/
			PolylineHitPointGroup( const Polyline* parentPolyline )
				:	m_parentPolyline( parentPolyline ),
					m_hitPointIndexFromRightAtMaxY(-1),
					m_hitPointIndexFromLeftAtMaxY(-1),
					m_hitPointIndexFromRightAtMinY(-1),
					m_hitPointIndexFromLeftAtMinY(-1)
			{
				assert( parentPolyline != NULL);
			}

			void SetParentPolyline( const Polyline* parentPolyline )
			{
				m_parentPolyline = parentPolyline;

				m_hitPointIndexFromRightAtMaxY = -1;
				m_hitPointIndexFromLeftAtMaxY = -1;
				m_hitPointIndexFromRightAtMinY = -1;
				m_hitPointIndexFromLeftAtMinY = -1;
			}

			/* 
			* If a horizontal line at y = MaximumYOfPointSymbol is drawn,
			* computes the first vertex hit by the line from the left right and sides.
			* @param hitPointFromLeftSide		-	populated with pointer to the vertex hit first
			*																	by the line y = MaximumYOfPointSymbol from the left side
			* @param hitPointFromRightSide	-	populated with pointer to the vertex hit first
			*																	by the line y = MaximumYOfPointSymbol from the right side
			* @reqid 002.0036
			*/
			void GetHitPointsAtMaxY(const Point **hitPointFromLeftSide,const Point **hitPointFromRightSide);

			/* 
			* If a horizontal line at y = MinimumYOfPointSymbol is drawn,
			* computes the first vertex hit by the line from the left right and sides.
			* @param hitPointFromLeftSide		-	populated with pointer to the vertex hit first
			*																	by the line y = MinimumYOfPointSymbol from the left side
			* @param hitPointFromRightSide	-	populated with pointer to the vertex hit first
			*																	by the line y = MinimumYOfPointSymbol from the right side
			* @reqid 002.0036
			*/
			void GetHitPointsAtMinY(const Point **hitPointFromLeftSide,const Point **hitPointFromRightSide);

			/* Get the index of the vertex hit first from the right side by the line.
			* @return	-	index of the vertex hit first from the right side by the line
			*						y = MaximumYOfPointSymbol
			* @reqid 002.0036
			*/
			int GetHitPointIndexFromRightAtMaxY()
			{
				const Point *hitPointFromRightSide,*hitPointFromLeftSide;

				if (m_hitPointIndexFromRightAtMaxY==-1)
					GetHitPointsAtMaxY(&hitPointFromLeftSide,&hitPointFromRightSide);

				return m_hitPointIndexFromRightAtMaxY;
			}

			/* Get the index of the vertex hit first from the left side by the line.
			* @return	-	index of the vertex hit first from the left side by the line
			*						y = MaximumYOfPointSymbol
			* @reqid 002.0036
			*/
			int GetHitPointIndexFromLeftAtMaxY()
			{
				const Point *hitPointFromRightSide,*hitPointFromLeftSide;

				if (m_hitPointIndexFromLeftAtMaxY==-1)
					GetHitPointsAtMaxY(&hitPointFromLeftSide,&hitPointFromRightSide);

				return m_hitPointIndexFromLeftAtMaxY;
			}

			/* Get the index of the vertex hit first from the right side by the line
			* @return	-	index of the vertex hit first from the right side by the line
			*						y = MinimumYOfPointSymbol
			* @reqid 002.0036
			*/
			int GetHitPointIndexFromRightAtMinY()
			{
				const Point *hitPointFromRightSide,*hitPointFromLeftSide;

				if (m_hitPointIndexFromRightAtMinY==-1)
					GetHitPointsAtMinY(&hitPointFromLeftSide,&hitPointFromRightSide);

				return m_hitPointIndexFromRightAtMinY;
			}

			/* Get the index of the vertex hit first from the left side by the line
			* @return	-	index of the vertex hit first from the left side by the line
			*						y = MinimumYOfPointSymbol
			* @reqid 002.0036
			*/
			int GetHitPointIndexFromLeftAtMinY()
			{
				const Point *hitPointFromRightSide,*hitPointFromLeftSide;

				if (m_hitPointIndexFromLeftAtMinY==-1)
					GetHitPointsAtMinY(&hitPointFromLeftSide,&hitPointFromRightSide);

				return m_hitPointIndexFromLeftAtMinY;
			}

		private:
			const Polyline* m_parentPolyline;
			int m_hitPointIndexFromRightAtMaxY;
			int m_hitPointIndexFromLeftAtMaxY;
			int m_hitPointIndexFromRightAtMinY;
			int m_hitPointIndexFromLeftAtMinY;
		};

		/**
		 * Polygon geometrical definition
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Polygon
		{
			public:
				/**
				 * A factory class to instantiate a new Polygon or its derived class.
				 * @reqid N/A.
				 */
				class Factory
				{
				public:
					/**
					 * Constructor.
					 * @param error					an error object to be populated
					 * @reqid 001.0063
					 */
					virtual Polygon *Create( Error& error ) = 0;

					/**
					 * Needs to be cloneable
					 * @param error[out] - Error info.
					 * @return - The clone or NULL on error.
					 * @reqid N/A.
					 */
					virtual Factory* Clone( Error& error ) = 0;

                    /**
                     * Virtual destructor needed.
                     * @reqid N/A.
                     */
                    virtual ~Factory()
                    {}
                };

				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_BoundaryCantBeNull,
				};

				/**
				 * Default constructor
				 * @param error		an error object to be populated
				 * @reqid 001.0063
				 */
				Polygon()
					:m_boundary( NULL )
				{
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				virtual ~Polygon()
				{
					delete m_boundary;
				}

				/**
				 * Get the boundary polyline.
				 * @reqid 001.0063
				 */
				Polyline* GetBoundary() const
				{
					return m_boundary;
				}

				/**
				 * Assign a boundary polyline and take ownership. Can't be NULL.
				 * Client should NOT deallocate the outline after assigning.
				 * It is assumed that the boundary and holes for a polygon
				 * passed into this API by the client are interior to the right.
				 * @reqid 001.0063
				 */
				void SetBoundary( Polyline* boundary, Error& error )
				{
					if ( boundary == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_BoundaryCantBeNull )
					}

					delete m_boundary;
					m_boundary = boundary;
				}

				/**
				 * Get the list of polylines representing the holes of the polygon.
				 * @reqid 001.0063
				 */
				Polylines* GetHoles()
				{
					return &m_holes;
				}

				/**
				 * Get the list of polylines representing the holes of the polygon.
				 * @reqid 001.0063
				 */
				const Polylines* GetHoles() const
				{
					return &m_holes;
				}

				/**
				 * Create a deep copy of this instance
				 * @param error an Error object
				 * @return the copy
				 * @reqid 001.0063
				 */
				virtual Polygon* Clone(Error& error) const
				{
					Polygon* clone = new Polygon();
					if (clone == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					if (error)
					{
						delete clone;
						clone = NULL;
					}
					else
					{
						Copy( *this, *clone, error );
						if (error)
						{
							delete clone;
							clone = NULL;
						}
					}
					return clone;
				}

				/**
				 * Scale the coordinates of all vertices of all polylines by specified factors.
				 * This operation assumes all the vertices of the polyline are
				 * relative to the origin.
				 * @param	horzScaleFactor		the factor by which to multiply the x coordinate
				 * @param	vertScaleFactor		the factor by which to multiply the y coordinate
				 * @reqid 001.0063
				 */
				void Scale(float horzScaleFactor, float vertScaleFactor)
				{
					m_boundary->Scale(horzScaleFactor, vertScaleFactor);

					for ( Polylines::Iterator hole = m_holes.Begin(); !hole.IsNull(); hole++ )
					{
						hole->Scale(horzScaleFactor, vertScaleFactor);
					}
				}

				/**
				 * Shift by specified amounts in x and y directions.
				 * @param	dx		the x coordinate shift
				 * @param	dy		the y coordinate shift
				 * @reqid 001.0063
				 */
				void Translate(float dx, float dy);

				/**
				 * Determine whether the specified point is within the polygon boundary.
				 * @param	point		the point to test
				 * @return true if inside; false otherwise
				 * @reqid 001.0063
				 */
				bool IsPointInPolygon(const Point& point);

				/**
				 * Get the coordinates of the polygon bounding box.
				 * @return the coordinates of the minimum enclosing upright rectangle
				 * @reqid 001.0063
				 */
				void GetBoundingBox(float* llx, float* lly, float* urx, float* ury) const;

				/**
				* Get the polygon bounding box.
				* @return the polygon boundary's bounding box as an upright rectangle pointer
				* @reqid 001.0063
				*/
				const UprightRectangle* GetBoundingBox() const;

				/**
				 * Clip the segments of the constituent polylines to an upright rectangle.
				 * @param rect the clipping window
				 * @param sine Sine of the angle by which the clipping rectangle
				 *             has been rotated to become upright. Needed to rotate
				 *             the obstacle accordingly.
				 * @param cosine Cosine of the angle by which the clipping rectangle
				 *             has been rotated to become upright.
				 * @param overlaps [out] the overlap geometries
				 * @param error error information
				 * @reqid 001.0065
				 */
				void ClipToRectangle(
					Rectangle& rect, float sine, float cosine, 
					OverlapGeometries& overlaps, Error& error, unsigned int analysisFlag = 0  );

				/**
				* Rotate self about the specified origin.
				* @param anchorPoint - point about which to rotate
				* @param sin_t - sine of angle by which to rotate
				* @param cos_t - cosine of angle by which to rotate
				* @reqid 004.0032
				*/
				void Rotate( const Point &anchorPoint, float sin_t, float cos_t );

				/**
				 * Get the minimum bounding rectangle - a rectangle with the minimum surface area. 
				 * The rectangle is oriented in a way that the longer side is always at the bottom.
				 * @param minRectangle - [out] The min bounding rectangle.
				 * @param error - Error info. Allocation failure may occur.
				 * @reqid 003.0032
				 */
				void GetMinimumBoundingRectangle( const Rectangle** minRectangle, Error& error ) const
				{
					*minRectangle = NULL;

					ComputeMinimumBoundingRectangle( error );
					if ( error )
					{
						return;
					}

					*minRectangle = &m_minimumBoundingRectangle;
				}

				/**
				 * Validate the polygon
				 * @param validationErrors - [out] Validation issues are reported here.
				 * @param error - [out] Other critical errors which may occur during 
				 *                the validation process are reported here. They would NOT
				 *                be caused by invalid input data.
				 * @reqid 001.0086
				 */
				void Validate( bool* valid, ErrorDetails** validationErrors, Error& error ) const;
            
                /**
                 * Generate the centroid. Based on mapnik algorithm: 
                 * https://github.com/mapnik/mapnik/blob/8c2f15c94a6de1540bb50a33dc8f7f052a94c943/include/mapnik/geom_util.hpp#L322
                 * @reqid TODO
                 */
                void GetCentroid( Point& centroid, Error& error ) const;
			
			protected:
				/**
				* Make deep copy of one instance's members to another
				* @param src
				* @param dest
				* @reqid 001.0063
				*/
				static void Copy( const Polygon& src, Polygon& dest, Error& error )
				{
					Polyline* boundary	= NULL;

					if (src.m_boundary != NULL)
					{
						boundary = src.m_boundary->Clone(error);
						if (error)
						{
							return;
						}
						dest.SetBoundary( boundary, error );
						if (error)
						{
							return;
						}
					}

					dest.m_holes.Clear();

					CollectionUtility<Polyline>::CopyElements( src.m_holes, dest.m_holes, error );
					if (error)
					{
						delete boundary;
						dest.SetBoundary( NULL, error );
						if (error)
						{
							return;
						}
						return;
					}

					dest.m_minimumBoundingRectangle = src.m_minimumBoundingRectangle;
					return;
				}
			private:

				/**
				* Compute the minimum bounding rectangle of the polygon
				* @param error error information
				* @reqid 003.0032
				*/
				void ComputeMinimumBoundingRectangle(Error& error) const;
			
#ifdef S11N
			public:
				/**
				 * Serialization.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				void Serialize( ISerializer& emitter, Error& error ) const;

				/**
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				void Deserialize( IDeserializer& deserializer, Error& error );


				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static Polygon* DeserializeNew( IDeserializer& deserializer, Error& error );
#endif		
			
				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				mutable Rectangle m_minimumBoundingRectangle;

			protected:
				Polyline* m_boundary;
				Polylines m_holes;
		};

		/**
		 * Holds a feature shape. This can be any defined shape (Point, Line, Area or Unknown).
		 * This simple base class should never be instantiated by a client.
		 * Derived class constructors, hoever, may need to invoke its constructor.
		 * If we define Clone, it will be pure virtual and, then, this class
		 * will be abstract and non-instantiable.  For now, protected ctors do the job.
		 * @author Andrzej Ciereszko
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Shape
		{
			public:

				enum ErrorCode
				{
					ErrorCode_AllocationError = 1,
					ErrorCode_AllocationFailure = 2,
				};
				/**
				 *	Constructor.
				 *	@param	featureType			the type of the feature
				 * @reqid 001.0063
				 */
				Shape( FeatureType featureType )
					:m_featureType( featureType )
				{
				}

				/**
				 * Get the type of feature described by the shape.
				 * @return the type
				 * @reqid 001.0063
				 */
				virtual FeatureType GetFeatureType() const
				{
					return m_featureType;
				}

				/**
				 * Ensure all the destructors are virtual.
				 * @reqid 001.0063
				 */
				virtual ~Shape()
				{
				};

				/**
				 * Clip the polylines of the associated polygons to an upright rectangle.
				 * @param rect the clipping window
				 * @param sine Sine of the angle by which the clipping rectangle
				 *             has been rotated to become upright. Needed to rotate
				 *             the obstacle accordingly.
				 * @param cosine Cosine of the angle by which the clipping rectangle
				 *             has been rotated to become upright.
				 * @param overlaps [out] the overlap geometries
				 * @param error error information
				 * @reqid 001.0063
				 */
				virtual void ClipToRectangle(
					Rectangle& rect, float sine, float cosine, 
					OverlapGeometries& overlaps, Error& error, unsigned int analysisFlag = 0 ) const = 0;

				/**
				* Creates copy of self.
				* @param error an error object to be populated
				* @return copy of self
				* @reqid 001.0076
				*/
				virtual Shape* Clone( Error& error ) const = 0;

				/**
				* Get the shape bounding box.
				* @return the shape's bounding box as an upright rectangle pointer
				* @reqid 001.0063
				*/
				virtual const UprightRectangle* GetBoundingBox() = 0;

                /**
                 * Update shape bounding box.
                 */
                virtual void UpdateBoundingBox() = 0;

				/**
				 * Validate the overall shape 
				 * @param valid - [out] True if shape is valid. False otherwise, details appended
				 *                through validationErrors
				 * @param validationErrors - [out] Validation issues are reported here.
				 * @param error - [out] Other critical errors which may occur during 
				 *                the validation process are reported here. They would NOT
				 *                be caused by invalid input data.
				 * @reqid 001.0086
				 */
				virtual void Validate( bool* valid, ErrorDetails** validationErrors, Error& error ) const = 0;

#ifdef GEOMETRY_STATS
                /**
                 * Get the total count of vertices of the shape.
                 * @return - Total vertex count.
                 * @reqid PERFORMANCE
                 */
                virtual unsigned int GetVerticesCount() = 0;
#endif
            
		protected:
				/**
				 * Make deep copy of one instance's members to another
				 * @param src
				 * @param dest
				 * @reqid 001.0063
				 */
				static void Copy( const Shape& src, Shape& dest, Error& error )
				{
					dest.m_featureType = src.m_featureType;
				}

#ifdef S11N
				
			public:
				/**
				 * Serialization.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				void Serialize( ISerializer& emitter, Error& error ) const;

				/**
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				void Deserialize( IDeserializer& deserializer, Error& error );

				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static Shape* DeserializeNew( IDeserializer& deserializer, Error& error );

			protected:
				/**
				 * Child specific serialization. Needs to be implemented by all 
				 * deriving classes.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				virtual void _Serialize( ISerializer& emitter, Error& error ) const = 0;

				/** 
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 */
				virtual void _Deserialize( IDeserializer& deserializer, Error& error ) = 0;
#endif

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				FeatureType m_featureType;
		};

		/**
		 *	Holds a polygon shape defined by a polygon and its boundary line width.
		 *	@author Phil Grenetz
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API PolygonShape
			: public Shape
		{
			public:
            
                enum PolygonShapeType
                {
                    PolygonShapeType_Vector = 1,
                    PolygonShapeType_Raster
                };
            
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_WidthHasToBeNonNegative,
				};

				/**
				 * Default constructor
				 * @reqid 001.0063
				 */
				PolygonShape(PolygonShapeType polygonShapeType = PolygonShapeType_Vector)
					:Shape( FeatureType_Area ), m_polygon( NULL ), m_width(0), m_polygonShapeType(polygonShapeType)
				{
				}

				/**
				 * Constructor
				 * @param polygon the polygon constituting the shape
				 * @reqid 001.0063
				 */
				PolygonShape( Polygon *polygon, PolygonShapeType polygonShapeType = PolygonShapeType_Vector )
					:Shape( FeatureType_Area ), m_polygon( NULL ), m_width(0), m_polygonShapeType(polygonShapeType)
				{
					SetPolygon( polygon );
				}
            
                PolygonShapeType GetPolygonShapeType()
                {
                    return m_polygonShapeType;
                }

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~PolygonShape()
				{
					delete m_polygon;
				}

				/**
				 * Get the polygon that defines this shape
				 * @reqid 001.0063
				 */
				Polygon* GetPolygon() const
				{
					return m_polygon;
				}

				/**
				 * Ownership is taken.
				 * @see GetPolygon
				 * @reqid 001.0063
				 */
				void SetPolygon( Polygon* polygon )
				{
					delete m_polygon;

					m_polygon = polygon;
				}

				/**
				 * Get the boundary width of the polygon that defines this shape
				 * @reqid 001.0063
				 */
				float GetWidth() const
				{
					return m_width;
				}

				/**
				 * @see GetWidth
				 * @param width - the new polygon shape width. Has to be >= 0.
				 * @param error - [out] error info.
				 * @reqid 001.0063
				 */
				void SetWidth( float width, Error& error )
				{
					if ( width < 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_WidthHasToBeNonNegative )
					}
					m_width = width;
				}

				/**
				* Create a deep copy of this instance.
				* @param error error information
				* @return the copy
				* @reqid 001.0063
				*/
				virtual PolygonShape* Clone( Error& error ) const;

				/**
				 * Clip the polylines of the associated polygon to an upright rectangle.
				 * @param rect the clipping window
				 * @param sine Sine of the angle by which the clipping rectangle
				 *             has been rotated to become upright. Needed to rotate
				 *             the obstacle accordingly.
				 * @param cosine Cosine of the angle by which the clipping rectangle
				 *             has been rotated to become upright.
				 * @param overlaps [out] the overlap geometries
				 * @param error error information
				 * @reqid 001.0065
				 */
				void ClipToRectangle(
					Rectangle& rect, float sine, float cosine, 
					OverlapGeometries& overlaps, Error& error, unsigned int analysisFlag = 0 ) const;

				/**
				* Get the shape bounding box.
				* @return the polygon boundary's bounding box as an upright rectangle pointer
				* @reqid 001.0063
				*/
				const UprightRectangle* GetBoundingBox()
				{
					return m_polygon->GetBoundingBox();
				}
            
                /**
                 * Update shape bounding box.
                 */
                void UpdateBoundingBox()
                {
                    m_polygon->GetBoundary()->UpdateBoundingBox();
                }

				void Validate( bool* valid, ErrorDetails** validationErrors, Error& error ) const;
            
#ifdef GEOMETRY_STATS
                /**
                 * @see Shape::GetVerticesCount
                 * @reqid PERFORMANCE
                 */
                unsigned int GetVerticesCount();
#endif

#ifdef S11N
			protected:
				/**
				 * Child specific serialization.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				virtual void _Serialize( ISerializer& emitter, Error& error ) const;

				/** 
				 * @see Shape::_Deserialize
				 * @reqid N/A
				 */
				virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif

			/**
			 * Make deep copy of one instance's members to another
			 * @param src
			 * @param dest
			 * @reqid 001.0063
			 */
			static void Copy( const PolygonShape& src, PolygonShape& dest, Error& error )
			{
				Shape::Copy(src, dest, error);
				if (error) {
					return;
				}
				dest.m_width = src.m_width;
				dest.m_polygon = src.m_polygon->Clone(error);
			}

			/****************************************************************************************
            THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
            TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
            ****************************************************************************************/
			private:
				float m_width;
				Polygon* m_polygon;
                PolygonShapeType m_polygonShapeType;
		};
		
		class PointShape;

		/**
		 *	Holds a polyline shape defined by a polyline and its line width.
		 *	@author Phil Grenetz
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API PolylineShape : public Shape
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_WidthHasToBeNonNegative,
				};

			public:
				/**
				 * Default constructor
				 * @reqid 001.0063
				 */
				PolylineShape()
					:Shape( FeatureType_Line ), m_width( 0 ), m_polyline( NULL ), m_decorations(NULL)
				{
				}

				/**
				 * Constructor
				 * @param polyline the polyline constituting the shape
				 * @param width the thickness of the polyline
				 * @param error and Error object
				 * @reqid 001.0063
				 */
				PolylineShape( Polyline *polyline, float width, Error& error )
					:Shape( FeatureType_Line ), m_polyline( polyline ), m_decorations(NULL)
				{
					SetWidth( width, error );
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~PolylineShape()
				{
					delete m_polyline;
                    delete m_decorations;
				}

				/**
				 * The polyline object that defines this shape
				 * @reqid 001.0063
				 */
				Polyline* GetPolyline() const
				{
					return m_polyline;
				}

				/**
				 * Ownership is taken
				 * @see GetPolyline
				 * @reqid 001.0063
				 */
				void SetPolyline( Polyline* polyline )
				{
					delete m_polyline;

					m_polyline = polyline;
				}

				/**
				 * Get the width of the polyline that defines this shape
				 * @reqid 001.0063
				 */
				float GetWidth() const
				{
					return m_width;
				}

				/**
				 * Set the width of the polyline that defines this shape. Needs to be >=0.
				 * @param width - new width of the polyline shape.
				 * @param error - [out] error info. ErrorCode_WidthHasToBeNonNegative can be returned.
				 * @reqid 001.0063
				 */
				void SetWidth( float width, Error& error )
				{
					if ( width < 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_WidthHasToBeNonNegative );
						return;
					}
					m_width = width;
				}

				/**
				* Create a deep copy of this instance.
				* @param error error information
				* @return the copy
				* @reqid 001.0063
				*/
				PolylineShape* Clone( Error& error ) const
				{
					Polyline* polyline = m_polyline->Clone(error);
					if ( error )
					{
						return NULL;
					}

					PolylineShape* clone = new PolylineShape( polyline, m_width, error );
					if (clone == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else if (error)
					{
						delete clone;
						clone = NULL;
					}
					else if(m_decorations)
					{
                        clone->m_decorations = new LinkedList<PointShape>();
                        
						CollectionUtility<PointShape>::CopyElements( *m_decorations, *clone->m_decorations, error );
						clone->Copy( *this, *clone, error );
					}

					return clone;
				}

				/**
				 * Clip the associated polyline to an upright rectangle.
				 * @param rect the clipping window
				 * @param sine Sine of the angle by which the clipping rectangle
				 *             has been rotated to become upright. Needed to rotate
				 *             the obstacle accordingly.
				 * @param cosine Cosine of the angle by which the clipping rectangle
				 *             has been rotated to become upright.
				 * @param overlaps [out] the overlap geometries
				 * @param error error information
				 * @reqid 001.0065
				 */
				void ClipToRectangle(
					Rectangle& rect, float sine, float cosine, 
					OverlapGeometries& overlaps, Error& error, unsigned int analysisFlag = 0 ) const;

				/**
				* Get the shape bounding box.
				* @return the polyline's bounding box as an upright rectangle pointer
				* @reqid 001.0063
				*/
				const UprightRectangle* GetBoundingBox()
				{
					return m_polyline->GetBoundingBox();
				}

            
                /**
                 * Update shape bounding box.
                 */
                void UpdateBoundingBox()
                {
                    m_polyline->UpdateBoundingBox();
                }
            
				/**
				 * @see Shape::Validate
				 * @reqid 001.0086
				 */
				void Validate( bool* valid, ErrorDetails** validationErrors, Error& error ) const;
            
#ifdef GEOMETRY_STATS
                /**
                 * @see Shape::GetVerticesCount
                 * @reqid PERFORMANCE
                 */
                unsigned int GetVerticesCount();
#endif

                /**
                 * Sets the collection of decorations.
                 * @reqid TODO
                 */
                void SetDecorations(LinkedList<PointShape>* decorations)
                {
                    m_decorations = decorations;
                }

                /**
				 * Gets the collection of decorations.
				 * @return a linked list of point shapes which represent decorations on the line.
				 * @reqid TODO
				 */
				LinkedList<PointShape>* GetDecorations()
				{
					return m_decorations;
				}

#ifdef S11N
			protected:
				/**
				 * Child specific serialization.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				virtual void _Serialize( ISerializer& emitter, Error& error ) const;

				/** 
				 * @see Shape::_Deserialize
				 * @reqid N/A
				 */
				virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				float m_width;
				Polyline* m_polyline;
				LinkedList<PointShape>* m_decorations;
		};

		/**
		 * A collection of polygons
		 * It is assumed that the boundary and holes for a polygon
		 * passed into this API by the client are interior to the right.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Polygons : public ClonableLinkedList < Polygon, Polygons >
		{
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_InstanceAlreadyExists = 1,
					ErrorCode_AllocationFailure
				};

			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @param ownsElements - if the collection should take ownership over the 
				 *                       items.
				 * @reqid 001.0063
				 */
				Polygons( bool ownsElements = true ) : ClonableLinkedList< Polygon, Polygons >( ownsElements )
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~Polygons()
				{
				}

				/**
				 * Generate a convex hull enclosing all the polygons in the collection.
				 * @return the convex hull polygon
				 * @reqid 001.0063
				 */
				void GenerateConvexHull(int numVertices, Polyline** convexHull, Error& error);

			private:
				/**
				 * Compute the vertices of the convex hull of a polygon.
				 * @param P - [in-out] array of vertices - those for a polygon boundary or the
				 *						aggregate of multiple polygon boundaries in, closed convex hull out.
				 *						This array includes sufficient elements for a copy of the first vertex.
				 * @param n - [in] number of unique polygon boundary vertices
				 *						This parameter excludes the repeated first vertex.
				 * @reqid 001.0063
				 */
				static int ComputeConvexHullVertices(float **P, int n);

				/**
				 * Compare two polyline vertices and report whether transiting from one to the other
				 * occurs in the counter-clockwise direction.
				 * This function is used for sorting the vertices of the "lower" portion of the polyline
				 * for counter-clockwise traversal.
				 * @param a - pointer to an array containing the x and y coordinate of the current vertex.
				 * @param b - pointer to an array containing the x and y coordinate of the next vertex.
				 * @reqid 001.0063
				 */
				static int CompareLower(const void *a, const void *b);

				/**
				 * Compare two polyline vertices and report whether transiting from one to the other
				 * occurs in the counter-clockwise direction.
				 * This method is used for sorting the vertices of the "upper" portion of the polyline
				 * for counter-clockwise traversal.
				 * @param a - pointer to an array containing the x and y coordinate of the current vertex.
				 * @param b - pointer to an array containing the x and y coordinate of the next vertex.
				 * @reqid 001.0063
				 */
				static int CompareUpper(const void *a, const void *b); 

				/**
				 * Determine whether three successive polyline vertices occur in counter-clockwise order traversal.
				 * This method is used for making the upper and lower parts of the convex hull.
				 * @param P - [in] array of vertices of the polygon boundary.
				 * @param i - index of the 1st polygon boundary vertex of the group
				 * @param j - index of the 2nd boundary vertex of the group
				 * @param k - index of the 3rd polygon boundary vertex of the group
				 * @reqid 001.0063
				 */
				static int CounterClockWise(float **P, int i, int j, int k);

				/**
				 * Compute the vertices of the upper or lower part of the convex hull of a polygon.
				 * @param V - [in-out] array of vertices - polygon boundary in, convex hull part out
				 * @param n - [in] remaining number of unique polygon boundary vertices not processed
				 * @param cmp - pointer to the applicable sort function
				 * @see CompareLower
				 * @see CompareUpper
				 * @reqid 001.0063
				 */
				static int MakeChain(float** V, int n, int (*cmp)(const void*, const void*));

				/**
				 * Allocate a two dimensional array of dynamically allocated objects.
				 * @param dim1 - the size of the first dimension, i.e., the number of rows
				 * @param dim2 - the size of the second dimension, i.e., the number of columns
				 * @param size - the size in number of bytes of each element of the array
				 * @reqid 001.0063
				 */
				static void **MallocMatrix (int dim1, int dim2, int size);

				/**
				 * Free the specified two dimensional array and its elements
				 * @param mat - the array to be freed
				 * @reqid 001.0063
				 */
				static void FreeMatrix(void **mat );
		};

		/**
		 * Holds a point shape. This can be a single point, or a point with an outline.
		 * @author Andrzej Ciereszko
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API PointShape : public Shape
		{
			public:
				enum ErrorCode
				{
					ErrorCode_ShapeNotPositioned = 1,
					ErrorCode_AllocationFailure
				};

				/**
				 * Default constructor.
				 * @reqid 001.0063
				 */
				PointShape()
					:	Shape( FeatureType_Point ),
						m_pointSymbol( NULL ), m_virtualSymbol( NULL ), m_pointSymbolAngle(0.0f)
				{
				}

				/**
				 *	Constructor.
				 * @param x		The x coordinate of the location
				 * @param y		The y coordinate of the location
				 * @reqid 001.0063
				 */
				PointShape( float x, float y )
					:	Shape( FeatureType_Point ),
						m_pointSymbol( NULL ), m_virtualSymbol( NULL ), m_pointSymbolAngle(0.0)
				{
					m_point.SetXY(x,y);

				}

				/**
				 * Constructor.
				 * @param x						the x coordinate of location
				 * @param y						the y coordinate of location
				 * @param pointSymbol	the symbol rendered at location
				 * @reqid 001.0063
				 */
				PointShape( float x, float y, PointSymbol *pointSymbol )
					:	Shape( FeatureType_Point ),
						m_pointSymbol( pointSymbol ), m_virtualSymbol( NULL ), m_pointSymbolAngle(0.0)
				{
					m_point.SetXY(x,y);

				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~PointShape();

				//GETTERS & SETTERS BEGIN

				/**
				 * Get the point member of the point shape.
				 * @reqid 001.0063
				 */
				const Point* GetPoint() const
				{
					return &m_point;
				}

				/**
				 * Set the coordinates of the point member of the point shape
				 * and reposition the point symbol if any.
				 * @param x   x coordinate of the point
				 * @param y   y coordinate of the point
				 * @reqid 001.0063
				 */
				void SetPoint( float x, float y );

				/**
				 * Get the point symbol that defines this shape
				 * @reqid 001.0063
				 */
				PointSymbol* GetPointSymbol() const
				{
					return m_pointSymbol;
				}

				/**
				 * @see GetPointSymbol
				 * Ownership is taken.
				 * The point must be set first.
				 * Symbol is cloned and clone is translated to this location.
				 * @param pointSymbol		the point symbol
				 * @param error					an Error object
				 * @reqid 001.0063
				 */
				void SetPointSymbol( const PointSymbol* pointSymbol, Error& error );

				//GETTERS and SETTERS END

				/**
				 * Clip the polylines of the associated polygons to an upright rectangle.
				 * @param rect the clipping window
				 * @param sine Sine of the angle by which the clipping rectangle
				 *             has been rotated to become upright. Needed to rotate
				 *             the obstacle accordingly.
				 * @param cosine Cosine of the angle by which the clipping rectangle
				 *             has been rotated to become upright.
				 * @param overlaps [out] the overlap geometries
				 * @param error error information
				 * @reqid 001.0065
				 */
				void ClipToRectangle(
					Rectangle& rect, float sine, float cosine, 
					OverlapGeometries& overlaps, Error& error, unsigned int analysisFlag = 0 ) const;

				/**
				* Create a deep copy of this instance
				* @param error error information
				* @return the copy
				* @reqid 001.0063
				*/
				PointShape* Clone( Error& error ) const;

				/**
				* Get the shape bounding box.
				* @return the point symbol's bounding box as an upright rectangle pointer
				* @reqid 001.0063
				*/
				const UprightRectangle* GetBoundingBox();

                /**
                 * Update shape bounding box.
                 */
                void UpdateBoundingBox();
            
				/**
				* Gets VirtualSymbol. This polyline (a square) is a "virtual" symbol used in obstacle
				* detection if the point symbol is not symbolized at all, but has label requests,
				* to provide a little extra room for it. It's size is controlled by the 
				* Configuration::NonSymbolizedPointFeatureObstacleSize property.
				* @return - VirtualSymbol
				* @reqid 002.0039
				*/
				void GetVirtualSymbol( Polyline** virtualSymbol, Error& error );

				/**
				 * @see Shape::Validate
				 * @reqid 001.0086
				 */
				void Validate( bool* valid, ErrorDetails** validationErrors, Error& error ) const;

				/**
				 * Sets the angle to add to the point style angle whose sum is the amount by which this point shape's 
				   point symbol is rotated.
				   @param pointSymbolAngle - instance angle of this point shape in radians
				 * @reqid TODO
				 */
				void SetPointSymbolAngle(float pointSymbolAngle);

				/**
				 * @See SetPointSymbolAngle
				   @return point symbol angle
				 * @reqid TODO
				 */
				float GetPointSymbolAngle();

#ifdef GEOMETRY_STATS
                /**
                 * @see Shape::GetVerticesCount
                 * @reqid PERFORMANCE
                 */
                unsigned int GetVerticesCount();
#endif

#ifdef S11N
			protected:
				/**
				 * Child specific serialization.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				virtual void _Serialize( ISerializer& emitter, Error& error ) const;

				/** 
				 * @see Shape::_Deserialize
				 * @reqid N/A
				 */
				virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:

				PointSymbol* m_pointSymbol;
				Point m_point;
				UprightRectangle m_bBox;
				Polyline* m_virtualSymbol;
				float m_pointSymbolAngle;
		};

		//forward Declaration
		class TopologicalPolygon;

		/**
		 * @reqid Simplification
		 */
		class LABELINGENGINE_API SharedBoundary
		{
		public:

			/**
			 * Possible error codes
			 * @reqid simplification
			 */
			enum ErrorCode
			{
				ErrorCode_AllocationFailure = 1
			};

			SharedBoundary(int numVertices,
						   TopologicalPolygon *leftPolygon,
						   TopologicalPolygon *rightPolygon,
						   Error &error);
			~SharedBoundary();

			//TODO comment me
 			Polyline *GetPolyline();
 			//TODO comment me
 			const TopologicalPolygon *GetLeftPolygon() const;
 			//TODO comment me
 			const TopologicalPolygon *GetRightPolygon() const;

			//TODO comment me
			class LABELINGENGINE_API PtrCollection : public LinkedList<SharedBoundary>
			{
			public:

				PtrCollection( bool ownsElements ) : LinkedList<SharedBoundary>( ownsElements )
				{
				}
			};

			//TODO comment me
			class LABELINGENGINE_API Collection : public LinkedList<SharedBoundary>
			{
			};

		private:

			friend class TopologicalPolygon;
			friend class PolygonTopologyGraph;

			Polyline *m_polyline;
			TopologicalPolygon *m_leftPolygon; //can be NULL
			TopologicalPolygon *m_rightPolygon; //can be NULL
			int m_firstSegmentLeftPolygonIndex,m_firstSegmentRightPolygonIndex;
			int m_lastSegmentLeftPolygonIndex,m_lastSegmentRightPolygonIndex;
		};

		/**
		 * @reqid Simplification
		 */
		class LABELINGENGINE_API TopologicalPolygon
		{
		public:

			/**
			 * Possible error codes
			 * @reqid simplification
			 */
			enum ErrorCode
			{
				ErrorCode_AllocationFailure = 1,
				ErrorCode_TopologyError
			};

			TopologicalPolygon(const Polygon *sourcePolygon,Error &error);
			~TopologicalPolygon();

			//TODO comment me
			void BuildPolygonFromSharedBoundaries(Polygon **polygon,Error &error) const;
			//TODO comment me
			const Polygon *GetSourcePolygon() const;
			//TODO comment me
			SharedBoundary::PtrCollection *GetSharedBoundaries();
			//TODO comment me
			bool IsDegenerate()
			{
				return m_isDegenerate;
			}
			//TODO comment me
			void SetDegenerate(bool isDegenerate)
			{
				m_isDegenerate=isDegenerate;
			}

			//TODO comment me
			class LABELINGENGINE_API Collection : public LinkedList<TopologicalPolygon>
			{
			};

		private:

			//TODO comment me
			class LABELINGENGINE_API SharedBoundaryRef
			{
			public:

				SharedBoundaryRef(SharedBoundary *sharedBoundary);
				~SharedBoundaryRef(){}

				SharedBoundary *m_sharedBoundary;
				SharedBoundaryRef *m_next;
			};

			SharedBoundary::PtrCollection *m_sharedBoundaries; //ordered list of shared boundaries for this polygon only
			const Polygon *m_sourcePolygon;
			bool m_isDegenerate;
		};
		
		/**
		 * Example usage scenario:
		 * A client feeds the PolygonTopologyGraph a set of polygons via AddPolygon(), 
		 * calls BuildGraph() to build topological view, calls GetSharedBoundaries() to iterate over 
		 * them and processes each shared boundary�s polyline, calles GetTopologicalPolygons() to 
		 * iterate over the polygons and for each TopologicalPolygon, 
		 * calles BuildPolygonFromSharedBoundaries() to get a newly built polygon from 
		 * its shared boundaries which have been processed.
		 * @reqid Simplification
		 */
		class LABELINGENGINE_API PolygonTopologyGraph
		{
		public:

			/**
			 * Possible error codes
			 * @reqid simplification
			 */
			enum ErrorCode
			{
				ErrorCode_AllocationFailure = 1,
				ErrorCode_BuildFailed
			};

			/**
			 * Create new polygon topology graph.
			 * @param error error object
			 * @param ownsElements (optional) if true PolygonTopologyGraph will take ownership of polygons
			 *                     passed using AddPolygon method
			 * @reqid simplification
			 */
			PolygonTopologyGraph(Error &error, bool ownsElements = false);

			~PolygonTopologyGraph();

			/**
			 * Add a polygon for topological analysis. If PolygonTopologyGraph was created to own polygons
			 * (ownsElement=true) then polygons will be deleted by PolygonTopologyGraph.
			 * Returns ErrorCode_InvalidPolygon if given polygon is invalid.
			 * @param polygon polygon for analysis
			 * @param error error object
			 * @reqid simplification
			 */
			void AddPolygon(Polygon *polygon,Error &error);

			//TODO comment me
			SharedBoundary::Collection::Iterator GetSharedBoundaries();
			//TODO comment me
			TopologicalPolygon::Collection::Iterator GetTopologicalPolygons();
			//TODO comment me
			void BuildGraph(Error &error);

			/**
			 * Return iterator to collection of input polygons (use <code>GetTopologicalPolygons</code>
			 * to get collection of topological polygons).
			 * @return iterator to collection of input polygons
			 * @reqid simplification
			 */
			Polygons::Iterator GetPolygons()
			{
				return m_polygons->Begin();
			}

		private:

			int CompareSharedSegments(const LineSegment &lineSegment1,const LineSegment &lineSegment2);

			class ConsumedSegment
			{
			public:

				ConsumedSegment(const LineSegment *lineSegment);
				~ConsumedSegment();

				const LineSegment *m_lineSegment;
				ConsumedSegment *m_next;
			};

			class SharedSegment
			{
			public:

				SharedSegment();
				~SharedSegment();

				bool InsertConsumedSegment(const LineSegment &lineSegment,bool performInsertion,Error &error);
				bool PartOfSameSharedBoundaryAs(const SharedSegment &sharedSegment) const;
				
				static int CompareSharedSegments(const void *firstElem,const void *secondElem);

				TopologicalPolygon *m_rightPolygon,*m_leftPolygon;
				int m_rightPolygonSegmentNumber,m_leftPolygonSegmentNumber;
				LineSegment m_segment;
				float m_originDistance;
				float m_slope; //TODO: may not need to compute up front
				bool m_processed;
				SharedSegment *m_next;
				SharedSegment *m_fromNeighbor,*m_toNeighbor;
				ConsumedSegment *m_consumedSegments;
			};

			class SharedSegmentNode
			{
			public:

				SharedSegmentNode();
				~SharedSegmentNode();

				static int CompareSharedSegmentNodes(const void *firstElem,const void *secondElem);

				const Point *m_node;
				SharedSegment *m_sourceSegment;
				bool m_processed;
			};

			Polygons *m_polygons;
			SharedBoundary::Collection m_sharedBoundaries; //all shared boundaries from all polygons
			TopologicalPolygon::Collection m_topologicalPolygons; //set of polygons built from shared polygons in same order as m_polygons
		};

	} // namespace Dtp
} // namespace MapText

#endif // DTP_MTGEOMETRY_H
