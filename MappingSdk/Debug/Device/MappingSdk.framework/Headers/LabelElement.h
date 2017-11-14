/**************************************************
 * Copyright (c)2008 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://mtsbs.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/LabelingEngine/LabelElement.h $
 *
 * Revision: $Revision: 15907 $
 *
 * Last modified on: $Date: 2011-03-01 14:00:47 -0500 (Tue, 01 Mar 2011) $, by: $Author: pgrenetz $
 *
 *************************************************/

#ifndef DTP_LABELELEMENT_H
#define DTP_LABELELEMENT_H

#include "LabelingEngineDefs.h"
#include "Collection.h"
#include "MtString.h"

namespace MapText
{
	namespace Dtp
	{
		/**
		 * Specifies to which type of buffer a BufferSizeCommand instance is to be applied.
		 * @reqid 001.0063
		 */
		enum BufferType
		{
			BufferType_TextToText = 0,
			BufferType_TextToPoint,
			BufferType_TextToLine,
			BufferType_TextToFeature,
			BufferType_TextToAll
		};

		/**
		 * Class that represents a label element to be hidden.
		 * @reqid 006.0014
		 */
		class LABELINGENGINE_API LabelElement
		{
			public:
				/**
				* Possible error codes
				* @reqid 006.0014
				*/
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
				};

				/**
				* Possible label element types that can be hidden.
				* @reqid 006.0014
				*/
				enum ElementType
				{
					ElementType_StackLevel,
					ElementType_TextObject,
					ElementType_TextComponent,
					ElementType_SymbolComponent,
				};

				/**
				 * Constructor used for named elements, currently all other than stack level.
				 * @param elementType - the element type of the label element to be hidden.
				 * @param elementName - the identifying name of the label element to be hidden.
				 * @param error - An Error object
				 * @reqid 006.0014
				 */
				LabelElement( ElementType elementType, const TCHAR* elementName, Error& error )
					: m_elementType( elementType )
				{
					m_elementId.AssignString( elementName, error );
				}

				/**
				 * Constructor used for a stack level.
				 * @param elementType - the element type of the label element to be hidden.
				 * @param elementNumber - the identifying number of the label element to be hidden.
				 * @param error - An Error object
				 * @reqid 006.0014
				 */
				LabelElement( ElementType elementType, int elementNumber, Error& error )
					: m_elementType( elementType )
				{
					TCHAR elementId[128];
					elementId[127] = _T('\0'); // for the however unlikely case
					_sntprintf( elementId, 127, _T("%d"), elementNumber );
					m_elementId.AssignString( elementId, error );
				}

				/**
				 * Destructor.
				 * @reqid 006.0014
				 */
				~LabelElement()
				{
				}

				/**
				 * Get the element type of the label element to be hidden.
				 * @return the label element type
				 * @reqid 006.0014
				 */
				ElementType GetElementType() const
				{
					return m_elementType;
				}

				/**
				 * Get the element identifier of the label element to be hidden.
				 * @return the label element identifier
				 * @reqid 006.0014
				 */
				const String* GetElementID() const
				{
					return &m_elementId;
				}

				/**
				 * Creates deep copy of self.
				 * @param error an error object to be populated
				 * @return copy of self
				 * @reqid 006.0014
				 */
				virtual LabelElement* Clone( Error& error ) const
				{
					LabelElement* copy =
						new LabelElement( m_elementType, m_elementId.GetString(), error );

					if ( error )
					{
						return NULL;
					}

					if ( copy == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}
					else
					{
						copy->m_elementType = m_elementType;
						copy->m_elementId.AssignString( m_elementId.GetString(), error );
						if (error)
						{
							delete copy;
							copy = NULL;
						}
					}
					return copy;					
				}

#ifdef S11N
				/**
				 * Serialization.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				void Serialize( ISerializer& emitter, Error& error ) const
				{
					SERIALIZE_SCALAR( m_elementType, emitter, error, );
					SERIALIZE_OBJECT( m_elementId, String, emitter, error, );
				}

				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static LabelElement* DeserializeNew( IDeserializer& deserializer, Error& error )
				{
					LabelElement* newLabelElement = new LabelElement();
					if ( newLabelElement == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
						return NULL;
					}

					newLabelElement->Deserialize( deserializer, error );
					if ( error )
					{
						delete newLabelElement;
						return NULL;
					}

					return newLabelElement;
				}

				/**
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				void Deserialize( IDeserializer& deserializer, Error& error )
				{
					DESERIALIZE_ENUM( m_elementType, deserializer, error, );
					DESERIALIZE_OBJECT( m_elementId, String, deserializer, error, );
				}

			private:
				/**
				 * Private default constructor for the need of deserialization
				 * @return -
				 * @reqid 006.0014
				 */
				LabelElement()
				{
				}
#endif

