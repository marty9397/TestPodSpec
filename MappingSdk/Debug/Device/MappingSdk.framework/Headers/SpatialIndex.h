/**************************************************
* Copyright (c)2008 Maptext, Inc. All rights reserved.
* www:   http://www.maptext.com
* email: support@maptext.com
*-------------------------------------------------
* Workfile: $HeadURL: https://martin@mtsrv.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/LabelingEngine/SpatialIndex.h $
* 
* Revision: $Revision: 43646 $
*
* Last modified on: $Date: 2013-01-03 12:05:16 -0500 (Thu, 03 Jan 2013) $, by: $Author: ptadych $
* 
*************************************************/

#ifndef DTP_SPATIALINDEX_H
#define DTP_SPATIALINDEX_H

#include "MtGeometry.h"

namespace MapText
{
	namespace Dtp
	{

		/**
		* Forward declare pointer-only dependencies
		*/
		class Label;
		class Leader;
		class TextOverlap;
		class TextPart;
		class ObstacleDetectionArgs;
		class SymbolComponent;

		/**
		 * A dictionary entry - a single entity indexed by the spatial index.
		 * @reqid 001.0064
		 */
		class LABELINGENGINE_API IndexDictionaryEntry
		{
		public:
			enum EntryType
			{
				EntryType_FeaturePolyline,
				EntryType_OpaqueFeaturePolygon,
				EntryType_TextPart,
				EntryType_Leader,
				EntryType_SymbolComponent,
			};

			enum ErrorCode
			{
				ErrorCode_AllocationError = 1,
				ErrorCode_NoFeatureTypeSpecified,
				ErrorCode_UnhandledFeatureType,
			};

			class LABELINGENGINE_API Collection : public LinkedList<IndexDictionaryEntry>
			{
			public:

				/**
				* Constructor.
				* @param ownsElements if true the elements will be destroyed with the collection.
				* @reqid 001.0064
				*/
				Collection( bool ownsElements )
					: LinkedList<IndexDictionaryEntry>( ownsElements )
				{
				}

				/**
				* Destructor declared explicitly to avoid compiler generated code.
				* @reqid 001.0064
				*/
				~Collection()
				{
				}
			};

			/**
			* Constructor.
			* @reqid 001.0064
			*/
			IndexDictionaryEntry( )
			{
			}

			/**
			* Assure cleanup of derived classes.
			* @reqid 001.0064
			*/
			virtual ~IndexDictionaryEntry()
			{

			}

			/**
			* Check the type of entry
			* @return the type allowing you to cast it to the right specialized entry class
			* @reqid 001.0064
			*/
			virtual EntryType GetType() const = 0;

			/**
			 * Process the index dictionary entry. Used during obstacle detection. It analyzes
			 * the owned obstacle against the obstacle detection query accessible through args
			 * and updates the overlap summary accordingly.
			 * @param args [in, out] Obstacle detection arguments. Includes overlap summary which
			 *        may be updated.
			 * @param error error information
			 * @reqid 001.0064
			 */
			virtual void Process( ObstacleDetectionArgs& args, Error& error ) = 0;

		protected:

			/**
			 * Given a rectangular obstacle (i.e. text part or a thick polyline segment box)
			 * this method calculates the rectilinear shifts required to clear the query out 
			 * of the obstacle area.
			 * @param obstacleRect The obstacle to be avoided
			 * @param args [in, out] obstacle detection arguments
			 * @param isOverlapping [out] set to true if obstacle overlaps at all, false 
			 *                      otherwise
			 * @param shiftLeft left shift to avoid obstacle
			 * @param shiftRight right shift to avoid obstacle
			 * @param shiftDown down shift to avoid obstacle
			 * @param shiftUp up shift to avoid obstacle
			 * @param error error information
			 * @reqid 001.0064
			 */
			static void CalculateOverlapShifts( Rectangle &obstacleRect, ObstacleDetectionArgs& args, 
				bool* isOverlapping, float* shiftLeft, float* shiftRight, 
				float* shiftDown, float* shiftUp, Error &error);

