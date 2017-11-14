//
//  DataProjector.h
//  MappingSdk
//
//  Created by Mark on 2/2/13.
//
//

#ifndef __MappingSdk__DataProjector__
#define __MappingSdk__DataProjector__

#include "MtGeometry.h"

namespace MapText
{
    namespace Dtp
	{
        /**
         * This will be used to convert given coordinates from one projection to another
         */        
        class DataProjector
        {
        public:
            
            enum ErrorCode
            {
                ErrorCode_ProjectionFailure = 1
            };
            
            enum ConversionType
            {
                ConversionType_None = 0,
                ConversionType_ToMapUnits,
                ConversionType_FromMapUnits,
                ConversionType_FromMapUnitsToMapUnits,
            };

            /**
             * This will be used to convert given coordinates from one projection to another
             * @param mapScale - scale to use for converting to map units
             * @param lowerLeft - lowerleft of data which is used as the origin of the map for converting to map units
             * @param inputProjectionString - the projection the incoming coordinates come from
             * @param outputProjectionString - the projection the incoming coordinates should be converted to
             * @param conversionType - whether the incoming coordinates will be converted to or from map units
             * @param error - error info.
             */
            DataProjector(float mapScale, const MapText::Dtp::DoublePoint& lowerLeft, const char* inputProjectionString, const char* outputProjectionString,
                          ConversionType conversionType, MapText::Dtp::Error& error  );

            DataProjector(const char* inputProjectionString, const char* outputProjectionString, MapText::Dtp::Error& error  );

            ~DataProjector();
            
            /**
             * Convert given coordinates to output projection
             * @param x - x coordinate (degrees for longitude value)
             * @param y - y coordinate (degrees for latitude value)
             * @param error - error info.
             */
            MapText::Dtp::Point GetTargetProjectionPoint(float x, float y, Error& error);
            
            MapText::Dtp::DoublePoint GetTargetProjectionDoublePoint(double x, double y, Error& error);

            /**
             * Convert given point to output projection
             * @param point - point to convert (degrees for latLong coordinate)
             * @param error - error info.
             */
            MapText::Dtp::Point GetTargetProjectionPoint(const MapText::Dtp::Point& point, Error& error);
            
            MapText::Dtp::DoublePoint GetTargetProjectionDoublePoint(const MapText::Dtp::DoublePoint& point, Error& error);

            /**
             * Convert all points of given shape to output projection
             * @param point - point to convert (degrees for latLong coordinate)
             * @param error - error info.
             */
            void ProjectShapeToTargetProjection(Shape* shape, Error& error);
                        
            void SetMapScale(float scale);
            
            void SetLowerLeft(MapText::Dtp::DoublePoint& lowerleft);
            
            const char* GetInputProjectionString();
            
            const char* GetOutputProjectionString();

            static void ReplaceProjectionStringUnitsWithInches(std::string& projectionString);

        private:
            class Impl; Impl* m_pimpl;
            DataProjector( const DataProjector& );
            DataProjector& operator =( const DataProjector& );
        };
        
    }
}

#endif /* defined(__MappingSdk__DataProjector__) */
