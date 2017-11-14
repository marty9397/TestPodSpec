/**************************************************
 * Copyright (c)2008 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://martin@mtsrv.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/LabelingEngine/PlacementCommand.h $
 *
 * Revision: $Revision: 79292 $
 *
 * Last modified on: $Date: 2015-12-08 16:12:39 -0500 (Tue, 08 Dec 2015) $, by: $Author: mfraczak $
 *
 *************************************************/

#ifndef DTP_PLACEMENTCOMMAND_H
#define DTP_PLACEMENTCOMMAND_H

#include "LabelingEngineDefs.h"
#include "Collection.h"
#include "MtString.h"
#include "MtGeometry.h"
#include "Map.h"
#include "LabelElement.h"

#include "Serialization.h"

namespace MapText
{
	namespace Dtp
	{
		/**
		 * Forward declare pointer-only dependencies
		 */
		class Error;
		class LabelRequest;
		class SequenceCommand;
		class StringParser;

		/**
		 * Possible point instruction index values
		 * @reqid 001.0063
		 */
		enum PointInstruction
		{
			PointInstruction_PointPos = 0,
			PointInstruction_Count
		};

		/**
		 * Possible line instruction index values
		 * @reqid 001.0063
		 */
		enum LineInstruction
		{
			LineInstruction_Above,
			LineInstruction_Below,
			LineInstruction_Middle,
			LineInstruction_Left,
			LineInstruction_Right,
			LineInstruction_Count
		};

		/**
		 * Possible area instruction index values
		 * @reqid 001.0063
		 */
		enum AreaInstruction
		{
			AreaInstruction_AreaPlace,
			AreaInstruction_Count
		};

		/**
		 * Possible persistence instruction index values
		 * @reqid 001.0063
		 */
		enum PersistenceInstruction
		{
			PersistenceInstruction_Offset,
			PersistenceInstruction_Overhang,
			PersistenceInstruction_Overline,
			PersistenceInstruction_Overregion,
			PersistenceInstruction_BufferSize,
			PersistenceInstruction_FontSize,
			PersistenceInstruction_Stack,
			PersistenceInstruction_Hide,
			PersistenceInstruction_Bump,
			PersistenceInstruction_Leader,
			PersistenceInstruction_Force,
			PersistenceInstruction_ForceLeader,
			PersistenceInstruction_Suppress,
			PersistenceInstruction_Count
		};

		/**
		 * Sequence of commands
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API ExecutionSequence : public ClonableLinkedList< SequenceCommand, ExecutionSequence >
		{
			public:

				enum ErrorCode
				{
					ErrorCode_InvalidSequence = 2,
					ErrCode_NoPreviousCommand,
					ErrorCode_PreviousCommandNotPlacementType
				};

				/**
				 * Default constructor
				 * @param error		an error object to be populated
				 * @reqid 001.0063
				 */
				ExecutionSequence() 
					: ClonableLinkedList< SequenceCommand, ExecutionSequence >( true )
				{
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~ExecutionSequence()
				{
				}

				/**
				 * Add an element to the sequence. It handles looping commands in a special way
				 * to establish the return targets.
				 * @param element - Command to be added. Ownership is taken.
				 * @param error - [out] Error info.
				 * @reqid 001.0063
				 */
				void AddElement( SequenceCommand* element, Error& error );

				/**
				 * Creates deep copy of self.
				 * @param error an error object to be populated
				 * @param cloner an optional Cloner object for use in cloning the commands in the sequence
				 * @return copy of self
				 * @reqid 001.0063
				 */
				ExecutionSequence* Clone( Error& error, const Cloner<SequenceCommand>& cloner = Cloner<SequenceCommand>() );

			private:
				/**
				 * Sets the return target for a LoopCommand instance and adds the .
				 * @param element - Command to be added. Ownership is taken.
				 * @param error - [out] Error info.
				 * @reqid 001.0063
				 */
				void AddLoopCommand( SequenceCommand* element, Error& error );
		};

		/**
		 * Base class for sequence commands.
		 * Derived classes defined for point, line, area, and persistence.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API SequenceCommand
		{
			public:

				/**
				* Possible error codes
				* @reqid 001.0063
				*/
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_MissingCommandHandler
				};

				/**
				 * Type of the sequence command
				 * @reqid 001.0063
				 */
				enum CommandType
				{
					CommandType_LinePlacement,
					CommandType_AreaPlacement,
					CommandType_PointPlacement,
					CommandType_Persistence,
					CommandType_Failure
				};

				/**
				 * Default constructor.
				 * @reqid 001.0063
				 */
				SequenceCommand()
				{
					m_commandOrdinal = LineInstruction_Above;
					m_sequenceCommandType = CommandType_LinePlacement;
				}

