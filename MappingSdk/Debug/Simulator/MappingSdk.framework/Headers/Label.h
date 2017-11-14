/**************************************************
 * Copyright (c)2008 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://martin@mtsrv.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/LabelingEngine/Label.h $
 *
 * Revision: $Revision: 81751 $
 *
 * Last modified on: $Date: 2016-03-17 17:18:55 -0400 (Thu, 17 Mar 2016) $, by: $Author: mark $
 *
 *************************************************/

#ifndef DTP_LABEL_H
#define DTP_LABEL_H

#include "LabelingEngineDefs.h"
#include "PlacementRule.h"
#include "MtGeometry.h"
#include "LabelElement.h"

#include "Collection.h"
#include "MtString.h"
#include "SharedObjects.h"
#include "Graph.h"
#include <float.h>

#include "Serialization.h"


//Turn on DISPLAY_DEBUGGER in all the configurations.
//If you plan to enable it selectively using command-line compiler flags, then ensures 
//that all compilation units which include Label.h have the flag defined in exactly same way.
//Failure to do it might lead to crashes which are difficult to track.
#ifndef DISPLAY_DEBUGGER
#define DISPLAY_DEBUGGER
#endif

namespace MapText
{
	namespace Dtp
	{
		/**
		 * Forward declare pointer-only dependencies
		 */
		class IObstacleAnalyzer;
		class LabelTextAnalysis;
		class LabelTextAnalysisMgr;
		class Leader;
		class Point;
		class Map;
		class ObstacleOverlapSummary;
		class ISpatialIndex;
		class LabelAnchor;
		class AssemblyLineLabelAnchor;
		class RegularLineLabelAnchor;
		class LeaderAnchorDefinition;
		class LeaderAnchorDefinitions;
		class AssemblyPointLabelAnchor;
		class RegularPointLabelAnchor;
		class AssemblyLabelBuilder;
		class ComponentActivator;
		class IUserData;
		class PlacementState;

		/**
		 * Class that holds a label style
		 * @reqid 001.0063
		 *
		 */
		class LABELINGENGINE_API TextStyle
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_FontSizeHasToBeGreaterThanZero,
					ErrorCode_FontNameIsZeroLength,
					ErrorCode_SpecifiedFontIsNotInstalled,
#ifdef S11N
					ErrorCode_DeserializationError,
#endif
				};

				/** Class that holds a collection of TextStyle object pointers.
				 * @reqid 001.0063
				 */
				class LABELINGENGINE_API Collection : public StringKeyedTree < TextStyle >
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
						Collection()
						{
						}

						/**
						 *	Retrieve specified text style from cache if present; else create and cache.
						 *	@param	fontName			font name
						 *	@param	fontSize			font size in points
						 *	@param	color					color formatted as a Windows RGB long value
						 *	@param	textStyle			[out] the requested text style
						 *	@param	error					an Error object
						 * @reqid 001.0063
						 */
						void GetTextStyle( const TCHAR* fontName,float fontSize,long color,const TextStyle** textStyle, Error& error,float haloSize=0,long haloColor = -1, const TCHAR* fontPath = NULL);
				};

			private:

                /**
                 * Constructor.
                 * Only one constructor that works in concert with
                 * TextStyles::GetTextStyle.
                 * @param	fontName			font name
                 * @param	fontSize			size for label style in inches
                 * @param	color					color for label style, formatted as a Windows RGB int value
                 * @param error					an Error object
                 * @reqid 001.0063
                 */
                TextStyle( const TCHAR* fontName, const TCHAR* fontFilePath, float fontSize, long color, float haloSize, long haloColor, Error& error );
                
				/**
				 * Constructor.
				 * Only one constructor that works in concert with
				 * TextStyles::GetTextStyle.
				 * @param	fontName			font name
				 * @param	fontSize			size for label style in inches
				 * @param	color					color for label style, formatted as a Windows RGB int value
				 * @param error					an Error object
				 * @reqid 001.0063
				 */
				TextStyle( const TCHAR* fontName, float fontSize, long color, float haloSize, long haloColor, Error& error, const TCHAR* fontResourcePath = NULL );

                void InitializeTextStyle(float fontSize, long color, float haloSize, long haloColor, Error& error);

			public:

				/** Destructor
				*	@reqid 001.0063
				*/
				~TextStyle()
				{
				}

				/*******************************************************************
				There are no setters for the properties that constitute the key.
				They should not be needed and invoking them would not remove
				and re-insert the instance in the string keyed collection to
				reflect the new key.
				*******************************************************************/

				/**
				 * Get the font name
				 * @return the font name as a pointer to a SharedString instance.
				 * @reqid 001.0063
				 */
				const SharedString* GetFontName() const
				{
					return &m_fontname;
				}

                /**
                 * Get the font file path
                 * @return the font file path as a pointer to a SharedString instance.
                 * @reqid 001.0063
                 */
                const SharedString* GetFontFilePath() const
                {
                    return &m_fontFilePath;
                }

                /**
                 * Get the font file folder path
                 * @return the font file folder path as a pointer to a SharedString instance.
                 * @reqid 001.0063
                 */
                const SharedString* GetFontFileFolderPath() const
                {
                    return &m_fileFolderPath;
                }
            
                /**
                 * Set the font path
                 * @reqid 001.0063
                 */
                void SetFontPath(const char* fontPath, Error& error)
                {
                    m_fileFolderPath.AssignString(fontPath, error);
                    if(error)
                    {
                        return;
                    }
                    
                    String filePath(fontPath, error);
                    if(error)
                    {
                        return;
                    }
                    
                    String fontName(m_fontname.GetString(), error);
                    if(error)
                    {
                        return;
                    }
                    fontName.Lcase();

                    filePath.ConcatString("/", error);
                    filePath.ConcatString(fontName.GetString(), error);
                    filePath.ConcatString(".ttf", error);
                    if(error)
                    {
                        return;
                    }
                    
                    m_fontFilePath.AssignString(filePath.GetString(), error);
                }
                        
				/**
				 * Get the font size.
				 * @return the font size in inches
				 * @reqid 001.0063
				 */
				float GetFontSize() const
				{
					return m_fontsize;
				}

				/**
				 * Get the halo size.
				 * @return the halo size in inches
				 * @reqid 001.0063
				 */
				float GetHaloSize() const
				{
					return m_haloSize;
				}

                /**
                 * Get the halo color.
                 * @return the halo color
                 */
                int GetHaloColor() const
                {
                    return m_haloColor;
                }
            
				/**
				 * Get the font color
				 * @return the font color, formatted as a Windows RGB int value
				 * @reqid 001.0063
				 */
				int GetColor() const
				{
					return m_color;
				}

				/**
				* Creates copy of self.
				* @param error an error object to be populated
				* @return copy of self
				* @reqid 001.0076
				*/
				virtual TextStyle* Clone( Error& error ) const
				{
					TextStyle* copy = new TextStyle( m_fontname.GetString(), m_fileFolderPath.GetString(), m_fontsize, m_color, m_haloSize, m_haloColor, error );
					if ( copy == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
						return copy;
					}
					if ( error )
					{
						delete copy;
						copy = NULL;
					}
					return copy;
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
				static TextStyle* DeserializeNew( IDeserializer& deserializer, Error& error );
#endif

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				SharedString m_fontname;
                SharedString m_fileFolderPath;
                SharedString m_fontFilePath;
				float m_fontsize;
				int m_color; //For now, just putting an RGB long color here
				float m_haloSize;
                int m_haloColor;
		};

		/**
		 * Class that holds a Label object.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Label
		{
			public:
				/**
				 * High level categorization of labels.
				 * @reqid 001.0063
				 */
				enum LabelType
				{
					LabelType_Unknown = -1,
					LabelType_Regular,
					LabelType_Assembly,
				};

				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_UninitializedLabel,
					ErrorCode_NotImplemented
				};

				/**
				 * Possible directions in which label can shift to avoid obstacles.
				 */
				enum ShiftDirection
				{
					ShiftDirection_Up,
					ShiftDirection_Down,
					ShiftDirection_Right,
					ShiftDirection_Left
				};

				/**
				*	Enumerates the possible directions a label can follow
				*	@reqid 001.0063
				*/
				enum LabelDirection
				{
					LabelDirection_FollowsGeometryDirection,
					LabelDirection_FollowsReversedGeometryDirection
				};

				/**
				 *	Destructor.
				 * @reqid 001.0063
				 */
				virtual ~Label();

				/** Get the label type
				 *	@return the label's LabelType
				 *	@reqid 001.0063
				 */
				LabelType GetLabelType() const
				{
					return m_labelType;
				}

				/** Set the label type
				 *	@param labelType	the LabelType of the label
				 *	@reqid 001.0063
				 */
				void SetLabelType( LabelType labelType )
				{
					m_labelType = labelType;
				}

				/** Get the label's leader if present.
				*	@return the address of the label's leader if present; NULL if not
				*	@reqid 001.0063
				*/
				Leader* GetLeader() const
				{
					return m_leader;
				}

				/** Set the label's leader.
				*	@param leader		the address of the leader to assign if present; NULL if none
				*	@reqid 001.0063
				*/
				void SetLeader( Leader* leader );

				/** Get the LabelTextAnalysis manager.
				 *  A container of the LabelTextAnalysis instances created and used
				 *	in the course of attempting to place this label.
				 *	@return LabelTextAnalysisMgr
				 *	@reqid 001.0063
				 */
				LabelTextAnalysisMgr* GetTextAnalysisMgr() const;

				/** Get the current justification of this label.
				 *	@return the current justification
				 *	@reqid 001.0063
				 */
				PlacementRule::LabelJustification GetCurrentJustification() const
				{
					return m_currentJustification;
				}

				/**
				* Unhide all elements of the label.
				* @reqid 006.0014
				*/
				virtual void Unhide() = 0;

				/**
				 * Create a deep copy of the object (feature and label class are NOT owned,
				 * therefore only a shallow copy is made).
				 * @param error error information
				 * @param parentMap (optional) a new parent map for a cloned object
				 * @return the copy
				 * @reqid 001.0063
				 */
				virtual Label* Clone( Error& error, Map* parentMap = NULL ) const = 0;

				/**
				 * Moves the label's parts by (dx,dy)
				 * @param dx - amount to move horizontally
				 * @param dy - amount to move vertically
				 * @reqid 001.0072
				 */
				virtual void Translate(float dx, float dy) = 0;

				/**
				* Move the label's lowerleft to location
				* @param location	Location to which the label is to be moved
				* @reqid 003.0019
				*/
				void MoveToLocation(const Point &location);

				/**
				* Rotate the label's parts to absolute angle
				* @param anchorPoint	anchor point about which to rotate
				* @param angle - absolute angle to rotate to
				* @reqid 003.0019
				*/
				virtual void RotateToAngle(const Point &anchorPoint, float angle) = 0;

				/**
				* Rotate self about the specified origin.
				* @param anchorPoint - point about which to rotate
				* @param angleDiff - rotation angle, needed to maintain internal angle
				*                    without expensive calculations
				* @param sin_t - sine of angle by which to rotate
				* @param cos_t - cosine of angle by which to rotate
				* @reqid 001.0063
				*/
				virtual void Rotate( const Point &anchorPoint, float angleDiff, float sin_t, float cos_t ) = 0;

				/**
				* Rotate the label back to horizontal
				* @reqid 003.0019
				*/
				virtual void MakeHorizontal();

				/**
				 * Determine the length of the bounding box that encloses the label.
				 * @return - bounding box length in inches
				 * @reqid 001.0072
				 */
				float GetBoundingBoxLength() const
				{
					return m_boundingBoxLength;
				}

				/**
				 * @see GetBoundingBoxLength
				 * @reqid 001.0072
				 */
				void SetBoundingBoxLength(float boundingBoxLength) 
				{
					m_boundingBoxLength=boundingBoxLength;
				}

				/**
				 * Determine the height of the bounding box that encloses the label.
				 * @return - bounding box height in inches
				 * @reqid 001.0072
				 */
				float GetBoundingBoxHeight() const
				{
					return m_boundingBoxHeight;
				}

				/**
				 * @see GetBoundingBoxHeight
				 * @return - bounding box height
				 * @reqid 001.0072
				 */
				void SetBoundingBoxHeight(float boundingBoxHeight)
				{
					m_boundingBoxHeight=boundingBoxHeight;
				}

				/** Get the lower left corner of the label bounding box.
				*	@return the bounding box lower left corner
				*	@reqid 001.0063
				*/
				const Point* GetLowerLeft() const
				{
					return &m_lowerLeft;
				}

				/**
				 * Returns the lowerLeft of the label
				 * @param lowerLeft - [out] filled with coordinates of lower left.
				 * @reqid 001.0063
				 */
				void GetLowerLeft(Point *lowerLeft) const;

				/**
				 * Returns the upperLeft of the label.
				 * @param upperLeft - [out] filled with coordinates of upper left.
				 * @reqid 001.0063
				 */
				void GetUpperLeft(Point *upperLeft) const;

				/**
				 * Returns the lowerRight of the label
				 * @param lowerRight - [out] filled with coordinates of lower right.
				 * @reqid 001.0063
				 */
				void GetLowerRight(Point *lowerRight) const;

				/**
				 * Gets the vector along the horizontal axis of the rectangle
				 * @param baselineVector - [out] filled with vector
				 * @reqid 001.0063
				 */
				void GetBaselineVector(Vector *baselineVector) const;

				/**
				* Populate a Rectangle object with the bounding box coordinates.
				* @param boundingBox	The Rectangle object to be populated
				* @reqid 003.0019
				*/
				void BuildBoundingBox( Rectangle& boundingBox ) const;

				/**
				 * Create a copy of all of the label text boxes and put it in the
				 * supplied collection.
				 * @param boxes [out] collection of text boxes. Ownership of the boxes
				 *                    has to be assumed by the client.
				 * @param error [out] error info
				 * @reqid 004.0032
				 */
				virtual void GetTextBoxes( LinkedList<Rectangle> &boxes, Error& error ) const = 0;

				/**
				 * Positions the label such that the lower left corner of the bounding box that encloses
				   the entire label sits on the origin.
				 * @param stackSpacing - fraction of the fontsize that is to be the space between multiple levels of a label
				 * @param justification - specifies how to align multiple lines of a label with one another.
				 * @param error - usual error object
				 * @reqid 001.0072
				 */
				virtual void PlaceAtOrigin(float stackSpacing,PlacementRule::LabelJustification justification,Error& error) = 0;

				/**
				* Retrieve the angle of the label.
				* This is meaningful for straight labels only.
				* @return the angle of the first text part in radians
				* @reqid 001.0063
				*/
				virtual float GetAngle() const = 0;

				/**
				 * Checks each part of the label for overlap. The search stops at the first part that shows
				   overlaps. The overlap summary for this part is returned to the client.
			   * @param map container of obstacles against which the label is checked for overlaps.
				 * @param feature feature that is currently being labeled.
				 * @param placementRule stores the rules that control placement.
				 * @param overlapSummary [out] method stores here NULL if there are no overlaps or the 
				 *                       overlap summary having overlaps.
				 * @param error an Error object
				 * @reqid 001.0065
				 */
				virtual void CheckOverlaps(
					Map *map, Feature *feature, PlacementRule *placementRule, PlacementState* placementState,
					ObstacleOverlapSummary **overlapSummary,Error &error) = 0;

				/**
				* Checks each part of the label for overlap. The overlap summary for this part is returned to 
				* the client. Use this version if some custom client behavior is needed from the obstacle 
				* detector and provide an IObstacleDetector implementation.
				* @param map container of obstacles against which the label is checked for overlaps.
			  * @param feature feature that is currently being labeled.
				* @param placementRule stores the rules that control placement.
				* @param obstacleAnalyzer Inject client specific behavior to obstacle detection
				* @param analyzeAllObjects If false the search stops at the first part that shows
				*                          overlaps. If true a complete overlap analysis is performed.
				* @param overlapSummary [out] method stores here NULL if there are no overlaps or the overlap summary having overlaps.
				* @param error and Error object
				* @reqid 001.0065
				*/
				virtual void CheckOverlaps(
					Map *map, Feature *feature, PlacementRule *placementRule, PlacementState* placementState,
					IObstacleAnalyzer* obstacleAnalyzer, bool analyzeAllObjects,ObstacleOverlapSummary **overlapSummary, Error &error) = 0;

				/* 
				 * Adds the label parts to the Spatial Index so that the Obstacle Detector detects overlaps with
				 * text or leader lines.
				 * @param spatialIndex - index to which to add the label parts.
				 * @error - error object
				 * @reqid 001.0064
				 */
				virtual void RegisterWithSpatialIndex(ISpatialIndex *spatialIndex, Error &error) = 0;

				/* 
				 * Searches for the set of label parts such that each part's bounding box's maximum y value is
				 * the same as the entire label's bounding box's maximum y value. Once this set is determined,
				 * the parts in the set are traversed to find the part having a left side with the smallest 
				 * x-value and to find the part having a right side with the largest x-value. The smallset x-value
				 * is stored in labelLeftX and the largest x-value is stored in labelRightX.
				 * @param labelleftX - x value of left side of range that holds all label parts satisfying the condition described above
				 * @param labelRightX - x value of right side of range that holds all label parts satisfying the condition described above
				 * @reqid 002.0036
				 */
				virtual void GetTopXRange(float &labelLeftX,float &labelRightX) const = 0;

				/* 
				 * Searches for the set of label parts such that each part's bounding box's minimum y value is
				 * the same as the entire label's bounding box's minimum y value. Once this set is determined,
				 * the parts in the set are traversed to find the part having a left side with the smallest 
				 * x-value and to find the part having a right side with the largest x-value. The smallset x-value
				 * is stored in labelLeftX and the largest x-value is stored in labelRightX.
				 * @param labelleftX - x value of left side of range that holds all label parts satisfying the condition described above
				 * @param labelRightX - x value of right side of range that holds all label parts satisfying the condition described above
				 * @reqid 002.0036
				 */
				virtual void GetBottomXRange(float &labelLeftX,float &labelRightX) const = 0;

				/* 
				 * Searches for the set of label parts such that each part's bounding box's maximum x value is
				 * the same as the entire label's bounding box's maximum x value. Once this set is determined,
				 * the parts in the set are traversed to find the part having a bottom side with the smallest 
				 * y-value and to find the part having a top side with the largest y-value. The smallset y-value
				 * is stored in labelBottomY and the largest y-value is stored in labelTopY.
				 * @param labelTopY - y value of top side of range that holds all label parts satisfying the condition described above
				 * @param labelBottomY - y value of bottom side of range that holds all label parts satisfying the condition described above
				 * @reqid 003.0029
				 */
				virtual void GetRightYRange(float &labelBottomY,float &labelTopY) const = 0;

				/* 
				 * Searches for the set of label parts such that each part's bounding box's minimum x value is
				 * the same as the entire label's bounding box's minimum x value. Once this set is determined,
				 * the parts in the set are traversed to find the part having a bottom side with the smallest 
				 * y-value and to find the part having a top side with the largest y-value. The smallest y-value
				 * is stored in labelBottomY and the largest y-value is stored in labelTopY.
				 * @param labelTopY - y value of top side of range that holds all label parts satisfying the condition described above
				 * @param labelBottomY - y value of bottom side of range that holds all label parts satisfying the condition described above
				 * @reqid 003.0029
				 */
				virtual void GetLeftYRange(float &labelBottomY,float &labelTopY) const = 0;

				/* 
				 * Justify the label according to the justification parameter
				 * @param justification - justification to perform
				 * @reqid 006.0046
				 */
				virtual void Justify(PlacementRule::LabelJustification justification) = 0;

				/**
				 * Report whether every text object of this label is hidden.
				 * @return true if hidden; false if shown
				 * @reqid 006.0014
				 */
				virtual bool IsHidden() const = 0;

				/* 
				* Justify the label according to its position relative to the supplied polyline
				* @param label - label to justify
				* @param featurePolyline - the feature polyline to which the label is to be justified
				* @param bApply - apply (do not just return) the appropriate justification
				* @return how label was justified - LEFT, RIGHT, or CENTER
				* @reqid 006.0046
				*/
				PlacementRule::LabelJustification JustifyToFeature( const Polyline& featurePolyline, bool bApply=true );

				/* 
				 * Computes the shortest horizontal perpendicular distance from the input point to the label's 
				   bounding box.
				 * @param point - distance computed relative to this location.
				 * @return - computed distance
				 * @reqid 003.0025
				 */
				float HorizontalDistanceTo(const Point *point) const;

				/* 
				 * Computes the shortest vertical perpendicular distance from the input point to the label's 
				   bounding box. Label is expected to be horizontal
				 * @param point - distance computed relative to this location.
				 * @return - computed distance
				 * @reqid 003.0025
				 */
				float VerticalDistanceTo(const Point *point) const;

				/* 
				 * Gets the y coordinate of the bottom of the bounding box (independently of whether the label is upright
				   or upside down). Label expected to be horizontal.
				 * @return - y coordinate of bounding box's bottom side.
				 * @reqid 003.0025
				 */
				float GetBoundingBoxBottomY() const;

				/* 
				 * Gets the y coordinate of the top of the bounding box (independently of whether the label is upright
				   or upside down). Label expected to be horizontal.
				 * @return - y coordinate of bounding box's top side.
				 * @reqid 003.0025
				 */
				float GetBoundingBoxTopY() const;

				/* 
				 * Gets the x coordinate of the left side of the bounding box (independently of whether the label is upright
				   or upside down). Label expected to be horizontal.
				 * @return - y coordinate of bounding box's left side.
				 * @reqid 003.0025
				 */
				float GetBoundingBoxLeftX() const;

				/* 
				 * Gets the x coordinate of the right side of the bounding box (independently of whether the label is upright
				   or upside down). Label expected to be horizontal.
				 * @return - x coordinate of bounding box's right side.
				 * @reqid 003.0025
				 */
				float GetBoundingBoxRightX() const;

				/* 
				 * Indicate whether the label is horizontal or not.
				 * @return - true if label is 0deg or 180deg, false if not.
				 * @reqid 003.0025
				 */
				bool IsHorizontal() const;

				/* 
				 * Determine if the label is upside down.
				 * @return - true if label is upside down, false if label is not upside down
				 * @reqid 001.0063
				 */
				bool IsUpsideDown() const;

				/**
				 * Validate the label.
				 * @param validationErrors - [out] Validation issues are reported here.
				 * @param error - [out] Other critical errors which may occur during 
				 *                the validation process are reported here. They would NOT
				 *                be caused by invalid input data.
				 * @reqid 001.0086
				 */
				virtual void Validate( bool* valid, ErrorDetails** validationErrors, Error& error ) const = 0;

				/* 
				 * Resets the lower left. Use with caution Label's cache a lot of data that is based on this value.
				 * @reqid 001.0063
				 */
				void ResetLowerLeft()
				{
					m_lowerLeft.SetXY(0,0);
				}

				/**
				 * Compare if two labels are 'equal', i.e. visually resulting in the same
				 * text being created.
				 * @param otherLabel - The label to compare this instance to.
				 * @return - True if labels are identical, false otherwise.
				 * @reqid 001.0063
				 */
				virtual bool IsEqualTo( const Label& otherLabel ) const = 0;

				/**
				 * Compose the full text of the label.
				 * @param labelText	[out] A reference to a string to be populated with the text
				 * @param error [out] error information
				 * @reqid 001.0063
				 */
				virtual void GetFullText( String& fullText, Error& error ) const = 0;

                /**
                 * Original scale at which this labe was generated which will be used for
                 * scale dependent rendering
                 * @param val reference scale
                 */
                void SetReferenceScale(float val)
                {
                    m_referenceScale = val;
                }
                
                float GetReferenceScale()
                {
                    return m_referenceScale;
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
				static Label* DeserializeNew( IDeserializer& deserializer, Error& error );

				/**
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				void Deserialize( IDeserializer& deserializer, Error& error );

			protected:
				/** 
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 */
				virtual void _Deserialize( IDeserializer& deserializer, Error& error ) = 0;

				/**
				 * Serialization of the child class.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				virtual void _Serialize( ISerializer& emitter, Error& error ) const = 0;
#endif

			protected:
				/**
				* Make deep copy of one instance's members to another
				* @param src
				* @param dest
				* @param error	an Error object
				* @reqid 001.0063
				*/
				static void Copy( const Label& src, Label& dest, Error &error );

				/**
				* Constructor. Members initialized to default values..
				* @param error		an error object to be populated
				* @reqid 001.0063
				*/
				Label(Error& error);

				/**
				* Constructor. Initializes members to incoming values.
				* @param	labelType		specifies the the label type
				* @param error				an error object to be populated
				* @reqid 001.0063
				*/
				Label( LabelType labelType, Error& error );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				LabelType m_labelType;

			protected:
				Leader* m_leader;
				LabelTextAnalysisMgr* m_textAnalysisMgr;

				float m_boundingBoxLength,m_boundingBoxHeight;
				Point m_lowerLeft;

				PlacementRule::LabelJustification m_currentJustification;

                float m_referenceScale;
		};

		/**
		 * Class that defines a uniquely oriented block of text that is part of a placed label's positioned text.
		 * This may be a single character in curved or character-spread text.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API TextPart
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_NotImplemented
				};

				/**
				 * Create a new TextPart
				 * @param location					location of the text part
				 * @param angle							angle of orientation of the text part
				 * @param text							text associated with this text part
				 * @param fontSize					size of the font for this text part
				 * @param labelTextAnalysis	label text analysis object for text length computation
				 * @param error							an error object to be populated
				 * @reqid 001.0063
				 */
				TextPart( const Point &location,float angle, 
					const TCHAR* text,float fontSize,
					LabelTextAnalysis *labelTextAnalysis,
					Error& error);

				/**
				 * @reqid 001.0063
				 */
				~TextPart();

				/**
				 * Get the location.
				 * The location is the absolute location of the TextPart with respect to the entire map. 
				 * @return the absolute location
				 * @reqid 001.063
				 */
				const Point* GetLocation() const
				{
					return m_enclosingRectangle.GetLowerLeft();
				}

				/** Set the location.
				 *	@param xcoord		x coordinate
				 *	@param ycoord		y coordinate
				 *	@see GetAbsoluteLocation
				 *	@reqid 001.063
				 */
				void SetLocation( float xcoord, float ycoord );

				/** Set the location.
				 *	@param point	absolute location
				 *	@see GetAbsoluteLocation
				 *	@reqid 001.063
				 */
				void SetLocation( const Point& point );

				/**
				 * Restructure the text part with new parameters. This allows you to change
                 * font style as well (e.g. after font reduction etc.)
				 * @param location - location of the text part
				 * @param angle - angle of orientation of the text part
				 * @param text - text associated with this text part
				 * @param fontSize - size of the font for this text part
				 * @param labelTextAnalysis - label text analysis object for text length computation
				 * @param error - an error object to be populated
				 * @reqid 001.0063
				 */
				void Rebuild( const Point &location,float angle, const TCHAR* text,float fontSize,
					LabelTextAnalysis *labelTextAnalysis, Error& error );

				/** Get the orientation angle of the text part.
				 * @return the angle in radians
				 * @reqid 001.063
				 */
				float GetAngle() const
				{
					return m_angle;
				}

				/** Set the angle.
				 * @see GetAngle
				 * @reqid 001.063
				 */
				void SetAngle( float angle );

				/** Get the label text string that constitutes the text part.
				 *	@return the address of the label text string
				 *	@reqid 001.063
				 */
				const String* GetText() const
				{
					return &m_text;
				}

				/** Set the text.
				 *	@see GetText
				 *	@reqid 001.063
				 */
				void SetText( const TCHAR* text, Error& error )
				{
					m_text.AssignString(text, error);
				}

				/**
				 * Moves the textpart's position by (dx,dy)
				 * @param dx - amount to move horizontally
				 * @param dy - amount to move vertically
				 * @reqid 001.0072
				 */
				void Translate(float dx,float dy);

				/**
				* Rotates the textpart about a given anchor point to a given angle
				* @param anchorPoint	anchor point about which to rotate
				* @param angle - angle to rotate to
				* @reqid 003.0019
				*/
				void RotateToAngle(const Point &anchorPoint, float angle);

				/**
				* Rotate self about the specified origin.
				* @param anchorPoint - point about which to rotate
				* @param sin_t - sine of angle by which to rotate
				* @param cos_t - cosine of angle by which to rotate
				* @reqid 001.0063
				*/
				void Rotate( const Point &anchorPoint, float sin_t, float cos_t );

				/**
				 * Create a deep copy of the TextPart Class.
				 * @return a new TextPart identical to the original.
				 * @reqid 001.0063
				 */
				TextPart* Clone(Error& error) const
				{
					TextPart* tp = new TextPart(error);
					if (tp == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					if (error)
					{
						delete tp;
						tp = NULL;
					}
					else
					{
						Copy( *this, *tp, error );
						if (error)
						{
							delete tp;
							tp = NULL;
						}
					}

					return tp;
				}

				/**
				* Get the enclosing rectangle of the text part.
				* @return the text part's enclosing rectangle
				* @reqid 001.0063
				*/
				const Rectangle* GetEnclosingRectangle() const
				{ 
					return &m_enclosingRectangle;
				}

				/**
				* Get the length of the text of the text part.
				* @return the length of the text of the text part in inches
				* @reqid 001.0063
				*/
				float GetTextLength() const 
				{ 
					return m_textLength; 
				}
		
				/**
				* Get the height enclosing rectangle.
				* @return height enclosing rectangle
				* @reqid 001.0063
				*/
				float GetHeightOfEnclosingRectangle() const 
				{ 
					return m_enclosingRectangle.GetLowerLeft()->DistanceFrom(
						*( m_enclosingRectangle.GetUpperLeft() ) );
				}

				/**
				* Flips the TextObject 180 degrees around its LowerLeft.
				* @reqid 006.0080
				*/
				void Flip();
            
#ifdef S11N
				/**
				 * Serialization.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				void Serialize( ISerializer& emitter, Error& error ) const;
#endif

			private:
				/**
				* Make deep copy of one instance's members to another
				* @param src
				* @param dest
				* @param error an Error object
				* @reqid 001.0063
				*/
				static void Copy( const TextPart& src, TextPart& dest, Error& error );

				/**
				* Create a new TextPart
				* @param error		an Error object
				* @reqid 001.0063
				*/
				TextPart(Error& error);

				/**
				* Populates the input text box with the enclosing rectangle of the text part.
				* @param location						lower left corner of text box
				* @param fontSize						size of the text part
				* @param labelTextAnalysis	used for computing length and actual height of the text.
				* @param textBox						text box to populate
				* @reqid 001.0065
				*/
				void GenerateEnclosingRectangle( const Point& location, float fontSize, LabelTextAnalysis &labelTextAnalysis,
					Rectangle &textBox );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				//The lower left corner of the enclosing rectangle stores the absolute position of the TextPart.
				Rectangle m_enclosingRectangle;
				float m_angle;
				String m_text;
				float m_textLength;
		};

		/**
		 * Class that holds an array of derived type TextPart object pointers.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API TextParts : public ClonableLinkedList < TextPart, TextParts >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				TextParts()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~TextParts()
				{
				}
		};

		/**
		 * A portion of label sharing the same text style and optionally identified by its name.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API TextObject
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_NullArgument,
					ErrorCode_TextObjectHasToHaveNonZeroLengthText
				};

				/**
				 * Construct new text object using the given parameters.
				 * @param textStyle style of the text object
				 * @param text					text of the text object
				 * @param name					name used to identify the text object
				 * @param spaceFollows	true if a space follows the text object; false if not
				 * @param error					an Error object
				 * @reqid 001.0063
				 */
				TextObject( const TextStyle &textStyle, const TCHAR* text,
					const TCHAR* name, bool spaceFollows, Error& error )
					:	m_textStyle( &textStyle ),
						m_text( text, error ),
						m_name( name, error ),
						m_spaceFollows( spaceFollows ),
						m_isHidden( false ),
						m_boundingBoxLength(-1.0f),
						m_boundingBoxHeight(-1.0f)
				{
					if ( text == NULL || _tcslen( text ) == 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_TextObjectHasToHaveNonZeroLengthText );
						return;
					}
				}

				/**
				 * Destructor.
				 * @reqid 001.0063
				 */
				~TextObject()
				{
				}

				/**
				 * Get the text style of the text object.
				 * @return the address of the the text style of the text object
				 * @reqid 001.0063
				 */
				const TextStyle* GetTextStyle() const
				{
					return m_textStyle;
				}

				/**
				 * Set the text style of this text object.
				 * @param textStyle the style to set
				 * @see GetTextStyle
				 * @reqid 001.0063
				 */
				void SetTextStyle( const TextStyle& textStyle )
				{
					m_textStyle = &textStyle;
				}

				/**
				 * Get the text of the text object.
				 * @return the string of text that constitutes this text object
				 * @reqid 001.0063
				 */
				const String* GetText() const
				{
					return &m_text;
				}

				/**
				 * Set the text of the text object
				 * @param text		the new text
				 * @param error		an Error object
				 * @see GetText
				 * @reqid 001.0063
				 */
				void SetText( const TCHAR* text, Error& error )
				{
					if ( text == NULL || _tcslen( text ) == 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_TextObjectHasToHaveNonZeroLengthText );
						return;
					}

					m_text.AssignString(text, error);
				}

				/**
				 * Get the name of the text object.
				 * @return the text object name as a shared string reference
				 * @reqid 001.0063
				 */
				const SharedString* GetName() const
				{
					return &m_name;
				}

				/**
				 * Set the name of this text object.
				 * @param name		the text object's name
				 * @param error		an Error object
				 * @reqid 001.0063
				 */
				void SetName( const TCHAR* name, Error& error )
				{
					if ( name == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_NullArgument );
						return;
					}
					m_name.AssignString(name, error);
				}

				/**
				* Get the 'space follows' property of the text object.
				* @return the 'space follows' property
				* @reqid 001.0063
				*/
				bool GetSpaceFollows() const
				{
					return m_spaceFollows;
				}

				/**
				* Set the 'space follows' property of the text object.
				*	@param spaceFollows	The new value of the 'space follows' property
				*/
				void SetSpaceFollows( bool spaceFollows )
				{
					m_spaceFollows = spaceFollows;
				}

				/** Get the list of text parts.
				 * A list of TextPart objects that represent the text of the TextObject.
				 * @return the style
				 * @reqid 001.0063
				 */
				TextParts* GetTextParts()
				{
					return &m_textParts;
				}

				/** Get the list of text parts as an immutable collection.
				* A list of TextPart objects that represent the text of the TextObject.
				* @return the style
				* @reqid 001.0063
				*/
				const TextParts* GetTextParts() const
				{
					return &m_textParts;
				}

				/**
				 * Utility function to obtain the proper label text analysis object for this text object
				 * in one single step.
				 * @param mgr the label text analysis manager to use
				 * @param error error information
				 * @return the requested analysis object
				 * @reqid 001.0063
				 */
				LabelTextAnalysis* GetLabelTextAnalysis( LabelTextAnalysisMgr& mgr, Error& error ) const;

				/**
				 * Gets the cached length of the TextObject.
				 * LabelBuilder::PlaceAtOrigin() must be called for this value to be valid.
				 * @return the text object length
				 * @reqid 001.0063
				 */
				float GetBoundingBoxLength() const;

				/**
				 * Sets the length of the TextObject.
				 * This method should only be called by LabelBuilder::PlaceAtOrigin().
				 * @param length		text object length
				 * @reqid 001.0063
				 */
				void SetBoundingBoxLength(float length);

				/**
				 * Gets the cached height of the TextObject.
				 * LabelBuilder::PlaceAtOrigin() must be called for this value to be valid.
				 * @return text object height
				 * @reqid 001.0063
				 */
				float GetBoundingBoxHeight() const;

				/**
				 * Sets the height of the TextObject.
				 * This method should only be called by LabelBuilder::PlaceAtOrigin().
				 * @param	 height		text object height
				 * @reqid 001.0063
				 */
				void SetBoundingBoxHeight(float height);

				/** Get the lower left corner of the label bounding box.
				*	@return the bounding box lower left corner
				*	@reqid 002.0039
				*/
				const Point* GetLowerLeft() const;

				/**
				* Get the orientation angle of the text object.
				* This is meaningful for straight labels only.
				* @return the angle of the first text part in radians
				* @reqid 002.0039
				*/
				float GetAngle() const;

				/**
				* Get the value of the hidden flag.
				* @return true if hidden; false if shown
				* @reqid 006.0014
				*/
				bool IsHidden() const
				{
					return m_isHidden;
				}

				/**
				* Hide the text object.
				* @reqid 006.0014
				*/
				void Hide()
				{
					m_isHidden = true;
				}
				
				/**
				* Unhide the text object.
				* @reqid 006.0014
				*/
				void Unhide()
				{
					m_isHidden = false;
				}
				
				/**
				* Populate a Rectangle object with the bounding box coordinates.
				* @param boundingBox	The Rectangle object to be populated
				* @reqid 002.0039
				*/
				void BuildBoundingBox( Rectangle& boundingBox ) const;

				/**
				* Create a deep copy of this instance
				* @param error error information
				* @param parentMap (optional) a new parent map for cloned object
				* @return the copy
				* @reqid 001.0063
				*/
				TextObject* Clone( Error& error, Map* parentMap = NULL ) const;

				/**
				 * Moves the textpart's position by (dx,dy)
				 * @param dx - amount to move horizontally
				 * @param dy - amount to move vertically
				 * @reqid 001.0072
				 */
				void Translate(float dx,float dy);

				/**
				* Rotates the textparts about a given anchor point to a given angle
				* @param anchorPoint	anchor point about which to rotate
				* @param angle - angle to rotate to
				* @reqid 003.0019
				*/
				void RotateToAngle(const Point &anchorPoint, float angle);

				/**
				* Rotate self about the specified origin.
				* @param anchorPoint - point about which to rotate
				* @param sin_t - sine of angle by which to rotate
				* @param cos_t - cosine of angle by which to rotate
				* @reqid 001.0063
				*/
				void Rotate( const Point &anchorPoint, float sin_t, float cos_t );

				/**
				* Flips the StackLevel 180 degrees around its LowerLeft.
				* @reqid 006.0080
				*/
				void Flip();

				/**
				 * Compare if two text objects are 'equal', i.e. visually resulting in the same
				 * text being created.
				 * @param otherTextObject - The text object to compate this instance to.
				 * @return - True if objects are identical, false otherwise.
				 * @reqid 001.0063
				 */
				bool IsEqualTo( const TextObject& otherLevel ) const;

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


				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static TextObject* DeserializeNew( IDeserializer& deserializer, Error& error );