			private:
				ElementType m_elementType;
				String m_elementId;

		};

		/**
		 * Class that represents a collection of label elements to be hidden on a given attempt.
		 * @reqid 006.0014
		 */
		class LABELINGENGINE_API HideAttempt : public ClonableLinkedList<LabelElement, HideAttempt>
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 006.0014
				 */
				HideAttempt()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 006.0014
				 */
				~HideAttempt()
				{
				}

#ifdef S11N
				/**
				 * Serialization.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				void Serialize( ISerializer& emitter, Error& error ) const
				{
					const HideAttempt& This = *this;
					SERIALIZE_LIST( This, LabelElement, emitter, error, );
				}

				/**
				 * Deserializes a new object from the deserializer.
				 * @param deserializer - The source.
				 * @param output - Deserialized object if no error occured. NULL otherwise.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				static HideAttempt* DeserializeNew( IDeserializer& deserializer, Error& error )
				{
					HideAttempt* newHideAttempt = new HideAttempt();
					if ( newHideAttempt == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
						return NULL;
					}

					newHideAttempt->Deserialize( deserializer, error );
					if ( error )
					{
						delete newHideAttempt;
						return NULL;
					}

					return newHideAttempt;
				}

				/**
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				void Deserialize( IDeserializer& deserializer, Error& error )
				{
					HideAttempt& This = *this;
					DESERIALIZE_LIST( This, LabelElement, HideAttempt, deserializer, error, );
				}
#endif
		};

		/**
		 * Class that represents a collection of hide attempts to be tried sequentially as needed.
		 * @reqid 006.0014
		 */
		class LABELINGENGINE_API HideAttempts : public ClonableLinkedList<HideAttempt, HideAttempts>
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 006.0014
				 */
				HideAttempts()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 006.0014
				 */
				~HideAttempts()
				{
				}
		};

		/**
		 * Class that holds the font size step and minimum font size in absolute units
		 * for a text object.
		 * @reqid 006.0088
		 */
		class LABELINGENGINE_API FontReductionGuide
		{
			public:
				/**
				 * Default constructor.
				 * @reqid 006.0088
				 */
				FontReductionGuide()
					: m_minFontSize(0.0f), m_fontSizeStep(0.0f)
				{}

				/**
				 * Get the minimum font size allowed in absolute units for a particular text object.
				 * @return the minimum font size allowed in absolute units for a particular text object.
				 * @reqid 006.0088
				 */
				float GetMinFontSize() const
				{
					return m_minFontSize;
				}

				/**
				 * Set the minimum font size allowed in absolute units for a particular text object.
				 * @see GetMinFontSize
				 * @reqid 006.0088
				 */
				void SetMinFontSize( float minFontSize )
				{
					m_minFontSize = minFontSize;
				}

				/**
				 * Get the font size step in absolute units for a particular text object.
				 * @return the font size step in absolute units for a particular text object.
				 * @reqid 006.0088
				 */
				float GetFontSizeStep() const
				{
					return m_fontSizeStep;
				}

				/**
				 * Set the font size step in absolute units for a particular text object.
				 * @see GetFontSizeStep
				 * @reqid 006.0088
				 */
				void SetFontSizeStep( float fontSizeStep )
				{
					m_fontSizeStep = fontSizeStep;
				}

			private:
				float m_minFontSize;
				float m_fontSizeStep;
		};

		/**
		 * Class that holds the buffer size step and minimum buffer size in absolute units
		 * for each buffer type.
		 * @reqid 006.0088
		 */
		class LABELINGENGINE_API BufferReductionGuide
		{
			public:
				/**
				 * Default constructor.
				 * @reqid 006.0089
				 */
				BufferReductionGuide()
					:	m_minTextToTextBufferSize(0.0f),
						m_minTextToLineBufferSize(0.0f),
						m_minTextToPointBufferSize(0.0f),
						m_textToTextBufferSizeStep(0.0f),
						m_textToLineBufferSizeStep(0.0f),
						m_textToPointBufferSizeStep(0.0f)
				{}

				/**
				 * Get the minimum buffer size allowed in absolute units.
				 * @param bufferType	- the buffer type requested
				 * @return the specified minimum buffer size in absolute units
				 * @reqid 006.0089
				 */
				float GetMinBufferSize( BufferType bufferType ) const
				{
					float minBufferSize = 0.0f;

					switch( bufferType )
					{
					case BufferType_TextToText:
						minBufferSize = m_minTextToTextBufferSize;
						break;

					case BufferType_TextToLine:
						minBufferSize = m_minTextToLineBufferSize;
						break;

					case BufferType_TextToPoint:
						minBufferSize = m_minTextToPointBufferSize;
						break;
					}
					return minBufferSize;
				}

				/**
				 * Set the minimum buffer size allowed in absolute units.
				 * @param bufferType		- the buffer type specified
				 * @param minBufferSize	- the minimum allowed buffer size of the specified type
				 * @see GetMinBufferSize
				 * @reqid 006.0089
				 */
				void SetMinBufferSize( BufferType bufferType, float minBufferSize )
				{
					assert( minBufferSize >= 0.0f );

					switch( bufferType )
					{
					case BufferType_TextToText:
						m_minTextToTextBufferSize = minBufferSize;
						break;

					case BufferType_TextToLine:
						m_minTextToLineBufferSize = minBufferSize;
						break;

					case BufferType_TextToPoint:
						m_minTextToPointBufferSize = minBufferSize;
						break;
					}
				}

				/**
				 * Get the buffer size step in absolute units.
				 * @param bufferType	- the buffer type requested
				 * @return the specified buffer size step in absolute units.
				 * @reqid 006.0089
				 */
				float GetBufferSizeStep( BufferType bufferType ) const
				{
					float bufferSizeStep = 0.0f;

					switch( bufferType )
					{
					case BufferType_TextToText:
						bufferSizeStep = m_textToTextBufferSizeStep;
						break;

					case BufferType_TextToLine:
						bufferSizeStep = m_textToLineBufferSizeStep;
						break;

					case BufferType_TextToPoint:
						bufferSizeStep = m_textToPointBufferSizeStep;
						break;
					}
					return bufferSizeStep;
				}

				/**
				 * Set the buffer size step in absolute units.
				 * @param bufferType			- the buffer type specified
				 * @param bufferSizeStep	- the buffer size step of the specified type
				 * @see GetBufferSizeStep
				 * @reqid 006.0089
				 */
				void SetBufferSizeStep( BufferType bufferType, float bufferSizeStep )
				{
					assert( bufferSizeStep >= 0.0f );

					switch( bufferType )
					{
					case BufferType_TextToText:
						m_textToTextBufferSizeStep = bufferSizeStep;
						break;

					case BufferType_TextToLine:
						m_textToLineBufferSizeStep = bufferSizeStep;
						break;

					case BufferType_TextToPoint:
						m_textToPointBufferSizeStep = bufferSizeStep;
						break;
					}
				}

				static void Copy( const BufferReductionGuide& src, BufferReductionGuide& dest )
				{
					dest.m_minTextToTextBufferSize = src.m_minTextToTextBufferSize;
					dest.m_minTextToLineBufferSize = src.m_minTextToLineBufferSize;
					dest.m_minTextToPointBufferSize = src.m_minTextToPointBufferSize;

					dest.m_textToTextBufferSizeStep = src.m_textToTextBufferSizeStep;
					dest.m_textToLineBufferSizeStep = src.m_textToLineBufferSizeStep;
					dest.m_textToPointBufferSizeStep = src.m_textToPointBufferSizeStep;
				}

			private:
				float m_minTextToTextBufferSize;
				float m_minTextToLineBufferSize;
				float m_minTextToPointBufferSize;

				float m_textToTextBufferSizeStep;
				float m_textToLineBufferSizeStep;
				float m_textToPointBufferSizeStep;
		};
	} // namespace Dtp
} // namespace MapText

#endif	// DTP_LABELELEMENT_H
