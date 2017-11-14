/**************************************************
 * Copyright (c)2008 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://martin@mtsrv.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/LabelingEngine/Feature.h $
 *
 * Revision: $Revision: 89192 $
 *
 * Last modified on: $Date: 2017-01-26 09:47:27 -0500 (Thu, 26 Jan 2017) $, by: $Author: lwroczynski $
 *
 *************************************************/

#ifndef DTP_FEATURE_H
#define DTP_FEATURE_H

#include "LabelingEngineDefs.h"
#include "Collection.h"
#include "SharedObjects.h"
#include "MtString.h"
#include "FeatureSymbolDefinition.h"

//Turn on DISPLAY_DEBUGGER in all the configurations.
//If you plan to enable it selectively using command-line compiler flags, then ensures 
//that all compilation units which include Feature.h have the flag defined in exactly same way.
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
		class LabelRequests;
		class Shape;

		/**
		 * Type of feature
		 * @reqid 001.0063
		 */
		enum FeatureType
		{
			FeatureType_Unknown = -1,
			FeatureType_Area,
			FeatureType_Line,
			FeatureType_Point,
			FeatureType_Count
		};

		class Map;
		class Feature;

		/**
		 * This is an abstract class which can be derived from by the client code and
		 * used to store any type of data it needs with some of the engine objects.
		 */
		#ifdef S11N
		// This interface helps us skip features when they are being deserialized.
		class LABELINGENGINE_API IFeatureReadingEliminator
		{
		public:
			
			/**
			 * Decides whether a feature should be skipped.
			 * @param feature - Feature to be tested.
			 * @return - Should return true if the feature should be skipped, false otherwise.
			 * @reqid N/A.
			 */
			virtual bool ShouldFeatureBeSkipped( Feature& feature ) = 0;
            
            /**
             * Virtual destructor is necessary.
             */
            virtual ~IFeatureReadingEliminator(){};
		};
