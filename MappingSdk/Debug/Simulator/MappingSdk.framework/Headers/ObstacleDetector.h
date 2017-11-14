/**************************************************
 * Copyright (c)2008 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://martin@mtsrv.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/LabelingEngine/ObstacleDetector.h $
 *
 * Revision: $Revision: 79292 $
 *
 * Last modified on: $Date: 2015-12-08 16:12:39 -0500 (Tue, 08 Dec 2015) $, by: $Author: mfraczak $
 *
 *************************************************/

#ifndef DTP_OBSTACLEDETECTOR_H
#define DTP_OBSTACLEDETECTOR_H

#include "LabelingEngineDefs.h"
#include "Collection.h"
#include "MtGeometry.h"
#include "PlacementRule.h"
#include "Configuration.h"
#include "LabelElement.h"

namespace MapText
{
	namespace Dtp
	{
		/**
		 * Forward declare pointer-only dependencies
		 */
		class ClippedPolyline;
		class Error;
		class Feature;
		class Leader;
		class IndexDictionaryEntry;
		class Map;
		class ObstacleDetectionArgs;
		class RegularLabel;
		class SymbolComponent;
		class TextPart;
		class Label;
		class PlacementState;

		/**
		 * Stores a single geometry overlap along with the information
		 * about shifts required to avoid this obstacle
		 * @reqid 001.0065
		 */
		class LABELINGENGINE_API OverlapGeometry
		{
			public:

				/**
				 * Constructor. Calculates the necessary shifts given some parameters.
				 * @param polyline the overlapping geometry
				 * @param clippingRectangle
				 * @param boundingBox bounding box of the overlapping geometry in the clipping
				 *                    rectangle coordinate system
				 * @reqid 001.0065
				 */
				OverlapGeometry( IPolylineView* polyline, Rectangle* clippingRectangle, 
					UprightRectangle* boundingBox );


				/**
				 * Constructor. Needs the shifts pre-calculated.
				 * @param polyline the overlapping geometry
				 * @param shiftLeft left-shift to avoid the obstacle
				 * @param shiftRight right-shift to avoid the obstacle
				 * @param shiftDown down-shift to avoid the obstacle
				 * @param shiftUp up-shift to avoid the obstacle
				 * @reqid 001.0064
				 */
				OverlapGeometry( IPolylineView* polyline, float shiftLeft, float shiftRight, float shiftDown, float shiftUp );

				/**
				 * Destructor
				 * @reqid 001.0065
				 */
				~OverlapGeometry();

				/**
				 * Get the overlap geometry
				 * @reqid 001.0065
				 */
				const IPolylineView* GetGeometry() const
				{
					return m_overlap;
				}

				/**
				 * Set the overlap geometry
				 * @param polylineView the geometry; ownership is assumed.
				 * @reqid 001.0065
				 */
				void SetGeometry( IPolylineView* polylineView )
				{
					m_overlap = polylineView;
				}

				/**
				 * @reqid 001.0065
				 */
				float GetShiftLeft() const
				{
					return m_shiftLeft + Configuration::GetObstacleAvoidanceShiftTolerance();
				}

				/**
				 * @reqid 001.0065
				 */
				float GetShiftRight() const
				{
					return m_shiftRight + Configuration::GetObstacleAvoidanceShiftTolerance();
				}

				/**
				 * @reqid 001.0065
				 */
				float GetShiftUp() const
				{
					return m_shiftUp + Configuration::GetObstacleAvoidanceShiftTolerance();
				}

				/**
				 * @reqid 001.0065
				 */
				float GetShiftDown() const
				{
					return m_shiftDown + Configuration::GetObstacleAvoidanceShiftTolerance();
				}

				/**
				 * @reqid 001.0065
				 */
				void SetShiftLeft(float shiftLeft)
				{
					m_shiftLeft = shiftLeft;
				}

				/**
				 * @reqid 001.0065
				 */
				void SetShiftRight(float shiftRight)
				{
					m_shiftRight = shiftRight;
				}

				/**
				 * @reqid 001.0065
				 */
				void SetShiftUp(float shiftUp)
				{
					m_shiftUp = shiftUp;
				}

				/**
				 * @reqid 001.0065
				 */
				void SetShiftDown(float shiftDown)
				{
					m_shiftDown = shiftDown;
				}

				/**
				* Gets Angle
				* @return Angle
				* @reqid 002.0039
				*/
				float GetCwAngle() const 
				{ 
					return m_cwAngle; 
				}

				/**
				* Sets Angle
				* @param val Angle
				* @reqid 002.0039
				*/
				void SetCwAngle(float val) 
				{ 
					m_cwAngle = val; 
				}

				/**
				* Gets Magnitude
				* @return Magnitude
				* @reqid 002.0039
				*/
				float GetCwMagnitude() const 
				{ 
					return m_cwMagnitude; 
				}

				/**
				* Sets Magnitude
				* @param val Magnitude
				* @reqid 002.0039
				*/
				void SetCwMagnitude(float val) 
				{ 
					m_cwMagnitude = val; 
				}

				/**
				* Gets CcwAngle
				* @return CcwAngle
				* @reqid 002.0039
				*/
				float GetCcwAngle() const 
				{ 
					return m_ccwAngle; 
				}

				/**
				* Sets CcwAngle
				* @param val CcwAngle
				* @reqid 002.0039
				*/
				void SetCcwAngle(float val) 
				{ 
					m_ccwAngle = val; 
				}

				/**
				* Gets CcwMagnitude
				* @return CcwMagnitude
				* @reqid 002.0039
				*/
				float GetCcwMagnitude() const 
				{ 
					return m_ccwMagnitude; 
				}

				/**
				* Sets CcwMagnitude
				* @param val CcwMagnitude
				* @reqid 002.0039
				*/
				void SetCcwMagnitude(float val) 
				{ 
					m_ccwMagnitude = val; 
				}

