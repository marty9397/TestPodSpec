/**************************************************
 * Copyright (c)2008 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://martin@mtsrv.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/LabelingEngine/PlacementRule.h $
 *
 * Revision: $Revision: 70920 $
 *
 * Last modified on: $Date: 2015-02-04 14:55:22 -0500 (Wed, 04 Feb 2015) $, by: $Author: mark $
 *
 *************************************************/

#ifndef DTP_PLACEMENTRULE_H
#define DTP_PLACEMENTRULE_H

#include "LabelingEngineDefs.h"
#include "Error.h"
#include "Collection.h"
#include "SharedObjects.h"
#include "MtGeometry.h"
#include "Units.h"

#include "Feature.h"

#include "Serialization.h"

namespace MapText
{
	namespace Dtp
	{
		/**
		 * Forward declare pointer-only dependencies
		 */
		class ExecutionSequence;

		/**
		 * Base class for a placement rule describing the placement pipeline
		 * for a label class.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API PlacementRule
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_IncorrectJustification,
					ErrorCode_IncorrectLabelDirection,
					ErrorCode_NoFeatureTypeSpecified,
					ErrorCode_UnhandledFeatureType,
					ErrorCode_PlacementRuleNotFound,
					ErrorCode_MinOffsetHasToBeNotLesserThanZero,
					ErrorCode_MinOffsetHasToBeNotGreaterThanMaxOffset,
					ErrorCode_MaxOffsetHasToBeNotLesserThanZero,
					ErrorCode_MaxOffsetHasToBeNotLesserThanMinOffset,
					ErrorCode_LeaderMaxCrossingsHasToBeNotLesserThanZero,
					ErrorCode_LeaderWidthHasToBeGreaterThanZero,
					ErrorCode_AnchoringZoneAroundVerticesSizeHasToBeNotLesserThanZero,
					ErrorCode_StackSpacingHasToBeNotLesserThanZero,
					ErrorCode_MaxLeaderLengthHasToBeNotLesserThanMinLeaderLength,
					ErrorCode_MinLeaderLengthHasToBeGreaterThanZero,
					ErrorCode_MaxLeaderLengthHasToBeGreaterThanZero,
					ErrorCode_MinFontSizeHasToBeNotLesserThanZero,
					ErrorCode_FontSizeStepHasToBeNotLesserThanZero,
					ErrorCode_LeaderTargetModeCannotBeNoLeader,
					ErrorCode_LeaderAnchorModeCannotBeNoLeader,
				};

				/**
				* Possible leader target modes.
				* @reqid 002.0039
				*/
				enum LeaderTargetMode
				{
					LeaderTargetMode_Center,
					LeaderTargetMode_FeaturePolyline,
					LeaderTargetMode_NoLeader
				};

				/**
				* Possible leader anchor modes.
				* @reqid 002.0039
				*/
				enum LeaderAnchorMode
				{
					LeaderAnchorMode_Center,
					LeaderAnchorMode_FeaturePolyline,
					LeaderAnchorMode_InsidePolyline,
					LeaderAnchorMode_NoLeader
				};

				/**
				* Describes where the leader line can be anchored on the feature.
				*/
				enum LeaderAnchorOnFeature
				{
					/**
					 * Leader line can only point to the vertices of the line feature and small zone
					 * around the vertex visually "belonging" the vertex.
					 * @see Configuration::GetFeatureAnchorSegmentEndZone
					 */
					LeaderAnchorOnFeature_VerticesOnly = 1,
					/**
					* Leader line can only point to the segments of the feature excluding the zone
					* around the vertices still visually belonging to the vertex. 
					* @see Configuration::GetFeatureAnchorSegmentEndZone
					*/
					LeaderAnchorOnFeature_SegmentsOnly = 2,
					/**
					* Leader line can only point to the start point of the line.
					*/
					LeaderAnchorOnFeature_StartPointOnly = 4,
					/**
					* Leader line can only point to the end point of the line.
					*/
					LeaderAnchorOnFeature_EndPointOnly = 8,
					/**
					* Leader line can point to start or end point of the line.
					*/
					LeaderAnchorOnFeature_StartOrEndPointOnly = 4 | 8,
					/**
					* Leader line can point to any part of the line.
					*/
					LeaderAnchorOnFeature_Anywhere = 32,
				};

				/**
				 * Possible orientations of a label relative to a line feature
				 * @reqid 001.0063
				 */
				enum OrientationType
				{
					OrientationType_Undefined = -1, 
					OrientationType_Horizontal = 0,
					OrientationType_Conform,
					OrientationType_Tangent,
					OrientationType_Perpendicular
				};

				/**
				 * Collection of placement rules
				 * @reqid 001.0063
				 */
				class LABELINGENGINE_API Collection : public StringKeyedTree< PlacementRule >
				{
					public:
						/**
						 * Default constructor
						 * @reqid 001.0063
						 */
						Collection()
						{
							m_units=Unit_Inch;
						}

						/**
						 * Destructor
						 * @reqid 001.0063
						 */
						~Collection()
						{
						}	

						/**
						 * Find the first placement rule of a given name
						 * @param name						name of the requested placement rule
						 * @param error						an Error object
						 * @param placementRule		[out] the requested rule or NULL if not found
						 * @param error						an Error object
						 * @reqid 001.0063
						 */
						void GetPlacementRule( const TCHAR* name, PlacementRule** placementRule, Error& error ) const;

						/**
						 * Create a new placement rule and add it to the collection. Its default units are
						   in inches. Use ConvertUnitsTo() to change the units before setting property values.
						 * @param name					the name used to identify the placement rule
						 * @param featureType		the feature type of the feature to be labeled
						 * @param placementRule	[out] the newly created rule
						 * @param error					an Error object
						 * @reqid 001.0063
						 */
						void CreatePlacementRule( const TCHAR* name,
							FeatureType featureType,
							PlacementRule** placementRule, 
							Error& error );

						/**
						 * Sets the units of all placement rules stored within this collection
						 * @param units - units in which placement rules are stored.
						 * @reqid 001.0063
						 */
						void SetUnits(Unit units)
						{
							m_units=units;
						}

						/**
						 * @see SetUnits
						 * @return - units in which the placement rules are stored.
						 * @reqid 001.0063
						 */
						Unit GetUnits()
						{
							return m_units;
						}