#endif

		/**
		 * Class that holds an array of JoinedFeature pointers, representing 
		 * the joined features of which a feature is a part.
		 * It does not to be clonable. Each cloned map's features will be joined.
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API JoinedFeatures : public LinkedList < Feature >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				JoinedFeatures()
				{
					this->m_ownsElements = false;
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~JoinedFeatures()
				{
				}
		};

		/**
		 * Class representing a feature.
		 * @author Andrzej Ciereszko
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Feature
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_NoFeatureTypeSpecified,
					ErrorCode_UnhandledFeatureType,
				};

				/**
				 * Default constructor.
				 * @param error		an error object to be populated
				 * @reqid 001.0063
				 */
				Feature(MapText::Dtp::Error& error);

				/**
				 * Initialize feature of known type - this also initializes the underlying shape.
				 * @param featureType type of the feature
				 * @param error		an error object to be populated
				 * @reqid 001.0063
				 */
				Feature( FeatureType featureType, MapText::Dtp::Error& error );

				/**
				 * Constructor.
				 * @param	featureType				the feature type
				 * @param	featureID					the id of the feature
				 * @param	featureClassName	the feature class name of the feature
				 * @param error		an error object to be populated
				 * @reqid 001.0063
				 */
				Feature( FeatureType featureType, const TCHAR* featureID,
					const TCHAR* featureClassName, const TCHAR *layerName, MapText::Dtp::Error& error );

				/**
				 * Constructor.
				 * @param	featureType			the feature type
				 * @param	featureID			the id of the feature
				 * @param	featureClassName	the feature class name of the feature
				 * @param	invisible			the invisibility of the feature
				 * @param error		an error object to be populated
				 * @reqid 001.0063
				 */
				Feature( FeatureType featureType, const TCHAR* featureID,
					const TCHAR* featureClassName, const TCHAR *layerName,
					bool invisible, MapText::Dtp::Error& error );

				/**
				 * Destructor
				 * @reqid 001.0063
				 */
				virtual ~Feature();

				//GETTERS & SETTERS BEGIN
				/**
				 * Sets the feature ID
				 * @see	GetFeatureID
				 * @reqid 001.0063
				 */
				void SetFeatureID( const TCHAR* featureID, MapText::Dtp::Error& error )
				{
					m_featureID.AssignString(featureID, error);
				}

				/**
				 * Gets the feature ID.
				 * FeatureID is a textual identifier of the feature, assigned by API client.
				 * This is for client use and may or may not be a unique identifier.
				 * @reqid 001.0063
				 */
				const String* GetFeatureID() const
				{
					return &m_featureID;
				}

				/**
				 * Sets the feature record ID
				 * @see	GetRecordId
				 * @reqid 001.0063
				 */
				void SetRecordID( const TCHAR* recordId, MapText::Dtp::Error& error )
				{
					m_recordID.AssignString(recordId, error);
				}

				/**
				 * Gets the record ID.
				 * Record ID is a textual identifier of the feature record, assigned by API client.
				 * It may be used for maintaining linkage between the DTP feature and a DB recordset.
				 * The property is not used internally by the DTP engine.
				 * @reqid 001.0063
				 */
				const String* GetRecordID() const
				{
					return &m_recordID;
				}
            
                const SharedString* GetUniqueId(Error& error)
                {
                    if(m_uniqueId.GetString() == NULL)
                    {
						if (m_layerName.GetString() == NULL) {
							SET_ERROR(error, -1, "Feature layer is not set.");
							return NULL;
						}
                        String uniqueId(m_layerName.GetString(), error);
                        uniqueId.ConcatString(m_featureID.GetString(), error);
                        m_uniqueId.AssignString(uniqueId.GetString(), error);
                    }
                    
                    return &m_uniqueId;
                }

				/**Gets the feature class name.
				 *
				 * @see GetFeatureClassName
				 * @reqid 001.0063
				 */
				void SetFeatureClassName( const TCHAR* featureClassName, MapText::Dtp::Error& error )
				{
					m_featureClassName.AssignString(featureClassName, error);
				}

				/**Gets the feature class name
				 * FeatureClassName is a textual identifier of the feature, assigned by API client.
				 * This is for client use and may or may not be a unique identifier.
				 * @return the feature's feature class name as a SharedString instance
				 * @reqid 001.0063
				 */
				const SharedString* GetFeatureClassName() const
				{
					return &m_featureClassName;
				}

				/**Gets the layer name
				 *
				 * @see GetLayerName
				 * @reqid 001.0063
				 */
				void SetLayerName( const TCHAR* layerName, MapText::Dtp::Error& error )
				{
					m_layerName.AssignString(layerName, error);
				}

				/**Gets the feature class name
				 * LayerName is a textual identifier of the layer from which the feature originates 
				 * assigned by API client.
				 * @return - Name of layer from which the feature originates.
				 * @reqid 001.0063
				 */
				const SharedString* GetLayerName() const
				{
					return &m_layerName;
				}

				/**
				 * This flag allows to enable/disable a feature. If set to true,
				 * the feature is processed by the engine as an obstacle and any label requests
				 * on it are processed as usually. If set to false the feature is simply
				 * ignored in both aspects.
				 * @return - true if the feature is enabled, false otherwise.
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

                bool GetIsVisible() const
                {
                    return m_isVisible;
                }

                void SetIsVisible( bool isVisible )
                {
                    m_isVisible = isVisible;
                }

				/**
				 * This flag allows to remove a feature. If set to true, the feature is removed
				 * from the map's feature collection before processing commences.
				 * @return - true if request is enabled, false otherwise.
				 * @reqid 001.0063
				 */
				bool GetDiscard() const
				{
					return m_discard;
				}

				/**
				 * @see GetDiscard
				 * @param discard - new value of the discard flag.
				 * @reqid 001.0063
				 */
				void SetDiscard( bool discard )
				{
					m_discard = discard;
				}

				/**Sets the invisibility flag
				 *
				 * @see GetInvisible
				 * @reqid 001.0063
				 */
				void SetInvisible( bool invisible )
				{
					m_invisible = invisible;
				}

				/**Gets the invisibility flag.
				 * Invisible is a flag indicating whether the feature should be treated as a potential obstacle.
				 * If true, ignore as potential obstacle.
				 *
				 * @reqid 001.0063
				 */
				bool GetInvisible() const
				{
					return m_invisible;
				}

				/**Gets the LabelRequests collection.
				 * The label requests associated with this feature. Holds a Label and Feature objects that the user wants labeled.
				 *
				 * @reqid 001.0063
				 */
				LabelRequests* GetLabelRequests() const
				{
					return m_labelRequests;
				}

				/**Gets the JoinedFeatures collection.
				 * The joined features of which this feature is a part..
				 *
				 * @reqid 001.0063
				 */
				JoinedFeatures* GetJoinedFeatures()
				{
					if (m_joinedFeatures == NULL)
					{
						m_joinedFeatures = new JoinedFeatures();
					}

					return m_joinedFeatures;
				}

                /**Sets the LabelRequests collection.
                 *
                 * @reqid 001.0063
                 */
                void SetLabelRequests(LabelRequests* labelRequests);
            
                /**Gets a clone the LabelRequests collection.
                 * A clone of the label requests associated with this feature.
                 * @param error an error object to be populated
                 * @param parentFeature a parent feature for a clone
                 * @param parentMap (optional) a parent map for a clone
                 * @reqid 001.0063
                 */
                LabelRequests* GetLabelRequestsClone(Error& error, Feature* parentFeature, Map* parentMap = NULL ) const;
            
				/**Gets the Shape.
				 * The feature shape that defines the geometry of this feature.
				 *
				 * @reqid 001.0063
				 */
				Shape* GetShape() const
				{
					return m_shape;
				}

				/**SGets the Shape.
				 * The feature shape that defines the geometry of this feature.
				 *
				 * @reqid 001.0063
				 */
				void SetShape(Shape *val);

				/**Gets the FeatureType
				 * @reqid 001.0063
				 */
				FeatureType GetFeatureType() const;

				/**
				 * Set the type of feature. Caution - this also initializes
				 * the underlying shape. Any previously existing shape will be discarded.
				 * @param featureType		the type of the feature
				 * @param error					an Error object
				 * @reqid 001.0063
				 */
				void SetFeatureType( FeatureType featureType, MapText::Dtp::Error& error )
				{
					InitializeShape( featureType, error );
				}

				/**
				* Creates copy of self.
				* @param error an error object to be populated
				* @param parentMap (optional) a parent map for a clone
				* @return copy of self
				* @reqid 001.0076
				*/
				virtual Feature* Clone( MapText::Dtp::Error& error, Map* parentMap = NULL ) const;

				/**
				* Gets OutOfExtent
				* @return OutOfExtent
				* @reqid 001.0063
				*/
				bool GetIsOutOfExtent() const 
				{ 
					return m_isOutOfExtent; 
				}

				/**
				* Sets OutOfExtent
				* @param val OutOfExtent
				* @reqid 001.0063
				*/
				void SetIsOutOfExtent(bool val) 
				{ 
					m_isOutOfExtent = val; 
				}

				/**
				 * In some cases it may be required for the client app to create
				 * a feature which is really a partial clone of another feature, e.g.
				 * only part of the line feature should be used for labeling. However
				 * for the purpose of overlap detection a parent feature collision
				 * should still be detected as a self-overlap. For this purpose
				 * a relation between the master and partial features should be
				 * established using this member. Obstacle detector will use this
				 * to determine whether an overlap caused by a owner feature or not.
				 * @return - parent feature ID
				 * @reqid 001.0063
				 */
				const String* GetParentFeatureId() const
				{ 
					return &m_parentFeatureId;
				}

				/**
				 * Sets ParentFeature
				 * @param val - ParentFeature
				 * @param error[out] - Error info.
				 * @reqid 001.0063
				 */
				void SetParentFeatureId(const TCHAR* val, MapText::Dtp::Error& error) 
				{ 
					m_parentFeatureId.AssignString( val, error ); 
				}

				/**
				 * Reports how many enabled label requests the feature has.
				 * @return the number of enabled label requests the feature has
				 * @reqid 001.0063
				 */
				int GetNumEnabledLabelRequests() const;
            
                /**
                 * Flag which semantics may be interpreted differently by the client code.
                 * This flag is purposfully volatile, i.e. not persisted durng serialization.
                 * @return - The value of the flag.
                 * @reqid 001.0063
                 */
                bool GetIsSelected() const
                {
                    return m_isSelected;
                }
            
                /**
                 * @see GetIsSelected
                 * @reqid 001.0063
                 */
                void SetIsSelected( bool value )
                {
                    m_isSelected = value;                    
                }
            
                /**
                 * This UDID may be used by to identify features which originated from a particular data source. 
                 * Data source implementations should use their UUIDs to tag the features.
                 * @reqid 001.0063
                 */
                const char* GetDataSourceUUID()
                {
                    return m_dataSourceUUID;
                }

                /**
                 * Assign the UUID of the data source to the feature. Ownership is NOT taken, the caller
                 * has to make sure this string lives at least as long as the feature.
                 * @reqid 001.0063
                 */
                void SetDataSourceUUID( const char* uuid )
                {
                    m_dataSourceUUID = uuid;
                }