#endif

			private:
				/**
				 * Default constructor for cloning
				 */
				TextObject(Error& error)
					:	m_textStyle( NULL ),
						m_spaceFollows( false ),
						m_name( NULL, error )
				{
				};

				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @param error error information
				* @param parentMap (optional) a new parent map for cloned object
				* @reqid 001.0063
				*/
				static void Copy( const TextObject& src, TextObject& dest, Error& error, Map* parentMap = NULL );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
		private:
				const TextStyle* m_textStyle;
				String m_text;
				SharedString m_name;
				TextParts m_textParts;
				float m_boundingBoxLength;
				float m_boundingBoxHeight;
				bool m_spaceFollows;
				bool m_isHidden;
		};

		/**
		 * Class that holds a list of derived type TextObject object pointers.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API TextObjects : public ClonableLinkedList < TextObject, TextObjects >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				TextObjects()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~TextObjects()
				{
				}

				/**
				 * Get a shown element iterator positioned at the first shown element in the list
				 * @return the iterator
				 * @reqid 001.0063
				 */
				ShownElementIterator BeginShownElements()
				{
					ShownElementIterator iter( *this, m_head );
					// Advance past all hidden elements.
					iter.ResetToBeginning();
					return iter;
				}

				/**
				* Get an iterator positioned at the end of the list
				* @return the iterator
				* @reqid 001.0063
				*/
				ShownElementIterator EndShownElements()
				{
					ShownElementIterator iter( *this, m_tail );
					// Advance ahead of all hidden elements.
					iter.ResetToEnd();
					return iter;
				}

				/**
				* Get a const iterator positioned at the beginning of the list
				* @return the iterator
				* @reqid 001.0063
				*/
				ShownElementConstIterator BeginShownElements() const
				{
					ShownElementConstIterator iter( *this, m_head );
					// Advance past all hidden elements.
					iter.ResetToBeginning();
					return iter;
				}

				/**
				* Get a const iterator positioned at the end of the list
				* @return the iterator
				* @reqid 001.0063
				*/
				ShownElementConstIterator EndShownElements() const
				{
					ShownElementConstIterator iter( *this, m_tail );
					// Advance ahead of all hidden elements.
					iter.ResetToEnd();
					return iter;
				}

				/**
				* Get the count of elements not hidden.
				* @return the count of elements not hidden
				* @reqid 001.0063
				*/
				int GetShownElementCount() const
				{
					return BeginShownElements().GetShownElementCount();
				}
		};

		/**
		 * A single line of text within a label; it groups all the text objects within
		 * the same line of text.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API StackLevel
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
				StackLevel()
					:	m_boundingBoxLength(-1.0f),
						m_boundingBoxHeight(-1.0f)
				{
				}

				/**
				 * Destructor.
				 * @reqid 001.0063
				 */
				~StackLevel()
				{
				}

				/**
				 * Get the text objects collection at this stack level.
				 * @return the collection of text objects at this stack level
				 * @reqid 001.0063
				 */
				TextObjects* GetTextObjects()
				{
					return &m_textObjects;
				}

				/**
				* Get the text objects collection at this stack level as an immutable collection.
				* @return the collection of text objects at this stack level
				* @reqid 001.0063
				*/
				const TextObjects* GetTextObjects() const
				{
					return &m_textObjects;
				}

				/**
				 * Gets the cached length of the StackLevel's bounding box. LabelBuilder::PlaceAtOrigin() must be called for this value to be valid.
				 * @return StackLevel's bounding box length
				 * @reqid 001.0063
				 */
				float GetBoundingBoxLength() const;

				/**
				 * Sets the cached height of the StackLevel's bounding box. LabelBuilder::PlaceAtOrigin() should be the only method calling this function.
				 * @param	height		the height of the bounding box
				 * @reqid 001.0063
				 */
				void SetBoundingBoxHeight(float height);

				/**
				 * Gets the cached height of the StackLevel's bounding box. LabelBuilder::PlaceAtOrigin() must be called for this value to be valid.
				 * @return StackLevel's bounding box height
				 * @reqid 001.0063
				 */
				float GetBoundingBoxHeight() const;

				/**
				 * Sets the cached length of the StackLevel's bounding box. LabelBuilder::PlaceAtOrigin() should be the only method calling this function.
				 * @param	length		the length of the bounding box
				 * @reqid 001.0063
				 */
				void SetBoundingBoxLength(float length);

				/** Get the lower left corner of the label bounding box.
				*	@return the bounding box lower left corner
				*	@reqid 002.0039
				*/
				const Point* GetLowerLeft() const;

				/**
				* Get the orientation angle of the text object.
				* This is meaningful for straight labels only.
				* @return the angle of the first text part in radians
				* @reqid 002.0039
				*/
				float GetAngle() const;

				/**
				* Report whether every text object at this stack level is hidden.
				* @return true if hidden; false if shown
				* @reqid 006.0014
				*/
				bool IsHidden() const;

				/**
				* Hide the text objects that constitute this stack level.
				* @reqid 006.0014
				*/
				void Hide();

				/**
				* Unhide the text objects that constitute this stack level.
				* @reqid 006.0014
				*/
				void Unhide();

				/**
				* Populate a Rectangle object with the bounding box coordinates.
				* @param boundingBox	The Rectangle object to be populated
				* @reqid 002.0039
				*/
				void BuildBoundingBox( Rectangle& boundingBox ) const;

				/**
				 * Moves the textObject's position by (dx,dy)
				 * @param dx - amount to move horizontally
				 * @param dy - amount to move vertically
				 * @reqid 001.0072
				 */
				void Translate(float dx,float dy);

				/**
				 * Moves the textObject's so that the boundingbox lowerleft is at location
				 * @param location - point to move to
				 * @reqid 001.0072
				 */
				void MoveToLocation(const Point &location);

				/**
				* Rotates the textObjects about a given anchor point to a given angle
				* @param anchorPoint	anchor point about which to rotate
				* @param angle - angle to rotate to
				* @reqid 003.0019
				*/
				void RotateToAngle(const Point &anchorPoint, float angle);

				/**
				* Rotate self about the specified origin.
				* @param anchorPoint - point about which to rotate
				* @param sin_t - sine of angle by which to rotate
				* @param cos_t - cosine of angle by which to rotate
				* @reqid 001.0063
				*/
				void Rotate( const Point &anchorPoint, float sin_t, float cos_t );

				/**
				* Create a deep copy of this instance
				* @param error error information
				* @param parentMap (optional) a new parent map for a cloned object
				* @return the copy
				* @reqid 001.0063
				*/
				StackLevel* Clone( Error& error, Map* parentMap = NULL ) const
				{
					StackLevel* sl = new StackLevel();
					if (sl == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					if (error)
					{
						delete sl;
						sl = NULL;
					}
					else
					{
						Copy( *this, *sl, error, parentMap );
						if (error)
						{
							delete sl;
							sl = NULL;
						}
					}
					return sl;
				}

				/**
				 * Compare if two stack levels are 'equal', i.e. visually resulting in the same
				 * text being created.
				 * @param otherLevel - The level to compate this instance to.
				 * @return - True if levels are identical, false otherwise.
				 * @reqid 001.0063
				 */
				bool IsEqualTo( const StackLevel& otherLevel ) const;

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


				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static StackLevel* DeserializeNew( IDeserializer& deserializer, Error& error );
#endif

			private:
				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @param error error information
				* @param parentMap (optional) a new parent map for a cloned object
				* @reqid 001.0063
				*/
				static void Copy( const StackLevel& src, StackLevel& dest, Error& error, Map* parentMap = NULL );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				TextObjects m_textObjects;
				float m_boundingBoxLength;
				float m_boundingBoxHeight;
		};

		/**
		 * Class that holds a list of derived type StackLevels object pointers.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API StackLevels : public LinkedList < StackLevel >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				StackLevels()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~StackLevels()
				{
				}

				/**
				 * Get a shown element iterator positioned at the first shown element in the list
				 * @return the iterator
				 * @reqid 001.0063
				 */
				ShownElementIterator BeginShownElements()
				{
					ShownElementIterator iter( *this, m_head );
					// Advance past all hidden elements.
					iter.ResetToBeginning();
					return iter;
				}

				/**
				* Get an iterator positioned at the end of the list
				* @return the iterator
				* @reqid 001.0063
				*/
				ShownElementIterator EndShownElements()
				{
					ShownElementIterator iter( *this, m_tail );
					// Advance ahead of all hidden elements.
					iter.ResetToEnd();
					return iter;
				}

				/**
				* Get a const iterator positioned at the beginning of the list
				* @return the iterator
				* @reqid 001.0063
				*/
				ShownElementConstIterator BeginShownElements() const
				{
					ShownElementConstIterator iter( *this, m_head );
					// Advance past all hidden elements.
					iter.ResetToBeginning();
					return iter;
				}

				/**
				* Get a const iterator positioned at the end of the list
				* @return the iterator
				* @reqid 001.0063
				*/
				ShownElementConstIterator EndShownElements() const
				{
					ShownElementConstIterator iter( *this, m_tail );
					// Advance ahead of all hidden elements.
					iter.ResetToEnd();
					return iter;
				}

				/**
				* Get the count of elements not hidden.
				* @return the count of elements not hidden
				* @reqid 001.0063
				*/
				int GetShownElementCount() const
				{
					return BeginShownElements().GetShownElementCount();
				}
		};

		/**
		 * Class that holds a simple or complex label
		 * @reqid 001.0063
		 *
		 */
		class LABELINGENGINE_API RegularLabel : public Label
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_IndexOutOfRange = 1,
					ErrorCode_AllocationFailure,
					ErrorCode_LabelNull,
					ErrorCode_MemberNull,
					ErrorCode_IncorrectJustification
				};

				/**
				 * Constructor.
				 * in the text objects collection with the attributes given
				 * @param error		an error object to be populated
				 * @reqid 001.0063
				 */
				RegularLabel(Error& error) : Label( LabelType_Regular, error )
				{
				}

				/**
				 * Constructor.
				 * Creates a single stack level with a single text object.
				 * in the text objects collection with the attributes given
				 * @param	textStyle			label style
				 * @param	text					string holding text label
				 * @param error					error object to be populate on exceptional condition
				 * @reqid 001.0063
				 */
				RegularLabel(const TextStyle &textStyle, const TCHAR *text, Error &error);

				/**
				 * Destructor. Deletes the stack levels of the label.
				 * @reqid 001.0063
				 *
				 */
				~RegularLabel()
				{
				}

				/**
				   If any one of the lines of the label to be placed is longer than the input length, this
				   method returns true, otherwise, false.
				 * @param maxLabelLength - maximum label length allowed.
				 * @param error - standard error object
				 * @return true->label too long, false->label not too long
				 * @reqid TODO
				 */
				bool IsLabelTooLong(float maxLabelLength,Error& error);

				/**
				 * Report whether the label is a simple, one-line label.
				 * @return true if label is simple, one-line label; false if not.
				 * @reqid 001.0063
				 */
				bool IsSimpleOneLineLabel() const;

				/**
				 * Report whether the label is simple in the sense that all text objects
				 * have the same text style.
				 * @return true if label is simple; false if not.
				 * @reqid 001.0063
				 */
				bool IsSimpleLabel() const;

				/**
				 * Compose the full text of the label
				 * @param labelText	[out] a reference to a string to be populated with the text
				 * @param error error information
				 * @reqid 001.0063
				 */
				void GetFullText( String& fullText, Error& error ) const;

				/**
				 * Compose the full text of the label. New lines are replaced with spaces when needed.
				 * @param labelText	[out] a reference to a string to be populated with the text
                 * @param preserveNewLineCharacters - If true the newline characters are inserted
                 *        between stack levels. Otherwise newlines are replaced by spaces.
				 * @param error error information
				 * @reqid ANNOTATION
				 */
				void GetFullText( String& fullText, bool preserveNewLineCharacters, Error& error ) const;

				/**
				* Unhide all elements of the label.
				* @reqid 006.0014
				*/
				virtual void Unhide();

				/**
				 * Create a copy of the current object
				 * @param error error information
				 * @param parentMap (optional) a new parent map for a cloned object
				 * @return the copy
				 * @reqid 001.0063
				 */
				RegularLabel* Clone( Error& error, Map* parentMap = NULL ) const
				{
					RegularLabel* rl = new RegularLabel(error);
					if (rl == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					if (error)
					{
						delete rl;
						rl = NULL;
					}
					else
					{
						Copy( *this, *rl, error, parentMap );
						if (error)
						{
							delete rl;
							rl = NULL;
						}
					}
					return rl;
				}

				/**
				 * Moves the text part's position by (dx,dy)
				 * @param dx - amount to move horizontally
				 * @param dy - amount to move vertically
				 * @reqid 001.0072
				 */
				virtual void Translate(float dx,float dy);

				/**
				* Rotate the label's parts to an absolute angle
				* @param anchorPoint	anchor point about which to rotate
				* @param angle - angle to rotate to
				* @reqid 003.0019
				*/
				virtual void RotateToAngle(const Point &anchorPoint, float angle);

				/**
				* Rotate self about the specified origin.
				* @param anchorPoint - point about which to rotate
				* @param angleDiff - rotation angle, needed to maintain internal angle
				*                    without expensive calculations
				* @param sin_t - sine of angle by which to rotate
				* @param cos_t - cosine of angle by which to rotate
				* @reqid 001.0063
				*/
				void Rotate( const Point &anchorPoint, float angleDiff, float sin_t, float cos_t );

				/**
				* Get the angle of the label.
				* @see Label::GetAngle
				* @reqid 001.0063
				*/
				virtual float GetAngle() const;

				/**
				 * Get the stack levels collection.
				 * @return the collection of stack levels for the label
				 * @reqid 001.0063
				 */
				StackLevels* GetStackLevels()
				{
					return &m_stackLevels;
				}

				/**
				* Get the stack levels collection as an immutable collection.
				* @return the collection of stack levels for the label
				* @reqid 001.0063
				*/
				const StackLevels* GetStackLevels() const
				{
					return &m_stackLevels;
				}

				/**
				 *  @see abstract method: Label::PlaceAtOrigin
				 *	@param stackSpacing		the distance between two stack levels
				 *	@param justification	the text justification for the label. The
																	input justification's value cannot be Feature_Justify.
				 *	@param	error					an Error object
				 *	@reqid 001.0063
				 */
				virtual void PlaceAtOrigin( float stackSpacing,PlacementRule::LabelJustification justification,Error& error);

				/**
				 *  Traverse the text objects in the label and find the first one whose name matches the input name.
				 *	@param name - name of TextObject to find.
				 *  @return NULL if the text object can't be found or the text object
				 *	@reqid 001.0072
				 */
				TextObject *FindTextObjectByName(const SharedString &name);

				/**
				 * @see Label::CheckOverlaps
				 * @param map 
				 * @param feature 
				 * @param placementRule 
				 * @param overlapSummary 
				 * @param error
				 * @reqid 001.0065
				 */
				void CheckOverlaps(
					Map *map, Feature *feature, PlacementRule *placementRule, PlacementState* placementState,
					ObstacleOverlapSummary **overlapSummary,Error &error);

				/**
				 * @see Label::CheckOverlaps
				 * @param map 
				 * @param feature 
				 * @param placementRule 
				 * @param obstacleAnalyzer 
				 * @param analyzeAllObjects 
				 * @param overlapSummary 
				 * @param error 
				 * @reqid 001.0065
				 */
				void CheckOverlaps(
					Map *map, Feature *feature, PlacementRule *placementRule, PlacementState* placementState,
					IObstacleAnalyzer* obstacleAnalyzer, bool analyzeAllObjects,
					ObstacleOverlapSummary **overlapSummary, Error &error);

				/* 
				 * @see Label::RegisterWithSpatialIndex
				 * @param spatialIndex - index to which to add the label parts.
				 * @error - error object
				 * @reqid 001.0064
				 */
				virtual void RegisterWithSpatialIndex(ISpatialIndex *spatialIndex, Error &error);

				/* 
				 * @see Label::GetTopXRange
				 * @reqid 002.0036
				 */
				virtual void GetTopXRange(float &labelLeftX,float &labelRightX) const;

				/* 
				 * @see Label::GetBottomXRange
				 * @reqid 002.0036
				 */
				virtual void GetBottomXRange(float &labelLeftX,float &labelRightX) const;

				/* 
				 * @see Label::GetRightYRange
				 * @reqid 003.0029
				 */
				virtual void GetRightYRange(float &labelBottomY,float &labelTopY) const;

				/* 
				 * @see Label::GetLeftYRange
				 * @reqid 002.0036
				 */
				virtual void GetLeftYRange(float &labelBottomY,float &labelTopY) const;

				/**
				 * Regular label returns all the text parts boxes
				 * @see Label::GetTextBoxes
				 * @param boxes [out] the text boxes
				 * @param error [out] error info
				 * @reqid 004.0032
				 */
				virtual void GetTextBoxes( LinkedList<Rectangle> &boxes, Error& error ) const;

				/**
				 * Returns the upright rectangle of placed label that fully encloses all textparts
				 * @param error [out] error info
				 * @reqid 
				 */
				UprightRectangle GetPlacedBoudingBox( Error& error ) const;

				/* 
				 * Justify the label according to the justification parameter
				 * @param justification - justification to perform
				 * @reqid 006.0046
				 */
				virtual void Justify(PlacementRule::LabelJustification justification);

				/**
				* Flips the RegularLabel 180 degrees around its LowerLeft.
				* @reqid 006.0080
				*/
				void Flip();

				/**
				 * @see Label::Validate
				 * @reqid 001.0086
				 */
				virtual void Validate( bool* valid, ErrorDetails** validationErrors, Error& error ) const;

				/* 
				 * @reqid 006.0014
				 */
				virtual bool IsHidden() const;

				/**
				 * @see Label::IsEqualTo
				 * @param otherLabel -
				 * @return -
				 * @reqid 001.0063
				 */
				bool IsEqualTo( const Label& otherLabel ) const;

			protected:
				/**
				 * Make deep copy of one instance's members to another
				 * @param src source label
				 * @param dest destination label
				 * @param error error information
				 * @param parentMap (optional) a new parent map for a cloned object
				 * @reqid 001.0063
				 */
				static void Copy( const RegularLabel& src, RegularLabel& dest, Error& error, Map* parentMap = NULL );

#ifdef S11N
				/** 
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 */
				virtual void _Deserialize( IDeserializer& deserializer, Error& error );

				/**
				 * Serialization of the child class.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				virtual void _Serialize( ISerializer& emitter, Error& error ) const;
#endif

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				/**
				 * A collection of stack levels that constitute the label
				 * @reqid 001.0063
				 */
				StackLevels m_stackLevels;
		};

		///////////////////////////////////////////////////////////////////////////////
		// AssemblyComponentDefinition
		///////////////////////////////////////////////////////////////////////////////

		/**
		 * Base class for basic assembly component definition.
		 * Assemblies are defined in their own coordinate system. It is used only to
		 * positioned the components against each other. User may choose to build the
		 * label around point (0,0) or (100,100) - it does not matter. Relative position
		 * of the components matters. After all the components are positioned the label 
		 * is moved around the map space in search for space without changing the relative 
		 * positions, but disregarding the absolute values of the "template" coordinates.
		 */
		class LABELINGENGINE_API AssemblyComponentDefinition
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_NotImplemented
				};

				/**
				 * This defines how the position member should be applied to the component.
				 * For example, it the position is specified as (0,0) and the anchor is set
				 * to NorthEast, the component northeast corner will be at (0,0) coordinates.
				 * @reqid 001.0063
				 */
				enum AnchorAt
				{
					AnchorAt_East = 0,
					AnchorAt_NorthEast,
					AnchorAt_North,
					AnchorAt_NorthWest,
					AnchorAt_West,
					AnchorAt_SouthWest,
					AnchorAt_South,
					AnchorAt_SouthEast,
					AnchorAt_Center,
				};

				/**
				 * Anchor type tells the label builder what the relative position
				 * is relative to :-)
				 */
				enum AnchorType
				{
					/**
					 * Relative position is given with respect to the origin of 
					 * the assembly coordinates system.
					 */
					AnchorType_RelativeToAssembly,
					/**
					 * Relative position is given with respect to a point chosen
					 * on another component of the assembly.
					 */
					AnchorType_RelativeToOtherComponent,
				};

				/**
				 * Type of the component (RTTI)
				 */
				enum ComponentDefinitionType
				{
					ComponentDefinitionType_Text,
					ComponentDefinitionType_Symbol
				};

				/**
				 * Get the type of the component definition (RTTI)
				 * @reqid 001.0063
				 */
				virtual ComponentDefinitionType GetType() const = 0;

				/**
				 * Default constructor. Initializes the positioning to be absolutely centered.
				 * @reqid 001.0063
				 */
				AssemblyComponentDefinition();

				/** 
				 * Constructor. Initializes the positioning to be absolutely centered.
				 *
				 * @param name - the name of the AssemblyComponenetDefinition
				 * @param error - error object
				 *
				 * @reqid 001.0063
				 */
				AssemblyComponentDefinition(const TCHAR* name, Error& error)
					: m_anchor( AnchorAt_Center ), m_anchorType( AnchorType_RelativeToAssembly ),
					m_positioningComponentAnchor( AnchorAt_Center )
				{
					m_name.AssignString(name, error);
					m_invisible = false;
					m_isDynamic = false;
				}

				/**
				 *	@reqid 001.0063
				 */
				virtual ~AssemblyComponentDefinition();

				/** Get the Anchor.
				 *	@return the AnchorAt enumerated value for the assembly component
				 *	@reqid 001.0063
				 */
				AnchorAt GetAnchor() const
				{
					return m_anchor;
				}

				/** Set the Anchor.
				 *	@see GetAnchor
				 *	@param anchor the AnchorAt enumerated value for the assembly component
				 *	@reqid 001.0063
				 */
				void SetAnchor( AnchorAt anchor )
				{
					m_anchor = anchor;
				}

				/**
				 * Get the AssemblyComponentDefinition name. The name is used to reference the 
				 * definition where required (e.g. positioning, stretching, placement rule).
				 * @return - the AssemblyComponentDefinition name as a shared string reference
				 * @reqid 001.0063
				 */
				const SharedString* GetName() const
				{
					return &m_name;
				}

				/** Set the AssemblyComponentDefinition name.
				 *	@see GetName
				 *	@reqid 001.0063
				 */
				void SetName( const TCHAR* name, Error& error )
				{
					m_name.AssignString(name, error);
				}

				/** 
				 * Get the RelativePosition.
				 * This determines where in relation to the assembly origin the component 
				 * is to be placed. Assembly is defined in its own, private and abstract 
				 * coordinate system (though the units used are still the same engine units). 
				 * Relative positions of the components have to make sense among them, 
				 * they have nothing to do with the feature space of the map - they will be moved
				 * along with the whole label as it is repositioned in search for free space in the map.
				 * @return - The relative position.
				 * @reqid 001.0063
				 */
				const Point* GetRelativePosition() const
				{
					return &m_relativePosition;
				}

				/** Set the RelativePosition.
				 * @see GetRelativePosition
				 * @reqid 001.0063
				 */
				void SetRelativePosition( const Point& position );

				/** Set the RelativePosition.
				 *
				 *	@param x	The X coordinate of the relative position
				 *	@param y	The Y coordinate of the relative position
				 *	@see GetRelativePosition
				 *	@reqid 001.0063
				 */
				void SetRelativePosition( float x, float y );

				/**
				* Creates copy of self.
				* @param error an error object to be populated
				* @return copy of self
				* @reqid 001.0076
				*/
				virtual AssemblyComponentDefinition* Clone( Error& error ) const = 0;

				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @param error an error object to be populated
				* @reqid 001.0076
				*/
				static void Copy( const AssemblyComponentDefinition& src, 
					AssemblyComponentDefinition& dest, Error &error );

				/**
				 * Get the invisible flag If true then the component defined by this definition
				 * is "invisible" during obstacle detection. This is a consequence of a performance 
				 * tweak - all symbols are treated as boxes during the obstacle detection. If the 
				 * symbol component is not a rectangle (e.g. an arrowhead for the parking stand
				 * symbol) but overlaps should not happen anyway because of the nature of the features
				 * location, setting this flag on the component should allow tightly packed labels 
				 * (e.g. parking stands) to be placed.
				 *	@return m_invisible	is the component invisible
				 *	@reqid 006.0079
				 */
				const bool* GetInvisibleFlag() const
				{
					return &m_invisible;
				}

				/** Set the InvisibleFlag.
				 *	@see SetInvisibleFlag
				 *	@reqid 006.0079
				 */
				void SetInvisibleFlag( bool val )
				{
					m_invisible = val;
				}

				/**
				* Gets AnchorType
				* @return - AnchorType
				* @reqid 007.0049
				*/
				AnchorType GetAnchorType() const 
				{ 
					return m_anchorType; 
				}

				/**
				* Sets AnchorType
				* @param val - AnchorType
				* @reqid 007.0049
				*/
				void SetAnchorType( AnchorType val )
				{ 
					m_anchorType = val; 
				}

				/**
				* Gets RelativePositionComponent
				* @return - RelativePositionComponent
				* @reqid 007.0049
				*/
				const SharedString* GetRelativePositionComponent() const 
				{ 
					return &m_relativePositionComponent; 
				}

				/**
				* Sets RelativePositionComponent
				* @param val - RelativePositionComponent
				* @reqid 007.0049
				*/
				void SetRelativePositionComponent(const TCHAR* name, Error& error )
				{ 
					m_relativePositionComponent.AssignString( name, error ); 
				}

				/**
				* Gets PositioningComponentAnchor
				* @return - PositioningComponentAnchor
				* @reqid 007.0049
				*/
				AnchorAt GetPositioningComponentAnchor() const 
				{ 
					return m_positioningComponentAnchor; 
				}

				/**
				* Sets PositioningComponentAnchor
				* @param val - PositioningComponentAnchor
				* @reqid 007.0049
				*/
				void SetPositioningComponentAnchor(AnchorAt val) 
				{ 
					m_positioningComponentAnchor = val; 
				}

				/**
				 * Compute the point on the input bounding box where the anchor point specifier lies.
				 * @param boundingBox - bounding box on which the anchor point is computed
				 * @param anchor - the anchor which should be used
				 * @return the location of anchor point on the bounding box.
				 * @reqid 001.0072:
				 */
				static Point ComputeAnchorPointOnBox( const UprightRectangle &boundingBox, AnchorAt anchor );

				/** Get the IsDynamic flag.
				 * The IsDynamic flag indicates whether the AssemblyComponent defined
				 * will be potentially affected by the ComponentActivators.
				 * @return the IsDynamic flag
				 * @reqid 001.0063
				 */
				bool GetIsDynamic() const
				{
					return m_isDynamic;
				}

				/** Set the IsDynamic flag.
				 * @return the IsDynamic flag
				 * @see	GetIsDynamic
				 * @reqid 001.0063
				 */
				void SetIsDynamic( bool isDynamic )
				{
					m_isDynamic = isDynamic;
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

				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static AssemblyComponentDefinition* DeserializeNew( IDeserializer& deserializer, Error& error );

			protected:

				/** 
				 * Called on the specific implementation to deserialize itself.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 */
				virtual void _Deserialize( IDeserializer& deserializer, Error& error ) = 0;

				/**
				 * Serialization specific for the child class.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				virtual void _Serialize( ISerializer& emitter, Error& error ) const = 0;
#endif

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				SharedString m_name;
				bool m_invisible;
				bool m_isDynamic;

			protected:
				// The relative position specifies where the anchor is located,
				// which is defined with respect either to assembly (absolute positioning)
				// or another component. m_anchorType decides which one it is.
				Point m_relativePosition;
				AnchorAt m_anchor;

				AnchorType m_anchorType;
				SharedString m_relativePositionComponent;
				AnchorAt m_positioningComponentAnchor;
		};

		///////////////////////////////////////////////////////////////////////////////
		// LeaderAnchor
		///////////////////////////////////////////////////////////////////////////////

		/**
		 * A class that models the anchor characteristics of a leader
		 * The LeaderAnchorDefinition is a definition class; it's not owned by any particular label. Hence, it has no 
		 * permanent absolute position. It simply exposes the ability to turn a relative position into a absolute 
		 * position based on a particular label's transformed origin. 
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API LeaderAnchorDefinition
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_MaxAngleHasToBeNotLesserThanMinAngle
				};

				/**
				 * Creates new anchor definition.
				 * @param relativePosition the position of the anchor relative to the assembly label origin
				 * @param - minAngle Minimum acceptable placement angle of a leader, relative to the horizontal
				 *                   in the assembly label coordinate system. Has to be <= maxAngle.
				 * @param - maxAngle Maximum acceptable placement angle of a leader, relative to the horizontal
				 *                   in the assembly label coordinate system. Has to be >= minAngle.
				 * @param error - [out] Error info.
				 * @reqid 001.0063
				 */
				LeaderAnchorDefinition(const Point& relativePosition, float minAngle, float maxAngle, Error& error );

				/**
				 * @reqid 001.0063
				 */
				~LeaderAnchorDefinition();

				/**
				 * Get the point at which the leader is anchored
				 * @return - the position of the anchor relative to the assembly label origin
				 * @reqid 001.0063
				 */
				const Point* GetRelativePosition() const
				{
					return &m_relativePosition;
				}

				/**
				 * @see GetRelativePosition
				 * @reqid 001.0063
				 */
				void SetRelativePosition(const Point &relativePosition)
				{
					m_relativePosition=relativePosition;
				}

				/**
				 * Get the minimum acceptable placement angle of a leader, relative to the horizontal
				 * in the assembly label coordinate system, using this anchor.
				 * Angle is expressed in radians.
				 * @return minimum acceptable angle in radians
				 * @reqid 001.0063
				 */
				float GetMinAngle() const
				{
					return m_minAngle;
				}

				/**
				 * Set the angular constraints of the leader anchor.
				 * @see GetMinAngle, GetMaxAngle
				 * @param - minAngle Minimum acceptable placement angle of a leader, relative to the horizontal
				 *                   in the assembly label coordinate system. Has to be <= maxAngle.
				 *									 Angle must be expressed in radians.
				 * @param - maxAngle Maximum acceptable placement angle of a leader, relative to the horizontal
				 *                   in the assembly label coordinate system. Has to be >= minAngle.
				 * @param error - [out] Error info.
				 * @reqid 001.0063
				 */
				void SetMinMaxAngle( float minAngle, float maxAngle, Error& error )
				{
					if ( minAngle > maxAngle )
					{
						SET_ERROR_NOMSG( error, ErrorCode_MaxAngleHasToBeNotLesserThanMinAngle );
						return;
					}
					m_minAngle = minAngle;
					m_maxAngle = maxAngle;
				}

				/**
				 * Get the maximum acceptable placement angle of a leader, relative to the horizontal
				 * in the assembly label coordinate system, using this anchor.
				 * Angle is expressed in radians.
				 * @reqid 001.0063
				 */
				float GetMaxAngle() const
				{
					return m_maxAngle;
				}

				/**
				* Gets AnchorType
				* @return AnchorType
				* @reqid 007.0049
				*/
				AssemblyComponentDefinition::AnchorType GetAnchorType() const 
				{ 
					return m_anchorType; 
				}

				/**
				* Sets AnchorType
				* @param val AnchorType
				* @reqid 007.0049
				*/
				void SetAnchorType(AssemblyComponentDefinition::AnchorType val) 
				{ 
					m_anchorType = val; 
				}

				/**
				* Gets RelativePositionComponent
				* @return RelativePositionComponent
				* @reqid 007.0049
				*/
				const MapText::Dtp::SharedString* GetRelativePositionComponent() const 
				{ 
					return &m_relativePositionComponent; 
				}

				/**
				* Sets RelativePositionComponent
				* @param val RelativePositionComponent
				* @reqid 007.0049
				*/
				void SetRelativePositionComponent(const TCHAR* name, Error& error ) 
				{ 
					m_relativePositionComponent.AssignString( name, error );
				}

				/**
				* Gets PositioningComponentAnchor
				* @return PositioningComponentAnchor
				* @reqid 007.0049
				*/
				AssemblyComponentDefinition::AnchorAt GetPositioningComponentAnchor() const 
				{ 
					return m_positioningComponentAnchor; 
				}

				/**
				* Sets PositioningComponentAnchor
				* @param val PositioningComponentAnchor
				* @reqid 007.0049
				*/
				void SetPositioningComponentAnchor(AssemblyComponentDefinition::AnchorAt val) 
				{ 
					m_positioningComponentAnchor = val; 
				}

				/**
				 * Create a deep copy of this instance
				 * @return the copy
				 * @reqid 001.0063
				 */
				LeaderAnchorDefinition* Clone(Error& error) const
				{
					LeaderAnchorDefinition* la = new LeaderAnchorDefinition();

					if (la == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					{
						Copy( *this, *la, error );
						if ( error )
						{
							delete la;
							return NULL;
						}
					}

					return la;
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


			/**
			 * Deserializes a new object from the deserializer.
			 * @param deserializer - The source.
			 * @param output - Deserialized object if no error occured. NULL otherwise.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			static LeaderAnchorDefinition* DeserializeNew( IDeserializer& deserializer, Error& error );

#endif
			private:
				/**
				 * @reqid 001.0063
				 */
				LeaderAnchorDefinition();


				/**
				* Make deep copy of one instance's members to another
				* @param src
				* @param dest
				* @param error - error info
				* @reqid 001.0063
				*/
				static void Copy( const LeaderAnchorDefinition& src, LeaderAnchorDefinition& dest, Error& error );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				//This property stores the relative position of the anchor with respect to other components.
				Point m_relativePosition;

				/*Gives the angle range allowed for the leader line emanating from this anchor. The angle is given
				  with respect to the label's coordinate system's positive x-axis.*/
				float m_minAngle;
				float m_maxAngle;
				AssemblyComponentDefinition::AnchorType m_anchorType;
				SharedString m_relativePositionComponent;
				AssemblyComponentDefinition::AnchorAt m_positioningComponentAnchor;
		};

		/**
		 * Class that holds an array of derived type LeaderAnchor object pointers.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API LeaderAnchorDefinitions : public ClonableLinkedList < LeaderAnchorDefinition, LeaderAnchorDefinitions >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				LeaderAnchorDefinitions()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~LeaderAnchorDefinitions()
				{
				}
		};

		/**
		 * Text component is a textual part of an assembly label. It really is a container for in instance
		 * of a RegularLabel. It also needs a "recipe" for it's construction - an instance of the 
		 * TextComponentDefinition. Collection of the definitions is owned by the label definition.
		 */
		class LABELINGENGINE_API TextComponentDefinition : public AssemblyComponentDefinition
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_IncorrectJustification
				};

				/**
				 * The text component can either be justified using the BLOCK option,
				 * the LEVEL option, or the TEXTOBJ option.
				 * @reqid 001.0063
				 */
				enum AnchorMode
				{
					/**
					 * For the BLOCK option, the justification is indicated with respect
					 * to the bounding box enclosing the text component.
					 * @reqid 001.0063
					 */
					AnchorMode_Block,
					/**
					 * For the LEVEL option, the justification is indicated with respect
					 * to the bounding box enclosing the specified level.
					 * @reqid 001.0063
					 */
					AnchorMode_Level,
					/**
					 * For the TEXTOBJ option, the justification is indicated with respect
					 * to the bounding box enclosing the specified text object.
					 * @reqid 001.0063
					 */
					AnchorMode_TextObject,
				};

				/**	Default constructor
				 *
				 *	@reqid 001.0063
				 */
				TextComponentDefinition()
					:	m_anchorMode( AnchorMode_Block ),
						m_anchorLevelNumber( 0 ),
						m_justification( PlacementRule::LabelJustification_Center )
				{
				}

				/** 
				 *	Constructor
				 *
				 *	@param	name	the name of the TextComponentDefinition
				 *	@param	error	an Error object
				 *
				 *	@reqid 001.0063
				 */
				TextComponentDefinition( const TCHAR* name, Error& error )
					: AssemblyComponentDefinition( name, error )
				{
					if ( error )
					{
						return;
					}

					m_anchorMode=AnchorMode_Block;
					m_anchorLevelNumber=0;
					m_justification=PlacementRule::LabelJustification_Center;
				}

				/**	Default destructor
				 *
				 *	@reqid 001.0063
				 */
				~TextComponentDefinition()
				{}

				/**
				 * @see AssemblyComponentDefinition::GetType
				 * @return - component definition type
				 * @reqid 001.0063
				 */
				ComponentDefinitionType GetType() const
				{
					return ComponentDefinitionType_Text;
				}

				/** Get the AnchorMode
				 *	@return the AnchorMode of the text component definition
				 *	@see AnchorMode
				 *	@reqid 001.0063
				 */
				AnchorMode GetAnchorMode() const
				{
					return m_anchorMode;
				}

				/** Set the AnchorMode
				 *	@see AnchorMode
				 *	@see GetAnchorMode
				 *	@reqid 001.0063
				 */
				void SetAnchorMode( AnchorMode anchorMode )
				{
					m_anchorMode = anchorMode;
				}

				/**
				 *	Get the AnchorTextObjectName.
				 *	A text object used for relative positioning of this text component
				 *	if AnchorMode_TextObject is the specified anchor mode.
				 *	@return	name of the text object to be used for anchoring
				 *	@reqid 001.0063
				 */
				const SharedString &GetAnchorTextObjectName() const
				{
					return m_anchorTextObjectName;
				}

				/**
				*	Set the name of the text object to be used for anchoring.
				*	@see GetAnchorTextObjectName
				*	@reqid 001.0063
				*/
				void SetAnchorTextObjectName( const TCHAR* anchorTextObjectName, Error& error )
				{
					m_anchorTextObjectName.AssignString(anchorTextObjectName,error);
				}

				/**
				 *	Get the number of the stack level which should be used for
				 *	the justification of this text object.
				 *	@return the anchor level number
				 *	@reqid 001.0063
				 */
				int GetAnchorLevelNumber() const
				{
					return m_anchorLevelNumber;
				}

				/** Set the AnchorLevelNumber
				 *	@see GetAnchorLevelNumber
				 *	@reqid 001.0063
				 */
				void SetAnchorLevelNumber( int anchorLevelNumber )
				{
					m_anchorLevelNumber = anchorLevelNumber;
				}

				/** Get the label justification.
				 * This determines how the text will be justified in the TextComponent.
				 *	@return the label justification as a PlacementRule::Justification type
				 *	@see PlacementRule::Justification
				 *	@reqid 001.0063
				 */
				PlacementRule::LabelJustification GetJustification() const
				{
					return m_justification;
				}

				/** Set the LabelJustification.
				 *	@see PlacementRule::Justification
				 *	@see GetJustification
				 *	@reqid 001.0063
				 */
				void SetJustification(PlacementRule::LabelJustification justification,Error &error)
				{
					if (justification==PlacementRule::LabelJustification_Unknown)
					{
						SET_ERROR(error, ErrorCode_IncorrectJustification,_T("SetJustification does not accept PlacementRule::LabelJustification_Unknown."));
						return;
					}

					m_justification = justification;
				}

				/**
				* @see AssemblyComponentDefinition::Clone
				* @reqid 001.0076
				*/
				virtual TextComponentDefinition* Clone( Error& error ) const
				{
					TextComponentDefinition* copy 
						= new TextComponentDefinition(GetName()->GetString(), error);
					if ( copy == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
						return copy;
					}
					if ( !error )
					{
						Copy( *this, *copy, error );
					}
					if (error) //ctor or copy failed
					{
						delete copy;
						copy = NULL;
					}

					return copy;
				}

				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @param error an error object to be populated
				* @reqid 001.0076
				*/
				static void Copy( const TextComponentDefinition& src, 
					TextComponentDefinition& dest, Error &error )
				{
					AssemblyComponentDefinition::Copy( src, dest, error );
					if ( error )
					{
						return; //pass error to a client
					}

					dest.m_anchorMode = src.m_anchorMode;
					dest.m_anchorTextObjectName.AssignString( src.m_anchorTextObjectName.GetString(), error );
					if ( error )
					{
						return;
					}
					dest.m_anchorLevelNumber = src.m_anchorLevelNumber;
					dest.m_justification = src.m_justification;
				}

#ifdef S11N
			protected:
				/** 
				 * Called on the specific implementation to deserialize itself.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 */
				virtual void _Deserialize( IDeserializer& deserializer, Error& error );

				/**
				 * Serialization specific for the child class.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				virtual void _Serialize( ISerializer& emitter, Error& error ) const;
#endif

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				// This property determines the box with which the anchor is interpreted.
				// The anchor specifies where the relative position is located.
				AnchorMode m_anchorMode;

				//This property applies to the TEXTOBJ anchoring mode
				SharedString m_anchorTextObjectName;

				//This property applies to the LEVEL anchoring mode
				int m_anchorLevelNumber;

				//This specifies how to justify the regular label stored in this component.
				PlacementRule::LabelJustification m_justification;
		};

		/**
		 * Class that holds a list of TextComponentDefinition objects.
		 */
		class LABELINGENGINE_API TextComponentDefinitions 
			: public ClonableLinkedList < TextComponentDefinition, TextComponentDefinitions >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				TextComponentDefinitions()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~TextComponentDefinitions()
				{
				}
		};

		/**
		 * Class that holds a list of assembly components (either text or symbol)
		 */
		class LABELINGENGINE_API AssemblyComponentDefinitions 
			: public ClonableLinkedList < AssemblyComponentDefinition, AssemblyComponentDefinitions >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				AssemblyComponentDefinitions( bool ownsElements = true )
					: ClonableLinkedList < AssemblyComponentDefinition, AssemblyComponentDefinitions >( ownsElements )
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~AssemblyComponentDefinitions()
				{
				}
		};



		/**
		 * SymbolComponentDefinition. Every symbol component needs a SymbolComponentDefinition,
		 * which is a recipe of building the component. The definition resides in a collection
		 * owned by a Map instance. Definition may be shared by multiple assembly label instances.
		 */
		class LABELINGENGINE_API SymbolComponentDefinition : public AssemblyComponentDefinition
		{
			public:

				enum ErrorCode
				{
					ErrorCode_IdentifierCantBeNull = 1,
					ErrorCode_IdentifierCantBeEmptyString,
					ErrorCode_SourceCantBeNull,
					ErrorCode_SourceCantBeEmptyString,
					ErrorCode_ComponentSizeHasToBeGreaterThanZero,
				};

				/**
				 * Control if the symbol component should be scaled according to 
				 * other components size.
				 */
				enum Scaling
				{
					Scaling_None = 0,
					/**
					 * Scale component horizontally according to the size of components
					 * referenced in the horizontal scaling components list
					 */
					Scaling_Horizontal = 1,
					/**
					 * Scale component vertically according to the size of components
					 * referenced in the vertical scaling components list
					 */
					Scaling_Vertical = 2,
					/**
					 * Scale component vertically and horizontally according to the size of components
					 * referenced in the vertical/horizontal scaling components list
					 */
					Scaling_Both = Scaling_Horizontal | Scaling_Vertical,
				};

				/** 
				 * Constructor.
				 * @param name	- The name of the SymbolComponentDefinition
				 * @param source - Source of the symbol. This is not used by the engine. Client code may store
				 *                 information needed by the renderer here (e.g. font name, symbol library 
				 *                 name etc.). Can't be NULL.
				 * @param identifier - ID of the symbol within the source (symbol name, number etc.). This is 
				 *                     not used by the engine. Client code may store information needed by 
				 *                     the renderer here. Can't be NULL.
				 * @param error - [out] Error object.
				 * @reqid 001.0063
				 */
				SymbolComponentDefinition(const TCHAR* name, const TCHAR* source, 
					const TCHAR* identifier, Error& error) : AssemblyComponentDefinition( name, error ),
					m_horizontalScalingComponentDefinitions( true ),
					m_verticalScalingComponentDefinitions( true )
				{
					m_size=6.0f/72.0f;
					m_color=0;
					m_leftInternalScalingBuffer = 0;
					m_rightInternalScalingBuffer = 0;
					m_topInternalScalingBuffer = 0;
					m_bottomInternalScalingBuffer = 0;
					m_scaling = Scaling_None;
					SetSource( source, error );
					if ( error )
					{
						return;
					}
					SetIdentifier( identifier, error );
					if ( error )
					{
						return;
					}
				}

				/**	Default destructor
				 *
				 *	@reqid 001.0063
				 */
				~SymbolComponentDefinition()
				{}

				/**
				 * @see AssemblyComponentDefinition::GetType
				 * @return - component definition type
				 * @reqid 001.0063
				 */
				ComponentDefinitionType GetType() const
				{
					return ComponentDefinitionType_Symbol;
				}

				/**
				 * This returns the source of the symbol, either a file name, a font name, etc.
				 * @return a string identifying the source entity containing the symbol.
				 * This is not used by the engine. Client code may store information needed 
				 * by the renderer here. Can't be NULL.
				 * @reqid 001.0063
				 */
				const SharedString& GetSource() const
				{
					return m_source;
				}

				/**
				 * @see GetSource
				 * @reqid 001.0063
				 */
				void SetSource(const TCHAR* source, Error& error);

                /**
                 * @see GetSource file path
                 * @reqid 001.0063
                 */
                const SharedString& GetSourceFilePath() const
                {
                    return m_sourcePath;
                }
                
                /**
                 * Set the font path
                 * @reqid 001.0063
                 */
                void SetSourcePath(const char* sourcePath, Error& error)
                {
                    String filePath(sourcePath, error);
                    if(error)
                    {
                        return;
                    }
                    
                    String sourceName(m_source.GetString(), error);
                    if(error)
                    {
                        return;
                    }
                    sourceName.Lcase();
                    filePath.ConcatString("/", error);
                    filePath.ConcatString(sourceName.GetString(), error);
                    filePath.ConcatString(".ttf", error);
                    if(error)
                    {
                        return;
                    }
                    
                    m_sourcePath.AssignString(filePath.GetString(), error);
                }
            
				/**
				 * Get the string that identifies the symbol within the source container.
				 * This is either a character in a font, a symbol number, etc.
				 * It is not used by the engine. Client code may store information needed by 
				 * the renderer here. Can't be NULL.
				 * @return - identifier of symbol
				 * @reqid 001.0063
				 */
				const String& GetIdentifier() const
				{
					return m_identifier;
				}

				/**
				 * @see GetIdentifier
				 * @reqid 001.0063
				 */
				void SetIdentifier(const TCHAR* identifier, Error &error);

				/**
				 * Get the size of the symbol to give to the display system.
				 * @return the size of symbol in inches
				 * @reqid 001.0063
				 */
				float GetSize() const
				{
					return m_size;
				}

				/**
				 * @see GetSize
				 * @reqid 001.0063
				 */
				void SetSize(float size, Error& error )
				{
					if ( size <= 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_ComponentSizeHasToBeGreaterThanZero );
						return;
					}
					m_size=size;
				}

				/**
				 * Get the color of the symbol in which to draw it.
				 * @return the color of the symbol as an RGB long value
				 * @reqid 001.0063
				 */
				long GetColor() const
				{
					return m_color;
				}

				/**
				 * @see GetColor
				 * @reqid 001.0063
				 */
				void SetColor(long color)
				{
					m_color = (int)color;
				}

				/**
				 * Get the relative position of the bounding box that encloses the symbol in the 
				   assembly coordinate system. It's position is such that the symbol's relative position is
				   sitting on the symbol component's anchor which is defined with respect to the symbol's 
				   anchoring box.
				 * @return the bounding box as an upright rectangle
				 * @reqid 001.0063
				 */
				UprightRectangle& GetRelativeBoundingBox()
				{
					return m_relativeBoundingBox;
				}

				/**
				 * @see GetRelativeBoundingBox
				 * @reqid 001.0063
				 */
				void SetRelativeBoundingBox(UprightRectangle &relativeBoundingBox)
				{
					m_relativeBoundingBox=relativeBoundingBox;
				}

				/**
				* Gets Scaling
				* @return - Scaling
				* @reqid 007.0049
				*/
				Scaling GetScaling() const 
				{ 
					return m_scaling; 
				}

				/**
				* Sets Scaling
				* @param val - Scaling
				* @reqid 007.0049
				*/
				void SetScaling( Scaling val ) 
				{ 
					m_scaling = val; 
				}

				/**
				* Gets HorizontalScalingComponentDefinitions
				* @return - HorizontalScalingComponentDefinitions
				* @reqid 007.0049
				*/
				SharedString::Collection* GetHorizontalScalingComponentDefinitions()
				{ 
					return &m_horizontalScalingComponentDefinitions; 
				}

				/**
				* Gets VerticalScalingComponentDefinitions
				* @return - VerticalScalingComponentDefinitions
				* @reqid 007.0049
				*/
				SharedString::Collection* GetVerticalScalingComponentDefinitions()
				{ 
					return &m_verticalScalingComponentDefinitions; 
				}

				/**
				* Gets LeftInternalScalingBuffer
				* @return - LeftInternalScalingBuffer
				* @reqid 007.0049
				*/
				float GetLeftInternalScalingBuffer() const 
				{ 
					return m_leftInternalScalingBuffer; 
				}

				/**
				* Sets LeftInternalScalingBuffer
				* @param val - LeftInternalScalingBuffer
				* @reqid 007.0049
				*/
				void SetLeftInternalScalingBuffer(float val) 
				{ 
					m_leftInternalScalingBuffer = val; 
				}

				/**
				* Gets RightInternalScalingBuffer
				* @return - RightInternalScalingBuffer
				* @reqid 007.0049
				*/
				float GetRightInternalScalingBuffer() const 
				{ 
					return m_rightInternalScalingBuffer; 
				}

				/**
				* Sets RightInternalScalingBuffer
				* @param val - RightInternalScalingBuffer
				* @reqid 007.0049
				*/
				void SetRightInternalScalingBuffer(float val) 
				{ 
					m_rightInternalScalingBuffer = val; 
				}

				/**
				* Gets TopInternalScalingBuffer
				* @return - TopInternalScalingBuffer
				* @reqid 007.0049
				*/
				float GetTopInternalScalingBuffer() const 
				{ 
					return m_topInternalScalingBuffer; 
				}

				/**
				* Sets TopInternalScalingBuffer
				* @param val - TopInternalScalingBuffer
				* @reqid 007.0049
				*/
				void SetTopInternalScalingBuffer(float val) 
				{ 
					m_topInternalScalingBuffer = val; 
				}

				/**
				* Gets BottomInternalScalingBuffer
				* @return - BottomInternalScalingBuffer
				* @reqid 007.0049
				*/
				float GetBottomInternalScalingBuffer() const 
				{ 
					return m_bottomInternalScalingBuffer; 
				}

				/**
				* Sets BottomInternalScalingBuffer
				* @param val - BottomInternalScalingBuffer
				* @reqid 007.0049
				*/
				void SetBottomInternalScalingBuffer(float val) 
				{ 
					m_bottomInternalScalingBuffer = val; 
				}

				/**
				* Creates copy of self.
				* @param error an error object to be populated
				* @return copy of self
				* @reqid 001.0076
				*/
				virtual SymbolComponentDefinition* Clone( Error& error ) const
				{
					SymbolComponentDefinition* copy 
						= new SymbolComponentDefinition( GetName()->GetString(), 
						m_source.GetString(), m_identifier.GetString(), error);
					if ( copy == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
						return copy;
					}
					if ( !error )
					{
						Copy( *this, *copy, error );
					}
					if (error) //ctor or copy failed
					{
						delete copy;
						copy = NULL;
					}
					return copy;
				}

				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @param error an error object to be populated
				* @reqid 001.0076
				*/
				static void Copy( const SymbolComponentDefinition& src, 
					SymbolComponentDefinition& dest, Error &error )
				{
					AssemblyComponentDefinition::Copy( src, dest, error );
					if ( error )
					{
						return; //pass error to a client
					}
					
					dest.m_relativeBoundingBox = src.m_relativeBoundingBox;
					dest.m_source.AssignString( src.m_source.GetString(), error );
					if ( error )
					{
						return; //pass error to a client
					}
                    if(src.m_sourcePath.GetString() != NULL)
                    {
                        dest.m_sourcePath.AssignString(src.m_sourcePath.GetString(), error);
                        if ( error )
                        {
                            return; //pass error to a client
                        }
                    }
					dest.m_identifier.AssignString( src.m_identifier.GetString(), error );
					if ( error )
					{
						return; //pass error to a client
					}
					dest.m_size = src.m_size;
					dest.m_color = src.m_color;
					dest.m_scaling = src.m_scaling;
					dest.m_horizontalScalingComponentDefinitions.Clear();
					for ( SharedString::Collection::ConstIterator scalingComponent = src.m_horizontalScalingComponentDefinitions.Begin();
						!scalingComponent.IsNull(); scalingComponent++ )
					{
						SharedString* name = new SharedString( *scalingComponent );
						if ( name == NULL )
						{
							SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
							return;
						}

						dest.m_horizontalScalingComponentDefinitions.AddElement( name, error );
						if ( error )
						{
							delete name;
							return;
						}
					}

					dest.m_verticalScalingComponentDefinitions.Clear();
					for ( SharedString::Collection::ConstIterator scalingComponent = src.m_verticalScalingComponentDefinitions.Begin();
						!scalingComponent.IsNull(); scalingComponent++ )
					{
						SharedString* name = new SharedString( *scalingComponent );
						if ( name == NULL )
						{
							SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
							return;
						}

						dest.m_verticalScalingComponentDefinitions.AddElement( name, error );
						if ( error )
						{
							delete name;
							return;
						}
					}
                    
                    dest.m_leftInternalScalingBuffer = src.m_leftInternalScalingBuffer;
                    dest.m_rightInternalScalingBuffer = src.m_rightInternalScalingBuffer;
                    dest.m_topInternalScalingBuffer = src.m_topInternalScalingBuffer;
                    dest.m_bottomInternalScalingBuffer = src.m_bottomInternalScalingBuffer;

				}
			
