/**************************************************
 * Copyright (c)2008 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL$
 *
 * Revision: $Revision$
 *
 * Last modified on: $Date$, by: $Author$
 *
 *************************************************/

#ifndef DTP_DISPLAYENVIRONMENT_H
#define DTP_DISPLAYENVIRONMENT_H

#include "GenericTextProcessor.h"
#include "Error.h"

namespace MapText
{
	namespace Dtp
	{
		/**
		 * Class that represents the rendering platform and its applicable characteristics.
		 * @reqid 001.0067
		 */
		class UNICODETEXTPROCESSOR_API DisplayEnvironment
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0067
				 */
				enum ErrorCode
				{
					ErrorCode_FontHeightMappingNotSet = 1,
				};

				/**
				 * possible rendering platforms
				 * @reqid 001.0067
				 */
				enum FontHeightMapping
				{
					FontHeightMapping_Unknown = -1,
					FontHeightMapping_UseWinAscender = 0,
					FontHeightMapping_UseEmSquare,
					FontHeightMapping_UseWinAscenderPlusDescender,
					FontHeightMapping_UseAscender
				};

			private:
				/**
				 * Default constructor.
				 * @reqid 001.0067
				 */
				DisplayEnvironment( void )
				{
				}

			public:
				/**
				 * Destructor.
				 * @reqid 001.0067
				 */
				~DisplayEnvironment( void ){}

			public:
				/**
				 * Retrieve the identifier of the rendering platform currently being targeted.
				 * @return the current FontHeightMapping enumerated value
				 * @reqid 001.0067
				 */
				static FontHeightMapping GetFontHeightMapping()
				{
					return s_fontHeightMapping;
				}

				/**
				 * @see GetFontHeightMapping
				 * @param error - an Error object
				 * @reqid 001.0067
				 */
				static void SetFontHeightMapping( FontHeightMapping fontHeightMapping, Error& error )
				{
					if ( fontHeightMapping == FontHeightMapping_Unknown )
					{
						SET_ERROR(
							error,
							ErrorCode_FontHeightMappingNotSet,
							_T("The font height mapping must be set to a \'known\' value."));

						return;
					}

					s_fontHeightMapping = fontHeightMapping;
				}

			public:
				static FontHeightMapping s_fontHeightMapping;
		};
	}
}

#endif // DTP_DISPLAYENVIRONMENT_H
