/**************************************************
 * Copyright (c)2009 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://martin@mtsrv.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/MapTextTypes/Graph.h $
 * 
 * Revision: $Revision: 29016 $
 *
 * Last modified on: $Date: 2012-01-27 08:52:20 -0500 (Fri, 27 Jan 2012) $, by: $Author: mfraczak $
 * 
 *************************************************/

#ifndef DTP_GRAPH_H
#define DTP_GRAPH_H

#include "Collection.h"

namespace MapText
{
	namespace Dtp
	{
		/**
		 * A pair of two elements. Elements have to implement a copy constructor as
		 * a copy of them is made in creation of the pair.
		 * @param C1 - first class of the pair.
		 * @param C2 - second class of the pair.
		 */
		template< class C1, class C2>
		class Pair
		{
		public:

			/**
			 * Constructor. 
			 * @param first - first element of the pair. A copy of the element is made.
			 * @param second - first element of the pair. A copy of the element is made.
			 * @reqid ????.????
			 */
			Pair( const C1& first, const C2& second )
				: m_first( first ), m_second( second )
			{				
			}

			/**
			 * Destructor.
			 * @reqid ????.????
			 */
			~Pair()
			{
			}

			/**
			 * Get the first element of the pair.
			 * @return - first element
			 * @reqid ????.????
			 */
			C1 GetFirst()
			{
				return m_first;
			}

			/**
			 * Get the second element of the pair.
			 * @return - second element
			 * @reqid ????.????
			 */
			C2 GetSecond()
			{
				return m_second;
			}
		private:
			C1 m_first;
			C2 m_second;
		};

		/**
		 * Universal graph represented as adjacency list.
		 * @param Content - type of elements being stored in the nodes.
		 * @param EdgeAttribute - type of element related to each edge in the graph.
		 */
		template< class Content, class EdgeAttribute >
		class Graph
		{

		public:

			class Node;

			/**
			 * List of nodes.
			 */
			class Nodes : public LinkedList< Node >
			{
			public:
				/**
				 * Constructor. Compiler generated code avoidance.
				 * @reqid ????.????
				 */
				Nodes()
					: LinkedList< Node >( true )
				{
				}

				/**
				 * Destructor. Compiler generated code avoidance.
				 * @reqid ????.????
				 */
				~Nodes()
				{
				}
			};

			/**
			 * Edge of the graph - a directed connection. It stores the destination node of it
			 * plus the custom attribute related to the edge itself. It should be nested in the
			 * collection of adjacent nodes of the source node.
			 */
			class Edge : public Pair<Node*,EdgeAttribute>
			{
			public:
				/**
				 * Constructor.
				 * @param node - target node of the edge. Ownership is not taken.
				 * @param edgeAttribute - custom edge attribute.
				 * @reqid ????.????
				 */
				Edge( Node* node, EdgeAttribute edgeAttribute )
					: Pair<Node*,EdgeAttribute>( node, edgeAttribute )
				{
				}

				~Edge()
				{
				}
			};

			/**
			 * List of edges.
			 */
			class Edges : public LinkedList<Edge>
			{
			public:

				/**
				 * Constructor. Compiler generated code avoidance.
				 * @reqid ????.????
				 */
				Edges() : LinkedList<Edge>(true)
				{
				}

				/**
				 * Destructor. Compiler generated code avoidance.
				 * @reqid ????.????
				 */
				~Edges()
				{
				}
			};

			/**
			 * Node of the graph. It stores the content plus list of edges originating from it.
			 */
			class Node
			{
				friend class Graph;
			public:

				/**
				 * Destructor.  Compiler generated code avoidance.
				 * @reqid ????.????
				 */
				~Node()
				{
				}

				/**
				 * Add an edge originating from this node.
				 * @param toNode - target node
				 * @param edgeAttribute - custom edge attribute
				 * @param error - [out] error info
				 * @reqid ????.????
				 */
				void AddEdge( Node& toNode, EdgeAttribute edgeAttribute, Error& error )
				{
					Edge* edge = new Edge( &toNode, edgeAttribute );
					if ( edge == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
						return;
					}

					m_adjacentNodes.AddElement( edge, error );
				}

				/**
				 * Get the list of edges originating from this node.
				 * @return - list of edges
				 * @reqid ????.????
				 */
				Edges* GetAdjacentNodes()
				{
					return &m_adjacentNodes;
				}

				/**
				 * Get the content of this node.
				 * @return - content.
				 * @reqid ????.????
				 */
				Content* GetContent()
				{
					return m_content;
				}

			private:
				Content* m_content;
				Edges m_adjacentNodes;

				Node( Content& content )
					: m_content( &content )
				{
				}
			};

			enum ErrorCode
			{
				ErrorCode_AllocationFailure = 1,

			};

			/**
			 * Constructor. Compiler generated code avoidance.
			 * @reqid ????.????
			 */
			Graph()
			{
			}

			/**
			 * Destructor. Compiler generated code avoidance.
			 * @reqid ????.????
			 */
			~Graph()
			{
			}

			/**
			 * Get the node with the content provided. Content type has to implement the equality
			 * operator '==' in order to find the existing node. If node with the content provided
			 * is not found in the graph a new node is introduced into the graph and returned 
			 * to the caller.
			 * @param element - content of the desired node.
			 * @param node - [out] node with the requested content.
			 * @param error - [out] error info.
			 * @reqid ????.????
			 */
			void GetNode( Content& element, Node** node, Error& error )
			{
				assert( node != NULL );
				*node = NULL;

				Node* nodePtr = NULL;
				for ( typename Nodes::Iterator n = m_nodes.Begin(); !n.IsNull(); n++ )
				{
					if ( n.GetElement()->GetContent() == &element )
					{
						nodePtr = n;
					}
				}

				if ( nodePtr == NULL )
				{
					AddNode( element, &nodePtr, error );
					if ( error )
					{
						return;
					}
					assert( nodePtr != NULL );
				}

				*node = nodePtr;
			}

			/**
			 * Introduce a new edge into the graph.
			 * @param fromNode - edge source.
			 * @param toNode - edge target.
			 * @param edgeAttribute - custom attribute of the new edge.
			 * @param error - [out] error info
			 * @reqid ????.????
			 */
			void AddEdge( Node& fromNode, Node& toNode, EdgeAttribute edgeAttribute, Error& error )
			{
				fromNode.AddEdge( toNode, edgeAttribute, error );
			}

			/**
			* Gets Nodes
			* @return - Nodes
			* @reqid ????.????
			*/
			typename Graph<Content, EdgeAttribute>::Nodes* GetNodes()
			{ 
				return &m_nodes;
			}

		private:
			Nodes m_nodes;

			/**
			 * Add a new node to the graph
			 * @param element - content of the node
			 * @param node - [out] new node
			 * @param error - [out] error info
			 * @reqid ????.????
			 */
			void AddNode( Content& element, Node** node, Error& error )
			{
				Node* n = new Node( element );
				if ( n == NULL )
				{
					SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
					return;
				}

				m_nodes.AddElement( n, error );

				if ( error )
				{
					delete n;
					return;
				}

				*node = n;
			}

		};

	} // namespace Dtp
} // namespace MapText

#endif // DTP_GRAPH_H