
#ifndef FEATURESYMBOLDEFINITION_H
#define FEATURESYMBOLDEFINITION_H

#include "LabelingEngineDefs.h"
#include "MtString.h"
#include "MapTextDefs.h"
#include "SharedObjects.h"
#include "Collection.h"
#include "Error.h"

namespace MapText
{
	namespace Dtp
	{
		// Forward declarations
		class PointSymbolDef;
		class LineSymbol;
		class Polygons;

		/**
		* Possible error codes
		* @reqid 001.0063
		*/
		enum ErrorCode
		{
			ErrorCode_AllocationFailure = 1,
			ErrorCode_TooSmallNumStrokePatterns,
			ErrorCode_IndexOutOfRange,
			ErrorCode_FileNotFound,
			ErrorCode_SymbolNotFound,
		};

		/**
		 * Class that encapsulates the properties of a stroke pattern including
		 * dashed line and point symbol.
		 *
		 * An array of instances of this class is held by a PatternCollection instance.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API StrokePattern
		{
		public:
		/**
		 * Default constructor.
		 * @reqid TODO
		 */
			StrokePattern();

			StrokePattern(float dashLength, float gapLength);

		/**
		 * Destructor.
		 * @reqid TODO
		 */
			~StrokePattern();

		/**
		 * Get the dash length of the stroke pattern.
		 * Units are assumed to be inches.
		 * @return the dash length
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			float GetDashLength()
			{
				return m_dashLength;
			}

		/**
		 * Set the dash length of the stroke pattern.
		 * @param dashLength - the dash length
		 * Units are assumed to be inches.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetDashLength( float dashLength )
			{
				m_dashLength = dashLength;
			}

		/**
		 * Get the gap length of the stroke pattern.
		 * Units are assumed to be inches.
		 * @return the gap length
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			float GetGapLength()
			{
				return m_gapLength;
			}

		/**
		 * Set the gap length of the stroke pattern.
		 * @param gapLength - the gap length
		 * Units are assumed to be inches.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetGapLength( float gapLength )
			{
				m_gapLength = gapLength;
			}

		/**
		 * Get the flag that determines whether to draw the dash.
		 * @return the flag that determines whether to draw the dash
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			bool GetDrawDash()
			{
				return m_drawDash;
			}

		/**
		 * Set the flag that determines whether to draw the dash.
		 * @param drawDash - the flag that determines whether to draw the dash
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetDrawDash( bool drawDash )
			{
				m_drawDash = drawDash;
			}

		/**
		 * Get the PointSymbolDef used for drawing a point symbol as part of the pattern.
		 * @return the PointSymbolDef used for drawing a point symbol as part of the pattern
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			PointSymbolDef* GetPointSymbol()
			{
				return m_pointSymbol;
			}

		/**
		 * Set the PointSymbolDef used for drawing a point symbol as part of the pattern.
		 * @param return - the PointSymbolDef to use for drawing a point symbol as part of the pattern
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetPointSymbolDef( PointSymbolDef* pointSymbol )
			{
				m_pointSymbol = pointSymbol;
			}

		/**
		 * Get the perpendicular offset from the line used for drawing the point symbol (if any) that is part of the pattern.
		 * Units are assumed to be inches.
		 * Negative ==> to the left and Positive ==> to the right.
		 * @return the offset used for drawing the point symbol
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			float GetPointSymbolOffset()
			{
				return m_pointSymbolOffset;
			}

		/**
		 * Set the perpendicular offset from the line used for drawing the point symbol (if any) that is part of the pattern.
		 * @param pointSymbolOffset - the offset to be used for drawing the point symbol
		 * Units are assumed to be inches.
		 * Negative ==> to the left and Positive ==> to the right.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetPointSymbolOffset( float pointSymbolOffset )
			{
				m_pointSymbolOffset = pointSymbolOffset;
			}

		/**
		 * Get the position of the point symbol (if any) along a dash-gap pair, relative to the start.
		 * Must be between 0 and 1 inclusive
		 * @return the relative position of the point symbol
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			float GetPointSymbolRelativePoition()
			{
				return m_pointSymbolRelativePoition;
			}

		/**
		 * Set the position of the point symbol (if any) along a dash-gap pair, relative to the start.
		 * Must be between 0 and 1 inclusive
		 * @param pointSymbolRelativePoition - the relative position of the point symbol
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetPointSymbolRelativePoition( float pointSymbolRelativePoition )
			{
				m_pointSymbolRelativePoition = pointSymbolRelativePoition;
			}


			bool operator== ( StrokePattern& strPat );

			StrokePattern* Clone(Error& error) const;

#ifdef S11N
				/**
				 * @see ISerializable::Serialize
				 * @reqid N/A.
				 */
			void Serialize( ISerializer& destination, Error& error ) const;

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
			void Deserialize( IDeserializer& deserializer, Error& error);

                /**
                 * The DeserializeNew method instantiates a new feature by deserializing it
                 * @param deserializer - The deserializer to use.
                 * @param error - Error info.
                 * @return - New feature instance or NULL on error.
                 *
                 * @reqid PERSISTENCE.
                 */
			static StrokePattern* DeserializeNew( IDeserializer& deserializer, Error& error );

#endif

		private:
			float m_dashLength;
			float m_gapLength;
			bool m_drawDash;
			PointSymbolDef* m_pointSymbol;
			float m_pointSymbolOffset;
			float m_pointSymbolRelativePoition;
		};

