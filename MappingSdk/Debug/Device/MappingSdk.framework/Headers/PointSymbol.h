/**************************************************
 * Copyright (c)2008 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://mtsbs.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/LabelingEngine/PointSymbol.h $
 *
 * Revision: $Revision: 816 $
 *
 * Last modified on: $Date: 2009-02-06 17:46:38 -0500 (Fri, 06 Feb 2009) $, by: $Author: mfraczak $
 *
 *************************************************/

#ifndef DTP_POINTSYMBOL_H
#define DTP_POINTSYMBOL_H

#include "LabelingEngineDefs.h"

#include "Collection.h"
#include "Error.h"
#include "MtString.h"
#include "MtGeometry.h"

#include <float.h>

namespace MapText
{
	namespace Dtp
	{
		/**
		 * A class that holds the polygons defining a symbol's outline and convex hull.
		 * It can be cloned for ownership be a point shape and translated to the
		 * feature's location.
		 * @author Phil Grenetz
		 * @reqid 001.0063
		 */
		class LABELINGENGINE_API PointSymbol
		{
			public:
				/**
				* Possible error codes
				* @reqid 001.0063
				*/
				enum ErrorCode
				{
					ErrorCode_NoVerticesPresent = 1,
					ErrorCode_AllocationFailure
				};

				/**
				 * A class that holds a cache collection of
				 * untranslated point symbol outlines for the map.
				 * @author Phil Grenetz
				 * @reqid 001.0063
				 */
				class LABELINGENGINE_API Collection
					: public StringKeyedTree < PointSymbol >
				{
				public:
						/**
						 * Possible error codes
						 * @reqid 001.0063
						 */
						enum ErrorCode
						{
							ErrorCode_AllocationFailure = 1,
						};

					/**
					 * Default constructor implemented to avoid compiler generated object code.
					 * @reqid 001.0063
					 */
					Collection()
					{
					}

					/**
					 * Destructor implemented to avoid compiler generated object code.
					 * @reqid 001.0063
					 */
					~Collection()
					{
					}

					/**
					 * Retrieve specified point symbol from cache if present.
					 * @param name  the name assigned on creation by client
					 * @return the named point symbol if present; else NULL
					 * @reqid 001.0063
					 */
					const PointSymbol* GetPointSymbol( const TCHAR* name ) const;

					/**
					 * Create a point symbol and add to cache if not present.
					 * @param name					the name to assign to the new point symbol
					 * @param pointSymbol		the new named point symbol if not present in cache; else NULL
					 * @param error					an Error object
					 * @reqid 001.0063
					 */
					void CreatePointSymbol(
						const TCHAR*  name,
						PointSymbol** pointSymbol, Error& error );

					void AddPointSymbol(
						const TCHAR* name,
						PointSymbol* pointSymbol, Error& error);
				};

				/**
				 * A class that holds a cache of ancillary geometric properties.
				 * The cache is invalidated by calls to PointSymbol methods that
				 * modify the outline: Translate, Rotate, Scale. Getters refresh
				 * the ancillary geometric properties if invalid.
				 *
				 * A client is discouraged from modifying the outline by the
				 * const pointer returned by GetOutline.  If a client converts
				 * this pointer to a mutable one, changes to the outline,
				 * other than those achieved via PointSymbol methods, should
				 * be followed by a call to the PointSymbol method SetOutline.
				 *
				 * @author Phil Grenetz
				 * @reqid 001.0063
				 */
				class LABELINGENGINE_API GeometricProperties
				{
					public:
						/**
					   * Default constructor.
					   * @reqid 001.0063
					   */
						GeometricProperties()
						:	m_polygons( NULL ),
							m_convexHull( NULL ),
							m_center( -FLT_MAX, -FLT_MAX ),
							m_boundingBox( 0, 0, 0, 0 ),
							m_valid( false )						{
						}

						/**
						 * Destructor.
						 * @reqid 001.0063
						 */
						~GeometricProperties()
						{
							delete m_convexHull;
						}

						/**
						 * Assign a reference to the outline of the point symbol.
						 * @param polygons		a reference to the outline of the client point symbol
						 * @reqid 001.0063
						 */
						void SetClientOutline( Polygons* polygons, Error& error )
						{
							assert( polygons != NULL);

							Invalidate();
							m_polygons = polygons;
							RefreshState(error);
						}

