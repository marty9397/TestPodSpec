/**************************************************
 * Copyright (c)2008 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://mtsbs.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/LabelingEngine/Map.h $
 *
 * Revision: $Revision: 80 $
 *
 * Last modified on: $Date: 2008-11-19 07:45:01 -0500 (Wed, 19 Nov 2008) $, by: $Author: pgrenetz $
 *
 *************************************************/

#ifndef DTP_MAP_H
#define DTP_MAP_H

#include "Serialization.h"

#include "LabelingEngineDefs.h"

#include "Collection.h"
#include "MtString.h"
#include "Error.h"
#include "Feature.h"
#include "Label.h"
#include "PlacementRule.h"
#include "PointSymbol.h"
#include "SpatialIndex.h"
#include "IAnnotation.h"
#include <vector>
#include <limits.h>

namespace MapText
{
	namespace Dtp
	{
		/**
		 * Forward declarations
		 */
		class CustomApiFactory;
		class BufferApplicator;

		/**
		 * This interface should be implemented to perform a pre- and postprocessing
		 * of the map for the purpose of labeling.
		 */
		class LABELINGENGINE_API MapProcessor
		{
		public:

			class LABELINGENGINE_API Collection : public ClonableLinkedList<MapProcessor, Collection>
			{
				public:
					/**
					 * Default constructor implemented to avoid compiler generated object code.
                     * @param ownsElements - You may override the default ownership over collection elements.
					 * @reqid 001.0063
					 */
					Collection( bool ownsElements = true )
                        : ClonableLinkedList<MapText::Dtp::MapProcessor, MapText::Dtp::MapProcessor::Collection>(ownsElements)
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
                     * Retrieve a map processor from the collection by specifying it's identity.
                     * If no processor of such identity is found a NULL is returned.
                     * @param processorIdentity - Identity of the map processor requested.
                     * @return - The requested processor or NULL if not found.
                     */
                    MapProcessor* GetByIdentity( const TCHAR* processorIdentity );
                
                    /**
                     * Call this method to sort the collection ascending according to the stack priorities
                     * defined by the processors themselves. @see MapProcessor::GetPriorityInStack.
                     */
                    void SortByStackPriority(Error& error);
			};
            
#ifdef S11N
			class LABELINGENGINE_API Factory
			{
			public:
				/**
				 * Instantiate a map processor implementation given it identity.
				 * @param identity - The identifier of the processor.
				 * @param error [out] - Error information.
				 * @return - The map processor instance or NULL if there was an error.
				 * @reqid N/A.
				 */
				virtual MapProcessor* CreateInstance( const TCHAR* identity, Error& error ) const = 0;

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
#endif

			/**
			 * A flag controlling the processor status.
			 * @return - If true is returned then the processor's event handlers are called.
			 *           If false is returned then processor is inactive - no events handlers are called.
			 * @reqid 001.0063
			 */
			virtual bool IsEnabled() = 0;

			/**
			 * This method will be called before the labeling process. The implementation
			 * should perform any preprocessing required by this processor.
			 * @param map[in,out] - The map to work on.
			 * @param error[out] - Error info.
			 * @reqid 001.0063
			 */
			virtual void PreProcessMap( Map& map, Error& error ) = 0;

			/**
			 * This method will be called after the labeling process. The implementation
			 * should perform any postprocessing required by this processor.
			 * @param map[in,out] - The map to work on.
			 * @param error[out] - Error info.
			 * @reqid 001.0063
			 */
			virtual void PostProcessMap( Map& map, Error& error ) = 0;

			/**
			 * Clone the processor.
			 * @param error[out] - Error info.
			 * @return - The cloned instance.
			 * @reqid 001.0063
			 */
			virtual MapProcessor* Clone( Error& error, Map* parentMap = NULL ) const = 0;

			/**
			 * Returns the processor identifier/description to allow RTTI
			 * @return - The string processor identifier. Implementer should
			 *           assert the uniqueness of the ID across the used processors.
			 * @reqid 001.0063
			 */
			virtual const TCHAR* GetProcessorIdentity() const = 0;

            /**
             * Virtual destructor needed to properly deallocate instances of derived classes.
             * @reqid 001.0063.
             */
            virtual ~MapProcessor()
            {}

            /**
            * By implementing this method the processor may define its place in the
            * processing stack. Before pre- and post-processing the list of filters is
            * sorted according to the priorities in ascending order. This means that the
            * lower priority will get executed first in pre-processing and last in post-processing.
            * The default implementation returns INT_MAX. Child classes don't have to implement the method
            * if they don't care for their place in execution stack. All the processors not overriding this
            * method will execute at the end of queue in non-deterministic order. The pre- and post-processing
            * order will still be maintained though.
            * @return - The priority in stack of map processors.
            */
            virtual int GetPriorityInStack() const
            {
                return INT_MAX;
            }
#ifdef S11N
			/**
			 * Return true if the processor should NOT be persisted in case of serialization.
			 * @return - True to avoid processor serialization, false if it should be persisted.
			 * @reqid N/A.
			 */
			virtual bool IsVolatile() const = 0;

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
			static MapProcessor* DeserializeNew( IDeserializer& deserializer, Error& error );
		protected:
			/**
			 * Serialization.
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
		};
        
        /**
         * Vehicle to provide progress feedback from lengthy operations performed
         * by the engine. An instance of an implmentation of this interface should
         * be passed to the engine method whenever a progress report is needed.
         */
        class LABELINGENGINE_API IProgressReport
        {
        public:
            
            /**
             * Virtual destructor to assert proper deletion of derived classes.
             */
            virtual ~IProgressReport(){};
            
