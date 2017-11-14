//
//  MapSchema.h
//  MappingSdk
//
//  Created by Marcin Fraczak on 6/22/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef MappingSdk_MapSchema_h
#define MappingSdk_MapSchema_h

#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include "Error.h"
#include "FeatureSymbolDefinition.h"

#define REDLINESGROUPNAME "Redlines"
#define OTHERDATALAYERSGROUPNAME "Other Data Layers"

#ifndef DTP_TARGET_PLATFORM_WINDOWS
namespace json11
{
    class Json;
}
#endif

namespace MapText
{
    namespace Dtp
    {
        /**
         * Forward declare.
         */
        class MapSchema;
        class OrderElements;
        
        /**
         * SchemaLayer has to be provided with an implementation of the serializer to be able
         * to setialize/deserialize itself to/from a JSON string. This interface will be probably
         * implemented in a platform specific manner.
         *
         */
        class IMapSchemaJSONSerializer
        {
        public:
            
            virtual ~IMapSchemaJSONSerializer(){};
            
            /**
             Deserialize a schema from a JSON string provided.
             @param json - UTF-8 input JSON string.
             @param schema[out] - The schema instance to be populated with the parsed JSON content.
             Caution: previous schema content will be cleared.
             */
            virtual void DeserializeSchema( const char* json, MapSchema& schema, MapText::Dtp::Error& error ) = 0;
            
        };

        /**
         * Text style defines parameters for displaying a single attribute value
         */
        class SchemaTextStyle
        {
        public:
            
            SchemaTextStyle() : m_italic( false ), m_bold( false ), m_underline( false ), m_size( 8 )
            {}
            
            // TODO: make private and add accessors
            std::string m_fontName;
            float m_size;
            bool m_bold;
            bool m_italic;
            bool m_underline;
        };
        
        typedef std::map<std::string, SchemaTextStyle*> SchemaTextStyles;
        
        /**
         * Highlight style defines parameters for rendering highlighted feature
         */
        class HighlightStyle
        {
        public:
            enum FeatureType
            {
                FeatureType_Area=1,
                FeatureType_Line=2,
                FeatureType_Point=3
            };
            
            HighlightStyle() : m_featureType(FeatureType_Line), m_color(0), m_opacity(1), 
            m_haloSize(0), m_lineWidth(0),m_belowMapObjects(false), m_virtualFeatureStyleDefined(false),
            m_virtualFeatureColor(0), m_virtualLabelTextStyleRef(NULL)
            {}
            
            ~HighlightStyle(){}
            
            FeatureType GetFeatureType()
            {
                return m_featureType;
            }
            
            void SetFeatureType(FeatureType type)
            {
                m_featureType = type;
            }
            
            long GetColor()
            {
                return m_color;
            }
            
            void SetColor(long color)
            {
                m_color = color;
            }
            
            float GetHaloSize()
            {
                return m_haloSize;
            }
            
            void SetHaloSize(float haloSize)
            {
                m_haloSize = haloSize;                
            }
            
            long GetOpacity();
            
            void SetOpacity(float opacity);
            
            float GetLineWidth()
            {
                return m_lineWidth;
            }
            
            void SetLineWidth(float lineWidth)
            {
                m_lineWidth = lineWidth;
            }
            
            bool GetBelowMapObjects()
            {
                return m_belowMapObjects;
            }
            
            void SetBelowMapObjects(bool belowMapObjects)
            {
                m_belowMapObjects = belowMapObjects;
            }
            
            /**
             * If the virtual feature style is defined the feature may be symboilized
             * /highlighted un the map even if the geometry is not found in the 
             * current map.
             * @return True if the style is defined. In this case you can expect
             *         the VirtualFeatureColor, VirtualPointSymbolFont(Size, Glyph) to be defined.
             */
            bool GetIsVirtualFeatureStyleDefined()
            {
                return m_virtualFeatureStyleDefined;
            }
            
            void SetIsVirtualFeatureStyleDefined( bool value )
            {
                m_virtualFeatureStyleDefined = value;
            }
            
            /**
             * Color with which the virtual feature should be drawn.
             */
            long GetVirtualFeatureColor()
            {
                return m_virtualFeatureColor;
            }
            
            void SetVirtualFeatureColor(long value)
            {
                m_virtualFeatureColor = value;
            }
            
            /**
             * Style of the virtual feature label
             */
            SchemaTextStyle* GetVirtualLabelTextStyle()
            {
                return m_virtualLabelTextStyleRef;
            }
            
            void SetVirtuaLabelTextStyle(SchemaTextStyle* value)
            {
                m_virtualLabelTextStyleRef = value;
            }
            
