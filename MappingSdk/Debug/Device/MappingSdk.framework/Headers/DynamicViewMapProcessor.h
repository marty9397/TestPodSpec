/*
 *  DynamicViewMapProcessor.h
 *  iDtp
 *
 *  Created by Marcin Fraczak on 5/3/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DYNAMICVIEWMAPPROCESSOR_H
#define DYNAMICVIEWMAPPROCESSOR_H

#include "Map.h"
#include "MTGLMap.h"
#include "MapSchema.h"
#include "IDataSource.h"
#include <map>
#include <set>

namespace MapText
{        
    namespace MappingSdk
    {
        class FeatureExclusionPredicate : public MapText::Dtp::IFeaturePredicate
        {
        public:
            FeatureExclusionPredicate() : IFeaturePredicate()
            {
            }
            
            ~FeatureExclusionPredicate(){}
            

            void AddFeatureIdToExclude(const char* featureID)
            {
                m_featureIds.insert(featureID);
            }
            
            void RemoveFeatureIdToExclude(const char* featureId)
            {
                std::set<std::string>::iterator idIterator = m_featureIds.find(featureId);
                if(idIterator != m_featureIds.end())
                {
                    m_featureIds.erase(idIterator);
                }
            }
            
            void ResetFeatureIds()
            {
                m_featureIds.clear();
            }
            
            bool Evaluate(MapText::Dtp::Feature& feature);            
            
        private:
            std::set<std::string> m_featureIds;
        };

        class DynamicViewMapProcessor : public MapText::Dtp::MapProcessor
        {
        public:
            DynamicViewMapProcessor( MapText::Dtp::Map& masterMap, MapText::Dtp::MTGLMap& mtglMap )
                : m_angle(0), m_viewportCenter(0,0), m_viewportWidth(0),
                m_viewportHeight(0), m_scale(1.0f), m_enabled( true ), m_releaseCache(false), m_mapCompositionTOC(NULL),
                m_mapDataSourceCollection(false)
            {
                m_masterMap = &masterMap;
                m_mtglMap = &mtglMap;
                m_exclusionPredicate = new FeatureExclusionPredicate();
            };
            
            /**
             * @see MapProcessor::IsEnabled
             */
            virtual bool IsEnabled()
            {
                return m_enabled;
            }
            
            /**
             * @see MapProcessor::PreProcessMap
             */
            virtual void PreProcessMap( MapText::Dtp::Map& map, MapText::Dtp::Error& error );
            
            /**
             * @see MapProcessor::PostProcessMap
             */
            virtual void PostProcessMap( MapText::Dtp::Map& map, MapText::Dtp::Error& error );
            
            /**
             * @see MapProcessor::Clone
             */
            virtual MapProcessor* Clone( MapText::Dtp::Error& error, MapText::Dtp::Map* parentMap = NULL ) const
            {
                // Not needed
                throw "Not implemented.";
            }
            
            /**
             * @see MapProcessor::GetProcessorIdentity
             */
            virtual const TCHAR* GetProcessorIdentity() const
            {
                return DynamicViewMapProcessor::_GetProcessorIdentity();
            }
            
            static const TCHAR* _GetProcessorIdentity()
            {
                return _T( "MapText.Dtp.iOsPrototype.DynamicViewMapProcessor" );
            }
            
            virtual int GetPriorityInStack() const
            {
                // Has to be executed as the first processor
                return 0;
            }
            
            void SetViewportWidth( float viewportWidth )
            {
                m_viewportWidth = viewportWidth;
            }
            
            void SetViewportHeight( float viewportHeight )
            {
                m_viewportHeight = viewportHeight;
            }
            
            void SetViewportCenter( const MapText::Dtp::Point& center )
            {
                m_viewportCenter = center;
            }
            
            MapText::Dtp::IDataSource* GetMapDataSource(std::string dataSourceIdentity)
            {
                for (MapText::Dtp::LinkedList<MapText::Dtp::IDataSource>::Iterator dataSource = m_mapDataSourceCollection.Begin();dataSource.IsNull(); dataSource++)
                {
                    if(dataSourceIdentity == dataSource->GetIdentity())
                    {
                        return dataSource.GetElement();
                    }
                }
                
                return NULL;
            }
            
            /**
             * Ownership is NOT assumed. It's a weak reference.
             */
            void AddMapDataSource( MapText::Dtp::IDataSource* dataSource, MapText::Dtp::Error& error )
            {
                m_mapDataSourceCollection.AddElement(dataSource, error);
            }

            /**
             * Removes a previously added data source.
             * @param source the data source to remove
             * @error [out] error information
             */
            void RemoveMapDataSource(Dtp::IDataSource *source, Dtp::Error &error);

            void SetCurrentScale( float scale )
            {
                m_scale = scale;
            }

            void SetCurrentRealScale( float scale )
            {
                m_realscale = scale;
            }

            void ReleaseCacheAtNextProcessing()
            {
                m_releaseCache = true;
            }
            
            void ReleaseCache( MapText::Dtp::Error& error );
            
            float GetCurrentAngle()
            {
                return m_angle;                    
            }
            
            void SetCurrentAngle( float angle )
            {
                m_angle = angle;
            }
            
            void SetMapCompositionTOC(MapText::Dtp::MapCompositionTOC& mapCompositionTOC)
            {
                m_mapCompositionTOC = &mapCompositionTOC;
            }
            
            FeatureExclusionPredicate* GetExclusionPredicate()
            {
                return m_exclusionPredicate;
            }
            
            void GetMapExtentFeatures( MapText::Dtp::Map& map, MapText::Dtp::UprightRectangle& windowExtent, float angle, MapText::Dtp::Error& error );
            
            Dtp::UprightRectangle GetMapExtext();

#ifdef S11N
            /**
             * @see MapProcessor::IsVolatile
             * Yes, this processor is volatile.
             */
            bool IsVolatile() const
            {
                return true;
            }

        protected:
            virtual void _Serialize( MapText::Dtp::ISerializer& emitter, MapText::Dtp::Error& error ) const
            {
                // Here the intent is to actually NOT serialize itself
                // but let's pretend we are. When resurrected we should be
                // inactive anyway.
            }
            
            /** 
             * Deserializes the object from a deserializer.
             * @param deserializer - Source for deserialization.
             * @param error - Error info.
             */
            virtual void _Deserialize( MapText::Dtp::IDeserializer& deserializer, MapText::Dtp::Error& error )
            {
                // After deserialization it should no longer be a dynamic map
                // so let's just disable ourselves.
                m_enabled = false;
            }

#endif
            
        private:
            MapText::Dtp::Map* m_masterMap;
            MapText::Dtp::MTGLMap* m_mtglMap;
            float m_viewportWidth;
            float m_viewportHeight;
            MapText::Dtp::Point m_viewportCenter;
            bool m_enabled;
            bool m_releaseCache;

            float m_angle;
            float m_scale;
            float m_realscale;
            MapText::Dtp::MapCompositionTOC *m_mapCompositionTOC;
            MapText::Dtp::LinkedList<MapText::Dtp::IDataSource> m_mapDataSourceCollection;
            FeatureExclusionPredicate* m_exclusionPredicate;
        };
    }
}

#endif