						/**
						 * Shift the cached geometric properties in the x and y directions.
						 * @param dx the horizontal shift
						 * @param dy the vertical shift
						 * @reqid 001.0063
						 */
						void Translate( float dx, float dy )
						{
							m_convexHull->Translate( dx, dy );
							m_center.Translate( dx, dy );
							UpdatePolygonBoundingBoxes();
						}

						/**
						 * Rotate the cached geometric properties about the specified origin.
						 * @param origin		the point about which rotation is requested
						 * @param sin_t		the sine of the angle through which rotation is requested
						 * @param cos_t		the cosine of the angle through which rotation is requested
						 * @reqid 001.0063
						 */
						void Rotate( const Point& origin, float sin_t, float cos_t )
						{
							m_convexHull->Rotate( origin, sin_t, cos_t );
							m_center.Rotate( origin, sin_t, cos_t );
							UpdatePolygonBoundingBoxes();
						}

						/**
						 * Get the convex hull enclosing the client symbol.
						 * @return the address of the convex hull polyline enclosing the symbol.
						 * @reqid 001.0063
						 */
						Polyline* GetConvexHull() const
						{
							return m_convexHull;
						}

						/**
						 * Get the point representing the center of the client point symbol.
						 * @param symbolCenter	Address of the symbol center point to be populated
						 * @reqid 001.0063
						 */
						void GetCenter( const Point** symbolCenter ) const
						{
							*symbolCenter = &m_center;
						}

						/**
						 * Get the coordinates of the bounding box of the point symbol.
						 * @param llx		Address of the lower left corner x coordinate to be populated
						 * @param lly		Address of the lower left corner y coordinate to be populated
						 * @param urx		Address of the upper right corner x coordinate to be populated
						 * @param ury		Address of the upper right corner y coordinate to be populated
						 * 
						 * @reqid 001.0063
						 */
						void GetBoundingBox( float* llx, float* lly, float* urx, float* ury )
						{
							*llx = m_boundingBox.GetLowerLeft()->GetX();
							*lly = m_boundingBox.GetLowerLeft()->GetY();

							*urx = m_boundingBox.GetUpperRight()->GetX();
							*ury = m_boundingBox.GetUpperRight()->GetY();
						}

						void GetBoundingBox( UprightRectangle& bbox )
						{
							bbox = m_boundingBox;
						}

					private:
						/**
						 * Invalidate the state of the property cache.
						 * Getters will detect the invalid state and refresh the cache.
						 * @reqid 001.0063
						 */
						void Invalidate()
						{
							// Clear the outline. Do NOT DELETE. It is a reference to the client's outline.
							m_polygons = NULL;

							delete m_convexHull;
							m_convexHull = NULL;

							m_center.SetXY( -FLT_MAX, -FLT_MAX );

							m_boundingBox.SetLowerLeft( 0, 0 );
							m_boundingBox.SetUpperRight( 0, 0 );

							m_valid = false;
						}

						/**
						 * Regenerate the geometric properties to reflect the current outline.
						 * @param error					an Error object
						 * @reqid 001.0063
						 */
						void RefreshState( Error& error )
						{
							GenerateConvexHull( error );
							if ( error )
							{
								return;
							}

							// Update all polygon bounding boxes and the symbol bounding box.
							UpdatePolygonBoundingBoxes();

							// Compute the center
							float llx, lly, urx, ury;
							GetBoundingBox(&llx, &lly, &urx, &ury);

							m_center.SetX( ( llx + urx ) * .5f );
							m_center.SetY( ( lly + ury ) * .5f );

							m_valid= true;
						}

						/**
						* Generate the convex hull enclosing the client symbol outline's vertices.
						* This is invoked internally only.
						* @param error An Error object to be populated
						* @reqid 001.0063
						*/
						void GenerateConvexHull(Error& error)
						{
							// Count total number of vertices in outline.
							int numVertices = 0;

							Polyline* boundary = NULL;

							Polygon* polygon = m_polygons->GetFirstElement();
							while (polygon != NULL)
							{
								boundary = polygon->GetBoundary();
								if ((boundary == NULL) || boundary->GetNumVertices() == 0)
								{
									SET_ERROR(error, ErrorCode_NoVerticesPresent,
										_T("Polygon has no boundary or one with no vertices."));

									return;
								}
								numVertices += boundary->GetNumVertices();
								polygon = m_polygons->GetNextElement();
							}

							m_polygons->GenerateConvexHull(numVertices, &m_convexHull, error);
						}

