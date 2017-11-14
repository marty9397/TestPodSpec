///////////////////////////////////////////////////////////
//  DataQuery.h
//  Implementation of the Class DataQuery
//  Created on:      26-Dec-2012 16:50:25
///////////////////////////////////////////////////////////

#if !defined(EA_56EC8D49_FD79_4f83_BD30_102B8D8B53B0__INCLUDED_)
#define EA_56EC8D49_FD79_4f83_BD30_102B8D8B53B0__INCLUDED_

#include "MtGeometry.h"
#include "MtString.h"
#include "IFeaturePredicate.h"
#include <map>
#include <vector>

namespace MapText
{
namespace Dtp
{

	/**
	 * DataQuery bundles the configuration parameters to tune the IDataSource query
	 * behavior to what the client needs.
	 */
	struct LABELINGENGINE_API DataQuery
	{
	public:

		DataQuery();

		/**
		* If true the query will be a "spatial" query, respecting the m_extent definition.
		* @return - ApplyExtent
		* @reqid PERSISTENCE.
		*/
		bool GetApplyExtent() const 
		{ 
			return m_applyExtent; 
		}

		/**
		* Sets ApplyExtent
		* @param val - ApplyExtent
		* @reqid PERSISTENCE.
		*/
		void SetApplyExtent(bool val) 
		{ 
			m_applyExtent = val; 
		}
		/**
		* Spatial part of the query - the extent. Only features overlapping the rectangle
		* in any way will be returned
		* @return - Extent
		* @reqid PERSISTENCE.
		*/
		const Rectangle* GetExtent() const 
		{ 
			return &m_extent; 
		}

		/**
		* Sets Extent
		* @param val - Extent
		* @reqid PERSISTENCE.
		*/
		void SetExtent(const Rectangle& val) 
		{ 
			m_extent = val; 
		}
		/**
		* If true the query should process all the layers available. Otherwise a list of
		* layers needs to be supplied in m_layerList.
		* @return - AllLayers
		* @reqid PERSISTENCE.
		*/
		bool GetUseAllLayers() const 
		{ 
			return m_allLayers; 
		}

		/**
		* Sets AllLayers
		* @param val - AllLayers
		* @reqid PERSISTENCE.
		*/
		void SetUseAllLayers(bool val) 
		{ 
			m_allLayers = val; 
		}

		/**
		* Accesses the list of layers which should be processed. The client should NOT delete the returned object.
		* @return - LayerList
		* @reqid PERSISTENCE.
		*/
		String::Collection* GetLayerList()
		{ 
			return &m_layerList; 
		}

		/**
		* If true the feature shapes are read. Otherwise features geometries will not be
		* read.
		* @return - ReadFeatureGeometries
		* @reqid PERSISTENCE.
		*/
		bool GetReadFeatureGeometries() const 
		{ 
			return m_readFeatureGeometries; 
		}

		/**
		* Sets ReadFeatureGeometries
		* @param val - ReadFeatureGeometries
		* @reqid PERSISTENCE.
		*/
		void SetReadFeatureGeometries(bool val) 
		{ 
			m_readFeatureGeometries = val; 
		}

		/**
		* If set to true the attribute data is read as well. Attribute list may be
		* limited by using the attributeList parameter.
		* @return - ReadAttributesData
		* @reqid PERSISTENCE.
		*/
		bool GetReadAttributesData() const 
		{ 
			return m_readAttributesData; 
		}

		/**
		* Sets ReadAttributesData
		* @param val - ReadAttributesData
		* @reqid PERSISTENCE.
		*/
		void SetReadAttributesData(bool val) 
		{ 
			m_readAttributesData = val; 
		}

        /**
         * This flag tells the data source whether the annotations should be read 
         * or not.
         * @return - True if the annotations should be read. False otherwise.
         *
         * @reqid PERSISTENCE.
         */
        bool GetReadAnnotations() const { return m_readAnnotations; }


        /**
         * This flag tells the data source whether the annotations should be read 
         * or not.
         * @param bool val - True if the annotations should be read. False otherwise.
         *
         * @reqid PERSISTENCE.
         */
        void SetReadAnnotations(bool val) { m_readAnnotations = val; }

        /**
		* If true labeling configuration for features (namely the LabelRequest
		* collection) is read.
		* @return - ReadLabelRequests
		* @reqid PERSISTENCE.
		*/
		bool GetReadLabelRequests() const 
		{ 
			return m_readLabelRequests; 
		}

		/**
		* Sets ReadLabelRequests
		* @param val - ReadLabelRequests
		* @reqid PERSISTENCE.
		*/
		void SetReadLabelRequests(bool val) 
		{ 
			m_readLabelRequests = val; 
		}

		/**
		* If true the features are filtered by their scale ranges - only the ones
		* presented at the currently defined scale will be processed (see
		* m_presentationScaleDenominator).
		* @return - ApplyPresentationScaleFilter
		* @reqid PERSISTENCE.
		*/
		bool GetApplyPresentationScaleFilter() const 
		{ 
			return m_applyPresentationScaleFilter; 
		}