#ifdef S11N
				protected:

				/** 
				 * Called on the specific implementation to deserialize itself.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 */
				virtual void _Deserialize( IDeserializer& deserializer, Error& error );

				/**
				 * Serialization.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				virtual void _Serialize( ISerializer& emitter, Error& error ) const;
#endif

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			protected:

				friend class AssemblyComponentDefinition;

				/**
				 * Default constructor. Cloning only.
				 * @reqid 001.0063
				 */
				SymbolComponentDefinition()
					: m_horizontalScalingComponentDefinitions( true ),
					m_verticalScalingComponentDefinitions( true )
				{
					m_size=6.0f/72.0f;
					m_color=0;
					m_leftInternalScalingBuffer = 0;
					m_rightInternalScalingBuffer = 0;
					m_topInternalScalingBuffer = 0;
					m_bottomInternalScalingBuffer = 0;
					m_scaling = Scaling_None;
				}


				//This defines where the enclosing bounding box is located in the assembly coordinate system.
				UprightRectangle m_relativeBoundingBox;

				// The following properties are used for generating the output.
				// They are not used for placement in the engine.
				SharedString m_source;
                SharedString m_sourcePath;
				String m_identifier;
				float m_size;
				int m_color;
				Scaling m_scaling;
				SharedString::Collection m_horizontalScalingComponentDefinitions;
				SharedString::Collection m_verticalScalingComponentDefinitions;
				float m_leftInternalScalingBuffer;
				float m_rightInternalScalingBuffer;
				float m_topInternalScalingBuffer;
				float m_bottomInternalScalingBuffer;
		};

		/**
		 * Class that holds a list of TextComponentDefinition objects.
		 */
		class LABELINGENGINE_API SymbolComponentDefinitions 
			: public ClonableLinkedList < SymbolComponentDefinition, SymbolComponentDefinitions >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				SymbolComponentDefinitions()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~SymbolComponentDefinitions()
				{
				}
		};

		/**
		 * Assembly label definition. The class store three collections and takes ownership 
		 * of all of them.
		 * Each assembly requires an assembly definition, which is a "recipe" for the assembly
		 * construction. Typically the same definition can be shared by many instances of 
		 * assembly labels. It may be identified by its name. The name-keyed collection of 
		 * assembly definitions is owned by a Map instance.
		 */
		class LABELINGENGINE_API AssemblyLabelDefinition
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_UndefinedComponentActivator
				};

				/**
				 * A collection of assembly label definitions
				 * @reqid 001.0063
				 */
				class LABELINGENGINE_API Collection : public StringKeyedTree < AssemblyLabelDefinition >
				{
					public:
						/**
						 * Default constructor implemented to avoid compiler generated object code.
						 * @reqid 001.0063
						 */
						Collection()
						{
						}

						/**
						 * Default destructor implemented to avoid compiler generated object code.
						 * @reqid 001.0063
						 */
						~Collection()
						{
						}

						/**
						 * Get the specified assembly label definition from the cache
						 * if present; else create and cache.
						 * @param	name										definition name
						 * @param assemblyLabelDefinition	the requested object
						 * @param error										an Error object to be populated
						 * @reqid 001.0063
						 */
						void GetAssemblyLabelDefinition(
							const TCHAR* name,
							AssemblyLabelDefinition** assemblyLabelDefinition,
							Error& error );
				};

			private:
				/** Constructor
				 * Only one constructor that works in concert with
				 * AssemblyLabelDefinitions::GetAssemblyLabelDefinition.
				 * @param name		the name used to identify the object
				 * @param error		an error object to be populated
				 * @reqid 001.0063
				 */
				AssemblyLabelDefinition( const TCHAR* name, Error& error )
					: m_name( name, error )
				{
					if ( error )
					{
						return;
					}

					m_leaderAnchors = new LeaderAnchorDefinitions();
					if (m_leaderAnchors == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
						return;
					}

					m_textComponentDefinitions =
						new TextComponentDefinitions();

					if (m_textComponentDefinitions == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
						return;
					}

					m_symbolComponentDefinitions =
						new SymbolComponentDefinitions();

					if (m_symbolComponentDefinitions == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
						return;
					}

					m_componentActivator = NULL;
				}

			public:
				/** Destructor
				 *	@reqid 001.0063
				 */
				~AssemblyLabelDefinition();

				/** Sets the leader anchors.
				 *	@param	leaderAnchors		take ownership
				 *	@see GetLeaderAnchors
				 *	@reqid 001.0063
				 */
				void SetLeaderAnchors( LeaderAnchorDefinitions* leaderAnchors )
				{
					delete m_leaderAnchors;

					m_leaderAnchors = leaderAnchors;
				}

				/** Get the leader anchors.
				 * LeaderAnchors collection holds a list of LeaderAnchor objects which determine 
				 * where the leader can be anchored to on the AssemblyLabel. Leader anchors are 
				 * only used when leadered placement is part of the execution sequence. If no 
				 * anchors are defined leadering fails. The anchors play no role in direct 
				 * (non-leadered) placement.
				 * @return LeaderAnchors
				 * @reqid 001.0063
				 */
				LeaderAnchorDefinitions* GetLeaderAnchors() const
				{
					return m_leaderAnchors;
				}

				/** Set the TextComponentDefinitions.
				 *	@param textComponentDefinitions		take ownership
				 *	@see GetTextComponentDefinitions
				 *	@reqid 001.0063
				 */
				void SetTextComponentDefinitions(
					TextComponentDefinitions* textComponentDefinitions )
				{
					delete m_textComponentDefinitions;

					m_textComponentDefinitions = textComponentDefinitions;
				}

				/** Get the TextComponentDefinitions.
				 * This is a collection of TextComponentDefinition objects that define the
				 * composition of the Assembly Label.
				 * @return the text component definition collection
				 * @reqid 001.0063
				 */
				TextComponentDefinitions* GetTextComponentDefinitions() const
				{
					return m_textComponentDefinitions;
				}

				/** Set the SymbolComponentDefinitions.
				 *	@param symbolComponentDefinitions		take ownership
				 *	@see GetSymbolComponentDefinitions
				 *	@reqid 001.0063
				 */
				void SetSymbolComponentDefinitions(SymbolComponentDefinitions* symbolComponentDefinitions)
				{
					delete m_symbolComponentDefinitions;

					m_symbolComponentDefinitions = symbolComponentDefinitions;
				}

				/** Get the SymbolComponentDefinitions.
				 * This is a collection of SymbolComponentDefinition objects that define the
				 * composition of the Assembly Label.
				 * @return SymbolComponentDefinitions
				 * @reqid 001.0063
				 */
				SymbolComponentDefinitions* GetSymbolComponentDefinitions() const
				{
					return m_symbolComponentDefinitions;
				}

				/** 
				 * Get the AssemblyLabelDefinition name. The name is not used by the placement
				 * engine, it is only maintained for user convenience and tracking capabilities.
				 * @return The name.
				 * @reqid 001.0063
				 */
				const SharedString* GetName() const
				{
					return &m_name;
				}

				/** 
				 * Sets the ComponentActivator, taking ownership
				 * @param ComponentActvator
				 * @reqid 006.0080
				 */
				void SetComponentActivator( ComponentActivator *componentActivator );

				/** 
				 * Gets the ComponentActivator
				 * @return ComponentActvator
			     * @reqid 006.0080
				 */
				ComponentActivator * GetComponentActivator( )
				{
					return m_componentActivator;
				}

				/**
				* Creates copy of self.
				* @param error an error object to be populated
				* @return copy of self
				* @reqid 001.0076
				*/
				virtual AssemblyLabelDefinition* Clone( Error& error ) const;
				/*******************************************************************
				There are no setters for the properties that constitute the key.
				They should not be needed and invoking them would not remove
				and re-insert the instance in the string keyed collection to
				reflect the new key.
				*******************************************************************/
			
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


				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static AssemblyLabelDefinition* DeserializeNew( IDeserializer& deserializer, Error& error );