						/**
						 * Update the bounding box of all the polygons of the symbol
						 * @reqid 002.0036
						 */
						void UpdatePolygonBoundingBoxes()
						{
							Polygon *polygon = m_polygons->GetFirstElement();

							polygon->GetBoundary()->UpdateBoundingBox();

							polygon = m_polygons->GetNextElement();
							while ( polygon )
							{
								polygon->GetBoundary()->UpdateBoundingBox();

								polygon = m_polygons->GetNextElement();
							}

							ComputeSymbolBoundingBox();
						}

						/**
						 * Update the bounding box of the symbol itself
						 * @reqid 002.0036
						 */
						void ComputeSymbolBoundingBox()
						{
							Polygon *polygon = m_polygons->GetFirstElement();

							const UprightRectangle* polygonBoundingBox =
								polygon->GetBoundingBox();

							assert( polygonBoundingBox != NULL );

							m_boundingBox.SetLowerLeft(
								polygonBoundingBox->GetLowerLeft()->GetX(),
								polygonBoundingBox->GetLowerLeft()->GetY() );

							m_boundingBox.SetUpperRight(
								polygonBoundingBox->GetUpperRight()->GetX(),
								polygonBoundingBox->GetUpperRight()->GetY() );

							polygon = m_polygons->GetNextElement();
							while ( polygon )
							{
								float nextllx, nextlly, nexturx, nextury;

								polygon->GetBoundingBox(
									&nextllx, &nextlly, &nexturx, &nextury);

								m_boundingBox.Grow( Point( nextllx, nextlly ) );
								m_boundingBox.Grow( Point( nexturx, nextury ) );

								polygon = m_polygons->GetNextElement();
							}
						}

					private:
						/**
						 * The Polygons member is a symbol outline reference
						 * passed by the client PointSymbol instance.
						 * @reqid 001.0063
						 */
						Polygons* m_polygons;

						Polyline* m_convexHull;
						Point m_center;
						UprightRectangle m_boundingBox;
						bool m_valid;
				};

		private:
			/**
			 * Default constructor.
			 * @reqid 001.0063
			 */
			PointSymbol();

			/**
			 * Constructor.
			 * @param name	the name by which to reference the point symbol
			 * @param error	an Error object
			 * @reqid 001.0063
			 */
			PointSymbol( const TCHAR* name, Error& error );

		public:
			/**
			 * Destructor.
			 * @reqid 001.0063
			 */
			virtual ~PointSymbol();

			/**
			 * Create a deep copy of the object.
			 * @param error	an Error object
			 * @return the cloned instance
			 * @reqid 001.0063
			 */
			virtual PointSymbol* Clone(Error& error) const
			{
				PointSymbol* clone = new PointSymbol;
				if (clone == NULL)
				{
					SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
				}
				else
				{
					Copy( *this, *clone, error );
					if ( error )
					{
						delete clone;
						return NULL;
					}
				}
				return clone;
			}

			/**
			 * Shift the polygons in the x and y directions.
			 * @param dx the horizontal shift
			 * @param dy the vertical shift
			 * @reqid 001.0063
			 */
			void Translate( float dx, float dy );

			/**
			* Rotate the symbol outline about the specified origin.
			* @param origin		the point about which rotation is requested
			* @param sin_t		the sine of the angle through which rotation is requested
			* @param cos_t		the cosine of the angle through which rotation is requested
			* @reqid 001.0063
			*/
			void Rotate( const Point& origin, float sin_t, float cos_t );

			/**
			 * Scale the coordinates of all vertices by specified factors.
			 * This operation assumes the vertices of the polylines are
			 * relative to the origin.
			 * @param	horzScaleFactor		the factor by which to multiply the x coordinate
			 * @param	vertScaleFactor		the factor by which to multiply the y coordinate
			 * @param error		an Error object
			 * @reqid 001.0063
			 */
			void Scale(float horzScaleFactor, float vertScaleFactor, Error& error);

			/**
			 * Retrieve the name of the point symbol.
			 * @return the name assigned to the point symbol..
			 * @reqid 001.0063
			 */
			const SharedString* GetName() const
			{
				return &m_name;
			}