				private:

					Unit m_units;
				};

				/**
				* Class for a buffer rule with contains the possible buffer information
				* This class is used as the type for the 4 supported buffer types.
				* @reqid 001.0063
				*/
				class LABELINGENGINE_API BufferRule
				{
				public:

					enum ErrorCode
					{
						ErrorCode_BufferSizeHasToBeNotLesserThanZero = 1,
						ErrorCode_BufferSizeHasToBeNotLesserThanMinBufferSize,
						ErrorCode_MinBufferSizeHasToBeNotLesserThanZero,
						ErrorCode_MinBufferSizeHasToBeNotGreaterThanBufferSize,
						ErrorCode_BufferStepHasToBeGreaterThanZero,
					};
					/**
					* Default constructor.
					* @reqid 001.0063
				    */
					BufferRule()
					{
						m_bufferSize = m_minBufferSize = m_bufferStep = 0;
						m_minBufferSizeUnitType = UnitType_Absolute;
						m_bufferSizeUnitType = UnitType_Absolute;
						m_bufferStepUnitType = UnitType_Absolute;
					}

					/**
					* Default destructor.
					* @reqid 001.0063
					*/
					~BufferRule()
					{
					}

					/**
					 * Copy constructor.
					 * @param bufferRule buffer rule to copy
					 * @reqid 001.0063
					 */
					BufferRule(const BufferRule& bufferRule)
					{
						Copy( bufferRule, *this );
					}

					/**
					* Assignment operator.
					* @param rhs source instance
					* @return instance identical to rhs
					* @reqid 001.0063
					*/
					BufferRule& operator=(const BufferRule& rhs)
					{
						if ( this == &rhs )
						{
							return *this;
						}

						Copy( rhs, *this );

						return *this;
					}

					/**
					 * Get the buffer size
					 * @reqid 001.0063
				     */
					float GetBufferSize() const
					{
						return m_bufferSize;
					}

					/**
					 * Set the buffer size. Specify the value in the units of the PlacementRule if not being given as a percentage.
					 * @reqid 001.0063
				     */
					void SetBufferSize( float bufferSize, Error& error )
					{
						if ( bufferSize < 0 )
						{
							SET_ERROR_NOMSG( error, ErrorCode_BufferSizeHasToBeNotLesserThanZero );
							return;
						}

						//////////////////////////////////////////////////////////////////////////////////////
						// The following validation rule is order dependent.
						// We cannot control the order in which SetBufferSize and SetMinBufferSize are called.
						// We initialize the buffer size to 0.
						// If SetMinBufferSize is called first, it will fail.
						// We cannot make this into a unified setter that sets both values.
						// The config parser would have to maintain state in an unnatural way
						// to make the call when both properties have been set on a feature class.
						// An occurrence of buffer size < min buffer size on execution of a
						// BUFFLOOP or BUFFREDUCE command simply prevents the command from reducing
						// the applicable buffer size, which seems like the proper interpretation.
						//////////////////////////////////////////////////////////////////////////////////////

						//if ( bufferSize < m_minBufferSize )
						//{
						//	SET_ERROR_NOMSG( error, ErrorCode_BufferSizeHasToBeNotLesserThanMinBufferSize );
						//	return;
						//}

						m_bufferSize = bufferSize;
					}

					/**
					 * Get the minimum buffer size
					 * @reqid 001.0063
				     */
					float GetMinBufferSize() const
					{
						return m_minBufferSize;
					}

					/**
					 * Set the minimum buffer size. Specify the value in the units of the PlacementRule if not being given as a percentage.
					 * @reqid 001.0063
				     */
					void SetMinBufferSize( float minBufferSize, Error& error )
					{
						if ( minBufferSize < 0 )
						{
							SET_ERROR_NOMSG( error, ErrorCode_MinBufferSizeHasToBeNotLesserThanZero );
							return;
						}

						//////////////////////////////////////////////////////////////////////////////////////
						// The following validation rule is order dependent.
						// We cannot control the order in which SetBufferSize and SetMinBufferSize are called.
						// We initialize the buffer size to 0.
						// If SetMinBufferSize is called first, it will fail.
						// We cannot make this into a unified setter that sets both values.
						// The config parser would have to maintain state in an unnatural way
						// to make the call when both properties have been set on a feature class.
						// An occurrence of buffer size < min buffer size on execution of a
						// BUFFLOOP or BUFFREDUCE command simply prevents the command from reducing
						// the applicable buffer size, which seems like the proper interpretation.
						//////////////////////////////////////////////////////////////////////////////////////

						//if ( minBufferSize > m_bufferSize )
						//{
						//	SET_ERROR_NOMSG( error, ErrorCode_MinBufferSizeHasToBeNotGreaterThanBufferSize );
						//	return;
						//}

						m_minBufferSize = minBufferSize;
					}

					/**
					 * Get the buffer step
					 * @reqid 001.0063
				     */
					float GetBufferStep() const
					{
						return m_bufferStep;
					}

					/**
					 * Set the buffer step. Specify the value in the units of the PlacementRule
					 * if not being given as a percentage.
					 * @reqid 001.0063
				     */
					void SetBufferStep( float bufferStep, Error& error )
					{
						if ( bufferStep <= 0 )
						{
							SET_ERROR_NOMSG( error, ErrorCode_BufferStepHasToBeGreaterThanZero );
							return;
						}

						m_bufferStep = bufferStep;
					}

					/**
					* Gets MinBufferSizeUnitType
					* @return - MinBufferSizeUnitType
					* @reqid 001.0063
					*/
					MapText::Dtp::UnitType GetMinBufferSizeUnitType() const 
					{ 
						return m_minBufferSizeUnitType; 
					}

					/**
					* Sets MinBufferSizeUnitType
					* @param val - MinBufferSizeUnitType
					* @reqid 001.0063
					*/
					void SetMinBufferSizeUnitType(MapText::Dtp::UnitType val) 
					{ 
						m_minBufferSizeUnitType = val; 
					}

					/**
					* Gets BufferSizeUnitType
					* @return - BufferSizeUnitType
					* @reqid 001.0063
					*/
					UnitType GetBufferSizeUnitType() const 
					{ 
						return m_bufferSizeUnitType; 
					}

