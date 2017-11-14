/**************************************************
 * Copyright (c)2011 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://mfraczak@mtsbs.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/LabelingEngine/Version.h $
 * 
 * Revision: $Revision: 17081 $
 *
 * Last modified on: $Date: 2011-03-25 01:07:08 -0400 (Fri, 25 Mar 2011) $, by: $Author: dailybuild $
 * 
 *************************************************/

#ifndef UNITS_H
#define UNITS_H

#include "LabelingEngineDefs.h"

namespace MapText
{
	namespace Dtp
	{
		
		enum UnitType
		{
			UnitType_Absolute,
			UnitType_Percentage,
		};

		/**
		 * Unit of position or distance measure
		 * km, m, in, ft, cm, mm
		 * @reqid 001.0063
		 */
		enum Unit
		{
			Unit_Inch = 0,
			Unit_Mm,
			Unit_Cm,
			Unit_M,
			Unit_Km,
			Unit_Mi,
			Unit_Ft,
			Unit_Count,
		};

		//////////////////////////////////////////
		// UnitConversionRates

		/**
		 * Conversion factors wrapper class
		 */
		class LABELINGENGINE_API UnitConversionRates
		{
		public:
			/**
			 * Get the conversion rates between units
			 * @param from - Unit from which you want to convert.
			 * @param to - Unit to which you want to convert.
			 * @return - The conversion factor
			 */
			static float GetConversionRate( Unit from, Unit to );
		private:
			static float m_unitConversionRates[ Unit_Count ][ Unit_Count ];
		};
	}; // namespace Dtp
}; // namespace MapText

#endif // UNITS_H