				/**
				* Analyzed is an auxiliary flag allowing partial analysis of
				* the summary including multiple overlaps. Initial analysis may mark
				* the overlap as analyzed and set the additional flag (@see GetFlag).
				* @return Analyzed
				* @reqid 001.0065
				*/
				bool GetAnalyzed() const 
				{ 
					return m_analyzed; 
				}

				/**
				* Sets Analyzed.
				* @see GetAnalyzed
				* @param val Analyzed
				* @reqid 001.0065
				*/
				void SetAnalyzed(bool val) 
				{ 
					m_analyzed = val; 
				}

				/**
				* Gets Flag. Usage and interpretation of this flag depends
				* on the client. It may be used for incremental analysis of the
				* overlap summery (@see GetAnalyzed).
				* @return Flag
				* @reqid 001.0065
				*/
				bool GetFlag() const 
				{ 
					return m_flag; 
				}

				/**
				* Sets Flag
				* @see GetFlag
				* @param val Flag
				* @reqid 001.0065
				*/
				void SetFlag(bool val) 
				{ 
					m_flag = val; 
				}

				/**
				 * Resets all the analysis flags (@see GetAnalyzed and GetFlag) to their initial
				 * state (false).
				 * @reqid 001.0065
				 */
				void ResetAnalysisFlags()
				{
					m_flag = false;
					m_analyzed = false;
				}

			private:
				IPolylineView* m_overlap;
				
				float m_shiftUp;
				float m_shiftDown;
				float m_shiftLeft;
				float m_shiftRight;
				float m_cwAngle;
				float m_cwMagnitude;
				float m_ccwAngle;
				float m_ccwMagnitude;
				bool m_analyzed;
				bool m_flag;
		};

		/**
		* Collection class
		*/
		class LABELINGENGINE_API OverlapGeometries : public LinkedList<OverlapGeometry>
		{
		public:

			/**
			* Constructor
			* @param ownsElements if true elements are owned by the collection and destroyed along with it.
			*        if false, they are not deleted with the collection
			* @see LinkedList::LinkedList
			* @reqid 001.0065
			*/
			OverlapGeometries( bool ownsElements )
				: LinkedList<OverlapGeometry>( ownsElements )
			{
			}

			/**
			* Destructor declared explicitly to avoid compiler generated code.
			* @reqid 001.0065
			*/
			~OverlapGeometries()
			{
			}
		};

		/**
		 * Abstract base class for storage of overlaps with different
		 * obstacles
		 * @reqid 001.0065
		 */
		class LABELINGENGINE_API ObstacleOverlap
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
				 * Type of the overlap
				 * @reqid 001.0065
				 */
				enum ObstacleOverlapType
				{
					ObstacleOverlapType_Feature,
					ObstacleOverlapType_Text,
					ObstacleOverlapType_Leader,
					ObstacleOverlapType_SymbolComponent,
				};

				/**
				 * Default constructor
				 * @reqid 001.0065
				 */
				ObstacleOverlap()
					: m_overlapGeometries( true ), m_analyzed( false ), m_flag( false ), 
					m_featureBeingLabeled(false)

				{
				}

				/**
				 * Get the type of the overlap
				 * @reqid 001.0065
				 */
				virtual ObstacleOverlapType GetObstacleType() const = 0;

				/**
				 * Make sure the destructors are virtual
				 * @reqid 001.0065
				 */
				virtual ~ObstacleOverlap()
				{
				}

				/**
				 * Access the overlap geometry
				 * @return the geometry
				 * @reqid 001.0065
				 */
				virtual OverlapGeometries* GetOverlapGeometries()
				{
					return &m_overlapGeometries;
				}

				/**
				* Analyzed is an auxiliary flag allowing partial analysis of
				* the summary including multiple overlaps. Initial analysis may mark
				* the overlap as analyzed and set the additional flag (@see GetFlag).
				* @return Analyzed
				* @reqid 001.0065
				*/
				bool GetAnalyzed() const 
				{ 
					return m_analyzed; 
				}

				/**
				* Sets Analyzed.
				* @see GetAnalyzed
				* @param val Analyzed
				* @reqid 001.0065
				*/
				void SetAnalyzed(bool val) 
				{ 
					m_analyzed = val; 
				}

				/**
				* Gets Flag. Usage and interpretation of this flag depends
				* on the client. It may be used for incremental analysis of the
				* overlap summery (@see GetAnalyzed).
				* @return Flag
				* @reqid 001.0065
				*/
				bool GetFlag() const 
				{ 
					return m_flag; 
				}

				/**
				* Sets Flag
				* @see GetFlag
				* @param val Flag
				* @reqid 001.0065
				*/
				void SetFlag(bool val) 
				{ 
					m_flag = val; 
				}

				/**
				* Gets featurebeinglabeled flag. This is the feature that is currently being labeled
				* @return featureBeingLabeled
				* @reqid 006.0002
				* @reqid 006.0003
				* @reqid 006.0004
				* @reqid 006.0006
				*/
				bool GetFeatureBeingLabeled() const 
				{ 
					return m_featureBeingLabeled; 
				}

				/**
				* Sets the featurebeinglabeled flag
				* @see GetFlag
				* @param val Flag
				* @reqid 001.0065
				*/
				void SetFeatureBeingLabeled(bool val) 
				{ 
					m_featureBeingLabeled = val; 
				}

				/**
				 * Resets all the analysis flags (@see GetAnalyzed and GetFlag) to their initial
				 * state (false).
				 * @return 
				 * @reqid 001.0065
				 */
				void ResetAnalysisFlags();

			private:
				OverlapGeometries m_overlapGeometries;
				bool m_analyzed;
				bool m_flag;
				bool m_featureBeingLabeled;
		};

