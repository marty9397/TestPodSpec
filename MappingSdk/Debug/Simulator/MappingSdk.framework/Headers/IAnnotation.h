//
//  IAnnotation.h
//  MappingSdk
//
//  Created by Mark on 4/23/13.
//
//

#ifndef __MappingSdk__IAnnotation__
#define __MappingSdk__IAnnotation__

#include "LabelingEngineDefs.h"

#include "Collection.h"
#include "Label.h"
#include "MtGeometry.h"
#include "PlacementRule.h"
#include "FeatureSymbolDefinition.h"

namespace MapText
{
	namespace Dtp
	{
        /**
         * This is the main interface for creating and persisting annotion in DTP
         * @reqid ANNOTATION
         */
        class LABELINGENGINE_API IAnnotation
        {
        public:
            /**
             * Error codes.
             * @reqid ANNOTATION
             */
            enum ErrorCode
            {
                ErrorCode_AllocationFailure = 1,
            };

            /**
             * Possible annotation types
             * @reqid ANNOTATION
             */
            enum AnnotationType
            {
                AnnotationType_None = 0,
                AnnotationType_Group,
                AnnotationType_ShapeItem,
                AnnotationType_LabelItem,
            };

            IAnnotation(AnnotationType type) : m_annotationType(type), m_visible(true)
            {
            }
            
            /**
             * Constructor
             * @param error		an error object to be populated
             * @reqid ANNOTATION
             */
            IAnnotation(AnnotationType type, const char* name, const char* UUID, Error& error): m_annotationType(type),
                m_visible(true)
            {
                m_UUID.AssignString(UUID, error);
                if(name)
                {
                    m_name.AssignString(name, error);
                }
            }
            
            virtual ~IAnnotation(){};
            
            void SetVisible(bool val)
            {
                m_visible = val;
            }
            
            bool GetVisible()
            {
                return m_visible;
            }
            
            /**
             * This UDID may be used identify annotation which originated from a particular data source.
             * Data source implementations should use their UUIDs to tag the annotation.
             * @reqid ANNOTATION
             */
            const char* GetDataSourceUUID()
            {
                return m_dataSourceUUID;
            }
            
            /**
             * Assign the UUID of the data source to the annotation. Ownership is NOT taken, the caller
             * has to make sure this string lives at least as long as the annotation.
             * @reqid ANNOTATION
             */
            void SetDataSourceUUID( const char* uuid )
            {
                m_dataSourceUUID = uuid;
            }
            
            /**
             * Get the name for this annotation.
             * it may be necessary to name the annotation espcecially in the case of an AnnotationGroup
             * @reqid ANNOTATION
             */
            SharedString* GetName()
            {
                return &m_name;
            }
            
            /**
             * Set the name for this annotation.
             * @reqid ANNOTATION
             */
            void SetName(const char* name, Error& error)
            {
                m_name.AssignString(name, error);
            }

            /**
             * Get the type of the annotation of this item
             * @reqid ANNOTATION
             */
            AnnotationType GetType()
            {
                return m_annotationType;
            }
            
            /**
             * Get the UUID for this annotation.
             * @reqid ANNOTATION
             */
            const char* GetUUID()
            {
                return m_UUID.GetString();
            }
            
            /**
             * Set the UUID for this annotation.
             * @reqid ANNOTATION
             */
            void SetUUID(const char* UUID, Error& error)
            {
                m_UUID.AssignString(UUID, error);
            }
            
            /**
             * Creates copy of self.
             * @param error an error object to be populated
             * @return copy of self
             * @reqid ANNOTATION
             */
            virtual IAnnotation* Clone( MapText::Dtp::Error& error ) const = 0;

            /**
             * Get the bounding box of this annotation
             * @reqid ANNOTATION
             */
            virtual UprightRectangle GetBoundingBox( ) = 0;

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
            static IAnnotation* DeserializeNew( IDeserializer& deserializer, Error& error );
            
        protected:
            /**
             * Child specific serialization. Needs to be implemented by all
             * deriving classes.
             * @param emitter - the emitter to serialize to.
             * @param error - error info.
             * @reqid N/A
             */
            virtual void _Serialize( ISerializer& emitter, Error& error ) const = 0;
            
            /** 
             * Deserializes the object from a deserializer.
             * @param deserializer - Source for deserialization.
             * @pmtgeoaram error - Error info.
             */
            virtual void _Deserialize( IDeserializer& deserializer, Error& error ) = 0;
#endif
            
        private:
            AnnotationType m_annotationType;
            bool m_visible;
            