#if defined(_DEBUG) || defined(DISPLAY_DEBUGGER)
				/* Gets the feature's index within the array view
				* @return index into the array of features
				*/
				int GetFeaturesArrayIndex() const
				{
					return m_featureIndex;
				}

				/* Sets the feature's index within the array view
				 * @param arrayIndex - the features array index
				 */
				void SetFeaturesArrayIndex(int arrayIndex)
				{
					m_featureIndex = arrayIndex;
				}

				/* Gets the feature recordnumber
				*/
				int GetRecordNumber() const
				{
					return m_recordNumber;
				}

				void SetRecordNumber(int recordNum)
				{
					m_recordNumber = recordNum;
				}

				/*
				 * Gets the feature z-order sorting value.
				 * Normally features are sorted by the value of GetRecordNumber
				 * If the record number is not set for both features then this method will be used for comparing.
				 */
				float GetSortOrder() const
				{
					return m_sortOrder;
				}

				void SetSortOrder(float sortOrder)
				{
					m_sortOrder = sortOrder;
				}

				SharedString* GetSymbologySetId()
				{
					return &m_symbologySetId;
				}

                /**
                 * Gets the name of the AR SymbologySet used to draw this feature in AR View
				 * @return name of AR SymbologySet
                 * @reqid TODO
                 */
				SharedString* GetARSymbologySetId()
				{
					return &m_arSymbologySetId;
				}

				/* Gets the feature symbology set
				*/
				SymbologySet* GetSymbologySet() const
				{
					return m_symbologySet;
				}

				void SetSymbologySet(SymbologySet* symbologySet)
				{
					m_symbologySet = symbologySet;
				}

                /**
                 * @See SetSymbologySet
				 * @return a reference to an Augmented Reality symbology set
                 * @reqid TODO
                 */
				SymbologySet *GetARSymbologySet() const
				{
					return m_arSymbologySet;
				}

                /**
                 * Allows setting of the symbology set to be used for rendering the feature when it is viewed in
				   the Augmented Reality view.
				 * @param arSymbologySet - symbology set for this feature to reference 
                 * @reqid TODO
                 */
				void SetARSymbologySet(SymbologySet *arSymbologySet)
				{
					m_arSymbologySet = arSymbologySet;
				}

				/* Gets the feature symbology set
				*/
				Layer* GetLayer() const
				{
					return m_layer;
				}

				void SetLayer(Layer* layer)
				{
					m_layer = layer;
				}