            /**
             * Text expression for building the virtual label text.
             * The string may contain several keywords, which should be replaced
             * by the proper content:
             *  - [@lat], [@long] - latitude or longitude of the feature; only 
             *    available formatting for now is [N/S]99.99 and [E/W]999.99
             *  - [FIELD] - contents of the field in the feature row in dmap.
             *  - "\n" - newline character will create a new label line
             * Examples: 
             *  - "RANDOM FIX\n[@lat] [@long] will produce a label like this:
             * 
             *           RANDOM FIX
             *          N74.34 W43.23
             *  - "[ERM_TITLE]" will produce a label with the value of ERM_TITLE db field
             * 
             * Caution: if the feature is purely virtual (e.g. a random fix) field names will not get resolved.
             */
            const char* GetVirtualLabelTextExpression()
            {
                return m_virtualLabelTextExpression.c_str();
            }
            
            void SetVirtualLabelTextExpression( const char* value )
            {
                m_virtualLabelTextExpression = value;
            }
            
            /**
             * Filename of the font to be used to render a point symbol feature.
             */
            const char* GetVirtualPointSymbolFont()
            {
                return m_virtualPointSymbolFont.c_str();
            }
            
            void SetVirtualPointSymbolFont( const char* value )
            {
                m_virtualPointSymbolFont = value;
            }
            
            /**
             * Number of the glyph within the font.
             */
            unsigned int GetVirtualPointSymbolGlyph()
            {
                return m_virtualPointSymbolFontGlyph;
            }
            
            void SetVirtualPointSymbolGlyph( unsigned int value )
            {
                m_virtualPointSymbolFontGlyph = value;
            }
            
            /**
             * Size of the point symbol in points
             */
            float GetVirtualPointSymbolSize()
            {
                return m_virtualPointSymbolFontSize;
            }
            
            void SetVirtualPointSymbolSize( float value )
            {
                m_virtualPointSymbolFontSize = value;
            }
            
        private:
            FeatureType m_featureType; 
            long m_color;
            float m_haloSize;
            float m_opacity;
            float m_lineWidth;
            bool m_belowMapObjects;
            bool m_virtualFeatureStyleDefined;
            long m_virtualFeatureColor;
            SchemaTextStyle* m_virtualLabelTextStyleRef;
            std::string m_virtualLabelTextExpression;
            std::string m_virtualPointSymbolFont;
            float m_virtualPointSymbolFontSize;
            unsigned int m_virtualPointSymbolFontGlyph;
            
        };

        /**
         * An element of the map schema
         */
        class MapElement
        {
        public:
            enum MapElementType
            {
                MapElementType_LayerSymbology,
                MapElementType_RedlineLayer,
                MapElementType_OnlineDataLayer,
                MapElementType_SchemaLayer = 10,
                MapElementType_SchemaLayerGroup,
                MapElementType_OnlineDataLayerGroup,
            };
            
            enum OnlineDataLayerType
            {
                OnlineDataLayerType_None,
                OnlineDataLayerType_ArcGISServer,
                OnlineDataLayerType_ArcGISTPK,
                OnlineDataLayerType_ArcGISTPKSublayer,
                OnlineDataLayerType_OpenStreetMap,
                OnlineDataLayerType_AppleImagery,
                OnlineDataLayerType_GoogleImagery,
                OnlineDataLayerType_AddonDataSource,
            };
            
            MapElement() : m_onlineDataLayerType(OnlineDataLayerType_None), m_opacity(1), m_isUnfolded(false), m_isVisible(true),
                            m_editable(false), m_locatable(true), m_isARViewActive(false)
            {
            }
            
            virtual ~MapElement()
            {}
            
            virtual MapElementType GetType() const = 0;
            // TODO: make private

            virtual const OrderElements* GetOrderedElements() const = 0;

            virtual MapElement* CloneWithoutOwningElements() = 0;

            std::string GetPrettyName() const
            {
                return m_prettyName;
            }
            
            void SetPrettyName( const char* prettyName )
            {
                m_prettyName = prettyName;
            }
            
            
            std::string GetGpsAccuracyAttributeName() const
            {
                return m_gpsAccuracyAttributeName;
            }
            
            void SetGpsAccuracyAttributeName( const char* attributeName )
            {
                m_gpsAccuracyAttributeName = attributeName;
            }
            
            std::string GetTimeStampAttributeName() const
            {
                return m_timeStampAttributeName;
            }
            
            void SetTimeStampAttributeName( const char* attributeName )
            {
                m_timeStampAttributeName = attributeName;
            }
            
            OnlineDataLayerType GetOnlineDataLayerType()
            {
                return m_onlineDataLayerType;
            }
            
            void SetOnlineDataLayerType(enum OnlineDataLayerType val)
            {
                m_onlineDataLayerType = val;
            }
            
            float GetOpacity()
            {
                return m_opacity;
            }
            
            void SetOpacity(float val)
            {
                m_opacity = val;
            }

            /*
             * Can be used to check whether the element is a group, that means whether it is an instance of SchemaLayerGroup or its subclass.
             */
            virtual bool IsGroup() const
            {
                return false;
            }

