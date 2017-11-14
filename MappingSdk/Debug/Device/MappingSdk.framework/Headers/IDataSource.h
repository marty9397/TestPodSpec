///////////////////////////////////////////////////////////
//  IDataSource.h
//  Implementation of the Class IDataSource
//  Created on:      01-May-2012 16:19:26
///////////////////////////////////////////////////////////

#if !defined(EA_7AB60998_A2CA_4569_B519_80D93BCA60FC__INCLUDED_)
#define EA_7AB60998_A2CA_4569_B519_80D93BCA60FC__INCLUDED_

#include "IFeaturePredicate.h"
#include "Map.h"
#include "MtGeometry.h"
#include "DataQuery.h"
#include "IAnnotation.h"
#include <vector>
#include <string>
#include "MapSchema.h"

namespace MapText
{
namespace Dtp
{
    
    class LABELINGENGINE_API LayerDescriptor
    {
    public:
        LayerDescriptor( const TCHAR* name, FeatureType featureType, Error& error )
        {
            m_name.AssignString(name, error);
            if ( error )
            {
                return;
            }
            
            m_featureType = featureType;
        }
        
        FeatureType GetFeatureType() const
        {
            return m_featureType;
        }
        
        void SetFeatureType( FeatureType featureType )
        {
            m_featureType = featureType;
        }
        
        const TCHAR* GetName() const
        {
            return m_name.GetString();
        }
        
        void SetName( const TCHAR* name, Error& error )
        {
            m_name.AssignString(name, error);
            if ( error )
            {
                return;
            }
        }
        
    private:
        FeatureType m_featureType;
        String m_name;
    };

	/**
	 * This is the main interface which any data persistence extension of DTP has to
	 * implement. It provides main entry points for feature and configuration saving
	 * and retrieval.
	 */
	class LABELINGENGINE_API IDataSource
	{

	public:
        
        enum ErrorCode
        {
            ErrorCode_ModeNotSupported = 1 >> 5, // Push up to avoid collision with subclasses codes
            ErrorCode_BadState,     // Internal state is incorrect for the action
            ErrorCode_NoFeatures    // Data source could not create any features
        };
        
		virtual ~IDataSource();

		enum DataSourceMode
		{
			/** This data source does not offer saving capabilities, it's read-only. */
			DataSourceMode_ReadOnly,
			/** This data source may persist changes to the features. */
			DataSourceMode_ReadWrite,
		};

		/**
		 * Get the mode of this data source.
		 * @reqid PERSISTENCE.
		 */
		virtual DataSourceMode GetMode() = 0;
        
        /**
         * Set the mode of this data source. You may want to connect to some sources in read-only mode.
         * Caution: Not all data sources have to support all modes. Verify first with SupportsMode
         * if the mode you need is supported. Otherwise an error may result.
         * @param mode - The mode the datasource should be set to.
         * @param error - Error info. ErrorCode_ModeNotSupported will be reported if the requested mode is not supported.
         */
        virtual void SetMode( DataSourceMode mode, Error& error ) = 0;
        
        /**
         * Check whether the data source supports certain mode of operation.
         * @param mode - The mode to test for.
         * @return - True if mode is supported, false otherwise.
         */
        virtual bool SupportsMode( DataSourceMode mode ) = 0;

		/**
		 * Persist the features' and optionally annotations current states into the data source. 
         * This may not be implemented if the data source is read-only @see GetMode.
		 * @param features - Collection of features to persist.
         * @param annotations - If not NULL the list of annotations will also be persisted.
		 * @param owningMap - Map which owns the features.
		 * @param error - Error info.
		 * @reqid PERSISTENCE.
		 */
		virtual void SaveFeatures(Features& features, Annotations* annotations, Map& owningMap, Error& error) =0;

        /**
         * Persist a new feature 
         * This may not be implemented if the data source is read-only @see GetMode.
         * @param feature - feature to persist.
         * @param owningMap - Map which owns the features.
         * @param error - Error info.
         * @reqid PERSISTENCE.
         */
        virtual void InsertFeature( Feature& feature, Map& owningMap, Error& error ) = 0;