#endif
				//GETTER & SETTERS END

#ifdef S11N
				/**
				 * @see ISerializable::Serialize
				 * @reqid N/A.
				 */
				virtual void Serialize( ISerializer& destination, MapText::Dtp::Error& error ) const;

				/**
				 * Deserializes the feature using the supplied deserializer.
				 * @param node - YAML node to be used for deserialization.
				 * @param ownerMap - Map which owns the feature
				 * @param error - Error info.
				 * @param eliminator - pass an instance of this interface implementation
				 *                     to check it the feature shooldn't be skipped
				 * @param shouldBeDiscarded [out] - if true is returned then feature has not 
				 *                                been fully deserialized based on the eliminator's
				 *                                decision and should be discarded. 
				 *                                If eliminator is not NULL this also has to be not NULL.
				 * @reqid N/A
				 */
				void Deserialize( IDeserializer& deserializer, MapText::Dtp::Error& error, IFeatureReadingEliminator* eliminator = NULL, bool* shouldBeDiscarded = NULL );

                /**
                 * The DeserializeNew method instantiates a new feature by deserializing it
                 * @param deserializer - The deserializer to use.
                 * @param error - Error info.
                 * @return - New feature instance or NULL on error.
                 *
                 * @reqid PERSISTENCE.
                 */
                static Feature* DeserializeNew( IDeserializer& deserializer, MapText::Dtp::Error& error );

