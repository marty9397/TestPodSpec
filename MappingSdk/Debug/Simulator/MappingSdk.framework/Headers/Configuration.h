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

#ifndef DTP_CONFIGURATION_H
#define DTP_CONFIGURATION_H

#include "LabelingEngineDefs.h"
#include "Error.h"
#include "MtString.h"
#include "Units.h"

namespace MapText
{
	namespace Dtp
	{
		/**
		 * Utility class to provide version information of the library.
		 * @reqid 001.0077
		 */
		class LABELINGENGINE_API LibraryVersion
		{
		public:
			/**
			 * Returns version number of the DTP library.
			 * @return version number string in a form "major.minor.build.revision"
			 * @reqid 001.0077
			 */
			static const char* Get();
		};

		/**
		* A class that defines default values for properties that parameterize the labeling engine.
		* @reqid 001.0063
		*/
		class LABELINGENGINE_API Configuration
		{
			public:
				/**
				* Possible error codes
				* @reqid 001.0063
				*/
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_ValueRangeError,
					ErrorCode_ValueLessThanZero
				};

				/**
				* Possible error codes
				* @reqid 001.0063
				*/
				enum PlacementPrecedence
				{
					PlacementPrecedence_ScreenStability,
					PlacementPrecedence_ConfiguredPriority
				};

			private:
				/**
				 * Constructor. Private because instances are controlled.
				 * @reqid 001.0063
				 */
				Configuration( void )
				{
					InitializeWithDefaults();
				}

				/**
				 * Destructor.  This is private because only static methods are exposed.
				 * @reqid 001.0063
				 */
				~Configuration( void )
				{
				}

			public:
				/**
				 * Initialize the engine configuration.
				 * @reqid 001.0063
				 */
				static bool Initialize( PlacementPrecedence placementPrecedence );

				/**
				* Initialize a Configuration instance by assigning the default property values.
				* @reqid 001.0063
				*/
				void InitializeWithDefaults();

				/**
				 * Set the instance of the class.
				 * Used primarily for restoring the previous set of property values after a temporary override.
				 * However, a client may maintain multiple configuration instances for use in different contexts.
				 * Therefore, it is up to the client to delete any instances no longer in use.
				 * @reqid 001.0063
				 */
				static void SetInstance( const Configuration* configuration );

				/**
				 * Get the current instance of the class.
				 * Used primarily for restoring the previous set of property values after a temporary override.
				 * @reqid 001.0063
				 */
				static const Configuration* GetInstance()
				{
					return m_instance;
				}

				/**
				 * Make a full copy of the instance of the class.
				 * Instantiate it on the first invocation.
				 * @reqid 001.0063
				 */
				static Configuration* Clone( Error& error );

				/**
				* Delete the instance member.
				* @reqid 001.0063
				*/
				static void Uninitialize();

				/**
				 * Retrieve the path to the engine DLLs for dynamic loading.
				 * @return the path to the engine DLLs
				 * @reqid 002.0039
				 */
				static const String* GetEngineDllPath()
				{
					return &m_instance->m_engineDllPath;
				}

				/**
				 * Set the path to the engine DLLs for dynamic loading.
				 * @param engineDllPath - the path to the engine DLLs
				 * @reqid 002.0039
				 */
				static void SetEngineDllPath( const String& engineDllPath, Error& error )
				{
                    const TCHAR* slash = NULL;
                    
#ifdef DTP_TARGET_PLATFORM_WINDOWS
                    slash = _T("\\");
#else
                    slash = _T("/");
#endif
					String dllPath( engineDllPath, error );
					if ( error )
					{
						return;
					}
					dllPath.Trim( error );
					if ( error )
					{
						return;
					}

					int length = dllPath.GetLength();
					if ( dllPath[length-1] != slash[0] )
					{
						dllPath.ConcatString( slash, error );
						if ( error )
						{
							return;
						}
					}
					m_instance->m_engineDllPath.AssignString( dllPath.GetString(), error );
				}

				/**
				 * Retrieve the size of the leader angle range allowed at a text object bounding box corner.
				 * @return the size of the leader angle range allowed at a corner.
				 * @reqid 002.0039
				 */
				static float GetLeaderCornerAngleRangeSize()
				{
					return m_instance->m_leaderCornerAngleRangeSize;
				}

				/**
				* Set the size of the leader angle range allowed at a text object bounding box corner.
				* Note that min and max leader angle at corner are set separately for assembly labels on the 
				* assembly label definition.
				* @see GetLeaderCornerAngleRangeSize
				* @param val - LeaderCornerAngleRangeSize. Allowed value range: ( 0 ; 90 ).
				* @param error - [out] Error info.
				* @reqid 002.0039
				*/
				static void SetLeaderCornerAngleRangeSize( float val, Error& error );

				/**
				* Retrieve the minimum leader angle allowed in the end zone of a text object bounding box edge.
				* @return the minimum leader angle allowed in an end zone.
				* @reqid 002.0039
				*/
				static float GetLeaderEndZoneMinAngle()
				{
					return m_instance->m_leaderEndZoneMinAngle;
				}