            /**
             * This method is called by the engine when the status should
             * be updated, i.e. significant progress has been made to update
             * the client. The implementation should then arrange passing the
             * information further to where it's needed for display or logging
             * purposes.
             * @param currentOperation - Here the engine will report a human readable
             *                           name for the current operation. The string should
             *                           not have to be retained by the client. The implementation
             *                           should rather make an internal copy.
             * @param currentOperationProgress - Here the progress of the current
             *                                   operation will be reported as a float
             *                                   in the range [0..1).
             *
             */
            virtual void UpdateStatus( const TCHAR* currentOperation,
                                      float currentOperationProgress ) = 0;
            
        };

		class LABELINGENGINE_API MobileMapProperties
		{
		public:

			/**
			* A default constructor.
			*/
			MobileMapProperties()
				: m_originalScale(0), m_backgroundColor(0xFFFFFFFF), m_minPresentableScale( 0 ), m_maxPresentableScale( FLT_MAX ), m_storageScale(0),
				m_symbologyScale( 1.0f ), m_mapLowerleft(0,0), m_minAreaSizeForDisplay(-FLT_MAX),
				m_minLineLengthForDisplay(-FLT_MAX), m_applyMinAreaSizeForDisplay(false), m_applyMinLineLengthForDisplay(false)
			{
			}

			/**
			* Constructor.
			* @param originalScale - The original map scale.
			* @reqid .
			*/
			MobileMapProperties( float originalScale, float storageScale )
				: m_originalScale( originalScale ), m_backgroundColor(0), m_minPresentableScale( 0 ), m_maxPresentableScale( FLT_MAX ), m_storageScale(storageScale),
				m_symbologyScale( 1.0f ), m_mapLowerleft(0,0), m_minAreaSizeForDisplay(-FLT_MAX),m_minLineLengthForDisplay(-FLT_MAX),
				m_applyMinAreaSizeForDisplay(false), m_applyMinLineLengthForDisplay(false)
			{
			}

			/**
			* Required for clonable owners.
			*/
			MobileMapProperties* Clone( Error& error );

			/**
			* Gets the scale at which the GIS data is scaled prior to being stored in the sqlite db
			* @return - Storage Scale
			*/
			float GetStorageScale() const 
			{ 
				return m_storageScale;
			}

			/**
			* Sets Storage Scale
			* @param val - Storage Scale
			*/
			void SetStorageScale(float val) 
			{ 
				m_storageScale = val; 
			}

			/**
			* Gets OriginalScale
			* @return - OriginalScale
			*/
			float GetOriginalScale() const 
			{ 
				return m_originalScale; 
			}

			/**
			* Sets OriginalScale
			* @param val - OriginalScale
			*/
			void SetOriginalScale(float val) 
			{ 
				m_originalScale = val; 
			}

			void GatherFeatureScaleRangeThresholds( Map& ownerMap );

			void AddThresholdIfNotYetKnown( int threshold );

			/**
			* Get the aggregated scale thresholds.
			* @return the thresholds
			*/
			std::vector<int>& GetScaleRangeThresholds()
			{
				return m_scaleRangeThresholds;
			}

			/**
			* Gets BackgroundColor
			* @return - BackgroundColor
			*/
			long GetBackgroundColor() const 
			{ 
				return m_backgroundColor; 
			}

			/**
			* Sets BackgroundColor
			* @param val - BackgroundColor
			*/
			void SetBackgroundColor(long val) 
			{ 
				m_backgroundColor = (int)val;
			}

			/**
			* Gets MaxPresentableScale
			* @return - MaxPresentableScale
			* @reqid N/A.
			*/
			float GetMaxPresentableScale() const 
			{ 
				return m_maxPresentableScale; 
			}

			/**
			* Sets MaxPresentableScale
			* @param val - MaxPresentableScale
			* @reqid N/A.
			*/
			void SetMaxPresentableScale(float val) 
			{ 
				m_maxPresentableScale = val; 
			}

			/**
			* Gets MinPresentableScale
			* @return - MinPresentableScale
			* @reqid N/A.
			*/
			float GetMinPresentableScale() const 
			{ 
				return m_minPresentableScale; 
			}

			/**
			* Sets MinPresentableScale
			* @param val - MinPresentableScale
			* @reqid N/A.
			*/
			void SetMinPresentableScale(float val) 
			{ 
				m_minPresentableScale = val; 
			}

			/**
			* Gets SymbologyScale
			* @return - SymbologyScale
			* @reqid N/A.
			*/
			float GetSymbologyScale() const 
			{ 
				return m_symbologyScale; 
			}

			/**
			* Sets SymbologyScale
			* @param val - SymbologyScale
			* @reqid N/A.
			*/
			void SetSymbologyScale(float val) 
			{ 
				m_symbologyScale = val; 
			}

			/**
			* Gets ProjectionString
			* @return - ProjectionString
			* @reqid N/A.
			*/
			const String *GetProjectionString() const
			{ 
				return &m_projectionString; 
			}

			/**
			* Sets ProjectionString
			* @param val - ProjectionString
			* @reqid N/A.
			*/
			void SetProjectionString(char *val, Error& error) 
			{ 
				m_projectionString.AssignString(val, error);
			}

			/**
			* Gets ProjectionWKTString
			* @return - ProjectionWKTString
			* @reqid N/A.
			*/
			const String *GetProjectionWKTString() const
			{ 
				return &m_projectionWKTString; 
			}

			/**
			* Sets ProjectionWKTString
			* @param val - ProjectionWKTString
			* @reqid N/A.
			*/
			void SetProjectionWKTString(char *val, Error& error) 
			{ 
				m_projectionWKTString.AssignString(val, error);
			}