            /*
             * Returns the unique key that identifies this layer within its layer group.
             * Should be overridden in subclasses. The default implementation returns GetPrettyName().
             */
            virtual const std::string GetKey() const
            {
                return GetPrettyName();
            }

        public:
            std::string m_id;
            bool m_isVisible;
            bool m_editable;
            bool m_locatable;
            bool m_isARViewActive;
            bool m_isUnfolded;
        private:
            std::string m_prettyName;
            std::string m_gpsAccuracyAttributeName;
            std::string m_timeStampAttributeName;
            OnlineDataLayerType m_onlineDataLayerType;
            float m_opacity;
        };
        
        typedef std::map<std::string,MapElement*> GroupElements;

        class OrderElements : public std::vector<MapElement*>
        {
        public:
            OrderElements(bool ownElements = false) : m_ownElements(ownElements)
            {}

            ~OrderElements()
            {
                if(m_ownElements)
                {
                    for(std::vector<MapElement*>::iterator element = this->begin();element!=this->end();element++)
                    {
                        delete *element;
                    }
                }
            }
            
            void clear()
            {
                if(m_ownElements)
                {
                    for(std::vector<MapElement*>::iterator element = this->begin();element!=this->end();element++)
                    {
                        delete *element;
                    }
                }
                
                std::vector<MapElement*>::clear();
            }

            void SetOwnsElements(bool val)
            {
                m_ownElements = val;
            }

            OrderElements* Clone();

            void UnfoldMapElements();

        private:
            bool m_ownElements;
        };

        class PickListDescriptor
        {
        public:
            PickListDescriptor(){}
            ~PickListDescriptor(){}
            
            // TODO: make private
        public:
            std::string m_keyAttributeName;
            std::string m_valueAttruteName;
            std::string m_tableName;
            
            std::map<std::string, std::string> m_keyValueMap;
            std::vector<std::pair<std::string, std::string> > m_keyValueVector;
        };
        
        class LookUpTableDescriptor
        {
        public:
            LookUpTableDescriptor() : m_defaultPickListDescriptor(NULL)
            {}
            
            ~LookUpTableDescriptor()
            {
                delete m_defaultPickListDescriptor;
            }
            
            // TODO: make private
        public:
            std::string m_pickListSelectorAttributeName;
            
            std::map<std::string, PickListDescriptor> m_pickListDescriptors;
            PickListDescriptor* m_defaultPickListDescriptor;
        };
        
        class AttributeDescriptor
        {
        public:
            
            enum AttributeSpecialType
            {
                AttributeSpecialType_None,
                AttributeSpecialType_MinScale,
                AttributeSpecialType_MaxScale,
                AttributeSpecialType_BBoxLLX,
                AttributeSpecialType_BBoxLLY,
                AttributeSpecialType_BBoxURX,
                AttributeSpecialType_BBoxURY,
                AttributeSpecialType_Edited,
                AttributeSpecialType_GeometryEdited,
                AttributeSpecialType_Version,
            };
            
            AttributeDescriptor() : m_textStyleRef(NULL), m_shownInPreview(false),
            m_isHidden(false), m_isSearchable(false), m_isReadOnly(false), m_specialType(AttributeSpecialType_None),
            m_isIdentity(false), m_isPickList(false), m_isDateField(false)
            {}
            
            ~AttributeDescriptor()
            {
            }
            
        // TODO: make private
        public:
            std::string m_dataSourceName;
            std::string m_prettyName;
            bool m_isIdentity;
            bool m_shownInPreview;
            bool m_isSearchable;
            bool m_isHidden;
            bool m_isReadOnly;
            bool m_isPickList;
            bool m_isDateField;
            LookUpTableDescriptor m_lookUpTableDescriptor;
            SchemaTextStyle* m_textStyleRef;
            AttributeSpecialType m_specialType;
        };
        
        class AttributeDescriptors : public std::vector<AttributeDescriptor*>
        {
        public:
            AttributeDescriptors(bool ownsElements = false) : m_ownsElements(ownsElements)
            {}

            ~AttributeDescriptors()
            {
                if(m_ownsElements)
                {
                    for(AttributeDescriptors::iterator attribute = this->begin();attribute!=this->end();attribute++)
                    {
                        delete *attribute;
                    }
                }
            }

            void SetOwnsElements(bool val)
            {
                m_ownsElements = val;
            }

        private:
            bool m_ownsElements;
        };

        class AttributeValue
        {
        public:
            enum ValueType
            {
                ValueType_Unsupported,
                ValueType_Null,
                ValueType_Int32,
                ValueType_Float,
                ValueType_String,
                ValueType_Bool,
            };
            
            AttributeValue() : m_valueType(ValueType_Null)
            {
            }
            
            AttributeValue( const AttributeValue& toCopy ) : m_valueType(ValueType_Null)
            {
                *this = toCopy;
            }
            
            
            
            ~AttributeValue()
            {
                if ( m_valueType == ValueType_String )
                {
                    free(m_value.m_stringValue);
                }
            }
            
