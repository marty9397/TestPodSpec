///////////////////////////////////////////////////////////
//  IFeaturePredicate.h
//  Implementation of the Class IFeaturePredicate
//  Created on:      26-Dec-2012 16:50:31
///////////////////////////////////////////////////////////

#if !defined(EA_861998CD_8596_415f_98FE_15AA2B2E2949__INCLUDED_)
#define EA_861998CD_8596_415f_98FE_15AA2B2E2949__INCLUDED_

#include "Feature.h"

namespace MapText
{
namespace Dtp
{
	/**
	 * Feature predicate is an abstract base class for classes which will implement
	 * testing certain characteristics of the features. Based on that evaluation
	 * features may get processed differently in other parts of the engine without the
	 * rules being hardcoded (dependency injection pattern).
	 */
	class IFeaturePredicate
	{

	public:

		/**
		 * Evaluates a feature and returns true if the predicate it met, false otherwise.
		 * @param feature - The feature to be tested.
		 * @return - Interpretation of this value depends on the client and the specific implementation.
		 * @reqid N/A.
		 */
		IFeaturePredicate();
		virtual ~IFeaturePredicate();

		virtual bool Evaluate(Feature& feature) =0;

	};
} // Dtp
} // MapText

#endif // !defined(EA_861998CD_8596_415f_98FE_15AA2B2E2949__INCLUDED_)