			/**
			* Gets map lowerleft
			* @return - map lowerleft
			* @reqid N/A.
			*/
			const DoublePoint *GetMapLowerleft() const
			{ 
				return &m_mapLowerleft; 
			}

			/**
			* Sets map lowerleft
			* @param val - map lowerleft
			* @reqid N/A.
			*/
			void SetMapLowerleft(double x, double y)
			{ 
				m_mapLowerleft.SetXY(x, y);
			}

			/**
			* Gets DataAttributionInformation
			* @return - DataAttributionInformation
			*/
			const char* GetDataAttributionInformation() const
			{ 
				return m_dataAttributionInformation.GetString();
			}

			/**
			* Sets DataAttributionInformation
			* @param val - DataAttributionInformation
			*/
			void SetDataAttributionInformation(const char* value, Error& error )
			{ 
				m_dataAttributionInformation.AssignString( value, error );
			}

			/**
			* Gets MinAreaSizeForDisplay
			* @return - MinAreaSizeForDisplay
			* @reqid N/A.
			*/
			float GetMinAreaSizeForDisplay() const 
			{ 
				return m_minAreaSizeForDisplay; 
			}

			/**
			* Sets MinAreaSizeForDisplay
			* @param val - MinAreaSizeForDisplay
			* @reqid N/A.
			*/
			void SetMinAreaSizeForDisplay(float val) 
			{ 
				m_minAreaSizeForDisplay = val;
			}

			/**
			* Gets MinLineLengthForDisplay
			* @return - MinLineLengthForDisplay
			* @reqid N/A.
			*/
			float GetMinLineLengthForDisplay() const 
			{ 
				return m_minLineLengthForDisplay; 
			}

			/**
			* Sets MinLineLengthForDisplay
			* @param val - MinLineLengthForDisplay
			* @reqid N/A.
			*/
			void SetMinLineLengthForDisplay(float val) 
			{ 
				m_minLineLengthForDisplay = val; 
			}

			/**
			* Gets ApplyMinAreaSizeForDisplay
			* @return - ApplyMinAreaSizeForDisplay
			* @reqid N/A.
			*/
			bool GetApplyMinAreaSizeForDisplay() const 
			{ 
				return m_applyMinAreaSizeForDisplay; 
			}

			/**
			* Sets ApplyMinAreaSizeForDisplay
			* @param val - ApplyMinAreaSizeForDisplay
			* @reqid N/A.
			*/
			void SetApplyMinAreaSizeForDisplay(bool val) 
			{ 
				m_applyMinAreaSizeForDisplay = val; 
			}

			/**
			* Gets ApplyMinLineLengthForDisplay
			* @return - ApplyMinLineLengthForDisplay
			* @reqid N/A.
			*/
			bool GetApplyMinLineLengthForDisplay() const 
			{ 
				return m_applyMinLineLengthForDisplay; 
			}

			/**
			* Sets ApplyMinLineLengthForDisplay
			* @param val - ApplyMinLineLengthForDisplay
			* @reqid N/A.
			*/
			void SetApplyMinLineLengthForDisplay(bool val) 
			{ 
				m_applyMinLineLengthForDisplay = val; 
			}

#ifdef S11N
			/**
			* @reqid N/A
			*/
			void Serialize( ISerializer& emitter, Error& error ) const;

			/** 
			*/
			void Deserialize( IDeserializer& deserializer, Error& error );

			static MobileMapProperties* DeserializeNew( IDeserializer& deserializer, MapText::Dtp::Error& error );

#endif

		private:

			float m_originalScale;
			float m_minPresentableScale;
			float m_maxPresentableScale;
			float m_storageScale;
			std::vector<int> m_scaleRangeThresholds;
			int m_backgroundColor;
			float m_symbologyScale;
			String m_projectionString;
			String m_projectionWKTString;
			String m_dataAttributionInformation;
			DoublePoint m_mapLowerleft;
			float m_minAreaSizeForDisplay;
			bool m_applyMinLineLengthForDisplay;
			float m_minLineLengthForDisplay;
			bool m_applyMinAreaSizeForDisplay;
		};

		/**
		 * Root container class for objects that define a map and control its labeling.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Map
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_ClientInterruption = 666, // Make sure it doesn't cover any other error
					ErrorCode_ValidationProblems,
					ErrorCode_MapExtentInvalid,
					ErrorCode_InvalidParameterValue,
#ifdef S11N
					ErrorCode_SerializationFailure,
					ErrorCode_DeserializationFailure,
#endif
				};

				/**
				 * Nested class that iterates over the placed labels of the map
				 * @reqid 001.0063
				 */
				class LABELINGENGINE_API FinalLabelIterator
				{
					public:
						/**
						 * Constructor.
						 * @param map - the map whose positioned labels are to be iterated
						 * @param filter - the composite set of placement status flags to returned during iteration
						 * @reqid 001.0063
						 */
						FinalLabelIterator( const Map* map, LabelRequest::PlacementStatus filter )
							:	m_map( map ),
								m_filter( filter ),
								m_currFeature( NULL ),
								m_currLabelRequest( NULL )
						{
						}

						/**
						* Retrieve the first placed label.
						* @return the first placed label
						* @reqid 001.0063
						*/
						const Label* GetFirst();

						/**
						 * Retrieve the next placed label.
						 * @return the next placed label
						 * @reqid 001.0063
						 */
						const Label* GetNext();

					private:
						/**
						 * Reposition the iterator just before the beginning
						 * @reqid 001.0063
						 */
						void Reset()
						{
							m_currFeature				= NULL;
							m_currLabelRequest	= NULL;
						}

					private:
						/**
						 * Report whether the placement status of the current label request's label
						 * matches the filter passed into the constructor.
						 * @return							true if successfully placed; false if not
						 * @reqid 001.0063
						 */
						bool DoesCurrentLabelRequestMatchFilter();