            ValueType GetValueType() const
            {
                return m_valueType;
            }
            
            void SetValueType( ValueType newValueType )
            {
                if ( m_valueType == newValueType )
                {
                    return;
                }
                
                if ( m_valueType == ValueType_String )
                {
                    free(m_value.m_stringValue);
                    m_value.m_stringValue = NULL;
                }
                
                m_valueType = newValueType;
                
            }
            
            void SetEmptyValue();
            
            const char* GetStringValue() const
            {
                return m_value.m_stringValue;
            }
            
            /**
             * Returns a best attempt at a human readable representation of
             * the content.
             * @return The string representation. Memory is managed by the class. Client
             *         should NOT deallocate it.
             */
            std::string GetStringRepresentation() const;
            
            /**
             * Ownership is taken
             */
            void SetStringValue( char* value )
            {
                SetValueType(ValueType_String);
                
                m_value.m_stringValue = value;
            }
            
            int GetInt32Value() const
            {
                return m_value.m_int32Value;
            }
            
            void SetInt32Value( int value )
            {
                SetValueType(ValueType_Int32);

                m_value.m_int32Value = value;
            }
            
            float GetFloatValue() const
            {
                return m_value.m_floatValue;
            }
            
            void SetFloatValue( float value )
            {
                SetValueType(ValueType_Float);

                m_value.m_floatValue = value;
            }

            bool GetBoolValue() const
            {
                return m_value.m_boolValue;
            }
            
            void SetBoolValue( bool value )
            {
                SetValueType(ValueType_Bool);

                m_value.m_boolValue = value;
            }
            
            bool IsNull() const
            {
                return m_valueType == ValueType_Null;
            }
            
            void SetNull()
            {
                SetValueType(ValueType_Null);
            }
            
            AttributeValue& operator=( const AttributeValue& rhs );

        private:
            
            union ValueStorage
            {
                float m_floatValue;
                int m_int32Value;
                char* m_stringValue;
                bool m_boolValue;
                
                ValueStorage() : m_floatValue(0)
                {}                
            };

            ValueType m_valueType;
            ValueStorage m_value;            
        };
        
        class Attribute
        {
        public:
            AttributeDescriptor* m_descriptorRef;
            AttributeValue m_value;
        };
        
        class Attributes : public std::vector<Attribute>
        {
        public:
            Attributes()
            {}
            
            ~Attributes()
            {}
            
            void push_back(Attribute& att)
            {
                std::vector<Attribute>::push_back(att);
                m_attributeMap[att.m_descriptorRef->m_dataSourceName] = (int)this->size()-1;
            }
            
            std::map<std::string, std::string> GetFeatureValueMap()
            {
                std::map<std::string, std::string> feaureValueMap;
                
                for(Attributes::iterator attribute = this->begin();attribute!=this->end();attribute++)
                {
                    feaureValueMap[attribute->m_descriptorRef->m_dataSourceName] = attribute->m_value.GetStringRepresentation();
                }
                
                return feaureValueMap;
            }
            
            Attribute* FindAttribute(const char* name)
            {
                if(m_attributeMap.find(name) != m_attributeMap.end())
                {
                    return &((*this)[m_attributeMap[name]]);
                }

                return NULL;
            }
            
            void CopyAttributes(const Attributes& attributes)
            {
                for(Attributes::const_iterator attribute = attributes.begin();attribute!=attributes.end();attribute++)
                {
                    Attribute att = *attribute;
                    push_back(att);
                }
            }
            
        private:
            std::map<std::string, int> m_attributeMap;
        };
        
        class LayerSymbolizerDescriptor
        {
        public:
            enum LayerSymbolizerType
            {
                LayerSymbolizerType_None,
                LayerSymbolizerType_UniqueValue,
            };
            
            LayerSymbolizerDescriptor(){};

            ~LayerSymbolizerDescriptor(){};
            
            LayerSymbolizerType m_type;
            std::string m_name;
        };

        class SchemaLayerSymbologyElement : public MapElement
        {
        public:
            SchemaLayerSymbologyElement(SymbologySet* symbologySet) : MapElement(), m_symbologySet(symbologySet)
            {
                if(m_symbologySet->GetLabel()->GetString() == NULL)
                {
                    SetPrettyName(m_symbologySet->GetSymbologySetID()->GetString());
                }
                else
                {
                    SetPrettyName(m_symbologySet->GetLabel()->GetString());
                }
            }
            
            ~SchemaLayerSymbologyElement() {}
            
            virtual MapElementType GetType() const
            {
                return MapElement::MapElementType_LayerSymbology;
            }

            virtual const OrderElements* GetOrderedElements() const
            {
                return  NULL;
            }
            
            virtual MapElement* CloneWithoutOwningElements()
            {
                SchemaLayerSymbologyElement* element = new SchemaLayerSymbologyElement(m_symbologySet);
                return element;
            }

