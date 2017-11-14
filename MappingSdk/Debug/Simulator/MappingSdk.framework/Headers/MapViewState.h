//
//  MapViewState.h
//  MappingSdk
//
//  Created by Mark on 10/18/15.
//
//

#import <Foundation/Foundation.h>

@interface MapViewState : NSObject <NSCoding, NSCopying>

- (id) initWithViewCenter:(CGPoint)viewCenter withScale:(NSNumber*)scale withFontScaleFactor:(NSNumber*)fontScaleFactor;

@property (nonatomic) CGPoint viewCenter;
@property (nonatomic) NSNumber* scale;
@property (nonatomic) NSNumber* fontScaleFactor;

@end
