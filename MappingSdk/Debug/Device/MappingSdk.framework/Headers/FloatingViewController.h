//
//  FloatingViewController.h
//  MappingSdk
//
//  Created by Mark on 10/23/12.
//
//

#import <UIKit/UIKit.h>

@protocol FloatingViewControllerDelegate;

@interface FloatingViewController : UINavigationController <UINavigationControllerDelegate>
{
    BOOL m_navigatorBarIsHidden;
}

@property (nonatomic, weak) UIViewController* popoverParentViewController;
@property (nonatomic, weak) UIViewController* rootViewController;
@property (nonatomic) CGRect popoverSourceRectInParentView;
@property (nonatomic, weak) NSArray* passthroughViews;
@property (nonatomic) BOOL isVisible;
@property (nonatomic, weak) id<FloatingViewControllerDelegate> floatingViewControllerDelegate;

- (id)initWithRootViewController:(UIViewController*)rootViewController parentViewController:(UIViewController*)popoverParentViewController passthroughViews:(NSArray*)passthroughViews;
- (void) presentController:(CGPoint)location andAdjustForTapLocation:(BOOL)adjustLocation animated:(BOOL)animated;
- (void)presentControllerAtLastPosition:(BOOL)animated;
- (void) dismiss:(BOOL)animated;

@end

@protocol FloatingViewControllerDelegate <NSObject>
- (void)floatingViewControllerDidDisappear:(FloatingViewController *)controller;
@end
