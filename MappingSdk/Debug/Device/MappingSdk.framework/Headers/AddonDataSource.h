//
// Created by Lukasz Wroczynski on 11.03.2016.
//

#ifndef MAPPINGSDK_ADDONDATASOURCE_H
#define MAPPINGSDK_ADDONDATASOURCE_H


#include "IDataSource.h"

namespace MapText {
namespace Dtp {


    class LABELINGENGINE_API AddonDataSource : public IDataSource
    {
    public:

        AddonDataSource();

        virtual ~AddonDataSource();

        virtual bool IsEnabled() const;

        virtual void SetEnabled(bool enabled);

        virtual const char *GetName() const;

        virtual void SetName(const char *name);

        virtual const bool HasOwnMap() { return false; }

    private:

        /**
         * Opaque internals. Forward declare.
         */
        class LABELINGENGINE_API Impl; Impl* m_pimpl;
    };

}
}


#endif //MAPPINGSDK_ADDONDATASOURCE_H