        /**
         * Persist a new feature possibly containing multiple geometries.
         * The features passed in 'featuresCollection' must belong to the same feature record and contain shapes of the same type.
         * Calling this method with a single feature in 'featureCollection' should work like InsertFeature variant with single
         * feature parameter.
         * This may not be implemented if the data source is read-only @see GetMode.
         * @param featureCollection - list features to persist.
         * @param owningMap - Map which owns the features.
         * @param error - Error info.
         * @reqid PERSISTENCE.
         */
        virtual void InsertFeature(Features &featureCollection, Map &owningMap, Error &error) = 0;

        /**
         * Update existing feature
         * This may not be implemented if the data source is read-only @see GetMode.
         * @param feature - feature to update/persist.
         * @param owningMap - Map which owns the features.
         * @param error - Error info.
         * @reqid PERSISTENCE.
         */
        virtual void UpdateFeature( Feature& feature, Map& owningMap, Error& error ) = 0;

		/**
		 * Clear any in-memory cache to reduce resource consumption.
		 * @reqid PERSISTENCE.
		 */
		virtual void ReleaseResources() =0;
        
        /**
         * This should cause the data source to remove any objects it placed in the provided
         * map instance from it. DataSource objects should be properly tagged with the source UUID.
         * @param map - The map to remove objects from
         * @param error - Error info.
		 * @reqid PERSISTENCE.
         */
        virtual void RemoveAllObjectsFromMap(Map& map, Error& error) = 0;

		/**
		 * Process the provided list of features and retrieve attributes for them.
		 * @param[inout] features - Collection of features which should be filled with the dictionaries. 
		 *               Their dictionaries (if any) will be replaced with the data source contents.
		 * @param attributeList - Defines a list of attributes which should be retireved.
		 *        If NULL is passed in all the attributes will be read.
		 * @param error - Error info.
		 * @reqid PERSISTENCE.
		 */
		virtual void ReadAttributesForFeatures(Features& features, String::Collection* attributeList, Error& error) =0;
		
        /**
		 * Synchronizes the feature collection with the backing up data source by adding and optionally removing
		 * the features given the DataQuery configuration. Returned is the number of features read.
		 * @param query - Query configuration controls the behavior of the data retrieval process.
		 * @param featuresCollection - The features collection will be enhanced and optionally trimmed 
		 *        to include all (or only) features meeting the configured criteria (see query).
		 * @param removeObsoleteFeatures - If true the features collection will be trimmed to only include
		 *        the features which match the query. If false the query will only update/add the matching features).
         * @param annotations - Should not be NULL if the ReadAnnotations flag of the query is set to true.
         *        In such case this collection will be populated with the matching annotations.
		 * @param owningMap - Owning map holds the registries of text placement elements such as text styles, placement rules,
		 *        label classes etc. As features are being read they require access to those.
		 * @param error - Error information.
		 * @reqid PERSISTENCE.
		 */
        virtual size_t ReadFeatures(DataQuery& query, Features& featuresCollection, bool removeObsoleteFeatures, Annotations* annotations, Map& owningMap, Error& error) =0;

        /**
         * Query the database for a set of recordids per layer matching the sql query
         * @param query - Query configuration controls the behavior of the data retrieval process.
         * @param featureRecordIds - A QueryResult containing an array of recordids per layer.
         * @param owningMap - Owning map holds the registries of text placement elements such as text styles, placement rules,
         *        label classes etc. As features are being read they require access to those.
         * @param error - Error information.
         * @reqid PERSISTENCE.
         */
        virtual size_t QueryFeatures(DataQuery& query, QueryResult& featureRecordIds, Map& owningMap, Error& error) =0;