		class LABELINGENGINE_API StrokePatterns : public ClonableLinkedList < StrokePattern, StrokePatterns >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				StrokePatterns( bool ownsElements = true ) : ClonableLinkedList < StrokePattern, StrokePatterns >( ownsElements )
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~StrokePatterns()
				{}
		};

		/**
		 * Class that encapsulates an ordered collection of stroke pattern instances.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API StrokePatternCollection
		{
		public:
		/**
		 * Enumerated type that models the methods by which a stroke pattern collection's
		 * dash-gap patterns can be adjusted to fit the line feature being rendered.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		enum StrokePatternAdjustmentMethod
		{
			StrokePatternAdjustmentMethod_None,
			StrokePatternAdjustmentMethod_Stretch,
			StrokePatternAdjustmentMethod_Shrink,
			StrokePatternAdjustmentMethod_Aesthetic,
		};

		/**
		 * Default constructor.
		 * @reqid TODO
		 */
			StrokePatternCollection();

		/**	
		 * Destructor.
		 * @reqid TODO
		 */
			~StrokePatternCollection();

		/**
		 * Get the method for adjusting dash-gap pattern instances to fit the line feature extent.
		 * @return the StrokePatternAdjustmentMethod used for drawing the dash-gap patterns
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			StrokePatternAdjustmentMethod GetAdjustmentMethod()
			{
				return m_adjustmentMethod;
			}

		/**
		 * Set the method for adjusting dash-gap pattern instances to fit the line feature extent.
		 * @param adjustmentMethod - the StrokePatternAdjustmentMethod to use for drawing the dash-gap patterns
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetAdjustmentMethod( StrokePatternAdjustmentMethod adjustmentMethod )
			{
				m_adjustmentMethod = adjustmentMethod;
			}

		/**
		 * Get the number of stroke patterns to be drawn in z-order sequence
		 * @return the number of stroke patterns drawn
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			StrokePatterns* GetStokePatterns()
			{
				return &m_patterns;			
			}

			StrokePattern* GetStrokePattern( int i );

		/**
		 * Add a stroke pattern
		 * @param pattern - new stroke pattern
		 * @param error error information
		 * @reqid TODO
		 */
			void AddStrokePattern( StrokePattern* pattern, Error& error )
			{
				m_patterns.AddElement(pattern, error);
			}

		bool operator== ( StrokePatternCollection& strPatCol );

		StrokePatternCollection* Clone(Error& error) const;

#ifdef S11N
				/**
				 * @see ISerializable::Serialize
				 * @reqid N/A.
				 */
			void Serialize( ISerializer& destination, Error& error ) const;

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
			void Deserialize( IDeserializer& deserializer, Error& error);

                /**
                 * The DeserializeNew method instantiates a new feature by deserializing it
                 * @param deserializer - The deserializer to use.
                 * @param error - Error info.
                 * @return - New feature instance or NULL on error.
                 *
                 * @reqid PERSISTENCE.
                 */
			static StrokePatternCollection* DeserializeNew( IDeserializer& deserializer, Error& error );

#endif

		private:
			StrokePatternAdjustmentMethod m_adjustmentMethod;
			StrokePatterns m_patterns;
		};

		/**
		 * Abstract class that encapsulates a feature rendering style.
		 * An array of instances of this class is held by each FeatureSymbol instance.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API FeatureStyle
		{
		public:
			enum StyleType
			{
				StyleType_Unknown = -1,
				StyleType_Area,
				StyleType_Line,
				StyleType_Point,
				StyleType_Count
			};

			FeatureStyle(StyleType styleType) : m_styleType(styleType), m_drawOrder(0)
			{
			}

			/**
			 * Get the style type of this feature style
			 * @return the type
			 * @reqid 001.0063
			 */
			virtual StyleType GetStyleType()
			{
				return m_styleType;
			}

			/**
			 * Ensure all the destructors are virtual.
			 * @reqid 001.0063
			 */
			virtual ~FeatureStyle()
			{
			};

			/**
			 * Get the draw order of this feature style
			 * @return the draw order
			 * @reqid TODO
			 */
			int GetDrawOrder()
			{
				return m_drawOrder;
			}

			/**
			 * Set the draw order of this feature style
			 * @return the draw order
			 * @reqid TODO
			 */
			void SetDrawOrder(int drawOrder)
			{
				m_drawOrder = drawOrder;
			}

			virtual bool operator== ( FeatureStyle& style );

			virtual FeatureStyle* Clone(Error& error) const = 0;