            SymbologySet* GetSymbologySet()
            {
                return m_symbologySet;
            }
            
        private:
            SymbologySet* m_symbologySet;
        };
        
        class SchemaLayer : public MapElement
        {
        public:
            SchemaLayer() : MapElement(), m_identifiable(false), m_searchable(false), m_layerEditable(false),
                                m_order(0), m_idAttributeDescriptor(NULL), m_layerSymbolizerDescriptor(NULL)
            {
                m_attributeDescriptors.SetOwnsElements(true);
                m_layerSymbologyElements.SetOwnsElements(true);
            }
            
            ~SchemaLayer()
            {
            }

            class FeatureClass
            {
            public:
                FeatureClass()
                {}
                
                FeatureClass(const char* name) : m_name(name)
                {}
                
                ~FeatureClass() {}
                
                class LabelClass
                {
                public:
                    LabelClass()
                    {}

                    LabelClass(const char* name) : m_name(name), m_fontSize(0), m_color(0)
                    {}

                    LabelClass(const char* name, const char* textExpression,
                               const char* fontName, float fontSize, long color) : m_name(name), m_textExpression(textExpression),
                    m_fontName(fontName), m_fontSize(fontSize), m_color(color)
                    {}
                    
                    ~LabelClass()
                    {}
                    
                    const char* GetName()
                    {
                        return m_name.c_str();
                    }
                    
                    const char* GetTextExpression()
                    {
                        return m_textExpression.c_str();
                    }
                    
                    void SetTextExpression(const char* textExpression)
                    {
                        m_textExpression = textExpression;
                    }
                    
                    const char* GetFontName()
                    {
                        return m_fontName.c_str();
                    }
                    
                    void SetFontName(const char* fontName)
                    {
                        m_fontName = fontName;
                    }
                    
                    float GetFontSize()
                    {
                        return m_fontSize;
                    }
                    
                    void SetFontSize(float fontSize)
                    {
                        m_fontSize = fontSize;
                    }
                    
                    long GetFontColor()
                    {
                        return m_color;
                    }
                    
                    void SetFontColor(long fontColor)
                    {
                        m_color = fontColor;
                    }
                    
                private:
                    std::string m_name;
                    std::string m_textExpression;
                    std::string m_fontName;
                    float m_fontSize;
                    long m_color;
                };
                
                const char* GetName()
                {
                    return m_name.c_str();
                }
                
                std::map<std::string, LabelClass>& GetLabelClasses()
                {
                    return m_labelClasses;
                }
                
            private:
                std::string m_name;
                std::map<std::string, LabelClass> m_labelClasses;
            };
            
            virtual MapElementType GetType() const
            {
                return MapElement::MapElementType_SchemaLayer;
            }

            virtual const OrderElements* GetOrderedElements() const
            {
                return  &m_layerSymbologyElements;
            }
            
            virtual MapElement* CloneWithoutOwningElements()
            {
                SchemaLayer* element = new SchemaLayer();
                *element = *this;
                element->m_isUnfolded = false;
                element->m_attributeDescriptors.SetOwnsElements(false);
                element->m_layerSymbologyElements.SetOwnsElements(false);
                return element;
            }

            virtual const std::string GetKey() const
            {
                return m_id;
            }
            
            std::map<std::string, FeatureClass>& GetFeatureClasses()
            {
                return m_featureClasses;
            }
            
            FeatureClass* FindFeatureClass(const char* name)
            {
                if(m_featureClasses.find(name) != m_featureClasses.end())
                {
                    return &m_featureClasses[name];
                }
                
                return NULL;
            }
            
            void SetDefaultFeatureClass(FeatureClass& fclass)
            {
                m_defaultFeatureClass = fclass;
            }

            FeatureClass* GetDefaultFeatureClass()
            {
                return &m_defaultFeatureClass;
            }

            void GetEditedAttributeDesriptors(AttributeDescriptor *&editedAttribute, AttributeDescriptor *&geometryEditedAttribute)
            {
                editedAttribute = NULL;
                geometryEditedAttribute = NULL;

                for ( AttributeDescriptors::iterator att = m_attributeDescriptors.begin(); att != m_attributeDescriptors.end(); att++ )
                {
                    if ( (*att)->m_specialType == AttributeDescriptor::AttributeSpecialType_Edited )
                    {
                        editedAttribute = (*att);
                    }
                    if ( (*att)->m_specialType == AttributeDescriptor::AttributeSpecialType_GeometryEdited )
                    {
                        geometryEditedAttribute = (*att);
                    }
                    if (editedAttribute && geometryEditedAttribute) {
                        break; // both found
                    }
                }
            }

            // TODO: make private