			/**
			 * Processes a stroked polyline in a proper way, meaning if the stroke is thick
			 * enough (as configured with Configuration::GetThickLineThreshold()) the polyline
			 * is processed as a set of rectangles, otherwise as a zero-width geometry.
			 * @param geometry			the stroked polyline, accessed via an IPolylineView interface
			 * @param strokeWidth		the width of the polyline stroke
			 * @param args					[in] obstacle detection arguments
			 * @param overlaps			[out] Results of the processing are saved here
			 * @param error					error information
			 * @reqid 001.0064
			 */
			static void ProcessStrokedPolyline( IPolylineView* geometry, float strokeWidth, ObstacleDetectionArgs& args, 
				OverlapGeometries &overlaps, Error& error );
		};

		/**
		 * Stores a polyline feature reference in the spatial index dictionary.
		 * @reqid 001.0064
		 */
		class LABELINGENGINE_API PolylineFeatureDictionaryEntry : public IndexDictionaryEntry
		{
		public:

			/**
			 * This index dictionary entry holds information about a feature with polyline geometry.
			 * @param featureRef feature owning the geometry
			 * @param polylinePart part of the polyline geometry
			 * @reqid 001.0064
			 */
			PolylineFeatureDictionaryEntry( const Feature& featureRef, PolylinePart* polylinePart )
				: m_featureRef( &featureRef ), m_geometry( polylinePart )
			{

			}

			/**
			 * Destructor.
			 * @reqid 001.0064
			 */
			~PolylineFeatureDictionaryEntry()
			{
				delete m_geometry;
			}

			/**
			* @see IndexDictionaryEntry::GetType
			* @reqid 001.0064
			*/
			EntryType GetType() const
			{
				return EntryType_FeaturePolyline;
			}
			/**
			* Gets FeatureRef
			* @return FeatureRef
			* @reqid 001.0064
			*/
			const Feature* GetFeatureRef() const 
			{ 
				return m_featureRef; 
			}

			/**
			* Sets FeatureRef
			* @param val FeatureRef
			* @reqid 001.0064
			*/
			void SetFeatureRef(const Feature* val) 
			{ 
				m_featureRef = val; 
			}

			/**
			* Gets Geometry
			* @return Geometry
			* @reqid 001.0064
			*/
			PolylinePart* GetGeometry() const 
			{ 
				return m_geometry; 
			}

			/**
			* Sets Geometry
			* @param val Geometry
			* @reqid 001.0064
			*/
			void SetGeometry(PolylinePart* val) 
			{ 
				m_geometry = val; 
			}

			/**
			 * @see IndexDictionaryEntry::Process
			 * @reqid 001.0064
			 */
			void Process( ObstacleDetectionArgs& args, Error& error );

		private:
			const Feature* m_featureRef;
			PolylinePart* m_geometry;
		};

		/**
		 * Stores an opaque polygon feature reference in the spatial index dictionary.
		 * @reqid 001.0064
		 */
		class LABELINGENGINE_API OpaqueFeaturePolygonDictionaryEntry : public IndexDictionaryEntry
		{
		public:

			/**
			 * This index dictionary entry holds information about an opaque feature with polygon geometry.
			 * @param featureRef feature owning the geometry
			 * @param polylinePart part of the polyline geometry
			 * @reqid 001.0064
			 */
			OpaqueFeaturePolygonDictionaryEntry( const Feature& featureRef )
				: m_featureRef( &featureRef )
			{

			}

			/**
			 * Destructor.
			 * @reqid 001.0064
			 */
			~OpaqueFeaturePolygonDictionaryEntry()
			{
			}

			/**
			* @see IndexDictionaryEntry::GetType
			* @reqid 001.0064
			*/
			EntryType GetType() const
			{
				return EntryType_OpaqueFeaturePolygon;
			}

			/**
			 * @see IndexDictionaryEntry::Process
			 * @reqid 001.0064
			 */
			void Process( ObstacleDetectionArgs& args, Error& error );

			/**
			 * Get the feature referenced by the entry
			 * @return - feature held by the entry
			 * @reqid 001.0064
			 */
			const Feature* GetFeatureRef() const
			{
				return m_featureRef;
			}

		private:
			const Feature* m_featureRef;
		};		

		/**
		 * Abstract parent class for label parts dictionary entries
		 * @reqid 001.0064
		 */
		class LABELINGENGINE_API LabelDictionaryEntry : public IndexDictionaryEntry
		{
		protected:
			/**
			 * Constructor.
			 * @param label label to which the part belongs
			 * @return 
			 * @reqid 001.0064
			 */
			LabelDictionaryEntry( Label* label )
				: m_labelRef( label )
			{}

