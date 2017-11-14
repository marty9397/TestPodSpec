//
//  LayerControllerTableViewController.h
//  MappingSdk
//
//  Created by Martin Tyberg on 6/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "MapSchema.h"

@class LayerControllerTableViewController;

@protocol LayerControllerTableViewControllerDelegate <NSObject>
@required
/**
	Layer controller notifies the delegate of state change by invoking this method.
	@param controller - The caler.
	@param schema - The schema containing the changes.
 */
- (void)layerControllerTableViewController:(LayerControllerTableViewController*)controller changedTOCState:(MapText::Dtp::MapCompositionTOC*)mapCompositionTOC;

@optional

- (void)updateMapAnnotations:(LayerControllerTableViewController *)controller;

- (void)layerControllerTableViewController:(LayerControllerTableViewController*)controller deleteMapAnnotation:(MapText::Dtp::RedlineLayer*)redlineLayer;

- (void)layerControllerTableViewController:(LayerControllerTableViewController*)controller updateOnlineDataSourcesforElement:(MapText::Dtp::MapElement*)element;

- (void)layerControllerTableViewController:(LayerControllerTableViewController*)controller deleteOnlineDataSource:(MapText::Dtp::MapElement*)onlineLayerElement;

- (void)layerControllerTableViewController:(LayerControllerTableViewController*)controller updateLayerOpacityForElement:(MapText::Dtp::MapElement*)element;

- (UIImage *)controller:(UIViewController*)controller getSymbologyImageForSymbologySet:(MapText::Dtp::SymbologySet *)symbologySet withBackGroundColor:(UIColor *)backGroundColor;

@end

@interface LayerControllerTableViewController : UITableViewController <UINavigationControllerDelegate>
{
    MapText::Dtp::OrderElements m_orderElements;
}

/**
	Initialize the controller. In order to use it in the client app, you will
    also have to prepare a MapSchema instance for the controller to work with.
    There are two versions of the UI of this controller provided in the SDK.
    Remember to instantiate the controller with a NIB which is proper for the device type!
    Example:
    <code>
        if(deviceIsPad)
        {
            layerControlViewController = [[LayerControllerTableViewController alloc] initWithNibName:@"LayerControllerTableViewController" bundle:nil schema:mySchema];
        }
        else
        {
            layerControlViewController = [[LayerControllerTableViewController alloc] initWithNibName:@"LayerControllerTableViewControllerIphone" bundle:nil schema:mySchema];
        }
 </code>
	@param nibNameOrNil - Name of the NIB to use for UI.
	@param nibBundleOrNil - Bundle from whith the NIB should be read.
	@param mapSchema The schema to work with.
	@returns - Self.
 */
- (id)initWithMapCompositionTOC:(MapText::Dtp::MapCompositionTOC*)mapCompositionTOC;

- (void) invalidateMapCompositionTOC;

@property (nonatomic, weak) id<LayerControllerTableViewControllerDelegate> delegate;

/**
	Allows for accessing the model of this controller.
 */
@property (nonatomic) MapText::Dtp::MapCompositionTOC* mapCompositionTOC;

///-----------------------------------------------------
/// @name UI Configuration
///
/// UI options can be customized via these properties.
/// NOTE: you should set those options before the controller's
/// view is loaded. They are not designed to be changed after
/// the views are already loaded.
///-----------------------------------------------------

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

@end