        public:
            std::string m_dataSourceName;
            bool m_identifiable;
            bool m_searchable;
            bool m_layerEditable;
            int m_order;
            // user-enabled/disabled search flag
            bool m_isUserSearchable;
            // searchable flag inherited from LayerGroup
            bool m_isParentUserSearchable;
            // Shortcut, a reference only, no ownership.
            AttributeDescriptor* m_idAttributeDescriptor;
            AttributeDescriptors m_attributeDescriptors;
            LayerSymbolizerDescriptor* m_layerSymbolizerDescriptor;
            std::vector<std::string> m_forms;
            OrderElements m_layerSymbologyElements;
            
        private:
            std::map<std::string, FeatureClass> m_featureClasses;
            FeatureClass m_defaultFeatureClass;
        };
        
        class RedlineLayer : public MapElement
        {
        public:
            RedlineLayer() : MapElement()
            {
            }
            
            ~RedlineLayer()
            {
            }
            
            virtual MapElementType GetType() const
            {
                return MapElement::MapElementType_RedlineLayer;
            }
            
            virtual const OrderElements* GetOrderedElements() const
            {
                return NULL;
            }
            
            virtual MapElement* CloneWithoutOwningElements()
            {
                RedlineLayer* layer = new RedlineLayer();
                *layer = *this;
                return layer;
            }

            virtual const std::string GetKey() const
            {
                return m_dataSourceName;
            }

        public:
            std::string m_dataSourceName;
            std::string m_redlineDataSourcePath;
        };
        
        class SchemaLayerGroup : public MapElement
        {
        public:
            SchemaLayerGroup() : MapElement()
            {
                this->m_isUserSearchable = true;
            }
            
            virtual ~SchemaLayerGroup()
            {
                ClearElements();
            }
            
            virtual void ClearElements()
            {
                for ( GroupElements::iterator mapElement = m_elements.begin();
                     mapElement != m_elements.end(); mapElement++ )
                {
                    delete mapElement->second;
                }                

                m_elements.clear();
                m_elementsOrder.clear();
            }
            
            void SetLayerGroupHidden();
            void SetLayerGroupUnsearchable();
            void ResetLayerGroupHiddenStatus();
            void ResetLayerGroupSearchableStatus();
            void ResetLayerGroupEditableStatus();
            void ResetLayerGroupLocatableStatus();
            void SetLayerGroupNotLocatable();
            void ResetLayerGroupUnfoldedStatus();


            virtual MapElementType GetType() const
            {
                return MapElement::MapElementType_SchemaLayerGroup;
            }

            virtual const OrderElements* GetOrderedElements() const
            {
                return &m_elementsOrder;
            }
            
            virtual MapElement* CloneWithoutOwningElements()
            {
                SchemaLayerGroup* layerGroup = new SchemaLayerGroup();
                *layerGroup = *this;
                return layerGroup;
            }

            virtual bool IsGroup() const
            {
                return true;
            }

            /**
             *  Adds a layer element as a child of this group.
             *  @param layer the layer to add
             */
            void AddLayer(MapElement *layer);
            
            /**
             *  Searches for a layer by its key.
             *  The search is one level deep (searches on top level and in top level groups).
             *  @param layerKey layer key (see GetKey())
             *  @return found layer or NULL if not found
             */
            MapElement *FindLayer(std::string layerKey) const;
            
            /**
             *  Removes a layer element from this group.
             *  It works one level deep (searches on top level and in top level groups).
             *  @param layer the layer to remove.
             */
            void RemoveLayer(MapElement *layer);
            
            /**
             *  Checks whether any layer in this group or its child groups is visible
             *  @return true if at least one layer is visible
             */
            bool IsAnyLayerVisible() const;
            
            /**
             *  Returns the number of layer elements in this group.
             *  @return the group size
             */
            unsigned long GetElementsCount() const;

            virtual const std::string GetKey() const
            {
                return m_id;
            }

            void pruneNonARElements();

#ifndef DTP_TARGET_PLATFORM_WINDOWS
            json11::Json SerializeStateToJSON();
#endif
        // TODO: make private
        public:
            bool m_isUserSearchable;

        protected:
            OrderElements m_elementsOrder;
            GroupElements m_elements;
        };
        
        class OnlineDataLayerGroup : public SchemaLayerGroup
        {
        public:
            OnlineDataLayerGroup() : SchemaLayerGroup(), m_topLevelOnlineDataLayerGroup(NULL), m_mainGroup(false)
            {
                this->m_isUserSearchable = false;
            }
            
            ~OnlineDataLayerGroup()
            {
            }
            
            virtual MapElementType GetType() const
            {
                return MapElement::MapElementType_OnlineDataLayerGroup;
            }

        public:
            int m_layerId;
            bool m_mainGroup;
            OnlineDataLayerGroup* m_topLevelOnlineDataLayerGroup;

        };
        
        class OnlineDataLayer : public MapElement
        {
        public:
            
            OnlineDataLayer() : MapElement(), m_topLevelOnlineDataLayerGroup(NULL), m_visibleAllowed(true)
            {
            }
            
            ~OnlineDataLayer()
            {
            }
            