						/**
						 * The map that is the target of the iteration
						 * @reqid 001.0063
						 */
						const Map* m_map;

						/**
						 * The placement status used to identify labels of interest
						 * @reqid 001.0063
						 */
						LabelRequest::PlacementStatus m_filter;

						/**
						 * The current feature on which the iterator is positioned
						 * @reqid 001.0063
						 */
						Feature* m_currFeature;

						/**
						 * The current label on which the iterator is positioned
						 * @reqid 001.0063
						 */
						LabelRequest* m_currLabelRequest;
				};

				/**
				 * Constructs new instance of Map.
				 * @reqid 001.0063
				 */
				Map();

				Map(Unit units);

				/**
				 * @reqid 001.0063
				 */
				~Map();

				/**
				 * Creates a buffer applicator if one has not been created already for this Map.
                 * @param error - standard error object
				 * @reqid 006.0002
				 */
				void CreateBufferApplicator(Error &error);

				/**
				 * Retrieve the placement rules
				 * @return placement rules collection
				 * @reqid 001.0063
				 */
				PlacementRule::Collection* GetPlacementRules()
				{
					return &m_placementRules;
				}

				/**
				 * Retrieve the label classes defined for this map.
				 * @return label classes collection
				 * @reqid 001.0063
				 */
				LabelClass::Collection* GetLabelClasses()
				{
					return &m_labelClasses;
				}

				/**
				 * Retrieve the text styles defined for this map.
				 * @return text styles collection
				 * @reqid 001.0063
				 */
				TextStyle::Collection* GetTextStyles()
				{
					return &m_textStyles;
				}

				/**
				 * Retrieve the features defined for the map.
				 * @return features collection
				 * @reqid 001.0063
				 */
				Features* GetFeatures()
				{
					return &m_features;
				}

                /**
                 * Retrieve the annotation defined for the map.
                 * @return annotation collection
                 * @reqid ANNOTATION
                 */
                Annotations* GetAnnotations()
                {
                    return &m_annotations;
                }
            
				/**
				 * Get the assembly label definitions collection.
				 * This is a container to allow easy storage of assembly label definitions
				 * which are shared between multiple assembly labels
				 * @return the collection
				 * @reqid 001.0063
				 */
				AssemblyLabelDefinition::Collection* GetAssemblyLabelDefinitions()
				{
					return &m_assemblyLabelDefinitions;
				}

				/**
				 * Get the point symbols collection.
				 * This is a container to allow easy storage of point symbols,
				 * which are shared between multiple point features.
				 * @return the collection
				 * @reqid 001.0063
				 */
				PointSymbol::Collection* GetPointSymbols()
				{
					return &m_pointSymbols;
				}

				/**
				 * Label the map instance once it has been fully populated.
				 * @param error [out] - Error info.
				 * @reqid 001.0063
				 */
				void Label( Error& error );

				/**
				 * Set the interruption flag. If in Label method and set to true the labeling will stop
				 * at before the next label request processed. The flag is lowered implicitely at the beginning of
				 * the labeling process.
				 * @param flag true for interruption.
				 * @reqid 007.0050
				 */
				void SetClientInterruptFlag( bool flag )
				{
					m_clientInterruptFlag = flag;
				}

				/**
				 * Get the current status of the client interruption flag.
				 * @return - the flag value. If true labeling process will stop at the next
				 *           label request processed.
				 * @reqid 007.0050
				 */
				bool GetClientInterruptFlag()
				{
					return m_clientInterruptFlag;
				}

				/**
				 * Get the pole location associated with this map
				 * @return	Point pole location
				 * @reqid 001.0063
				 */
				const Point* GetPoleLocation() const
				{
					return &m_poleLocation;
				}

				/**
				 * Set the pole location associated with this map
				 * @param	x		x coordinate of the pole 
				 * @param	y		y coordinate of the pole
				 * @reqid 001.0063
				 */
				void SetPoleLocation( float x, float y )
				{
					m_poleLocation.SetX( x );
					m_poleLocation.SetY( y );
				}

				/** Compute the graticule angle at the specified location on the latitude line.
				* @param location location at which to compute the angle tangent to the circle (latitude line)
				* @return computed graticule angle
				* @reqid 001.0063
				*/
				float ComputeGraticuleAngle(const Point &location) const;

				/**
				 * Sets time out in miliseconds after which the processing of map will be interrupted.
				 * If equal to 0 then no time out will occur.
				 * @param mapTimeOut time out in miliseconds; "0" indicates no time out
				 * @reqid 001.0063
				 */
				void SetMapTimeOut( unsigned int mapTimeOut )
				{
					m_mapTimeOut = mapTimeOut;
				}

				/**
				 * Gets time out in milliseconds after which the processing of map will be interrupted.
				 * @return time out in milliseconds; "0" indicates no time out
				 * @reqid 001.0063
				 */
				unsigned int GetMapTimeOut() const
				{
					return m_mapTimeOut;
				}

				/**
				* @see SetExtent
				* @return the map extent
				* @reqid 001.0063
				*/
				const UprightRectangle* GetExtent() const 
				{ 
					return &m_extent; 
				}

				/**
				 * Set the extent of the map. This will define the labeled area. For now we simply assume
				 * it's in the same coordinate system as the features (no assumption about page units vs
				 * map units made yet).
				 * @param llx x coordinate of the lower left
				 * @param lly y coordinate of the lower left
				 * @param urx x coordinate of the upper right
				 * @param ury y coordinate of the upper right
				 * @reqid 001.0063
				 */
				void SetExtent( float llx, float lly, float urx, float ury, Error& error )
				{
					if ( llx >= urx || lly >= ury )
					{
						SET_ERROR( error, ErrorCode_MapExtentInvalid, "Map extent has to be a proper "
							"rectangle (lower left south-west of upper right." );
						return;
					}
					m_extent.SetLowerLeft( llx, lly );
					m_extent.SetUpperRight( urx, ury );
				}