#ifdef S11N
			/**
			 * Serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			void Serialize( ISerializer& destination, Error& error ) const;

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
			static FeatureStyle* DeserializeNew( IDeserializer& deserializer, Error& error );

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

		protected:

			static void Copy( const FeatureStyle& src, FeatureStyle& dest, Error& )
			{
				dest.m_drawOrder = src.m_drawOrder;
			}

		private:
			StyleType m_styleType;
			int m_drawOrder;
		};

		/**
		 * Class that encapsulates the properties of a line feature rendering style.
		 * An array of instances of this class is held by each FeatureSymbol instance.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API LineStyle : public FeatureStyle
		{
		public:
            enum LineCapType
            {
                LineCapType_Round = 0,
                LineCapType_Square,
                LineCapType_Butt,
            };

			LineStyle();

			LineStyle(long color, float width, float drawOffset);
                        
			LineStyle(long color, float width, float drawOffset,
							PointSymbolDef* lineStartPointSymbol,
							PointSymbolDef* lineEndPointSymbol,
                            StrokePatternCollection* strokePatternCollection);
            
			~LineStyle();

		/**
		 * Get the color (includes alpha) in which the line feature is rendered.
		 * @return the color in which the line feature is rendered
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			long GetColor()
			{
				return m_color;
			}

		/**
		 * Set the color in which the line feature is to be rendered.
		 * @param color - the color in which the line feature is to be rendered
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetColor( long color )
			{
				m_color = (int)color;
			}

		/**
		 * Get the width at which the line feature is rendered.
		 * Units are assumed to be inches.
		 * @return the width at which the line feature is rendered
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			float GetWidth()
			{
				return m_width;
			}

		/**
		 * Set the width at which the line feature is to be rendered.
		 * @param width - the width at which the line feature is to be rendered
		 * Units are assumed to be inches.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetWidth( float width )
			{
				m_width = width;
			}

            /**
             * Get the lineCap that will be rendered at the ends of the line feature.
             * @return the lineCap 
             *
             * Used for mobile client rendering purposes.
             */
            LineCapType GetLineCap()
            {
                return m_lineCap;
            }
            
            /**
             * Set the lineCap that will be rendered at the ends of the line feature
             * @param lineCap - the linecap
             * Used for mobile client rendering purposes.
             */
            void SetLineCap( LineCapType lineCap )
            {
                m_lineCap = lineCap;
            }
		/**
		 * Get the perpendicular offset from the center line at which the line feature is rendered.
		 * Units are assumed to be inches.
		 * Negative ==> to the left and Positive ==> to the right.
		 * @return the offset from the center line at which the line feature is rendered
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			float GetLineDrawOffset()
			{
				return m_lineDrawOffset;
			}

		/**
		 * Set the perpendicular offset from the center line at which the line feature is to be rendered.
		 * @param lineDrawOffset - the offset from the center line at which the line feature is to be rendered
		 * Units are assumed to be inches.
		 * Negative ==> to the left and Positive ==> to the right.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetLineDrawOffset( float lineDrawOffset )
			{
				m_lineDrawOffset = lineDrawOffset;
			}

		/**
		 * Get the point symbol (if any) to draw at the line start.
		 * @return the point symbol to draw at the line start
		 * May be NULL.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		    PointSymbolDef* GetLineStartPointSymbol()
			{
				return m_lineStartPointSymbol;
			}

		/**
		 * Set the point symbol to be drawn at the line start.
		 * @param lineStartPointSymbolDef - the point symbol to be drawn at the line start
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetLineStartPointSymbol( PointSymbolDef* lineStartPointSymbol );

		/**
		 * Get the point symbol (if any) to draw at the line end.
		 * @return the point symbol to draw at the line end
		 * May be NULL.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		PointSymbolDef* GetLineEndPointSymbol()
		{
			return m_lineEndPointSymbol;
		}

		/**
		 * Set the point symbol to be drawn at the line end.
		 * @param lineStartPointSymbolDef - the point symbol to be drawn at the line end
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		void SetLineEndPointSymbol( PointSymbolDef* lineEndPointSymbol );

		/**
		 * Get the collection of stroke patterns with which the line feature is rendered.
		 * @return the collection of stroke patterns with which the line feature is rendered
		 * May be NULL.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		StrokePatternCollection* GetStrokePatternCollection()
		{
			return m_strokePatternCollection;
		}

		/**
		 * Set the collection of stroke patterns with which to render the line feature.
		 * @param strokePatternCollection - the collection of stroke patterns with which to render the line feature
		 * May be NULL.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetStrokePatternCollection( StrokePatternCollection* strokePatternCollection )
			{
				delete m_strokePatternCollection;
				m_strokePatternCollection = strokePatternCollection;
			}

			bool operator== ( LineStyle& style );

			LineStyle*Clone(Error& error) const;

#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& destination, Error& error ) const;

			/** 
			 * @see
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif

		private:
			int m_color;
			float m_width;
            LineCapType m_lineCap;
			float m_lineDrawOffset;
			PointSymbolDef* m_lineStartPointSymbol;
			PointSymbolDef* m_lineEndPointSymbol;
			StrokePatternCollection* m_strokePatternCollection;
		};

		/**
		 * Class that encapsulates the properties of a point feature rendering style.
		 * An array of instances of this class is held by each FeatureSymbol instance.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API PointStyle : public FeatureStyle
		{
		public:
			/**
			 * Possible anchor locations for a feature around a point symbol's bounding box
			 *
			 * Used for mobile client rendering purposes.
			 * @reqid TODO
			 */
			enum PointSymbolAnchor
			{
				PointSymbolAnchor_MiddleMiddle_GlyphBox = 0,
				PointSymbolAnchor_LeftMiddle_GlyphBox = 1,
				PointSymbolAnchor_LeftTop_GlyphBox = 2,
				PointSymbolAnchor_TopMiddle_GlyphBox = 3,
				PointSymbolAnchor_TopRight_GlyphBox = 4,
				PointSymbolAnchor_RightMiddle_GlyphBox = 5,
				PointSymbolAnchor_RightBottom_GlyphBox = 6,
				PointSymbolAnchor_BottomMiddle_GlyphBox = 7,
				PointSymbolAnchor_LeftBottom_GlyphBox = 8,

				PointSymbolAnchor_MiddleMiddle_CharCell = 9,
				PointSymbolAnchor_LeftMiddle_CharCell = 10,
				PointSymbolAnchor_LeftTop_CharCell = 11,
				PointSymbolAnchor_TopMiddle_CharCell = 12,
				PointSymbolAnchor_TopRight_CharCell = 13,
				PointSymbolAnchor_RightMiddle_CharCell = 14,
				PointSymbolAnchor_RightBottom_CharCell = 15,
				PointSymbolAnchor_BottomMiddle_CharCell = 16,
				PointSymbolAnchor_LeftBottom_CharCell = 17,

				PointSymbolAnchor_MiddleGlyphBoxY_MiddleCharCellX=18
			};

			enum PointStyleType
			{
				PointStyleType_None,
				PointStyleType_Font,
				PointStyleType_Raster,
			};

			PointStyle(PointStyleType pointStyleType) : FeatureStyle(StyleType_Point),
				m_pointStyleType(pointStyleType), m_anchorLocation(PointSymbolAnchor_MiddleMiddle_GlyphBox),
				m_offsetX(0), m_offsetY(0), m_rotation(0), m_shouldFlip(false)
			{
			}

			PointStyle(PointStyleType pointStyleType, PointSymbolAnchor anchor, float angle) : 
				FeatureStyle(StyleType_Point), m_pointStyleType(pointStyleType), 
				m_offsetX(0), m_offsetY(0), m_anchorLocation(anchor), m_rotation(angle), m_shouldFlip(false)
			{
			}

			virtual ~PointStyle(){}

		/**
		 * Get the feature's anchor location relative to the point symbol.
		 * @return the feature's anchor location relative to the point symbol
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			PointSymbolAnchor GetAnchorLocation()
			{
				return m_anchorLocation;
			}

		/**
		 * Set the feature's anchor location relative to the point symbol.
		 * @param the feature's anchor location relative to the point symbol
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetAnchorLocation( PointSymbolAnchor anchorLocation )
			{
				m_anchorLocation = anchorLocation;
			}

		/**
		 * Get the feature's x offset.
		 *
		 * Used for mobile client rendering purposes.
		 */
			float GetOffsetX()
			{
				return m_offsetX;
			}

		/**
		 * Set the feature's x offset.
		 *
		 * Used for mobile client rendering purposes.
		 */
			void SetOffsetX(float offsetX)
			{
				m_offsetX = offsetX;
			}

		/**
		 * Get the feature's y offset.
		 *
		 * Used for mobile client rendering purposes.
		 */
			float GetOffsetY()
			{
				return m_offsetY;
			}

		/**
		 * Set the feature's y offset.
		 *
		 * Used for mobile client rendering purposes.
		 */
			void SetOffsetY(float offsetY)
			{
				m_offsetY = offsetY;
			}

		/**
		 * Get the rotation through which the glyph is transformed in defining the point feature style.
		 * Units are assumed to be radians counter-clockwise.
		 * @return the feature anchor location on the point symbol
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			float GetRotation()
			{
				return m_rotation;
			}

		/**
		 * Set the rotation through which the glyph is transformed in defining the point feature style.
		 * @param the feature anchor location on the point symbol
		 * Units are assumed to be radians counter-clockwise.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetRotation( float rotation )
			{
				m_rotation = rotation;
			}

		/**
		 * check if this point style should be flipped.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			bool GetShouldFlip()
			{
				return m_shouldFlip;
			}

		/**
		 * Set the should flip flag
		 * @param shouldFlip 
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetShouldFlip( bool shouldFlip )
			{
				m_shouldFlip = shouldFlip;
			}

			/**
			 * Get the point style type of this point style
			 * @return the type
			 * @reqid 001.0063
			 */
			virtual PointStyleType GetPointStyleType()
			{
				return m_pointStyleType;
			}

			virtual bool operator== ( PointStyle& style ) = 0;