			/*******************************************************************
			For classes whose instances are maintained in a keyed collection,
			there are no setters for the properties that constitute the key.
			They are generally not be needed as the key values should be immutable.
			Invoking them would not remove and re-insert the instance in the
			string keyed collection to reflect the new key.

			In this case, however, cloning is required by the API design.
			We clone the "master" point symbol to position an instance of it
			at the coordinates of each feature symbolized with it.

			Clone calls Copy, which calls SetName.  This is OK because it is not
			changing the name of the master point symbol instance. It is only
			setting the name of the positioned clone to that of the master instance.

			It is understood that when calling GetPointSymbol, the keyed collection
			will return the master point symbol instance.

			To prevent exposing this setter to client code, it is marked private.
			*******************************************************************/
private:
			/**
			 * Assign the name of the point symbol.
			 * @param name		the name by which to reference the point symbol
			 * @param error		an Error object
			 * @reqid 001.0063
			 */
			void SetName(const TCHAR* name, Error& error)
			{
				m_name.AssignString(name, error);

                std::string pointSymbolName = m_name.GetString();
                pointSymbolName = pointSymbolName.substr(0, pointSymbolName.find(","));
			}

public:
			/**
			 * Retrieve the polygons collection.
			 * @return a pointer to the polygons collection.
			 * @reqid 001.0063
			 */
			const Polygons* GetOutline() const
			{
				return m_polygons;
			}

			/**
			 * Assign or reassign the polygons collection contents.
			 * Takes ownership of the collection.
			 * Client must not delete the collection or its contents.
			 * @param polygons		the polygons to be assigned
			 * @param error				an Error object
			 * @reqid 001.0063
			 */
			void SetOutline(Polygons* polygons, Error& error);

			/**
			 * Invalidate the state of the property cache.
			 * Getters will detect the invalid state and refresh the cache.
			 * @reqid 001.0063
			 */
			void Invalidate();

			/**
			 * Get the convex hull enclosing the symbol.
			 * @return the address of the convex hull polyline enclosing the symbol.
			 * @reqid 001.0063
			 */
			Polyline* GetConvexHull() const;

			/**
			* Get the coordinates of the bounding box of the point symbol.
			* @param llx		Address of the lower left corner x coordinate to be populated
			* @param lly		Address of the lower left corner y coordinate to be populated
			* @param urx		Address of the upper right corner x coordinate to be populated
			* @param ury		Address of the upper right corner y coordinate to be populated
			* @reqid 001.0063
			*/
			void GetBoundingBox(float* llx, float* lly, float* urx, float* ury);

			/**
			* Populate the minimum bounding upright rectangle.
			* @param bbox		Reference to the upright rectangle to be populated
			* @reqid 001.0063
			*/
			void GetBoundingBox( UprightRectangle& bbox );

			/**
			* Get the point representing the center of the point symbol.
			* @param symbolCenter		Address of the symbol center point to be populated
			* @reqid 001.0063
			*/
			void GetCenter( const Point** symbolCenter ) const;

			/**
			* Finds the point on the boundary intersected by the line segment formed by
			* an external point and the symbol center.
			* @param externalPoint				endpoint of the intersecting line segment outside convex hull
			* @param intersection					[out] point of intersection, to be populated
			* @param vecOutwardSegment0		[out] first adjoining segment's vector outward from intersection point
			* @param vecOutwardSegment1		[out] other adjoining segment's vector outward from intersection point
			*/
			bool FindIntersectionWithExternalPoint(
				const Point& externalPoint,
				Point& intersection,
				int& segmentIndex ) const;
			             
            /**
             * Get name from point symbol name string
             */
            std::string GetPointSymbolName();

		protected:
			/**
			 * Make a deep copy of one instance's members to the other.
			 * @reqid 001.0063
			 */
			static void Copy( const PointSymbol& src, PointSymbol& dest, Error& error );

		private:

#ifdef S11N
		public:
			/**
			 * Serialization - derived class specific.
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
			static PointSymbol* DeserializeNew( IDeserializer& deserializer, Error& error );
#endif

			/****************************************************************************************
			THIS CLASS IS CLONABLE. IF YOU ADD A DATA MEMBER, YOU SHOULD UPDATE THE CLONE OR COPY METHOD
			TO REFLECT THE NEW MEMBER VARIABLE.  FAILURE TO DO SO MAY RESULT IN UNPREDICTABLE BEHAVIOR!!
			****************************************************************************************/
		protected:
			SharedString m_name;
			Polygons* m_polygons;
			GeometricProperties m_geometricProperties;

		private:
		};
	} // namespace Dtp
} // namespace MapText

#endif // DTP_POINTSYMBOL_H