			/**
			 * Destructor (compiler generated code avoidance).
			 * @reqid 001.0064
			 */
			~LabelDictionaryEntry(){};

			
			/**
			 * Process a rectangular label part.
			 * @param args obstacle detection args
			 * @param box a rectangular label part.
			 * @param error error info
			 * @reqid 001.0064
			 */
			void ProcessLabelBox( ObstacleDetectionArgs& args, Rectangle* box, Error& error );

			/**
			 * Initialize the text overlap proper for given derived class. Needed
			 * inside the ProcessLabelBox method.
			 * To be implemented by children.
			 * @param overlap [out] the overlap to initialize
			 * @param error error info
			 * @reqid 001.0064
			 */
			virtual void CreateOverlap( TextOverlap** overlap, Error& error ) = 0;

			Label* m_labelRef;
		};

		/**
		 * An index dictionary to store a text part
		 * @reqid 001.0064
		 */
		class LABELINGENGINE_API TextPartDictionaryEntry : public LabelDictionaryEntry
		{
		public:
			/**
			 * Constructor.
			 * @param textPart text part referenced by the index entry
			 * @param label label to which the text part belongs
			 * @reqid 001.0064
			 */
			TextPartDictionaryEntry( TextPart* textPart, Label* label )
				: m_textPart( textPart ), LabelDictionaryEntry( label )
			{

			}

			/**
			* @see IndexDictionaryEntry::GetType
			* @reqid 001.0064
			*/
			virtual EntryType GetType() const
			{
				return EntryType_TextPart;
			}

			/**
			 * Get the referenced label
			 * @return the label
			 * @reqid 001.0064
			 */
			const Label* GetLabel() const
			{
				return m_labelRef;
			}

			/**
			* Gets the indexed text part
			* @return the text part
			* @reqid 001.0064
			*/
			TextPart* GetTextPart() const 
			{ 
				return m_textPart;
			}

			/**
			* @see IndexDictionaryEntry::Process
			* @reqid 001.0064
			*/
			void Process( ObstacleDetectionArgs& args, Error& error );

			/**
			 * @see LabelDictionaryEntry::CreateOverlap
			 * @reqid 001.0064
			 */
			void CreateOverlap( TextOverlap** overlap, Error& error );

		private:
			TextPart* m_textPart;
		};

		/**
		* An index dictionary to store a symbol component
		* @reqid 001.0064
		*/
		class LABELINGENGINE_API SymbolComponentDictionaryEntry : public LabelDictionaryEntry
		{
		public:
			/**
			* Constructor.
			* @param symbolComponent symbol component referenced by the index entry
			* @param label label to which the symbol component belongs
			* @reqid 001.0064
			*/
			SymbolComponentDictionaryEntry( SymbolComponent* symbolComponent, Label* label )
				: m_symbolComponent( symbolComponent ), LabelDictionaryEntry( label )
			{

			}

			/**
			* @see IndexDictionaryEntry::GetType
			* @reqid 001.0064
			*/
			virtual EntryType GetType() const
			{
				return EntryType_SymbolComponent;
			}

			/**
			* Get the referenced label
			* @return the label
			* @reqid 001.0064
			*/
			const Label* GetLabel() const
			{
				return m_labelRef;
			}

			/**
			* Gets the indexed symbol component
			* @return the text part
			* @reqid 001.0064
			*/
			SymbolComponent* GetSymbolComponent() const 
			{ 
				return m_symbolComponent;
			}

			/**
			* @see IndexDictionaryEntry::Process
			* @reqid 001.0064
			*/
			void Process( ObstacleDetectionArgs& args, Error& error );

			/**
			* @see LabelDictionaryEntry::CreateOverlap
			* @reqid 001.0064
			*/
			void CreateOverlap( TextOverlap** overlap, Error& error );

		private:
			SymbolComponent* m_symbolComponent;
		};
		/**
		* An index dictionary to store a leader
		* @reqid 001.0064
		*/
		class LABELINGENGINE_API LeaderDictionaryEntry : public IndexDictionaryEntry
		{
		public:
			/**
			* Constructor.
			* @param leader leader referenced by the index entry
			* @param label label to which the leader belongs
			* @reqid 001.0064
			*/
			LeaderDictionaryEntry( Leader* leader, Label* label )
				: m_leader( leader ), m_labelRef( label )
			{
			}