#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& destination, Error& error ) const;
			/** 
			 * @see
			 */
			virtual void _Deserialize( IDeserializer&, Error& )
			{

			}

			/** 
			 * @see
			 */
			void Deserialize( IDeserializer& deserializer, Error& error );

			/**
			* Deserializes a new object from the deserializer.
			* @param deserializer - The source.
			* @param output - Deserialized object if no error occured. NULL otherwise.
			* @param error - Error info.
			* @reqid N/A
			*/
			static PointStyle* DeserializeNew( IDeserializer& deserializer, Error& error );

		protected:
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void __Serialize( ISerializer& emitter, Error& error ) const = 0;

			/**
			 * Called on the specific implementation to deserialize itself.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void __Deserialize( IDeserializer& deserializer, Error& error ) = 0;

#endif

		protected:

			static void Copy( const PointStyle& src, PointStyle& dest, Error& error );

			PointSymbolAnchor m_anchorLocation;
			float m_offsetX;
			float m_offsetY;
			float m_rotation;
			PointStyleType m_pointStyleType;
			bool m_shouldFlip;
		};

		/**
		 * Class that encapsulates the properties of a point feature rendering style.
		 * An array of instances of this class is held by each FeatureSymbol instance.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API RasterPointStyle : public PointStyle
		{
		public:
			enum RasterType
			{
				RasterType_Unknown = -1,
				RasterType_PNG,
				RasterType_JPEG,
			};

			RasterPointStyle() : PointStyle(PointStyle::PointStyleType_Raster), m_opacity(1),
				m_rasterType(RasterType_PNG)
			{
			}

			RasterPointStyle(RasterType rasterType, const char* fileName, float angle, Error& error) : 
				PointStyle(PointStyle::PointStyleType_Raster, PointSymbolAnchor_MiddleMiddle_GlyphBox, angle),
				m_rasterType(rasterType), m_opacity(1)
			{
				m_fileName.AssignString(fileName, error);
			}

			RasterPointStyle(RasterType rasterType, const char* fileName, Error& error) : 
				PointStyle(PointStyle::PointStyleType_Raster), m_rasterType(rasterType), m_opacity(1)
			{
				m_fileName.AssignString(fileName, error);
			}

			~RasterPointStyle(){}

		/**
		 * Get the file name of the raster bitmap that is rendered for this point feature style.
		 * @return the filename of the raster file
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			SharedString* GetFileName()
			{
				return &m_fileName;
			}

		/**
		 * Set the file name of the raster bitmap that is to be rendered for this point feature style.
		 * @param fileName
		 * @param error
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetFileName( TCHAR* fileName, Error& error )
			{
				m_fileName.AssignString(fileName, error);
			}

		/**
		 * Get the opacity to render the point feature at
		 * @return the opacity
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			float GetOpacity()
			{
				return m_opacity;
			}

		/**
		 * Set the opacity to render the point feature at
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetOpacity( float opacity )
			{
				m_opacity = opacity;
			}

			RasterPointStyle* Clone(Error& error) const;

			virtual bool operator== ( PointStyle& style );

#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void __Serialize( ISerializer& destination, Error& error ) const;

			/** 
			 * @see
			 */
			virtual void __Deserialize( IDeserializer& deserializer, Error& error );