				/**
				* Retrieve the maximum leader angle allowed in the end zone of a text object bounding box edge.
				* @return the maximum leader angle allowed in an end zone.
				* @reqid 002.0039
				*/
				static float GetLeaderEndZoneMaxAngle()
				{
					return m_instance->m_leaderEndZoneMaxAngle;
				}

				/**
				* Set the minimum and maximum leader angle allowed in the end zone of a text object bounding box edge.
				* @see GetLeaderEndZoneMinAngle
				* @see GetLeaderEndZoneMaxAngle
				* @param minAngle - LeaderEndZoneMinAngle, has to be > 0 and < maxAngle.
				* @param maxAngle - LeaderEndZoneMinAngle, has to be > minAngle and < 90.
				* @param error - [out] Error info.
				* @reqid 002.0039
				*/
				static void SetLeaderEndZoneMinMaxAngles( float minAngle, float maxAngle, Error& error );

				/**
				* Retrieve the size of the allowed leader angle range in the mid-zone of a text object bounding box edge.
				* @return the size of the allowed leader angle along the mid-zone.
				* @reqid 002.0039
				*/
				static float GetLeaderSideAngleRangeSize()
				{
					return m_instance->m_leaderSideAngleRangeSize;
				}

				/**
				* Set the size of the allowed leader angle range in the mid-zone of a text object bounding box edge.
				* @see GetLeaderSideAngleRangeSize
				* @param val - LeaderSideAngleRangeSize, has to be > 0 and < 180.
				* @reqid 002.0039
				*/
				static void SetLeaderSideAngleRangeSize( float val, Error& error );

				/**
				* Retrieve the size of the end zone on a vertical side of a text object bounding box,
				* relative to the bounding box height.
				* @return the size of the end zone on a vertical side, relative to the height.
				* @reqid 002.0039
				*/
				static float GetLeaderMinPercentFromCornerVertSide()
				{
					return m_instance->m_leaderMinPercentFromCornerVertSide;
				}

				/**
				* Set the size of the end zone on a vertical side of a text object bounding box,
				* relative to the bounding box height.
				* @see GetLeaderMinPercentFromCornerVertSide
				* @param val - LeaderMinPercentFromCornerVertSide, has to be within the [ 0 ; 100 ) range.
				* @param error - [out] Error info.
				* @reqid 002.0039
				*/
				static void SetLeaderMinPercentFromCornerVertSide( float val, Error& error );

				/**
				* Retrieve the size of the end zone on a horizontal side of the label bounding box,
				* relative to the bounding box width.
				* @return the size of the end zone on a horizontal side, relative to the width.
				* @reqid 002.0039
				*/
				static float GetLeaderMinPercentFromCornerHorzSide()
				{
					return m_instance->m_leaderMinPercentFromCornerHorzSide;
				}

				/**
				* Set the size of the end zone on a horizontal side of the label bounding box,
				* relative to the width.
				* @see GetLeaderMinPercentFromCornerHorzSide
				* @param val - LeaderMinPercentFromCornerHorzSide, has to be >= 0 and < 100.
				* @param error - [out] Error info.
				* @reqid 002.0039
				*/
				static void SetLeaderMinPercentFromCornerHorzSide( float val, Error& error );

				/**
				* Retrieve the maximum distance of the leader text anchor from the inner limit point
				* relative to the distance to the outer limit point.
				* See LSY_200809_01-DTP-SA.04-LeaderingDesign.doc for details.
				* @return the maximum distance of the leader text anchor from the inner limit point
				* as a percentage of the distance to the outer limit point.
				* @reqid 002.0039
				*/
				static float GetLeaderMaxPercentTravelAlongSide()
				{
					return m_instance->m_leaderMaxPercentTravelAlongSide;
				}

				/**
				* Set the maximum distance of the leader text anchor from the inner limit point
				* relative to the distance to the outer limit point.
				* @see GetLeaderMaxPercentTravelAlongSide
				* @param val - LeaderMaxPercentTravelAlongSide, has to be > 0 and < 100.
				* @param error - [out] Error info.
				* @reqid 002.0039
				*/
				static void SetLeaderMaxPercentTravelAlongSide( float val, Error& error );

				/**
				* Retrieve the size of the buffer separating a leader from the label text, in page units.
				* @return the size of the buffer separating the tail of a leader from the projected
				* point of intersection with the regular label text box or assembly label anchor point.
				* @reqid 002.0039
				*/
				static float GetTextAnchorBuffer()
				{
					return m_instance->m_textAnchorBuffer;
				}

				/**
				* Set the size of the buffer separating a leader from the label text, in page units.
				* @see GetTextAnchorBuffer
				* @param val - TextAnchorBuffer, it has to be > 0.
				* @reqid 002.0039
				*/
				static void SetTextAnchorBuffer( float val, Error& error );

				/**
				* Retrieve the leader length quality weighting factor for complex or stacked labels.
				* @return the leader length quality weighting factor.
				* @reqid 005.0058
				*/
				static float GetComplexLeaderLengthWeight()
				{
					return m_instance->m_complexLeaderLengthWeight;
				}

				/**
				* Retrieve the leader angular divergence quality weighting factor for complex or stacked labels.
				* @return the leader angular divergence quality weighting factor.
				* @reqid 005.0058
				*/
				static float GetComplexLeaderAngularDivergenceWeight()
				{
					return m_instance->m_complexLeaderAngularDivergenceWeight;
				}