        /**
		 * Reads the map configuration from the data source. This method mainly populates the map registries
		 * of text placement configuration objects (label classes, placement rules, text styles etc).
		 * @param configurationName - Name of the map configuration to be read. Data source can keep multiple map
		 *        configurations differentiated by their names.
		 * @param map[in/out] - The map which should receive the configuration from the source if the data source provides its own map [out]
         *                      or the master map [in] otherwise - see HasOwnMap().
         * @param schema[out] - The schema of the map will be read and parsed into the provided instance if the 
         *        data source contains one.
		 * @param error - Error information.
		 * @reqid PERSISTENCE.
		 */
		virtual void ReadMapConfiguration(const char* configurationName, Map& map, MapSchema& schema, Error& error) =0;
        
        /**
         * Get the list of available map configurations valid for this data source.
         * @return - Vector of valid configuration names.
		 * @reqid PERSISTENCE.
         */
        virtual std::vector<std::string> AvailableMapConfigurations() = 0;
        
        /**
         * The default map configuration for this data source.
         * @return - ID of the default configuration.
		 * @reqid PERSISTENCE.
         */
        virtual const char* DefaultMapConfiguration() = 0;

		/**
		 * Saves the map configuration to the data source. This method mainly saves the map registries of
		 * text placement configuration objects (label classes, placement rules, text styles etc).
		 * @param map - The map whose configuration should be saved to the data source.
		 * @param configurationName - Name under which the map configuration should be persisted.
		 * @param error - Error information.
		 * @reqid PERSISTENCE.
		 */
		virtual void SaveMapConfiguration(Map& map, const char* configurationName, Error& error) =0;

		enum MapResourceType
		{
			MapResourceType_Fonts,
            MapResourceType_Symbols,
            MapResourceType_Redlines,
			MapResourceType_Forms,
		};

        /**
         * Get the path where a particular resource type of the source may be found.
         * @param resourceType - Type of the resource requested.
		 * @reqid PERSISTENCE.
         */
		virtual const TCHAR* GetMapResourcePath( MapResourceType resourceType ) = 0;

        /**
         * Get the path to the source.
		 * @reqid PERSISTENCE.
         */
        virtual const TCHAR* GetSourcePath() const = 0;
        
        /**
         * Each implementation has to identity itself with a unique string ID.
		 * @reqid PERSISTENCE.
         */
        virtual const TCHAR* GetIdentity() = 0;
        
        /**
         * This method has to be implemented by each source to provide a way of discovering persisted
         * sources. Each data source implementation should implement a way of doing that (detect if a particular
         * file exists, saving its identity in a designated area etc.
		 * @reqid PERSISTENCE.
         */
        virtual bool IsDataSourcePresentAtPath( const TCHAR* path ) = 0;
        
        /**
         * Factory method, provides a new instance of the implementation.
         * @param error - Error info.
         * @return - The new instance.
		 * @reqid PERSISTENCE.
         */
        virtual IDataSource* NewInstance( Error& error ) = 0;
        
        /**
         * Connect to the media at path
         * @param path - The path where data source media resides.
         * @param error - Error info.
		 * @reqid PERSISTENCE.
         */
        virtual void ConnectToSourceAtPath( const TCHAR* path, Error& error ) = 0;
        
        /**
         * Each source instance has to identify itself with a Universally Unique Identifier. This UUID
         * should be persisted, so that the same source when reopened should present the same UUID.
         * This ID may then be used to identify resources which have been brought by this source into the
         * Map object, which holds data from multiple data sources.
		 * @reqid PERSISTENCE.
         */
        virtual const char* SourceUUID() const = 0;

		/**
		 * This method immediately erases all data stored in this data source from the media.
		 * Only available for the read/write mode of a data source.
		 * @reqid PERSISTENCE.
		 */
		virtual void EraseAllData( Error& error ) = 0;
        
		/**
		 * This method will return the z order priority of this data source.  This will allow the 
           the renderer to determine the order in which features from different data sources should be drawn.
		 */
		virtual int GetZOrderPriority( ) = 0;

