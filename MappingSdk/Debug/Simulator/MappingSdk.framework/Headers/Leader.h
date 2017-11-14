/**************************************************
 * Copyright (c)2008 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://martin@mtsrv.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/LabelingEngine/Leader.h $
 *
 * Revision: $Revision: 70920 $
 *
 * Last modified on: $Date: 2015-02-04 14:55:22 -0500 (Wed, 04 Feb 2015) $, by: $Author: mark $
 *
 *************************************************/

#ifndef DTP_LEADER_H
#define DTP_LEADER_H

#include "LabelingEngineDefs.h"
#include "Label.h"

#include "Error.h"

namespace MapText
{
	namespace Dtp
	{
		/**
		 * Forward declare pointer-only dependencies
		 */
		class Error;
		class Feature;
		class FeatureOverlap;
		class IPolylineView;
		class Map;
		class ObstacleOverlapSummary;
		class OverlapGeometry;
		class OverlapGeometries;
		class PlacementRule;
		class Point;
		class PolylineShape;
		class Rectangle;
		class Shape;
		class Vector;

		/**
		 * Class that holds a Leader object.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Leader
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_GeometryTypeNotSupported,
				};

				enum PlacementTestResult
				{
					PlacementTestResult_Allowed = 0,
					PlacementTestResult_InvalidOverlaps,
					PlacementTestResult_InvalidNearbyObjects,
				};

				enum WhichLeaderPointFixed
				{
					WhichLeaderPointFixed_Start,
					WhichLeaderPointFixed_End,
				};

				/**
				 *	Default constructor. Members initialized to default values.
				 * @reqid 002.0039
				 */
				Leader();

				/**
				 *	Constructor.
				 * @param leaderLine a polyline with finite width that models the leader line
				 * @param arrowHead a polyline or polygon shape that models the arrowHead
				 * @reqid 002.0039
				 */
				Leader( const PolylineShape *leaderLine, Shape *arrowHead );

				/**
				 * Destructor.
				 * @reqid 002.0039
				 */
				~Leader();

				/**
				 * Create a deep copy of the object
				 * @reqid 001.0063
				 */
				Leader* Clone(Error& error) const;

				//GETTERS & SETTERS BEGIN
				/**
				 * Get the polyline with finite width that models the leader line.
				 * @reqid 002.0039
				 */
				const PolylineShape* GetLeaderLine() const
				{
					return m_leaderLine;
				}

				/**
				 * Get the polyline or polygon with finite width that models the arrow head.
				 * @reqid 002.0039
				 */
				const Shape* GetArrowhead() const
				{
					return m_arrowHead;
				}

				/**
				 * @see GetLeaderLine
				 * @reqid 002.0039
				 */
				void SetLeaderLine( PolylineShape* leaderLine )
				{
					m_leaderLine = leaderLine;
				}

				/**
				 * @see GetArrowhead
				 * @reqid 002.0039
				 */
				void SetArrowhead( Shape* arrowHead )
				{
					m_arrowHead = arrowHead;
				}

				/**
				* Perform spatial tests to assert if the leader placement is acceptable
				* @param map - the map in which leader was placed
				* @param placementRule - placement rule of the label request that owns
				*        the leader
				* @param owner - the feature for which the leader is being generated
				* @param bDisableEndZoneTest if true, ignore polyline overlaps into the proximity box end zone
				* @param immediateOverlaps - [out] overlaps with the smaller leader box 
				*        (built using the overlap buffer, 
				*        @see Configuration::GetLeaderOverlapTestBuffer). Those count 
				*        vs the crossings number limit and only allow feature classes 
				*        listed in placement rule 
				*        (@see PlacementRule::GetAllowLeaderCrossList)
				* @param proximityOverlaps - [out] overlaps with the larger leader box 
				*        (built using the overlap buffer, 
				*        @see Configuration::GetLeaderProximityTestBuffer). 
				*        Caution - this will be null if result is equal to 
				*        PlacementTestResult_InvalidOverlaps. If invalid overlaps are
				*        detected within the smaller buffer the proximity test is not
				*        performed.
				* @param fixedPoint - tells the analyzer which point of the leader line is fixed and
				*        which is moving.
				* @param result - return value
				* @param error - error info
				* @reqid 002.0039
				*/
				void IsLeaderPlacementAcceptable( Map& map, const PlacementRule& placementRule, 
					const Feature& owner, bool bDisableEndZoneTest,
					ObstacleOverlapSummary** immediateOverlaps, ObstacleOverlapSummary** proximityOverlaps,
					WhichLeaderPointFixed fixedPoint,  PlacementTestResult *result, Error& error );