				/**
				* Gets SpatialIndex
				* @return SpatialIndex
				* @reqid 001.0064
				*/
				ISpatialIndex* GetSpatialIndex() const 
				{ 
					return m_spatialIndex; 
				}

				/**
				* Builds the spatial index using the features fed to the map before.
				*
				* For now it's a crude implementation discarding 
				* the whole index and rebuilding it using the features collection.
				* @param ignoreInvisibleFlag - If true invisible features are also included
				*                              in the index. It should only be used when 
				*                              the index is used as data storage for partial
				*                              loading.
				* @param ignoreDisabledFlag - If true disabled features are also included
				*                              in the index. It should only be used when 
				*                              the index is used as data storage for partial
				*                              loading.
				* @param indexInterior - If true the polygons are indexed as opaque - they're
				*                        also saved in the cells completely inside the feature.
				*                        Note that this doesn't make their interiors obstacles,
				*                        it only makes them being reported inside the polygon
				*                        area. This option is needed when the index is used for
				*                        storage of the features. Without it the polygons are
				*                        only reported in cells crossed by the borders of the 
				*                        polygon.
				* @param errorInfo error information
				* @return 0 on success, non-zero error code instead
				* @reqid 001.0064
				*/
				void UpdateSpatialIndex( bool ignoreInvisibleFlag, bool ignoreDisabledFlag, bool indexPolygonsInterior, Error& errorInfo );

				/**
				 * This overload populates a custom spatial index provided by the client code. 
				 * Otherwise it's identical to the 
				 * UpdateSpatialIndex( bool ignoreInvisibleFlag, bool ignoreDisabledFlag, bool indexPolygonsInterior, Error& errorInfo )
				 * overload.
				 * @reqid 001.0064
				 */
				void UpdateSpatialIndex( ISpatialIndex* indexToUpdate, bool ignoreInvisibleFlag, bool ignoreDisabledFlag, bool indexPolygonsInterior, Error& errorInfo );

				/**
				* Builds the spatial index using the features fed to the map before.
				*
				* For now it's a crude implementation discarding 
				* the whole index and rebuilding it using the features collection.
				* It defaults to the labeling defaults of presetting the spatial index:
				* ignoreInvisibleFlag and indexInterior are both set to false.
				* @param errorInfo error information
				* @return 0 on success, non-zero error code instead
				* @reqid 001.0064
				*/
				void UpdateSpatialIndex( Error& errorInfo );

                /**
                 * Allow the client to drop the spatial index and free the resources.
                 * The index will get rebuilt at the nect labeling run.
                 * @reqid 001.0064
                 */
                void DeleteSpatialIndex();

				/**
				 * This overload uses the map owned spatial index.
				 * @see IndexFeature( Feature& feature, Error& errorInfo, bool indexInterior = false )
				 * @reqid 001.0064
				 */
				void IndexFeature( Feature& feature, Error& errorInfo, bool indexInterior = false );

				/**
				* Insert a single feature into the provided spatial index
				* @param indexToUse - Index which the feature should be inserted to.
				* @param feature - A feature to be indexed.
				* @param errorInfo [out] - Error info.
				* @param indexInterior - if set to true then the interior of the area feature 
				*                        will also be indexed, i.e. also the cells completely
				*                        inside the polygon will contain a reference to the
				*                        feature as opposed to only the ones through which the
				*                        border crosses.
				 * @reqid 001.0064
				 */
				void IndexFeature( ISpatialIndex* indexToUse, Feature& feature, Error& errorInfo, bool indexInterior = false );
				/**
				* Get the last recorded map object population time.
				* @return the last map object population time in seconds
				* @reqid 001.0063
				*/
				double GetLastMapPopulateTime() const
				{
					return m_lastMapPopulateTime;
				}

				/**
				* Set the map object population time in seconds to be reported to the client on request.
				* @param mapPopulateTime	The map object population time in seconds
				* @reqid 001.0063
				*/
				void SetLastObjectPopulateTime( double mapPopulateTime )
				{
					m_lastMapPopulateTime = mapPopulateTime;
				}

				/**
				* Increment the map object population time.
				* @param incrementalPopulateTime	the time to be added to the object population time in seconds
				* @reqid 001.0063
				*/
				double AddIncrementToObjectPopulateTime( double incrementalPopulateTime )
				{
					m_lastMapPopulateTime += incrementalPopulateTime;
					return m_lastMapPopulateTime;
				}

				/**
				* Get the last recorded spatial index update time.
				* @return the last spatial index update time in seconds
				* @reqid 001.0063
				*/
				double GetLastSpatialIndexUpdateTime() const
				{
					return m_lastSpatialIndexUpdateTime;
				}

				/**
				* Set the spatial index update time in seconds to be reported to the client on request.
				* @param spatialIndexUpdateTime	The spatial index update time in seconds
				* @reqid 001.0063
				*/
				void SetLastSpatialIndexUpdateTime( double spatialIndexUpdateTime )
				{
					m_lastSpatialIndexUpdateTime = spatialIndexUpdateTime;
				}

				/**
				* Get the last recorded labeling time.
				* @return the last labeling time in seconds
				* @reqid 001.0063
				*/
				double GetLastProcessingTime() const
				{
					return m_lastLabelingTime;
				}

