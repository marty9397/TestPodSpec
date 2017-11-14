//
//  FeatureAttachment.h
//  MappingSdk
//
//  Created by Marcin Gesina on 5/17/13.
//
//

#ifndef __MappingSdk__FeatureAttachment__
#define __MappingSdk__FeatureAttachment__


#include <string>
#include "Error.h"

#define ATTACHMENTS_TABLE_NAME "[_MT_ATTACHMENTS_]"

class FeatureDataReader;

namespace MapText
{
    namespace Dtp
    {
        class FeatureRecord;
        
        
        class FeatureAttachment
        {

        public:
            static const std::string STATUS_METADATA; // pulled from server, no data downloaded
            static const std::string STATUS_NOT_EXPORTED; // created locally and not exported yet to sync manager
            static const std::string STATUS_READY_TO_UPLOAD; // created locally and exported to sync manager
            static const std::string STATUS_SYNCED; // exported, pushed and confirmed by pull of metadata (photo might not be uploaded yet) OR pulled from server and downloaded
            
            FeatureAttachment(FeatureRecord const *featureRec);
            ~FeatureAttachment();
            
            double GetTimestamp() { return m_creationTimestamp; }
            void SetTimestamp(double timeStamp) { m_creationTimestamp = timeStamp; }
            std::string GetMimeType() { return m_mimeType; }
            void SetMimeType(std::string mimeType) { m_mimeType = mimeType; }
            std::string GetDescription() { return m_description; }
            void SetDescription(std::string description) { m_description = description; }
            
            size_t GetAttachmentDataSize();
            void* GetAttachmentData();
            
            void MarkForDeletion()
            {
                m_markedForDeletion = true;
            }
            bool GetMarkedForDeletion()
            {
                return m_markedForDeletion;
            }
            
            int operator==(const FeatureAttachment& attachment) const;
            // do that in derived class
            //size_t GetThumbnailDataSize();
            //void GetThumbnailData(unsigned char*& data);

            void SetInDB(bool val)
            {
                m_inDB = val;
            }
            
            bool GetInDB()
            {
                return m_inDB;
            }

            void SetStatus(std::string val)
            {
                m_status = val;
            }
            
            std::string GetStatus()
            {
                return m_status;
            }
            
            void SetExportName(std::string val){
                m_exportName = val;
            }
            
            std::string GetExportName(){
                return m_exportName;
            }
            
            
            // methods
            void FetchAttachmentData();

            void UpdateStatus(std::string status);
            void UpdateExportName(std::string exportName);
            void UpdateDescription(std::string description);
            void UpdateAttachmentProperty(std::string columnName,std::string value, Error& error);
            void UpdateAttachmentData(const void* photoData,int photoSize);
            void DeleteFromDB();

        private:
            friend class FeatureRecord;
            double m_creationTimestamp;
            std::string m_description;
            std::string m_mimeType;
            const FeatureRecord *m_featureRecord;
            void *m_attachmentData;
            std::size_t m_attachmentDataSize;
            bool m_inDB;

            std::string m_status;
            std::string m_exportName;
            
            bool m_markedForDeletion;
        
        };
    }
}


#endif /* defined(__MappingSdk__FeatureAttachment__) */