				/**
				* Retrieve the length of the largest feature polyline segment intersected by a leader
				* to be treated as a short segment, in page units.
				* @return the max short segment length.
				* @reqid 005.0058
				*/
				static float GetFeatureAnchorMaxShortSegmentLength()
				{
					return m_instance->m_featureAnchorMaxShortSegmentLength;
				}

				/**
				* Set the max short segment length, in page units.
				* @see GetFeatureAnchorMaxShortSegmentLength
				* @param val - FeatureAnchorMaxShortSegmentLength, has to be >= 0.
				* @param error - [out] Error info.
				* @reqid 002.0039
				*/
				static void SetFeatureAnchorMaxShortSegmentLength( float val, Error& error );

				/**
				* Retrieve the fraction of a feature polyline segment intersected by a leader to be treated
				* as close to the endpoint.
				* @return the segment end zone.
				* @reqid 005.0058
				*/
				static float GetFeatureAnchorSegmentEndZone()
				{
					return m_instance->m_featureAnchorSegmentEndZone;
				}

				/**
				* Set the segment end zone fraction.
				* @see GetFeatureAnchorSegmentEndZone
				* @param val - FeatureAnchorSegmentEndZone, has to be > 0 and < 1.
				* @param error - [out] Error info.
				* @reqid 002.0039
				*/
				static void SetFeatureAnchorSegmentEndZone( float val, Error& error );

				/**
				* Retrieve the size of the allowed leader angle range on a feature polyline segment.
				* @return the segment angle range size.
				* @reqid 005.0058
				*/
				static float GetFeatureAnchorSegmentAngleRangeSize()
				{
					return m_instance->m_featureAnchorSegmentAngleRangeSize;
				}

				/**
				* Set the segment angle range size.
				* @see GetFeatureAnchorSegmentAngleRangeSize
				* @param val - FeatureAnchorSegmentAngleRangeSize, has to be > 0 and < 180.
				* @reqid 002.0039
				*/
				static void SetFeatureAnchorSegmentAngleRangeSize( float val, Error& error );

				/**
				* Retrieve the size of the allowed leader angle range at a feature polyline vertex.
				* @return the vertex angle range size.
				* @reqid 005.0058
				*/
				static float GetFeatureAnchorVertexAngleRangeSize()
				{
					return m_instance->m_featureAnchorVertexAngleRangeSize;
				}

				/**
				* Set the vertex angle range size.
				* @see GetFeatureAnchorVertexAngleRangeSize
				* @param val - FeatureAnchorVertexAngleRangeSize, has to be > 0 and < 180.
				* @param error - [out] Error info.
				* @reqid 002.0039
				*/
				static void SetFeatureAnchorVertexAngleRangeSize( float val, Error& error );

				/**
				* Retrieve the maximum length of a leader's extension inside an area feature boundary,
				* in page units.
				* @return the maximum length of a leader's extension inside an area feature boundary
				* @reqid 002.0039
				*/
				static float GetLeaderExtensionMaxLength()
				{
					return m_instance->m_leaderExtensionMaxLength;
				}

				/**
				* Set the maximum length of a leader's extension inside an area feature boundary,
				* in page units.
				* @see GetLeaderMaxLength
				* @param val - LeaderExtensionMaxLength, has to be > 0.
				* @param error - [out] Error info.
				* @reqid 002.0039
				*/
				static void SetLeaderExtensionMaxLength( float val, Error& error )
				{
					if ( val <= 0 )
					{
						SET_ERROR( error, ErrorCode_ValueRangeError, "LeaderExtensionMaxLength has to be > 0." );
					}
					m_instance->m_leaderExtensionMaxLength = val;
				}

				/**
				* Retrieve the maximum fraction of the area feature section that a leader's extension may penetrate.
				* See LSY_200809_01-DTP-SA.04-LeaderingDesign.doc for details.
				* @return the maximum fraction of the area feature section that a leader's extension may penetrate
				* @reqid 002.0039
				*/
				static float GetLeaderExtensionMaxFractionOfSectionSize()
				{
					return m_instance->m_leaderExtensionMaxFractionOfSectionSize;
				}

				/**
				* Set the maximum fraction of the area feature section that a leader's extension may penetrate.
				* @see GetLeaderMaxFractionOfSectionSize
				* @param val - LeaderExtensionMaxFractionOfSectionSize, has to be > 0 and < 1.
				* @param error - [out] Error info.
				* @reqid 002.0039
				*/
				static void SetLeaderExtensionMaxFractionOfSectionSize( float val, Error& error );

				/**
				* Retrieve the minimum distance for an area feature from one boundary intersection to the next
				* for which leader anchoring is allowed, in page units.
				* See LSY_200809_01-DTP-SA.04-LeaderingDesign.doc for details.
				* @return the minimum area feature neck size
				* @reqid 002.0039
				*/
				static float GetLeaderMinAreaFeatureSectionSize()
				{
					return m_instance->m_leaderMinAreaFeatureSectionSize;
				}