				/**
				* Set the labeling time in seconds to be reported to the client on request.
				* @param labelingTime	The labeling time in seconds
				* @reqid 001.0063
				*/
				void SetLastProcessingTime( double labelingTime )
				{
					m_lastLabelingTime = labelingTime;
				}

				/**
				* Get the last recorded time for extracting data needed for rendering.
				* @return the last engine processing time in seconds
				* @reqid 001.0063
				*/
				double GetLastRenderingDataExtractTime() const
				{
					return m_lastRenderingDataExtractTime;
				}

				/**
				* Set the time in seconds for extracting data needed for rendering.
				* @param renderingDataExtractTime	The time for extracting data for rendering
				* @reqid 001.0063
				*/
				void SetLastRenderingDataExtractTime( double renderingDataExtractTime )
				{
					m_lastRenderingDataExtractTime = renderingDataExtractTime;
				}

				/**
				 * Creates copy of self.
				 * @param error an error object to be populated
                 * @param skipFeatures - If true then the clone only contains the map metadata
                 *                       without the features collection.
				 * @return copy of self
				 * @reqid 001.0076
				 */
				virtual Map* Clone( Error& error, bool skipFeatures = false ) const;
            
                /**
                 * Copies a map into another map. Dest old members are purged.
                 * @param error an error object to be populated
                 * @param skipFeatures - If true then the clone only contains the map metadata
                 *                       without the features collection.
                 * @return copy of self
                 * @reqid 001.0076
                 */
                static void Copy( const Map& src, Map& dest, bool skipFeatures, Error& error );

				/**
				* Extract all data from the map objects needed for rendering by the client.
				* Primarily used for time measurement purposes.
				* @reqid 001.0063
				*/
				void ExtractRenderingData() const;

				/**
				* If true the labels in this map should be align to graticule
				* (face the pole location instead of remaining horizontal). 
				* This only applies to labels which don't have the placement angle
				* defined explicitely in the LabelRequest.
				* @return AlignToGraticule
				* @reqid 001.0063
				*/
				bool GetAlignToGraticule() const 
				{ 
					return m_alignToGraticule; 
				}

				/**
				* Sets AlignToGraticule. @see GetAlignToGraticule
				* @param val AlignToGraticule
				* @reqid 001.0063
				*/
				void SetAlignToGraticule(bool val) 
				{ 
					m_alignToGraticule = val; 
				}

				/**
				  * Gets the Buffer Applicator for applying the buffers during overlap detection
				  * @return - buffer applicator
				  * @reqid 006.0002
				*/
				BufferApplicator *GetBufferApplicator()
				{
					return m_bufferApplicator;
				}

				/**
				* Gets ChartElements - a collection of features which are given in coordinates
				* relative to the extent's lower left instead of the absolute feature coordinates.
				* They are automatically translated by the engine with each frame to remain
				* at a fixed position in subsequent frames. This is an easy way to introduce
				* exclusion zones which are overlaid in the screen (compass variation, scale bar etc.).
				* In order to do this create polygon features using the relative coordinates, include 
				* the feature class to which they belong in the GlobalExclusionFeatureClasses collection 
				* of the map. Ahter this they will be treated as opaque obstacles and no label will be placed
				* in the area defined by their bounding boxes.
				* @return - ChartElements
				* @reqid 007.0049
				*/
				MapText::Dtp::Features* GetChartElements()
				{ 
					return &m_chartElements; 
				}

				/**
				* Gets GlobalExclusionFeatureClasses. All features belonging to the listed feature classes
				* will be treated as exclusions: their bounding boxes will be no-go zones for any label
				* in the map.
				* @return - GlobalExclusionFeatureClasses
				* @reqid 007.0049
				*/
				SharedString::Collection* GetGlobalExclusionFeatureClasses()
				{ 
					return &m_globalExclusionFeatureClasses; 
				}

				/**
				 * Validate the map. All the user populated members that may be pre-checked are
				 * tested here and a list of problems is reported if some problems exist.
				 * @param result - [out] True if the map is valid. False otherwise (@see validationErrors).
				 * @param validationErrors - [out] Null if no problems detected. A list of problems otherwise. The list
				 *                           is build of instances of ErrorDetails implementations, most commonly
				 *                           ErrorObjectReference<> template class passing back the object which has
				 *                           some issues.
				 *                           List is populated top -> down. E.g. if a feature has a label request
				 *                           
				 * @param error -
				 * @return -
				 * @reqid 001.0086
				 */
				void Validate( bool* result, ErrorDetails** validationErrors, Error& error );

				/**
				* Gets the time it took to validate the map during the last call to Label
				* @return - the time
				* @reqid 001.0086
				*/
				double GetLastValidationTime() const 
				{ 
					return m_lastValidationTime; 
				}

				/**
				 * Returns the units of the map
				 * @return - units of the map coordinates and properties.
				 * @reqid 001.0063
				 */
				Unit GetUnits()
				{
					return m_units;
				}

				/**
				* Get the MapProcessors collection. This way you can add/remove processors from this
				* map instance. They will be pre-processed in the order defined by their PriorityInStack
                * property (@see MapProcessor::GetPriorityInStack), and post-processed in the inverted order.
				* @return - Modifable collection of map processors for this map.
				* @reqid 001.0063
				*/
				MapProcessor::Collection* GetMapProcessors()
				{ 
					return &m_mapProcessors;
				}

				/**
				* Gets LastPreProcessingTime
				* @return - LastPreProcessingTime
				* @reqid 001.0063
				*/
				double GetLastPreProcessingTime() const 
				{ 
					return m_lastPreProcessingTime; 
				}
				/**
				 * Gets LastPostProcessingTime
				 * @return - LastPostProcessingTime
				 * @reqid 001.0063
				 */
				double GetLastPostProcessingTime() const 
				{ 
					return m_lastPostProcessingTime; 
				}
            