		/**
		 * Overlap with an area feature
		 * @reqid 001.0065
		 */
		class LABELINGENGINE_API FeatureOverlap
			: public ObstacleOverlap
		{
			public:
				/**
				* Collection class
				*/
				class LABELINGENGINE_API Collection : public LinkedList<FeatureOverlap>
				{
				public:

					/**
					* Constructor
					* @param ownsElements if true elements are owned by the collection and destroyed along with it.
					*        if false, they are not deleted with the collection
					* @see LinkedList::LinkedList
					* @reqid 001.0065
					*/
					Collection( bool ownsElements )
						: LinkedList<FeatureOverlap>( ownsElements )
					{
					}

					/**
					* Destructor declared explicitly to avoid compiler generated code.
					* @reqid 001.0065
					*/
					~Collection()
					{
					}
				};

				/**
				 * Default constructor.
				 * @reqid 001.0065
				 */
				FeatureOverlap()
					: m_overlapSource( NULL )
				{
				}

				/**
				 * Constructor
				 * @param overlapSource the feature responsible for the overlap
				 * @reqid 001.0065
				 */
				FeatureOverlap( const Feature* overlapSource )
					: m_overlapSource( overlapSource )
				{
				}

				/**
				 * Destructor.
				 * @reqid 001.0065
				 */
				~FeatureOverlap()
				{
				}

				/**
				 * @see ObstacleOverlap::GetObstacleType
				 * @reqid 001.0065
				 */
				ObstacleOverlapType GetObstacleType() const
				{
					return ObstacleOverlapType_Feature;
				}

				/**
				 * Get the feature causing the overlap
				 * @return the feature
				 * @reqid 001.0065
				 */
				const Feature* GetOverlapSource() const
				{
					return m_overlapSource;
				}

			private:
				const Feature* m_overlapSource;
		};

		/**
		 * Overlap with a placed label - either a text part or a symbol component
		 * @reqid 001.0065
		 */
		class LABELINGENGINE_API TextOverlap
			: public ObstacleOverlap
		{
			public:
				/**
				* Collection class
				*/
				class LABELINGENGINE_API Collection : public LinkedList<TextOverlap>
				{
				public:

					/**
					* Constructor
					* @param ownsElements if true elements are owned by the collection and destroyed along with it.
					*        if false, they are not deleted with the collection
					* @see LinkedList::LinkedList
					* @reqid 001.0065
					*/
					Collection( bool ownsElements )
						: LinkedList<TextOverlap>( ownsElements )
					{
					}

					/**
					* Destructor declared explicitly to avoid compiler generated code.
					* @reqid 001.0065
					*/
					~Collection()
					{
					}
				};

				enum LabelPartType
				{
					LabelPartType_TextPart,
					LabelPartType_SymbolComponent,
				};

				/**
				 * @see ObstacleOverlap::GetObstacleType
				 * @reqid 001.0065
				 */
				ObstacleOverlapType GetObstacleType() const
				{
					return ObstacleOverlapType_Text;
				}

				/**
				 * Default constructor
				 * @reqid 001.0065
				 */
				TextOverlap()
					: m_textPart( NULL )
				{
				}

				/**
				 * Get the type of label part causing the overlap
				 * @return type of label part
				 * @reqid 001.0065
				 */
				LabelPartType GetLabelPartType() const
				{
					return m_labelPartType;
				}

				/**
				 * Constructor
				 * @param overlapSource the text part responsible for the overlap
				 * @reqid 001.0065
				 */
				TextOverlap( TextPart* overlapSource )
					:	m_textPart( overlapSource ), m_symbolComponent( NULL ), 
						m_labelPartType( LabelPartType_TextPart )
				{
				}

				/**
				* Constructor
				* @param overlapSource the symbol component responsible for the overlap
				* @reqid 001.0065
				*/
				TextOverlap( SymbolComponent* overlapSource )
					:	m_textPart( NULL ), m_symbolComponent( overlapSource ),
						m_labelPartType( LabelPartType_SymbolComponent )
				{
				}

				/**
				 * Get the text part which is causing this overlap
				 * @return text part
				 * @reqid 001.0065
				 */
				TextPart* GetTextPart() const
				{
					return m_textPart;
				}

				/**
				* Get the symbol component which is causing this overlap
				* @return symbol component
				* @reqid 001.0065
				*/
				SymbolComponent* GetSymbolComponent() const
				{
					return m_symbolComponent;
				}

			private:
				TextPart* m_textPart;
				SymbolComponent* m_symbolComponent;
				LabelPartType m_labelPartType;
		};

		/**
		 * Overlap with a leader
		 * @reqid 001.0065
		 */
		class LABELINGENGINE_API LeaderOverlap : public ObstacleOverlap
		{
			public:
				/**
				* Collection class
				*/
				class LABELINGENGINE_API Collection : public LinkedList<LeaderOverlap>
				{
				public:

					/**
					* Constructor
					* @param ownsElements if true elements are owned by the collection and destroyed along with it.
					*        if false, they are not deleted with the collection
					* @see LinkedList::LinkedList
					* @reqid 001.0065
					*/
					Collection( bool ownsElements )
						: LinkedList<LeaderOverlap>( ownsElements )
					{
					}

					/**
					* Destructor declared explicitly to avoid compiler generated code.
					* @reqid 001.0065
					*/
					~Collection()
					{
					}
				};

				/**
				 * @see ObstacleOverlap::GetObstacleType
				 * @reqid 001.0065
				 */
				ObstacleOverlapType GetObstacleType() const
				{
					return ObstacleOverlapType_Leader;
				}

				/**
				 * Default constructor
				 * @reqid 001.0065
				 */
				LeaderOverlap()
					: m_overlapSource( NULL )
				{
				}

				/**
				 * Constructor
				 * @param overlapSource the leader responsible for the overlap
				 * @reqid 001.0065
				 */
				LeaderOverlap( Leader* overlapSource )
					: m_overlapSource( overlapSource )
				{
				}

				/**
				 * Get the leader which is causing this overlap
				 * @reqid 001.0065
				 */
				Leader* GetOverlapSource() const
				{
					return m_overlapSource;
				}

			private:
				Leader* m_overlapSource;
		};

		class ObstacleDetector;