#endif
			

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				SharedString m_name;

				SymbolComponentDefinitions* m_symbolComponentDefinitions;
				TextComponentDefinitions* m_textComponentDefinitions;
				LeaderAnchorDefinitions* m_leaderAnchors;

				ComponentActivator* m_componentActivator;
		};

		/**
		 * Base class for basic building blocks of an assembly (SymbolComponent and TextComponent).
		 * 
		 */
		class LABELINGENGINE_API AssemblyComponent
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
				 * Type of the component (RTTI)
				 */
				enum ComponentType
				{
					ComponentType_Text,
					ComponentType_Symbol
				};

				/**
				 * Get the type of component (RTTI)
				 * @return - type of the derived class
				 * @reqid 001.0063
				 */
				virtual ComponentType GetType() const = 0;

				/**
				 * Default constructor. Build a component
				 * @reqid 001.0063
				 */
				AssemblyComponent()
					: m_isPositioned( false )
				{
				}

				/**
				 * Destructor.
				 * @reqid 001.0063
				 */
				virtual ~AssemblyComponent()
				{
				}

				/**
				 * Report whether this component is hidden.
				 * @return true if hidden; false if shown
				 * @reqid 006.0014
				 */
				virtual bool IsHidden() const = 0;

				/**
				 * Get the assembly component definition (whatever derived type it really is).
				 * @return - the definition as an abstract base class pointer
				 * @reqid 001.0063
				 */
				virtual AssemblyComponentDefinition* GetAssemblyComponentDefinition() = 0;

				/**
				* Gets Positioned
				* @return - Positioned
				* @reqid 007.0049
				*/
				bool GetIsPositioned() const 
				{ 
					return m_isPositioned; 
				}

				/**
				* Sets Positioned
				* @param val - Positioned
				* @reqid 007.0049
				*/
				void SetIsPositioned(bool val) 
				{ 
					m_isPositioned = val; 
				}

				/**
				 * Create a deep copy of this instance
				 * @param error error object
				 * @param parentMap (optional) a new parent map for cloned object
				 * @return the copy
				 * @reqid 001.0063
				 */
				virtual AssemblyComponent* Clone( Error& error, Map* parentMap = NULL ) const = 0;

				/**
				 * Get the bounding box of the component.
				 * @return - the bounding box
				 * @reqid 007.0049
				 */
				virtual UprightRectangle GetBoundingBox() = 0;

#ifdef S11N
				mutable SharedString m_componentDefinitionName;
#endif

		protected:
			/**
			* Make deep copy of one instance's members to another
			* @param src source object
			* @param dest destination object
			* @param error error object
			* @reqid 001.0063
			*/
			static void Copy( const AssemblyComponent& src,AssemblyComponent& dest )
			{
				dest.m_isPositioned = src.m_isPositioned;
			}

			/****************************************************************************************
			THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
			TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
			****************************************************************************************/
		private:
			bool m_isPositioned;
		};

		///////////////////////////////////////////////////////////////////////////////
		// TextComponent
		///////////////////////////////////////////////////////////////////////////////

		/**
		 * Text component is the textual part of an assembly label. It really is a container for 
		 * an instance of the RegularLabel class. It also needs a "recipe" for it's construction -
		 * an instance of the Text Component definition. Collection of the definitions is owned 
		 * by the label definition.
		 */
		class LABELINGENGINE_API TextComponent : public AssemblyComponent
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_TextComponentDefinitionCantBeNull,
				};

				/**
				 * Constructor. Create a text component.
				 * @param name - A name to identify the component.
				 * @param componentDefinition - Definition to use for this component. It has to be nested in the
				 *                              label definition of the owning assembly.
				 * @param error - Error object to be populated.
				 * @reqid 001.0063
				 */
				TextComponent(TextComponentDefinition& componentDefinition, Error& error)
					:	AssemblyComponent( )
				{
					m_regularLabel = new RegularLabel(error);
					if (m_regularLabel == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}

					SetTextComponentDefinition( componentDefinition, error );
					if ( error )
					{
						return;
					}
				}

				/**
				 * Destructor. Delete the regular label.
				 * @reqid 001.0063
				 */
				~TextComponent()
				{
					delete m_regularLabel;
				}

				/**
				 * @see AssemblyComponent::GetType
				 * @return - component type
				 * @reqid 001.0063
				 */
				ComponentType GetType() const
				{
					return ComponentType_Text;
				}

				/**
				 * Create a copy of the current object.
				 * @param error error information
				 * @param parentMap (optional) a new parent map for cloned object
				 * @return the copy
				 * @reqid 001.0063
				 */
				virtual TextComponent* Clone( Error& error, Map* parentMap = NULL ) const
				{
					TextComponent* tc = new TextComponent( *m_textComponentDefinition, error);
					if (tc == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					if (error)
					{
						delete tc;
						tc = NULL;
					}
					else
					{
						Copy( *this, *tc, error, parentMap );
						if (error)
						{
							delete tc;
							tc = NULL;
						}
					}
					return tc;
				}

				/** Get the TextComponentDefinition, which stores data on how to render the TextComponent.
				 * @return the associated text component definition
				 * @reqid 001.063
				 */
				TextComponentDefinition* GetTextComponentDefinition() const
				{
					return m_textComponentDefinition;
				}

				/** Set the TextComponentDefinition.
				 * @see	GetTextComponentDefinition
				 * @reqid 001.063
				 */
				void SetTextComponentDefinition( TextComponentDefinition& textComponentDefinition, Error& error )
				{
					if ( (TextComponentDefinition*)(&textComponentDefinition) == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_TextComponentDefinitionCantBeNull );
						return;
					}
					m_textComponentDefinition = &textComponentDefinition;
				}

				/**
				 * Get the assembly component definition (whatever derived type it really is).
				 * @return - the definition as an abstract base class pointer
				 * @reqid 001.0063
				 */
				AssemblyComponentDefinition* GetAssemblyComponentDefinition()
				{
					return m_textComponentDefinition;
				}

				/**
				* Flips the TextComponent 180 degrees around its LowerLeft.
				* @reqid 006.0080
				*/
				void Flip();

				/**
				* Report whether this component is hidden.
				* @return true if hidden; false if shown
				* @reqid 006.0014
				*/
				virtual bool IsHidden() const;

				/**
				* Hide all the stack levels that constitute this text component's regular label.
				* @reqid 006.0014
				*/
				void Hide();

				/**
				* Unhide all the stack levels that constitute this text component's regular label.
				* @reqid 006.0014
				*/
				void Unhide();

				/** Get the regular label associated with the text component.
				 * @return the associated regular label
				 * @reqid 001.063
				 */
				RegularLabel* GetRegularLabel() const
				{
					return m_regularLabel;
				}

				/** Set the regular label to the input one. Takes ownership of the memory pointed to by the
				    input regularLabel.
				 * @reqid 001.063
				 */
				void SetRegularLabel(RegularLabel *regularLabel)
				{
					if (m_regularLabel!=NULL)
						delete m_regularLabel;

					m_regularLabel=regularLabel;
				}

				/**
				 * Get the bounding box of the component.
				 * @return - the bounding box
				 * @reqid 007.0049
				 */
				UprightRectangle GetBoundingBox()
				{
					Rectangle bbox;
					m_regularLabel->BuildBoundingBox( bbox );
					UprightRectangle uprightBbox;
					bbox.GetBoundingBox( uprightBbox );

					return uprightBbox;
				}

				/**
				 * Compare if the two text components are 'equal', i.e. visually resulting in the same
				 * text being created.
				 * @param otherComponent - The component to compare this instance to.
				 * @return - True if components are identical, false otherwise.
				 * @reqid 001.0063
				 */
				virtual bool IsEqualTo( const TextComponent& otherComponent );

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


				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static TextComponent* DeserializeNew( IDeserializer& deserializer, Error& error );
			protected:
				TextComponent(){};

