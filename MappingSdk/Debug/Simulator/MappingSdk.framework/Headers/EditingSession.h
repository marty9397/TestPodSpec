//
//  EditingSession.h
//  MappingSdk
//
//  Created by Mark on 2/14/14.
//
//

#import <Foundation/Foundation.h>
#include "IDataSource.h"
#include "FeatureRecord.h"

typedef void (^EditingCompletionBlock)(MapText::Dtp::IDataSource* dataSource, MapText::Dtp::Error& error);

enum EditingSessionType
{
    EditingSessionType_Vector,
    EditingSessionType_DamageAssessment,
    EditingSessionType_RedLine,
    EditingSessionType_DataCollection,
    EditingSessionType_TraceTool,
};

@class EditingSession;

@protocol EditingSessionDelegate

- (void) setDefaultEditingColor:(EditingSession*)editingSession defaultColor:(long)color;

@end

@interface EditingSession : NSObject

@property (nonatomic, strong) NSTimer* autoSaveTimer;
@property (nonatomic, copy) EditingCompletionBlock completion;
@property (nonatomic) enum EditingSessionType sessionType;
@property (nonatomic) long defaultColor;

@property (nonatomic, weak) id<EditingSessionDelegate> delegate;

- (id)initWithType:(enum EditingSessionType)type;

@end

@interface RedLineSession : EditingSession

@property (nonatomic) bool updatingRedlines;
@property (nonatomic, strong) NSString* name;
@property (nonatomic, strong) NSString* dataSourcePath;
@property (nonatomic, strong) NSString* dataSourceName;

@end

typedef void (^ResultsCompletionBlock)(NSString* jsonString, MapText::Dtp::Error& error);

class FeatureDataReader;

@interface DamageAssessmentSession : EditingSession

@property (nonatomic, copy) ResultsCompletionBlock resultsCompletion;
@property (nonatomic) MapText::Dtp::Polyline* selectionBoundary;
@property (nonatomic) void* damageAssessment;

- (id)initWithJson:(NSString*)jsonString withFeatureDataReader:(FeatureDataReader&)featureDataReader withMapCompositionTOC:(MapText::Dtp::MapCompositionTOC&)mapCompositionTOC withError:(MapText::Dtp::Error&)error;

- (void)addFeatureRecords:(MapText::Dtp::FeatureRecords&)featureRecordList;

- (void)updateFeatureRecords:(MapText::Dtp::FeatureRecords&)featureRecordList;

- (NSString*)damageAssessmentReport:(MapText::Dtp::Error&)error;

@end

@interface TraceToolSession : EditingSession

@property (nonatomic, copy) ResultsCompletionBlock resultsCompletion;
@property (nonatomic) void* traceReport;

- (id)initWithJson:(NSString*)jsonString withFeatureDataReader:(FeatureDataReader&)featureDataReader withMapCompositionTOC:(MapText::Dtp::MapCompositionTOC&)mapCompositionTOC withError:(MapText::Dtp::Error&)error;

//- (void)addFeatureRecords:(MapText::Dtp::FeatureRecords&)featureRecordList;
//
//- (void)updateFeatureRecords:(MapText::Dtp::FeatureRecords&)featureRecordList;

- (NSString*)traceReport:(MapText::Dtp::Error&)error;

@end