                /**
                 * Get the currently assigned progress reporter instance.
                 * @return - The assigned instance progress reporter implementation.
				 * @reqid 001.0063
                 */
                IProgressReport* GetProgressReporter()
                {
                    return m_progressReport;
                }
            
                /**
                 * Set the assigned progress reporter. Ownership is NOT taken.
                 * @param progressReporter - The instance of a progress reporter implementation
                 *                           to use. NULL may be safely passed to disable reporting.
                 *                           NULL is also the default value as no default progress
                 *                           reporting is used.
				 * @reqid 001.0063
                 */
                void SetProgressReport( IProgressReport* progressReporter )
                {
                    m_progressReport = progressReporter;
                }

				/**
				 * Gets MaxPlacedLabels. @see SetMaxPlacedLabels.
				 * @return - MaxPlacedLabels
				 * @reqid 001.0063
				 */
				int GetMaxPlacedLabels() const 
				{ 
					return m_maxPlacedLabels; 
				}

				/**
				 * Sets MaxPlacedLabels. Set this to the max number of labels the client wants
				 * to be placed. This comes handy when number of labels should be restricted,
				 * e.g. in a dynamic client with limited real estate available on the screen
				 * as a way to avoid cluttering with too many labels or as a performance tweak
				 * to shorten processing time.
				 * @param val - Set the max number of labels. -1 means 'no limit'.
				 * @reqid 001.0063
				 */
				void SetMaxPlacedLabels(int val) 
				{ 
					m_maxPlacedLabels = val; 
				}

				/**
				* This property should be used to persist the scale at which
				* text placement has been performed. When the labeled map is later
				* passed to the renderer this scale should be used to properly overlay
				* the text and map features.
				* @return - The scale at which labeling occured.
				*/
				float GetLabelingScale()
				{
					return m_labelingScale;
				}

				/**
				* @see GetLabelingScale
				* @param labelingScale - The scale at which labeling was invoked.
				*/
				void SetLabelingScale( float labelingScale )
				{
					m_labelingScale = labelingScale;
				}

				/**
				* This property should be used to persist the angle at which
				* text placement has been performed. When the labeled map is later
				* passed to the renderer this angle should be used to properly overlay
				* the text and map features.
				* @return - The map angle at which labeling occured.
				*/
				float GetLabelingAngle()
				{
					return m_labelingAngle;
				}

				/**
				* @see GetLabelingAngle
				* @param labelingAngle - The map angle at which labeling was invoked.
				*/
				void SetLabelingAngle( float labelingAngle )
				{
					m_labelingAngle = labelingAngle;
				}
			
#ifdef S11N
				/**
				 * Serialization.
				 * @param emitter - the emitter to serialize to.
				 * @param error[out] - error info.
				 * @param serializeFeatures - Set to true to include features. If set to false then
				 *                            only the metadata of the map is serialized.
				 * @reqid N/A
				 */
				void Serialize( ISerializer& emitter, Error& error, bool serializeFeatures = true ) const;

				/**
				 * Deserializes the object using a supplied deserializer.
				 * @param deserializer - The deserializer to read from.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				void Deserialize( IDeserializer& deserializer, Error& error );

				/**
				* Access the set of factories for user derived classes used in the engine.
				* @return - The supplied factory.
				* @reqid N/A
				*/
				CustomApiFactory* GetCustomApiFactory() const 
				{ 
					return m_customApiFactory; 
				}

				/**
				 * Set the set of factories for user derived classes used in the engine. 
				 * Ownership is taken. NULL may be safely passed not to use any custom types.
				 * @return - The supplied factory.
				 * @reqid N/A
				 */
				void SetCustomApiFactory( CustomApiFactory* factory );

				/**
				 * Gets MobileMapProperties - properties needed on mobile device
				 * @return - The MobileMapProperties instance assigned.
				 * @reqid 001.0063
				 */
				MobileMapProperties* GetMobileMapProperties() const 
				{ 
					return m_mobileMapProperties; 
				}

				/**
				* Sets MobileMapProperties.
				* @param val - MobileMapProperties
				*              NULL may be passed to delete the previously assigned instance.
				* @reqid 001.0063
				*/
				void SetMobileMapProperties(MobileMapProperties* val) 
				{ 
					delete m_mobileMapProperties;
					m_mobileMapProperties = val; 
				}

				/**
				* Clips the features collection to the map's extent
				* @param error - error object
				* @reqid TODO
				*/
				void ClipFeatures(Error &error);
            
                const char* GetMapProjectionString() const;

				void SetMapProjectionString( const char* projectionString, Error& error );
#ifdef __APPLE__            
				bool HasSimilarProjection(const char *projectionString, const UprightRectangle &areaOfInterest, Error &error) const;
#endif
			private:
				mutable CustomApiFactory* m_customApiFactory;

#endif

			private:
				/**
				* Extract all data from the regular label needed for rendering by the client.
				* Primarily used for time measurement purposes.
				* @param regularLabel	The regular label from which to extract the rendereing data
				* @reqid 001.0063
				*/
				static void ExtractRenderingDataFromRegularLabel( const RegularLabel& regularLabel );

				/**
				 * Initializes the spatial index.
				 * @reqid 001.0064
				 */
				void InitializeSpatialIndex( Error& errorInfo );
            