				/**
				* Set the minimum distance for an area feature from one boundary intersection to the next,
				* for which leader anchoring is allowed, in page units.
				* @see GetMinimumAreaFeatureSectionSize
				* @param val - MinimumAreaFeatureSectionSize, has to be > 0.
				* @reqid 002.0039
				*/
				static void SetLeaderMinAreaFeatureSectionSize( float val, Error& error )
				{
					if ( val <= 0 )
					{
						SET_ERROR( error, ErrorCode_ValueRangeError, "LeaderMinAreaFeatureSectionSize has to be > 0." );
					}
					m_instance->m_leaderMinAreaFeatureSectionSize = val;
				}

				/**
				* Retrieve the square of the minimum distance from one boundary intersection to the next
				* for which leader anchoring is allowed, in squared page units.
				* @return the minimum area feature neck size (squared for efficient comparison purposes)
				* @reqid 002.0039
				*/
				static float GetLeaderMinAreaFeatureSectionSizeSquared()
				{
					return	m_instance->m_leaderMinAreaFeatureSectionSize *
									m_instance->m_leaderMinAreaFeatureSectionSize;
				}

				/**
				* Gets LeaderSelfOverlapBuffer
				* This buffer is used to build a box around the label, stack levels, and/or text objects
				* to ensure avoidance of self-overlap by a candidate leader.
				* Buffer is needed to avoid any contact by a leader with any part of the label except the
				* text object to which the tail of the leader line is anchored.
				* Value is specified in inches.
				* @return LeaderSelfOverlapBuffer
				* @reqid 002.0039
				*/
				static float GetLeaderSelfOverlapBuffer()
				{ 
					return m_instance->m_leaderSelfOverlapBuffer;
				}

				/**
				* Sets LeaderSelfOverlapBuffer
				* @see GetSelfLeaderOverlapBuffer
				* @param val - LeaderSelfOverlapBuffer, has to be > 0.
				* @param error - [out] Error info.
				* @reqid 002.0039
				*/
				static void SetLeaderSelfOverlapBuffer(float val, Error& error )
				{
					if ( val <= 0 )
					{
						SET_ERROR( error, ErrorCode_ValueRangeError, "Leader self-overlap buffer "
							"has to be > 0.");
						return;
					}

					m_instance->m_leaderSelfOverlapBuffer = val;
				}

				/**
				* Retrieve the arc length used for creating a piecewise linear approximation of a circular arc,
				* in page units.
				* Used in creating the search path for positioning the text when doing leader placement for a
				* point feature.
				* @return the arc length used for creating a piecewise linear
				* approximation of a circular arc.
				* @reqid 002.0039
				*/
				static float GetApproximatingSubtendedArcLength()
				{
					return m_instance->m_approximatingSubtendedArcLength;
				}

				/**
				* Gets LeaderOverlapTestBuffer
				* This buffer is used to build a box around the leader line to perform overlap testing.
				* Buffer is needed to avoid zero-width box when performing the clipping.
				* Value is specified in inches.
				* @return LeaderOverlapTestBuffer
				* @reqid 002.0039
				*/
				static float GetLeaderOverlapTestBuffer()
				{ 
					return m_instance->m_leaderOverlapTestBuffer; 
				}

				/**
				* Gets LeaderProximityTestBuffer
				* This buffer is used to build a box around the leader line to perform proximity testing.
				* We do not allow some objects to be too close to the leader line to avoid ambiguity.
				* The box is used to perform the spatial search for those potential objects. 
				* Value is specified in inches.
				* @return LeaderProximityTestBuffer
				* @reqid 002.0039
				*/
				static float GetLeaderProximityTestBuffer()
				{ 
					return m_instance->m_leaderProximityTestBuffer; 
				}

				/**
				* Sets LeaderOverlapTestBuffer
				* @see GetLeaderOverlapTestBuffer
				* @param overlapBuffer - LeaderOverlapTestBuffer, has to be > 0 and < Leader proximity test buffer.
				* @param proximityBuffer - LeaderProximityTestBuffer, has to be > Leader proximity test buffer.
				* @param error - [out] Error info.
				* @reqid 002.0039
				*/
				static void SetLeaderOverlapAndProximityTestBuffers(float overlapBuffer, float proximityBuffer, 
					Error& error );

				/**
				* Gets LeaderCrossingMinimunAngleCosine
				* This value controls the minimum angle at which a leader can cross other linear geometries on the map.
				* Cosine value should be used here for performance.
				* @return LeaderCrossingMinimunAngleCosine
				* @reqid 002.0039
				*/
				static float GetLeaderCrossingMinimunAngleCosine()
				{ 
					return m_instance->m_leaderCrossingMinimunAngleCosine; 
				}

				/**
				* Sets LeaderCrossingMinimunAngleCosine
				* @see GetLeaderCrossingMinimunAngleCosine
				* @param val - LeaderCrossingMinimunAngleCosine, has to be > -1 and < 1.
				* @param error - [out] Error info.
				* @reqid 002.0039
				*/
				static void SetLeaderCrossingMinimunAngleCosine( float val, Error& error );
            
                /**
                 * If set to true offsetting of the candidate label is allowed up to 
                 * the max leader length as configured in the placement rule.
                 * If false it globally disables offsetting in leadering (performance tweak).
                 * @param value - The new value of the parameter.
                 */
                static void SetLeaderAllowOffsetting( bool value )
                {
                    m_instance->m_leaderAllowOffsetting = value;
                }
            