#endif

		private:
			float m_opacity;
			SharedString m_fileName;
			RasterType m_rasterType;
		};


		/**
		 * Class that encapsulates the properties of a point feature rendering style.
		 * An array of instances of this class is held by each FeatureSymbol instance.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API FontPointStyle : public PointStyle
		{
		public:

			FontPointStyle();

			FontPointStyle(long color, float size, PointSymbolAnchor anchorLocation,
								float rotation, const char* fontName, TCHAR character, Error& error);

			FontPointStyle(long color, float size, const char* fontName, TCHAR character, Error& error);

			~FontPointStyle();

		/**
		 * Get the color in which the point feature symbol is rendered.
		 * Formatted as a Windows RGB long value
		 * @return the color in which the point feature symbol is rendered
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			long GetColor()
			{
				return m_color;
			}

		/**
		 * Set the color in which the point feature symbol is to be rendered.
		 * @param color - the color in which the point feature symbol is to be rendered
		 * Formatted as a Windows RGB long value
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetColor( long color )
			{
				m_color = (int)color;
			}

		/**
		 * Get the size at which the point feature symbol is rendered.
		 * Units are assumed to be inches.
		 * @return the size at which the point feature symbol is rendered
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			float GetSize()
			{
				return m_size;
			}

		/**
		 * Set the size at which the point feature symbol is to be rendered.
		 * @param size - the size at which the point feature symbol is to be rendered
		 * Units are assumed to be inches.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetSize( float size )
			{
				m_size = size;
			}

		/**
		 * Get the size at which the point feature symbol halo is rendered.
		 * Units are assumed to be inches.
		 * @return the size at which the point feature symbol halo is rendered
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			float GetHaloSize()
			{
				return m_haloSize;
			}

		/**
		 * Set the size at which the point feature symbol halo is to be rendered.
		 * @param size - the size at which the point feature symbol halo is to be rendered
		 * Units are assumed to be inches.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetHaloSize( float size )
			{
				m_haloSize = size;
			}

		/**
		 * Get the fontname from which the glyph is rendered for this point feature style.
		 * @return the font name from which the glyph is rendered
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			SharedString* GetFontName()
			{
                return &m_fontName;
			}

        /**
         * Get the font name
         * @return the font name as a pointer to a SharedString instance.
         * @reqid 001.0063
         */
        SharedString* GetFontFilePath()
        {
            return &m_fontFilePath;
        }

        /**
		 * Set the fontname from which the glyph is to be rendered for this point feature style.
		 * @param fontName
		 * @param error
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetFontName( TCHAR* fontName, Error& error )
			{
				m_fontName.AssignString(fontName, error);
			}

            /**
             * Set the font path
             * @reqid TODO
             */
            void SetFontPath(const char* fontPath, Error& error)
            {
                String filePath(fontPath, error);
                if(error)
                {
                    return;
                }
                
                String fontName(m_fontName.GetString(), error);
                if(error)
                {
                    return;
                }
                fontName.Lcase();

                filePath.ConcatString("/", error);
                filePath.ConcatString(fontName.GetString(), error);
                if(error)
                {
                    return;
                }
                
                m_fontFilePath.AssignString(filePath.GetString(), error);
            }
            
		/**
		 * Get the glyph that is rendered for this point feature style.
		 * @return the glyph that is rendered
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			TCHAR GetCharacter()
			{
				return m_character;
			}

		/**
		 * Set the translucency with which the glyph is to be rendered for this point feature style.
		 * @param the translucency with which the glyph is to be rendered
		 * Must be between 0 and 1 inclusive. 0 ==> fully opaque and 1 ==> fully transparent.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetCharacter( TCHAR character )
			{
				m_character = character;
			}

		/**
		 * Get the symbol outline
		 * @return the symbol outline
		 *
		 * @reqid TODO
		 */
		Polygons* GetGlyphOutline();

		/**
		 * Set symbol outline this point feature style.
		 * @param the outline 
		 *
		 * @reqid TODO
		 */
		void SetGlyphOutline( Polygons* outline );

		virtual bool operator== ( PointStyle& style );

		FontPointStyle* Clone(Error& error) const;

#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void __Serialize( ISerializer& destination, Error& error ) const;

			/** 
			 * @see
			 */
			virtual void __Deserialize( IDeserializer& deserializer, Error& error );