		/**
		 * This class encapsulates an overlap summary of a placement candidate.
		 * @reqid 001.0065
		 */
		class LABELINGENGINE_API ObstacleOverlapSummary
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
				 * Constructor
				 * @param query		the query to serve as the source of this overlap summary
				 * @reqid 001.0065
				 */
				ObstacleOverlapSummary( Rectangle& query );

				/**
				 * Destructor
				 * @reqid 001.0065
				 */
				~ObstacleOverlapSummary();

				/**
				 * Get the overlaps with area features
				 * @reqid 001.0065
				 */
				LinkedList<FeatureOverlap>* GetAreaFeatureOverlaps()
				{
					return &m_areaOverlaps;
				}

				/**
				 * Get the overlaps with line features
				 * @reqid 001.0065
				 */
				LinkedList<FeatureOverlap>* GetLineFeatureOverlaps()
				{
					return &m_lineOverlaps;
				}

				/**
				 * Get the overlaps with point features
				 * @reqid 001.0065
				 */
				LinkedList<FeatureOverlap>* GetPointFeatureOverlaps()
				{
					return &m_pointOverlaps;
				}

				/**
				 * Get the overlaps with placed text
				 * @reqid 001.0065
				 */
				LinkedList<TextOverlap>* GetTextOverlaps()
				{
					return &m_textOverlaps;
				}

				/**
				 * Get the overlaps with leaders
				 * @reqid 001.0065
				 */
				LinkedList<LeaderOverlap>* GetLeaderOverlaps()
				{
					return &m_leaderOverlaps;
				}

				/**
				 * Get the overlap status
				 * @reqid 001.0065
				 */
				bool GetIsOverlapping() const
				{
					return m_isOverlapping;
				}

				/**
				 * Check if self-overlap exists
				 * @return 
				 * @reqid 001.0065
				 */
				bool GetIsSelfOverlapping() const
				{
					return m_selfOverlapShiftDown != 0 
						|| m_selfOverlapShiftLeft != 0
						|| m_selfOverlapShiftRight != 0
						|| m_selfOverlapShiftUp != 0;
				}

				/**
				 * Set the overlap status
				 * @reqid 001.0065
				 */
				void SetIsOverlapping( bool overlaps )
				{
					m_isOverlapping = overlaps;
				}

				/**
				 * Get the overlap complete status
				 * @reqid 001.0065
				 */
				bool GetIsSummaryComplete() const
				{
					return m_isSummaryComplete;
				}

				/**
				 * Set the overlap complete status
				 * @reqid 001.0065
				 */
				void SetIsSummaryComplete( bool summaryComplete )
				{
					m_isSummaryComplete = summaryComplete;
				}

				/**
				 * @reqid 001.0065
				 */
				float GetShiftLeft() const
				{
					return m_shiftLeft;
				}

				/**
				 * @reqid 001.0065
				 */
				float GetShiftRight() const
				{
					return m_shiftRight;
				}

				/**
				 * @reqid 001.0065
				 */
				float GetShiftUp() const
				{
					return m_shiftUp;
				}

				/**
				 * @reqid 001.0065
				 */
				float GetShiftDown() const
				{
					return m_shiftDown;
				}

				/**
				 * @reqid 001.0065
				 */
				void SetShiftLeft(float shiftLeft)
				{
					m_shiftLeft = shiftLeft;
				}

				/**
				 * @reqid 001.0065
				 */
				void SetShiftRight(float shiftRight)
				{
					m_shiftRight = shiftRight;
				}

				/**
				 * @reqid 001.0065
				 */
				void SetShiftUp(float shiftUp)
				{
					m_shiftUp = shiftUp;
				}

				/**
				 * @reqid 001.0065
				 */
				void SetShiftDown(float shiftDown)
				{
					m_shiftDown = shiftDown;
				}

				/**
				* Gets NeatlineCollisionXShift
				* This is the horizontal shift needed to put the candidate completely within the
				* map area.
				* @return NeatlineCollisionXShift
				* @reqid 001.0065
				*/
				float GetNeatlineCollisionXShift() const 
				{ 
					return m_neatlineCollisionXShift; 
				}

				/**
				* Sets NeatlineCollisionXShift
				* @see GetNeatlineCollisionXShift
				* @param val NeatlineCollisionXShift
				* @reqid 001.0065
				*/
				void SetNeatlineCollisionXShift(float val) 
				{ 
					m_neatlineCollisionXShift = val; 
				}

				/**
				* Gets NeatlineCollisionYShift
				* This is the vertical shift needed to put the candidate completely within the
				* map area.
				* @return NeatlineCollisionYShift
				* @reqid 001.0065
				*/
				float GetNeatlineCollisionYShift() const 
				{ 
					return m_neatlineCollisionYShift; 
				}

				/**
				* Sets NeatlineCollisionYShift
				* @param val NeatlineCollisionYShift
				* @reqid 001.0065
				*/
				void SetNeatlineCollisionYShift(float val) 
				{ 
					m_neatlineCollisionYShift = val; 
				}

				/**
				 * Calculate and return the overall left shift to avoid
				 * both obstacles and neatline collision.
				 * @param shiftImpossible set to true if shift right is required to
				 * avoid neatline collision
				 * @return the shift or FLT_MAX if shift impossible
				 * @reqid 001.0065
				 */
				float GetOverallShiftLeft( bool *shiftImpossible ) const;

				/**
				* Calculate and return the overall right shift to avoid
				* both obstacles and neatline collision.
				* @param shiftImpossible set to true if shift left is required to
				* avoid neatline collision
				* @return the shift or FLT_MAX if shift impossible
				* @reqid 001.0065
				*/
				float GetOverallShiftRight( bool *shiftImpossible ) const;

				/**
				* Calculate and return the overall right up to avoid
				* both obstacles and neatline collision.
				* @param shiftImpossible set to true if shift down is required to
				* avoid neatline collision
				* @return the shift or FLT_MAX if shift impossible
				* @reqid 001.0065
				*/
				float GetOverallShiftUp( bool *shiftImpossible ) const;