            virtual MapElementType GetType() const
            {
                return MapElement::MapElementType_OnlineDataLayer;
            }
            
            virtual const OrderElements* GetOrderedElements() const
            {
                return NULL;
            }

            virtual MapElement* CloneWithoutOwningElements()
            {
                OnlineDataLayer* layer = new OnlineDataLayer();
                *layer = *this;
                return layer;
            }

        public:
            bool m_visibleAllowed;
            int m_layerId;
            int m_parentLayerID;
            OnlineDataLayerGroup* m_topLevelOnlineDataLayerGroup;
        };


        class AddonDataSourceLayer : public OnlineDataLayer {
        public:
            virtual const std::string GetKey() const
            {
                return m_id;
            }
        };

        class MapSchema : public SchemaLayerGroup
        {
        public:
            MapSchema() : SchemaLayerGroup()
            {
                m_isUnfolded = true;
            }
            
            ~MapSchema()
            {
                ClearElements();
            }
            
            class GeometryNetwork
            {
            public:
                GeometryNetwork()
                {}
                
                ~GeometryNetwork()
                {}
                
                class TracingTool
                {
                    friend class GeometryNetwork;
                    
                public:
                    TracingTool(){}
                    
                    ~TracingTool(){}
                    
                    class ConfigurationOptions
                    {
                        friend class TracingTool;
                        
                    public:
                        ConfigurationOptions() {}
                        
                        ~ConfigurationOptions() {}
                        
                        void AddDisablingLayer(std::string layerName)
                        {
                            m_disablingLayers.insert(layerName);
                        }
                        
                        std::set<std::string>& GetDisablingLayers()
                        {
                            return m_disablingLayers;
                        }
                        
                    private:
                        std::set<std::string> m_disablingLayers;
                    };
                    
                    void SetToolName(const char* toolName)
                    {
                        m_toolName = toolName;
                    }
                    
                    const std::string& GetName()
                    {
                        return m_toolName;
                    }
                    
                    ConfigurationOptions& GetOptions()
                    {
                        return m_options;
                    }
                    
                private:
                    std::string m_toolName;
                    ConfigurationOptions m_options;
                };
                
                std::set<std::string>& GetNetworkLayers()
                {
                    return m_networkLayers;
                }
                
                void AddTracingTool(TracingTool& tool)
                {
                    m_tracingTools.push_back(tool);
                }
                
                std::vector<TracingTool>& GetTracingTools()
                {
                    return m_tracingTools;
                }
                
            private:
                std::set<std::string> m_networkLayers;
                std::vector<TracingTool> m_tracingTools;
            };
            
            virtual void ClearElements()
            {
                SchemaLayerGroup::ClearElements();
                m_layerNameToLayerRefMapping.clear();
            }

            void BuildLayerNameToLayerRefMapping();
            
            void SetActiveEditableLayer(const char* layerName);
            
            MapElement* GetActiveEditableLayer();
                        
            // TODO: make private

            /**
             * Get a platform-specific implementation of the schema serializer.
             * @return - An instance of the serializer implementation. Client code is
             *           responsible for disposing of the object.
             */
            static IMapSchemaJSONSerializer* GetPlatformSpecificSerializer();
            
            void SetName(const char* name)
            {
                m_name = name;
                SetPrettyName(name);
            }
            
            const char* GetName()
            {
                return m_name.c_str();
            }
            
            MapElement* FindSchemaLayer(const char* layerName)
            {
                if(m_layerNameToLayerRefMapping.find(layerName) != m_layerNameToLayerRefMapping.end())
                {
                    return (MapElement*)m_layerNameToLayerRefMapping[layerName];
                }
                
                return NULL;
            }
            
            GeometryNetwork& GetGeometryNetwork()
            {
                return m_geometryNetwork;
            }
            
            void AddGeometryNetworkLayer(const char* layerName)
            {
                if(FindSchemaLayer(layerName))
                {
                    m_geometryNetwork.GetNetworkLayers().insert(layerName);
                }
            }
            
            bool ARAvailable();
            
            OrderElements GetEditableLayers();
            
            std::string SerializeStateToJSON();
            
            void DeserializeJSONState(const char* jsonString);
            
        private:
            std::string m_name;
            GeometryNetwork m_geometryNetwork;
            
            bool IsARAvailable(const OrderElements& elements);
            OrderElements EditableLayers(const OrderElements& elements);
#ifndef DTP_TARGET_PLATFORM_WINDOWS
            void ProcessJSONLayerGroup(const json11::Json& layerGroup);
#endif
        public:
            std::map<std::string, MapElement*> m_layerNameToLayerRefMapping;
            std::map<std::string, std::vector<SchemaLayerGroup*> > m_layerGroupNameGroupingMapping;
            std::map<std::string, SchemaLayerGroup* > m_layerGroupIdlayerGroupRefMapping;
            void AddMapElementsToMapping(const OrderElements &mapElements);
        };
        