#endif

		private:
			int m_color;
			float m_size;
			float m_haloSize;
			SharedString m_fontName;
			SharedString m_fontFilePath;
			TCHAR m_character;
			Polygons* m_glyphOutline;
		};

		/**
		 * Class that encapsulates the properties of an area feature rendering style.
		 * An array of instances of this class is held by each FeatureSymbol instance.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API AreaStyle : public FeatureStyle
		{
		public:

			AreaStyle() : FeatureStyle(StyleType_Area)
			{
				m_fillColor = 0xFF000000;
				m_boundaryLineSymbol = NULL;
			}

			AreaStyle(long fillColor, LineSymbol* boundaryLineSymbol = NULL) : 
							FeatureStyle(StyleType_Area), m_fillColor((int)fillColor),
							m_boundaryLineSymbol(boundaryLineSymbol)
			{
			}

			~AreaStyle();

		/**
		 * Get the color with which the interior is filled.
		 * Formatted as a Windows RGB long value
		 * @return the color with which the interior is filled
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			long GetFillColor()
			{
				return m_fillColor;
			}

		/**
		 * Set the color with which the interior is to be filled.
		 * @param fillColor - the color with which the interior is to be filled
		 * Formatted as a Windows RGB long value
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetFillColor( long fillColor )
			{
				m_fillColor = (int)fillColor;
			}

		/**
		 * Get the line symbol (if any) with which the boundary is drawn.
		 * @return the line symbol with which the boundary is drawn
		 * May be NULL.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			LineSymbol* GetBoundaryLineSymbol()
			{
				return m_boundaryLineSymbol;
			}

		/**
		 * Set the line symbol (if any) with which to draw the boundary.
		 * @param boundaryLineSymbolDef - the line symbol with which to draw the boundary
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
			void SetBoundaryLineSymbol( LineSymbol* boundaryLineSymbol )
			{
				m_boundaryLineSymbol = boundaryLineSymbol;
			}

			bool operator== ( AreaStyle& style );

			AreaStyle* Clone(Error& error) const;

#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer& destination, Error& error ) const;

			/** 
			 * @see
			 */
			virtual void _Deserialize( IDeserializer& deserializer, Error& error );
#endif


		private:
			int m_fillColor;
			LineSymbol* m_boundaryLineSymbol;
		};

		class LABELINGENGINE_API FeatureStyles : public ClonableLinkedList < FeatureStyle, FeatureStyles >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				FeatureStyles( bool ownsElements = true ) : ClonableLinkedList < FeatureStyle, FeatureStyles >( ownsElements )
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~FeatureStyles()
				{}
		};

		/**
		 * Class that encapsulates a feature symbol
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API FeatureSymbol
		{
		public:
			enum SymbolType
			{
				SymbolType_Unknown = -1,
				SymbolType_Area,
				SymbolType_Line,
				SymbolType_Point,
				SymbolType_Count
			};

			FeatureSymbol(SymbolType symbolType) : 
			  m_symbolType(symbolType)
			{
			}

			virtual ~FeatureSymbol()
			{
			}

			/**
			 * Get the collection of styles
			 */
			FeatureStyles* GetFeatureStyles()
			{
				return &m_featureStyles;
			}

			/**
			 * Add a style to the style collection
			 */
			void AddFeatureStyle(FeatureStyle* featureStyle, Error& error)
			{
				m_featureStyles.AddElement(featureStyle, error);
			}

			bool operator== ( FeatureSymbol& symbol );

			virtual FeatureSymbol* Clone(Error& error) const = 0;

#ifdef S11N
			/**
			 * Serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			void Serialize( ISerializer& destination, Error& error ) const;

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
			static FeatureSymbol* DeserializeNew( IDeserializer& deserializer, Error& error );

			SymbolType GetSymbolType()
			{
				return m_symbolType;
			}

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

		protected:
			FeatureStyles m_featureStyles;

			static void Copy( const FeatureSymbol& src, FeatureSymbol& dest, Error& error );

		private:
			SymbolType m_symbolType;
		};

		/**
		 * Class that encapsulates a line feature symbol
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API LineSymbol : public FeatureSymbol
		{
		public:
			LineSymbol() : FeatureSymbol(SymbolType_Line)
			{
			}

			~LineSymbol(){}

			float GetOverallLineWidth();

			LineSymbol* Clone(Error& error) const;

#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer&, Error& ) const
			{

			}

			/** 
			 * @see
			 */
			virtual void _Deserialize( IDeserializer&, Error& )
			{

			}
#endif

		};

		/**
		 * Class that encapsulates an point feature symbol
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API PointSymbolDef : public FeatureSymbol
		{
		public:
			PointSymbolDef() : FeatureSymbol(SymbolType_Point)
			{
			}

			~PointSymbolDef(){}

			PointSymbolDef* Clone(Error& error) const;

		#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer&, Error& ) const
			{

			}

			/** 
			 * @see
			 */
			virtual void _Deserialize( IDeserializer&, Error& )
			{

			}
#endif
            std::string BuildPointSymbolName(int angle);
};

		/**
		 * Class that encapsulates an area feature symbol
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API AreaSymbol : public FeatureSymbol
		{
		public:
			AreaSymbol() : FeatureSymbol(SymbolType_Area)
			{
			}

			~AreaSymbol(){}

			AreaSymbol* Clone(Error& error) const;

#ifdef S11N
			/**
			 * Child specific serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void _Serialize( ISerializer&, Error& ) const
			{

			}

			/** 
			 * @see
			 */
			virtual void _Deserialize( IDeserializer&, Error& )
			{

			}
