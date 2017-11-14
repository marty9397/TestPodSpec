//
//  FeatureRecord.h
//  
//
//  Created by Marcin Fraczak on 10/14/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef FEATURERECORD_H
#define FEATURERECORD_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <float.h>

#include "Error.h"
#include "MtGeometry.h"
#include "MapSchema.h"
#include "Feature.h"
#include "FeatureAttachment.h"

namespace MapText
{
    namespace Dtp
    {
        /**
         * Feature bookmark points to a feature by its layer and feature id
         */
        class FeatureBookMark
        {
        public:
            
            enum ErrorCode
            {
                ErrorCode_AllocationFailure = 1,
            };
            
            FeatureBookMark()
            {}
            
            virtual ~FeatureBookMark(){}
            
            FeatureBookMark( const char* layerName, const char* recordId, double lat, double lon, bool isVirtual ) : m_layerConfigRef(NULL)
            {
                m_recordId = recordId;
                m_layerName = layerName;
                m_decimalDegLat = lat;
                m_decimalDegLong = lon;
                m_isVirtual = isVirtual;
            };
            
            FeatureBookMark& operator= (const FeatureBookMark& rhs)
            {
                if ( this == &rhs )
                {
                    return *this;
                }
                
                m_recordId = rhs.m_recordId;
                m_layerName = rhs.m_layerName;
                m_decimalDegLat = rhs.m_decimalDegLat;
                m_decimalDegLong = rhs.m_decimalDegLong;
                m_isVirtual = rhs.m_isVirtual;
                SetLayerConfig( rhs.GetLayerConfig() );
                
                return *this;
            }
            
//            FeatureBookMark *Clone( Error& error )
//            {
//                FeatureBookMark* clone = new FeatureBookMark();
//                if ( clone == NULL )
//                {
//                    SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
//                    return NULL;
//                }
//                
//                *clone = *this;
//                
//                return clone;
//            }
            
            // TODO: make private
            
            SchemaLayer* GetLayerConfig() const
            {
                return m_layerConfigRef;
            }
            
            void SetLayerConfig( SchemaLayer* layerConfigRef )
            {
                m_layerConfigRef = layerConfigRef;
            }
        public:
            std::string m_recordId;
            std::string m_layerName;
            float m_decimalDegLat;
            float m_decimalDegLong;
            bool m_isVirtual;
        private:
            SchemaLayer* m_layerConfigRef;
        };
        
        /**
         * Feature record reflects the data related to a feature read from the DB
         */
        class FeatureRecord : public FeatureBookMark
        {
        public:
            
            class Impl;
            
            enum ErrorCode
            {
                ErrorCode_SpecialFieldsNotFound = 1,
                ErrorCode_IdentifyFieldNotFound,
                ErrorCode_SpecialFieldTypeInvalid,
            };
            
            FeatureRecord( const char* featureId );
            
            FeatureRecord( const FeatureRecord& other );
            
            ~FeatureRecord();
            
            std::string GetPreviewString( ) const;
            
            const SchemaTextStyle* GetPreviewTextStyle( ) const;

            void ClearAttachments()
            {
                m_attachments.clear();
            }
            
            void ClearAttributes()
            {
                // Attributes are owned
                m_attributes.clear();
            }
            
            void ClearNonSpecialTypeAttributeValues()
            {
                for (Attributes::iterator attribute = m_attributes.begin(); attribute != m_attributes.end(); attribute++)
                {
                    if(!attribute->m_descriptorRef->m_specialType)
                    {
                        attribute->m_value.SetEmptyValue();
                    }
                }
            }
            
            FeatureRecord* CloneWithoutAttributes( Error& error )
            {
                FeatureRecord* clone = new FeatureRecord( *this );
                if (clone == NULL )
                {
                    SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
                    return NULL;
                }
                
                // Avoid ownership conflicts by detaching attributes and detail records
                clone->m_attributes.clear();
                clone->m_attachments.clear();
                
                return clone;                
            }
            
            FeatureRecord* Clone( Error& error )
            {
                FeatureRecord* clone = CloneWithoutAttributes( error );
                if ( error )
                {
                    return NULL;
                }

                // Clone the attributes and attachments
                clone->m_attributes.CopyAttributes(m_attributes);
                clone->m_attachments.insert(clone->m_attachments.begin(), m_attachments.begin(), m_attachments.end());
                
                return clone;
            }
            
            FeatureRecord& operator= (const FeatureRecord& rhs);
            