				/**
				 * Create a new sequence command for a given specific command
				 * @param commandType type of the packaged command
				 * @param commandOrdinal index into specialized instruction enumeration for this command type
				 * @reqid 001.0063
				 */
				SequenceCommand( CommandType commandType, int commandOrdinal )
					:	m_sequenceCommandType( commandType ),
						m_commandOrdinal(commandOrdinal )
				{
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				virtual ~SequenceCommand()
				{
				}

				/**
				 * Get the ordinal of the derived command.
				 * @return the index into the command enumeration for the specified command type
				 * Used to index into the implementing function pointer array.
				 * @reqid 001.0063
				 */
				int GetCommandOrdinal() const
				{
					return m_commandOrdinal;
				}

				/**
				 * Get the type of the command - Line, Point, Area, or Persistence.
				 * @return the type of the command as a value from the CommandType enumeration
				 * @reqid 001.0063
				 */
				CommandType GetCommandType() const
				{
					return m_sequenceCommandType;
				}

				/**
				 * Invoke command handler
				 * @param map - The map on which labeling is being done.
				 * @param labelRequest - The request for labeling.
				 * @param bSuccess - [out] If true is returned it means the placement succeeded.
				 * @param error - [out] Error information.
				 * @param returnLimit - Used by looping command when the loop is nested. In such 
				 *                      case the inner loop should never go back beyond the 
				 *                      return point of the outer loop. This limit may override
				 *                      the pre-set return target of the loop command.
				 * @reqid 001.0063
				 */
				virtual void Execute( Map *map, LabelRequest* labelRequest,
					bool* bSuccess, Error& error, ExecutionSequence::Iterator *returnLimit = NULL ) = 0;

				/**
				 * Creates deep copy of self.
				 * @param error an error object to be populated
				 * @return copy of self
				 * @reqid 001.0076
				 */
				virtual SequenceCommand* Clone( Error& error ) const = 0;

				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @reqid 001.0076
				*/
				static void Copy( const SequenceCommand& src, SequenceCommand& dest )
				{
					dest.m_sequenceCommandType = src.m_sequenceCommandType;
					dest.m_commandOrdinal = src.m_commandOrdinal;
				}

				/**
				 * SequenceCommand which implements looping should override this method
				 * and return true. Otherwise the default implementation returns false.
				 * @return - true if the command is a looping command
				 * @reqid 001.0076
				 */
				virtual bool ImplementsLooping() const
				{
					return false;
				}

				bool IsPlacementCommand()
				{
					bool bPlacementCommand = false;

					if (	( GetCommandType() != CommandType_Persistence ) &&
								( GetCommandType() != CommandType_Failure ) )
					{
						bPlacementCommand = true;
					}
					else
					if (	( GetCommandType() == CommandType_Persistence ) &&
								( GetCommandOrdinal() == PersistenceInstruction_Leader ) )
					{
						bPlacementCommand = true;
					}
					return bPlacementCommand;
				}

				/**
				 * Validate the command. Default implementation simply returns true. If the command
				 * deriving from this class needs to implement additional validation it should overload
				 * this method.
				 * @param featType - feature type to proceed with some feature-type-dependent validations
				 * @param valid[out] - True if the command is valid. False otherwise, details appended
				 *                through validationErrors.
				 * @param validationErrors[out] - Validation issues are reported here.
				 * @param error[out] - Other critical errors which may occur during 
				 *                the validation process are reported here. They would NOT
				 *                be caused by invalid input data.
				 * @reqid 001.0086
				 */
				virtual void Validate( FeatureType, bool* valid, ErrorDetails**,
                                      Error& )
				{
					*valid = true;
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
				static SequenceCommand* DeserializeNew( IDeserializer& deserializer, Error& error );

		protected:
				/**
				 * Serialization - derived class specific.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				virtual void _Serialize( ISerializer& emitter, Error& error ) const = 0;
			
				/**
				 * Deserializes the child class instance from a deserializer.
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
			protected:
				CommandType m_sequenceCommandType;
				int m_commandOrdinal;
		};

		/**
		 * Specifies where/how to place the label along the line feature
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API LinePlacementCommand:public SequenceCommand
		{
			public:
				/**
				 * Possible choices of where to place the label. For each, one specifies the ideal location and the range
				 * within which a search can be performed to find an acceptable location if the ideal location is not
				 * acceptable.  The range is specified by two locations, the minimum position and maximum position along
				 * the line feature within which the search can be performed.
				 * @reqid 001.0063
				 */
				enum PositionWhere
				{
					PositionWhere_AtStartOfFeature = 0,
					PositionWhere_AtEndOfFeature,
					PositionWhere_AtCenterOfFeature
				};

				/**
				 * The possible choices for placement
				 * @reqid 001.0063
				 */
				enum PlaceWhere
				{
					PlaceWhere_Above = 0, // The entire label is to be placed above the line feature.
					PlaceWhere_Below, // The entire label is to be placed below the line feature.
					PlaceWhere_Middle, // The entire label is to be placed on the line feature.
					PlaceWhere_Left, // The label is placed on the left side of the feature based on the direction of the vertices.
					PlaceWhere_Right, // The label is placed on the right side of the feature based on the direction of the vertices.
				};

				/**
				 * Constructor
				 * @reqid 001.0063
				 */
				LinePlacementCommand()
				{
					m_positionWhere = PositionWhere_AtCenterOfFeature;
					m_initialPosition = 50;
					m_minPosition = 0;
					m_maxPosition = 100;
					m_initialPositionUnitType = UnitType_Percentage;
					m_maxPositionUnitType = UnitType_Percentage;
					m_minPositionUnitType = UnitType_Percentage;

					m_placeWhere = PlaceWhere_Above;
					m_commandOrdinal = LineInstruction_Above;
					m_sequenceCommandType = CommandType_LinePlacement;
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~LinePlacementCommand()
				{}

				/**
				 * Sets where to position the label
				 * @param positionWhere - AtStartOfFeature, AtEndOfFeature, or AtCenterOfFeature
				 * @reqid 001.0063
				 */
				void SetWhereToPosition( PositionWhere positionWhere )
				{
					m_positionWhere = positionWhere;
				}

				/**
				 * Gets where to position the label
				 * @return where to position the label
				 * @reqid 001.0063
				 */
				PositionWhere GetWhereToPosition() const
				{
					return m_positionWhere;
				}

				/**
				 * Sets the ideal location. Specify the value in the units of the PlacementRule if not being given as a percentage.
				 * @param initialPosition - value giving the location
				 * @reqid 001.0063
				 */
				void SetInitialPosition( float initialPosition )
				{
					m_initialPosition = initialPosition;
				}

				/**
				 * Gets the ideal location
				 * @return - value giving the location
				 * @reqid 001.0063
				 */
				float GetInitialPosition() const
				{
					return m_initialPosition;
				}

				/**
				 * Sets the maximum position to which a label can move to avoid overlap. Specify the value in the units of the PlacementRule if not being given as a percentage.
				 * @param maxPosition - distance along the line feature.
				 * @reqid 001.0063
				 */
				void SetMaxPosition( float maxPosition )
				{
					m_maxPosition = maxPosition;
				}

				/**
				 * Gets the maximum position to which a label can move to avoid overlap
				 * @return the distance along the line feature.
				 * @reqid 001.0063
				 */
				float GetMaxPosition() const
				{
					return m_maxPosition;
				}

				/**
				 * Sets the minimum position to which a label can move to avoid overlap. Specify the value in the units of the PlacementRule if not being given as a percentage.
				 * @param minPosition - distance along the line feature.
				 * @reqid 001.0063
				 */
				void SetMinPosition( float minPosition )
				{
					m_minPosition = minPosition;
				}

				/**
				 * Gets the minimum position to which a label can move to avoid overlap
				 * @return the distance along the line feature.
				 * @reqid 001.0063
				 */
				float GetMinPosition() const
				{
					return m_minPosition;
				}

				/**
				* Gets InitialPositionUnitType
				* @return - InitialPositionType
				* @reqid 001.0063
				*/
				UnitType GetInitialPositionUnitType() const 
				{ 
					return m_initialPositionUnitType; 
				}

				/**
				* Sets InitialPositionUnitType
				* @param val - InitialPositionUnitType
				* @reqid 001.0063
				*/
				void SetInitialPositionUnitType(UnitType val)
				{ 
					m_initialPositionUnitType = val; 
				}

				/**
				* Gets MaxPositionUnitType
				* @return - MaxPositionUnitType
				* @reqid 001.0063
				*/
				UnitType GetMaxPositionUnitType() const 
				{ 
					return m_maxPositionUnitType; 
				}

				/**
				* Sets MaxPositionUnitType
				* @param val - MaxPositionUnitType
				* @reqid 001.0063
				*/
				void SetMaxPositionUnitType(UnitType val) 
				{ 
					m_maxPositionUnitType = val; 
				}

				/**
				* Gets MinPositionUnitType
				* @return - MinPositionUnitType
				* @reqid 001.0063
				*/
				UnitType GetMinPositionUnitType() const 
				{ 
					return m_minPositionUnitType; 
				}

				/**
				* Sets MinPositionUnitType
				* @param val - MinPositionUnitType
				* @reqid 001.0063
				*/
				void SetMinPositionUnitType(UnitType val) 
				{ 
					m_minPositionUnitType = val; 
				}

				/**
				* @see GetPlaceWhere
				* @reqid 001.0063
				*/
				void SetWhereToPlace( PlaceWhere placeWhere )
				{
					m_placeWhere = placeWhere;

					if ( m_placeWhere == PlaceWhere_Above )
					{
						m_commandOrdinal = LineInstruction_Above;
					}
					else if ( m_placeWhere == PlaceWhere_Below )
					{
						m_commandOrdinal = LineInstruction_Below;
					}
					else if ( m_placeWhere == PlaceWhere_Middle )
					{
						m_commandOrdinal = LineInstruction_Middle;
					}
					else if ( m_placeWhere == PlaceWhere_Left )
					{
						m_commandOrdinal = LineInstruction_Left;
					}
					else if ( m_placeWhere == PlaceWhere_Right )
					{
						m_commandOrdinal = LineInstruction_Right;
					}
				}

				/**
				* Get value indicating where to place a label
				* @reqid 001.0063
				*/
				PlaceWhere GetPlaceWhere() const
				{
					return m_placeWhere;
				}

				/**
				 * @see SequenceCommand::Clone
				 * @reqid 001.0076
				 */
				virtual LinePlacementCommand* Clone( Error& error ) const
				{
					LinePlacementCommand* copy = new LinePlacementCommand();
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

				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @param error an error object to be populated
				* @reqid 001.0076
				*/
				static void Copy(
					const LinePlacementCommand& src,
					LinePlacementCommand& dest )
				{
					SequenceCommand::Copy( src, dest );

					dest.m_positionWhere = src.m_positionWhere;
					dest.m_placeWhere = src.m_placeWhere;
					dest.m_initialPosition = src.m_initialPosition;
					dest.m_maxPosition = src.m_maxPosition;
					dest.m_minPosition = src.m_minPosition;
					dest.m_initialPositionUnitType = src.m_initialPositionUnitType;
					dest.m_maxPositionUnitType = src.m_maxPositionUnitType;
					dest.m_minPositionUnitType = src.m_minPositionUnitType;
				}

				/**
				 * @see SequenceCommand::Execute
				 * @reqid 001.0063
				 */
				virtual void Execute( Map *map, LabelRequest* labelRequest, bool* bSuccess, Error& error,
					ExecutionSequence::Iterator *returnLimit = NULL );

				/**
				 * @see SequenceCommand::Validate
				 * @reqid 001.0086
				 */
				void Validate( FeatureType featType, bool* valid, ErrorDetails** validationErrors, Error& error );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				PositionWhere m_positionWhere;
				PlaceWhere m_placeWhere;
				float m_initialPosition;
				float m_maxPosition;
				float m_minPosition;
				UnitType m_initialPositionUnitType;
				UnitType m_maxPositionUnitType;
				UnitType m_minPositionUnitType;
			
#ifdef S11N
			protected:
			
				/**
				 * Serialization - derived class specific.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				virtual void _Serialize( ISerializer& emitter, Error& error ) const;

				/**
				 * Deserializes the child class instance from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
		};

		/**
		 * Specifies where to place the label for a point feature.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API PointPlacementCommand
			: public SequenceCommand
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
				 * Positioning of the label with respect to the feature
				 * @reqid 001.0063
				 */
				enum PositionAt
				{
					PositionAt_East = 0,
					PositionAt_NorthEast,
					PositionAt_North,
					PositionAt_NorthWest,
					PositionAt_West,
					PositionAt_SouthWest,
					PositionAt_South,
					PositionAt_SouthEast,
					PositionAt_OnPoint,
					PositionAt_Unknown
				};

				/**
				 * Constructor.
				 * @param error		an error object to be populated
				 * @reqid 001.0063
				 */
				PointPlacementCommand(Error& error)
					:	SequenceCommand(
							SequenceCommand::CommandType_PointPlacement,
							PointInstruction_PointPos ),
						m_angle(0.0f)
				{
					m_positions = new Integers();
					if (m_positions == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~PointPlacementCommand()
				{
					delete m_positions;
				}

				/**
				 * To set the angle at which the label should be placed.
				 * @param angle - in degrees
				 * @reqid 001.0063
				 */
				void SetAngle( float angle )
				{
					m_angle = angle;
				}

				/**
				 * To get the angle at which the label should be placed.
				 * @return - in degrees
				 * @reqid 001.0063
				 */
				float GetAngle() const
				{
					return m_angle;
				}

				/**
				 * Start iterating over the positions
				 * @return the initial position or NULL if the positions list is empty
				 * @reqid 001.0063
				 */
				const PositionAt* GetFirstPosition() const
				{
					return reinterpret_cast< PositionAt* >(m_positions->GetFirstElement());
				}

				/**
				 * Get the next position
				 * @return the next position or NULL if at the end of list
				 * @reqid 001.0063
				 */
				const PositionAt* GetNextPosition() const
				{
					return reinterpret_cast< PositionAt* >(m_positions->GetNextElement());
				}

				/**
				 * Add a new position to the list
				 * @param position	the position to be added
				 * @param error			an Error object
				 * @reqid 001.0063
				 */
				void AddPosition( PositionAt position, Error& error )
				{
					assert( ( position == PositionAt_North )			||
									( position == PositionAt_South )			||
									( position == PositionAt_East )				||
									( position == PositionAt_West )				||
									( position == PositionAt_NorthEast )	||
									( position == PositionAt_SouthEast )	||
									( position == PositionAt_NorthWest )	||
									( position == PositionAt_SouthWest )	||
									( position == PositionAt_OnPoint ) );

					int* positionElement = new int( (int)position );
					if (positionElement == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
						return;
					}
					m_positions->AddElement( positionElement, error );
				}

				/**
				 * @see SequenceCommand::Execute
				 * @reqid 001.0063
				 */
				virtual void Execute( Map *map, LabelRequest* labelRequest, bool* bSuccess, Error& error,
					ExecutionSequence::Iterator *returnLimit = NULL );

				/**
				* @see SequenceCommand::Clone
				* @reqid 001.0076
				*/
				virtual PointPlacementCommand* Clone( Error& error ) const
				{
					PointPlacementCommand* copy = new PointPlacementCommand(error);
					if ( copy == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					{
						if ( !error )
						{
							Copy( *this, *copy, error );
						}
						if (error) //either c'tor or Copy failed
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
				static void Copy(
					const PointPlacementCommand& src,
					PointPlacementCommand& dest, Error &error )
				{
					SequenceCommand::Copy( src, dest );

					dest.m_angle = src.m_angle;

					assert( src.m_positions );
					assert( dest.m_positions );

					dest.m_positions->Clear();
					int* position = src.m_positions->GetFirstElement();
					while ( position )
					{
						dest.m_positions->AddElement( new int( *position ), error );
						if ( error )
						{
							return; //pass error to a client
						}
						position = src.m_positions->GetNextElement();
					}
				}

				/**
				 * @see SequenceCommand::Validate
				 * @reqid 001.0086
				 */
				void Validate( FeatureType featType, bool* valid, ErrorDetails** validationErrors, Error& error );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				/**
				 * Preference ordered list of indexes of positions to place the label
				 * @reqid 001.0063
				 */
				Integers *m_positions;
				float m_angle;
			
#ifdef S11N
		protected:
			/**
			 * Serialization - derived class specific.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the child class instance from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
			
		};

		/**
		 * Specifies where to place the label for an area feature.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API AreaPlacementCommand : public SequenceCommand
		{
			public:
				/**
				 * The possible choices for placement.
				 * @reqid 001.0063
				 */
				enum PlaceHow
				{
					/** 
					 * Entirely inside the polygon.
					 */
					PlaceHow_InsideHorizontally = 0,
					/** 
					 * Label starts inside the polygon and extends outside to the right.
					 */
					PlaceHow_ExtendingOutsideToRightHorizontally,
					/**
					 * Label starts outside and extends into the polygon from the left.
					 */
					PlaceHow_ExtendingOutsideToLeftHorizontally,
					PlaceHow_ExtendingOutsideOnBothSidesHorizontally, // Label starts outside on the left, extends across the polygon,
					//   and ends outside on the right.
					PlaceHow_EntirelyOutsideHorizontally,           // Label is placed entirely outside the polygon as if it were a point feature.
					PlaceHow_InsideAtDominantAngle,                         // Label is placed centered inside the polygon at the polygon's dominant angle.
					PlaceHow_InsideAtArbitraryAngle,					// Successive angles are tried until one allows the label to be placed inside
					//   the polygon.
					PlaceHow_Unknown
				};

				/**
				 * Constructor
				 * @reqid 001.0063
				 */
				AreaPlacementCommand()
				{
					m_sequenceCommandType = CommandType_AreaPlacement;

					m_placeHow = PlaceHow_InsideHorizontally;
					m_commandOrdinal = AreaInstruction_AreaPlace;
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~AreaPlacementCommand()
				{}

				/**
				 * Sets how to place the label for the area feature
				 * @param placeHow - any of the enum PlaceHow options.
				 * @reqid 001.0063
				 */
				void SetHowToPlace( PlaceHow placeHow )
				{
					m_placeHow = placeHow;
				}

				/**
				 * Gets how to place the label for the area feature
				 * @return - how to place the label
				 * @reqid 001.0063
				 */
				PlaceHow GetHowToPlace() const
				{
					return m_placeHow;
				}

				/**
				 * @see SequenceCommand::Execute
				 * @reqid 001.0063
				 */
				virtual void Execute( Map *map, LabelRequest* labelRequest, bool* bSuccess, Error& error,
					ExecutionSequence::Iterator *returnLimit = NULL );

				/**
				* @see SequenceCommand::Clone
				* @reqid 001.0076
				*/
				virtual AreaPlacementCommand* Clone( Error& error ) const
				{
					AreaPlacementCommand* copy = new AreaPlacementCommand();
					if ( copy == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					{
						Copy( *this, *copy );
					}
					return copy;					
				}

				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @reqid 001.0076
				*/
				static void Copy(
					const AreaPlacementCommand& src,
					AreaPlacementCommand& dest )
				{
					SequenceCommand::Copy( src, dest );

					dest.m_placeHow = src.m_placeHow;
				}

				/**
				 * @see SequenceCommand::Validate
				 * @reqid 001.0086
				 */
				void Validate( FeatureType featType, bool* valid, ErrorDetails** validationErrors, Error& error );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				PlaceHow m_placeHow;
			
#ifdef S11N
		protected:
			/**
			 * Serialization - derived class specific.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the child class instance from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
			
		};

		/**
		 * Invokes leadered placement on a label.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API LeaderCommand:public SequenceCommand
		{
			public:

				enum ErrorCode
				{
				};

				/**
				 * The label can be either leadered at the end of processing all other labels (DeferToEnd)
				 * or processed immediately (LeaderNow).
				 * @reqid 001.0063
				 */
				enum LeaderWhen
				{
					LeaderWhen_DeferToEnd = 0,
					LeaderWhen_LeaderNow
				};

				/**
				 * Constructor
				 * @reqid 001.0063
				 */
				LeaderCommand():
					SequenceCommand( CommandType_Persistence,
							PersistenceInstruction_Leader )
				{
					m_leaderWhen = LeaderWhen_LeaderNow;
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~LeaderCommand()
				{}

				/**
				 * Sets when to leader the label, later or immediately.
				 * @param leaderWhen - DeferToEnd or LeaderNow
				 * @reqid 001.0063
				 */
				void SetWhenToLeader( LeaderWhen leaderWhen )
				{
					m_leaderWhen = leaderWhen;
				}

				/**
				 * Gets when to leader the label, later or immediately.
				 * @return - DeferToEnd or LeaderNow
				 * @reqid 001.0063
				 */
				LeaderWhen GetWhenToLeader() const
				{
					return m_leaderWhen;
				}

				/**
				 * @see SequenceCommand::Execute
				 * @reqid 001.0063
				 */
				virtual void Execute( Map *map, LabelRequest* labelRequest, bool* bSuccess, Error& error,
					ExecutionSequence::Iterator *returnLimit = NULL );

				/**
				* @see SequenceCommand::Clone
				* @reqid 001.0076
				*/
				virtual LeaderCommand* Clone( Error& error ) const
				{
					LeaderCommand* copy = new LeaderCommand();
					if ( copy == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					{
						Copy( *this, *copy );
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
				static void Copy(
					const LeaderCommand& src,
					LeaderCommand& dest )
				{
					SequenceCommand::Copy( src, dest );

					dest.m_leaderWhen = src.m_leaderWhen;
				}

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				LeaderWhen m_leaderWhen;
			
#ifdef S11N
		protected:
			/**
			 * Serialization - derived class specific.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the child class instance from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
			
		};

		/**
		 * Common ancestor for all the looping commands. Implements the basic looping functionality
		 * and defines the interface that has to be implemented by all its children.
		 */
		class LABELINGENGINE_API LoopCommand : public SequenceCommand
		{
		public:

			/**
			 * Defines the default return target for a sequence command.
			 * Definition consists of the pair of CommandType and CommandOrdinal.
			 */
			class DefaultReturnTarget
			{
			public:

				/**
				 * Constructor.
				 * @param commandType - CommandType of the return target.
				 * @param commandOrdinal - Ordinal of the return target.
				 * @reqid TODO mfraczak.Looping
				 */
				DefaultReturnTarget( SequenceCommand::CommandType commandType, int commandOrdinal )
					: m_commandType( commandType ), m_commandOrdinal( commandOrdinal )
				{
				}

				/**
				 * Destructor (compiler generated code avoidance).
				 * @reqid TODO mfraczak.Looping
				 */
				~DefaultReturnTarget()
				{
				}

				/**
				* Gets CommandType
				* @return - CommandType
				* @reqid TODO mfraczak.Looping
				*/
				SequenceCommand::CommandType GetCommandType() const 
				{ 
					return m_commandType; 
				}

				/**
				* Sets CommandType
				* @param val - CommandType
				* @reqid TODO mfraczak.Looping
				*/
				void SetCommandType(SequenceCommand::CommandType val) 
				{ 
					m_commandType = val; 
				}

				/**
				* Gets CommandOrdinal
				* @return - CommandOrdinal
				* @reqid TODO mfraczak.Looping
				*/
				int GetCommandOrdinal() const 
				{ 
					return m_commandOrdinal; 
				}

				/**
				* Sets CommandOrdinal
				* @param val - CommandOrdinal
				* @reqid TODO mfraczak.Looping
				*/
				void SetCommandOrdinal(int val) 
				{ 
					m_commandOrdinal = val; 
				}

			private:
				SequenceCommand::CommandType m_commandType;
				int m_commandOrdinal;
			};

			/**
			 * Constructor.
			 * @param commandOrdinal - Ordinal of the command deriving from this class. It has to be provided.
			 * @param whereToLoop - Looping target. It has to be either NULL (then the deafult return target
			 *                      will be set by the execution sequence on adding it) or set to an iterator
			 *                      set to a command already existing in the execution sequence BEFORE this
			 *                      command.
			 * @reqid TODO mfraczak.Looping
			 */
			LoopCommand( int commandOrdinal, ExecutionSequence::Iterator* whereToLoop )
				:	SequenceCommand( CommandType_Persistence, commandOrdinal ),
					m_whereToLoop( whereToLoop )
			{
			}

			/**
			 * Destructor. Compiler generated code avoidance.
			 * @reqid TODO mfraczak.Looping
			 */
			virtual ~LoopCommand()
			{
				delete m_whereToLoop;
			}

				/**
				 * LoopCommand which implements looping by returning the previous command in the sequence
				 * should override this method and return true.
				 * Otherwise the default implementation returns false.
				 * @return - true if the looping command returns to the previous command
				 * @reqid 006.0088
				 */
				virtual bool ReturnsToPreviousCommand() const
				{
					return false;
				}

			/**
			 * Reports whether the candidate loop target command is a valid return target.
			 * BufferSizeCommand overrides this method to check for a matching buffer type.
			 * Otherwise the default implementation checks.
			 * @return - true if the command is a valid loop return target
			 * @reqid 006.0089
			 */
			virtual bool IsCommandLoopTarget( SequenceCommand& command );

			/**
			 * Called in the beginning of each loop on the deriving class. It should perform all the logic
			 * suitable for loop start here (like re-stacking the label, reducing the buffer etc).
			 * @param map - The map being labeled
			 * @param labelRequest - The label request being processed.
			 * @param shouldContinue - [out] If true is returned the loop is executed. If the deriving class
			 *                         returns false then loop execution is aborted.
			 * @param error - [out] Error info.
			 * @reqid TODO mfraczak.Looping
			 */
			virtual void OnLoopReentry( Map* map, LabelRequest* labelRequest, bool* shouldContinue, Error& error ) = 0;

			/**
			 * Called at the end of each loop. This event handler should allow each deriving class to terminate
			 * the loop by returning false in shouldContinue when end conditions are met.
			 * @param labelRequest - The label request being processed.
			 * @param shouldContinue - [out] If true is returned the loop continues. Otherwise it terminates.
			 * @param error - [out] error info.
			 * @reqid TODO mfraczak.Looping
			 */
			virtual void ShouldLoopTerminate( LabelRequest* labelRequest, bool* shouldContinue, Error& error ) = 0;

			/**
			 * This "event handler" is called before looping starts allowing the deriving class to 
			 * reset itself in between different label requests (the execution sequence is shared
			 * among all of them in the same label class).
			 * @param labelRequest - The label request being processed.
			 * @param error - [out] Error info.
			 * @reqid TODO mfraczak.Looping
			 */
			virtual void Reset( LabelRequest* labelRequest, Error& error ) = 0;

			/**
			 * This "event handler" is called when looping has ended (for any reason possible)
			 * allowing the deriving class to "cleanup", e.g. reset a stacked label to its
			 * original state or reset the current buffer size.
			 * @param labelRequest - The label request being processed.
			 * @param error - [out] Error info.
			 * @reqid TODO mfraczak.Looping
			 */
			virtual void LoopDone( LabelRequest* labelRequest, Error& error ) = 0;

			/**
			 * @see SequenceCommand::Execute
			 * For the looping commands this is where the looping logic actually lies.
			 * Deriving classes should NOT reimplement this unless some very specific looping
			 * logic is needed. They should implement the "event handlers" instead.
			 * @reqid 001.0063
			 */
			void Execute( Map *map, LabelRequest* labelRequest, bool* bSuccess, Error& error,
				ExecutionSequence::Iterator *returnLimit = NULL );

			/**
			* Sets the looping target (see description in constructor docs).
			* @param val - WhereToLoop
			* @reqid TODO mfraczak.Looping
			*/
			void SetWhereToLoop(ExecutionSequence::Iterator* val) 
			{ 
				delete m_whereToLoop;
				m_whereToLoop = val;
			}

			/**
			* Gets the looping target.
			* @return - WhereToLoop
			* @reqid TODO mfraczak.Looping
			*/
			ExecutionSequence::Iterator* GetWhereToLoop() const 
			{ 
				return m_whereToLoop; 
			}

			/**
			 * @see SequenceCommand::ImplementsLooping
			 * Here it returns true because all of deriving classes also have to implement
			 * the interface required by client code.
			 * @reqid 001.0063
			 */
			virtual bool ImplementsLooping() const
			{
				return true;
			}

			/**
			 * When implemented in the deriving class, this method has to return an array of default
			 * return targets. Execution sequence will use this table and search for default targets
			 * already in the sequence to set as return targets on the command if no specific target
			 * was set by the client.
			 * @param targets - [out] Table of default return targets.
			 * @param targetCount - [out] Size of the table.
			 * @reqid TODO mfraczak.Looping
			 */
			virtual void GetDefaultReturnTargets( const DefaultReturnTarget** targets, int* targetCount  ) = 0;

		protected:
			/****************************************************************************************
			THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
			TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
			****************************************************************************************/

#ifdef _MSC_VER
#if _MSC_VER <= 1400
			// GCC doesn't seem to need the explicit specialization
			// VC10 issues compile error as a result (I believe) of
			// increased compliance with the C++ standard. // Phil Grenetz
			template class LABELINGENGINE_API ExecutionSequence::Iterator;
#endif // _MSC_VER <= 1400
#endif // _MSC_VER

			ExecutionSequence::Iterator* m_whereToLoop;

			/**
			 * Make deep copy of one instance's members to another
			 * @param src source object
			 * @param dest destination object
			 * @param error an error object to be populated
			 * @reqid 001.0076
			 */
			static void Copy( const LoopCommand& src, LoopCommand& dest, Error& error )
			{
				SequenceCommand::Copy( src, dest );

				if ( src.m_whereToLoop )
				{
					delete dest.m_whereToLoop;
					dest.m_whereToLoop = new ExecutionSequence::Iterator( *src.m_whereToLoop );
					if ( dest.m_whereToLoop == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
						return;
					}
				}
			}
		};

		/**
		 * Specifies that the previous set of placement commands should be re-executed but with bumping
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API BumpCommand:public SequenceCommand
		{
			public:
				/**
				 * Constructor
				 * @reqid 001.0063
				 */
				BumpCommand()
					:SequenceCommand( CommandType_Persistence,
							  PersistenceInstruction_Bump )
				{
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~BumpCommand()
				{}

				/**
				 * @see SequenceCommand::Execute
				 * @reqid 001.0063
				 */
				virtual void Execute( Map *map, LabelRequest* labelRequest, bool* bSuccess, Error& error,
					ExecutionSequence::Iterator *returnLimit = NULL );

				/**
				* @see SequenceCommand::Clone
				* @reqid 001.0076
				*/
				virtual BumpCommand* Clone( Error& error ) const
				{
					BumpCommand* copy = new BumpCommand();
					if ( copy == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					{
						Copy( *this, *copy );
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
				static void Copy(
					const BumpCommand& src,
					BumpCommand& dest )
				{
					SequenceCommand::Copy( src, dest );
				}

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
			
#ifdef S11N
		protected:
			/**
			 * Serialization - derived class specific.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the child class instance from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
			
		};

		/**
		 * Specifies that the previous set of placement commands should be re-executed but with hidden elements
		 * to reduce the space consumed by the label.
		 \code
		 String parameters;
		 HideAttempts hideAttempts;

		 parameters.AssignString( _T("TC(name1, name2) & SC(name3), L(1), TO(name3)"), error );
		 if ( error )
			{
			return;
			}

			HideCommand::ParseParameters( parameters.GetString(), &hideAttempts, error );

			HideCommand* command = new HideCommand( &hideAttempts, NULL, error );
			if ( error )
			{
			return;
			}
			\endcode
		 * @reqid 006.0014
		 */
		class LABELINGENGINE_API HideCommand : public LoopCommand
		{
			public:
				/**
				* Possible error codes
				* @reqid 006.0014
				*/
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_ParseError,
					ErrorCode_LabelNotSupported,
					ErrorCode_AssemblyStackLevelNotSupported,
					ErrorCode_AssemblyTextObjectNotSupported,
					ErrorCode_CommandInitializedWithNullHideAttempts,
				};

				/**
				 * Constructor
				 * @param hideAttempts -	Set of hiding instructions for the command. Proper syntax is required.
				 *												The client must pass in a dynamically allocated object.
				 *												The HideCommand takes ownership.
				 * @param loopWhere - As a looping command HideCommand may have the looping target 
				 *                    specified.
				 * @param error - [out] Error info.
				 * @reqid 006.0014
				 */
				HideCommand( HideAttempts* hideAttempts, ExecutionSequence::Iterator* loopWhere, Error& error )
					:	LoopCommand( PersistenceInstruction_Hide, loopWhere ),
						m_hideAttempts( hideAttempts )
				{
					if ( hideAttempts == NULL )
					{
						SET_ERROR( error, ErrorCode_CommandInitializedWithNullHideAttempts,
							_T("A NULL HideAttempts pointer was passed to the HideCommand constructor.") );

						return;
					}
				}

				/**
				 * Destructor
				 * @reqid 006.0014
				 */
				~HideCommand()
				{
					delete m_hideAttempts;
				}

				/**
				* Parse the command parameter string, validate its form against documented command syntax,
				* and report errors or populate a collection object hierarchy of label elements.
				*
				* HIDE	=	SL|TO|TC(ID) & SL|TO|TC(ID) & ... & SL|TO|TC(ID),	<== first Hide Attempt
				*					...
				*					SL|TO|TC(ID) & SL|TO|TC(ID) & ... & SL|TO|TC(ID)	<== last Hide Attempt
				*
				* @param parameters - the right hand side of the command expression
				* @param hideAttempts - [in-out] the collection object hierarchy to be populated
				* @param error - an error object to be populated
				* @reqid 006.0014
				*/
				static void ParseParameters( const TCHAR* parameters, HideAttempts *hideAttempts, Error& error );

				/**
				 * Parses a list of names for the hide part identifier pointed to by the input string parser and stores
				   them in the input hideAttempt collection.
				   @param stringParser - object for parsing the hide requests string.
				   @param hideAttempt - collection into which the element members are inserted.
				   @param elementType - either ElementType_TextObject, ElementType_TextComponent, or ElementType_SymbolComponent,
				   @param error - standard error object
				 * @reqid 006.0014
				 */
				static void ParseNamesSet(StringParser &stringParser,HideAttempt *hideAttempt,LabelElement::ElementType elementType,Error &error);

				/**
				 * Parses a list of levels for the hide part identifier pointed to by the input string parser and stores
				   them in the input hideAttempt collection.
				   @param stringParser - object for parsing the hide requests string.
				   @param hideAttempt - collection into which the element members are inserted.
				   @param error - standard error object
				 * @reqid 006.0014
				 */
				static void ParseLevelsSet(StringParser &stringParser,HideAttempt *hideAttempt,Error &error);

				/**
				 * @see LoopCommand::
				 * @reqid 006.0014
				 */
				void OnLoopReentry( Map* map, LabelRequest* labelRequest, bool* shouldContinue, Error& error );

				/**
				 * @see LoopCommand::ShouldLoopTerminate
				 * @reqid 006.0014
				 */
				void ShouldLoopTerminate( LabelRequest* labelRequest, bool* shouldContinue, Error& error );

				/**
				 * @see LoopCommand::Reset
				 * @reqid 006.0014
				 */
				void Reset( LabelRequest* labelRequest, Error& error );

				/**
				 * @see LoopCommand::LoopDone
				 * @reqid 006.0014
				 */
				void LoopDone( LabelRequest* labelRequest, Error& error );

				/**
				 * @see LoopCommand::GetDefaultReturnTargets
				 * @reqid 006.0014
				 */
				void GetDefaultReturnTargets( const DefaultReturnTarget** targets, int* targetCount );

				/**
				* @see SequenceCommand::Clone
				* @reqid 006.0014
				*/
				virtual HideCommand* Clone( Error& error ) const
				{
					HideAttempts* hideAttempts = NULL; 

					if ( m_hideAttempts != NULL )
					{
						hideAttempts = m_hideAttempts->Clone( error );
						if ( error )
						{
							return NULL;
						}
					}

					ExecutionSequence::Iterator* newWhereToLoop = new ExecutionSequence::Iterator( *m_whereToLoop );
					if ( newWhereToLoop == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
						return NULL;
					}

					HideCommand* copy = new HideCommand( hideAttempts, newWhereToLoop, error );
					if ( copy == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
						return NULL;
					}
					if ( error )
					{
						delete copy;
						return NULL;
					}

					return copy;				
				}

				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @param error an error object to be populated
				* @reqid 006.0014
				*/
				static void Copy(
					const HideCommand& src,
					HideCommand& dest, Error &error )
				{
					LoopCommand::Copy( src, dest, error );
					if ( error )
					{
						return;
					}

					delete dest.m_hideAttempts;
					dest.m_hideAttempts = src.m_hideAttempts->Clone( error );
					if ( error )
					{
						return;
					}
				}

				/**
				* Hide the group of label elements specified by hide attempt iterator.
				* @param originalLabel - a working Label object that reflects the original label structure
				* @param success - true if valid; false if not valid
				* @param error - an error object to be populated
				* @reqid 006.0014
				*/
				void HideElementGroup( Label* originalLabel, HideAttempts::ConstIterator* hideAttemptsIterator,
					bool* success, Error& error );

			private:
				/* 
				 * Hide the specified label element.
				 * @param originalLabel - a Label object that reflects the original label structure
				 * @param element - the element to hide
				 * @param error - an Error object
				 * @param elementHidden - true if the label element could be hidden, false if not
				 * @reqid 006.0014
				 */
				static void HideElement( Label* originalLabel, LabelElement::ElementType elementType, const String& element, bool* elementHidden, Error& error );
				/* 
				 * Hide the specified stack level.
				 * @param originalLabel - a RegularLabel object that reflects the original label structure
				 * @param hideStackLevelNumber - the number identifying the stack level to hide
				 * @return - true if the element was hidden; false if not
				 * @reqid 006.0014
				 */
				static bool HideStackLevel( RegularLabel* originalLabel, int hideStackLevelNumber );

				/* 
				 * Hide the specified text object.
				 * @param originalLabel - a RegularLabel object that reflects the original label structure
				 * @param hideTextObjectName - the name identifying the text object to hide
				 * @return - true if the element was hidden; false if not
				 * @reqid 006.0014
				 */
				static bool HideTextObject( RegularLabel* originalLabel, const TCHAR* hideTextObjectName );

				/* 
				 * Hide the specified text component.
				 * @param originalLabel - an AssemblyLabel object that reflects the original label structure
				 * @param hideTextComponentName - the name identifying the text component to hide
				 * @return - true if the element was hidden; false if not
				 * @reqid 006.0014
				 */
				static bool HideTextComponent( AssemblyLabel* originalLabel, const TCHAR* hideTextComponentName );

				/* 
				 * Hide the specified symbol component.
				 * @param originalLabel - an AssemblyLabel object that reflects the original label structure
				 * @param hideSymbolComponentName - the name identifying the symbol symbol to hide
				 * @return - true if the element was hidden; false if not
				 * @reqid 006.0014
				 */
				static bool HideSymbolComponent( AssemblyLabel* originalLabel, const TCHAR* hideSymbolComponentName );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				HideAttempts* m_hideAttempts;
			
#ifdef S11N
		public:
			/**
			 * Default constructor needed for serialization
			 * @reqid N/A.
			 */
			HideCommand() : LoopCommand( PersistenceInstruction_Hide, NULL ), m_hideAttempts(NULL)
			{};
		protected:
			/**
			 * Serialization - derived class specific.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the child class instance from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );

#endif
			
		};

		/**
		 * Specifies that the previous set of placement commands should be re-executed but with overline allowed on
		 *   line features and area feature boundaries.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API OverlineCommand:public SequenceCommand
		{
			public:
				/**
				 * Constructor
				 * @reqid 001.0063
				 */
				OverlineCommand()
					:SequenceCommand( CommandType_Persistence,
							  PersistenceInstruction_Overline )
				{
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~OverlineCommand()
				{}

				/**
				 * @see SequenceCommand::Execute
				 * @reqid 001.0063
				 */
				virtual void Execute( Map *map, LabelRequest* labelRequest, bool* bSuccess, Error& error,
					ExecutionSequence::Iterator *returnLimit = NULL );

				/**
				* @see SequenceCommand::Clone
				* @reqid 001.0076
				*/
				virtual OverlineCommand* Clone( Error& error ) const
				{
					OverlineCommand* copy = new OverlineCommand();
					if ( copy == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					{
						Copy( *this, *copy );
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
				static void Copy(
					const OverlineCommand& src,
					OverlineCommand& dest )
				{
					SequenceCommand::Copy( src, dest );
				}

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
			
#ifdef S11N
		protected:
			/**
			 * Serialization - derived class specific.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the child class instance from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
			
		};

		/**
		 * Specifies that the previous set of placement commands should be re-executed but with overregion
		 *   allowed on the interiors of area features.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API OverregionCommand:public SequenceCommand
		{
			public:
				/**
				 * Constructor
				 * @reqid 001.0063
				 */
				OverregionCommand()
					:SequenceCommand( CommandType_Persistence,
							  PersistenceInstruction_Overregion )
				{
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~OverregionCommand()
				{}

				/**
				 * @see SequenceCommand::Execute
				 * @reqid 001.0063
				 */
				virtual void Execute( Map *map, LabelRequest* labelRequest, bool* bSuccess, Error& error,
					ExecutionSequence::Iterator *returnLimit = NULL );

				/**
				* @see SequenceCommand::Clone
				* @reqid 001.0076
				*/
				virtual OverregionCommand* Clone( Error& error ) const
				{
					OverregionCommand* copy = new OverregionCommand();
					if ( copy == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					{
						Copy( *this, *copy );
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
				static void Copy(
					const OverregionCommand& src,
					OverregionCommand& dest )
				{
					SequenceCommand::Copy( src, dest );
				}

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
			
#ifdef S11N
		protected:
			/**
			 * Serialization - derived class specific.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the child class instance from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
			
		};

		/**
		 * Allows for either reduction of the buffer size and the previous set of placement commands re-executed
		 * or for the buffer size to be reset to its original value.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API BufferSizeCommand : public LoopCommand
		{
			public:
				enum ErrorCode
				{
					ErrorCode_NumberOfStepsHasToBePositive = 1,
				};

				/**
				 * Whether to reduce the buffer size or to reset the buffer size.
				 * @reqid 001.0063
				 */
				enum BufferSizeAction
				{
					BufferSizeAction_ReduceBufferSize = 0,
					BufferSizeAction_LoopOnBufferSize,
					BufferSizeAction_ResetBufferSize
				};

				/**
				 * Constructor.
				 * @param whereToLoop - Deriving from the looping command buffer size command
				 *                      may specify the return target for the loop.
				 *                      @see LoopCommand::LoopCommand
				 * @reqid 001.0063
				 */
				BufferSizeCommand(  ExecutionSequence::Iterator* whereToLoop )
					: LoopCommand( PersistenceInstruction_BufferSize, whereToLoop )
				{
					m_bufferSizeAction = BufferSizeAction_ReduceBufferSize;
					m_numBufferSteps = 1;
					m_bufferType = BufferType_TextToText;
				}

				/**
				 * Destructor.
				 * @reqid 001.0063
				 */
				~BufferSizeCommand()
				{}

				/**
				 * LoopCommand which implements looping by returning the previous command in the sequence
				 * should override this method and return true.
				 * Otherwise the default implementation returns false.
				 * @return - true if the looping command returns to the previous command
				 * @see LoopCommand::ReturnsToPreviousCommand
				 * @reqid 006.0088
				 */
				virtual bool ReturnsToPreviousCommand() const
				{
					return ( m_bufferSizeAction == BufferSizeAction_ReduceBufferSize );
				}

				/**
				 * Sets type of buffer that applies to this instance of a buffer size command.
				 * @param bufferType - type of buffer to apply to this command
				 * @reqid 001.0063
				 */
				void SetBufferType( BufferType bufferType )
				{
					m_bufferType = bufferType;
				}

				/**
				 * Gets type of buffer that applies to this instance of a buffer size command.
				 * @return the type of buffer that applies to this command from the BufferType enumeration.
				 * @reqid 001.0063
				 */
				BufferType GetBufferType() const
				{
					return m_bufferType;
				}

				/**
				 * Sets the maximum number of buffer reductions allowed
				 * @param numBufferSteps -	The number of times the buffer size can be reduced.
				 *													It must be > 0.
				 * @param error - [out] Error info.
				 * @reqid 001.0063
				 */
				void SetNumBufferReductionSteps( int numBufferSteps, Error& error )
				{
					if ( numBufferSteps <= 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_NumberOfStepsHasToBePositive );
						return;
					}

					m_numBufferSteps = numBufferSteps;
				}

				/**
				 * Gets the maximum number of buffer reductions allowed
				 * @return - number of times the buffer size can be reduced
				 * @reqid 001.0063
				 */
				int GetNumBufferReductionSteps() const
				{
					return m_numBufferSteps;
				}

				/**
				 * Sets whether to reduce the buffer size or reset the buffer size
				 * @param bufferSizeAction - ReduceBufferSize or ResetBufferSize
				 * @reqid 001.0063
				 */
				void SetBufferSizeAction(
					BufferSizeAction bufferSizeAction )
				{
					m_bufferSizeAction = bufferSizeAction;
				}

				/**
				 * Gets whether to reduce the buffer size or reset the buffer size
				 * @return - ReduceBufferSize or ResetBufferSize
				 * @reqid 001.0063
				 */
				BufferSizeAction GetBufferSizeAction() const
				{
					return m_bufferSizeAction;
				}

				/**
				 * Reports whether the candidate loop target command is a valid return target.
				 * BufferSizeCommand overrides this method to check for a matching buffer type
				 * after calling the default implementation.
				 * @return - true if the command is a valid loop return target
				 * @reqid 006.0089
				 */
				virtual bool IsCommandLoopTarget( SequenceCommand& command );

				/**
				 * @see LoopCommand::OnLoopReentry
				 * @reqid 006.0007
				 */
				void OnLoopReentry( Map* map, LabelRequest* labelRequest, bool* shouldContinue, Error& error );

				/**
				 * @see LoopCommand::ShouldLoopTerminate
				 * @reqid 006.0007
				 */
				void ShouldLoopTerminate( LabelRequest* labelRequest, bool* shouldContinue, Error& error );

				/**
				 * @see LoopCommand::Reset
				 * @reqid 006.0007
				 */
				void Reset( LabelRequest* labelRequest, Error& error );

				/**
				 * @see LoopCommand::GetDefaultReturnTargets
				 * @reqid 006.0007
				 */
				void GetDefaultReturnTargets( const DefaultReturnTarget** targets, int* targetCount );

				/**
				 * @see LoopCommand::LoopDone
				 * @reqid 006.0007
				 */
				void LoopDone( LabelRequest* labelRequest, Error& error );

				/**
				* @see SequenceCommand::Clone
				* @reqid 001.0076
				*/
				virtual BufferSizeCommand* Clone( Error& error ) const
				{
					BufferSizeCommand* copy = new BufferSizeCommand( NULL );
					if ( copy == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					{
						Copy( *this, *copy );
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
				static void Copy(
					const BufferSizeCommand& src, BufferSizeCommand& dest )
				{
					SequenceCommand::Copy( src, dest );

					dest.m_bufferSizeAction = src.m_bufferSizeAction;
					dest.m_bufferType = src.m_bufferType;
					dest.m_numBufferSteps = src.m_numBufferSteps;
				}

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				BufferSizeAction m_bufferSizeAction;
				BufferType m_bufferType;
				int m_numBufferSteps;
			
#ifdef S11N
		protected:
			/**
			 * Serialization - derived class specific.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the child class instance from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
			
		};

		/**
		 * Allows for either reduction of the font size and the previous set of placement commands re-executed
		 * or for the font size to be reset to its original value.
		 * @reqid 006.0088
		 */
		class LABELINGENGINE_API FontSizeCommand : public LoopCommand
		{
			public:
				enum ErrorCode
				{
					ErrorCode_OnlyRegularLabelSupported = 1,
					ErrorCode_NumberOfStepsHasToBePositive
				};

				/**
				 * Whether to reduce the font size or to reset the font size.
				 * @reqid 006.0088
				 */
				enum FontSizeAction
				{
					FontSizeAction_ReduceFontSize = 0,
					FontSizeAction_LoopOnFontSize,
					FontSizeAction_ResetFontSize
				};

				/**
				 * Constructor.
				 * @param whereToLoop - Deriving from the looping command font size command
				 *                      may specify the return target for the loop.
				 *                      @see LoopCommand::LoopCommand
				 * @reqid 006.0088
				 */
				FontSizeCommand(  ExecutionSequence::Iterator* whereToLoop )
					: LoopCommand( PersistenceInstruction_FontSize, whereToLoop )
				{
					m_fontSizeAction = FontSizeAction_ReduceFontSize;
					m_numFontSteps = 1;
				}

				/**
				 * Destructor.
				 * @reqid 006.0088
				 */
				~FontSizeCommand()
				{
				}

				/**
				 * LoopCommand which implements looping by returning the previous command in the sequence
				 * should override this method and return true.
				 * Otherwise the default implementation returns false.
				 * @return - true if the looping command returns to the previous command
				 * @see LoopCommand::ReturnsToPreviousCommand
				 * @reqid 006.0088
				 */
				virtual bool ReturnsToPreviousCommand() const
				{
					return ( m_fontSizeAction == FontSizeAction_ReduceFontSize );
				}

				/**
				 * Sets the maximum number of font reductions allowed
				 * @param numFontSteps -	The number of times the font size can be reduced.
				 *												Must be > 0.
				 * @param error - [out] Error info.
				 * @reqid 006.0088
				 */
				void SetNumFontReductionSteps( int numFontSteps, Error& error )
				{
					if ( numFontSteps <= 0 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_NumberOfStepsHasToBePositive );
						return;
					}

					m_numFontSteps = numFontSteps;
				}

				/**
				 * Gets the maximum number of font reductions allowed
				 * @return - number of times the font size can be reduced
				 * @reqid 006.0088
				 */
				int GetNumFontReductionSteps() const
				{
					return m_numFontSteps;
				}

				/**
				 * Sets whether to reduce the font size or reset the font size
				 * @param fontSizeAction - ReduceFontSize or ResetFontSize
				 * @reqid 006.0088
				 */
				void SetFontSizeAction(
					FontSizeAction fontSizeAction )
				{
					m_fontSizeAction = fontSizeAction;
				}

				/**
				 * Gets whether to reduce the font size or reset the font size
				 * @return - ReduceFontSize or ResetFontSize
				 * @reqid 006.0088
				 */
				FontSizeAction GetFontSizeAction() const
				{
					return m_fontSizeAction;
				}

				/**
				 * @see LoopCommand::OnLoopReentry
				 * @reqid 006.0088
				 */
				void OnLoopReentry( Map* map, LabelRequest* labelRequest, bool* shouldContinue, Error& error );

				/**
				 * @see LoopCommand::ShouldLoopTerminate
				 * @reqid 006.0088
				 */
				void ShouldLoopTerminate( LabelRequest* labelRequest, bool* shouldContinue, Error& error );
				/**
				 * @see LoopCommand::Reset
				 * @reqid 006.0088
				 */
				void Reset( LabelRequest* labelRequest, Error& error );

				/**
				 * @see LoopCommand::GetDefaultReturnTargets
				 * @reqid 006.0088
				 */
				void GetDefaultReturnTargets( const DefaultReturnTarget** targets, int* targetCount );

				/**
				 * @see LoopCommand::LoopDone
				 * @reqid 006.0088
				 */
				void LoopDone( LabelRequest* labelRequest, Error& error );

				/**
				* @see SequenceCommand::Clone
				* @reqid 006.0088
				*/
				virtual FontSizeCommand* Clone( Error& error ) const
				{
					FontSizeCommand* copy = new FontSizeCommand( NULL );
					if ( copy == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					{
						Copy( *this, *copy );
					}
					return copy;					
				}

				/**
				* Make deep copy of one instance's members to another
				* @param src source object
				* @param dest destination object
				* @param error an error object to be populated
				* @reqid 006.0088
				*/
				static void Copy(
					const FontSizeCommand& src, FontSizeCommand& dest )
				{
					SequenceCommand::Copy( src, dest );

					dest.m_fontSizeAction = src.m_fontSizeAction;
					dest.m_numFontSteps = src.m_numFontSteps;
				}

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				FontSizeAction m_fontSizeAction;
				int m_numFontSteps;
			
#ifdef S11N
		protected:
			/**
			 * Serialization - derived class specific.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the child class instance from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
			
		};

		/**
		 * Allows division of the label into multiple lines and the previous set of placement commands re-executed.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API StackCommand : public LoopCommand
		{
			public:

				enum ErrorCode
				{
					ErrorCode_OnlyRegularLabelSupported = 1,
					ErrorCode_NumberOfStackedLinesHasToBeGreaterThanOne,
					ErrorCode_LabelNotSupported,
				};

				/**
				 * Constructor.
				 * @param loopWhere - As a looping command StackCommand may have the looping target 
				 *                    specified.
				 * @reqid 001.0063
				 */
				StackCommand( ExecutionSequence::Iterator* whereToLoop )
					:	LoopCommand( PersistenceInstruction_Stack, whereToLoop )
				{
					m_maxNumberOfLines = 2;
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~StackCommand()
				{
				}

				/**
				 * Sets the maximum number of lines the label can be stacked into.
				 * @param numStackLines - Max number of stack lines. The command will iterate
				 *                        as long as the max number of stack lines was reached
				 *                        or the placement was successfull. 
				 *                        It has to be greater than 1.
				 * @param error - [out] Error info.
				 * @reqid 001.0063
				 */
				void SetNumStackLines( int numStackLines, Error& error )
				{
					if ( numStackLines <= 1 )
					{
						SET_ERROR_NOMSG( error, ErrorCode_NumberOfStackedLinesHasToBeGreaterThanOne );
						return;
					}
					m_maxNumberOfLines = numStackLines;
				}

				/**
				 * Gets the maximum number of lines the label can be stacked into.
				 * @reqid 001.0063
				 */
				int GetNumStackLines() const
				{
					return m_maxNumberOfLines;
				}

				/**
				 * @see LoopCommand::OnLoopReentry
				 * @reqid TODO mfraczak.Stacking
				 */
				void OnLoopReentry( Map* map, LabelRequest* labelRequest, bool* shouldContinue, Error& error );

				/**
				 * @see LoopCommand::ShouldLoopTerminate
				 * @reqid TODO mfraczak.Stacking
				 */
				void ShouldLoopTerminate( LabelRequest* labelRequest, bool* shouldContinue, 
					Error& error );

				/**
				 * @see LoopCommand::Reset
				 * @reqid TODO mfraczak.Stacking
				 */
				void Reset( LabelRequest* labelRequest, Error& error );

				/**
				 * @see LoopCommand::LoopDone
				 * @reqid TODO mfraczak.Stacking
				 */
				void LoopDone( LabelRequest* labelRequest, Error& error );

				/**
				 * @see LoopCommand::GetDefaultReturnTargets
				 * @reqid TODO mfraczak.Stacking
				 */
				void GetDefaultReturnTargets( const DefaultReturnTarget** targets, int *targetCount  );

				/**
				* @see SequenceCommand::Clone
				* @reqid 001.0076
				*/
				virtual StackCommand* Clone( Error& error ) const
				{
					StackCommand* copy = new StackCommand( NULL );
					if ( copy == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					{
						Copy( *this, *copy, error );
						if ( error )
						{
							delete copy;
							return NULL;
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
				static void Copy(
					const StackCommand& src,
					StackCommand& dest, Error& error )
				{
					LoopCommand::Copy( src, dest, error );
					if ( error )
					{
						return;
					}

					dest.m_maxNumberOfLines = src.m_maxNumberOfLines;
				}

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				int m_maxNumberOfLines;
			
#ifdef S11N
		protected:
			/**
			 * Serialization - derived class specific.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the child class instance from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
			
		};

		/**
		 * Allows the offset of the label to be increased from the feature and the previous set of
		 * placement commands to be re-executed.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API OffsetCommand:public SequenceCommand
		{
			public:
				/**
				 * Constructor
				 * @reqid 001.0063
				 */
				OffsetCommand()
					:SequenceCommand( CommandType_Persistence,
							  PersistenceInstruction_Offset )
				{
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~OffsetCommand()
				{}

				/**
				 * @see SequenceCommand::Execute
				 * @reqid 001.0063
				 */
				virtual void Execute( Map *map, LabelRequest* labelRequest, bool* bSuccess, Error& error,
					ExecutionSequence::Iterator *returnLimit = NULL );

				/**
				* @see SequenceCommand::Clone
				* @reqid 001.0076
				*/
				virtual OffsetCommand* Clone( Error& error ) const
				{
					OffsetCommand* copy = new OffsetCommand();
					if ( copy == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					{
						Copy( *this, *copy );
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
				static void Copy(
					const OffsetCommand& src,
					OffsetCommand& dest )
				{
					SequenceCommand::Copy( src, dest );
				}

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
			
#ifdef S11N
		protected:
			/**
			 * Serialization - derived class specific.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the child class instance from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
			
		};

		/**
		* Allows the label to be placed beyond the ends of the feature and the previous set of
		*         placement commands to be re-executed.
		* @reqid 003.0075
		*/
		class LABELINGENGINE_API OverhangCommand:public SequenceCommand
		{
		public:

			enum ErrorCode
			{
				ErrorCode_OverhangHasToBeGreaterThanZero = 1,
			};
			/**
			* Constructor
			* @param overhangAmount percentage of label-length that can extend outside the extent of the geometry
			* @param error validation error 
			* @reqid 003.0075
			*/
			OverhangCommand(float overhangAmount, Error& error )
				:SequenceCommand( CommandType_Persistence,
				PersistenceInstruction_Overhang )
			{
				SetOverhangAmount( overhangAmount, error );
				if ( error )
				{
					return;
				}
			}

			/**
			* Destructor
			* @reqid 003.0075
			*/
			~OverhangCommand()
			{}

			/**
			* Sets the overhang amount which is a percentage.
			* The actual overhang amount will be computed from this percentage and the label length.
			* @param overhangAmount percentage of label-length that can extend outside the extent of the geometry
			* @param error validation error 
			* @reqid 003.0075
			*/
			void SetOverhangAmount( float overhangAmount, Error& error )
			{
				if ( overhangAmount <= 0 )
				{
					SET_ERROR_NOMSG( error, ErrorCode_OverhangHasToBeGreaterThanZero );
					return;
				}
				m_overhangAmt = overhangAmount;
			}

			/**
			* Gets the overhang amount.
			* @return percentage of label-length that can extend outside the extent of the geometry
			* @reqid 003.0075
			*/
			float GetOverhangAmount() const
			{
				return m_overhangAmt;
			}

			/**
			* @see SequenceCommand::Execute
			* @reqid 003.0075
			*/
			virtual void Execute( Map *map, LabelRequest* labelRequest, bool* bSuccess, Error& error,
				ExecutionSequence::Iterator *returnLimit = NULL );

			/**
			* @see SequenceCommand::Clone
			* @reqid 001.0076
			*/
			virtual OverhangCommand* Clone( Error& error ) const
			{
				OverhangCommand* copy = new OverhangCommand( m_overhangAmt, error );
				if ( copy == NULL)
				{
					SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					return NULL;
				}
				if ( error )
				{
					return NULL;
				}

				Copy( *this, *copy );
				return copy;					
			}

			/**
			* Make deep copy of one instance's members to another
			* @param src source object
			* @param dest destination object
			* @param error an error object to be populated
			* @reqid 001.0076
			*/
			static void Copy(
				const OverhangCommand& src,
				OverhangCommand& dest )
			{
				SequenceCommand::Copy( src, dest );

				dest.m_overhangAmt = src.m_overhangAmt;
			}

			/****************************************************************************************
			THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
			TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
			****************************************************************************************/
		private:
			float m_overhangAmt;
			
#ifdef S11N
		protected:
			/**
			 * Serialization - derived class specific.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the child class instance from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
			
		};

		/**
		 * Specifies how the label should fail if it cannot be placed successfully.
		 *   be re-executed.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API FailureCommand:public SequenceCommand
		{
			public:
				/**
				 * If the label cannot be placed, either Suppress it or Force it on the map.
				 * @reqid 001.0063
				 */
				enum FailHow
				{
					FailHow_SuppressLabel = 0,
					FailHow_ForceLabel,
					FailHow_ForceLeaderLabel
				};

				/**
				 * Constructor
				 * @reqid 001.0063
				 */
				FailureCommand()
				{
					m_failHow = FailHow_SuppressLabel;
					m_commandOrdinal = PersistenceInstruction_Suppress;
					m_sequenceCommandType = CommandType_Failure;
				}

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				~FailureCommand()
				{}

				/**
				 * Sets whether the label should be suppressed or forced if it can't be placed acceptably.
				 * @param failHow - FailHow_SuppressLabel or FailHow_ForceLabel
				 * @reqid 001.0063
				 */
				void SetHowToFail( FailHow failHow )
				{
					m_failHow = failHow;

					if ( failHow == FailHow_SuppressLabel )
					{
						m_commandOrdinal = PersistenceInstruction_Suppress;
					}
					else if(failHow == FailHow_ForceLabel)
					{
						m_commandOrdinal = PersistenceInstruction_Force;
					}
					else
					{
						m_commandOrdinal = PersistenceInstruction_ForceLeader;
					}
				}

				/**
				 * Gets whether the label should be suppressed if it can't be placed acceptably or
				 * to force it on the map.
				 * @return - FailHow_SuppressLabel or FailHow_ForceLabel
				 * @reqid 001.0063
				 */
				FailHow GetHowToFail() const
				{
					return m_failHow;
				}

				/**
				 * @see SequenceCommand::Execute
				 * @reqid 001.0063
				 */
				virtual void Execute( Map *map, LabelRequest* labelRequest, bool* bSuccess, Error& error,
					ExecutionSequence::Iterator *returnLimit = NULL );

				/**
				* @see SequenceCommand::Clone
				* @reqid 001.0076
				*/
				virtual FailureCommand* Clone( Error& error ) const
				{
					FailureCommand* copy = new FailureCommand();
					if ( copy == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					{
						Copy( *this, *copy );
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
				static void Copy(
					const FailureCommand& src,
					FailureCommand& dest )
				{
					SequenceCommand::Copy( src, dest );

					dest.m_failHow = src.m_failHow;
				}

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				FailHow m_failHow;
			
#ifdef S11N
		protected:
			/**
			 * Serialization - derived class specific.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the child class instance from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
			
		};

	} // namespace Dtp
} // namespace MapText

#endif// DTP_PLACEMENTCOMMAND_H