                /**
                 * If true, offsetting of the candidate label is allowed up to 
                 * the max leader length as configured in the placement rule.
                 * If false offsetting in leadering is globally disabled (performance tweak).
                 * @return The current value of the parameter.
                 */
                static bool GetLeaderAllowOffsetting()
                {
                    return m_instance->m_leaderAllowOffsetting;
                }

				/**
				* This parameter defines the width of a line feature below which
				* it is considered to be 'thin', i.e. during obstacle avoidance
				* it will be approximated with a zero-width linear geometry.
				* If the stroke is thicker than this value it shall be considered
				* precisely during the obstacle detection.
				* @return ThickLineThreshold
				* @reqid 001.0065
				*/
				static float GetThickLineThreshold()
				{ 
					return m_instance->m_thickLineThreshold; 
				}

				/**
				* @see GetThickLineThreshold
				* @param val - ThickLineThreshold, has to be >= 0.
				* @reqid 001.0065
				*/
				static void SetThickLineThreshold(float val, Error& error);

				/**
				* Gets the tolerance used to compare two floating point numbers, in page units.
				* @return - Gets the tolerance value.
				* @reqid 001.0074
				*/
				static float GetComparisonTolerance()
				{
					return m_instance->m_comparisonTolerance;
				}

				/**
				* Gets the squared value of the tolerance used to compare two floating point numbers,
				* in squared page units.
				* @return - Gets the tolerance value squared.
				* @reqid 001.0074
				*/
				static float GetComparisonToleranceSquared()
				{
					return m_instance->m_comparisonToleranceSquared;
				}

				/**
				* Gets the percentage of the symbol height that is used for determining the location of the 
				* corner positions, North East, North West, South East, South West
				* @return - Gets the percentage that defines corner regions for PointPos
				* @reqid 002.0036
				*/
				static float GetCornerPointPositionAsPercentageOfSymbolHeight()
				{ 
					return m_instance->m_cornerPointPositionAsPercentageOfSymbolHeight;
				}

				/**
				* Gets GetMaxAreaLabelOverhangPercentage
				* This tells the area placer how much of the label can protrude
				* outside of the polygon while performing partially-outside placement.
				* @return MaxAreaLabelPercentageOverhang
				* @reqid 004.0032
				*/
				static float GetMaxAreaLabelOverhangPercentage()
				{ 
					return m_instance->m_maxAreaLabelOverhangPercentage; 
				}

				/**
				* Sets GetMaxAreaLabelOverhangPercentage
				* @see GetMaxAreaLabelOverhangPercentage
				* @param val - MaxAreaLabelPercentageOverhang, has to be > 0 and < 1
				* @param error - [out] Error info.
				* @reqid 004.0032
				*/
				static void SetMaxAreaLabelOverhangPercentage(float val, Error& error);

				/**
				* @return - gets the ideal angular change in radians between adjacent characters in a curve label.
				* @reqid 003.0023
				*/
				static float GetIdealAngularChange()
				{
					return m_instance->m_idealAngularChange;
				}

				/**
				* @return - gets the maximum number of segments that will be generated to approximate a circular arc.
				* @reqid 003.0023
				*/
				static int GetMaximumNumberOfSubArcs()
				{
					return m_instance->m_maximumNumberOfSubArcs;
				}

				/**
				* @return - gets the angular change in radians for a segment pair at which the segment pair will not be
				            smoothened.
				* @reqid 003.0023
				*/
				static float GetAngularChangeNotToSmoothen()
				{
					return m_instance->m_angularChangeNotToSmoothen;
				}

				/**
				* @return - gets the square of the fraction of the average character width that if multiplied against
				            the average width yields a length that is greater than the segment length, the segment pair
							is not smoothened.
				* @reqid 003.0023
				*/
				static float GetSegmentTooSmallForSmootheningAsFractionSquared()
				{
					return m_instance->m_segmentTooSmallForSmootheningAsFractionSquared;
				}

				/**
				* @return - Determines whether acceptability rules should be skipped in order to minimize jumping in dynamic mode.
				* @reqid 003.0023
				*/
				static bool GetSkipAcceptabilityRules()
				{
					return m_instance->m_skipAcceptabilityRules;
				}

				/**
				* @return - gets the cushion distance between the smoothening circular arc and the feature corner.
				* @reqid 003.0023
				*/
				static float GetCushionDistanceFromTouchingFeatureCorner()
				{
					return m_instance->m_cushionDistanceFromTouchingFeatureCorner;
				}

				/**
				* @return - gets the length of the segments that will approximate the circular arc on the smoothened 
				  baseline, in page units.
				* @reqid 003.0023
				*/
				static float GetSegmentLengthOnSampledArc()
				{
					return m_instance->m_segmentLengthOnSampledArc;
				}

				/**
				* @return - This gets the maximum allowed inter textpart angular change.
				* @reqid 003.0023
				*/
				static float GetInterTextPartAllowAngle()
				{
					return m_instance->m_interTextPartAllowAngle;
				}

				/**
				* @see GetInterTextPartAllowAngle
				* @param val - InterTextPartAllowAngle
				* @reqid 003.0023
				*/
				static void SetInterTextPartAllowAngle(float val);