            /**
             * Set the internal sqlite3 db handle.
             * @param dbHandle - Opaque pointer to avoid pulling in sqlite dependencies. 
                                 It needs to be a valid sqlite3* pointer.
             */
            void SetDbHandle( void* dbHandle );
            
            /**
             * Get the internal sqlite3 db handle.
             * @return - An opaque pointer to avoid pulling in sqlite dependencies.
                         It really is a valid sqlite3* pointer.
             */
            void* GetDbHandle() const;
            
            void GetBBox( Point& lowerLeft, Point& upperRight, Dtp::Error& error  ) const
            {
                lowerLeft.SetXY(-FLT_MAX, -FLT_MAX);
                upperRight.SetXY(-FLT_MAX, -FLT_MAX);
                
                if ( m_lowerLeft.GetX() == -FLT_MAX )
                {
                    // Find the special attributes
                    int attsFound = 0;
                    
                    for ( Attributes::const_iterator attribute = m_attributes.begin();
                         attribute != m_attributes.end(); attribute++ )
                    {
                        switch ( (*attribute).m_descriptorRef->m_specialType )
                        {
                            case AttributeDescriptor::AttributeSpecialType_BBoxLLX:
                            {
                                assert( (*attribute).m_value.GetValueType() == AttributeValue::ValueType_Float );
                                attsFound++;
                                m_lowerLeft.SetX( (*attribute).m_value.GetFloatValue() );
                            }
                                break;
                            case AttributeDescriptor::AttributeSpecialType_BBoxLLY:
                            {
                                assert( (*attribute).m_value.GetValueType() == AttributeValue::ValueType_Float );
                                attsFound++;
                                m_lowerLeft.SetY( (*attribute).m_value.GetFloatValue() );
                            }
                                break;
                            case AttributeDescriptor::AttributeSpecialType_BBoxURX:
                            {
                                assert( (*attribute).m_value.GetValueType() == AttributeValue::ValueType_Float );
                                attsFound++;
                                m_upperRight.SetX( (*attribute).m_value.GetFloatValue() );
                            }
                                break;
                            case AttributeDescriptor::AttributeSpecialType_BBoxURY:
                            {
                                assert( (*attribute).m_value.GetValueType() == AttributeValue::ValueType_Float );
                                attsFound++;
                                m_upperRight.SetY( (*attribute).m_value.GetFloatValue() );
                            }
                                break;
                            default:
                                break;
                        }
                    }
                    
                    if ( attsFound < 4 )
                    {
                        m_lowerLeft.SetXY(-FLT_MAX, -FLT_MAX);
                        m_upperRight.SetXY(-FLT_MAX, -FLT_MAX);
                        
                        SET_ERROR_NOMSG( error, ErrorCode_SpecialFieldsNotFound );
                        return;
                    }
                }
                
                lowerLeft = m_lowerLeft;
                upperRight = m_upperRight;
            }
            
            /**
             * Retrieves the MIN/MAX scale at which the feature should appear according to the
             * database MinScale and MaxScale fields as defined by the schema.
             * @param minScale[out] - The min scale denominator at which the feature appears or 0 if
             *        there's no min scale defined (NULL in the DB).
             * @param maxScale[out] - The max scale denominator at which the feature appears or 0 if
             *        there's no max scale defined (NULL in the DB).
             * @param error[out] - Error info.
             */
            void GetMinMaxScale( float* minScale, float* maxScale, Dtp::Error& error  ) const
            {
                *minScale = -FLT_MAX;
                *maxScale = -FLT_MAX;
                
                if ( m_minScale == -FLT_MAX )
                {
                    // Find the special attributes
                    int attsFound = 0;
                    
                    for ( Attributes::const_iterator attribute = m_attributes.begin();
                         attribute != m_attributes.end(); attribute++ )
                    {
                        switch ( (*attribute).m_descriptorRef->m_specialType )
                        {
                            case AttributeDescriptor::AttributeSpecialType_MinScale:
                            {
                                attsFound++;
                                if ( (*attribute).m_value.GetValueType() == AttributeValue::ValueType_Null )
                                {
                                    m_minScale = 0;
                                }
                                else
                                {
                                    assert( (*attribute).m_value.GetValueType() == AttributeValue::ValueType_Float );
                                    m_minScale = (*attribute).m_value.GetFloatValue();
                                }
                            }
                                break;
                            case AttributeDescriptor::AttributeSpecialType_MaxScale:
                            {
                                attsFound++;
                                if ( (*attribute).m_value.GetValueType() == AttributeValue::ValueType_Null )
                                {
                                    m_maxScale = 0;
                                }
                                else
                                {
                                    assert( (*attribute).m_value.GetValueType() == AttributeValue::ValueType_Float );
                                    m_maxScale = (*attribute).m_value.GetFloatValue();
                                }
                            }
                                break;
                            default:
                                break;
                        }
                    }
                    
                    if ( attsFound < 2 )
                    {
                        m_minScale = -FLT_MAX;
                        m_maxScale = -FLT_MAX;
                        
                        SET_ERROR_NOMSG( error, ErrorCode_SpecialFieldsNotFound );
                        return;
                    }
                }
                
                *minScale = m_minScale;
                *maxScale = m_maxScale;
            }
            