					/**
					* Sets BufferSizeUnitType
					* @param val - BufferSizeUnitType
					* @reqid 001.0063
					*/
					void SetBufferSizeUnitType(UnitType val) 
					{ 
						m_bufferSizeUnitType = val; 
					}

					/**
					* Gets BufferStepUnitType
					* @return - BufferStepUnitType
					* @reqid 001.0063
					*/
					MapText::Dtp::UnitType GetBufferStepUnitType() const 
					{ 
						return m_bufferStepUnitType; 
					}

					/**
					* Sets BufferStepUnitType
					* @param val - BufferStepUnitType
					* @reqid 001.0063
					*/
					void SetBufferStepUnitType(MapText::Dtp::UnitType val) 
					{ 
						m_bufferStepUnitType = val; 
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

				private:
					/**
					 * Copy one buffer rule into another.
					 * @param src - Source rule.
					 * @param dest - [out] Target rule.
					 * @reqid 001.0063
					 */
					void Copy( const BufferRule& src, BufferRule& dest )
					{
						dest.m_bufferSize = src.m_bufferSize;
						dest.m_bufferSizeUnitType = src.m_bufferSizeUnitType;
						dest.m_bufferStep = src.m_bufferStep;
						dest.m_bufferStepUnitType = src.m_bufferStepUnitType;
						dest.m_minBufferSize = src.m_minBufferSize;
						dest.m_minBufferSizeUnitType = src.m_minBufferSizeUnitType;
					}

					UnitType m_minBufferSizeUnitType;
					UnitType m_bufferSizeUnitType;
					UnitType m_bufferStepUnitType;

					float m_bufferSize;
					float m_minBufferSize;
					float m_bufferStep;
				};

				/*
				 * Possible placement state type codes
				 * @reqid 001.0063
				 */
				enum PlacementRuleType
				{
					PlacementRuleType_Area,
					PlacementRuleType_Line,
					PlacementRuleType_Point
				};

				/**
				 * Justification of the label
				 * @reqid 001.0063
				 */
				enum LabelJustification
				{
					LabelJustification_Unknown = -1,
					LabelJustification_Feature,
					LabelJustification_Left,
					LabelJustification_Center,
					LabelJustification_Right
				};

				/**
				 * Options for anchoring the leader
				 * @reqid 001.0063
				 */
				enum AnchorLeaderWhere
				{
					AnchorLeaderWhere_Corners = 0,			// The default.
					AnchorLeaderWhere_LeftAndRightSide,
					AnchorLeaderWhere_TopAndBottomSide,
					AnchorLeaderWhere_Anywhere
				};

				/**
				 * The label direction options for line feature placement
				 * @reqid 003.0062
				 */
				enum LabelDirection
				{
					LabelDirection_Unknown = -1,
					LabelDirection_FollowRightSideUpDirection = 0,			// The default.
					LabelDirection_FollowGeometryDirection,
					LabelDirection_FollowReversedGeometryDirection
				};

			protected:
				/**
				 * Default constructor
				 * @param error		an error object to be populated
				 * @reqid 001.0063
				 */
				PlacementRule(Error& error);

				/**
				 * Constructor -- build a placement rule with a given name and feature type.
				 * @param name					the name to identify the rule
				 * @param featureType		type of the feature this rule applies to
				 * @param error					an error object to be populated
				 * @reqid 001.0063
				 */
				PlacementRule( const TCHAR* name, FeatureType featureType, Error& error );

				/**
				* Initializer -- invoked by constructors to populate common members.
				* @param error					an error object to be populated
				* @reqid 001.0063
				*/
				void Initialize(Error& error);

			public:
				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				virtual ~PlacementRule();

				/**
				* Get the text to text buffer rule
				* @return text to text buffer rule
				* @reqid 001.0063
				*/
				BufferRule* GetTexttoTextBufferRule()
				{
					return &m_textToTextBuffer;
				}

				/**
				* Get the text to line buffer rule
				* @return text to line buffer rule
				* @reqid 001.0063
				*/
				BufferRule* GetTexttoLineBufferRule()
				{
					return &m_textToLineBuffer;
				}

				/**
				* Get the text to point buffer rule
				* @return text to point buffer rule
				* @reqid 001.0063
				*/
				BufferRule* GetTexttoPointBufferRule()
				{
					return &m_textToPointBuffer;
				}

				/**
				 * Get the minimum offset
				 * @reqid 001.0063
				 */
				float GetMinOffset() const
				{
					return m_minOffset;
				}

				/**
				 * Set the minimum offset. Specify the value in the units of the PlacementRule.
				 * @reqid 001.0063
				 */
				void SetMinOffset( float minOffset, Error& error )
				{
					if ( minOffset < 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_MinOffsetHasToBeNotLesserThanZero );
						return;
					}

					if ( minOffset > m_maxOffset )
					{
						SET_ERROR_NOMSG( error, ErrorCode_MinOffsetHasToBeNotGreaterThanMaxOffset );
						return;
					}
					m_minOffset = minOffset;
				}

				/**
				 * Get the maximum offset. Specify the value in the units of the PlacementRule.
				 * @reqid 001.0063
				 */
				float GetMaxOffset() const
				{
					return m_maxOffset;
				}

				/**
				* Set the maximum offset
				* @reqid 003.0019
				*/
				void SetMaxOffset( float maxOffset, Error& error )
				{
					if ( maxOffset < 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_MaxOffsetHasToBeNotLesserThanZero );
						return;
					}

					if ( maxOffset < m_minOffset )
					{
						SET_ERROR_NOMSG( error, ErrorCode_MaxOffsetHasToBeNotLesserThanMinOffset );
						return;
					}
					m_maxOffset = maxOffset;
				}

				/**
				 * Set label anchor component name.
				 * Name of component of label to anchor to line feature.
				 * @param anchorLabelPartName - name of object of label for anchoring
				 * @param error	Error object to be populated
				 * @reqid 003.0019
				 */
				void AssignLabelPartAnchorName( const SharedString &anchorLabelPartName, Error &error )
				{
					m_anchorLabelPartName.AssignString(anchorLabelPartName.GetString(), error);						
				}

				/**
				* Get label anchor component name.
				* Name of component of label to anchor to line feature.
				* @reqid 001.0063
				*/
				const SharedString *GetLabelPartAnchorName() const
				{
					return &m_anchorLabelPartName;
				}