#endif
		};

		/**
		 * Class that encapsulates a scale range enabled feature symbol.
		 * Each instance is contained by an instance of a SymbologySet.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API RangeSymbology
		{
		public:
			RangeSymbology();

			RangeSymbology(float minScale, float maxScale, FeatureSymbol* featureSymbol = NULL ) : 
								m_minScale(minScale), m_maxScale(maxScale), m_featureSymbol(featureSymbol)
			{
			}

			~RangeSymbology()
			{
				delete m_featureSymbol;
			}

			float GetMinScale()
			{
				return m_minScale;
			}

			void SetMinScale(float scale)
			{
				m_minScale = scale;
			}
			
			float GetMaxScale()
			{
				return m_maxScale;
			}

			void SetMaxScale(float scale)
			{
				m_maxScale = scale;
			}

			FeatureSymbol* GetFeatureSymbol()
			{
				return m_featureSymbol;
			}

			void SetFeatureSymbol(FeatureSymbol* featureSymbol)
			{
				delete m_featureSymbol;
				m_featureSymbol = featureSymbol;
			}

			bool operator== ( RangeSymbology& range );

			RangeSymbology* Clone(Error& error) const;

#ifdef S11N
				/**
				 * @see ISerializable::Serialize
				 * @reqid N/A.
				 */
			void Serialize( ISerializer& destination, Error& error ) const;

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
			void Deserialize( IDeserializer& deserializer, Error& error);

                /**
                 * The DeserializeNew method instantiates a new feature by deserializing it
                 * @param deserializer - The deserializer to use.
                 * @param error - Error info.
                 * @return - New feature instance or NULL on error.
                 *
                 * @reqid PERSISTENCE.
                 */
			static RangeSymbology* DeserializeNew( IDeserializer& deserializer, Error& error );

#endif

		private:
			float m_minScale;
			float m_maxScale;
			FeatureSymbol* m_featureSymbol;
		};

		class LABELINGENGINE_API RangeSymbologys : public ClonableLinkedList < RangeSymbology, RangeSymbologys >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				RangeSymbologys( bool ownsElements = true ) : ClonableLinkedList < RangeSymbology, RangeSymbologys >( ownsElements )
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~RangeSymbologys()
				{}
		};

		/**
		 * Class that encapsulates a scale range enabled collection of feature symbols.
		 * An instance of this class is held by the Shape object that defines a feature's shape.
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API SymbologySet
		{
		public:
			SymbologySet() : m_defaultFeatureSymbol(NULL), m_isExpressionBasedSymbol(false)
			{}

			SymbologySet(const TCHAR* symbologysetID, Error& error) : m_defaultFeatureSymbol(NULL), m_isExpressionBasedSymbol(false)
			{
				m_symbologySetId.AssignString(symbologysetID, error);
			}

			SymbologySet(const TCHAR* symbologysetID, const TCHAR* label, Error& error) : m_defaultFeatureSymbol(NULL), m_isExpressionBasedSymbol(false)
			{
				m_symbologySetId.AssignString(symbologysetID, error);
				m_label.AssignString(label, error);
			}

			SymbologySet(FeatureSymbol* featureSymbol) : m_defaultFeatureSymbol(featureSymbol), m_isExpressionBasedSymbol(false)
			{}

			SymbologySet(const TCHAR* symbologysetID, FeatureSymbol* featureSymbol, Error& error) : 
				m_defaultFeatureSymbol(featureSymbol), m_isExpressionBasedSymbol(false)
			{
				m_symbologySetId.AssignString(symbologysetID, error);
			}

			~SymbologySet()
			{
				delete m_defaultFeatureSymbol;
			}

			/**
			* Sets the symbologyset ID
			* @see	GetSymbologySetID
			* @reqid
			*/
			void SetSymbologySetID( const TCHAR* symbologysetID, MapText::Dtp::Error& error )
			{
				m_symbologySetId.AssignString(symbologysetID, error);
			}

			/**
			* Gets the symbologyset ID.
			* SymbologysetID is a textual identifier of the symbologyset, assigned by API client.
			* This is for client use and may or may not be a unique identifier.
			* @reqid
			*/
			const String* GetSymbologySetID() const
			{
				return &m_symbologySetId;
			}

			/**
			* Sets the symbologyset label
			* @see	GetLabel
			* @reqid
			*/
			void SetLabel( const TCHAR* label, MapText::Dtp::Error& error )
			{
				m_label.AssignString(label, error);
			}

			/**
			* Gets the label.
			* Label is the presented name, assigned by API client.
			* This is for client use and may or may not be a unique identifier.
			* @reqid
			*/
			const SharedString* GetLabel() const
			{
				return &m_label;
			}

			void SetDefaultFeatureSymbol(FeatureSymbol* featureSymbol)
			{
				delete m_defaultFeatureSymbol;
				m_defaultFeatureSymbol = featureSymbol;
			}

			RangeSymbologys* GetRangeSymbologies()
			{
				return &m_rangeSymbologies;
			}

			/**
			 * Find the feature symbol (if any) to be rendered
			 * at the specified scale.
			 *
			 * Used for mobile client rendering purposes.
			 *
			 * @param scale - the scale at which the feature is to be rendered
			 * @return the first applicable feature symbol or NULL if none
			 * @reqid TODO
			 */
			FeatureSymbol* GetFeatureSymbolForScale(float scale);

			/**
			 * Get the default feature symbol when no scale ranges are present
			 *
			 * Used for mobile client rendering purposes.
			 *
			 * @return the default feature symbol
			 * @reqid TODO
			 */
			FeatureSymbol* GetDefaultFeatureSymbol()
			{
				return m_defaultFeatureSymbol;
			}

            /**
			 * Get the first available featuresymbol
			 *
			 * Used for mobile client rendering purposes.
			 *
			 * @return the default feature symbol
			 * @reqid TODO
			 */
			FeatureSymbol* GetFirstAvailableFeatureSymbol();
            
			/**
			* Add a style to the style collection
			*/
			void AddRangeSymbology(RangeSymbology* rangeSymbology, Error& error)
			{
				m_rangeSymbologies.AddElement(rangeSymbology, error);
			}

            /**
			 * Set whether this SymbologySet contains symbols built based on a feature's field values or not.
			 *
			 * Used for mobile client rendering purposes.
			 *
			 * @param isExpressionBasedSymbol - true-contains symbols built from field value, false, it does not.
			 * @reqid TODO
			 */
			void SetIsExpressionBasedSymbol(bool isExpressionBasedSymbol)
			{
				m_isExpressionBasedSymbol = isExpressionBasedSymbol;
			}

            /**
			 * @see SetIsExpressionBasedSymbol
			 *
			 * Used for mobile client rendering purposes.
			 *
			 * @return true-contains symbols built from field value, false, it does not.
			 * @reqid TODO
			 */
			bool GetIsExpressionBasedSymbol()
			{
				return m_isExpressionBasedSymbol;
			}

		bool operator== ( SymbologySet& symbologySet );

		SymbologySet* Clone(Error& error) const;

