//
//  LayerControllerViewController.h
//  MappingSdk
//
//  Created by Mark on 5/4/14.
//
//

#import <UIKit/UIKit.h>
#include "MapSchema.h"
#import "LayerControllerTableViewController.h"

@interface LayerControllerViewController : UINavigationController

@property (nonatomic, weak) id<LayerControllerTableViewControllerDelegate> lcDelegate;

/**
 If YES layer control will allow for toggling of the editing of a layer.
 Otherwise the editing controls are hidden. Default: YES.
 */
@property (nonatomic) BOOL showEditingControls;

/**
 If NO, layer control will allow disable the editing buton
 */
@property (nonatomic) BOOL disableEditingControls;

/**
 If YES layer control will allow for toggling of the identifiability
 editing of a layer. Otherwise the editing controls are hidden. Default: YES.
 */
@property (nonatomic) BOOL showIdentifiabilityControls;

/**
 If YES layer control will allow for toggling of the visibility
 editing of a layer. Otherwise the editing controls are hidden. Default: YES.
 */
@property (nonatomic) BOOL showVisibilityControls;

/**
 If YES layer control will allow  user to add online data layers
 */
@property (nonatomic) BOOL showAddLayerButton;

- (id)initWithMapCompositionTOC:(MapText::Dtp::MapCompositionTOC*)mapCompositionTOC withDelegate:(id<LayerControllerTableViewControllerDelegate>)delegate;

@end