				/**
				* Calculate and return the overall right down to avoid
				* both obstacles and neatline collision.
				* @param shiftImpossible set to true if shift up is required to
				* avoid neatline collision
				* @return the shift or FLT_MAX if shift impossible
				* @reqid 001.0065
				*/
				float GetOverallShiftDown( bool *shiftImpossible ) const;

				/**
				* Takes the input overlap and updates the overall rectilinear shifts based on the shifts
				* necessary to avoid the input overlap.
				* @param obstacleOverlap - obstacle to avoid
				* @reqid 001.0065
				*/
				void UpdateTotalShifts(ObstacleOverlap *obstacleOverlap);

				/**
				* Gets SelfOverlapShiftRight
				* @return SelfOverlapShiftRight
				* @reqid 004.0032
				*/
				float GetSelfOverlapShiftRight() const 
				{ 
					return m_selfOverlapShiftRight; 
				}

				/**
				* Gets SelfOverlapShiftLeft
				* @return SelfOverlapShiftLeft
				* @reqid 004.0032
				*/
				float GetSelfOverlapShiftLeft() const 
				{ 
					return m_selfOverlapShiftLeft; 
				}

				/**
				* Gets SelfOverlapShiftUp
				* @return SelfOverlapShiftUp
				* @reqid 004.0032
				*/
				float GetSelfOverlapShiftUp() const 
				{ 
					return m_selfOverlapShiftUp; 
				}

				/**
				* Gets SelfOverlapShiftDown
				* @return SelfOverlapShiftDown
				* @reqid 004.0032
				*/
				float GetSelfOverlapShiftDown() const 
				{ 
					return m_selfOverlapShiftDown; 
				}

				/**
				* Get the query which was the source of this overlap summary
				* @return Query rectangle
				* @reqid 001.0065
				*/
				const Rectangle* GetQuery() const
				{ 
					return &m_query; 
				}

				/**
				* Get whether the only overlap is self overlap
				* @return bool
				* @reqid 001.0065
				*/
				bool IsThereOnlySelfOverlap()
				{ 
					if(m_shiftLeft == 0 && m_shiftRight == 0 && m_shiftUp == 0 && m_shiftDown == 0 &&
						m_neatlineCollisionXShift == 0 && m_neatlineCollisionYShift == 0)
						return true;
					
					return false;
				}


				/**
				* This will reverse and up/down and left/right shifts and change the signs on the neatline shifts
				* @reqid 003.0025
				*/
				void ReverseShifts();


				/**
				 * This method is useful when an overlap summary is produced incrementally
				 *		for a composite object (e.g. a complex label with multiple text parts
				 *		or an assembly). In such case obstacle detection is performed on separate
				 *		parts of the composite objects and those results may be then merged into
				 *		one summary with the properly added shifts and all the obstacles found.
				 * @param summaryToAbsorb		This summary will be merged into the summary on 
				 *		which this method is involved. Content of the parameter summary are 
				 *		transfered to the master summary rendering it unusable, therefore it should
				 *		not be used afterwards.
				 *		An assumption is made that all the merger summaries were created using
				 *		query rectangles which were parallel (rotated at the same angle).
				 * @param error
				 * @reqid 003.0019
				 */
				void Absorb( ObstacleOverlapSummary* summaryToAbsorb, Error& error );

				/**
				 * Resets all the analysis flags (@see GetAnalyzed and GetFlag) to their initial
				 * state (false).
				 * @reqid 001.0065
				 */
				void ResetAnalysisFlags();

			private:

				FeatureOverlap::Collection m_areaOverlaps;
				FeatureOverlap::Collection m_lineOverlaps;
				FeatureOverlap::Collection m_pointOverlaps;
				TextOverlap::Collection m_textOverlaps;
				LeaderOverlap::Collection m_leaderOverlaps;
				bool m_isOverlapping;
				bool m_isSummaryComplete;

				float m_shiftUp;
				float m_shiftDown;
				float m_shiftLeft;
				float m_shiftRight;

				float m_selfOverlapShiftUp;
				float m_selfOverlapShiftDown;
				float m_selfOverlapShiftLeft;
				float m_selfOverlapShiftRight;

				float m_neatlineCollisionXShift;
				float m_neatlineCollisionYShift;