#endif
			protected:
				/**
				 * Make deep copy of one instance's members to another
				 * @param src
				 * @param dest
				 * @param error error information
				 * @param parentMap a new parent map for cloned object
				 * @reqid 001.0063
				 */
				static void Copy( const TextComponent& src, TextComponent& dest, Error& error, Map* parentMap = NULL )
				{
					AssemblyComponent::Copy( src, dest );

					delete dest.m_regularLabel;

					dest.m_regularLabel = src.m_regularLabel->Clone( error, parentMap );
					if(error)
					{
						return;
					}

					dest.m_textComponentDefinition = src.m_textComponentDefinition;
				}

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				RegularLabel* m_regularLabel;
				TextComponentDefinition* m_textComponentDefinition;
		};

		/**
		 * Class that holds an array of derived type TextComponent object pointers.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API TextComponents : public LinkedList < TextComponent >
		{
			public:

				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				TextComponents()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~TextComponents()
				{
				}
		};

		///////////////////////////////////////////////////////////////////////////////
		// SymbolComponent
		///////////////////////////////////////////////////////////////////////////////

		/**
		 * Symbol building block of the assembly label. It's a single symbol identified 
		 * by the source (may be a font name) and identifier (possibly a code point). 
		 * Those values are only passed through the engine, not being used by it. 
		 * To the placement engine all the symbol component is, is a rectangle, which its
		 * bounding box. This is the most important property to set on the symbol component
		 * definition.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API SymbolComponent : public AssemblyComponent
		{
			public:
				/**
				 * Constructor.
				 * @reqid 001.0063
				 */
				SymbolComponent( SymbolComponentDefinition& scDef, Error& error);

				/**
				 * Destructor.
				 * @reqid 001.0063
				 */
				~SymbolComponent();

				/**
				 * @see AssemblyComponent::GetType
				 * @return - component type
				 * @reqid 001.0063
				 */
				ComponentType GetType() const
				{
					return ComponentType_Symbol;
				}

				/**
				 * Get the absolute location of the Symbol Component relative to the entire map.
				 * @return the absolute location of the Symbol Component
				 * @reqid 001.0063
				 */
				const Point* GetAbsoluteLocation() const
				{
					return m_enclosingRectangle.GetLowerLeft();
				}

				/**
				 * Set the absolute position of the Symbol Component relative to the entire map.
				 * This is computed by the LabelBuilder and then stored in the SymbolComponent
				 * via this method. This should NOT be set by the user, as repositioning will 
				 * take place during the text placement process and the initial position will
				 * be discarded anyway. Client code should read this value to get to know the 
				 * final placement position when results are returned.
				 * @param position - [out] the absolute position.
				 * @see GetAbsoluteLocation
				 * @reqid 001.0063
				 */
				void SetAbsoluteLocation( const Point& position );

				/**
				* Moves the component
				* @param dx - amount to move horizontally
				* @param dy - amount to move vertically
				* @reqid 001.0072
				*/
				void Translate( float dx, float dy )
				{
					m_enclosingRectangle.Translate( dx, dy );
				}

				/**
				* Rotate the label's parts to absolute angle
				* @param anchorPoint	anchor point about which to rotate
				* @param angle - absolute angle to rotate to
				* @reqid 001.0072
				*/
				void RotateToAngle(const Point &anchorPoint, float angle);

				/**
				* Rotate self about the specified origin.
				* @param anchorPoint - point about which to rotate
				* @param sin_t - sine of angle by which to rotate
				* @param cos_t - cosine of angle by which to rotate
				* @reqid 001.0063
				*/
				void Rotate( const Point &anchorPoint, float sin_t, float cos_t );

				/** Sets the absolute position.
				 *	@param x	the X coordinate of the absolute position
				 *	@param y	the Y coordinate of the absolute position
				 *	@see GetAbsolutePosition
				 *	@reqid 001.0063
				 */
				void SetAbsoluteLocation( float x, float y );

				/** Get the associated SymbolComponentDefinition.
				 *	@return the associated SymbolComponentDefinition
				 *	@reqid 001.063
				 */
				SymbolComponentDefinition* GetSymbolComponentDefinition() const
				{
					return m_symbolComponentDefinition;
				}

				/** Set the associated SymbolComponentDefinition.
				 * @see	GetSymbolComponentDefinition
				 * @reqid 001.063
				 */
				void SetSymbolComponentDefinition(SymbolComponentDefinition& symbolComponentDefinition )
				{
					m_symbolComponentDefinition = &symbolComponentDefinition;
				}

				/**
				 * Get the assembly component definition (whatever derived type it really is).
				 * @return - the definition as an abstract base class pointer
				 * @reqid 001.0063
				 */
				AssemblyComponentDefinition* GetAssemblyComponentDefinition()
				{
					return m_symbolComponentDefinition;
				}

				/**
				* Get the value of the hidden flag.
				* @return true if hidden; false if shown
				* @reqid 006.0014
				*/
				virtual bool IsHidden() const
				{
					return m_isHidden;
				}

				/**
				* Hide the symbol component.
				* @reqid 006.0014
				*/
				void Hide()
				{
					m_isHidden = true;
				}

				/**
				* Unhide the symbol component.
				* @reqid 006.0014
				*/
				void Unhide()
				{
					m_isHidden = false;
				}

				/**
				* Gets the height of the symbol component
				* @return height
				* @reqid TODO
				*/
				float GetHeight() const;

				/**
				* Gets the width of the symbol component
				* @return width
				* @reqid TODO
				*/
				float GetWidth() const;

				/**
				 * Create a deep copy of this instance
				 * @return the copy
				 * @param error error information
				 * @param parentMap (optional) a new parent map for a cloned object
				 * @reqid 001.0063
				 */
				virtual SymbolComponent* Clone( Error& error, Map* parentMap = NULL ) const
				{
					SymbolComponent* sc = new SymbolComponent( *m_symbolComponentDefinition, error );
					if (sc == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					if (error)
					{
						delete sc;
						sc = NULL;
					}
					else
					{
						Copy( *this, *sc, error, parentMap );
						if (error)
						{
							delete sc;
							sc = NULL;
						}
					}

					return sc;
				}

				/**
				* Get the enclosing rectangle of the symbol. 
				* @param angle - angle at which the symbol component is rotated.
				* @return the address of the enclosing rectangle
				* @reqid 001.0065
				*/
				Rectangle* GetEnclosingRectangle( float angle );

				/**
				* Get the enclosing rectangle of the symbol at its current angle. 
				* @return the address of the enclosing rectangle
				* @reqid 001.0065
				*/
				const Rectangle* GetEnclosingRectangle() const;

				/**
				* Get the enclosing rectangle angle of the symbol. 
				* @return the angle of the enclosing rectangle
				* @reqid 001.0065
				*/
				float GetEnclosingRectangleAngle() const
				{
					return m_enclosingRectangleAngle;
				}

				/**
				* Flips the SymbolComponent 180 degrees around its LowerLeft.
				* @reqid 006.0080
				*/
				void Flip();

				/**
				 * Get the bounding box of the component.
				 * @return - the bounding box
				 * @reqid 007.0049
				 */
				UprightRectangle GetBoundingBox()
				{
					UprightRectangle bbox;
					GetEnclosingRectangle()->GetBoundingBox( bbox );
					return bbox;
				}

				/**
				 * This method should NOT be used by the client application, it's only 
				 * meant for internal engine use. To set the bounding box of the symbol use
				 * the associated SymbolComponentDefinition::SetRelativeBoundingBox.
				 * @param enclosingRectangle -
				 * @return -
				 * @reqid 007.0049
				 */
				void SetEnclosingRectangle( const Rectangle& enclosingRectangle )
				{
					m_enclosingRectangle = enclosingRectangle;
					m_enclosingRectangleAngle = enclosingRectangle.GetAngleSineCosine();
				}

				/**
				 * Compare if the two symbol components are 'equal', i.e. visually resulting in the same
				 * text being created.
				 * @param otherComponent - The component to compare this instance to.
				 * @return - True if components are identical, false otherwise.
				 * @reqid 001.0063
				 */
				virtual bool IsEqualTo( const SymbolComponent& otherComponent ) const;

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


				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static SymbolComponent* DeserializeNew( IDeserializer& deserializer, Error& error );
			protected:
				SymbolComponent(){};
#endif
			protected:


				/**
				 * Make deep copy of one instance's members to another
				 * @param src
				 * @param dest
				 * @param error error information
				 * @param parentMap a new parent map for cloned object
				 * @reqid 001.0063
				 */
				static void Copy( const SymbolComponent& src, SymbolComponent& dest, Error& error, Map* )
				{
					AssemblyComponent::Copy( src, dest );

					dest.m_enclosingRectangle = src.m_enclosingRectangle;
					dest.m_enclosingRectangleAngle = src.m_enclosingRectangleAngle;
					dest.m_isHidden = src.m_isHidden;
				}

            private:
				/**
				* Populates the enclosing rectangle of the symbol.
				* @reqid 001.0065
				*/
				void GenerateEnclosingRectangle();

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				SymbolComponentDefinition* m_symbolComponentDefinition;
				Rectangle m_enclosingRectangle;
				float m_enclosingRectangleAngle;
				bool m_isHidden;
		};

		/**
		 * Class that holds an array of derived type SymbolComponent object pointers.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API SymbolComponents : public LinkedList < SymbolComponent >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				SymbolComponents()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~SymbolComponents()
				{
				}
		};

		///////////////////////////////////////////////////////////////////////////////
		// LeaderAnchor
		///////////////////////////////////////////////////////////////////////////////

		/**
		 * This is an instantiation of the leader anchor definition inside the assembly
		 * label itself.
		 */
		class LABELINGENGINE_API LeaderAnchor
		{
		public:
			LeaderAnchor()
			{
			}
			~LeaderAnchor()
			{
			}

			/**
			* Gets MinAngle
			* @return MinAngle
			* @reqid 001.0063
			*/
			float GetMinAngle() const 
			{ 
				return m_minAngle; 
			}

			/**
			* Sets MinAngle
			* @param val MinAngle
			* @reqid 001.0063
			*/
			void SetMinAngle(float val) 
			{ 
				m_minAngle = val; 
			}

			/**
			* Gets MaxAngle
			* @return MaxAngle
			* @reqid 001.0063
			*/
			float GetMaxAngle() const 
			{ 
				return m_maxAngle; 
			}

			/**
			* Sets MaxAngle
			* @param val MaxAngle
			* @reqid 001.0063
			*/
			void SetMaxAngle(float val) 
			{ 
				m_maxAngle = val; 
			}

			float GetX()
			{
				return m_anchorPoint.GetX();
			}

			/**
			 * Gets X
			 * @return - X coordinate
			 * @reqid 001.0063
			 */
			float GetX() const 
			{ 
				return m_anchorPoint.GetX(); 
			}

			/**
			 * Sets X
			 * @param val - X coordinate
			 * @reqid 001.0063
			 */
			void SetX(float val) 
			{ 
				m_anchorPoint.SetX( val );
			}


			/**
			 * Gets Y
			 * @return - Y coordinate
			 * @reqid 001.0063
			 */
			float GetY() const 
			{ 
				return m_anchorPoint.GetY(); 
			}

			/**
			 * Sets Y
			 * @param val - Y coordinate
			 * @reqid 001.0063
			 */
			void SetY(float val) 
			{ 
				m_anchorPoint.SetY( val );
			}


			/**
			 * Rotate the anchor point about.
			 * @param origin - Origin of rotation.
			 * @param sin_t - Sine value of the rotation angle.
			 * @param cos_t - Cosine value of the rotation angle
			 * @reqid 001.0063
			 */
			void Rotate(const Point& origin, float sin_t, float cos_t)
			{
				m_anchorPoint.Rotate( origin, sin_t, cos_t );
			}

			/**
			 * Shift by specified amounts in x and y directions.
			 * @param	dx		the x coordinate shift
			 * @param	dy		the y coordinate shift
			 * @reqid 001.0063
			 */
			void Translate(float dx, float dy)
			{
				m_anchorPoint.Translate( dx, dy );
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
			float m_minAngle;
			float m_maxAngle;
			Point m_anchorPoint;
		};

		///////////////////////////////////////////////////////////////////////////////
		// AssemblyLabel
		///////////////////////////////////////////////////////////////////////////////

		/**
		 * An assembly label. It forms an instance of the label. It requires 
		 * an AssemblyDefinition, the "recipe",  assigned to it. The collections
		 * of components and anchors are instances of the corresponding definition
		 * collections.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API AssemblyLabel : public Label
		{
			friend class AssemblyLabelBuilder;

			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_IncorrectJustification,
					ErrorCode_IndexOutOfRange,
					ErrorCode_NullPtr,
					ErrorCode_RelativePositioningComponentMissing,
					ErrorCode_StretchingComponentMissing,
					ErrorCode_ComponentActivatorMissing,
				};

				/**
				 * Default constructor. Create the text and symbol components.
				 * @param error		an error object to be populated
				 * @reqid 001.0063
				 */
				AssemblyLabel(Error& error) :
					Label( Label::LabelType_Assembly, error ),
					m_assemblyOrigin( 0.0f, 0.0f ),
					m_anchors( NULL ), m_anchorsCount( 0 ), m_isRestructured( false ),
					m_assemblyLabelDefinition( NULL )
				{
					if (error)
					{
						return;
					}

					m_textComponents = new TextComponents();

					if (m_textComponents == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
						return;
					}

					m_symbolComponents = new SymbolComponents();

					if (m_symbolComponents == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
						return;
					}

					m_inactiveTextComponents = new TextComponents();
					if ( m_inactiveTextComponents == NULL )
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
						return;
					}

					m_inactiveSymbolComponents = new SymbolComponents();
					if ( m_inactiveSymbolComponents == NULL )
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
						return;
					}

					m_angle=0.0f;
				}

				/**
				 * Destructor. Delete the text and symbol components.
				 * @reqid 001.0063
				 */
				~AssemblyLabel()
				{
					delete m_textComponents;
					delete m_symbolComponents;
					delete[] m_anchors;
					delete m_inactiveTextComponents;
					delete m_inactiveSymbolComponents;
				}

				/** Get the collection of TextComponent objects of the assembly.
				 * @return the text component collection
				 * @reqid 001.0063
				 */
				TextComponents* GetTextComponents() const
				{
					return m_textComponents;
				}

				/** Get the collection of SymbolComponent objects of the assembly.
				 * @return the symbol component collection
				 * @reqid 001.0063
				 */
				SymbolComponents* GetSymbolComponents() const
				{
					return m_symbolComponents;
				}

				/** Get the InactiveTextComponents. The AssemblyLabel holds two lists, of active and inactive components (text and symbol)
				 *  which can become activated provided certain conditions are met in the ComponentActivator
				 * @return the inactive Text Components
				 * @reqid 006.0080
				 */
				TextComponents* GetInactiveTextComponents() const
				{
					return m_inactiveTextComponents;
				}

				/** Get the assembly label definition.
				*	This holds the definition used for constructing the assembly label.
				*	@return AssemblyLabelDefinition
				*	@reqid 001.0063
				*/
				AssemblyLabelDefinition* GetAssemblyLabelDefinition() const
				{
					return m_assemblyLabelDefinition;
				}

				/** Set the assembly label definition. The ownership is NOT taken.
				 * @reqid 001.0063
				 * @see GetAssemblyLabelDefinition
				 */
				void SetAssemblyLabelDefinition(AssemblyLabelDefinition& assemblyLabelDefinition )
				{
					m_assemblyLabelDefinition = &assemblyLabelDefinition;
				}

				/* 
				* Get the label's absolute origin, with respect to which all components are defined.
				* @return the transformed origin of the assembly.
				* &reqid 001.0072
				*/
				const Point* GetAssemblyOrigin() const
				{
					return &m_assemblyOrigin;
				}
				 
				/*  @see abstract method: Label::PlaceAtOrigin
				 *	@param stackSpacking - the distance between two stack levels
				 *  @param justification - how to align the lines of a label. If the value text component's
				    justification is Feature_Justify, then use the input justification's value. The
					  input justification's value cannot be Feature_Justify.
				 *	@param error - [out] the error
				 *  @reqid 001.0063
				 */
				virtual void PlaceAtOrigin(float stackSpacing,PlacementRule::LabelJustification justification,Error& error);

				/** Get the angle at which the label is oriented.
				* Symbol components are placed at this angle.
				* @see Label::GetAngle
				* @reqid 001.063
				*/
				virtual float GetAngle() const
				{
					return m_angle;
				}

				/* @see GetAngle
				* @reqid 001.063
				*/
				void SetAngle(float angle)
				{
					m_angle=angle;
				}

				/**
				 * Get a component by name. It may be a symbol or text component. First matching component
				 * is returned, symbol components are searched first.
				 * @param name - name of the component wanted
				 * @param component - [out] the component or NULL if not found
				 * @reqid 007.0049
				 */
				void GetComponentByDefinitionName( const SharedString& name, AssemblyComponent** component ) const;

				/**
				 * Get component definition by name. It may be a symbol or text component. First matching 
				 * component definition is returned, symbol components definitions are searched first.
				 * @param name - name of the component wanted
				 * @param component - [out] the component or NULL if not found
				 * @reqid 001.0086
				 */
				void GetComponentDefinitionByName( const SharedString& name, AssemblyComponentDefinition** component ) const;

				/**
				* Unhide all elements of the label.
				* @reqid 006.0014
				*/
				virtual void Unhide();

				/**
				 * Create a copy of the current object.
				 * @param error error information
				 * @param parentMap (optional) a new parent map for a cloned object
				 * @return the copy
				 * @reqid 001.0063
				 */
				virtual AssemblyLabel* Clone( Error& error, Map *parentMap = NULL ) const
				{
					AssemblyLabel* al = new AssemblyLabel(error);
					if (al == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					if (error)
					{
						delete al;
						al = NULL;
					}
					else
					{
						Copy( *this, *al, error, parentMap );
						if ( error)
						{
							delete al;
							al = NULL;
						}
					}
					return al;
				}

				/**
				 * Moves the label's parts by (dx,dy)
				 * @param dx amount to move horizontally
				 * @param dy amount to move vertically
				 * @reqid 001.0072
				 */
				virtual void Translate(float dx,float dy);

				/**
				* Rotate the label's parts by angle
				* @param anchorPoint	anchor point about which to rotate
				* @param angle amount to rotate
				* @reqid 003.0019
				*/
				virtual void RotateToAngle(const Point &anchorPoint, float angle);

				/**
				* Rotate self about the specified origin.
				* @param anchorPoint - point about which to rotate
				* @param angleDiff - rotation angle, needed to maintain internal angle
				*                    without expensive calculations
				* @param sin_t - sine of angle by which to rotate
				* @param cos_t - cosine of angle by which to rotate
				* @reqid 001.0063
				*/
				void Rotate( const Point &anchorPoint, float angleDiff, float sin_t, float cos_t );

				/**
				 * @see Label::CheckOverlaps
				 * @param map 
				 * @param feature 
				 * @param placementRule 
				 * @param overlapSummary 
				 * @param error
				 * @reqid 001.0065
				 */
				virtual void CheckOverlaps(
					Map *map, Feature *feature, PlacementRule *placementRule, PlacementState* placementState,
					ObstacleOverlapSummary **overlapSummary, Error &error);

				/**
				* @copydoc Label::CheckOverlaps(Map*,Feature*,PlacementRule*,IObstacleAnalyzer*,bool,ObstacleOverlapSummary**, Error&)
				* @reqid 001.0065
				*/
				void CheckOverlaps(
					Map *map, Feature *feature, PlacementRule *placementRule, PlacementState* placementState,
					IObstacleAnalyzer* obstacleAnalyzer, bool analyzeAllObjects, ObstacleOverlapSummary **overlapSummary, Error &error);

				/* 
				 * @see Label::RegisterWithSpatialIndex
				 * @param spatialIndex - index to which to add the label parts.
				 * @error - error object
				 * @reqid 001.0064
				 */
				virtual void RegisterWithSpatialIndex(ISpatialIndex *spatialIndex, Error &error);

				/* 
				 * @see Label::GetTopXRange
				 * @reqid 002.0036
				 */
				virtual void GetTopXRange(float &labelLeftX,float &labelRightX) const;

				/* 
				 * @see Label::GetBottomXRange
				 * @reqid 002.0036
				 */
				virtual void GetBottomXRange(float &labelLeftX,float &labelRightX) const;

				/* 
				 * @see Label::GetRightYRange
				 * @reqid 003.0029
				 */
				virtual void GetRightYRange(float &labelBottomY,float &labelTopY) const;

				/* 
				 * @see Label::GetLeftYRange
				 * @reqid 002.0036
				 */
				virtual void GetLeftYRange(float &labelBottomY,float &labelTopY) const;

				/**
				* @see Label::GetTextBoxes
				* @param boxes
				* @param error
				* @reqid 004.0032
				*/
				virtual void GetTextBoxes( LinkedList<Rectangle> &boxes, Error& error ) const;

				/* 
				 * Justify the label according to the justification parameter
				 * @param justification - justification to perform
				 * @reqid 006.0046
				 */
				virtual void Justify(PlacementRule::LabelJustification justification);

				/**
				* Report whether every text object at this stack level is hidden.
				* @return true if hidden; false if shown
				 * @reqid 006.0014
				 */
				virtual bool IsHidden() const;

				/**
				* Gets AnchorsCount
				* @return AnchorsCount
				* @reqid 001.0063
				*/
				int GetAnchorsCount() const 
				{ 
					return m_anchorsCount; 
				}

				/**
				 * Get the i-th leader anchor
				 * @param index - index of the anchor to retrieve
				 * @param anchor - [out] the anchor requested or NULL if index 
				 *                 out of range
				 * @param error - error information, possibly ErrorCode_IndexOutOfRange
				 * @reqid 005.0058
				 */
				void GetAnchorByIndex( int index, const LeaderAnchor** anchor, Error& error ) const
				{
					*anchor = NULL;
					if ( index < 0 || index >= m_anchorsCount )
					{
						SET_ERROR_NOMSG( error, ErrorCode_IndexOutOfRange );
						return;
					}

					*anchor = &(m_anchors[ index ]);
				}

				enum DependencyType
				{
					DependencyType_Positioning,
					DependencyType_Stretching
				};

				typedef Graph<AssemblyComponent,DependencyType> ComponentDependencyGraph;

				
				/**
				 * Builds a component dependency graph for this label. Graph has dual structure:
				 * It reflects two types of dependencies: stretching and positioning dependencies.
				 * Each node in the list of nodes can have two types of edge connected to it - 
				 * - DependencyType_Positioning edge leads to an element, which relies on this node
				 *   when it is being positioned.
				 * - DependencyType_Stretching edge leads to an element, ON which the current
				 *   element depends for stretching.
				 * This allows the client code to easily process the construction of a label.
				 * @param depsGraph - [out] The graph to be populated.
				 * @param error - [out] Error info.
				 * @reqid 007.0049
				 */
				void BuildComponentDependencyGraph( ComponentDependencyGraph* depsGraph, Error& error );

				/**
				 * @see Label::Validate
				 * @reqid 001.0086
				 */
				void Validate( bool* valid, ErrorDetails** validationErrors, Error& error ) const;

				/**
				 * Restructures the AssemblyLabel (Text and Symbol components), transferring inactive components to the Activator and active ones
				 * to the AssemblyLabel. The active and inactive components are defined in the ComponentActivator.
				 * The ComponentActivator must be defined in the AssemblyLabedDefinition.
				 *
				 * @param error - [out] error info
				 * @reqid 006.0080
				 */
				void Restructure(Error &error);

				/**
				 * @see Label::IsEqualTo
				 * @param otherLabel - The label to compare this instance to.
				 * @return - True if labels are identical, false otherwise.
				 * @reqid 001.0063
				 */
				virtual bool IsEqualTo( const Label& otherLabel ) const;

				/**
				 * Compose the full text of the label.
				 * @param labelText	[out] A reference to a string to be populated with the text
				 * @param error [out] error information
				 * @reqid 001.0063
				 */
				void GetFullText( String& fullText, Error& error ) const;


			private:

				/**
				 * Tests if names used for assembly components are unique across the label.
				 * @param valid - [out] True if no name duplication has been found. False otherwise.
				 * @param validationErrors - [out] Validation problem details are appended here.
				 * @param error - [out] Error info
				 * @reqid 001.0086
				 */
				void TestComponentNameUniqueness( bool* valid, ErrorDetails** validationErrors, Error& error ) const;

				/**
				 * Check if the scaling components are properly set and available.
				 * @param valid - [out] True if scaling components lists are OK. False otherwise.
				 * @param validationErrors - [out] Validation problem details are appended here.
				 * @param error - [out] Error info
				 * @reqid 001.0086
				 */
				void CheckSymbolScalingComponentsLists( bool* valid, ErrorDetails** validationErrors, Error& error ) const;

				/**
				 * Test of the relative positioning dependencies are not wrong (excluding cyclic dependencies,
				 * those are expensive to check and validated by LabelBuilder itself).
				 * @param valid - [out] True if relative positioning components lists are OK. False otherwise.
				 * @param validationErrors - [out] Validation problem details are appended here.
				 * @param error - [out] Error info
				 * @reqid 001.0086
				 */
				void TestRelativePositioning( bool* valid, ErrorDetails** validationErrors, Error& error ) const;

				/**
				 * Verify if all the component definitions used by components are owned by this label.
				 * @param valid - [out] True if all components use definitions owned by this label. 
				 *                False otherwise.
				 * @param validationErrors - [out] Validation problem details are appended here.
				 * @param error - [out] Error info
				 * @reqid 001.0086
				 */
				void VerifyDefinitionsOwnership( bool* valid, ErrorDetails** validationErrors, Error& error ) const;

				/**
				 * Set the number of anchor this label should have.
				 * @param count - Number of anchors to set.
				 * @param error - [out] Error info.
				 * @reqid 007.0049
				 */
				void SetAnchorsCount( int count, Error& error )
				{
					delete[] m_anchors;

					m_anchors = new LeaderAnchor[ count ];

					if ( m_anchors == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
						return;
					}

					m_anchorsCount = count;
				}
				
				/**
				 * Set the parameters of an anchor specified by its index within the 
				 * anchors collection of the label.
				 * @param index - Index of the anchor to set.
				 * @param anchor - Position of the anchor.
				 * @param minAngle - Min angle of the anchor.
				 * @param maxAngle - Max angle of the anchor.
				 * @param error - [out] Error info.
				 * @reqid 007.0049
				 */
				void SetAnchorByIndex( int index, const Point& anchor, float minAngle, 
					float maxAngle, Error& error )
				{
					if ( index < 0 || index >= m_anchorsCount )
					{
						SET_ERROR_NOMSG( error, ErrorCode_IndexOutOfRange );
						return;
					}

					m_anchors[index].SetX( anchor.GetX() );
					m_anchors[index].SetY( anchor.GetY() );
					m_anchors[index].SetMaxAngle( maxAngle );
					m_anchors[index].SetMinAngle( minAngle );
				}

				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @param error error information
				* @param parentMap (optional) a new parent map for a cloned object
				* @reqid 001.0063
				*/
				static void Copy( const AssemblyLabel& src, AssemblyLabel& dest, Error& error, 
					Map* parentMap = NULL );

#ifdef S11N
			protected:
				/** 
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 */
				virtual void _Deserialize( IDeserializer& deserializer, Error& error );

				/**
				 * Serialization of the child class.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			private:
				void ReattachDefinitionToComponent( AssemblyComponent& component, Error& error );
#endif


				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				float m_angle;
				TextComponents* m_textComponents;
				SymbolComponents* m_symbolComponents;
				AssemblyLabelDefinition* m_assemblyLabelDefinition;				
				Point m_assemblyOrigin;
				LeaderAnchor* m_anchors;
				int m_anchorsCount;

				TextComponents* m_inactiveTextComponents;
				SymbolComponents* m_inactiveSymbolComponents;
				bool m_isRestructured;
		};

		/**
		 * Class that defines a label class.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API LabelClass
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
				 * A collection of the label classes defined for the map.
				 * Contains a private linked list member to enable it to expose
				 * traversal, e.g., GetFirstElement and getNextElement, methods.
				 * GetLabelClass inserts a new label class (if the requested one
				 * is not found) into the string key tree and adds it to the
				 * private linked list as well.  The traversal methods are
				 * delegated to the provate linked list.
				 * @reqid 001.0063
				 */
				class LABELINGENGINE_API Collection : public StringKeyedTree < LabelClass >
				{
					public:
						/**
						 * Possible error codes
						 * @reqid 001.0063
						 */
						enum ErrorCode
						{
							ErrorCode_AllocationFailure = 1,
							ErrorCode_LabelClassAlreadyExists
						};

						/**
						 * Default constructor implemented to avoid compiler generated object code.
						 * @reqid 001.0063
						 */
						Collection()
						{
						}

						/**
						 * Default destructor implemented to avoid compiler generated object code.
						 * @reqid 001.0063
						 */
						~Collection()
						{
						}

						/**
						 * Get the specified label class from the cache if present; else create and cache.
						 * @param name				the name of the requested label class
						 * @param labelClass	[out] the requested label class
						 * @param error				an Error object to be populated
						 * @reqid 001.0063
						 */
						void GetLabelClass( const TCHAR* name, LabelClass** labelClass, Error& error );

						/**
						 * Add specified label class to the collection.
						 * Takes ownership. Returns ErrorCode_LabelClassAlreadyExists if label class
						 * with given name already exists.
						 * @param labelClass label class to add
						 * @param error Error object to be populated
						 * @reqid 001.0076
						 */
						void AddLabelClass( LabelClass* labelClass, Error& error );

						/**
						* Get the first label class in the collection.
						* @return the address of the first label class
						* @reqid 001.0063
						*/
						LabelClass* GetFirstElement()
						{
							return m_labelClassList.GetFirstElement();
						}

						/**
						* Get the next label class in the collection.
						* @return the address of the next label class
						* @reqid 001.0063
						*/
						LabelClass* GetNextElement()
						{
							return m_labelClassList.GetNextElement();
						}
				private:
					class LABELINGENGINE_API LabelClassList : public LinkedList < LabelClass >
					{
						// The contained linked list should not take ownership.
						// The string keyed collection takes ownership of the elements.
					public:
						LabelClassList() : LinkedList < LabelClass >( false )
						{
						}
					};

					LabelClassList m_labelClassList;
				};

			private:
				/**
				 * Default constructor.
				 * @param error	an Error object
				 * @reqid 001.0063
				 */
				LabelClass(Error& error)
					:	m_priority( 12 ),
						m_placementRule( NULL ),
						m_labelAnchor( NULL ),
                        m_minScale(0), m_maxScale(FLT_MAX),
                        m_maskEnabled(false)
				{
					m_name.AssignString(_T( "" ), error);
				}

				/**
				 * Build a label class with the given name.
				 * @param name name of the new rule
				 * @param error	an Error object
				 * @reqid 001.0063
				 */
				LabelClass( const TCHAR* name, Error& error )
					:	m_placementRule( NULL ), 
						m_name( name, error ),
						m_labelAnchor( NULL ),
                        m_minScale(0), m_maxScale(FLT_MAX),
                        m_maskEnabled(false)
				{
				}

			public:
				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~LabelClass();

				/**
				* Get the priority for placing labels associated with this label class.
				* A lower value indicates higher priority.
				* @return the priority level of the label class
				* @reqid 001.0063
				*/
				int GetPriority() const
				{
					return m_priority;
				}

				/**
				* Set the priority for placing labels associated with this label class.
				* @see GetPriority
				* @reqid 001.0063
				*/
				void SetPriority( int priority )
				{
					m_priority = priority;
				}

				/**
				* Get the name of the label class.
				* @return the name of the label class as a shared string reference
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
				 * Get the placement rule used for placement of labels associated with this label class.
				 * @return the address of the placement rule of the label class (NULL if none provided)
				 * @reqid 001.0063
				 */
				PlacementRule* GetPlacementRule() const
				{
					return m_placementRule;
				}

				/**
				 * Assign a placement rule. Ownership over the placement rule is NOT taken.
				 * Allocator of the placement rule has to handle the deallocation.
				 * @reqid 001.0063
				 */
				void SetPlacementRule( PlacementRule* placementRule )
				{
					m_placementRule = placementRule;
				}

				/**
				* Gets MinScale
				* @return - MinScale
				*/
				float GetMinScale() const 
				{ 
					return m_minScale; 
				}

				/**
				* Sets MinScale
				* @param val - MinScale
				*/
				void SetMinScale(float val) 
				{ 
					m_minScale = val; 
				}

				/**
				* Gets MaxScale
				* @return - MaxScale
				*/
				float GetMaxScale() const 
				{ 
					return m_maxScale; 
				}

				/**
				* Sets MaxScale
				* @param val - MaxScale
				*/
				void SetMaxScale(float val) 
				{ 
					m_maxScale = val; 
				}

				/**
				* Gets MaskingEnabled
				* @return - MaskingEnabled
				*/
				bool GetMaskEnabled() const 
				{ 
					return m_maskEnabled; 
				}

				/**
				* Sets MaskingEnabled
				* @param val - MaskingEnabled
				*/
				void SetMaskEnabled(bool val) 
				{ 
					m_maskEnabled = val; 
				}

				/**
				 * Get the label anchor of the label class.
				 * It is used for Vertical Alignment purposes.
				 * @return the address of the label anchor of the label class (NULL if none provided)
				 * @reqid 001.0063
				 */
				LabelAnchor *GetLabelAnchor() const
				{
					return m_labelAnchor;
				}

				/** Initialize the AssemblyLineLabelAnchor.
				 *  Set it to true middle placement (middle on line feature anchoring, entire label).
				 *  If LabelClass has the LabelAnchor already set, it is deleted it and reinitialized.
				 *
				 *	@param		error		[in,out]
				 * 
				 *	@return		<UL>
				 *					<LI>!NULL the initialized AssemblyLineLabelAnchor pointer</LI>
				 *					<LI>NULL error</LI>
				 *				</UL>
				 *
				 * @reqid 001.0063
				 */
				AssemblyLineLabelAnchor* InitializeAssemblyLineLabelAnchor(Error &error);

				/** Initizlizes the RegularLineLabelAnchor. Sets it to true middle placement (middle on line feature anchoring, entire label).
				 *  If LabelClass has the LabelAnchor alread set, deletes it and reinitializes it.
				 *
				 *	@param		error		[in,out]
				 * 
				 *	@return		<UL>
				 *					<LI>!NULL the initialized RegularLineLabelAnchor pointer</LI>
				 *					<LI>NULL error</LI>
				 *				</UL>
				 *
				 * @reqid 001.0063
				 */
				RegularLineLabelAnchor* InitializeRegularLineLabelAnchor(Error &error);

				/** Initialize the AssemblyPointLabelAnchor.
				 *  If LabelClass has the LabelAnchor already set, it is deleted it and reinitialized.
				 *
				 *	@param		error		[in,out]
				 * 
				 *	@return		<UL>
				 *					<LI>!NULL the initialized AssemblyPointLabelAnchor pointer</LI>
				 *					<LI>NULL error</LI>
				 *				</UL>
				 *
				 * @reqid 001.0063
				 */
				AssemblyPointLabelAnchor* InitializeAssemblyPointLabelAnchor(Error &error);

				/** Initializes the RegularPointLabelAnchor.
				 *  If LabelClass has the LabelAnchor alread set, deletes it and reinitializes it.
				 *
				 *	@param		error		[in,out]
				 * 
				 *	@return		<UL>
				 *					<LI>!NULL the initialized RegularPointLabelAnchor pointer</LI>
				 *					<LI>NULL error</LI>
				 *				</UL>
				 *
				 * @reqid 001.0063
				 */
				RegularPointLabelAnchor* InitializeRegularPointLabelAnchor(Error &error);

				/**
				 * Validate the label class.
				 * @param validationErrors - [out] Validation issues are reported here.
				 * @param featType - feature type to proceed with some feature-type-dependent validations
				 * @param error[out] - Other critical errors which may occur during 
				 *                the validation process are reported here. They would NOT
				 *                be caused by invalid input data.
				 * @reqid 001.0086
				 */
				virtual void Validate( bool* valid, ErrorDetails** validationErrors, FeatureType featType, Error& error ) const;

				/**
				* Creates copy of self.
				* @param error an error object to be populated
				* @param parentMap (optional) new parent map for a cloned object
				* @return copy of self
				* @reqid 001.0076
				*/
				virtual LabelClass* Clone( Error& error, Map* parentMap = NULL ) const;

				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @param error an error object to be populated
				* @param parentMap (optional) new parent map for a cloned object
				* @reqid 001.0076
				*/
				static void Copy( const LabelClass& src, LabelClass& dest, Error &error, Map* parentMap = NULL );

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


				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static LabelClass* DeserializeNew( IDeserializer& deserializer, Error& error );
#endif

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				SharedString m_name;
				int m_priority; //doesn't have to be limited to 12
				PlacementRule* m_placementRule;
				LabelAnchor* m_labelAnchor;
				float m_minScale;
				float m_maxScale;
				bool m_maskEnabled;
		};

		/**
		 * Base abstract Class that defines a label anchoring. Defines how the label will be vertically anchored.
		 * @reqid 003.0019
		 */
		class LABELINGENGINE_API LabelAnchor
		{
			public:
				/**
				 * Possible error codes for the class and all its children
				 * @reqid 003.0019
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_NullArgument = 2,
					ErrorCode_UnrecognizedAnchoringConfiguration = 100,
					ErrorCode_SpecifiedTextObjectNotFound = 101,
					ErrorCode_SpecifiedStackLevelNotFound = 102,
					ErrorCode_SpecifiedTextComponentNotFound = 103,
					ErrorCode_SpecifiedSymbolComponentNotFound = 104,
					ErrorCode_ParameterValueOutOfRange = 105,
					ErrorCode_InvalidLabelType = 106
				};

				/**
				 * Type of label anchor (RTTI)
				 */
				enum LabelAnchorType
				{
					LabelAnchorType_Point,
					LabelAnchorType_Line,
					LabelAnchorType_AssemblyLine,
					LabelAnchorType_RegularLine,
					LabelAnchorType_AssemblyPoint,
					LabelAnchorType_RegularPoint,
				};

				/**
				 * Get the type of the implementation.
				 * @return - Type info.
				 * @reqid 003.0019
				 */
				virtual LabelAnchorType GetType() const = 0;

				/**
				 * Default constructor
				 * @reqid 003.0019
				 */
				LabelAnchor()
				{
				}

				/**
				 * Default destructor
				 * @reqid 003.0019
				 */
				virtual ~LabelAnchor()
				{}

				/**
				* Creates copy of self.
				* @param error an error object to be populated
				* @return copy of self
				* @reqid 001.0076
				*/
				virtual LabelAnchor* Clone( Error& error ) const = 0;

				/*
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @param error an error object to be populated
				* @reqid 001.0076
				*/
				//static void Copy( const LabelAnchor& src, LabelAnchor& dest, Error& error )
				//{
				//}

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


			/**
			 * Deserializes a new object from the deserializer.
			 * @param deserializer - The source.
			 * @param output - Deserialized object if no error occured. NULL otherwise.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			static LabelAnchor* DeserializeNew( IDeserializer& deserializer, Error& error );
		protected:
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const = 0;

			/**
			 * Called on the specific implementation to deserialize itself.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error ) = 0;

#endif


				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
		};


		class LABELINGENGINE_API PointLabelAnchor : public LabelAnchor
		{
		 public:

		/**
			 * Possible AnchorLocation around the label part's bounding box
			 * @reqid 002.0037
			 */
			enum AnchorLocation
			{
				AnchorLocation_MiddleMiddle = 0,
				AnchorLocation_LeftMiddle = 1,
				AnchorLocation_LeftTop = 2,
				AnchorLocation_TopMiddle = 3,
				AnchorLocation_TopRight = 4,
				AnchorLocation_RightMiddle = 5,
				AnchorLocation_RightBottom = 6,
				AnchorLocation_BottomMiddle = 7,
				AnchorLocation_LeftBottom = 8,
			};

			/**
			 * Default constructor. By default sets true middle placement. Middle of Label, Middle of line feature.
			 * @reqid 002.0037
			 */
			PointLabelAnchor(Error &error)
				: LabelAnchor(),
					m_anchorLocation(AnchorLocation_MiddleMiddle),
					m_stackLevelNumber(-1),
					m_textObjectName(_T(""), error)
			{
			}


			/**
			 * @see LabelAnchor::GetType
			 * @return -
			 * @reqid 003.0019
			 */
			virtual LabelAnchorType GetType() const
			{
				return LabelAnchorType_Point;
			}

			/**
			 * Default destructor
			 * @reqid 002.0037
			 */
			virtual ~PointLabelAnchor()
			{}

			/** Computes the Anchor offset for the Label's current location. By how much does the entire label need to be offset in order to achieve the configured effect.
			*	@param	label			Label to be offset
			*	@param  offset			[out]	the computed offset
			*	@param	error			[in, out] Error
			* 
			*	@reqid 003.0019
			*/
			virtual void ComputeAnchorOffset(const Label &label, Point *offset, Error &error ) = 0;

			/** Sets the anchor location relative to the LabelPart (entire Label, StackLevel, TextObject, SymbolComponent etc.). 
			*	@param	location AnchorLocation on labelparts bounding box
			* 
			*	@reqid 002.0037
			*/
			void SetLabelPartAnchor(const PointLabelAnchor::AnchorLocation &val)
			{
				m_anchorLocation = val;
			}

			/** Sets the TextObject name. This will be used to find the TextObject to be anchored on
			*
			*	@param	textObjectName
			*	@param	error				[in, out] Error
			* 
			*	@reqid 002.0037
			*/
			void SetTextObjectName(const TCHAR *textObjectName, Error &error)
			{
				m_textObjectName.AssignString(textObjectName, error);
			}

			/** Gets the TextObject name.
			*
			*	@return		TextObject name
			* 
			*	@reqid 002.0037
			*/
			const TCHAR* GetTextObjectName() const
			{
				return m_textObjectName.GetString();
			}

			/** Sets the StackLevel number. 
			*
			*	@param	val		value must be > 0
			* 
			*	@reqid 002.0037
			*/
			void SetStackLevelNumber(int val)
			{
				m_stackLevelNumber = val;
			}

			/** Gets the StackLevel number. 
			*
			*	@return	stackLevelNumber
			* 
			*	@reqid 002.0037
			*/
			int GetStackLevelNumber() const
			{
				return m_stackLevelNumber;
			}

			/**
			* Make deep copy of one instance's members to another
			* @param src source object
			* @param dest destination object
			* @param error an error object to be populated
			* @reqid 001.0076
			*/
			static void Copy( const PointLabelAnchor& src, PointLabelAnchor& dest, Error &error );

		 protected:
			
#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/** 
			 * @see LabelAnchor::_Deserialize
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif

			/** Computes the offset of a Label Part from the Label's location.
			*	@param	labelLowerLeft		the lower left of the entire label
			*	@param	labelPartLowerLeft	the lower left of the LabelPart
			*	@param  labelPartOffset		the offset of the labelPart after it has been shifted to the new position, based on user vertical alignment configuration
			*	@param	offset				[out]	the entire offset needed to move the entire label to a new location
			* 
			*	@reqid 002.0037
			*/
			void ComputeLabelPartOffsetFromLabel( const Point &labelLowerLeft, const Point &labelPartLowerLeft, const Point &labelPartOffset, Point *offset );

			/** Computes the offset of a LabelPart (represented by the lowerLeft parameter) from its current location
			*	@param	lowerLeft			the lower left of the LabelPart
			*	@param	height				the bounding box height of the LabelPart
			*	@param	length				the bounding box length of the LabelPart
			*	@param  angle					the angle of the LabelPart
			*	@param	offset				[out]	the offset needed to move the LabelPart to configured location 
			*	@param	error					[in, out] Error
			*	@reqid 002.0037
			*/
			void ComputeLabelPartOffset( const Point &lowerLeft, float height, float length, float angle, Point *offset, Error &error );

			AnchorLocation m_anchorLocation;
			int m_stackLevelNumber;
			SharedString m_textObjectName;		
		};

		/**
		 * Base class that defines a label anchoring for line features (Middle placement). Defines how the label will be vertically anchored.
		 * @reqid 003.0019
		 */
		 class LABELINGENGINE_API LineLabelAnchor : public LabelAnchor
		 {
		 public:

			/**
			 * Possible AnchorLocation used to specify whether the computations should be started relative to the Top or Bottom
			 * @reqid 003.0019
			 */
			enum AnchorLocation
			{
				AnchorLocation_Top = 1,
				AnchorLocation_Bottom = 2,
				AnchorLocation_Left = 3,
				AnchorLocation_Right = 4
			};

			/**
			 * Default constructor. By default sets true middle placement. Middle of Label, Middle of line feature.
			 * @reqid 003.0019
			 */
			LineLabelAnchor(Error &error)
				: LabelAnchor(),
					m_onLineAnchorLocation(AnchorLocation_Top),
					m_onLineDistanceFromAnchorLocation(50),
					m_onLabelPartAnchorLocation(AnchorLocation_Top),
					m_onLabelPartDistanceFromAnchorLocation(50),
					m_stackLevelNumber(-1),
					m_textObjectName(_T(""), error)
			{
			}

			/**
			 * Default destructor
			 * @reqid 003.0019
			 */
			virtual ~LineLabelAnchor()
			{}

			/**
			 * @see LabelAnchor::GetType
			 * @return -
			 * @reqid 003.0019
			 */
			virtual LabelAnchorType GetType() const
			{
				return LabelAnchorType_Line;
			}

			/** Computes the Anchor offset for the Label's current location. By how much does the entire label need to be offset in order to achieve the configured effect.
			*	@param	label			Label to be offset
			*	@param	lineWidth		The line feature width
			*	@param  offset			[out]	the computed offset
			*	@param  vertical		compute the offset vertically or horizontally
			*	@param	error			[in, out] Error
			* 
			*	@reqid 003.0019
			*/
			virtual void ComputeAnchorOffset(const Label &label, float lineWidth, Point *offset, bool vertical, Error &error ) = 0;

			/** Sets the location of the anchor relative to the line feature. User must specify whether the distance given is from the top or bottom of the feature.
			*	@param	location						AnchorLocation (Top or Bottom) of the line feature
			*	@param	percentDistanceFromLocation		The distance relative to the Top or Bottom of the line feature
			* 
			*	@reqid 003.0019
			*/
			void SetLineFeatureAnchor(const LineLabelAnchor::AnchorLocation &location, int percentDistanceFromLocation);

			/** Sets the location of the anchor relative to the LabelPart (entire Label, StackLevel, TextObject, SymbolComponent etc.). 
			*	User must specify whether the distance given is from the top or bottom of the LabelPart.
			*	@param	location						AnchorLocation (Top or Bottom) of the LabelPart
			*	@param	percentDistanceFromLocation		The distance relative to the Top or Bottom of the LabelPart
			* 
			*	@reqid 003.0019
			*/
			void SetLabelPartAnchor(const LineLabelAnchor::AnchorLocation &location, int percentDistanceFromLocation);

			/** Sets the TextObject name. This will be used to find the TextObject that will be positioned at a configured 
			*	location for Middle placement.
			*
			*	@param	textObjectName
			*	@param	error				[in, out] Error
			* 
			*	@reqid 003.0019
			*/
			void SetTextObjectName(const TCHAR *textObjectName, Error &error);

			/** Gets the TextObject name. This will be used to find the TextObject that will be positioned at a configured 
			*	location for Middle placement.
			*
			*	@return		TextObject name
			* 
			*	@reqid 003.0019
			*/
			const SharedString *GetTextObjectName() const;

			/** Sets the StackLevel number. This will be used to find the StackLevel that will be positioned at a configured 
			*	location for Middle placement. StackLevels are numbered from bottom - up.
			*
			*	@param	stackLevelNumber		value must be > 0
			* 
			*	@reqid 003.0019
			*/
			void SetStackLevelNumber(int stackLevelNumber);

			/** Gets the StackLevel number. 
			*
			*	@return	stackLevelNumber
			* 
			*	@reqid 003.0019
			*/
			int GetStackLevelNumber() const;

			/**
			* Make deep copy of one instance's members to another
			* @param src source object
			* @param dest destination object
			* @param error an error object to be populated
			* @reqid 001.0076
			*/
			static void Copy( const LineLabelAnchor& src, LineLabelAnchor& dest, Error &error );

			/**
			 * The line anchor is defined with respect to the 0-point being at one edge of the symbolized line or
			   the other. This method changes the offset of the line anchor such that the 0-point is the center line
			   of the polyline. The offset on the Above side of the line is positive and it is negative on the Below
			   side of the line
		     * @return the anchor offset with respect to the center of the line instead either edge of the line
			 * @reqid 003.0019
			 */
			float ComputeLineAnchorOffsetFromLineCenter() const;

			/** 
			* Getter for the anchor location for the label part.
			* @return anchor location
			* @reqid 003.0019
			*/
			AnchorLocation GetLabelPartAnchorLocation() const
			{
				return m_onLabelPartAnchorLocation;
			}

			/** 
			* Getter for the distance along the label part box where the text anchor is.
			* @return label part distance
			* @reqid 003.0019
			*/
			int GetLabelPartDistance() const
			{
				return m_onLabelPartDistanceFromAnchorLocation;
			}

		 protected:

#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/** 
			 * Called on the specific implementation to deserialize itself.
			 * @param node - node to be used for deserialization.
			 * @param ownerMap - The direct or indirect owner of the object
			 * @param error - Error info.
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif

			/** Validates the configuration parameters. Checks if percentage values are within range. Sets the error if they are not.
			*	@param	error			[in, out] Error
			* 
			*	@reqid 003.0019
			*/
			void ValidateParameters(Error &error);
			
			/** Computes the offset of a Label Part from the Label's location.
			*	@param	labelLowerLeft		the lower left of the entire label
			*	@param	labelPartLowerLeft	the lower left of the LabelPart
			*	@param  labelPartOffset		the offset of the labelPart after it has been shifted to the new position, based on user vertical alignment configuration
			*	@param	offset				[out]	the entire offset needed to move the entire label to a new location
			* 
			*	@reqid 003.0019
			*/
			void ComputeLabelPartOffsetFromLabel( const Point &labelLowerLeft, const Point &labelPartLowerLeft, const Point &labelPartOffset, Point *offset );

			/** Computes the offset of a LabelPart (represented by the lowerLeft parameter) from its current location to satisfy the vertical alignment configuration.
			*	@param	lowerLeft			the lower left of the LabelPart
			*	@param	height				the bounding box height of the LabelPart
			*	@param  angle					the angle of the LabelPart
			* @param	lineWidth			thickness of the feature polyline
			*	@param	offset				[out]	the offset needed to move the LabelPart to configured location 
			*	@param	error					[in, out] Error
			*	@reqid 003.0019
			*/
			void ComputeLabelPartVerticalOffset( const Point &lowerLeft, float height, float angle, float lineWidth, Point *offset, Error &error );

			/** Computes the offset of a LabelPart (represented by the lowerLeft parameter) from its current location to satisfy the horizontal alignment configuration.
			*	@param	lowerLeft			the lower left of the LabelPart
			*	@param	length				the bounding box length of the LabelPart
			*	@param  angle					the angle of the LabelPart
			* @param	lineWidth			thickness of the feature polyline
			*	@param	offset				[out]	the offset needed to move the LabelPart to configured location 
			*	@param	error					[in, out] Error
			*	@reqid 003.0029
			*/
			void ComputeLabelPartHorizontalOffset( const Point &lowerLeft, float height, float angle, float lineWidth, Point *offset, Error &error );

			/**
			 * The location on the line feature where the LabelPart is supposed to be anchored to
			 * @reqid 003.0019
			 */
			AnchorLocation m_onLineAnchorLocation;

			/**
			 * The distance from the location on the line feature where the LabelPart is supposed to be anchored to (in percents)
			 * @reqid 003.0019
			 */
			int m_onLineDistanceFromAnchorLocation;

			/**
			 * The location on the LabelPart where the LabelPart is supposed to be anchored on to the Line Feature
			 * @reqid 003.0019
			 */
			AnchorLocation m_onLabelPartAnchorLocation;

			/**
			 * The distance from the location on the LabelPart where the LabelPart is supposed to be anchored on to the Line Feature (in percents)
			 * @reqid 003.0019
			 */
			int m_onLabelPartDistanceFromAnchorLocation;

			/**
			 * The requested StackLevel number to be anchored on the Line Feature. Must be > 1.
			 * @reqid 003.0019
			 */
			int m_stackLevelNumber;

			/**
			 * The requested TextObject name to be anchored on the Line Feature
			 * @reqid 003.0019
			 */
			SharedString m_textObjectName;		
		 };

		/**
		 * Class that defines a label anchoring for line features (Middle placement) for assemblies.
		 * Defines how the label will be anchored vertically.
		 * @reqid 003.0019
		 */
		 class LABELINGENGINE_API AssemblyLineLabelAnchor : public LineLabelAnchor
		 {
		 public:

			/**
			 * Possible anchoring of different AssemblyLabel LabelParts on to the LineFeture
			 * @reqid 003.0019
			 */
			enum AnchorTo
			{
				AnchorTo_LabelBoundingBox = 0,
				AnchorTo_TextComponent = 1,
				AnchorTo_StackLevel = 2,
				AnchorTo_TextObject = 3,
				AnchorTo_SymbolComponent = 4
			};

			/**
			 * Default constructor. By default sets true middle placement. Middle of Label, Middle of line feature. No Text or Symbol Component name specified.
			 * @reqid 003.0019
			 */
			AssemblyLineLabelAnchor(Error &error)
				:	LineLabelAnchor(error),
      		m_anchorTo(AnchorTo_LabelBoundingBox),
					m_symbolComponentName(_T(""), error)
			{}

			/**
			 * Default destructor.
			 * @reqid 003.0019
			 */
			~AssemblyLineLabelAnchor()
			{}

			/**
			 * @see LabelAnchor::GetType
			 * @return -
			 * @reqid 003.0019
			 */
			virtual LabelAnchorType GetType() const
			{
				return LabelAnchorType_AssemblyLine;
			}

			/** Gets the AnchorTo property.
			* This property defines where to anchor AssemblyLabel label parts on a line feature.
			*	@return	AnchorTo value
			*	@reqid 003.0019
			*/
			AnchorTo GetAnchorTo() const;

			/** Sets the AnchorTo property.
			* @see GetAnchorTo
			*	@param anchorTo	the anchoring of AssemblyLabel label parts on the line feature
			*	@reqid 003.0019
			*/
			void SetAnchorTo(AssemblyLineLabelAnchor::AnchorTo anchorTo);

			/** Computes the Anchor offset for the Assembly Label's current location. 
			*	By how much does the entire label need to be offset in order to achieve the configured effect.
			*	
			*	@param	label			AssemblyLabel to be offset
			*	@param	lineWidth		The line feature width
			*	@param  offset			[out]	the computed offset
			*	@param  vertical		compute the offset vertically or horizontally
			*	@param	error			[in, out] Error
			* 
			*	@reqid 003.0019
			*/
			void ComputeAnchorOffset(const Label &label, float lineWidth, Point *offset, bool vertical, Error &error );

			/** Get the TextComponent names.
			* Identifies the TextComponents to be positioned on a line feature.
			*	@return		TextComponent list
			*	@reqid 003.0019
			*/
			const LinkedList<SharedString> *GetTextComponentNames() const;

			/** Set the TextComponent name.
			* @see GetTextComponentName
			*	@param	textComponentName	the name used to identify the object
			*	@param	error							an Error object
			*	@reqid 003.0019
			*/
			void SetTextComponentName(const TCHAR *textComponentName, Error &error);

			/** Get the SymbolComponent name.
			* Identifies the SymbolComponent to be positioned on a line feature.
			*	@return		SymbolComponent name
			*	@reqid 003.0019
			*/
			const SharedString* GetSymbolComponentName() const;

			/** Set the SymbolComponent name.
			*	@see GetSymbolComponentName
			*	@param	symbolComponentName	the name used to identify the object
			*	@param	error								an Error object
			*	@reqid 003.0019
			*/
			void SetSymbolComponentName(const TCHAR *symbolComponentName, Error &error);

			/**
			* @see LabelAnchor::Clone
			* @reqid 001.0076
			*/
			virtual AssemblyLineLabelAnchor* Clone( Error& error ) const;

			/**
			* Make deep copy of one instance's members to another
			* @param src source object
			* @param dest destination object
			* @param error an error object to be populated
			* @reqid 001.0076
			*/
			static void Copy( const AssemblyLineLabelAnchor& src, AssemblyLineLabelAnchor& dest, Error &error );

			/**
			 * Computes the location on the bounding box of the label which is the text anchor and returns a position
			   vector to this point. Label's lower left must be at the origin - (0,0) unless being called from
			   AssemblyLineLabelAnchor in which case the parent assembly label's lower left must be at the origin (0,0).
		     * @param label - label whose text anchor is computed
			 * @param textAnchorPositionVector - [out] position vector to the text anchor.
			 * @reqid 003.0019
			 */
			void ComputeTextAnchorPositionVector(const Label &label,Vector *textAnchorPositionVector) const;

		protected:
#ifdef S11N

			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/** 
			 * @see LabelAnchor::_Deserialize
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
		private:
			/** Computes the offset of an entire AssemblyLabel from the Label's current location.
			*	Takes into account the vertical alignment configuration.
			*
			*	@param	label					the AssemblyLabel for which the offset is to be computed
			*	@param	lineWidth				the Line Feature width
			*	@param	offset					[out]	the offset needed to move the AssemblyLabel to a new, vertically aligned location
			*	@param  vertical				compute the offset vertically or horizontally
			*	@param	error					[in, out] Error
			* 
			*	@reqid 003.0019
			*/
			void ComputeLabelAnchorOffset(const AssemblyLabel &label, float lineWidth, Point *offset, bool vertical, Error &error);

			/** Computes the offset of a TextComponent from the TextComponent's current location.
			*	Takes into account the vertical alignment configuration, and the m_textComponentName.
			*
			*	@param	label					the AssemblyLabel containing the TextComponent for which the offset is to be computed
			*	@param	lineWidth				the Line Feature width
			*	@param	offset					[out]	the offset needed to move the TextComponent to a new, vertically aligned location
			*	@param  vertical				compute the offset vertically or horizontally
			*	@param	error					[in, out] Error
			* 
			*	@reqid 003.0019
			*/
			void ComputeTextComponentAnchorOffset(const AssemblyLabel &label, float lineWidth, Point *offset, bool vertical, Error &error);

			/** Computes the offset of a SymbolComponent from the SymbolComponent's current location.
			*	Takes into account the vertical alignment configuration, and the m_symbolComponentName.
			*
			*	@param	label					the AssemblyLabel containing the SymbolComponent for which the offset is to be computed
			*	@param	lineWidth				the Line Feature width
			*	@param	offset					[out]	the offset needed to move the SymbolComponent to a new, vertically aligned location
			*	@param  vertical				compute the offset vertically or horizontally
			*	@param	error					[in, out] Error
			* 
			*	@reqid 003.0019
			*/
			void ComputeSymbolComponentAnchorOffset(const AssemblyLabel &label, float lineWidth, Point *offset, bool vertical, Error &error);

			/** Computes the offset of a StackLevel from the StackLevel's current location.
			*	Takes into account the vertical alignment configuration, and the m_stackLevelNumber (defined in the parent class).
			*
			*	@param	label					the AssemblyLabel containing the StackLevel for which the offset is to be computed
			*	@param	lineWidth				the Line Feature width
			*	@param	offset					[out]	the offset needed to move the StackLevel to a new, vertically aligned location
			*	@param  vertical				compute the offset vertically or horizontally
			*	@param	error					[in, out] Error
			* 
			*	@reqid 003.0019
			*/
			void ComputeStackLevelAnchorOffset(const AssemblyLabel &label, float lineWidth, Point *offset, bool vertical, Error &error);

			/** Computes the offset of a TextObject from the TextObjects's current location.
			*	Takes into account the vertical alignment configuration, and the m_textObjectName (defined in the parent class).
			*
			*	@param	label					the AssemblyLabel containing the TextObject for which the offset is to be computed
			*	@param	lineWidth				the Line Feature width
			*	@param	offset					[out]	the offset needed to move the TextObject to a new, vertically aligned location
			*	@param  vertical				compute the offset vertically or horizontally
			*	@param	error					[in, out] Error
			* 
			*	@reqid 003.0019
			*/
			void ComputeTextObjectAnchorOffset(const AssemblyLabel &label, float lineWidth, Point *offset, bool vertical, Error &error);

			/** Checks whether the AssemblyLabel contains a named TextComponent. Returns its position in the collection
			*
			*	@param	label					the AssemblyLabel to be checked for the presence of the TextComponent
			*	@param	textComponentName		the TextComponent name
			*	@param	error					[in, out] Error
			* 
			*	@return	<UL>
			*				<LI>-1	Error</LI>
			*				<LI>0	No TextComponent with the specified name found</LI>
			*				<LI>>0	The index in the TextComponents collection to the requested TextComponent</LI>
			*			</UL>
			*
			*	@reqid 003.0019
			*/
			int HasTextComponent(const AssemblyLabel &label, const TCHAR *textComponentName, Error &error);

			/****************************************************************************************
			THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
			TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
			****************************************************************************************/
		 protected:
			 AnchorTo m_anchorTo;

		 private:
			 SharedString::Collection m_textComponentNames;
			 SharedString m_symbolComponentName;
		 };

		/**
		 * Class that defines a label anchoring for line features (Middle placement) for regular labels. Defines how the label will be vertically anchored.
		 * @reqid 001.0063
		 */
		 class LABELINGENGINE_API RegularLineLabelAnchor : public LineLabelAnchor
		 {
		 public:

			/**
			 * Possible anchoring of different RegularLabel label parts on to the LineFeture
			 * @reqid 003.0019
			 */
			enum AnchorTo
			{
				AnchorTo_LabelBoundingBox = 0,
				AnchorTo_StackLevel = 2,
				AnchorTo_TextObject = 3,
			};

			/**
			 * Default constructor. By default sets true middle placement. Middle of Label, Middle of line feature.
			 *
			 *	@param	error		[in,out]
			 *
			 * @reqid 003.0019
			 */
			RegularLineLabelAnchor(Error &error)  : LineLabelAnchor(error),
													m_anchorTo(AnchorTo_LabelBoundingBox)
			{}

			/**
			 * Default destructor.
			 *
			 * @reqid 003.0019
			 */
			~RegularLineLabelAnchor()
			{}

			/**
			 * @see LabelAnchor::GetType
			 * @return -
			 * @reqid 003.0019
			 */
			virtual LabelAnchorType GetType() const
			{
				return LabelAnchorType_RegularLine;
			}

			/** Gets the AnchorTo property.
			* This property defines where to anchor RegularLabel label parts on a line feature.
			*	@return	AnchorTo value
			*	@reqid 003.0019
			*/
			AnchorTo GetAnchorTo() const;

			/** Sets the AnchorTo property.
			* @see GetAnchorTo
			*	@param	anchorTo	Defines different possible anchorings of RegularLabel label parts on the line feature
			*	@reqid 003.0019
			*/
			void SetAnchorTo(RegularLineLabelAnchor::AnchorTo anchorTo);

			/** Computes the Anchor offset for the RegularLabel's current location. 
			*	By how much does the entire label need to be offset in order to achieve the configured effect.
			*	
			*	@param	label			Regular to be offset
			*	@param	lineWidth		The line feature width
			*	@param  offset			[out]	the computed offset
			*	@param  vertical		compute the offset vertically or horizontally
			*	@param	error			[in, out] Error
			* 
			*	@reqid 003.0019
			*/
			void ComputeAnchorOffset(const Label &label, float lineWidth, Point *offset, bool vertical,  Error &error);

			/**
			* @see LabelAnchor::Clone
			* @reqid 001.0076
			*/
			virtual RegularLineLabelAnchor* Clone( Error& error ) const;

			/**
			* Make deep copy of one instance's members to another
			* @param src source object
			* @param dest destination object
			* @param error an error object to be populated
			* @reqid 001.0076
			*/
			static void Copy( const RegularLineLabelAnchor& src, RegularLineLabelAnchor& dest, Error &error );

			/**
			 * Computes the location on the bounding box of the label which is the text anchor and returns a position
			   vector to this point. Label's lower left must be at the origin - (0,0)
		     * @param label - label whose text anchor is computed
			 * @param textAnchorPositionVector - [out] position vector to the text anchor.
			 * @reqid 003.0019
			 */
			static void ComputeTextAnchorPositionVector(const Label &label,const AnchorTo anchorTo,AnchorLocation anchorLocation,int distanceFromAnchorLocation,int stackLevelNumber,const SharedString &textObjectName,Vector *textAnchorPositionVector);
		 protected:

#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/** 
			 * @see LabelAnchor::_Deserialize
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif

		 private:
			/** Computes the offset of an entire RegularLabel from the Label's current location.
			*	Takes into account the vertical alignment configuration.
			*
			*	@param	label					the RegularLabel for which the offset is to be computed
			*	@param	lineWidth				the Line Feature width
			*	@param	offset					[out]	the offset needed to move the AssemblyLabel to a new, vertically aligned location
			*	@param  vertical				compute the offset vertically or horizontally
			*	@param	error					[in, out] Error
			* 
			*	@reqid 003.0019
			*/
			void ComputeLabelAnchorOffset(const RegularLabel &label, float lineWidth, Point *offset, bool vertical, Error &error);

			/** Computes the offset of a StackLevel from the StackLevel's current location.
			*	Takes into account the vertical alignment configuration, and the m_stackLevelNumber (defined in the parent class).
			*
			*	@param	label					the RegularLabel containing the StackLevel for which the offset is to be computed
			*	@param	lineWidth				the Line Feature width
			*	@param	offset					[out]	the offset needed to move the StackLevel to a new, vertically aligned location
			*	@param  vertical				compute the offset vertically or horizontally
			*	@param	error					[in, out] Error
			* 
			*	@reqid 003.0019
			*/
			void ComputeStackLevelAnchorOffset(const RegularLabel &label, float lineWidth, Point *offset, bool vertical, Error &error);
			
			/** Computes the offset of a TextObject from the TextObjects's current location.
			*	Takes into account the vertical alignment configuration, and the m_textObjectName (defined in the parent class).
			*
			*	@param	label					the RegularLabel containing the TextObject for which the offset is to be computed
			*	@param	lineWidth				the Line Feature width
			*	@param	offset					[out]	the offset needed to move the TextObject to a new, vertically aligned location
			*	@param  vertical				compute the offset vertically or horizontally
			*	@param	error					[in, out] Error
			* 
			*	@reqid 003.0019
			*/			
			void ComputeTextObjectAnchorOffset(const RegularLabel &label, float lineWidth, Point *offset, bool vertical, Error &error);

			/****************************************************************************************
			THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
			TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
			****************************************************************************************/
		 protected:
			 AnchorTo m_anchorTo;
		 };

		 /**
		  * This class tells a point feature assembly label how to anchor itself on the feature.
		  * A variety of options is possible, the default being to anchor to the label bounding 
		  * box. When anchoring to a text, symbol component or text object the name
		  * of the proper entity has to be provided. For stack level the number is needed. 
		  * For all the options the anchoring position also needs to be set (right, left etc.).
		  */
		 class LABELINGENGINE_API AssemblyPointLabelAnchor : public PointLabelAnchor
		 {
		 public:

		/**
			 * Possible anchoring of different AssemblyLabel LabelParts on to the point feature
			 * @reqid 002.0037
			 */
			enum AnchorTo
			{
				AnchorTo_LabelBoundingBox = 0,
				AnchorTo_TextComponent = 1,
				AnchorTo_StackLevel = 2,
				AnchorTo_TextObject = 3,
				AnchorTo_SymbolComponent = 4
			};

			/**
			 * Default constructor. By default sets true middle placement. Middle of Label 
			 * on point feature. No Text or Symbol Component name specified.
			 * @reqid 002.0037
			 */
			AssemblyPointLabelAnchor(Error &error)
				:	PointLabelAnchor(error),
      		m_anchorTo(AnchorTo_LabelBoundingBox),
					m_textComponentName(_T(""), error),
					m_symbolComponentName(_T(""), error)
			{}

			/**
			 * Default destructor.
			 * @reqid 002.0037
			 */
			~AssemblyPointLabelAnchor()
			{}

			/**
			 * @see LabelAnchor::GetType
			 * @return -
			 * @reqid 003.0019
			 */
			virtual LabelAnchorType GetType() const
			{
				return LabelAnchorType_AssemblyPoint;
			}


			/** Gets the AnchorTo property.
			* This property defines where to anchor AssemblyLabel label parts on a point feature.
			*	@return	AnchorTo value
			*	@reqid 002.0037
			*/
			AnchorTo GetAnchorTo() const
			{
				return m_anchorTo;
			}

			/** Sets the AnchorTo property.
			* @see GetAnchorTo
			*	@param val	the anchoring of AssemblyLabel label parts on the point feature
			*	@reqid 002.0037
			*/
			void SetAnchorTo(AssemblyPointLabelAnchor::AnchorTo val)
			{
				m_anchorTo = val;
			}

			/** Computes the Anchor offset for the Assembly Label's current location. 
			*	By how much does the entire label need to be offset in order to achieve the configured effect.
			*	
			*	@param	label			AssemblyLabel to be offset
			*	@param  offset			[out]	the computed offset
			*	@param  vertical		compute the offset vertically or horizontally
			*	@param	error			[in, out] Error
			* 
			*	@reqid 002.0037
			*/
			void ComputeAnchorOffset(const Label &label, Point *offset, Error &error );

			/** Get the TextComponent name.
			* Identifies the TextComponent to be positioned on a point feature.
			*	@return		TextComponent name
			*	@reqid 002.0037
			*/
			const SharedString* GetTextComponentName() const
			{
				return &m_textComponentName;
			}

			/** Set the TextComponent name.
			* @see GetTextComponentName
			*	@param	val	the name used to identify the object
			*	@param	error							an Error object
			*	@reqid 002.0037
			*/
			void SetTextComponentName(const TCHAR *val, Error &error)
			{
				m_textComponentName.AssignString(val, error);

			}

			/** Get the SymbolComponent name.
			* Identifies the SymbolComponent to be positioned on a point feature.
			*	@return		SymbolComponent name
			*	@reqid 002.0037
			*/
			const SharedString* GetSymbolComponentName() const
			{
				return &m_symbolComponentName;
			}

			/** Set the SymbolComponent name.
			*	@see GetSymbolComponentName
			*	@param	val	the name used to identify the object
			*	@param	error								an Error object
			*	@reqid 002.0037
			*/
			void SetSymbolComponentName(const TCHAR *val, Error &error)
			{
				m_symbolComponentName.AssignString(val, error);
			}

			/**
			* @see LabelAnchor::Clone
			* @reqid 001.0076
			*/
			virtual AssemblyPointLabelAnchor* Clone( Error& error ) const
			{
				AssemblyPointLabelAnchor* copy = new AssemblyPointLabelAnchor( error );
				if ( copy == NULL )
				{
					SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
				}
				else
				{
					if (! error )
					{
						Copy( *this, *copy, error );
					}
					if ( error )  //ctor or Copy failed
					{
						delete copy;
						copy = NULL;
					}
				}
				
				return copy;
			}
			/**
			* Make deep copy of one instance's members to another
			* @param src source object
			* @param dest destination object
			* @param error an error object to be populated
			* @reqid 001.0076
			*/
			static void Copy( const AssemblyPointLabelAnchor& src, AssemblyPointLabelAnchor& dest, Error &error )
			{
				PointLabelAnchor::Copy( src, dest, error );
				if ( error )
				{
					return;
				}

				dest.m_anchorTo = src.m_anchorTo;

				dest.m_symbolComponentName.AssignString( src.m_symbolComponentName.GetString(), error );
				if ( error )
				{
					return;
				}

				dest.m_textComponentName.AssignString( src.m_textComponentName.GetString(), error );
				if ( error )
				{
					return;
				}
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
			 * @see LabelAnchor::_Deserialize
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif

		private:
			/** Computes the offset of an entire AssemblyLabel from the Label's current location.
			*	Takes into account the vertical alignment configuration.
			*
			*	@param	label					the AssemblyLabel for which the offset is to be computed
			*	@param	offset					[out]	the offset needed to move the AssemblyLabel to a new, vertically aligned location
			*	@param	error					[in, out] Error
			* 
			*	@reqid 002.0037
			*/
			void ComputeLabelAnchorOffset(const AssemblyLabel &label, Point *offset, Error &error);

			/** Computes the offset of a TextComponent from the TextComponent's current location.
			*
			*	@param	label					the AssemblyLabel containing the TextComponent for which the offset is to be computed
			*	@param	offset					[out]	the offset needed to move the TextComponent to a new
			*	@param	error					[in, out] Error
			* 
			*	@reqid 002.0037
			*/
			void ComputeTextComponentAnchorOffset(const AssemblyLabel &label, Point *offset, Error &error);

			/** Computes the offset of a SymbolComponent from the SymbolComponent's current location.
			*	Takes into account the vertical alignment configuration, and the m_symbolComponentName.
			*
			*	@param	label					the AssemblyLabel containing the SymbolComponent for which the offset is to be computed
			*	@param	offset					[out]	the offset needed to move the SymbolComponent to a new, vertically aligned location
			*	@param	error					[in, out] Error
			* 
			*	@reqid 002.0037
			*/
			void ComputeSymbolComponentAnchorOffset(const AssemblyLabel &label, Point *offset, Error &error);

			/** Computes the offset of a StackLevel from the StackLevel's current location.
			*
			*	@param	label					the AssemblyLabel containing the StackLevel for which the offset is to be computed
			*	@param	offset					[out]	the offset needed to move the StackLevel to a new
			*	@param	error					[in, out] Error
			* 
			*	@reqid 002.0037
			*/
			void ComputeStackLevelAnchorOffset(const AssemblyLabel &label, Point *offset, Error &error);

			/** Computes the offset of a TextObject from the TextObjects's current location.
			*
			*	@param	label					the AssemblyLabel containing the TextObject for which the offset is to be computed
			*	@param	offset					[out]	the offset needed to move the TextObject to a new
			*	@param	error					[in, out] Error
			* 
			*	@reqid 002.0037
			*/
			void ComputeTextObjectAnchorOffset(const AssemblyLabel &label, Point *offset, Error &error);

			/** Checks whether the AssemblyLabel contains a named TextComponent. Returns its position in the collection
			*
			*	@param	label					the AssemblyLabel to be checked for the presence of the TextComponent
			*	@param	textComponentName		the TextComponent name
			*	@param	error					[in, out] Error
			* 
			*	@return	<UL>
			*				<LI>-1	Error</LI>
			*				<LI>0	No TextComponent with the specified name found</LI>
			*				<LI>>0	The index in the TextComponents collection to the requested TextComponent</LI>
			*			</UL>
			*
			*	@reqid 002.0037
			*/
			int HasTextComponent(const AssemblyLabel &label, const TCHAR *textComponentName, Error &error);

			/****************************************************************************************
			THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
			TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
			****************************************************************************************/
		 protected:
			 AnchorTo m_anchorTo;

		 private:
			SharedString m_textComponentName;
			SharedString m_symbolComponentName;
		 };

		/**
		 * Class that defines a label anchoring for line features (Middle placement) for regular labels. Defines how the label will be vertically anchored.
		 * @reqid 001.0063
		 */
		 class LABELINGENGINE_API RegularPointLabelAnchor : public PointLabelAnchor
		 {
		 public:

			/**
			 * Possible anchoring of different RegularLabel label parts on to the point feature
			 * @reqid 002.0037
			 */
			enum AnchorTo
			{
				AnchorTo_LabelBoundingBox = 0,
				AnchorTo_StackLevel = 2,
				AnchorTo_TextObject = 3,
			};

			/**
			 * Default constructor. By default sets true middle placement. Middle of Label on point feature.
			 *
			 *	@param	error		[in,out]
			 *
			 * @reqid 002.0037
			 */
			RegularPointLabelAnchor(Error &error)  : PointLabelAnchor(error),
													m_anchorTo(AnchorTo_LabelBoundingBox)
			{}

			/**
			 * Default destructor.
			 *
			 * @reqid 002.0037
			 */
			~RegularPointLabelAnchor()
			{}

			/**
			 * @see LabelAnchor::GetType
			 * @return -
			 * @reqid 003.0019
			 */
			virtual LabelAnchorType GetType() const
			{
				return LabelAnchorType_RegularPoint;
			}


			/** Gets the AnchorTo property.
			* This property defines where to anchor RegularLabel label parts on a line feature.
			*	@return	AnchorTo value
			*	@reqid 002.0037
			*/
			AnchorTo GetAnchorTo() const
			{
				return m_anchorTo;
			}

			/** Sets the AnchorTo property.
			* @see GetAnchorTo
			*	@param	val	Defines different possible anchorings of RegularLabel label parts on the line feature
			*	@reqid 002.0037
			*/
			void SetAnchorTo(RegularPointLabelAnchor::AnchorTo val)
			{
				m_anchorTo = val;
			}

			/** Computes the Anchor offset for the RegularLabel's current location. 
			*	By how much does the entire label need to be offset in order to achieve the configured effect.
			*	
			*	@param	label			Regular to be offset
			*	@param  offset			[out]	the computed offset
			*	@param	error			[in, out] Error
			* 
			*	@reqid 002.0037
			*/
			void ComputeAnchorOffset(const Label &label, Point *offset,  Error &error);

			/**
			* @see LabelAnchor::Clone
			* @reqid 001.0076
			*/
			virtual RegularPointLabelAnchor* Clone( Error& error ) const
			{
				RegularPointLabelAnchor* copy = new RegularPointLabelAnchor( error );
				if ( copy == NULL )
				{
					SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
				}
				else
				{
					if (! error )
					{
						Copy( *this, *copy, error );
					}
					if ( error )  //ctor or Copy failed
					{
						delete copy;
						copy = NULL;
					}
				}
				return copy;
			}

			/**
			* Make deep copy of one instance's members to another
			* @param src source object
			* @param dest destination object
			* @param error an error object to be populated
			* @reqid 001.0076
			*/
			static void Copy( const RegularPointLabelAnchor& src, RegularPointLabelAnchor& dest, Error &error )
			{
				PointLabelAnchor::Copy( src, dest, error );
				if ( error )
				{
					return;
				}
				dest.m_anchorTo = src.m_anchorTo;
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
			 * @see LabelAnchor::_Deserialize
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif


		 private:
			/** Computes the offset of an entire RegularLabel from the Label's current location.
			*	Takes into account the vertical alignment configuration.
			*
			*	@param	label					the RegularLabel for which the offset is to be computed
			*	@param	offset					[out]	the offset needed to move the AssemblyLabel to a new, vertically aligned location
			*	@param	error					[in, out] Error
			* 
			*	@reqid 002.0037
			*/
			void ComputeLabelAnchorOffset(const RegularLabel &label, Point *offset, Error &error);

			/** Computes the offset of a StackLevel from the StackLevel's current location.
			*
			*	@param	label					the RegularLabel containing the StackLevel for which the offset is to be computed
			*	@param	offset					[out]	the offset needed to move the StackLevel to a new
			*	@param	error					[in, out] Error
			* 
			*	@reqid 002.0037
			*/
			void ComputeStackLevelAnchorOffset(const RegularLabel &label, Point *offset, Error &error);
			
			/** Computes the offset of a TextObject from the TextObjects's current location.
			*
			*	@param	label					the RegularLabel containing the TextObject for which the offset is to be computed
			*	@param	offset					[out]	the offset needed to move the TextObject to a new
			*	@param	error					[in, out] Error
			* 
			*	@reqid 002.0037
			*/			
			void ComputeTextObjectAnchorOffset(const RegularLabel &label, Point *offset, Error &error);

			/****************************************************************************************
			THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
			TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
			****************************************************************************************/
		 protected:
			 AnchorTo m_anchorTo;
		 };

		 /**
		 * Class that holds a collection of type Label object pointers.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Labels :
			public LinkedList < Label >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				Labels()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~Labels()
				{
				}
		};

		/**
		* Base class that models a CandidatePosition for a label.
		* Derived classes store custom attributes that guide persistence command handlers.
		* @reqid 002.0038
		*/
		class LABELINGENGINE_API CandidatePosition
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
			* @param referencedLabel	A reference to the label associated with the candidate position
			* @param lowerLeft				The location of the lower left corner of the referenced label according to the candidate position
			* @param angle						The angle of orientation (in radians) of the referenced label according to the candidate position
			* @param justification		The stack level justification of the referenced label according to the candidate position
			* @reqid 002.0038
			*/
			CandidatePosition( Label* referencedLabel, const Point& lowerLeft, float angle,
				PlacementRule::LabelJustification justification = PlacementRule::LabelJustification_Unknown )
 				: m_referencedLabel( referencedLabel ),
					m_angle( angle ),
					m_justification( justification )
			{
				m_lowerLeft.SetXY( lowerLeft.GetX(), lowerLeft.GetY() );
			}

			/**
			* Destructor
			* The label object is not owned.  It is only for reference to the pre-built structure.
			* @reqid 002.0038
			*/
			virtual ~CandidatePosition()
			{
			}

			/**
			* Get the candidate position's referenced label.
			* It returns a mutable pointer so that the label can be translated or rotated.
			* The lower left and orientation angle can be used to reposition it when
			* referenced through another candidate position instance.
			* @return a pointer to the candidate position's referenced label.
			* @reqid 002.0038
			*/
			Label* GetReferencedLabel() const
			{
				return m_referencedLabel;
			}

			/**
			* Get the candidate position's lower left corner
			* @return a pointer to the candidate position's lower left corner.
			* @reqid 002.0038
			*/
			const Point* GetLowerLeft() const
			{
				return &m_lowerLeft;
			}

			/**
			* Get the candidate position's orientation angle.
			* @return the candidate position's orientation angle.
			* @reqid 002.0038
			*/
			float GetAngle() const
			{
				return m_angle;
			}

			/** Get the candidate position's justification.
			 *	@return the candidate position's justification.
			 *	@reqid 001.0063
			 */
			PlacementRule::LabelJustification GetJustification() const
			{
				return m_justification;
			}

			/**
			* Create a deep copy of this instance.
			* @param error error information
			* @return the copy
			* @reqid 001.0076
			*/
			virtual CandidatePosition* Clone( Error& error ) const = 0;

		protected:
			Label* m_referencedLabel;
			Point m_lowerLeft;
			float m_angle;
			PlacementRule::LabelJustification m_justification;
		};

		/**
		* Class that holds a collection of CandidatePosition object pointers.
		* @reqid 002.0038
		*/
		class LABELINGENGINE_API CandidatePositions :
			public LinkedList < CandidatePosition >
		{
		public:
			/**
			* Default constructor implemented to avoid compiler generated object code.
			* @reqid 002.0038
			*/
			CandidatePositions()
				:	m_candidateBookmarkForOffset( NULL ),
					m_candidateBookmarkForOverline( NULL ),
					m_candidateBookmarkForBumping( NULL )
			{
			}

			/**
			* Default destructor implemented to avoid compiler generated object code.
			* @reqid 002.0038
			*/
			~CandidatePositions()
			{
			}

			/**
			* Get an iterator positioned to the element representing a bookmark from which to continue
			* iterating over candidate positions in response to an OFFSET command
			* @reqid 002.0038
			*/
			CandidatePositions::Iterator GetBookmarkedIteratorForOffset()
			{
				if ( m_candidateBookmarkForOffset == NULL )
				{
					return Begin();
				}
				return Begin( m_candidateBookmarkForOffset )++;
			}

			/**
			* @see GetBookmarkedIteratorForOffset
			* @reqid 002.0038
			*/
			void SetIteratorBookmarkForOffset( CandidatePositions::Iterator& iterator )
			{
				m_candidateBookmarkForOffset = iterator.GetElementContainer();
			}

			/**
			* Get an iterator positioned to the element representing a bookmark from which to continue
			* iterating over candidate positions in response to an OVERLINE command
			* @reqid 002.0038
			*/
			CandidatePositions::Iterator GetBookmarkedIteratorForOverline()
			{
				if ( m_candidateBookmarkForOverline == NULL )
				{
					return Begin();
				}
				return Begin( m_candidateBookmarkForOverline )++;
			}

			/**
			* @see GetBookmarkedIteratorForOverline
			* @reqid 002.0038
			*/
			void SetIteratorBookmarkForOverline( CandidatePositions::Iterator& iterator )
			{
				m_candidateBookmarkForOverline = iterator.GetElementContainer();
			}

			/**
			* Get an iterator positioned to the element representing a bookmark from which to continue
			* iterating over candidate positions in response to a Bumping command
			* @reqid 002.0038
			*/
			CandidatePositions::Iterator GetBookmarkedIteratorForBumping()
			{
				if ( m_candidateBookmarkForBumping == NULL )
				{
					return Begin();
				}
				return Begin( m_candidateBookmarkForBumping )++;
			}

			/**
			* @see GetBookmarkedIteratorForBumping
			* @reqid 002.0038
			*/
			void SetIteratorBookmarkForBumping( CandidatePositions::Iterator& iterator )
			{
				m_candidateBookmarkForBumping = iterator.GetElementContainer();
			}

			/**
			* Deallocation of the whole list
			* @reqid 002.0038
			*/
			void Clear()
			{
				LinkedList < CandidatePosition >::Clear();

				m_candidateBookmarkForOffset = NULL;
				m_candidateBookmarkForOverline = NULL;
				m_candidateBookmarkForBumping = NULL;				
			}

		private:
			Element* m_candidateBookmarkForOffset;
			Element* m_candidateBookmarkForOverline;
			Element* m_candidateBookmarkForBumping;
		};

		/**
		 * Class that holds all the information that defines the current state of processing of a label.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API PlacementState
		{
			public:
			/**
			 * Base class for a specialized type for each feature type that holds the information
			 * that defines the current state of processing a label using a placement algorithm
			 * designed for that feature type.  It is possible that one label request may be
			 * processed using placement algorithms designed for more than one feature type.
			 * Examples are the use of conforming line feature algorithms for placement along
			 * the boundary of an area feature and the use of the point feature placement algorithm
			 * for placement along an edge of a nearly rectangular area feature.
			 * @reqid 001.0063
			 */
				class LABELINGENGINE_API CustomPlacementState
				{
					public:
					/**
					* Possible error codes
					* @reqid 001.0063
					*/
					enum ErrorCode
					{
						ErrorCode_AllocationFailure = 1,
					};

					/**
					* Default constructor.
					* @reqid 001.0063
					*/
					CustomPlacementState()
					{
					}

					virtual ~CustomPlacementState()
					{
					}

					/**
					* clear any custom state properties.
					* @reqid 003.0075
					*/
					virtual void ClearState() = 0;

					/**
					* Creates copy of self.
					* @param error an error object to be populated
					* @return copy of self
					* @reqid 001.0076
					*/
					virtual CustomPlacementState* Clone( Error& error ) const = 0;
				};

				/**
				* Possible error codes
				* @reqid 001.0063
				*/
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_NullParameter
				};

				/**
				* Possible placement state type codes
				* @reqid 001.0063
				*/
				enum PlacementStateType
				{
					PlacementStateType_Area,
					PlacementStateType_Line,
					PlacementStateType_Point
				};

				/**
				* Default constructor
				* @reqid 001.0063
				*/
				PlacementState()
					: m_currentlyInPersistenceHandler( false ),
						m_persistenceExecSeq( NULL ),
						m_overhangAmount( 0 ),
						m_wasPlaced(false),
						m_workingLabel(NULL),
						m_areaPlacementState(NULL),
						m_linePlacementState(NULL),
						m_pointPlacementState(NULL),
						m_hideAttemptsIterator(NULL),
						m_currentNumStackLevels(0),
						m_currentTextToTextBufferSize(0),
						m_currentTextToLineBufferSize(0),
						m_currentTextToPointBufferSize(0),
						m_currentTextToTextBufferStepNumber(0),
						m_currentTextToLineBufferStepNumber(0),
						m_currentTextToPointBufferStepNumber(0),
						m_currentFontStepNumber(0),
						m_numTextObjects(0),
						m_workingTextObjects(NULL),
						m_fontReductionGuides(NULL),
						m_originalFontSizes(NULL)
				{
				};

				/**
				* Destructor
				* @reqid 001.0063
				*/
				virtual ~PlacementState();

				/**
				* Initialize properties from default and user specified values.
				* Invoked by LabelRequest::ClearPlacmentState
				* @param originalLabel	- the original label
				* @reqid 001.0063
				*/
				void Initialize( const Label& originalLabel, PlacementRule& placementRule )
				{
					m_currentTextToLineBufferSize =
						placementRule.GetTexttoLineBufferRule()->GetBufferSize();

					m_currentTextToPointBufferSize =
						placementRule.GetTexttoPointBufferRule()->GetBufferSize();

					m_currentTextToTextBufferSize =
						placementRule.GetTexttoTextBufferRule()->GetBufferSize();

					const RegularLabel* regularLabel = NULL;
					if ( originalLabel.GetLabelType() == Label::LabelType_Regular )
					{
						regularLabel = static_cast<const RegularLabel*>( &originalLabel );
						assert( regularLabel != NULL );
					}
					else
					{
						const AssemblyLabel* assemblyLabel = static_cast<const AssemblyLabel*>( &originalLabel );
						TextComponents* textComponents = assemblyLabel->GetTextComponents();
						if ( textComponents != NULL )
						{
							TextComponent* textComponent = textComponents->GetFirstElement();
							if ( textComponent != NULL )
							{
								regularLabel = textComponent->GetRegularLabel();
								assert( regularLabel != NULL );
							}
						}
					}

					if ( regularLabel != NULL )
					{
						StackLevels* stackLevels = const_cast<StackLevels*>( regularLabel->GetStackLevels() );
						assert( stackLevels != NULL );

						StackLevel* stackLevel = stackLevels->GetFirstElement();
						assert( stackLevel != NULL );

						TextObjects* textObjects = stackLevel->GetTextObjects();
						assert( textObjects != NULL );

						TextObject* textObject = textObjects->GetFirstElement();
						assert( textObject != NULL );

						const TextStyle* textStyle = textObject->GetTextStyle();
						assert( textStyle != NULL );

						m_originalFontSize = textStyle->GetFontSize();
					}
				}

				/**
				* Clear out the candidate labels and candidate positions collections.
				* Also delete the persistence execution sequence, which holds a
				* collection of sequence commands.  They are populated in each new frame.
				* This is invoked at the end of each frame in the process controller
				* to avoid leaking memory from frame to frame.  The invocation is performed
				* indirectly via a call to ClearPlacementHistory for each label request.
				* @reqid 001.0063
				*/
				void ClearPlacementHistory();

				/** Get the candidate labels.
				*	@return the candidate labels as a reference to an ordered collection
				* of Label instances
				*	@reqid 002.0038
				*/
				Labels* GetCandidateLabels()
				{
					return &m_candidateLabels;
				}

				/** Get the candidate label positions.
				 *	@return the candidate label positions as an ordered collection
				 *   of CandidatePosition instances
				 *	@reqid 002.0038
				 */
				CandidatePositions* GetCandidatePositions()
				{
					return &m_candidatePositions;
				}

				/** Get the candidate label positions.
				*	@return the candidate label positions as a reference to an ordered collection
				* of CandidatePosition instances
				*	@reqid 002.0038
				*/
				CandidatePositions* GetLeaderCandidatePositions()
				{
					return &m_leaderCandidatePositions;
				}

				/** Add a referenced label to the candidate labels collection.
				* Label is referenced by candidate positions.
				* @param referencedLabel	The referenced label to be added to the collection.
				* @param error						an Error object
				* @reqid 002.0038
				*/
				void AddCandidateLabel( Label* referencedLabel, Error& error )
				{
					m_candidateLabels.AddElement( referencedLabel, error );
				}

				/** Add a candidate position to the candidate positions collection.
				 * Includes a reference to the label to which the position refers.
				 * @param candidatePosition	The candidate position to be added to the collection.
				 * @param error							an Error object
				 * @reqid 002.0038
				 */
				void AddCandidatePosition( CandidatePosition* candidatePosition, Error& error )
				{
					m_candidatePositions.AddElement( candidatePosition, error );
				}

				/** Add a leader candidate position to the leader candidate positions collection.
				* Includes a reference to the label to which the position refers.
				* @param candidatePosition	The candidate position to be added to the collection.
				* @param error							an Error object
				* @reqid 002.0038
				*/
				void AddLeaderCandidatePosition( CandidatePosition* candidatePosition, Error& error )
				{
					m_leaderCandidatePositions.AddElement( candidatePosition, error );
				}

				/**
				* Get the working label. This is a working copy of the original label, modified by
				* persistence commands to reflect relaxation in properties, e.g., font and/or buffer sizes,
				* to create an improved opportunity for successful placement.
				* @return m_workingLabel
				* @reqid 006.0088
				*/
				Label* GetWorkingLabel() const
				{
					return m_workingLabel;
				}

				/**
				* Set the working label. This is a working copy of the original label, modified by
				* persistence commands to reflect relaxation in properties, e.g., font and/or buffer sizes,
				* to create an improved opportunity for successful placement.
				* Called by LabelRequest::GetWorkingLabel on first call to that method on the
				* containing label request.
				* This PlacementState instance takes ownership of the working label.
				* @see GetWorkingLabel
				* @param workingLabel	the working label
				* @reqid 006.0088
				*/
				void SetWorkingLabel( Label& workingLabel )
				{
					if ( &workingLabel != m_workingLabel )
					{
						delete m_workingLabel;
						m_workingLabel = &workingLabel;
					}
				}

				/**
				* Get if the label was previously placed
				* @return m_wasPlaced
				* @reqid ???.????
				*/
				bool GetWasPlaced()
				{
					return m_wasPlaced;
				}

				/**
				* Set if the label was placed
				* @param val the flag will be true or false
				* @reqid ???.????
				*/
				void SetWasPlaced(bool val)
				{
					m_wasPlaced = val;
				}

				/**
				* clear the candidate labels and the candidate positions
				* @reqid 003.0075
				*/
				void ClearState()
				{
					m_candidateLabels.Clear();
					m_candidatePositions.Clear();
					m_leaderCandidatePositions.Clear();

					if ( m_areaPlacementState != NULL )
					{
						m_areaPlacementState->ClearState();
					}
					
					if ( m_linePlacementState != NULL )
					{
						m_linePlacementState->ClearState();
					}

					if ( m_pointPlacementState != NULL )
					{
						m_pointPlacementState->ClearState();
					}
				}

				//////////////////////////////////////////////////////////////////////////////
				// Custom placement state properties managed by area, line, and point
				// feature placement modules.
				//////////////////////////////////////////////////////////////////////////////

				/** Get the custom area placement state.
				 * @return the contained area placement state instance
				 * @reqid 001.0063
				 */
				CustomPlacementState* GetCustomAreaPlacementState() const
				{
					return m_areaPlacementState;
				}

				/** Set the custom area placement state.
				 * @see GetCustomAreaPlacementState
				 * @reqid 001.0063
				 */
				void SetCustomAreaPlacementState( CustomPlacementState& areaPlacementState )
				{
					delete m_areaPlacementState;
					m_areaPlacementState = &areaPlacementState;
				}

				/** Get the custom line placement state.
				 * @return the contained line placement state instance
				 * @reqid 001.0063
				 */
				CustomPlacementState* GetCustomLinePlacementState() const
				{
					return m_linePlacementState;
				}

				/** Set the custom line placement state.
				 * @see GetCustomLinePlacementState
				 * @reqid 001.0063
				 */
				void SetCustomLinePlacementState( CustomPlacementState& linePlacementState )
				{
					delete m_linePlacementState;
					m_linePlacementState = &linePlacementState;
				}

				/** Get the custom point placement state.
				 * @return the contained point placement state instance
				 * @reqid 001.0063
				 */
				CustomPlacementState* GetCustomPointPlacementState() const
				{
					return m_pointPlacementState;
				}

				/** Set the custom area placement state.
				 * @see GetCustomPointPlacementState
				 * @reqid 001.0063
				 */
				void SetCustomPointPlacementState( CustomPlacementState& pointPlacementState )
				{
					delete m_pointPlacementState;
					m_pointPlacementState = &pointPlacementState;
				}

				//////////////////////////////////////////////////////////////////////////////
				// Placement state properties managed by looping command classes.
				// They must reside in the PlacementState class, rather than the
				// applicable class derived from SequenceCommand, because the BUMP command
				// interrupts the traversal through the ExecutionSequence to process a
				// previously processed label request, then resumes processing of the
				// original label request if BUMP fails to free a satisfactory place for it.
				// This label request switching would invalidate loop state properties if
				// managed by the command object.
				//////////////////////////////////////////////////////////////////////////////

				//////////////////////////////////////////////////////////////////////////////
				// Overhang command

				/** Set the persistence handler flag.
				* @param val the flag will be on or off
				* @reqid 003.0075
				*/
				void SetPersistenceHandlerFlag(bool val)
				{
					m_currentlyInPersistenceHandler = val;
				}

				/** Get the persistence handler flag.
				* @return m_currentlyInPersistenceHandler
				* @reqid 003.0075
				*/
				bool GetPersistenceHandlerFlag() const
				{
					return m_currentlyInPersistenceHandler;
				}

				/**
				* Get the persistence execution sequence
				* @return m_persistenceExecSeq
				* @reqid 003.0075
				*/
				ExecutionSequence* GetPersistenceExecSequence(Error& error);

				/**
				* delete current persistence execution sequence and set up for a new one
				* @reqid 003.0075
				*/
				void ReInitializePersistenceExecSequence(Error& error);

				/**
				* Set the overhang amount
				* @reqid 003.0075
				*/
				void SetOverhangAmount(float val)
				{
					m_overhangAmount = val;
				}

				/**
				* Get the overhang amount
				* @return m_overhangAmount
				* @reqid 003.0075
				*/
				float GetOverhangAmount() const
				{
					return m_overhangAmount;
				}

				//////////////////////////////////////////////////////////////////////////////
				// Stack command

				/** Get the current number of stack levels.
				 *	This may reflect the current state of a STACK operation.
				 *	@return the current number of stack levels
				 *	@reqid 001.0063
				 */
				int GetNumStackLevels()  const
				{
					return m_currentNumStackLevels;
				}

				/** Set the number of stack levels.
				 * @see GetNumStackLevels
				 * @reqid 001.0063
				 */
				void SetNumStackLevels( int currentNumStackLevels )
				{
					m_currentNumStackLevels = currentNumStackLevels;
				}

				//////////////////////////////////////////////////////////////////////////////
				// Hide command

				/** Get the hide attempts iterator.
				 * Called by HideCommand::OnLoopReentry to retrieve the current state relative
				 * to the label request on behalf of which it is being invoked.
				 * @reqid 001.0063
				 */
				HideAttempts::ConstIterator* GetHideAttemptsIterator()
				{
					return m_hideAttemptsIterator;
				}

				/** Set the hide attempts iterator.
				 * Called by HideCommand::Reset to initialize its state relative
				 * to the label request on behalf of which it is being invoked.
				 * @see GetHideAttemptsIterator
				 * @reqid 001.0063
				 */
				void SetHideAttemptsIterator( HideAttempts::ConstIterator* hideAttemptsIterator )
				{
					m_hideAttemptsIterator = hideAttemptsIterator;
				}

				//////////////////////////////////////////////////////////////////////////////
				// Buffer size commands

				/** Get the buffer size reduction rule computed for this label request.
				 *	@return the buffer size reduction rule
				 *	@reqid 001.0063
				 */
				BufferReductionGuide* GetBufferReductionGuide()
				{
					return &m_bufferReductionGuide;
				}

				/** Get the current buffer size reduction step number.
				 * @param bufferType	- the buffer reduction type for which the step number is being queried
				 * @return the current buffer size reduction step number
				 * @reqid 001.0063
				 */
				int GetBufferStepNumber( BufferType bufferType ) const
				{
					int currentBufferStepNumber = 0;

					switch ( bufferType )
					{
						case BufferType_TextToText:
							currentBufferStepNumber = m_currentTextToTextBufferStepNumber;
							break;

						case BufferType_TextToLine:
							currentBufferStepNumber = m_currentTextToLineBufferStepNumber;
							break;

						case BufferType_TextToPoint:
							currentBufferStepNumber = m_currentTextToPointBufferStepNumber;
							break;

						default:
							assert( false );
					}
					return currentBufferStepNumber;
				}

				/** Set the current buffer size reduction step number.
				 * @see GetBufferStepNumber
				 * @reqid 001.0063
				 */
				void SetBufferStepNumber( BufferType bufferType, int currentBufferStepNumber )
				{
					switch ( bufferType )
					{
						case BufferType_TextToText:
							m_currentTextToTextBufferStepNumber = currentBufferStepNumber;
							break;

						case BufferType_TextToLine:
							m_currentTextToLineBufferStepNumber = currentBufferStepNumber;
							break;

						case BufferType_TextToPoint:
							m_currentTextToPointBufferStepNumber = currentBufferStepNumber;
							break;

						default:
							assert( false );
					}
				}

				/** Get the current buffer size, possibly reflecting a buffer related persistence operation
				 *	@return the current buffer size in inches
				 *	@reqid 001.0063
				 */
				float GetCurrentBufferSize( BufferType bufferType ) const
				{
					float bufferSize = 0.0f;

					switch( bufferType )
					{
					case BufferType_TextToText:
						bufferSize = m_currentTextToTextBufferSize;
						break;

					case BufferType_TextToLine:
						bufferSize = m_currentTextToLineBufferSize;
						break;

					case BufferType_TextToPoint:
						bufferSize = m_currentTextToPointBufferSize;
						break;

					default:
						assert( false );
					}

					return bufferSize;
				}

				/** Set the current buffer size.
				 * @see GetCurrentBufferSize
				 * @reqid 001.0063
				 */
				void SetCurrentBufferSize( BufferType bufferType, float currentBufferSize )
				{
					switch( bufferType )
					{
					case BufferType_TextToText:
						m_currentTextToTextBufferSize = currentBufferSize;
						break;

					case BufferType_TextToLine:
						m_currentTextToLineBufferSize = currentBufferSize;
						break;

					case BufferType_TextToPoint:
						m_currentTextToPointBufferSize = currentBufferSize;
						break;

					default:
						assert( false );
					}
				}

				/** Get the font size of the first text object constituting the original label.
				 * @return the font size of the original label's first text object
				 * @reqid 001.0063
				 */
				float GetOriginalFontSize()
				{
					return m_originalFontSize;
				}

				/** Set the original font size used for computing buffer reduction rule properties.
				 * @param originalFontSize	- the original label's font size
				 * @see GetOriginalFontSize
				 * @reqid 001.0063
				 */
				void SetOriginalFontSize( float originalFontSize )
				{
					m_originalFontSize = originalFontSize;
				}

				//////////////////////////////////////////////////////////////////////////////
				// Font size commands

				/** Get the current font size reduction step number.
				 *	@return the current font size reduction step number
				 *	@reqid 001.0063
				 */
				int GetFontStepNumber() const
				{
					return m_currentFontStepNumber;
				}

				/** Set the current font size reduction step number.
				 * @see GetFontStepNumber
				 * @reqid 001.0063
				 */
				void SetFontStepNumber( int currentFontStepNumber )
				{
					m_currentFontStepNumber = currentFontStepNumber;
				}

				/** Get the number of text objects in the working label.
				 *	@return the number of text objects in the working label
				 *	@reqid 001.0063
				 */
				int GetNumWorkingTextObjects() const
				{
					return m_numTextObjects;
				}

				/** Set the number of text objects in the working label.
				 * @see GetNumWorkingTextObjects
				 * @reqid 001.0063
				 */
				void SetNumWorkingTextObjects( int numTextObjects )
				{
					m_numTextObjects = numTextObjects;
				}

				/** Get the text objects constituting the working label.
				 *	@return an array of pointers to the text objects constituting the working label
				 *	@reqid 001.0063
				 */
				TextObject** GetWorkingTextObjects() const
				{
					return m_workingTextObjects;
				}

				/** Set the text objects constituting the working label.
				 * @see GetWorkingTextObjects
				 * @reqid 001.0063
				 */
				void SetWorkingTextObjects( TextObject** workingTextObjects )
				{
					m_workingTextObjects = workingTextObjects;
				}

				/** Get the font reduction rules for the text objects constituting the working label.
				 *	@return an array of font reduction rules for the text objects constituting the working label
				 *	@reqid 001.0063
				 */
				FontReductionGuide* GetFontReductionGuides() const
				{
					return m_fontReductionGuides;
				}

				/** Set the font reduction rules for the text objects constituting the working label.
				 * @see GetFontReductionGuides
				 * @reqid 001.0063
				 */
				void SetFontReductionGuides( FontReductionGuide* fontReductionGuides )
				{
					m_fontReductionGuides = fontReductionGuides;
				}

				/** Get the font sizes of the text objects constituting the original label.
				 *	@return an array of font sizes of the text objects constituting the original label
				 *	@reqid 001.0063
				 */
				float* GetOriginalFontSizes() const
				{
					return m_originalFontSizes;
				}

				/** Set the font sizes of the text objects constituting the original label.
				 * @see GetWorkingTextObjects
				 * @reqid 001.0063
				 */
				void SetOriginalFontSizes( float* originalFontSizes )
				{
					m_originalFontSizes = originalFontSizes;
				}

				void ClearFontReductionState()
				{
					delete[] m_originalFontSizes;
					delete[] m_fontReductionGuides;
					delete[] m_workingTextObjects;	// We do not own the text objects referenced by array elements.

					m_originalFontSizes = NULL;
					m_fontReductionGuides = NULL;
					m_workingTextObjects = NULL;

					m_numTextObjects = 0;
					m_currentFontStepNumber = 0;
				}

				/**
				 * Make deep copy of one instance's members to another.
				 * @param src source object
				 * @param dest destination object
				 * @param error an error object to be populated
				 * @reqid 001.0076
				 */
				static void Copy( const PlacementState& src, PlacementState& dest, Error &error );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				Labels m_candidateLabels;
				CandidatePositions m_candidatePositions;
				CandidatePositions m_leaderCandidatePositions;
				Label* m_workingLabel;
				bool m_wasPlaced;

				//////////////////////////////////////////////////////////////////////////////
				// Feature type custom placement state properties
				CustomPlacementState* m_areaPlacementState;
				CustomPlacementState* m_linePlacementState;
				CustomPlacementState* m_pointPlacementState;

				//////////////////////////////////////////////////////////////////////////////
				// LoopCommand placement state properties

				// Overhang command
				ExecutionSequence *m_persistenceExecSeq;
				bool m_currentlyInPersistenceHandler;
				float m_overhangAmount;

				// Stack command
				int m_currentNumStackLevels;

				// Hide command
				HideAttempts::ConstIterator* m_hideAttemptsIterator;

				// Buffer size commands
				BufferReductionGuide m_bufferReductionGuide;
				int m_currentTextToTextBufferStepNumber;
				int m_currentTextToLineBufferStepNumber;
				int m_currentTextToPointBufferStepNumber;
				float m_currentTextToTextBufferSize;
				float m_currentTextToLineBufferSize;
				float m_currentTextToPointBufferSize;
				float m_originalFontSize;	// Apply buffer reduction % to first text object's original font size.

				// Font size commands
				int m_currentFontStepNumber;
				int m_numTextObjects;
				TextObject** m_workingTextObjects;
				FontReductionGuide* m_fontReductionGuides;
				float* m_originalFontSizes;	// Apply font reduction % to each text object's original font size.
		};

		/** Class holding a LabelRequest that the user needs to populate to request a placement
		 *	@reqid 001.0063
		 */
		class LABELINGENGINE_API LabelRequest
		{	
			public:
				/**
				* Possible error codes
				* @reqid 001.0063
				*/
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_NullParameter,
					ErrorCode_NullLabelClass,
				};

				/**
				 * Possible values of placement status
				 * @reqid 001.0063
				 */
				enum PlacementStatus
				{
					PlacementStatus_All = 0,
					PlacementStatus_UnPlaced = 1,
					PlacementStatus_Supressed = 2,
					PlacementStatus_PlacedWithHiding = 4,
					PlacementStatus_Placed = 8,
					PlacementStatus_Forced = 16,
					PlacementStatus_Invisible =
					PlacementStatus_UnPlaced	|
					PlacementStatus_Supressed,
					PlacementStatus_Visible =
					PlacementStatus_PlacedWithHiding	|
					PlacementStatus_Placed						|
					PlacementStatus_Forced,
				};

				/** Constructor
				 * @param originalLabel	- The original label as supplied by the caller to be placed. Can't be NULL.
				 * @param labelClass - The label class to associate with the label request.
				 * @reqid 001.0063
				 */
				LabelRequest( Label* originalLabel,
					LabelClass& labelClass, Error& error );

				/** Constructor
 				* @param originalLabel	- The original label as supplied by the caller to be placed. Can't be NULL.
 				 * @param labelClass			the label class to associate with the label request
				 * @param feature					the feature associated with the label
				 * @reqid 001.0063
				 */
				LabelRequest( Label *originalLabel,
					LabelClass& labelClass,
					Feature &feature, Error& error );

				/** Destructor
				 *	@reqid 001.0063
				 */
				~LabelRequest();

				/**
				 * This flag allows to enable/disable a label request. If set to true
				 * the label request is processed by the engine placement algorithms.
				 * If it's false then the request is ignored and the placement engine
				 * set its placement status to PlacementStatus_UnPlaced.
				 * @return - true if request os enabled, false otherwise.
				 * @reqid 001.0063
				 */
				bool GetIsEnabled() const
				{
					return m_isEnabled;
				}

				/**
				 * @see GetIsEnabled
				 * @param isEnabled - new value of the enabled flag.
				 * @reqid 001.0063
				 */
				void SetIsEnabled( bool isEnabled )
				{
					m_isEnabled = isEnabled;
				}

				/**
				   If any one of the lines of the label to be placed is longer than the input length, this
				   method returns true, otherwise, false.
				 * @param maxLabelLength - maximum label length allowed.
				 * @param error - standard error object
				 * @return true->label too long, false->label not too long
				 * @reqid TODO
				 */
				bool IsLabelTooLong(Error& error);

				/** Get the original label.
				 * The original label is the initial label that is to be placed.
				 * @return the original label
				 * @reqid 001.0063
				 */
				const Label* GetOriginalLabel() const
				{
					return m_originalLabel;
				}

				/** 
				 * Set the original label. Ownership is taken.
				 * @see GetOriginalLabel
				 * @param error - [out] error info
				 * @reqid 001.0063, 006.0088
				 */
				void SetOriginalLabel( Label* originalLabel, Error& error )
				{
					if ( originalLabel == NULL )
					{
						SET_ERROR( error, ErrorCode_NullParameter, "OriginalLabel cannot be NULL." );
						return;
					}

					delete m_originalLabel;
					m_originalLabel = originalLabel;
				}

				/** Get the working label.
				 * The working label is a copy of the original label reflecting
				 * any property changes made by persistence commands.
				 * This is a convenience method that retrieves the working label from the placement state.
				 * @return the working label
				 * @reqid 006.0088
				 */
				Label* GetWorkingLabel( Error& error )
				{
					Label* workingLabel = NULL;

					workingLabel = m_placementState.GetWorkingLabel();

					if ( ( workingLabel == NULL ) && ( m_originalLabel != NULL) )
					{
						workingLabel = m_originalLabel->Clone( error );
						if ( error )
						{
							return NULL;
						}
						m_placementState.SetWorkingLabel( *workingLabel );
					}

					return workingLabel;
				}

				/** 
				 * Get the PlacedLabel, the label that has been placed for this label request.
				 * @return the placed label
				 * @reqid 001.0063
				 */
				Label* GetPlacedLabel() const
				{
					return m_placedLabel;
				}

				/** Set the PlacedLabel.
				 * @see	GetPlacedLabel
				 * @reqid 001.0063
				 */
				void SetPlacedLabel( Label* label )
				{
					delete m_placedLabel;

					m_placedLabel = label;
				}

				/** Get the feature. This is the feature associated with the original label.
				 * @return the feature associated with the original label
				 * @reqid 001.0063
				 */
				Feature* GetFeature() const
				{
					return m_feature;
				}

				/** Set the feature
				 * @param	feature		LabelRequest does not take ownership
				 * @see GetFeature
				 * @reqid 001.0063
				 */
				void SetFeature( Feature& feature )
				{
					m_feature = &feature;
				}

				/** Get the placement state, the data that constitutes the state of label placement.
				 * @return the placement state
				 * @reqid 001.0063
				 */
				PlacementState* GetPlacementState()
				{
					return &m_placementState;
				}

				/** Get the associated label class.
				 * @return the address of the label class with which this label request is associated
				 * @reqid 001.0063
				 */
				LabelClass* GetLabelClass() const
				{
					return m_labelClass;
				}

				/** Set the associated label class.
				 * @see GetLabelClass
				 * @reqid 001.0063
				 */
				void SetLabelClass( LabelClass& labelClass )
				{
					m_labelClass = &labelClass;
				}

				/** Get the placement angle.
				 *	@return the angle at which the positioned label is to rotated, in radians
				 *	@reqid 001.0063
				 */
				float GetPlacementAngle() const
				{
					return m_placementAngle;
				}

				/** Set the placement angle.
				 * @see GetPlacementAngle
				 * @reqid 001.0063
				 */
				void SetPlacementAngle( float angle )
				{
					m_placementAngle = angle;
				}

				/** Get the PlacementStatus describing the result of label placement.
				 * @return the PlacementStatus associated with this label request
				 * @see PlacementStatus
				 * @reqid 001.0063
				 */
				PlacementStatus GetPlacementStatus()  const
				{
					return m_placementStatus;
				}

				/** Set the PlacementStatus describing the result of label placement.
				 *	@see GetPlacementStatus
				 *	@see PlacementStatus
				 *	@reqid 001.0063
				 */
				void SetPlacementStatus( PlacementStatus status )
				{
					m_placementStatus = status;
				}

				/** Get the Placement Priority of this label request.
				 * Computed from the label class priority and the placement status, so as
				 * to minimize jumping labels while honoring the user specified priority.
				 * @return the PlacementPriority associated with this label request
				 * @reqid 001.0063
				 */
				int GetPlacementPriority()  const
				{
					return m_placementPriority;
				}

				/** Set the Priority of this label request.
				 *	@see GetPlacementPriority
				 *	@reqid 001.0063
				 */
				void SetPlacementPriority( int placementPriority )
				{
					m_placementPriority = placementPriority;
				}

				/** Get the Placement sequence of this label request.
				 * For label requests inherited from the previous frame, it reflects the order in which
				 * the label requests were honored in that frame.  For new label requests in this frame,
				 * it reflects the order in which they are added to the array of label requests.
				 * @return the PlacementSequence associated with this label request
				 * @reqid 001.0063
				 */
				int GetPlacementSequence()  const
				{
					return m_placementSequence;
				}

				/** Set the Placement sequence of this label request.
				 *	@see GetPlacementSequence
				 *	@reqid 001.0063
				 */
				void SetPlacementSequence( int placementSequence )
				{
					m_placementSequence = placementSequence;
				}

				/**
				 * Reset the label request. This means all the placement algorithms should
				 * start from scratch. I.e. this means remove the placement state and change
				 * the placement status.
				 * @return 
				 * @reqid 001.0063
				 */
				void Reset()
				{
					m_placementState.ClearPlacementHistory();
					m_placementStatus = PlacementStatus_UnPlaced;

					delete m_placedLabel;
					m_placedLabel = NULL;
				}

				void ClearPlacementState()
				{
					m_placementState.ClearState();

					PlacementRule* placementRule = m_labelClass->GetPlacementRule();
					assert( placementRule != NULL );

					m_placementState.Initialize( *m_originalLabel, *placementRule );
				}

				/**
				* Clear out the candidate labels and candidate positions collections.
				* This is invoked at the end of each frame in the process controller
				* to avoid leaking memory from frame to frame.
				* @reqid 001.0063
				*/
				void ClearPlacementHistory()
				{
					m_placementState.ClearPlacementHistory();
				}

				/**
				* Creates copy of self.
				* @param error an error object to be populated
				* @param parentMap (optional) a new parent map for a cloned object
				* @return copy of self
				* @reqid 001.0076
				*/
				virtual LabelRequest* Clone( Error& error, Map* parentMap = NULL ) const;

				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @param error an error object to be populated
				* @param parentMap (optional) a new parent map for a copy
				* @reqid 001.0076
				*/
				static void Copy( const LabelRequest& src, LabelRequest& dest, Error &error, Map* parentMap );

				/**
				 * Validate the label request.
				 * @param valid - [out] True if the label request is valid. False otherwise.
				 * @param validationErrors - [out] If problems are present the details are stored here.
				 * @param error - [out] error info.
				 * @reqid 001.0086
				 */
				void Validate( bool* valid, ErrorDetails** validationErrors, Error& error ) const;