			/**
			* @see IndexDictionaryEntry::GetType
			* @reqid 001.0064
			*/
			virtual EntryType GetType() const
			{
				return EntryType_Leader;
			}

			/**
			* Get the referenced label
			* @return the label
			* @reqid 001.0064
			*/
			const Label* GetLabel() const
			{
				return m_labelRef;
			}

			/**
			* Gets the indexed leader
			* @return the text part
			* @reqid 001.0064
			*/
			Leader* GetLeader() const 
			{ 
				return m_leader;
			}

			/**
			* @see IndexDictionaryEntry::Process
			* @reqid 001.0064
			*/
			void Process( ObstacleDetectionArgs& args, Error& error );

		private:
			Leader* m_leader;
			Label* m_labelRef;
		};
		
		/**
		 * Interface to be implemented by every implementation of a
		 * spatial index.
		 * @reqid 001.0064
		 */
		class LABELINGENGINE_API ISpatialIndex
		{
		public:

#ifdef S11N
			/**
			 * A factory class to instantiate a new Polyline or its derived class.
			 * @reqid N/A.
			 */
			class LABELINGENGINE_API Factory
			{
			public:
				/**
				 * Constructor.
				 * @param numVertices		the number of vertices that constitute the polyline
				 * @param error					an error object to be populated
				 * @reqid 001.0063
				 */
				virtual ISpatialIndex* Create( float minCellSize, float llx, float lly, float width, float height, Error& errorInfo ) = 0;

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
			* Insert a polyline into the spatial index
			* @param polyline the polyline to be inserted into the index.
			* @param feature the feature owning the polyline
			* @param errorInfo
			* @return 0 on success, non-zero error code otherwise
			* @reqid 001.0064
			*/
			virtual void Insert( Polyline& polyline, const Feature& feature, Error &errorInfo ) = 0;

			/**
			 * Insert an opaque polygon into the spatial index.
			 * @param opaquePolygon - the polygon to be inserted.
			 * @param feature - owner of the polygon.
			 * @param errorInfo - [out] error info.
			 * @reqid 001.0064
			 */
			virtual void Insert( const Polygon& opaquePolygon, const Feature& feature, Error &errorInfo ) = 0;

			/**
			 * Insert a text part into the spatial index
			 * @param label the label which owns the indexed text part
			 * @param textPart the text part being indexed
			 * @param error error information
			 * @reqid 001.0064
			 */
			virtual void Insert( Label* label, TextPart* textPart, Error& error ) = 0;

			/**
			* Insert a leader into the spatial index
			* @param label the label which owns the indexed leader
			* @param leader the leader being indexed
			* @param error error information
			* @reqid 001.0064
			*/
			virtual void Insert( Label* label, Leader* leader, Error& error ) = 0;

			/**
			* Insert a symbol component into the spatial index
			* @param label						the label which owns the indexed symbol component
			* @param symbolComponent	the symbol component being indexed
			* @param angle						the orientation of the symbol component
			* @param error						error information
			* @reqid 001.0064
			*/
			virtual void Insert( Label* label, SymbolComponent* symbolComponent, float angle, Error& error ) = 0;
			/**
			 * Given a rectangle call to this method produces a collection of map cell
			 * dictionaries which are the potential overlaps for the rectangle.
			 * @param query rectangle for which potential obstacles should be returned
			 * @param candidates collection of map cell dictionaries
			 * @param errorInfo error information
			 * @reqid 001.0064
			 */
			virtual void GetPotentialOverlaps( const Rectangle& query, LinkedList<IndexDictionaryEntry::Collection>** candidates, Error& errorInfo ) const = 0;

			/**
			 * Virtual destructor to provide cleanup of derived classes.
			 * @reqid 001.0064
			 */
			virtual ~ISpatialIndex()
			{}

			/**
			 * RTTI replacement.
			 * @reqid N/A.
			 */
			virtual const TCHAR* GetIdentity() = 0;
		};
	}
}

#endif //DTP_SPATIALINDEX_H