                /**
                 * Invoke post-processing on any active map processors.
                 * @param processors - The processors list which defines the order of post-processing.
                 * Post processing will take place in the REVERSE order of this list.
                 * @reqid 001.0064
                 */
                void PostProcessMap(MapProcessor::Collection& processors, MapText::Dtp::Error &error);

            
				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			private:
				LabelClass::Collection m_labelClasses;
				TextStyle::Collection m_textStyles;
				PlacementRule::Collection m_placementRules;
				PointSymbol::Collection m_pointSymbols;
				AssemblyLabelDefinition::Collection m_assemblyLabelDefinitions;
				Point m_poleLocation;
				UprightRectangle m_extent;
				bool m_alignToGraticule;
				BufferApplicator *m_bufferApplicator;
				Features m_chartElements;
				unsigned int m_mapTimeOut;
				double m_lastMapPopulateTime;
				double m_lastSpatialIndexUpdateTime;
				double m_lastLabelingTime;
				double m_lastValidationTime;
				double m_lastRenderingDataExtractTime;
				double m_lastPreProcessingTime;
				double m_lastPostProcessingTime;
				SharedString::Collection m_globalExclusionFeatureClasses;
				bool m_clientInterruptFlag;
				Unit m_units;
				MapProcessor::Collection m_mapProcessors;
				MobileMapProperties* m_mobileMapProperties;
				int m_maxPlacedLabels;
				float m_labelingScale;
				float m_labelingAngle;
                String m_projectionString;
            
                /**
                 * Disallow the use of assignment operator. Not supported.
                 * @req 001.0063
                 */
                Map& operator = ( const Map& otherMap )
                {
                    return *this;
                }
                
                /**
                 * Disallow the use of copy constructor. Not supported.
                 * @req 001.0063
                 */
                Map( const Map& otherMap )
                {
                }
                
			protected:
				ISpatialIndex* m_spatialIndex;
				Features m_features;
                Annotations m_annotations;
                IProgressReport* m_progressReport;
		};

		/**
		* Element cloner that sets new map instance for cloned objects.
		* @reqid 001.0076
		*/
		template <class C>
		class ElementCloner : public Cloner< C >
		{
		private:
			Map* m_newParentMap;

		public:
			/**
			* Construct element cloner.
			* @param newParentMap instance of a new parent map
			* @reqid 001.0076
			*/
			ElementCloner( Map* newParentMap )
				: m_newParentMap( newParentMap )
			{}

			/**
			* Clone element and set a new parent map to the cloned instance.
			* @param valueToClone element to clone
			* @param error error information
			* @return new instance
			* @reqid 001.0076
			*/
			virtual C* operator() ( const C* valueToClone, Error& error ) const
			{
				return valueToClone->Clone( error, m_newParentMap ); 
			}
		};

#ifdef S11N

		/**
		 * Gather the factories for user implemented derivations of engine types.
		 * Those factories will be used wherever a custom type could be introduced
		 * and needs to be reinstantiated during deserialization.
		 * @reqid N/A.
		 */
		class LABELINGENGINE_API CustomApiFactory
		{
		public:

			enum ErrorCode
			{
				ErrorCode_AllocationFailure = 1,
			};

			/**
			 * Constructor. All factories are set to NULL.
			 */
			CustomApiFactory();

			/**
			 * Destroys all owned factories.
			 */
			~CustomApiFactory();

			/**
			 * Access the factory of map processors.
			 * @return - The supplied factory.
			 * @reqid N/A
			 */
			MapProcessor::Factory* GetMapProcessorFactory() const 
			{ 
				return m_mapProcessorFactory; 
			}

			/**
			* Sets MapProcessorFactory. Ownership is taken.
			* @param val - MapProcessorFactory
			* @reqid N.A.
			*/
			void SetMapProcessorFactory(MapProcessor::Factory* val) 
			{ 
				delete m_mapProcessorFactory;
				m_mapProcessorFactory = val; 
			}

			/**
			 * Access the factory of custom polylines implementation.
			 * @return - The supplied factory.
			 * @reqid N/A
			 */
			Polyline::Factory* GetPolylineFactory() const 
			{ 
				return m_polylineFactory; 
			}

			/**
			* Sets PolylineFactory. Ownership is taken.
			* @param val - PolylineFactory
			* @reqid N/A.
			*/
			void SetPolylineFactory(Polyline::Factory* val) 
			{ 
				delete m_polylineFactory;
				m_polylineFactory = val; 
			}

			/**
			 * Access the factory of custom polygons implementation.
			 * @return - The supplied factory.
			 * @reqid N/A
			 */
			Polygon::Factory* GetPolygonFactory() const 
			{ 
				return m_polygonFactory; 
			}

			/**
			* Sets PolygonFactory. Ownership is taken.
			* @param val - PolygonFactory
			* @reqid N/A.
			*/
			void SetPolygonFactory(Polygon::Factory* val) 
			{ 
				delete m_polygonFactory;
				m_polygonFactory = val; 
			}

			/**
			* Gets SpatialIndexFactory
			* @return - SpatialIndexFactory
			* @reqid N/A.
			*/
			ISpatialIndex::Factory* GetSpatialIndexFactory() const 
			{ 
				return m_spatialIndexFactory; 
			}

			/**
			* Sets SpatialIndexFactory. Ownership is taken.
			* @param val - SpatialIndexFactory
			* @reqid N/A.
			*/
			void SetSpatialIndexFactory(ISpatialIndex::Factory* val) 
			{ 
				delete m_spatialIndexFactory;
				m_spatialIndexFactory = val; 
			}

			CustomApiFactory* Clone( Error& error );
		private:
			MapProcessor::Factory* m_mapProcessorFactory;
			Polyline::Factory* m_polylineFactory;
			Polygon::Factory* m_polygonFactory;
			ISpatialIndex::Factory* m_spatialIndexFactory;
		};
#endif

	} // namespace Dtp
} // namespace MapText

#endif // DTP_MAP_H