				/**
				* @return - Gets the cos of the angle that is considered sharp during offsetting such that if
						    this sharp turn is detected, instead of purely offsetting, the Offsetter will
							offset the corner point by the offset amount.
				* @reqid 003.0023
				*/
				static float GetCosSharpOffsetAngle()
				{
					return m_instance->m_cosSharpOffsetAngle;
				}

				/**
				* @return - Gets the overhang tolerance percent used when computing the overhang amount for line placement.
				* @reqid 003.0075
				*/
				static float GetOverhangTolerance()
				{
					return m_instance->m_overhangTolerance;
				}

				/**
				* @return - Gets the maximum conform position attempts
				* @reqid 
				*/
				static int GetMaxConformPositionAttemps()
				{
					return m_instance->m_maxConformPositionAttemps;
				}

				/**
				* @return - Gets the maximum conform position attempts
				* @reqid 
				*/
				static void SetMaxConformPositionAttemps(int val)
				{
					m_instance->m_maxConformPositionAttemps = val;
				}

				/**
				* @return - Gets the angle of the ray from the position x-axis that is shot toward the search path and
						    intersected with it to find the initial label position on the search path.
				* @reqid 001.0063
				*/
				static float GetIdealInitialLeaderRayAngle()
				{
					return m_instance->m_idealInitialLeaderRayAngle;
				}

				/**
				* @return - Gets the value to use to compute final value used for gap tolerance
				*						used to filter a polyline
				* @reqid 001.0063
				*/
				static int GetPolylineFilterGapToleranceCoefficient()
				{
					return m_instance->m_polylineFilterGapToleranceCoefficient;
				}
			
				/**
				* Gets UseExhaustiveAreaSearch
				* If true area module performs more fine-grained search for space.
				* Setting it to false improves performance but may cause the labels
				* not being placed in rare cases.
				* @return UseExhaustiveAreaSearch
				* @reqid 004.0032
				*/
				static bool GetUseExhaustiveAreaSearch()
				{
					return m_instance->m_useExhaustiveAreaSearch;
				}

				/**
				 * Sets UseExhaustiveAreaSearch
				 * @see GetUseExhaustiveAreaSearch
				 * @param val UseExhaustiveAreaSearch
				 * @reqid 004.0032
				 */
				static void SetUseExhaustiveAreaSearch(bool val)
				{
					m_instance->m_useExhaustiveAreaSearch = val;
				}

				/**
				 * Gets ObstacleAvoidanceShiftTolerance, in page units.
				 * The raw shifts returned by the obstacle detector may not be sufficient,
				 * because of floating point issues.
				 * @return ObstacleAvoidanceShiftTolerance
				 * @reqid 002.0039
				 */
				static float GetObstacleAvoidanceShiftTolerance()
				{ 
					return m_instance->m_obstacleAvoidanceShiftTolerance;
				}

				/**
				 * Sets ObstacleAvoidanceShiftTolerance, in page units.
				 * @see GetObstacleAvoidanceShiftTolerance
				 * @param val - ObstacleAvoidanceShiftTolerance, has to be > 0.
				 * @param error - [out] Error info.
				 * @reqid 002.0039
				 */
				static void SetObstacleAvoidanceShiftTolerance(float val, Error& error );

				/**
				 * Gets MinFixedGridIndexCellSize
				 * This should eventually be computed dynamically, based on avg text size.
				 * @return MinFixedGridIndexCellSize
				 * @reqid 001.0064
				 */
				static float GetMinFixedGridIndexCellSize()
				{ 
					return m_instance->m_minFixedGridIndexCellSize;
				}

				/**
				 * Sets MinFixedGridIndexCellSize
				 * @see GetMinFixedGridIndexCellSize
				 * @param val MinFixedGridIndexCellSize, has to be > 0.
				 * @param error - [out] Error info.
				 * @reqid 001.0064
				 */
				static void SetMinFixedGridIndexCellSize(float val, Error& error );

				/**
				 * Gets MaxFixedGridIndexRowsColumns
				 * The maximum number of rows and the maximum number of columns
				 * @return MaxFixedGridIndexRowsColumns
				 * @reqid 001.0064
				 */
				static int GetMaxFixedGridIndexRowsColumns()
				{ 
					return m_instance->m_maxFixedGridIndexRowsColumns;
				}

				/**
				 * Sets MaxFixedGridIndexRowsColumns
				 * @see GetMaxFixedGridIndexRowsColumns
				 * @param val MaxFixedGridIndexRowsColumns, has to be > 1.
				 * @param error - [out] Error info.
				 * @reqid 001.0064
				 */
				static void SetMaxFixedGridIndexRowsColumns(int val, Error& error );

				/**
				 * Gets the slope of the line that defines the boundary between lines for which left/right
				   justification is applied to the label and center justification is applied to the label.
				 * @reqid 003.0025
				 */
				static float GetSlopeThreshold1()
				{
					return m_instance->m_slopeThreshold1;
				}

				/**
				 * Gets the slope of the line that defines the boundary between lines for which decisions are
				   made for horizontal placement.
				 * @reqid 003.0025
				 */
				static float GetSlopeThreshold2()
				{
					return m_instance->m_slopeThreshold2;
				}