            void SetIdentifyFieldValue (std::string& identValue, Error& error)
            {
                bool identFieldFound = false;
                for ( Attributes::iterator attribute = m_attributes.begin();
                     attribute != m_attributes.end(); attribute++ )
                {
                    if((*attribute).m_descriptorRef->m_isIdentity )
                    {
                        (*attribute).m_value.SetStringValue(strdup(identValue.c_str()));
                        identFieldFound = true;
                        break;
                    }
                }
                
                if ( !identFieldFound )
                {
                    SET_ERROR_NOMSG( error, ErrorCode_IdentifyFieldNotFound );
                }
            }
            
            Attribute* GetIdentifyField(Error& error)
            {
                for ( Attributes::iterator attribute = m_attributes.begin();
                     attribute != m_attributes.end(); attribute++ )
                {
                    if((*attribute).m_descriptorRef->m_isIdentity )
                    {
                        return &(*attribute);
                        break;
                    }
                }
                
                SET_ERROR_NOMSG( error, ErrorCode_IdentifyFieldNotFound );
                
                return NULL;
            }

            void SetStringValueForAttributeName(std::string attrName, const char* value, Error& error)
            {
                for ( Attributes::iterator attribute = m_attributes.begin();
                     attribute != m_attributes.end(); attribute++ )
                {
                    if((*attribute).m_descriptorRef->m_dataSourceName == attrName )
                    {
                        return (*attribute).m_value.SetStringValue(strdup(value));
                    }
                }
            }

            std::string GetStringValueForAttributeName(std::string attrName, Error& error)
            {
                for ( Attributes::iterator attribute = m_attributes.begin();
                     attribute != m_attributes.end(); attribute++ )
                {
                    if((*attribute).m_descriptorRef->m_dataSourceName == attrName )
                    {
                        return (*attribute).m_value.GetStringRepresentation();
                    }
                }
                
                return "";
            }
            
            AttributeValue GetAttributeValueForAttributeName(std::string attrName, bool ignoreCase, Error& error)
            {
                if ( ignoreCase )
                {
                    std::transform(attrName.begin(), attrName.end(),attrName.begin(), ::toupper);
                }
                
                for ( Attributes::iterator attribute = m_attributes.begin();
                     attribute != m_attributes.end(); attribute++ )
                {
                    std::string currentAttributeName = (*attribute).m_descriptorRef->m_dataSourceName;
                    if ( ignoreCase )
                    {
                        std::transform(currentAttributeName.begin(), currentAttributeName.end(),currentAttributeName.begin(), ::toupper);
                    }
                    
                    if(currentAttributeName == attrName )
                    {
                        return (*attribute).m_value;
                    }
                }
                
                AttributeValue null;
                null.SetNull();
                
                return null;
            }
            
            void SetAttributesEdited (bool edited, Error& error);
            
            bool GetAttributesEdited( Error& error );

            void SetGeometryEdited (short edited, Error& error);
            
            short GetGeometryEdited( Error& error );

            /**
             * Set the referenced feature.
             * @param featureRef - Internally a clone of the feature is made. Ownership
             *        over the parameter is not assumed.
             * @param error - Error info.
             */
            void SetFeatureReference(Feature* featureRef, Error& error )
            {
                if ( featureRef == m_featureRef )
                {
                    // Nothing to do.
                    return;
                }
                delete m_featureRef;
                m_featureRef = NULL;
                if(featureRef != NULL)
                {
                    m_featureRef = featureRef->Clone(error);
                    if ( error )
                    {
                        return;
                    }
                }
            }
            
            void UpdateBBoxAttributes();

            Feature* GetFeatureReference()
            {
                return m_featureRef;
            }
            