		/**
		* Sets ApplyPresentationScaleFilter
		* @param val - ApplyPresentationScaleFilter
		* @reqid PERSISTENCE.
		*/
		void SetApplyPresentationScaleFilter(bool val) 
		{ 
			m_applyPresentationScaleFilter = val; 
		}

		/**
		* The denominator of the current presentation scale (e.g. for the scale of 1:7500
		* the value should be set to 7500). If applied only features appearing at this
		* scale will be retrieved.
		* @return - PresentationScaleDenominator
		* @reqid PERSISTENCE.
		*/
		float GetPresentationScaleDenominator() const 
		{ 
			return m_presentationScaleDenominator; 
		}

		/**
		* Sets PresentationScaleDenominator
		* @param val - PresentationScaleDenominator
		* @reqid PERSISTENCE.
		*/
		void SetPresentationScaleDenominator(float val) 
		{ 
			m_presentationScaleDenominator = val; 
		}

		/**
		* If true an auxiliary query will be used to filter the results. See
		* m_auxiliaryQuery as well.
		* @return - ApplyAuxQuery
		* @reqid PERSISTENCE.
		*/
		bool GetApplyAuxQuery() const 
		{ 
			return m_applyAuxQuery; 
		}

		/**
		* Sets ApplyAuxQuery
		* @param val - ApplyAuxQuery
		* @reqid PERSISTENCE.
		*/
		void SetApplyAuxQuery(bool val) 
		{ 
			m_applyAuxQuery = val; 
		}
		/**
		* You can define an auxiliary query here, which is basically the WHERE part of
		* the SQL query.
		* @return - AuxiliaryQuery
		* @reqid PERSISTENCE.
		*/
		const TCHAR* GetAuxiliaryQuery() const 
		{ 
			return m_auxiliaryQuery.GetString();
		}

		/**
		* Sets AuxiliaryQuery
		* @param val - AuxiliaryQuery
		* @reqid PERSISTENCE.
		*/
		void SetAuxiliaryQuery(const TCHAR* val, Error& error) 
		{ 
			m_auxiliaryQuery.AssignString( val, error );
		}
		/**
		* Pass a predicate instance to include certain features in the results when
		* predicate is met.
		* @return - Client code should NOT deallocate the return value.
		* @reqid PERSISTENCE.
		*/
		IFeaturePredicate* GetInclusionPredicate() const 
		{ 
			return m_inclusionPredicate; 
		}

		/**
		* Sets InclusionPredicate. Ownership is taken.
		* @param val - InclusionPredicate
		* @reqid PERSISTENCE.
		*/
		void SetInclusionPredicate(IFeaturePredicate* val) 
		{ 
			if ( val != m_inclusionPredicate )
			{
				delete m_inclusionPredicate;
				m_inclusionPredicate = val; 
			}
		}

		/**
		* Pass a predicate instance to exclude certain features from the results when
		* predicate is met.
		* @return - Client code should NOT deallocate the return value.
		* @reqid PERSISTENCE.
		*/
		IFeaturePredicate* GetExclusionPredicate() const 
		{ 
			return m_exclusionPredicate; 
		}

		/**
		* Sets ExclusionPredicate. Ownership is taken.
		* @param val - ExclusionPredicate
		* @reqid PERSISTENCE.
		*/
		void SetExclusionPredicate(IFeaturePredicate* val) 
		{ 
			if ( val != m_exclusionPredicate )
			{
				delete m_exclusionPredicate;
				m_exclusionPredicate = val; 
			}
		}

		/**
		* If true only the specified attributes are read. All available attributes of the
		* features are read otherwise (SELECT *).
		* @return - ApplyAttributesList
		* @reqid PERSISTENCE.
		*/
		bool GetApplyAttributesList() const 
		{ 
			return m_applyAttributesList; 
		}

		/**
		* Sets ApplyAttributesList
		* @param val - ApplyAttributesList
		* @reqid PERSISTENCE.
		*/
		void SetApplyAttributesList(bool val) 
		{ 
			m_applyAttributesList = val; 
		}

		/**
		* A list of attributes to read from the source for every feature.
		* @return - AttributesList. Client code should NOT deallocate the returned value.
		* @reqid PERSISTENCE.
		*/
		String::Collection* GetAttributesList()
		{ 
			return &m_attributesList; 
		}

	private:
		bool m_applyExtent;
		Rectangle m_extent;
		bool m_allLayers;
		String::Collection m_layerList;
		bool m_readFeatureGeometries;
		bool m_readAttributesData;
		bool m_readLabelRequests;
        bool m_readAnnotations;
        bool m_applyPresentationScaleFilter;
		float m_presentationScaleDenominator;
		bool m_applyAuxQuery;
		String m_auxiliaryQuery;
		IFeaturePredicate* m_inclusionPredicate;
		IFeaturePredicate* m_exclusionPredicate;
		bool m_applyAttributesList;
		String::Collection m_attributesList;
	};

    typedef std::vector<std::string> QueryResultStrings;
    
    /* mapping of layername to vector of feature ids*/
    typedef std::map<std::string, QueryResultStrings > QueryResult;

} //Dtp
} // MapText


#endif // !defined(EA_56EC8D49_FD79_4f83_BD30_102B8D8B53B0__INCLUDED_)