        protected:
            const char* m_dataSourceUUID;
            SharedString m_name;
            String m_UUID;
        };

        /**
         * Class that holds an array of IAnnotation objects.
         * @reqid ANNOTATION
         */
        class LABELINGENGINE_API Annotations
        : public ClonableLinkedList < IAnnotation, Annotations >
        {
        public:
            /**
             * Default constructor implemented to avoid compiler generated object code.
             * @reqid ANNOTATION
             */
            Annotations();
            
            /**
             * Default destructor implemented to avoid compiler generated object code.
             * @reqid ANNOTATION
             */
            ~Annotations();
            
            /**
             * Find annotation in the collection using its ID. This will only find top level annotations
             * and not drill down into AnnotationGroup's
             * This implementation has an avg O(1) complexity (hash table lookup).
             * @param UUID - ID of the annotation to find. Passing NULL is safe and
             *                    always returns an empty iterator.
             * @return - Iterator pointing to the annotation requested or returning
             *           true from the IsNull() method if it was not found.
             * @reqid ANNOTATION
             */
            Iterator FindById( const TCHAR* UUID );
            
            /**
             * Overriding the default implementation to maintain the quick-access index.
             * @reqid ANNOTATION
             */
            virtual void AddElement( IAnnotation* element, Error& error );
            
            /**
             * Overriding the default implementation to maintain the quick-access index.
             * @reqid ANNOTATION
             */
            virtual void Clear();
            
            void RemoveAnnotation(IAnnotation* annotation);
            
            /**
             * Get the bounding box of the collection of annotations
             * @reqid ANNOTATION
             */
            UprightRectangle GetBoundingBox( );

            
        protected:
            virtual void RemoveElement( Element* e );
            
            /**
             * Hide the private parts
             */
            class LABELINGENGINE_API Impl; Impl* m_pimpl;
        };
        
        /**
         * This will contain a collection of annotations that need to be grouped
         * @reqid ANNOTATION
         */
        class LABELINGENGINE_API AnnotationGroup : public IAnnotation
        {
		public:
            AnnotationGroup() : IAnnotation(AnnotationType_Group)
            {
            }

            /**
             * Constructor
             * @param error		an error object to be populated
             * @reqid ANNOTATION
             */
            AnnotationGroup(const char* name, const char* UUID, Error& error) : IAnnotation(AnnotationType_Group, name, UUID, error)
            {
            }
            
            ~AnnotationGroup()
            {
            }
                        
            /**
             * Add a new annotation to the annotation list and take ownership
             * @param annotation - the new annotation
             * @param error an error object to be populated
             * @reqid ANNOTATION
             */
            void AddAnnotation(IAnnotation* annotation, Error& error)
            {
                m_annotations.AddElement(annotation, error);
            }
            
            /**
             * Get the annotation list
             * @return the annotation collection
             * @reqid ANNOTATION
             */
            Annotations* GetAnnotations()
            {
                return &m_annotations;
            }
            
            /**
             * Creates copy of self.
             * @param error an error object to be populated
             * @return copy of self
             * @reqid ANNOTATION
             */
            AnnotationGroup* Clone( MapText::Dtp::Error& error ) const
            {
                AnnotationGroup* clone = new AnnotationGroup(m_name.GetString(), m_UUID.GetString(), error);
            
                LinkedList< IAnnotation >::ConstIterator annotation = m_annotations.Begin();
                while(!annotation.IsNull())
                {
                    IAnnotation* copy = annotation->Clone(error);
                    if(error)
                    {
                        delete clone;
                        return NULL;
                    }
                    clone->AddAnnotation(copy, error);
                    if(error)
                    {
                        delete copy;
                        delete clone;
                        return NULL;
                    }
                    
                    annotation++;
                }
        
                return clone;
            }
            
            /**
             * Get the bounding box of this annotation
             * @reqid ANNOTATION
             */
            virtual UprightRectangle GetBoundingBox( );

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
             * @see Shape::_Deserialize
             * @reqid N/A
             */
            virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif            
            
        private:
            Annotations m_annotations;
        };
        
        /**
         * This will contain annotation geometry
         * @reqid ANNOTATION
         */
        class LABELINGENGINE_API AnnotationShapeItem : public IAnnotation
        {
        public:

            AnnotationShapeItem() : IAnnotation(AnnotationType_ShapeItem), m_shape(NULL), m_featureSymbol(NULL)
            {
            }