				/* Get the allowed overline list
				 * A comma delimited list, in preference order, of the names of feature classes
				 * to permit line overlap -- line feature or area feature boundary.
				 * @reqid 001.0063
				 */
				const SharedString* GetAllowOverlineList() const
				{
					return &m_allowOverlineList;
				}

				/**
				 * Set the allowed overline list
				 * @see GetAllowOverlineList
				 * @reqid 001.0063
				 */
				void SetAllowOverlineList( const TCHAR* allowOverlineList, Error& error )
				{
					m_allowOverlineList.AssignString(allowOverlineList, error);
				}

				/* Get the shouldAddGraticuleAngleToPlacementAngle flag
				 * @reqid 002.0037
				 */
				bool GetShouldAddGraticuleAngleToPlacementAngle() const
				{
					return m_shouldAddGraticuleAngleToPlacementAngle;
				}

				/**
				 * Set the shouldAddGraticuleAngleToPlacementAngle
				 * @reqid 002.0037
				 */
				void SetShouldAddGraticuleAngleToPlacementAngle( bool val )
				{
					m_shouldAddGraticuleAngleToPlacementAngle = val;
				}

				/**
				 * @see SetMaxLabelLength
				   @return the maximum label length
				 * @reqid 002.0037
				 */
				float GetMaxLabelLength()
				{
					return m_maxLabelLength;
				}

				/**
				 * @param maxLabelLength - specifies the maximum allowed length of each line of text in a regular label. 
						If any line of text is longer than this value, the text cannot be placed unless the label is re-configured by stacking, font reduction, etc.
				 * @reqid 002.0037
				 */
				void SetMaxLabelLength(float maxLabelLength)
				{
					m_maxLabelLength=maxLabelLength;
				}

				/**
				 * Get the allowed leader cross feature class name list.
				 * In preference order, of the names of feature classes to permit leader overlap of lines
				 * for line feature or area feature boundary.
				 * @return a collection of shared strings representing the
				 *  names of feature classes allowed to be crossed
				 * @reqid 001.0063
				 */
				const SharedString::Collection* GetAllowLeaderCrossList() const
				{
					return &m_allowLeaderCrossList;
				}

				/**
				* Add a feature class name to the allowed leader cross list.
				* @param featureClassName		the feature class name to add to the list, as a shared string.
				* @param error							an Error object
				* @reqid 001.0063
				*/
				void AddAllowLeaderCrossFeatureClassName( SharedString* featureClassName, Error& error )
				{
					m_allowLeaderCrossList.AddElement( featureClassName, error );
				}

				/**
				 * Set the leader anchoring option
				 * @reqid 001.0063
				 */
				void SetAnchorLeaderWhere( AnchorLeaderWhere anchorLeaderWhere )
				{
					m_anchorLeaderWhere = anchorLeaderWhere;
				}

				/**
				 * Get the leader anchoring option
				 * @return the part of label to anchor the leader from the AnchorLeaderWhere enumeration
				 * @reqid 001.0063
				 */
				AnchorLeaderWhere GetAnchorLeaderWhere() const
				{
					return m_anchorLeaderWhere;
				}

				/**
				 * Set the leader arrow style.
				 * Arrowhead style index into mtleads.ini
				 * @reqid 001.0063
				 */
				void SetArrowStyle( int arrowStyle )
				{
					m_arrowStyle = arrowStyle;
				}

				/**
				 * Get the leader arrow style.
				 * Arrowhead style index into mtleads.ini
				 * @reqid 001.0063
				 */
				int GetArrowStyle() const
				{
					return m_arrowStyle;
				}

				/**
				 * This method creates a clone of the IUserData implementation supplied
				 * to the SetLeaderUserData method and returns it. If no implementation
				 * has been provided it returns NULL.
				 * @param error - Error info, possibly the allocation may fail.
				 * @return - The new cloned instance or NULL if no implementation hasbeen provided.
				 *           Client takes ownership of the clone.
				 * @reqid 001.0063
				 */
				int GetLeaderColor() const
				{
					return m_leaderColor;
				}

				/**
				 * The color of every instance of a leader created using this rule. 
				 * @param color - color of the leader
				 *                         Ownership is assumed.
				 * @reqid 001.0063
				 */
				void SetLeaderColor( int color )
				{
					m_leaderColor = color;
				}

