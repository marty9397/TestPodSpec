/**************************************************
 * Copyright (c)2008 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL$
 *
 * Revision: $Revision$
 *
 * Last modified on: $Date$, by: $Author$
 *
 *************************************************/

#ifndef DTP_TRIGONOMETRY_H
#define DTP_TRIGONOMETRY_H

#include "MapTextDefs.h"

#define M_3PI_2	4.71238884
#define M_2PI 6.283185307

namespace MapText
{
	namespace Dtp
	{
		/**
		* A singleton class designed to execute trigonometric functions quickly.
		* @reqid 001.0063
		*/
		class MAPTEXTTYPES_API Trig
		{
			/**
			* The possible units in which an angle can be specified.
			* @reqid 001.0063
			*/
			public:
				enum AngleUnits
				{
					Radians,
					Degrees
				};

			public:
				/**
				* Convert an angle in radians to degrees.
				* @param radians	The angle in radians to be converted
				* @reqid 001.0063
				*/
				static float RadiansToDegrees( float radians )
				{
					return radians * 57.29577951f;
				}

				/**
				* Convert an angle in degrees to radians.
				* @param degrees	The angle in degrees to be converted
				* @reqid 001.0063
				*/
				static float DegreesToRadians( float degrees )
				{
					return degrees * .017453292f;
				}

				/**
				* Initialize the static members of the class.
				* Build the table used for quick lookup of sine and cosine values.
				* @reqid 001.0063
				*/
				static bool Initialize();

				/**
				* Get the sine of an angle using table lookup.
				* @param angle	The angle for which the sine is requested
				* @param units	The units in which the angle is specified
				* @reqid 001.0063
				*/
				static float qsin( float angle, AngleUnits units );

				/**
				* Get the cosine of an angle using table lookup.
				* @param angle	The angle for which the cosine is requested
				* @param units	The units in which the angle is specified
				* @reqid 001.0063
				*/
				static float qcos( float angle, AngleUnits units );

				/**
				 * Normalize the angle - reduce the value to fall 
				 * in the [ min, max ) range
				 * @param angleRadians	angle in radians
				 * @param rangeMin			minimum value of normalized angle
				 * @param rangeMax			maximum value of normalized angle
				 * @return the normalized angle
				 * @reqid 002.0039
				 */
				static float NormalizeAngle( float angleRadians, float rangeMin, float rangeMax );

				/**
				* Compute angle
				* @param dx		delta x
				* @param dy		delta y
				* @return the angle in radians
				* @reqid 003.0021
				*/
				static float ComputeAngle( float dx, float dy );

		private:
				/**
				* Constructor.
				* @reqid 001.0063
				*/
				Trig();

				/**
				* Get the sine of an angle using table lookup.
				* This method is invoked on the singleton instance.
				* @param angle	The angle for which the sine is requested
				* @param units	The units in which the angle is specified
				* @reqid 001.0063
				*/
				float _qsin( float angle, AngleUnits units ) const;

				/**
				* Get the cosine of an angle using table lookup.
				* This method is invoked on the singleton instance.
				* @param angle	The angle for which the cosine is requested
				* @param units	The units in which the angle is specified
				* @reqid 001.0063
				*/
				float _qcos( float angle, AngleUnits units ) const;

				/**
				* Get the singleton instance of the class.
				* Instantiate it on the first invocation.
				* @reqid 001.0063
				*/
				static const Trig& GetInstance();

			private:
				static float s_sinetable[90000];
		};
	}
}

#endif //DTP_TRIGONOMETRY_H