		/**
		 * This method will add a new layer with the given name and return the new Layer instance.
		 * @param name - name of a layer
		 * @param error - Error info.
		 * @return - The new instance.
		 * @reqid PERSISTENCE.
		 */
		virtual Layer* AddLayer( const TCHAR* name, Error& error) = 0;

		/**
		 * This method will delete a layer with the given name
		 * @param name - name of a layer
		 * @param error - Error info.
		 * @reqid PERSISTENCE.
		 */
		virtual void DeleteLayer( const TCHAR* name, Error& error) = 0;

		/**
		 * This method will return the layers collection.  It will load the layers into memory
		 * the first time its called
		 * @param name - name of a layer
		 * @param error - Error info.
		 * @reqid PERSISTENCE.
		 */
        virtual const Layers& GetLayers() = 0;

		/**
		 * This method will save the current layers collection.  This will be called implicitly when calling
		 * SaveFeatures but should also be called after adding layers to the data source.
		 * @param name - name of a layer
		 * @param error - Error info.
		 * @reqid PERSISTENCE.
		 */
		virtual void SaveLayers(Error& error) = 0;

        /**
         * Informs whether this data source manages its own map or is just an add-on to a map
         * provided by a different data source.
         * If a data source does not manage its own map, it will receive the master map in ReadMapConfiguration(...).
         * Additionally the data source's ReadFeatures(...) function will be called by the dynamic map processor,
         * so it can add features to the master map.
         *
         * @return true if the data source provides its own map, false otherwise
         */
        virtual const bool HasOwnMap() { return true; }

	};

    /**
     * The manager class is aware of all supported implementations of IDataSource. It facilitates
     * creation of the desired DataSource implementations and connecting to the existing sources.
     * Since the data sources operations may not be thread safe we implement the singleton pattern
     * here to be able to control the threading issues better.
     */
    class LABELINGENGINE_API DataSourceManager
    {
    public:
        
        enum ErrorCode
        {
            ErrorCode_AllocationError = 1,
            ErrorCode_NoSupportingSourceImplementationFound,
        };
        
        /**
         * This method connects to an existing data source at a path. Proper data source format
         * is chosen at runtime
		 * @reqid PERSISTENCE.
         */
        IDataSource* ConnectToSource( const TCHAR* path, Error& error );
        
        /**
         * Create an instance of IDataSource based on the class identity.
         * @param identity - Identity of the data source implementation to create. See the
         *        implementation class for a way of obtaining it.
         * @param error - The error info.
         * @return - The desired class instance or NULL on error.
		 * @reqid PERSISTENCE.
         */
        IDataSource* CreateDataSourceInstanceForIdentity( const TCHAR* identity, Error& error );
        
        /**
         * Access the singleton.
         * @return - A singleton instance of the manager to be used to access the API.
		 * @reqid PERSISTENCE.
         */
        static DataSourceManager& DefaultManager();
        
        /**
         * Add an implementation of the data source at runtime
         * @param instance - The instance of the data source implementation which will be used
         *        to discover and instantiate new data sources of that class. Ownership is taken.
         * @param error - Error info.
		 * @reqid PERSISTENCE.
         */
        void AddDataSourceImplementation( IDataSource* instance );
        
        /**
         * Removes a data source implementation from the managed portfolio at runtime.
         * @param sourceIdentity - Identity of the source implementation to remove.
		 * @reqid PERSISTENCE.
         */
        void RemoveDataSourceImplementation( const TCHAR* identity );
    private:
        /**
         * This class is a singleton class, it should not be explicitely instantiated.
		 * @reqid PERSISTENCE.
         */
        DataSourceManager();
        DataSourceManager(DataSourceManager const&);              // Don't Implement
        void operator=(DataSourceManager const&);                // Don't implement
        ~DataSourceManager();
        
        class LABELINGENGINE_API Impl; Impl* m_pimpl;
    };

} // Dtp
} // MapText

#endif // !defined(EA_7AB60998_A2CA_4569_B519_80D93BCA60FC__INCLUDED_)