				/**
				 * Set the maximum number of leader crossings
				 * @reqid 001.0063
				 */
				void SetMaxLeaderCrossings( int maxLeaderCrossings, Error& error )
				{
					if ( maxLeaderCrossings < 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_LeaderMaxCrossingsHasToBeNotLesserThanZero );
						return;
					}
					m_maxLeaderCrossings = maxLeaderCrossings;
				}

				/**
				 * Get the maximum number of leader crossings
				 * @reqid 001.0063
				 */
				int GetMaxLeaderCrossings() const
				{
					return m_maxLeaderCrossings;
				}

				/**
				 * Get the allow overregion list
				 * A comma delimited list, in preference order, of the names of polygon feature classes
				 * from the ExclusionPolygons list whose interiors can be overlapped if the
				 * OVERREGION sequence command is invoked.
				 * @reqid 001.0063
				 */
				const SharedString* GetAllowOverregionList() const
				{
					return &m_allowOverregionList;
				}

				/**
				 * Set the allow overregion list
				 * @reqid 001.0063
				 */
				void SetAllowOverregionList( const TCHAR* allowOverregionList, Error& error )
				{
					m_allowOverregionList.AssignString(allowOverregionList, error);
				}

				/**
				 * Set the minimum and maximum leader line length. Specify the value in the units of the PlacementRule.
				 * @reqid 001.0063
				 */
				void SetMinMaxLeaderLength( float minLeaderLength, float maxLeaderLength, Error& error )
				{
					if ( minLeaderLength <= 0 )
					{
						SET_ERROR( error, ErrorCode_MinLeaderLengthHasToBeGreaterThanZero,
							"Min leader length has to be > 0.");
						return;
					}

					if ( maxLeaderLength <= 0 )
					{
						SET_ERROR( error, ErrorCode_MaxLeaderLengthHasToBeGreaterThanZero,
							"Max leader length has to be > 0.");
						return;
					}

					if ( maxLeaderLength < minLeaderLength )
					{
						SET_ERROR( error, ErrorCode_MaxLeaderLengthHasToBeNotLesserThanMinLeaderLength,
							"Max leader length has to be >= Min leader length.");
						return;
					}

					m_minLeaderLength = minLeaderLength;
					m_maxLeaderLength = maxLeaderLength;
				}

				/**
				 * Get the minimum leader line length
				 * @reqid 001.0063
				 */
				float GetMinLeaderLength() const
				{
					return m_minLeaderLength;
				}

				/**
				 * Set the upright angle range
				 * @reqid 001.0063
				 */
				void SetUprightAngleLimit( float uprightAngleLimit )
				{
					m_uprightAngleLimit = uprightAngleLimit;
				}

				/**
				 * Get the upright angle range
				 * @reqid 001.0063
				 */
				float GetUprightAngleLimit() const
				{
					return m_uprightAngleLimit;
				}

				/**
				 * Get the maximum leader line length
				 * @reqid 001.0063
				 */
				float GetMaxLeaderLength() const
				{
					return m_maxLeaderLength;
				}

				/**
				* Set the thickness of the leader line. Specify the value in the units of the PlacementRule.
				* @param leaderWidth - width of the leader. Has to be > 0.
				* @param error - [out] error info.
				* @reqid 001.0063
				*/
				void SetLeaderWidth( float leaderWidth, Error& error )
				{
					if ( leaderWidth <= 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_LeaderWidthHasToBeGreaterThanZero );
						return;
					}
					m_leaderWidth = leaderWidth;
				}

				/**
				 * Get the thickness of the leader line.
				 * @reqid 001.0063
				 */
				float GetLeaderWidth() const
				{
					return m_leaderWidth;
				}

				/**
				* Gets AnchoringZoneAroundVerticesSize. This tells the leadering
				* algorithms how large (percentage of the segment length)
				* is the zone around vertices in which there should be no 
				* leader anchoring, when anchoring to the vertices is not allowed.
				* If not set it defaults to 0.2 in
				* @return AnchoringZoneAroundVerticesSize
				* @reqid 003.0030
				*/
				float GetAnchoringZoneAroundVerticesSize() const 
				{ 
					return m_noAnchoringZoneAroundVerticesSize; 
				}

				/**
				* Sets AnchoringZoneAroundVerticesSize.
				* @see GetAnchoringZoneAroundVerticesSize
				* @param val - Has to be >= 0.
				* @param error - Error info.
				* @reqid 003.0030
				*/
				void SetAnchoringZoneAroundVerticesSize(float val, Error& error) 
				{ 
					if ( val < 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_AnchoringZoneAroundVerticesSizeHasToBeNotLesserThanZero );
						return;
					}

					m_noAnchoringZoneAroundVerticesSize = val; 
				}

				/**
				* Get leader target mode option.
				* @return - leader target mode
				* @reqid 003.0030
				*/
				LeaderTargetMode GetLeaderTargetMode() const
				{
					return m_leaderTargetMode;
				}

				/**
				* Set leader target mode option.
				* @param value - new leader target mode
				* @param error - an Error object
				* Center is the only valid choice for non-symbolized point features
				* and is the recommended choice for area and symbolized point features.
				* FeaturePolyline is the only valid choice for line features.
				* @reqid 003.0030
				*/
				void SetLeaderTargetMode( LeaderTargetMode value, Error& error )
				{
					if ( value == LeaderTargetMode_NoLeader )
					{
						SET_ERROR_NOMSG( error, ErrorCode_LeaderTargetModeCannotBeNoLeader );
						return;
					}
					m_leaderTargetMode = value;
				}

				/**
				* Get leader anchor mode option.
				* @return - leader anchor mode
				* @reqid 003.0030
				*/
				LeaderAnchorMode GetLeaderAnchorMode() const
				{
					return m_leaderAnchorMode;
				}

				/**
				* Set leader anchor mode option.
				* @param value - new leader anchor mode
				* @param error - an Error object
				* Center is the only valid choice for non-symbolized point features.
				* FeaturePolyline is the only valid choice for line features
				* and is the recommended choice for symbolized point features.
				* Center, FeaturePolyline, or InsidePolyline can be used for area features.
				* @reqid 003.0030
				*/
				void SetLeaderAnchorMode( LeaderAnchorMode value, Error& error )
				{
					if ( value == LeaderAnchorMode_NoLeader )
					{
						SET_ERROR_NOMSG( error, ErrorCode_LeaderAnchorModeCannotBeNoLeader );
						return;
					}
					m_leaderAnchorMode = value;
				}

				/**
				* Get leader anchoring on feature side option. Binary combination 
				* of flags is allowed
				* @return - current leader anchoring on feature side options
				* @reqid 003.0030
				*/
				LeaderAnchorOnFeature GetLeaderAnchorOnFeature() const
				{
					return m_leaderAnchorOnFeature;
				}

				/**
				* Set leader anchoring option.
				* @param value - new leader anchoring on feature side options
				* @reqid 003.0030
				*/
				void SetLeaderAnchorOnFeature( LeaderAnchorOnFeature value )
				{
					m_leaderAnchorOnFeature = value;
				}

				/**
				 * Get the label justification to be applied to levels of stacked text.
				 * @return justification from the LabelJustification enumeration
				 * @reqid 001.0063
				 */
				LabelJustification GetLabelJustification() const
				{
					return m_labelJustification;
				}

				/**
				 * Set the label justification to be applied to levels of stacked text.
				 * @param val - LabelJustification
				 * @param error - an Error object
				 * @reqid 001.0063
				 */
				void SetLabelJustification( LabelJustification val, Error &error )
				{
					if ( val == LabelJustification_Unknown )
					{
						SET_ERROR( error, ErrorCode_IncorrectJustification,_T("SetLabelJustification does not accept LabelJustification_Unknown." ) );
						return;
					}

					m_labelJustification = val;
				}

				/**
				 * Set stack spacing as a percentage that has been converted to decimal.
				 * The spacing between levels of stacked text, bottom of one level to top of next.
				 * @param stackSpacing - percentage divided by 100.0. Has to be >= 0.
				 * @param error - [out] error info.
				 * @reqid 001.0063
				 */
				void SetStackSpacing( float stackSpacing, Error& error )
				{
					if ( stackSpacing < 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_StackSpacingHasToBeNotLesserThanZero );
						return;
					}
					m_stackSpacing = stackSpacing;
				}

				/**
				 * Get stack spacing.
				 * The spacing between levels of stacked text, bottom of one level to top of next
				 * @reqid 001.0063
				 */
				float GetStackSpacing() const
				{
					return m_stackSpacing;
				}

				/**
				 * Set bumping lock flag.
				 * Specifies whether labels placed using this rule can be bumped.
				 * @reqid 001.0063
				 */
				void SetBumpLockFlag( bool bumpLockFlag )
				{
					m_bumpLockFlag = bumpLockFlag;
				}

				/**
				 * Get bumping lock flag.
				 * Specifies whether labels placed using this rule can be bumped.
				 * @reqid 001.0063
				 */
				bool GetBumpLockFlag() const
				{
					return m_bumpLockFlag;
				}

				/**
				 * Get feature type
				 * @reqid 001.0063
				 */
				FeatureType GetFeatureType() const
				{
					return m_featureType;
				}

				/**
				 * Set feature type
				 * @reqid 001.0063
				 */
				void SetFeatureType( FeatureType featureType )
				{
					m_featureType = featureType;
				}

				/**
				 * Get the label direction relative to the feature direction.
				 * @return the label direction from the LabelDirection enumeration
				 * @reqid 003.0062
				 */
				LabelDirection GetLabelDirection() const
				{
					return m_labelDirection;
				}

				/**
				 * Set the label direction relative to the feature direction.
				 * @reqid 003.0062
				 */
				void SetLabelDirection(LabelDirection labelDirection, Error &error )
				{
					if (labelDirection==LabelDirection_Unknown)
					{
						SET_ERROR(error, ErrorCode_IncorrectLabelDirection,_T("SetLabelDirection does not accept LabelDirection_Unknown."));
						return;
					}

					m_labelDirection = labelDirection;
				}

				/**
				 * Set allow initial overhang flag.
				 * Determines if the initial position on a line feature can 
				 * overhang the feature.
				 * @reqid 003.0078
				 */
				void SetAllowInitialOverhang( bool allowInitialOverhang )
				{
					m_allowInitialOverhang = allowInitialOverhang;
				}

				/**
				 * Get allow initial overhang flag.
				 * Determines if the initial position on a line feature can 
				 * overhang the feature.
				 * @reqid 003.0078
				 */
				bool GetAllowInitialOverhang() const
				{
					return m_allowInitialOverhang;
				}

				/**
				 * Set the orientation of the label relative to the line feature.
				 * @reqid 001.0063
				 */
				void SetOrientation( enum OrientationType orientation )
				{
					m_orientation = orientation;
				}

				/**
				 * Get the orientation of the label relative to the line feature.
				 * @reqid 001.0063
				 */
				OrientationType GetOrientation() const
				{
					return m_orientation;
				}

				/**
				 * Set the minimum font size
				 * @param minFontSize	the minimum allowed font size
				 * @reqid 006.0088
				 */
				void SetMinFontSize( float minFontSize, Error& error )
				{
					if ( minFontSize < 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_MinFontSizeHasToBeNotLesserThanZero );
						return;
					}

					m_minFontSize = minFontSize;
				}

				/**
				 * Get the minimum font size
				 * @return the minimum allowed font size
				 * @reqid 006.0088
				 */
				float GetMinFontSize() const
				{
					return m_minFontSize;
				}

				/**
				 * Set the minimum font size unit type
				 * @param minFontSizeUnitType	the unit type of the minimum allowed font size
				 * @reqid 006.0088
				 */
				void SetMinFontSizeUnitType( UnitType minFontSizeUnitType )
				{
					m_minFontSizeUnitType = minFontSizeUnitType;
				}

				/**
				 * Get the minimum font size unit type
				 * @return the unit type of the minimum allowed font size
				 * @reqid 006.0088
				 */
				UnitType GetMinFontSizeUnitType() const
				{
					return m_minFontSizeUnitType;
				}

				/**
				 * Set the font size step
				 * @param fontSizeStep	the font size reduction step
				 * @reqid 006.0088
				 */
				void SetFontSizeStep( float fontSizeStep, Error& error )
				{
					if ( fontSizeStep < 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_FontSizeStepHasToBeNotLesserThanZero );
						return;
					}

					m_fontSizeStep = fontSizeStep;
				}

				/**
				 * Get the font size step
				 * @return the font size reduction step
				 * @reqid 006.0088
				 */
				float GetFontSizeStep() const
				{
					return m_fontSizeStep;
				}

				/**
				 * Set the font size step unit type
				 * @param fontSizeStepUnitType	the unit type of the font size reduction step
				 * @reqid 006.0088
				 */
				void SetFontSizeStepUnitType( UnitType fontSizeStepUnitType )
				{
					m_fontSizeStepUnitType = fontSizeStepUnitType;
				}

				/**
				 * Get the font size step unit type
				 * @return the unit type of the font size reduction step
				 * @reqid 006.0088
				 */
				UnitType GetFontSizeStepUnitType() const
				{
					return m_minFontSizeUnitType;
				}

				/**
				 * Get name of the rule
				 * @reqid 001.0063
				 */
				const SharedString* GetName() const
				{
					return &m_name;
				}

				/*******************************************************************
				There are no setters for the properties that constitute the key.
				They should not be needed and invoking them would not remove
				and re-insert the instance in the string keyed collection to
				reflect the new key.
				*******************************************************************/

				/**
				 * Gets the complete list of SequenceCommands to be executed by this placement rule.
				 * @return execution sequence
				 * @reqid 001.0063
				 */
				ExecutionSequence* GetExecutionSequence()
				{
					return m_executionSequence;
				}

				/**
				* Creates copy of self.
				* @param error an error object to be populated
				* @return copy of self
				* @reqid 001.0076
				*/
				PlacementRule* Clone( Error& error ) const;

				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @param error an error object to be populated
				* @reqid 001.0076
				*/
				static void Copy( const PlacementRule& src, 
					PlacementRule& dest, Error &error );

				/**
				* Gets the list of Invisible Feature Classes - those feature classes
				* will NOT be treated as obstacles when this placement rule is used.
				* @return - InvisibleFeatureClasses
				* @reqid 006.0011
				*/
				SharedString::Collection* GetInvisibleFeatureClasses()
				{ 
					return &m_invisibleFeatureClasses;
				}
            
                /**
                 * Set the flag for making all the area features invisible for labels placed with this placement rule.
                 * If set to true, area features will NOT be considered as obstacles during the text placement process.
                 * @param value - The value to set.
                 * @reqid 006.0011
                 */
                void SetAreaFeaturesInvisible( bool value )
                {
                    m_areaFeaturesInvisible = value;
                }
            
                /**
                 * Get the settings for area features invisibility. The default value is false.
                 * @see SetAreaFeaturesInvisible
                 * @return - The current value.
                 * @reqid 006.0011
                 */
                bool GetAreaFeaturesInvisible()
                {
                    return m_areaFeaturesInvisible;
                }
            
                /**
                 * Set the flag for making all the point features invisible for labels placed with this placement rule.
                 * If set to true, point features will NOT be considered as obstacles during the text placement process.
                 * @param value - The value to set.
                 * @reqid 006.0011
                 */
                void SetPointFeaturesInvisible( bool value )
                {
                    m_pointFeaturesInvisible = value;
                }
            
                /**
                 * Get the settings for point features invisibility. The default value is false.
                 * @see SetPointFeaturesInvisible
                 * @return - The current value.
                 * @reqid 006.0011
                 */
                bool GetPointFeaturesInvisible()
                {
                    return m_pointFeaturesInvisible;
                }
            
                /**
                 * Set the flag for making all the line features invisible for labels placed with this placement rule.
                 * If set to true, line features will NOT be considered as obstacles during the text placement process.
                 * @param value - The value to set.
                 * @reqid 006.0011
                 */
                void SetLineFeaturesInvisible( bool value )
                {
                    m_lineFeaturesInvisible = value;
                }
            
                /**
                 * Get the settings for line features invisibility. The default value is false.
                 * @see SetLineFeaturesInvisible
                 * @return - The current value.
                 * @reqid 006.0011
                 */
                bool GetLineFeaturesInvisible()
                {
                    return m_lineFeaturesInvisible;
                }
            
				/**
				 * Returns the units in which the properties are stored.
				 * @return - units of dimensioned properties
				 * @reqid 001.0063
				 */
				Unit GetUnits()
				{
					return m_units;
				}

				/**
				 * Validate the placement rule. If the rule deriving from this class needs to implement 
				 * additional validation it should overload this method but always invoke the parent
				 * implementation from within.
				 * @param featType - feature type to proceed with some feature-type-dependent validations
				 * @param valid[out] - True if the rule is valid. False otherwise, details appended
				 *                through validationErrors.
				 * @param validationErrors[out] - Validation issues are reported here.
				 * @param error[out] - Other critical errors which may occur during 
				 *                the validation process are reported here. They would NOT
				 *                be caused by invalid input data.
				 * @reqid 001.0086
				 */
				void Validate( FeatureType featType, bool* valid, ErrorDetails** validationErrors, Error& error );
			
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
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static PlacementRule* DeserializeNew( IDeserializer& deserializer, Error& error );

				/**
				 * Deserializes a key collection from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				void Deserialize( IDeserializer& deserializer, Error& error );

#endif

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!

				ALSO, ADD YOUR NEW PROPERTY TO THE ConvertUnitsTo METHOD!!
				****************************************************************************************/
			private:

				/**
				 * Converts the dimensioned properties from their existing units into the input units.
				 * @param units - new units to change the properties to.
				 * @reqid 001.0063
				 */
				void ConvertUnitsTo(Unit units);

				Unit m_units;

				SharedString m_name;
				FeatureType m_featureType;
				ExecutionSequence *m_executionSequence;

				//General Placement Rule properties
				BufferRule m_textToTextBuffer;
				BufferRule m_textToLineBuffer;
				BufferRule m_textToPointBuffer;
				float m_minOffset;
				float m_maxOffset;
				SharedString m_allowOverlineList;
				bool m_shouldAddGraticuleAngleToPlacementAngle;

				/**
				* Comma delimited list of the names of polygon feature classes whose interiors cannot be overlapped
				* by labels following this placement rule.
				* @reqid 001.0063
				*/
				SharedString m_exclusionPolygons;

				SharedString m_allowOverregionList;

				//Leader Properties
				int m_maxLeaderCrossings;
				float m_leaderWidth;
				float m_minLeaderLength;
				float m_maxLeaderLength;
				float m_noAnchoringZoneAroundVerticesSize;
				int m_arrowStyle;
				int m_leaderColor;
				AnchorLeaderWhere m_anchorLeaderWhere;
				LeaderAnchorOnFeature m_leaderAnchorOnFeature;
				LeaderTargetMode m_leaderTargetMode;
				LeaderAnchorMode m_leaderAnchorMode;

				//Stacking Properties
				float m_stackSpacing;
				LabelJustification m_labelJustification;
				SharedString::Collection m_allowLeaderCrossList;
				bool m_bumpLockFlag;
				SharedString m_anchorLabelPartName;
				
				LabelDirection m_labelDirection;
				float m_uprightAngleLimit;

				SharedString::Collection m_invisibleFeatureClasses;
            
                bool m_areaFeaturesInvisible;
                bool m_pointFeaturesInvisible;
                bool m_lineFeaturesInvisible;

				//Line placement properties
				OrientationType m_orientation;
				bool m_allowInitialOverhang;

				// Font reduction properties
				float m_minFontSize;
				float m_fontSizeStep;
				UnitType m_minFontSizeUnitType;
				UnitType m_fontSizeStepUnitType;

				float m_maxLabelLength;
		};