#ifdef S11N
				/**
				 * @see ISerializable::Serialize
				 * @reqid N/A.
				 */
			void Serialize( ISerializer& destination, Error& error ) const;

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
			void Deserialize( IDeserializer& deserializer, Error& error);

                /**
                 * The DeserializeNew method instantiates a new feature by deserializing it
                 * @param deserializer - The deserializer to use.
                 * @param error - Error info.
                 * @return - New feature instance or NULL on error.
                 *
                 * @reqid PERSISTENCE.
                 */
			static SymbologySet* DeserializeNew( IDeserializer& deserializer, Error& error );

#endif


		private:

			FeatureSymbol* GetFeatureSymbolForValue(float scale);

			RangeSymbologys m_rangeSymbologies;
			FeatureSymbol* m_defaultFeatureSymbol;
			String m_symbologySetId;
			SharedString m_label; //feature class name
			bool m_isExpressionBasedSymbol;
		};

		class LABELINGENGINE_API SymbologySets : public ClonableLinkedList < SymbologySet, SymbologySets >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				SymbologySets( bool ownsElements = true );

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~SymbologySets();
            
                /**
                 * Find a symbologyset in the collection using its ID.
                 * This implementation has an avg O(1) complexity (hash table lookup).
                 * @param symbologySetId - ID of the symbologySet to find. Passing NULL is safe and
                 *                    always returns NULL.
                 * @return - the Symbologyset if it was not found or NULL.
                 * @reqid
                 */
                SymbologySet* FindById( const TCHAR* symbologySetId );
            
				/**
				 * Overriding the default implementation to maintain the quick-access index.
				 * @reqid N/A
				 */

				virtual void AddElement( SymbologySet* element, Error& error );
            
        /**
         * Overriding the default implementation to maintain the quick-access index.
         * @reqid N/A
         */
        virtual void Clear();
            
		protected:
				virtual void RemoveElement( Element* e );

		private:
			/**
			 * Hide the private parts
			 */
			class LABELINGENGINE_API Impl; Impl* m_pimpl;
		};

		/**
		 * Class that encapsulates a set of symbologySets
		 *
		 * Used for mobile client rendering purposes.
		 * @reqid TODO
		 */
		class LABELINGENGINE_API Layer
		{
		public:
			enum ErrorCode
			{
				ErrorCode_AllocationFailure = 1,
#ifdef S11N
				ErrorCode_SerializationFailure,
				ErrorCode_DeserializationFailure,
#endif
			};

			Layer() : m_drawOrder(0)
			{}

			Layer(const TCHAR* name, Error& error) : m_drawOrder(0)
			{
				m_name.AssignString(name, error);
			}

			~Layer()
			{}

			const SharedString* GetLayerName() const
			{
				return &m_name;
			}

			int GetDrawOrder()
			{
				return m_drawOrder;
			}

			void SetDrawOrder(int drawOrder )
			{
				m_drawOrder = drawOrder;
			}

			SymbologySets* GetSymbologySets()
			{
				return &m_symbologySets;
			}

			void AddSymbologySet(SymbologySet* symbologySet, Error& error)
			{
				m_symbologySets.AddElement(symbologySet, error);
			}

			Layer* Clone(Error& error) const;

#ifdef S11N
				/**
				 * @see ISerializable::Serialize
				 * @reqid N/A.
				 */
			void Serialize( ISerializer& destination, Error& error ) const;

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
			void Deserialize( IDeserializer& deserializer, Error& error);

#endif
		private:
			SymbologySets m_symbologySets;
			SharedString m_name;
			int m_drawOrder;
		};

		class LABELINGENGINE_API Layers : public ClonableLinkedList < Layer, Layers >
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				Layers( bool ownsElements = true );

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~Layers();
            
                /**
                 * Find a Layer in the collection using its name.
                 * This implementation has an avg O(1) complexity (hash table lookup).
                 * @param name - name of the layer to find. Passing NULL is safe and
                 *                    always returns NULL.
                 * @return - the Layer if it was not found or NULL.
                 * @reqid
                 */
                Layer* FindByLayerName( const TCHAR* layerName ) const;
            
				/**
				 * Overriding the default implementation to maintain the quick-access index.
				 * @reqid N/A
				 */

				virtual void AddElement( Layer* element, Error& error );
            
        /**
         * Overriding the default implementation to maintain the quick-access index.
         * @reqid N/A
         */
        virtual void Clear();
            
		void RemoveLayerByName(const TCHAR* layerName);

		protected:
				virtual void RemoveElement( Element* e );

		private:
			/**
			 * Hide the private parts
			 */
			class LABELINGENGINE_API Impl; Impl* m_pimpl;
		};


	}
}

#endif 