				/**
				* Gets MaxStackLevelVariationPercentage - this defines what is the max
				* percentage the stack level length can be of the ideal length (which is 
				* the total label length divided by the number of stack levels). High value
				* will allow labels to have stack levels which are sometimes much different
				* in length like:
				*               Tadeusz Kosciuszko
				*                      St.
				* Low value will disallow this and create a label like this:
				*                    Tadeusz
				*                 Kosciuszko St.
				* The value is given in percents (e.g. 120.0 for 120%, each stack level can
				* be no more than 120% of the ideal length).
				* This works in combination with the MinStackLevelLengthPercentage
				* @return - MaxStackLevelLengthPercentage
				* @reqid TODO mfraczak.Stacking
				*/
				static float GetMaxStackLevelLengthPercentage()
				{ 
					return m_instance->m_maxStackLevelLengthPercentage; 
				}

				/**
				* @see GetMaxStackLevelLengthPercentage
				* @param val - MaxStackLevelLengthPercentage
				* @reqid TODO mfraczak.Stacking
				*/
				static void SetMaxStackLevelLengthPercentage(float val) 
				{ 
					m_instance->m_maxStackLevelLengthPercentage = val; 
				}

				/**
				* Gets MinStackLevelLengthPercentage - this defines what is the min
				* percentage the stack level length can be of the ideal length (which is 
				* the total label length divided by the number of stack levels). Low value
				* will allow labels to have stack levels which are very short, like:
				*
				*               Tadeusz Kosciuszko
				*                      St.
				*
				* Low value will disallow this and create a label like this:
				*
				*                    Tadeusz
				*                 Kosciuszko St.
				*
				* The value is given in percents (e.g. 20.0 for 20%, each stack level can
				* be no less than 20% of the ideal length).
				* This works in combination with the MaxStackLevelLengthPercentage
				* @return - MinStackLevelLengthPercentage
				* @reqid TODO mfraczak.Stacking
				*/
				static float GetMinStackLevelLengthPercentage()
				{ 
					return m_instance->m_minStackLevelLengthPercentage; 
				}

				/**
				* @see GetMinStackLevelLengthPercentage
				* @param val - MinStackLevelLengthPercentage
				* @reqid TODO mfraczak.Stacking
				*/
				static void SetMinStackLevelLengthPercentage(float val) 
				{ 
					m_instance->m_minStackLevelLengthPercentage = val; 
				}

				static unsigned char GetAsciiNonBreakingSpaceCharacter()
				{
					return m_instance->m_asciiNonBreakingSpaceCharacter;
				}

				/**
				* Gets NonSymbolizedPointFeatureObstacleSize. This value shall be used
				* inside the obstacle detection to control the size of a "virtual obstacle"
				* for the non-symbolized point feature.
				* @return - NonSymbolizedPointFeatureObstacleSize
				* @reqid 002.0039
				*/
				static float GetNonSymbolizedPointFeatureObstacleSize()
				{ 
					return m_instance->m_nonSymbolizedPointFeatureObstacleSize; 
				}

				/**
				* Sets NonSymbolizedPointFeatureObstacleSize
				* @param val - NonSymbolizedPointFeatureObstacleSize
				* @reqid 002.0039
				*/
				static void SetNonSymbolizedPointFeatureObstacleSize(float val) 
				{ 
					m_instance->m_nonSymbolizedPointFeatureObstacleSize = val; 
				}

				/**
				* When a label is shifted from the feature for perpendicular or tangent placement to account for
				  line width, each side of the label needs to shift a different amount depending on the angle of the
				  segments of the feature. This method gets what is deemed as a large different between these two
				  shifts, in page units.
				* @return - what is deemed as a large difference between the two line width shifts.
				* @reqid 003.0018,003.0020,003.0027,003.0029,003.0071
				*/
				static float GetLargeDifferenceBetweenLineWidthShifts() 
				{ 
					return m_instance->m_largeDifferenceBetweenLineWidthShifts;
				}

				/**
				* When a label is shifted from the feature for perpendicular or tangent placement to account for
				  line width, each side of the label needs to shift a different amount depending on the angle of the
				  segments of the feature. This method sets what is deemed as a large different between these two
				  shifts. When the difference is large as determined by m_largeDifferenceBetweenLineWidthShifts,
				  then a percentage of the smaller shift is applied. The size of this percentage is stored by
				  m_percentageLineWidthShiftDifferenceToApply.
				* @return - percentage of the smaller line width shift to apply.
				* @reqid 003.0018,003.0020,003.0027,003.0029,003.0071
				*/
				static float GetPercentageLineWidthShiftDifferenceToApply() 
				{ 
					return m_instance->m_percentageLineWidthShiftDifferenceToApply;
				}

				/**
				* When testing if a vector is along the same direction as an angle, this property defines
				  the tolerance range in which this decision is made. The vector is tested to be within the
				  range of the angle +- AngleTolerance.
				* @return - value of the angle tolerance.
				* @reqid 003.0027,003.0028,003.0029
				*/
				static float GetAngleTolerance()
				{ 
					return m_instance->m_angleTolerance;
				}

				/**
				* Determine whether to set label request placement priority based primarily on the
				* user specified priority of the associated label class or based primarily on the
				* placement status so as to minimize jumping labels.
				* @return - value of the placement precedence.
				* @reqid TODO pgrenetz.PlacementPrecedence
				*/
				static PlacementPrecedence GetPlacementPrecedence()
				{ 
					return s_placementPrecedence;
				}