				/**
				 * Performs a spatial analysis of the overlaps invalidating the current leader line
				 * and suggests two vectors forming a wedge which is invalidated by the obstacles 
				 * detected. The clockwise and counter-clockwise vectors can be used to build 
				 * the next suggested leader line.
				 * @param placementRule rule controlling the placement of current label
				 * @param whichLeaderPointFixed - this tells the search algorithm if the start or the end
				 * @param immediateOverlaps overlaps detected with the immediate leader box
				 * @param proximityOverlaps overlaps detected with the larger leader proximity box
				 * @param cwVector [in,out] - in: how far can the CW obstacle avoidance vector go?
				 *                            out: the CW obstacle avoidance vector if possible (check
				 *                            avoidanceImpossible first).
				 * @param ccwVector [in,out] - in: how far can the CCW obstacle avoidance vector go?
				 *                             out: the CCW obstacle avoidance vector if possible (check
				 *                            avoidanceImpossible first).
				 * @param cwAavoidanceImpossible - [out] if true it means that the wedge defined as input does
				 *                                 not allow for clockwise avoidance of the current obstacles
				 * @param ccwAavoidanceImpossible - [out] if true it means that the wedge defined as input does
				 *                                  not allow for counter-clockwise avoidance of the current 
				 *                                  obstacles
				 * @param error error information
				 * @reqid 002.0039
				 */
				void GetVectorsToAcceptablePlacement( const PlacementRule& placementRule,
					WhichLeaderPointFixed whichLeaderPointFixed,
					ObstacleOverlapSummary* immediateOverlaps, ObstacleOverlapSummary* proximityOverlaps,
					Vector& cwVector, Vector& ccwVector,
					bool* cwAvoidanceImpossible, bool* ccwAvoidanceImpossible,
					bool bDisableEndZoneTest, Error& error );

				/**
				 * Given an overlap geometry add the ones that are growing the overlap
				 * to the output collection.
				 * @param geometry - geometry to analyze
				 * @param cwAngle - current cw avoidance vector angle
				 * @param ccwAngle - current ccw avoidance vector angle
				 * @param cwMagnitude - current cw avoidance vector magnitude
				 * @param ccwMagnitude - current ccw avoidance vector magnitude
				 * @param proximity - set to true if the analysis should be done in 
				 *                    the context of proximity buffer; if false the
				 *                    analysis is done for the overlap buffer.
				 * @param pushingOverlaps - [out] if the geometry is causing the obstacle
				 *                          avoidance wedge to grow it's added here.
				 * @param error - [out] error info
				 * @reqid 002.0039
				 */
				void AddVectorsPushingOverlap( OverlapGeometry* geometry, float cwAngle, float ccwAngle, 
					float cwMagnitude, float ccwMagnitude, bool proximity, 
					OverlapGeometries &pushingOverlaps, Error& error );


				/**
				 * Rotate the point about the specified origin.
				 * @param origin - the point about which rotation is requested
				 * @param sin_t	- the sine of the angle through which rotation is requested
				 * @param cos_t	- the cosine of the angle through which rotation is requested
				 * @reqid 002.0039
				 */
				void Rotate( const Point& origin, float sin_t, float cos_t)
				{
					if ( m_leaderLine )
					{
						m_leaderLine->GetPolyline()->Rotate( origin, sin_t, cos_t );
					}
					if ( m_arrowHead )
					{
						// Since we don't know what it is right now, do nothing
						assert( false );
					}
				}

				/**
				* Set the color of the leader
				* @param color
				* @reqid 
				*/
				void SetColor(long color)
				{
					m_color = (int)color;
				}