#if defined(_DEBUG) || defined(DISPLAY_DEBUGGER)
				/* Get the label requests index within the array view
				* @return index into array of labels
				*/
				int GetLabelsArrayIndex() const
				{
					return m_labelIndex;
				}

				/* Set the label requests index within the array view
				 * @param arrayIndex - the labels array index
				 */
				void SetLabelsArrayIndex(int arrayIndex)
				{
					m_labelIndex=arrayIndex;
				}
#endif

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

				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static LabelRequest* DeserializeNew( IDeserializer& deserializer, Error& error );
#endif

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				Label* m_originalLabel;
				Label* m_placedLabel;
				Feature* m_feature;
				LabelClass* m_labelClass;
				PlacementState m_placementState;
				float m_placementAngle;
				PlacementStatus m_placementStatus;
				bool m_isEnabled;
				int m_placementPriority;
				int m_placementSequence;
				
				/**Default constructor. Cloning only.
				 *	@reqid 001.0063
				 */
				LabelRequest();

#if defined(_DEBUG) || defined(DISPLAY_DEBUGGER)
				int m_labelIndex;
#endif
		};

		/**
		 * Class that holds an array of derived type LabelRequest object pointers.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API LabelRequests : public ClonableLinkedList < LabelRequest, LabelRequests >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				LabelRequests()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~LabelRequests()
				{
				}
		};

		//ComponetActivator classes

		/**
		 *	Base class for all ComponentActivator classes. Abstract.
		 *
		 *	ComponentActivator subclasses allow the user to define conditional restructuring of the Assembly Label. The ComponentActivator 
		 *	base pointer is a member of the AssemblyLabelDefinition class, and needs to be initialized with subclass specific configuration
		 *  data in order for the DTP engine to apply the Assembly Label restructuring functionality.
		 *	Once the subclass is instantiated and set up ( @see LabelDirectionComponentActivator, @see FlipTextComponentActivator
		 *	@see LineOverhangComponentActivator for subclass specific examples ), make sure to call the 
		 *	@see AssemblyLabelDefinition::SetComponentActivator method to assign an Activator to an AssemblyLabelDefinition.
		 *
		 *	Note: All activators require the configuration of a set or sets of Assembly Components, these can either be:
		 *	Text or Symbol Components for the LabelDirectionComponentActivator and LineOverhangComponentActivator
		 *	Text Components only for the FlipTextComponentActivator
		 *
		 * @reqid 006.0080
		 */
		class LABELINGENGINE_API ComponentActivator
		{
		public:

			/**
			* Possible error codes
			* @reqid 006.0080			
			*/
			enum ErrorCode
			{
				ErrorCode_AllocationFailure = 1,
				ErrorCode_InvalidActivatorType
			};

			/**
			 * Possible subclass types for the ComponentActivator family
			 * @reqid 006.0080
			 */
			enum Type
			{
				ComponentActivator_Undefined = 0,
				ComponentActivator_LabelDirectionComponentActivator,
				ComponentActivator_FlipTextComponentActivator,
				ComponentActivator_LineOverhangComponentActivator
			};

			/** Constructor
			 * @reqid 006.0080	
			 */
			ComponentActivator()
			{
				m_type = ComponentActivator_Undefined;
				m_isRebuildNecessary = true;
				m_condition = 0;
			}

			/** Destructor
			 * @reqid 006.0080	
			 */
			virtual ~ComponentActivator()
			{
			}

			/** Gets the type of the subclass
			 * @return type of the subclass
			 * @reqid 006.0080		
			 */
			Type GetType() const
			{
				return m_type;
			}

			/** Gets the type of the subclass
			 * @return type of the subclass
			 * @reqid 006.0080	
			 */
			bool IsRebuildNecessary() const
			{
				return m_isRebuildNecessary;
			}

			/**
			* Returns a list of names for the Active Components (both text and symbol), for the ComponentActivator.
			* @return a list of Assembly Component names
			* @reqid 006.0080		
			*/
			virtual const SharedString::Collection* GetActiveComponentNames() const = 0;

			/**
			* Returns a list of names for the InActive Components (both text and symbol), for the ComponentActivator.
			* @return a list of Assembly Component names
			* @reqid 006.0080		
			*/
			virtual const SharedString::Collection* GetInActiveComponentNames() const = 0;

			/**
			* Creates copy of self.
			* @param error an error object to be populated
			* @return copy of self
			* @reqid 006.0080
			*/
			virtual ComponentActivator* Clone( Error& error ) const = 0;

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
			static ComponentActivator* DeserializeNew( IDeserializer& deserializer, Error& error );

			/**
			 * Deserializes the object from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			void Deserialize( IDeserializer& deserializer, Error& error );
		protected:
			/**
			 * Child specific serialization.
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

		protected:

			/**
			* Make deep copy of one instance's members to another
			* @param src source object
			* @param dest destination object
			* @param error an error object to be populated
			* @reqid 006.0080
			*/
			static void Copy( const ComponentActivator& src, ComponentActivator& dest )
			{
				dest.m_type = src.m_type;
				dest.m_isRebuildNecessary = src.m_isRebuildNecessary;
			}

			Type m_type;

			int m_condition;

			bool m_isRebuildNecessary;
		};

		/**
		 * A class that enables the user to configure which components of the Assembly Label are to be rotated by 180 degrees during the 
		 * Line Leader placement of Assembly Labels (Activator is ignored for all other types of placement). The Line Leader functionality
		 * contains the logic on when to call upon this Activator for the list of components that are flippable. This functionality is used
		 * to ensure that the defined components of an line leadered Assembly Label will remain upright regardless of the placement angle.

			\code
			MapText::Dtp::SharedString component1( "topFlagComponent" );
			MapText::Dtp::SharedString component2( "bottomFlagComponent" );

			MapText::Dtp::FlipTextComponentActivator *flipTextComponentActivator = new MapText::Dtp::FlipTextComponentActivator();
			assert(flipTextComponentActivator);

			MapText::Dtp::SharedString::Collection *components;

			components = flipTextComponentActivator->GetFlipTextComponents();

			components->AddElement( component1, error );
			if(error)
			{
				return NULL;
			}

			components->AddElement( component2, error );
			if(error)
			{
			return NULL;
			}

			//assuming assemblyLabelDef is predefined somewhere before this code AssemblyLabelDefinition *assemblyLabelDef, most
			//likely returned from a Map or AssemblyLabel object

			assemblyLabelDef->SetComponentActivator( flipTextComponentActivator );
			\endcode

			This will result in Text Components component1 and component2 always remaining upright during the line leader placement.

		 *
		 * @reqid 006.0080
		 */
		class LABELINGENGINE_API FlipTextComponentActivator : public ComponentActivator
		{
		public:

			/** Constructor
			 * @reqid 006.0080
			 */
			FlipTextComponentActivator() : 
			m_flipTextComponents( true )
			{
				m_type = ComponentActivator_FlipTextComponentActivator;
				m_isRebuildNecessary = true;
			}

			/** Destructor
			 * @reqid 006.0080
			 */
			~FlipTextComponentActivator()
			{
			}

			/**
			* Returns a list of names for AssemblyComponents that are configured to be flippable
			* @return a list of Assembly Component names
			* @reqid 006.0080
			*/
			SharedString::Collection* GetFlipTextComponentNames()
			{
				return &m_flipTextComponents;
			}

			/**
			* Returns a list of names for AssemblyComponents that are configured to be flippable
			* @param error a list of Assembly Component names
			* @reqid 006.0080
			*/
			const SharedString::Collection* GetActiveComponentNames() const
			{
				return &m_flipTextComponents;
			}

			/**
			* Returns a list of names for AssemblyComponents that are configured to be unflippable
			* @param error a list of Assembly Component names
			* @reqid 006.0080
			*/
			const SharedString::Collection* GetInActiveComponentNames() const
			{
				return NULL;
			}

			/**
			* Creates copy of self.
			* @param error an error object to be populated
			* @return copy of self
			* @reqid 006.0080
			*/
			virtual FlipTextComponentActivator* Clone( Error& error ) const
			{
				FlipTextComponentActivator* copy = new FlipTextComponentActivator();
				if ( copy == NULL )
				{
					SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
				}
				else
				{
					Copy( *this, *copy );
				}
				return copy;					
			}

		protected:

#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/** 
			 * Deserializes the object from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif

			/**
			* Make deep copy of one instance's members to another
			* @param src source object
			* @param dest destination object
			* @param error an error object to be populated
			* @reqid 006.0080
			*/
			static void Copy( const FlipTextComponentActivator& src, FlipTextComponentActivator& dest )
			{
				Error error;

				ComponentActivator::Copy(src, dest);

				dest.m_flipTextComponents.Assign(src.m_flipTextComponents ,error);
				if(error)
				{
					return;
				}
			}

		private:

			SharedString::Collection m_flipTextComponents;
		};

		/** 
		 * A class with a list of Assembly Components that are activated based on whether the label direction follows the feature direction, or follows
		 * the reversed feature direction. Depending on which of the conditions is true, either AlongGeometry components are active or AgainstGeometry components are active.
		 * Please note that LABEL DIRECTION is understood as a vector going from the first letter to the last of the label.
			\code
			MapText::Dtp::SharedString component1( "RightArrow" );
			MapText::Dtp::SharedString component2( "LeftArrow" );

			MapText::Dtp::LabelDirectionComponentActivator *labelDirectionComponentActivator = new MapText::Dtp::LabelDirectionComponentActivator();
			assert(labelDirectionComponentActivator);

			MapText::Dtp::SharedString::Collection *components;

			components = labelDirectionComponentActivator->GetAlongGeometryComponents();

			components->AddElement( component1, error );
			if(error)
			{
				return NULL;
			}
			
			components = labelDirectionComponentActivator->GetAgainstGeometryComponents();

			components->AddElement( component2, error );
			if(error)
			{
				return NULL;
			}

			//assuming assemblyLabelDef is predefined somewhere before this code AssemblyLabelDefinition *assemblyLabelDef, most
			//likely returned from a Map or AssemblyLabel object

			assemblyLabelDef->SetComponentActivator( (MapText::Dtp::LabelDirectionComponentActivator*) labelDirectionComponentActivator );
			\endcode
		   
		   Lets assume that component1 is symbolized by '>' component2 by '<' and there is also another textcomponent that is not part of the
		   Activator definition.

		   This type of configuration allows the line placement (only Tangent and Perpendicular supported) to be configured in a way that component1 
		   will be visible if label follows the direction of the feature :

		   \verbatim
					textcomponent >
			------------------------------------>

		   and component2 to be visible when the label follows the reversed direction of the feature:

					< textcomponent
		    <------------------------------------
		   \endverbatim

		   Note: If the user desires a directional indicator type of placement, where the TextComponents of the Assembly are to remain upright 
		   and the SymbolComponent is to denote a direction, LabelDirection_FollowRightSideUpDirection must be configured on the PlacementRule.
		 *
		 *
		 * @reqid 006.0080
		 */
		class LABELINGENGINE_API LabelDirectionComponentActivator : public ComponentActivator
		{
		public:

			/** Constructor
			 * @reqid 006.0080
			 */
			LabelDirectionComponentActivator() : 
			  m_AlongGeometryComponents( true ),
				  m_AgainstGeometryComponents( true )
			  {
				  m_type = ComponentActivator_LabelDirectionComponentActivator;
				  m_isRebuildNecessary = false;
				  m_condition = (int) Label::LabelDirection_FollowsGeometryDirection;
			  }

			/** Destructor
			 * @reqid 006.0080
			 */
			~LabelDirectionComponentActivator()
			{
			}

			/**
			* Returns a list of names for AssemblyComponents that are configured to be active when the label follows the feature geometry
			* @return a list of Assembly Component names
			* @reqid 006.0080
			*/
			SharedString::Collection* GetAlongGeometryComponents()
			{
			  return &m_AlongGeometryComponents;
			}

			/**
			* Returns a list of names for AssemblyComponents that are configured to be active when the label follows the reversed feature geometry
			* @return a list of Assembly Component names
			* @reqid 006.0080
			*/
			SharedString::Collection* GetAgainstGeometryComponents()
			{
			  return &m_AgainstGeometryComponents;
			}

			/**
			* Returns a list of names for AssemblyComponents that are active ( based on ComputeCondition() )
			* @return a list of Assembly Component names
			* @reqid 006.0080
			*/
			const SharedString::Collection* GetActiveComponentNames() const
			{
				if (m_condition == (int) Label::LabelDirection_FollowsGeometryDirection )
				  return &m_AlongGeometryComponents;
			  else
				  return &m_AgainstGeometryComponents;
			}

			/**
			* Returns a list of names for AssemblyComponents that are inactive ( based on ComputeCondition() )
			* @return a list of Assembly Component names
			* @reqid 006.0080
			*/
			const SharedString::Collection* GetInActiveComponentNames() const
			{
				if (m_condition == (int) Label::LabelDirection_FollowsGeometryDirection )
				  return &m_AgainstGeometryComponents;
			  else
				  return &m_AlongGeometryComponents;
			}

			/**
			* Sets the internal state of the object (activation condition) based on the passed labelDirection
			* @param angle - the angle of the to be placed label
			* @param error - error info
			* @reqid 006.0080
			*/
			void ComputeCondition(int labelDirection);

			/**
			* Creates copy of self.
			* @param error an error object to be populated
			* @return copy of self
			* @reqid 006.0080
			*/
			  virtual LabelDirectionComponentActivator* Clone( Error& error ) const
			  {
				  LabelDirectionComponentActivator* copy = new LabelDirectionComponentActivator();
				  if ( copy == NULL )
				  {
					  SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
				  }
				  else
				  {
					  Copy( *this, *copy );
				  }
				  return copy;					
			  }

		protected:

#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/** 
			 * Deserializes the object from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
			/**
			* Make deep copy of one instance's members to another
			* @param src source object
			* @param dest destination object
			* @param error an error object to be populated
			* @reqid 006.0080
			*/
			static void Copy( const LabelDirectionComponentActivator& src, LabelDirectionComponentActivator& dest )
			{
				Error error;

				ComponentActivator::Copy(src, dest);

				dest.m_AlongGeometryComponents.Assign(src.m_AlongGeometryComponents ,error);
				if(error)
				{
					return;
				}

				dest.m_AgainstGeometryComponents.Assign(src.m_AgainstGeometryComponents ,error);
				if(error)
				{
					return;
				}
			}

		private:

			SharedString::Collection m_AlongGeometryComponents;
			SharedString::Collection m_AgainstGeometryComponents;
		};


		/** 
		 * A class with a list of Assembly Components that are activated if the placement of the AssemblyLabel overhangs a line feature or not.
		 * If the Assembly Label overhangs the end points of a line feature, one set of components is activated, if it doesn't, another set of 
		 * components is.
		 * 
		 *
			\code
			MapText::Dtp::SharedString component1( "Elevation" );
			MapText::Dtp::SharedString component2( "Bearing" );

			MapText::Dtp::LineOverhangComponentActivator *lineOverhangComponentActivator = new MapText::Dtp::LineOverhangComponentActivator();
			assert(lineOverhangComponentActivator);

			MapText::Dtp::SharedString::Collection *components;

			components = lineOverhangComponentActivator->GetLineOverHangComponents();

			components->AddElement( component1, error );
			if(error)
			{
				return NULL;
			}
			
			components->AddElement( component2, error );
			if(error)
			{
				return NULL;
			}

			components = lineOverhangComponentActivator->GetLineNoOverhangComponents();

			components->AddElement( component1, error );
			if(error)
			{
				return NULL;
			}

			//assuming assemblyLabelDef is predefined somewhere before this code AssemblyLabelDefinition *assemblyLabelDef, most
			//likely returned from a Map or AssemblyLabel object

			assemblyLabelDef->SetComponentActivator( (MapText::Dtp::LineOverhangComponentActivator*) lineOverhangComponentActivator );
			\endcode
		   
		   Lets assume that component1 denotes the Elevation chart data and component2 the Bearing chart data and an additional textcomponent
		   that is not part of the Activator definition

		   This type of configuration allows the line placement (only Tangent and Perpendicular supported) to be configured in a way that 
		   components: component1 and component2 will be visible if Assembly Label is overhanging the line but only component1 will be visible
		   when the AssemblyLabel is not overhaning the line:

		   \verbatim

		   Overhanging									Not Overhanging
				
				|											|
				|											|
				|											|
				|											|
		   textc|omponent								text|component
		   Eleva|tion									Elev|ation
		   Bearing											|

		  \endverbatim
		 *
		 *
		 * @reqid 006.0080
		 */
		class LABELINGENGINE_API LineOverhangComponentActivator : public ComponentActivator
		{
		public:

			/** Constructor
			 * @reqid 006.0085
			 */
			LineOverhangComponentActivator() : 
			  m_LineOverhangComponents( true ),
				  m_LineNoOverhangComponents( true )
			  {
				  m_type = ComponentActivator_LineOverhangComponentActivator;
				  m_isRebuildNecessary = false;
				  m_condition = 1;
			  }

			/** Destructor
			 * @reqid 006.0085
			 */
			~LineOverhangComponentActivator()
			{
			}

			/**
			* Returns a list of names for AssemblyComponents that are configured to be active in when the AssemblyLabel is overhanging a line.
			* @return a list of Assembly Component names
		    * @reqid 006.0085
			*/
			SharedString::Collection* GetLineOverHangComponents()
			{
			  return &m_LineOverhangComponents;
			}

			/**
			* Returns a list of names for AssemblyComponents that are configured to be active in when the AssemblyLabel is NOT overhanging a line.
			* @return a list of Assembly Component names
			* @reqid 006.0085
			*/
			SharedString::Collection* GetLineNoOverhangComponents()
			{
			  return &m_LineNoOverhangComponents;
			}

			/**
			* Returns a list of names for AssemblyComponents that are active ( based on ComputeCondition() )
			* @return a list of Assembly Component names
			* @reqid 006.0085
			*/
			const SharedString::Collection* GetActiveComponentNames() const
			{
			  if (m_condition == 1)
				  return &m_LineOverhangComponents;
			  else
				  return &m_LineNoOverhangComponents;
			}

			/**
			* Returns a list of names for AssemblyComponents that are inactive ( based on ComputeCondition() )
			* @return a list of Assembly Component names
			* @reqid 006.0085
			*/
			const SharedString::Collection* GetInActiveComponentNames() const
			{
			  if (m_condition == 1)
				  return &m_LineNoOverhangComponents;
			  else
				  return &m_LineOverhangComponents;
			}

			/**
			* Sets the internal state of the object (activation condition) based on the passed in overhang flag
			* @param isOverhang - overhang flag for the current AssemblyLabel candidate
			* @param error - error info
			* @reqid 006.0085
			*/
			void ComputeCondition(bool isOverhang);

			/**
			* Creates copy of self.
			* @param error an error object to be populated
			* @return copy of self
			* @reqid 006.0085
			*/
			virtual LineOverhangComponentActivator* Clone( Error& error ) const
			{
			  LineOverhangComponentActivator* copy = new LineOverhangComponentActivator();
			  if ( copy == NULL )
			  {
				  SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
			  }
			  else
			  {
				  Copy( *this, *copy );
			  }
			  return copy;					
			}

		protected:

			/**
			* Make deep copy of one instance's members to another
			* @param src source object
			* @param dest destination object
			* @param error an error object to be populated
			* @reqid 006.0085
			*/
			static void Copy( const LineOverhangComponentActivator& src, LineOverhangComponentActivator& dest )
			{
				Error error;

				ComponentActivator::Copy(src, dest);

				dest.m_LineOverhangComponents.Assign(src.m_LineOverhangComponents ,error);
				if(error)
				{
					return;
				}

				dest.m_LineNoOverhangComponents.Assign(src.m_LineNoOverhangComponents ,error);
				if(error)
				{
					return;
				}
			}

#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/** 
			 * Deserializes the object from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif

		private:

			SharedString::Collection m_LineOverhangComponents;
			SharedString::Collection m_LineNoOverhangComponents;
		};
	} // namespace Dtp
} // namespace MapText

#endif // DTP_LABEL_H