            void SetOverideAttributeReadOnly(bool val)
            {
                m_overideAttributeReadOnly = val;
            }
            
            bool GetOverideAttributeReadOnly()
            {
                return m_overideAttributeReadOnly;
            }
            
            void MarkForDeletion();
            
            bool GetMarkedForDeletion()
            {
                return m_markedForDeletion;
            }
            
            void DeleteFromDB();
            
            // TODO: make private and add accessors
        public:
            Impl* m_pimpl;

            /**
             * Database attributes go here. Order of the attributes defines the order of displying them.
             */
            Attributes m_attributes;
            std::vector<FeatureAttachment*> m_attachments;
            
            FeatureAttachment& CreateNewAttachment(std::string description, std::string mimeType, const void *data, std::size_t size);
            FeatureAttachment& CreateNewMetadataAttachment(std::string exportName, double timeStamp, std::string description,std::string mimeType);

        private:
            mutable MapText::Dtp::Point m_lowerLeft;
            mutable MapText::Dtp::Point m_upperRight;
            mutable float m_minScale;
            mutable float m_maxScale;
            Feature* m_featureRef;
            bool m_overideAttributeReadOnly;
            bool m_markedForDeletion;
            
            void _SetEdited(bool geometry, short edited, Error& error);
            
            short _GetEdited( bool geometry, Error& error );
        };
        
        typedef std::map<std::string, std::vector<FeatureRecord*> > FeatureRecordMap;
        typedef std::vector<std::pair<FeatureRecord*, bool> > FeatureRecordPairs;
        typedef std::set<std::string> FeatureRecordIdSet;

        class FeatureRecords : public std::vector<FeatureRecord*>
        {
        public:
            FeatureRecords(bool ownsElements = false) : m_ownsElements(ownsElements)
            {
                
            }
            
            ~FeatureRecords()
            {
                if(m_ownsElements)
                {
                    for(std::vector<FeatureRecord*>::iterator element = begin();element != end();element++)
                    {
                        delete *element;
                    }
                }
            }
            
            void SetOwnsElements(bool val)
            {
                m_ownsElements = val;
            }
            
            void push_back(FeatureRecord* record)
            {
                m_featureRecordMap[record->m_recordId] = record;
                std::vector<FeatureRecord*>::push_back(record);
            }

            void pop_back()
            {
                FeatureRecord* lastRecord = this->back();
                m_featureRecordMap.erase(m_featureRecordMap.find(lastRecord->m_recordId));
                std::vector<FeatureRecord*>::pop_back();
            }

            void clear()
            {
                if(m_ownsElements)
                {
                    for(std::vector<FeatureRecord*>::iterator element = begin();element != end();element++)
                    {
                        delete *element;
                    }
                }

                m_featureRecordMap.clear();
                std::vector<FeatureRecord*>::clear();
            }
            
            bool Contains(FeatureRecord* record)
            {
                return m_featureRecordMap.find(record->m_recordId) != m_featureRecordMap.end();
            }
            
        private:
            bool m_ownsElements;
            std::map<std::string, FeatureRecord* > m_featureRecordMap;
        };

        class LayerFeatureGroup
        {
        public:
            LayerFeatureGroup(const char* layerName) : m_layerName(layerName), m_includedInReport(true)
            {
            }
            
            ~LayerFeatureGroup()
            {
                for(int i=0;i<(int)m_featureRecords.size();i++)
                {
                    delete m_featureRecords[i];
                }
            }
            
            FeatureRecords* GetFeatureRecords()
            {
                return &m_featureRecords;
            }
            
            FeatureRecordPairs* GetFeatureRecordPairs()
            {
                return &m_featureRecordPairs;
            }
            
            void AddFeatureRecord(FeatureRecord* featureRecord, bool included = true);
            
            void RemoveFeatureRecordsNotInSet(FeatureRecordIdSet& featureRecordIdSet);
            
            void SetIncluded(bool val)
            {
                m_includedInReport = val;
            }
            
            bool GetIncluded()
            {
                return m_includedInReport;
            }
            
            std::string& GetLayerName()
            {
                return m_layerName;
            }
            
        private:
            std::string m_layerName;
            bool m_includedInReport;
            FeatureRecordPairs m_featureRecordPairs;
            FeatureRecords m_featureRecords;
            FeatureRecordIdSet m_featureRecordIdSet;
        };
    } // namespace Dtp
} // namespace MapText

#endif // FEATURERECORD_H