				Rectangle m_query;
		};

		/**
		 * Provides a way to inject logic into the obstacle detector by implementing
		 * a set of methods which will be called by obstacle detector during the course 
		 * of generating results. Application of Command design pattern.
		 */
		class LABELINGENGINE_API IObstacleAnalyzer
		{
		public:

			/**
			 * Test if an overlap is really an obstacle or if it can be ignored.
			 * @param obstacle The overlap to analyze.
			 * @param bufferedQueryBox The upright buffered box tested for overlap
			 * @param result True if it really is an obstacle, false otherwise.
			 * @param error Error information.
			 * @reqid 001.0065
			 */
			virtual void IsPotentialObstacle( IndexDictionaryEntry& obstacle, const Rectangle& bufferedQueryBox, bool* result ) = 0;

			/**
			 * You can chain the analyzers. Set the next one in chain. The logic of the chain is 
			 * 'AND'. If any analyzer in chain returns false as a result the analysis stops for 
			 * the entry and it's completely ignored.
			 * @param nextInChain - the analyzer to be chained.
			 * @param takeOwnership - if true ownership over the analyzer is taken.
			 * @reqid 001.0065
			 */
			void SetNext( IObstacleAnalyzer* nextInChain, bool takeOwnership )
			{
				if ( m_nextOwned )
				{
					delete m_next;
				}

				m_next = nextInChain;
				m_nextOwned = takeOwnership;
			}

			/**
			 * Get the next analyzer in chain.
			 * @return - next analyzer in chain.
			 * @reqid 001.0065
			 */
			IObstacleAnalyzer* GetNext()
			{
				return m_next;
			}

			virtual ~IObstacleAnalyzer()
			{
				if ( m_nextOwned )
				{
					delete m_next;
				}
			}
		protected:
			IObstacleAnalyzer() : m_next( NULL )
			{}

		private:
			IObstacleAnalyzer* m_next;
			bool m_nextOwned;
		};

		/**
		 * Implementation of the IObstacleAnalyzer which allows self-overlap of feature polylines,
		 * e.g., parts of a line feature or boundary of an area feature.
		 * if and only if the obstacles overlap the label -- not just the
		 * buffered label.
		 */
		class LABELINGENGINE_API SelfAllowedPolylineObstacleAnalyzer : public IObstacleAnalyzer
		{
		public:
			/**
			* Default constructor.
			* @reqid 004.0032
			*/
			SelfAllowedPolylineObstacleAnalyzer()
				:	m_labelOwner( NULL ),
					m_textToLineBuffer( 0.0f )
			{
			}

			/**
			* Analysis typical, for example, of entirely outside area feature placement:
			* - ignore self overlap if it occurs with buffered label only.
			*
			* @see IObstacleAnalyzer::IsObstacle
			* @reqid 004.0032
			*/
			void IsPotentialObstacle( IndexDictionaryEntry& obstacle, const Rectangle& bufferedQueryBox, bool* result );

			/**
			* Gets LabelOwner. Label owner is not treated as obstacle.
			* @return LabelOwner
			* @reqid 004.0032
			*/
			const Feature* GetLabelOwner() const 
			{ 
				return m_labelOwner; 
			}

			/**
			* Sets LabelOwner
			* @param val LabelOwner
			* @reqid 004.0032
			*/
			void SetLabelOwner(const Feature* val) 
			{ 
				m_labelOwner = val; 
			}

			/**
			* Gets TextToLineBuffer.
			* @return Label
			* @reqid 004.0032
			*/
			float GetTextToLineBuffer() const
			{
				return m_textToLineBuffer;
			}

			/**
			* Sets TextToLineBuffer
			* @param val TextToLineBuffer
			* @reqid 004.0032
			*/
			void SetTextToLineBuffer(float val)
			{
				m_textToLineBuffer = val;
			}

		private:
			const Feature* m_labelOwner;
			float m_textToLineBuffer;
		};

		/**
		 * This utility class may be used if you want to append an instance of the IObstacleAnalyzer
		 * to a chain of analyzers but only for the scope of the current block of code. 
		 * Provided analyzer will be appended at the end of the chain and trimmed from it when this 
		 * object goes out of scope.
		 */
		class LABELINGENGINE_API IObstacleAnalyzerScopedAppender
		{
		public:
			/**
			 * Constructor. Append an analyzer to the chain.
			 * @param chain - Chain to append to. If the chain is NULL then it will point to 
			 *                the analyzer to append. It will be reset to NULL when object is
			 *                destructed.
			 * @param analyzerToAppend - Analyzer to be appended.
			 * @reqid 001.0065
			 */
			IObstacleAnalyzerScopedAppender( IObstacleAnalyzer** chain, IObstacleAnalyzer& analyzerToAppend )
				: m_chain( chain )
			{
				if ( *chain == NULL )
				{
					*chain = &analyzerToAppend;
					m_lastElementOfTheChain = NULL;
				}
				else
				{
					m_lastElementOfTheChain = *chain;
					while ( m_lastElementOfTheChain->GetNext() )
					{
						m_lastElementOfTheChain = m_lastElementOfTheChain->GetNext();
					}

					m_lastElementOfTheChain->SetNext( &analyzerToAppend, false );
				}
			}

			/**
			 * Destructor. Reverts the changes to the chain done by the constructor.
			 * @reqid 001.0065
			 */
			~IObstacleAnalyzerScopedAppender()
			{
				if ( m_lastElementOfTheChain )
				{
					m_lastElementOfTheChain->SetNext( NULL, false );
				}
				else
				{
					*m_chain = NULL;
				}
			}

		private:
			IObstacleAnalyzer** m_chain;
			IObstacleAnalyzer* m_lastElementOfTheChain;
		};

		/**
		 * A spatial analyzer of the map
		 * @reqid 001.0065
		 */
		class LABELINGENGINE_API ObstacleDetector
		{
			public:
				/**
				 * Any combination of those flags is allowed to
				 * tell the analyzer precisely how to perform the analysis
				 * @reqid 001.0065
				 */
				enum ObstacleDetectionFlag
				{
					ObstacleDetectionFlag_None = 0,
					ObstacleDetectionFlag_CheckAreaFeatures = 1,
					ObstacleDetectionFlag_CheckLineFeatures = 2,
					ObstacleDetectionFlag_CheckPointFeatures = 4,
					ObstacleDetectionFlag_CheckText = 8,
					ObstacleDetectionFlag_CheckLeaders = 16,
					ObstacleDetectionFlag_CheckOnlyUntilFirstObstacleFound = 32,
					ObstacleDetectionFlag_CheckLabels =
						ObstacleDetectionFlag_CheckText
						| ObstacleDetectionFlag_CheckLeaders,
					ObstacleDetectionFlag_CheckAll =
						ObstacleDetectionFlag_CheckAreaFeatures
						| ObstacleDetectionFlag_CheckLineFeatures
						| ObstacleDetectionFlag_CheckPointFeatures
						| ObstacleDetectionFlag_CheckText
						| ObstacleDetectionFlag_CheckLeaders,
				};

				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_UnsupportedLabelType,
					ErrorCode_UnsupportedDictionaryEntryType,
				};

				/**
				 * Perform the spatial analysis and compute overlaps for a rectangle given other params:
				 * @param rect the rectangle to test
				 * @param feature owner of the label being placed
				 * @param map the map in which the placement is happening
				 * @param flags any combination of ObstacleDetectionFlag
				 * @param results [out] resulting overlaps
				 * @param error error reporting
				 * @reqid 001.0065
				 */
				static void GetOverlaps( Rectangle& rect, const Feature *feature, Map& map, ObstacleDetectionFlag flags,
					ObstacleOverlapSummary** results,
					Error& error );

				/**
				* Perform the spatial analysis and compute overlaps for a rectangle given other params:
				* @param rect the rectangle to test
				* @param feature owner of the label being placed
				* @param obstacleAnalyzer analyzer to customize the behavior of the obstacle
				*                         detector.
				* @param map the map in which the placement is happening
				* @param flags any combination of ObstacleDetectionFlag
				* @param results [out] resulting overlaps
				* @param error error reporting
				* @reqid 001.0065
				*/
				static void GetOverlaps( Rectangle& rect, const Feature *feature, IObstacleAnalyzer* obstacleAnalyzer, 
					Map& map, ObstacleDetectionFlag flags, ObstacleOverlapSummary** results,
					Error& error );

				/**
				 * Updates the neatline collision shifts - shifts needed to avoid being outside of the map region.
				 * @param args the ObstacleDetectionArgs to use
				 * @reqid 001.0065
				 */
				static void UpdateNeatlineCollisionShifts( ObstacleDetectionArgs& args );
			private:
				static unsigned int m_analysisCounter;
		};

		// Disable warning about assignment operator not being generated
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4512 )
#endif
		/**
		 * A vehicle for obstacle detection arguments passed throughout the
		 * obstacle detection call stack.
		 * @reqid 001.0065
		 */
		class LABELINGENGINE_API ObstacleDetectionArgs
		{
		public:

			/**
			 * Constructor.
			 * @param query obstacle detection query rectangle
			 * @param uprightQuery query rectangle rotated to be upright
			 * @param flags detection flags
			 * @param map master map
			 * @param summary summary passed to be updated
			 * @param feature feature being labeled
			 * @reqid 001.0065
			 */
			ObstacleDetectionArgs( Rectangle& query, Rectangle& uprightQuery, 
				ObstacleDetector::ObstacleDetectionFlag& flags, 
				Map& map, ObstacleOverlapSummary& summary, const Feature *feature)
				: m_query( query ), m_uprightQuery( uprightQuery ), 
				m_flags( flags ), m_map( map ), m_summary( summary ),
				m_analyzer( NULL ), m_featureBeingLabeled(feature)
			{
			}

			/**
			* Constructor.
			* @param query obstacle detection query rectangle
			* @param uprightQuery query rectangle rotated to be upright
			* @param flags detection flags
			* @param map master map
			* @param summary summary passed to be updated
			* @param obstacleAnalyzer interface pointer to obstacle analyzer to use
			* @param feature feature being labeled
			* @reqid 001.0065
			*/
			ObstacleDetectionArgs( Rectangle& query, Rectangle& uprightQuery, 
				ObstacleDetector::ObstacleDetectionFlag& flags, 
				Map& map, ObstacleOverlapSummary& summary, IObstacleAnalyzer* obstacleAnalyzer,
				const Feature *feature, unsigned int analysisCounter )
				: m_query( query ), m_uprightQuery( uprightQuery ), 
				m_flags( flags ), m_map( map ), m_summary( summary ), 
				m_analyzer( obstacleAnalyzer ), m_featureBeingLabeled(feature),
				m_analysisCounter( analysisCounter )
			{
			}

			/**
			* Gets Query
			* @return Query
			* @reqid 001.0065
			*/
			Rectangle* GetQuery()
			{ 
				return &m_query; 
			}

			/**
			* Gets UprightQuery
			* @return UprightQuery
			* @reqid 001.0065
			*/
			Rectangle* GetUprightQuery() const 
			{ 
				return &m_uprightQuery; 
			}

			/**
			* Gets Flags
			* @return Flags
			* @reqid 001.0065
			*/
			ObstacleDetector::ObstacleDetectionFlag GetFlags() const 
			{ 
				return m_flags; 
			}

			/**
			* Gets Map
			* @return Map
			* @reqid 001.0065
			*/
			Map* GetMap()
			{ 
				return &m_map; 
			}

			/**
			* Gets Summary
			* @return Summary
			* @reqid 001.0065
			*/
			ObstacleOverlapSummary* GetSummary()
			{ 
				return &m_summary; 
			}

			/**
			* Gets Analyzer
			* @return Analyzer
			* @reqid 003.0019
			*/
			IObstacleAnalyzer* GetAnalyzer() const 
			{ 
				return m_analyzer; 
			}

			/**
			* Gets the feature that is being labeled
			* @return FeatureBeingLabeled
			* @reqid 006.0002
			* @reqid 006.0003
			* @reqid 006.0004
			* @reqid 006.0006
			*/
			const Feature* GetFeatureBeingLabeled() const
			{
				return m_featureBeingLabeled;
			}

			/**
			* Gets AnalysisCounter
			* @return AnalysisCounter
			* @reqid 001.0065
			*/
			unsigned int GetAnalysisCounter() const 
			{ 
				return m_analysisCounter; 
			}

		private:
			Rectangle& m_query;
			Rectangle& m_uprightQuery;
			ObstacleDetector::ObstacleDetectionFlag m_flags;
			Map& m_map;
			ObstacleOverlapSummary& m_summary;
			IObstacleAnalyzer* m_analyzer;
			const Feature *m_featureBeingLabeled;
			unsigned int m_analysisCounter;
		};

		/**
		  * Abstract class that returns the appropriate buffer size from its contained PlacementRule and
		    which obstacles to check based on the derived type
		  * @reqid 006.0002
		  */
		class LABELINGENGINE_API BoxBuffer
		{
		public:

		    /* Constructor
		     * @reqid 006.0002
		     */
			BoxBuffer()
			{
				m_placementState = NULL;
			}

		    /* Destructor
		     * @reqid 006.0002
		     */
			virtual ~BoxBuffer(){}

		    /* Getter for the buffer size
			 * @return buffer size
		     * @reqid 006.0002
		     */
			virtual float GetBufferSize() const = 0;

		    /* Getter for which obstacles to check
			 * @return flag indicating which obstacles to check
		     * @reqid 006.0002
		     */
			virtual ObstacleDetector::ObstacleDetectionFlag GetWhichObstaclesToCheck() const = 0;

		    /* Sets a reference to the placement rule held by the buffer applicator
		     * @reqid 006.0002
		     */
			void SetPlacementState( PlacementState* placementState )
			{
				m_placementState = placementState;
			}

		protected:
			PlacementState* m_placementState;
		};

		/**
		  * @see BoxBuffer
		  * @reqid 006.0003
		  */
		class LABELINGENGINE_API LineBoxBuffer : public BoxBuffer
		{
		public:
			/**
			  * @see BoxBuffer::GetBufferSize
			  * @reqid 006.0003
			*/	
			virtual float GetBufferSize() const;

			/**
			  * @see BoxBuffer::GetWhichObstaclesToCheck
			  * @reqid 006.0003
			*/	
			virtual ObstacleDetector::ObstacleDetectionFlag GetWhichObstaclesToCheck() const
			{
				return (ObstacleDetector::ObstacleDetectionFlag)(ObstacleDetector::ObstacleDetectionFlag_CheckAreaFeatures | ObstacleDetector::ObstacleDetectionFlag_CheckLineFeatures);
			}
		};

		/**
		  * @see BoxBuffer
		  * @reqid 006.0002
		  */
		class LABELINGENGINE_API TextBoxBuffer : public BoxBuffer
		{
		public:
			/**
			  * @see BoxBuffer::GetBufferSize
			  * @reqid 006.0002
			*/	
			virtual float GetBufferSize() const;

			/**
			  * @see BoxBuffer::GetWhichObstaclesToCheck
			  * @reqid 006.0002
			*/	
			virtual ObstacleDetector::ObstacleDetectionFlag GetWhichObstaclesToCheck() const
			{
				return (ObstacleDetector::ObstacleDetectionFlag)ObstacleDetector::ObstacleDetectionFlag_CheckLabels;
			}
		};

		/**
		  * @see BoxBuffer
		  * @reqid 006.0004
		  */
		class LABELINGENGINE_API PointBoxBuffer : public BoxBuffer
		{
		public:
			/**
			  * @see BoxBuffer::GetBufferSize
			  * @reqid 006.0004
			*/	
			virtual float GetBufferSize() const;

			/**
			  * @see BoxBuffer::GetWhichObstaclesToCheck
			  * @reqid 006.0004
			*/	
			virtual ObstacleDetector::ObstacleDetectionFlag GetWhichObstaclesToCheck() const
			{
				return (ObstacleDetector::ObstacleDetectionFlag)ObstacleDetector::ObstacleDetectionFlag_CheckPointFeatures;
			}
		};

		/**
		  * Supports traversal of the different buffer sizes held by a PlacementRule and facilitates obstacle 
		    checking of the different box sizes based on these different buffer sizes.
		  * @reqid 006.0002
		  */
		class LABELINGENGINE_API BufferApplicator
		{
		public:

			/**
			  * Constructor
			  * @reqid 006.0002
			*/	
			BufferApplicator()
			{
				m_currentBoxBuffer = 0;
				m_placementState = NULL;

				m_boxBuffers[0] = new TextBoxBuffer;
				m_boxBuffers[1] = new PointBoxBuffer;
				m_boxBuffers[2] = new LineBoxBuffer;

				for(int i=0;i<3;i++)
				{
					m_boxBuffers[i]->SetPlacementState( m_placementState );
				}
			}

			/**
			  * Destructor
			  * @reqid 006.0002
			*/	
			virtual ~BufferApplicator()
			{
				for(int i=0;i<3;i++)
				{
					delete m_boxBuffers[i];
				}
			}

			/**
			  * Gets the first box buffer whose buffer should be applied during obstacle detection
			  * @return box buffer to be applied
			  * @reqid 006.0002
			*/	
			const BoxBuffer *GetFirstBoxBuffer()
			{
				m_currentBoxBuffer = 0;
				return m_boxBuffers[0];
			}

			/**
			  * Gets the next box buffer whose buffer should be applied during obstacle detection
			  * @return box buffer to be applied
			  * @reqid 006.0002
			*/	
			const BoxBuffer *GetNextBoxBuffer()
			{
				m_currentBoxBuffer++;

				if ( m_currentBoxBuffer < 3 )
				{
					return m_boxBuffers[m_currentBoxBuffer];
				}
				else
				{
					return NULL;
				}
			}

			/**
			  * Set the placement state containing the buffer sizes to be applied during obstacle detection
			  * @param placementState - placement state that holds the current buffer sizes to be applied
			  * @reqid 006.0002
			*/	
			void SetPlacementState(PlacementState *placementState)
			{
				m_placementState = placementState;

				for(int i=0;i<3;i++)
				{
					m_boxBuffers[i]->SetPlacementState( placementState );
				}
			}

		private:
			BoxBuffer* m_boxBuffers[3];
			int m_currentBoxBuffer;
			PlacementState* m_placementState;
		};

		/**
		 * Implementation of the IObstacleAnalyzer which ignores the feature classes and types
		 * defined as invisible in the placement rule.
		 */
		class LABELINGENGINE_API InvisibleObstacleAnalyzer : public IObstacleAnalyzer
		{
		public:

			/**
			 * Constructor. Placement Rule needs to be supplied.
			 * @param placementRule - if it defines a list of invisible feature classes,
			 *                        features belonging to this class will be ignored
			 *                        during the obstacle detection.
			 */
			InvisibleObstacleAnalyzer( PlacementRule& placementRule )
				: m_placementRule( &placementRule )
			{
			}

			/**
			 * @see IObstacleAnalyzer::IsPotentialObstacle
			 * @param obstacle - potential obstacle
			 * @param result - [out] if true the obstacle should be analyzed. 
			 *                 It should be ignored otherwise.
			 * @reqid 006.0011
			 */
			void IsPotentialObstacle( IndexDictionaryEntry& obstacle, const Rectangle& bufferedQueryBox, bool* result );

		protected:
			PlacementRule* m_placementRule;
		};


#ifdef _MSC_VER
#pragma warning( pop )
#endif
	} // namespace Dtp
} // namespace MapText

#endif // DTP_OBSTACLEDETECTOR_H
