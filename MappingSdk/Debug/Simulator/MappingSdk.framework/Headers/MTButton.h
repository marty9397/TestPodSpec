//
//  MTButton.h
//  Lido_mPilot
//
//

#import <Foundation/Foundation.h>

typedef void(^MTBlockAction)();

@interface MTButton : UIButton

+ (MTButton*)buttonWithTitle:(NSString*)title target:(id)target action:(SEL)action;
+ (MTButton*)buttonWithImage:(UIImage*)image target:(id)target action:(SEL)action;
+ (MTButton *)buttonWithTitle:(NSString *)title image:(UIImage *)image target:(id)target action:(SEL)action;

+ (UIBarButtonItem*)barButtonWithTitle:(NSString*)title target:(id)target action:(SEL)action;
+ (UIBarButtonItem*)barButtonWithImage:(UIImage*)image target:(id)target action:(SEL)action;
+ (UIBarButtonItem*)barButtonWithTitle:(NSString *)title image:(UIImage *)image target:(id)target action:(SEL)action;
+ (UIBarButtonItem*)smallBarButtonWithImage:(UIImage*)image target:(id)target action:(SEL)action;

- (void)addActionBlock:(MTBlockAction)block forControlEvents:(UIControlEvents)controlEvents;

/** 
 Sets whether or not the colors should be inverted. Useful for toggle switches. 
 Defaults to NO.
 */
@property (nonatomic) BOOL colorsInverted;
@property (nonatomic, strong) UIColor* invertedForegroundColor;
@property (nonatomic, strong) UIColor* invertedBackgroundColor;


@end