        class MapCompositionTOC
        {
        public:
            MapCompositionTOC() : m_appleImageryLayer(NULL)
            {
                m_redlineLayers.SetPrettyName(REDLINESGROUPNAME);
                m_redlineLayers.m_isUnfolded = true;
                m_onlineDataLayers.SetPrettyName(OTHERDATALAYERSGROUPNAME);
                m_onlineDataLayers.m_isUnfolded = true;
                m_onlineDataLayers.m_mainGroup = true;
            }
            
            ~MapCompositionTOC()
            {
                Clear();
            }
            
            void Clear()
            {
                for(int i=0;i<(int)m_mapSchemas.size();i++)
                {
                    delete m_mapSchemas[i];
                }
                
                m_mapSchemas.clear();
                                
                m_onlineDataLayers.ClearElements();
                
                m_redlineLayers.ClearElements();
            }
            
            void Reset()
            {
                Clear();
                m_redlineLayers.m_isUnfolded = true;
            }
            
            void AddMapSchema(MapSchema* mapSchema)
            {
                m_mapSchemas.push_back(mapSchema);
            }
            
            std::vector<MapSchema*>* GetMapSchemas()
            {
                return &m_mapSchemas;
            }

            void AddRedlineLayer(RedlineLayer* redlineLayer)
            {
                m_redlineLayers.AddLayer(redlineLayer);
            }
            
            SchemaLayerGroup* GetRedlineLayers()
            {
                return &m_redlineLayers;
            }

            void AddOnlineDataSourceLayer(MapElement* onlineDataLayer)
            {
                m_onlineDataLayers.AddLayer(onlineDataLayer);
            }
            
            OnlineDataLayerGroup* GetOnlineDataLayers()
            {
                return &m_onlineDataLayers;
            }

            void AddAddonDataSourceLayer(MapElement*layer)
            {
                m_addonDataSourcesLayers.AddLayer(layer);
            }

            OnlineDataLayerGroup* GetAddonDataSourcesLayers()
            {
                return &m_addonDataSourcesLayers;
            }

            void RemoveAddonDataSourceLayer(MapElement*layer)
            {
                m_addonDataSourcesLayers.RemoveLayer(layer);
            }

            MapElement*FindFeatureLayer(std::string layerKey)
            {
                for(int i=0;i<(int)m_mapSchemas.size();i++)
                {
                    if(m_mapSchemas[i]->m_layerNameToLayerRefMapping.find(layerKey) != m_mapSchemas[i]->m_layerNameToLayerRefMapping.end())
                    {
                        return m_mapSchemas[i]->m_layerNameToLayerRefMapping[layerKey];
                    }
                }
                
                MapElement* element = FindRedLineLayer(layerKey);

                return element;
            }
            
            RedlineLayer* FindRedLineLayer(std::string layerKey)
            {
                return (RedlineLayer *) m_redlineLayers.FindLayer(layerKey);
            }
            
            void RemoveRedlineLayer(RedlineLayer* redlineLayer)
            {
                m_redlineLayers.RemoveLayer(redlineLayer);
            }

            OnlineDataLayer* FindOnlineDataLayerElement(std::string layerKey)
            {
                return (OnlineDataLayer *) m_onlineDataLayers.FindLayer(layerKey);
            }

            MapElement*FindAddonDataSourcesLayer(std::string layerKey)
            {
                return m_addonDataSourcesLayers.FindLayer(layerKey);
            }
            
            bool IsAnyOnlineDataLayerVisible()
            {
                return m_onlineDataLayers.IsAnyLayerVisible();
            }
            
            void RemoveOnlineDataLayerElement(MapElement* element)
            {
                m_onlineDataLayers.RemoveLayer(element);
            }

            void SetActiveEditableLayer(const char* layerName)
            {
                for(int i=0;i<(int)m_mapSchemas.size();i++)
                {
                    m_mapSchemas[i]->SetActiveEditableLayer(layerName);
                }
            }

            MapElement* GetActiveEditableLayer()
            {
                MapElement* activeLayer;
                
                for(int i=0;i<(int)m_mapSchemas.size();i++)
                {
                    activeLayer = m_mapSchemas[i]->GetActiveEditableLayer();
                    if(activeLayer)
                    {
                        return activeLayer;
                    }
                }
                
                return NULL;
            }
            
            void SetAppleImageryOnlineDataLayer(OnlineDataLayer* appleImageryLayer)
            {
                m_appleImageryLayer = appleImageryLayer;
            }
            
            OnlineDataLayer* GetAppleImageryOnlineLayer()
            {
                return m_appleImageryLayer;
            }
            
        private:

            std::vector<MapSchema*> m_mapSchemas;
            SchemaLayerGroup m_redlineLayers;
            OnlineDataLayerGroup m_onlineDataLayers;
            OnlineDataLayer* m_appleImageryLayer;
            OnlineDataLayerGroup m_addonDataSourcesLayers;
        };
        
    }
}

#endif