            /**
             * Constructor
             * @param error		an error object to be populated
             * @reqid ANNOTATION
             */
            AnnotationShapeItem(Shape* shape, FeatureSymbol* feaureSymbol, const char* name,
								const char* UUID, Error& error) : 
								IAnnotation(AnnotationType_ShapeItem, name, UUID, error), m_shape(shape),
								m_featureSymbol(feaureSymbol)
            {
            }
            
            ~AnnotationShapeItem()
            {
                delete m_shape;
				delete m_featureSymbol;
            }
            
            /**
             * Get the shape for this annotation.
             * @reqid ANNOTATION
             */
            Shape* GetShape()
            {
                return m_shape;
            }

            /**
             * Get the feature symbol for this annotation.
             * @reqid ANNOTATION
             */
            FeatureSymbol* GetFeatureSymbol()
            {
                return m_featureSymbol;
            }

            /**
             * Creates copy of self.
             * @param error an error object to be populated
             * @return copy of self
             * @reqid ANNOTATION
             */
            AnnotationShapeItem* Clone( MapText::Dtp::Error& error ) const;
            
            /**
             * Get the bounding box of this annotation
             * @param error		an error object to be populated
             * @reqid ANNOTATION
             */
            virtual UprightRectangle GetBoundingBox( )
            {
                return *m_shape->GetBoundingBox();
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
             * @see Shape::_Deserialize
             * @reqid N/A
             */
            virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
            
        private:
            Shape* m_shape;
			FeatureSymbol* m_featureSymbol;
        };

        /**
         * This will contain an annotation label
         * @reqid ANNOTATION
         */
        class LABELINGENGINE_API AnnotationLabelItem : public IAnnotation
        {
        public:
            AnnotationLabelItem() : IAnnotation(AnnotationType_LabelItem), m_label(NULL), m_referenceScale(0), m_angle(0)
            {
            }

            /**
             * Constructor
             * @param error		an error object to be populated
             * @reqid ANNOTATION
             */
            AnnotationLabelItem(Label* label, const char* name, const char* UUID, Error& error)
                : IAnnotation(AnnotationType_LabelItem, name, UUID, error), m_label(label),
                m_justification(PlacementRule::LabelJustification_Left), m_stackSpacing(0.35f)
            {
                m_referenceScale = m_label->GetReferenceScale();
                m_angle = m_label->GetAngle();
            }
            
            ~AnnotationLabelItem()
            {
                delete m_label;
            }
            
            /**
             * Get the label for this annotation.
             * @reqid ANNOTATION
             */
            Label* GetLabel()
            {
                return m_label;
            }
            
            /**
             * Get the lowerleft of the label relative to the font character cell baseline.
             * @reqid ANNOTATION
             */
            float GetFontDescenderFactor(Error& error);
            
            /**
             * The GetStackSpacing method returns the stack spacing which should be used
             * for multi-line annotations.
             * @return - The stack spacing value.
             *
             * @reqid ANNOTATION.
             */
            float GetStackSpacing() const { return m_stackSpacing; }

            /**
             * The SetStackSpacing method sets the stack spacing which should be used
             * for multi-line annotations.
             * @param val - The stack spacing value.
             *
             * @reqid ANNOTATION.
             */
            void SetStackSpacing(float val) { m_stackSpacing = val; }

            /**
             * The GetJustification method returns the justification with which a multi-line
             * annotation should be shown.
             * @return - The text justification.
             *
             * @reqid ANNOTATION.
             */
            PlacementRule::LabelJustification GetJustification() const { return m_justification; }

            /**
             * The SetJustification method sets the justification with which a multi-line
             * annotation should be shown.
             * @patam val - The text justification.
             *
             * @reqid ANNOTATION.
             */
            void SetJustification(PlacementRule::LabelJustification val) { m_justification = val; }

            void AdjustLabelStackSpacingForScaleFactor(float scaleFactor,Error& error);
            
            /**
             * Creates copy of self.
             * @param error an error object to be populated
             * @return copy of self
             * @reqid ANNOTATION
             */
            AnnotationLabelItem* Clone( Error& error ) const;

            /**
             * Get the bounding box of this annotation
             * @param error		an error object to be populated
             * @reqid ANNOTATION
             */
            virtual UprightRectangle GetBoundingBox( );
            
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
             * @see Shape::_Deserialize
             * @reqid N/A
             */
            virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif
            
        private:
            Label* m_label;
            float m_referenceScale;
            float m_angle;
            float m_stackSpacing;
            PlacementRule::LabelJustification m_justification;
        };
    }
}

#endif
