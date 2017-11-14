

//
//  MappingSDKSettings.h
//  MappingSdk
//
//  Created by Mark on 5/17/13.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

// Disabled as the GoogleMaps framework causes conflicts with WikitudeSDK
//#define GOOGLEMAPVIEWCONTROLLER
extern NSString *const SettingsString;

typedef void(^CellConfigurationBlock)(UITableViewCell *cell);
typedef void(^ActionBlock)(UITableView* tableview, NSIndexPath* indexpath);

enum Units
{
    Units_Feet,
    Units_Miles,
    Units_Meters,
    Units_Kilometers,
};

extern NSString *const kSettingName;
extern NSString *const kSettingGroupName;
extern NSString *const kSettingAction;
extern NSString *const kSettingConfigurationBlock;

@class MappingSDKSettings;

@protocol MappingSDKSettingsDelegate <NSObject>

- (void) setHighlightColor:(UIColor*)color;
- (void) deleteAllPhotosForCurrentMap;
- (bool) isSynchronizationSupported;

@end

@interface SettingsGroup : NSObject

@property (nonatomic, strong) NSString* name;
@property (nonatomic, strong) NSArray* settings;

@end

@interface MappingSDKSettings : NSObject

@property (nonatomic, weak) id<MappingSDKSettingsDelegate> delegate;
@property (nonatomic, strong) UIColor* highlightColor;
@property (nonatomic) enum Units units;
@property (nonatomic, strong) NSDictionary* unitStrings;
@property (nonatomic, strong) NSDictionary* unitConversions;
@property (readonly) NSString* currentUnitsString;
@property (readonly) float currentUnitsConversion;
@property (nonatomic) float polygonOpacity;

@property (nonatomic, strong) NSMutableOrderedSet* groups;

- (void) addSetting:(NSDictionary*)setting;

- (NSInteger)totalSettings;

@end