#endif

			private:
				/**
				* Initializes the internal shape member
				* according to the feature type.
				* @reqid 001.0063
				*/
				void InitializeShape( FeatureType featureType, MapText::Dtp::Error& error );

				/**
				* Make deep copy of one instance's members to another
				* @param src
				* @param dest
				* @param error	an Error object
				* @reqid 001.0063
				*/
				static void Copy( const Feature& src, Feature& dest, MapText::Dtp::Error& error );

				/****************************************************************************************
				THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
				TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
				****************************************************************************************/
			protected:
				String m_featureID;
				String m_recordID;

                SharedString m_uniqueId;
				SharedString m_featureClassName;
				SharedString m_layerName;
				
				bool m_invisible;

				LabelRequests* m_labelRequests;

				JoinedFeatures* m_joinedFeatures;	// This does not need to be cloned.

				Shape* m_shape;

				bool m_isOutOfExtent;
            
				bool m_isSelected;

				Layer* m_layer;
				SymbologySet* m_symbologySet;
				SharedString m_symbologySetId;
				SymbologySet *m_arSymbologySet;
				SharedString m_arSymbologySetId;

				bool m_isEnabled;
                bool m_isVisible;

				bool m_discard;

				String m_parentFeatureId;

				int m_recordNumber;

				float m_sortOrder;
            
        const char* m_dataSourceUUID;

#if defined(_DEBUG) || defined(DISPLAY_DEBUGGER)
				int m_featureIndex;
#endif
		};

		/**
		 * Class that holds an array of Feature objects.
		 * @author Andrzej Ciereszko
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API Features
			: public ClonableLinkedList < Feature, Features >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				Features( bool ownsElements = true );

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~Features();
            
                /**
                 * Find a feature in the collection using its ID.
                 * This implementation has an avg O(1) complexity (hash table lookup).
                 * @param featureId - ID of the feature to find. Passing NULL is safe and
                 *                    always returns an empty iterator.
                 * @return - Iterator pointing to the feature requested or returning
                 *           true from the IsNull() method if it was not found.
                 * @reqid 001.0063
                 */
                Iterator FindById( const TCHAR* featureId );

                /**
                 * Find features in the collection using its ID and feature class.
                 * @warning This is a very basic implementation with O(n) complexity,
                 * where n is the size of the collection.
                 * @param recordId - ID of the feature record by which seach should be done.
                 * @param layerName - Name of the feature's layer.
                 * @param featureClassName - Name of the feature's class. It may be NULL to search for all feature classes.
                 * @param error[out] - Error information.
                 * @return - List of iterators pointing to the features matching the search criteria.
                 * @reqid 001.0063
                 */
                LinkedList<Iterator>* FindByRecordIdLayerAndFeatureClass( const TCHAR* recordId, const TCHAR* layerName, const TCHAR* featureClassName, Error& error );

                /**
                 * Find features in the collection using its datasource id.
                 * @warning This is a very basic implementation with O(n) complexity,
                 * where n is the size of the collection.
                 * @param datasourceuuid - data source ID of the feature
                 * @param error[out] - Error information.
                 * @return - List of iterators pointing to the features matching the search criteria.
                 * @reqid 001.0063
                 */
                LinkedList<Iterator>* FindByDataSourceUUID( const TCHAR* dataSourceUUID, Error& error );
            
				/**
				 * Gets a unique feature ID.
				 * This may be needed for assigning an ID to a feature generated internally,
				 * e.g., by a MapProcessor implementation such as FeatureJoiningProcessor,
				 * which generates features representing parts of client supplied features.
				 * A client may create features without IDs and we need to generate the IDs.
				 *
				 * The resulting ID is of the format:
				 *
				 *      <prefix><suffix>,
				 *      where <prefix> is provided by the caller {may be NULL or empty] and
				 *      <suffix> is a string that guarantees to make the complete ID string
				 *      unique across the collection and, eventually globally.
				 *
				 * Caller takes ownership.
				 *
				 * @param prefix - A prefix string for the generated ID. It could contain,
				 *								 for example, a brief reason the feature was generated.
				 *								 The FeatureJoingProcessor will pass "JOINEDFEAT"
				 *								 It may be NULL or empty.
				 * @return - The generated feature ID string
				 * @reqid 001.0063
				 */
				String* GetUniqueFeatureId( const TCHAR* prefix, Error& error );
				/**
				 * Overriding the default implementation to maintain the quick-access index.
				 * @reqid N/A
				 */

				virtual void AddElement( Feature* element, Error& error );
            
        /**
         * Overriding the default implementation to maintain the quick-access index.
         * @reqid N/A
         */
        virtual void Clear();

        void RemoveFeature(Feature* feature);
            
		protected:
				virtual void RemoveElement( Element* e );

		private:
			unsigned int m_lastIssuedIdNumber;

			/**
			 * Hide the private parts
			 */
			class LABELINGENGINE_API Impl; Impl* m_pimpl;
		};
	} // namespace Dtp
} // namespace MapText

#endif // DTP_FEATURE_H