				/**
				* Specify whether to set label request placement priority based primarily on the
				* user specified priority of the associated label class or based primarily on the
				* placement status so as to minimize jumping labels.
				* @param val - value of the placement precedence.
				* @reqid TODO pgrenetz.PlacementPrecedence
				*/
				static void SetPlacementPrecedence( PlacementPrecedence val )
				{ 
					s_placementPrecedence = val;
				}

				/**
				* Gets units in which property values are stored. The units setting affects configuration values
				* whose default values are defined in page units. When changed it also converts the pre-set values to new units.
				* After this is set all the newly assigned values are also interpreted in NEW units.
				* @return - units
				* @reqid TODO mfraczak.Units
				*/
				static Unit GetUnits()
				{ 
					return m_instance->m_units; 
				}

				/**
				* Converts the property values stored within into the input units.
				* @see GetUnits
				* @param newUnits - units to convert to.
				* @reqid TODO mfraczak.Units
				*/
				static void ConvertUnitsTo(Unit newUnits);

				static const std::string & GetEcwLicenseKey() {
					return m_instance->m_ecwLicenseKey;
				}

				static void SetEcwLicenseKey(const std::string ecwLicenseKey) {
					m_instance->m_ecwLicenseKey = ecwLicenseKey;
				}

				static const std::string & GetEcwLicenseCompanyName() {
					return m_instance->m_ecwLicenseCompanyName;
				}

				static void SetEcwLicenseCompanyName(const std::string ecwLicenseCompanyName) {
					m_instance->m_ecwLicenseCompanyName = ecwLicenseCompanyName;
				}

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!

				ALSO, ADD YOUR NEW PROPERTY TO THE ConvertUnitsTo METHOD!!
				****************************************************************************************/
			private:
				static PlacementPrecedence s_placementPrecedence;
				static Configuration m_initialInstance;
				static Configuration* m_instance;
				static bool s_initialized;

				String m_engineDllPath;

				///////////////////////////////////////////////////////////////////////
				// Note that min and max leader angle at corner
				// are set separately for assembly labels
				// on the assembly label definition
				///////////////////////////////////////////////////////////////////////
				float m_leaderCornerAngleRangeSize;
				float m_leaderEndZoneMinAngle;
				float m_leaderEndZoneMaxAngle;
				float m_leaderSideAngleRangeSize;
				float m_leaderMinPercentFromCornerVertSide;
				float m_leaderMinPercentFromCornerHorzSide;
				float m_leaderMaxPercentTravelAlongSide;

				float m_textAnchorBuffer;

				float m_complexLeaderLengthWeight;
				float m_complexLeaderAngularDivergenceWeight;

				float m_featureAnchorMaxShortSegmentLength;
				float m_featureAnchorSegmentEndZone;
				float m_featureAnchorSegmentAngleRangeSize;
				float m_featureAnchorVertexAngleRangeSize;

				float m_leaderExtensionMaxLength;
				float m_leaderExtensionMaxFractionOfSectionSize;
				float m_leaderMinAreaFeatureSectionSize;
                bool m_leaderAllowOffsetting;

				float m_leaderSelfOverlapBuffer;

				float m_approximatingSubtendedArcLength;

				float m_leaderOverlapTestBuffer;
				float m_leaderProximityTestBuffer;
				float m_leaderCrossingMinimunAngleCosine;
				float m_thickLineThreshold;

				float m_comparisonTolerance;
				float m_comparisonToleranceSquared;
				float m_cornerPointPositionAsPercentageOfSymbolHeight;
				float m_overhangTolerance;
				int	m_maxConformPositionAttemps;

				float m_maxAreaLabelOverhangPercentage;

				float m_idealAngularChange;
				int   m_maximumNumberOfSubArcs;
				float m_angularChangeNotToSmoothen;
				float m_segmentTooSmallForSmootheningAsFractionSquared;
				float m_cushionDistanceFromTouchingFeatureCorner;
				float m_segmentLengthOnSampledArc;
				float m_cosSharpOffsetAngle;

				float m_interTextPartAllowAngle;

				float m_idealInitialLeaderRayAngle;

				int m_polylineFilterGapToleranceCoefficient;
				
				bool m_useExhaustiveAreaSearch;

				float m_obstacleAvoidanceShiftTolerance;

				float m_minFixedGridIndexCellSize;
				int m_maxFixedGridIndexRowsColumns;

				float m_slopeThreshold1;
				float m_slopeThreshold2;

				bool m_skipAcceptabilityRules;

				float m_maxStackLevelLengthPercentage;
				float m_minStackLevelLengthPercentage;
				unsigned char m_asciiNonBreakingSpaceCharacter;

				float m_nonSymbolizedPointFeatureObstacleSize;
				
				float m_largeDifferenceBetweenLineWidthShifts;
				float m_percentageLineWidthShiftDifferenceToApply;

				float m_angleTolerance;

				Unit m_units;

				std::string m_ecwLicenseKey;
				std::string m_ecwLicenseCompanyName;
		};
	} // namespace Dtp
} // namespace MapText

#endif // DTP_CONFIGURATION_H