				/**
				* Get the color of the leader
				* @param color
				* @reqid 
				*/
				long GetColor() const
				{
					return m_color;
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
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static Leader* DeserializeNew( IDeserializer& deserializer, Error& error );

				/**
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				void Deserialize( IDeserializer& deserializer, Error& error );
#endif

			private:
				/**
				* Performs a series of tests on leader segment overlaps to check whether 
				* the position is acceptable taking a range of criteria into account.
				* @param segment geometry of the segment being analyzed
				* @param overlapSummary complete overlap summery of the leader box created using a small buffer 
				*        around it (@see Configuration::GetLeaderOverlapTestBuffer).
				* @param placementRule placement rule driving the current placement
				* @param completeAnalysis if true a complete analysis is performed. Otherwise
				*        analysis stops after first unacceptable obstacle is met.
				* @param result [out] true if the position is acceptable
				* @param error an Error object
				* @reqid 002.0039
				*/
				void AreSegmentOverlapsAcceptable( Ray& segment, ObstacleOverlapSummary& overlapSummary, 
					const PlacementRule& placementRule,  bool completeAnalysis, bool* result, Error& error );

				/**
				* Performs a series of tests on the overlaps generated for some proximity of the leader
				* segment to check whether the placement is acceptable.
				* @param point0 1st point of the segment being analyzed
				* @param point1 2nd point of the segment being analyzed
				* @param overlapSummary complete overlap summery of the leader proximity. The box should be
				*        generated using an enlarged buffer (@see Configuration::GetLeaderProximityTestBuffer).
				* @param placementRule placement rule driving the current placement
				* @param completeAnalysis if true a complete analysis is performed. Otherwise
				*        analysis stops after first unacceptable obstacle is met.
				* @param bDisableEndZoneTest if true, ignore polyline overlaps into the proximity box end zone
				* @param result [out] true if the position is acceptable
				* @param error an Error object
				* @reqid 002.0039
				*/
				void AreSegmentProximityFeaturesAcceptable( const Point& point0, const Point& point1,
					ObstacleOverlapSummary& overlapSummary, const PlacementRule& placementRule,
					bool completeAnalysis, bool bDisableEndZoneTest, bool* result, Error& error );

				/**
				* Generate a rectangle enclosing a leader line segment
				* buffered above and below by a specified distance.
				* @param segmentNumber point to the segment which should be surrounded with a box
				* @param leaderBoxBufferSize the distance above and below the line
				* @param extendLenghtwise should the buffer be applied in all directions (true) 
				*                         or only to the sides of the segment (false)
				* @param box [out] the box constructed
				* @param error an error object reference to be populated
				* @return the leader box rectangle to use for obstacle detection
				* @reqid 001.0063
				*/
				void BuildSegmentBox( int segmentNumber, float leaderBoxBufferSize, 
					bool extendLenghtwise, Rectangle& box, Error& error );

				/**
				* Test if feature class of a feature overlap is allowed according
				* to the placement rule supplied
				* @param placementRule the placement rule
				* @param result [out] true if fclass is allowed
				* @param fo the overlap to test
				* @reqid 002.0039
				*/
				void CheckIfFeatureClassAllowed( const PlacementRule &placementRule, bool* result, 
					FeatureOverlap* fo );

				/**
				* Test the angle between the overlap and the leader line.
				* @param fo overlap
				* @param segment leader line segment
				* @param completeAnalysis if true all the overlap geometries will be
				*                         analyzed and flagged. If false first improper
				*                         geometry stops the tests and a result of false
				*                         is returned.
				* @param result [out] true if overlaps are at an allowed angle, false otherwise
				* @param error error code
				* @reqid 002.0039
				*/
				void CheckOverlapAngles( FeatureOverlap* fo, Vector& segment, bool completeAnalysis, 
					bool* result, Error& error );


				/**
				* Test two vectors, one proceeding in ccw direction, the other i cw direction 
				* and check if they are passing each other. The test is done by tracking they're
				* relative positions. The state is saved in the m_pieAnglePassed member.
				* The assumption is in place that both vectors start at the same position and 
				* then move apart going in opposite directions. After they pass the 180 degs 
				* angle, their cross product becomes negative. After that when it becomes 
				* positive again, we know they made a full circle. The m_pieAnglePassed 
				* member should be reset before calling the method for the first time.
				*
				* @param cwVector clockwise traveling vector
				* @param ccwVector counter-clockwise traveling vector
				* @return true if the vectors have done a full circle
				* @reqid 002.0039
				*/
				bool AreVectorsPassingEachOther( Vector& cwVector, Vector& ccwVector );

				/**
				* Given a polyline build a wedge of clockwise and counter-cw
				* vectors from the centroid to avoid it.
				* @param poly - the polyline to avoid
				* @param pivotalPoint - origin of the two vectors
				* @param cwAngle - [out] angle of the clockwise vector
				* @param cwMagnitude - [out] magnitude of the clockwise vector
				* @param ccwAngle - [out] angle of the counter-clockwise vector
				* @param ccwMagnitude - [out] magnitude of the counter-clockwise vector
				* @param obstacleAllAround - [out] set to true if vectors passed each other,
				*                          which means the polyline surrounds the feature centroid completely
				*                          or the obstacle is ~identical with the pivot point.
				* @reqid 002.0039
				*/
				void CalculateAvoidanceVectors( const IPolylineView* poly, 
					const Point &pivotalPoint, float* cwAngle, float* cwMagnitude, float* ccwAngle,
					float* ccwMagnitude, bool* obstacleAllAround );

				/**
				* Test if the geometry can be used to drive the obstacle avoidance or if it's
				* 'degenerate', meaning it's only composed of vertices which are VERY close to
				* the leader anchor, thus impossible to avoid but also harmless.
				* @param geometry - the geometry to test.
				* @param leaderAnchor - leader anchor, i.e. the end which cannot move.
				* @return true if the geometry is degenerate.
				* @reqid 002.0039
				*/
				bool IsGeometryDegenerate( const IPolylineView& geometry, const Point& leaderAnchor );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				const PolylineShape* m_leaderLine;
				Shape* m_arrowHead;
				int m_crossingsCount;
				bool m_pieAnglePassed;
				int m_color;
		};
	} // namespace Dtp
} // namespace MapText

#endif // DTP_LEADER_H