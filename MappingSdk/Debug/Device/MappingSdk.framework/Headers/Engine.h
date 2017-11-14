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

// Client application interface to the Labeling Engine

#ifndef DTP_ENGINE_H
#define DTP_ENGINE_H

#include "LabelingEngineDefs.h"

/**************************************************************************/
// For client convenience.  This is the only header that must be #included.
#include <string.h>
#include "Logger.h"
#include "DisplayEnvironment.h"
#include "Configuration.h"
#include "Map.h"
/**************************************************************************/

/**
 * @mainpage
 * <h1>Welcome to the API documentation of %MapText Labeling Engine!</h1>
 * <p>This is the main source of developer's documentation of the engine.</p><p>If you're just starting using 
 * the engine please have a look at the tutorial here: DtpExampleClient.cpp.</p>
 * <p>If you're looking for the answers to some  freqently asked question go here: <a href="../../FAQ/index.html">F.A.Q.</a></p>
 * <p>In case you still need some questions answered please write to us any time at support@maptext.com.</p>
 *
 * <p><b><i>Sincerely yours,<br>
 * The %MapText, Inc. Team.</i></b></p>
 */

namespace MapText
{
	namespace Dtp
	{
		/**
		 * DTP engine initializer. The engine must be initialized using this class before use.
		 */
		class LABELINGENGINE_API LabelingEngine
		{
			public:
				/**
				 * Possible error codes
				 * @reqid 001.0063
				 */
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_LicenseCheckFailed
				};

			private:
				/**
				 * Constructor. Private for utility class.
				 * @reqid 007.0048
				 */
				LabelingEngine()
				{
				}
				/**
				 * Destructor. Private for utility class.
				 * @reqid 007.0048
				 */
				~LabelingEngine() {}

			public:
				/**
				 * Initialize text placement engine.
				 * @param fontNames array of font names that should be preloaded by engine in order to speed up
				 *     the processing. This parameter is optional, if NULL is passed then fonts are lazily 
				 *     loaded on first request.
				 * @param numFonts number of fonts in fontNames array
				 * @param fontHeightMapping The font size given for a character does not necessarily specify the
											distance from the bottom of the character to the top of the character -
											this distance may be smaller or larger than the font size. How a rendering(display)
											system decides how large to actually draw a character based on the given
											font size is determined by the fontHeightMapping. There are several options for this
											some of which are supported in the FontHeightMapping enumeration. Most systems
											use the UseEmSquare option. The EmSquare is a particular property in the font. In
											this case, the font size would be considered equivalent to this value. The other options
											in the enumeration are less common. The criticality of this setting matching the way the
											rendering system handles the font size is based on the fact that this setting also ends
											up controlling the length of text which affects avoiding overlaps.
				 * @param heapSizeInMB currently not used
				 * @param minLevel minimum level of criticality of log messages. Use CRITICALITYLEVEL_NONE for silent mode.
				 *                 Note: in DEBUG mode this param is ignored and the log is alway 
				 *                 created at CRITICALITYLEVEL_INFO
				 * @param error error information
				 * @reqid 007.0048
				 */
				static void Initialize(
					const String* fontNames,
					int numFonts,
					DisplayEnvironment::FontHeightMapping fontHeightMapping,
					int heapSizeInMB, MapText::Dtp::Logger::CriticalityLevel minLevel,
					Error& error,
					Configuration::PlacementPrecedence placementPrecedence =
						Configuration::PlacementPrecedence_ScreenStability );

				/**
				 * Uninitialize engine.
				 * @reqid 007.0048
				 */
				static void Uninitialize();
            
                /**
                 * Check the engine initialization state.
                 * @reqid 007.0048
                 * @return - True if engine is initialized. False otherwise.
                 */
                static bool IsInitialized();
		};
	}
}

#endif // DTP_ENGINE_H