//		/**
//		 * Line feature specific placement rule describing the placement pipeline
//		 * for a label class.
//		 * @reqid 001.0063
//		 */
//		class LABELINGENGINE_API LinePlacementRule
//			: public PlacementRule
//		{
//			friend class PlacementRule::Collection;
//			public:
//				/**
//				* Get the type of derived placement rule
//				* @return the type
//				* @reqid 001.0063
//				*/
//				PlacementRuleType GetPlacementRuleType() const
//				{
//					return PlacementRuleType_Line;
//				}
//
//				/**
//				* @see PlacementRule::Clone
//				* @reqid 001.0076
//				*/
//				virtual LinePlacementRule* Clone( Error& error ) const;
//
//				/**
//				* Make deep copy of one instance's members to another
//				* @param src source object
//				* @param dest destination object
//				* @param error an error object to be populated
//				* @reqid 001.0076
//				*/
//				static void Copy( const LinePlacementRule& src, LinePlacementRule& dest, Error &error );
//
//
//		protected:
//				/**
//				 * Default constructor.
//				 * @param error		an error object to be populated
//				 * @reqid 001.0063
//				 */
//				LinePlacementRule(Error& error);
//
//				/**
//				 * Constructor.
//				 * @param name the name of the rule
//				 * @param error		an error object to be populated
//				 * @reqid 001.0063
//				 */
//				LinePlacementRule( const TCHAR*name, Error& error );
//
//				/**
//				 * Constructor
//				 * @param name					the name to identify the rule
//				 * @param featureType		type of the feature this rule applies to
//				 * @param error					an error object to be populated
//				 * @reqid 001.0063
//				 */
//				LinePlacementRule( const TCHAR* name, FeatureType featureType, Error& error );
//public:
//
//				/****************************************************************************************
//				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
//				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
//				****************************************************************************************/
//			private:
//		};
//
		/**
		 * Area feature sprcific placement rule describing the placement pipeline
		 * for a label class.
		 * @reqid 001.0063
		 */
		//class LABELINGENGINE_API AreaPlacementRule
		//	: public LinePlacementRule
		//{
		//	friend class PlacementRule::Collection;

		//	public:

		//		/**
		//		* Get the type of derived placement rule
		//		* @return the type
		//		* @reqid 001.0063
		//		*/
		//		PlacementRuleType GetPlacementRuleType() const
		//		{
		//			return PlacementRuleType_Area;
		//		}

		//		/**
		//		* @see PlacementRule::Clone
		//		* @reqid 001.0076
		//		*/
		//		virtual AreaPlacementRule* Clone( Error& error ) const;

		//		/**
		//		* Make deep copy of one instance's members to another
		//		* @param src source object
		//		* @param dest destination object
		//		* @param error an error object to be populated
		//		* @reqid 001.0076
		//		*/
		//		static void Copy( const AreaPlacementRule& src, AreaPlacementRule& dest, Error &error );

		//	protected:
		//		/**
		//		 * Default constructor.
		//		 * @param error		an error object to be populated
		//		 * @reqid 001.0063
		//		 */
		//		AreaPlacementRule(Error& error)
		//			: LinePlacementRule(error)
		//		{
		//			if (error)
		//			{
		//				return;
		//			}
		//			SetFeatureType( FeatureType_Area );
		//			SetLeaderTargetMode( LeaderTargetMode_Center );
		//			SetLeaderAnchorMode( LeaderAnchorMode_FeaturePolyline );
		//		}

		//		/**
		//		 * Constructor.
		//		 * @param name the name of the rule
		//		 * @param error		an error object to be populated
		//		 * @reqid 001.0063
		//		 */
		//		AreaPlacementRule( const TCHAR* name, Error& error )
		//			: LinePlacementRule ( name, FeatureType_Area, error)
		//		{
		//			SetLeaderTargetMode( LeaderTargetMode_Center );
		//			SetLeaderAnchorMode( LeaderAnchorMode_FeaturePolyline );
		//		}

		//		/****************************************************************************************
		//		THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
		//		TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
		//		****************************************************************************************/
		//	private:
		//};


		/**
		 * Point feature specific placement rule describing the placement pipeline
		 * for a label class.
		 * @reqid 001.0063
		 */
		//class LABELINGENGINE_API PointPlacementRule
		//	: public PlacementRule
		//{
		//	friend class PlacementRule::Collection;

		//	public:

		//		/**
		//		* Get the type of derived placement rule
		//		* @return the type
		//		* @reqid 001.0063
		//		*/
		//		PlacementRuleType GetPlacementRuleType() const
		//		{
		//			return PlacementRuleType_Point;
		//		}

		//		/**
		//		* @see PlacementRule::Clone
		//		* @reqid 001.0076
		//		*/
		//		virtual PointPlacementRule* Clone( Error& error ) const
		//		{
		//			PointPlacementRule* copy = new PointPlacementRule( error );
		//			if ( copy == NULL)
		//			{
		//				SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
		//			}
		//			else
		//			{
		//				if ( !error )
		//				{
		//					Copy( *this, *copy, error );
		//				}
		//				if (error) //ctor or Copy failed
		//				{
		//					delete copy;
		//					copy = NULL;
		//				}
		//			}
		//			return copy;					
		//		}

		//		/**
		//		* Make deep copy of one instance's members to another
		//		* @param src source object
		//		* @param dest destination object
		//		* @param error an error object to be populated
		//		* @reqid 001.0076
		//		*/
		//		static void Copy( const PointPlacementRule& src, PointPlacementRule& dest, Error &error )
		//		{
		//			PlacementRule::Copy( src, dest, error );
		//			if ( error )
		//			{
		//				return; //pass error to a client
		//			}
		//		}

		//	protected:
		//		/**
		//		 * Default constructor.
		//		 * @param error		an error object to be populated
		//		 * @reqid 001.0063
		//		 */
		//		PointPlacementRule(Error& error)
		//			: PlacementRule(error)
		//		{
		//			if (error)
		//			{
		//				return;
		//			}
		//			SetFeatureType( FeatureType_Point );
		//			SetLeaderTargetMode( LeaderTargetMode_Center );
		//			SetLeaderAnchorMode( LeaderAnchorMode_FeaturePolyline );
		//		}

		//		/**
		//		 * Constructor.
		//		 * @param name the name of the rule
		//		 * @param error		an error object to be populated
		//		 * @reqid 001.0063
		//		 */
		//		PointPlacementRule( const TCHAR* name, Error& error )
		//			: PlacementRule ( name, FeatureType_Point, error )
		//		{
		//			SetLeaderTargetMode( LeaderTargetMode_Center );
		//			SetLeaderAnchorMode( LeaderAnchorMode_FeaturePolyline );
		//		}

		//		/****************************************************************************************
		//		THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
		//		TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
		//		****************************************************************************************/
		//	private:
		//};
	} // namespace Dtp
} // namespace MapText

#endif // DTP_PLACEMENTRULE_H
