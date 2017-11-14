//
//  LayerControlTableViewCellCell.h
//  MappingSdk
//
//  Created by Martin Tyberg on 6/28/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "MapSchema.h"

@protocol LayerControlTableViewCellDelegate <NSObject>

- (void)refreshTable:(UITableViewCell*)cell;

- (void)refreshMap:(UITableViewCell*)cell;

- (void)layerControlTableViewCell:(UITableViewCell*)cell insertRowsForGroup:(MapText::Dtp::MapElement*)groupElement;

- (void)layerControlTableViewCell:(UITableViewCell*)cell deleteRowsForGroup:(MapText::Dtp::MapElement*)groupElement;

@optional

- (void)updateMapAnnotations:(UITableViewCell*)cell;

- (void)layerControlTableViewCell:(UITableViewCell*)cell updateOnlineDataSourcesforElement:(MapText::Dtp::MapElement*)element;

- (void)layerControlTableViewCell:(UITableViewCell*)cell updateLayerOpacityForElement:(MapText::Dtp::MapElement*)element;

@end

@interface LayerControlTableViewCell : UITableViewCell
{
@protected
    NSString* m_layerControlCellId;
    MapText::Dtp::OrderElements *m_orderElements;
    MapText::Dtp::MapElement *m_element;
    int m_treeLevel;
    
    UISwitch *m_visibleSwitch;
}

- (void)setMapElement:(MapText::Dtp::MapElement*)element andTreeLevel:(int)level;
- (MapText::Dtp::MapElement*)getMapElement;
+ (NSString *) cellId;

- (void)initialize:(MapText::Dtp::OrderElements*)orderElements;

- (void)unfoldTableViewCell;


@property (nonatomic, strong) NSString* layerControlCellId;
@property (nonatomic, weak) IBOutlet UILabel* titleLabel;
@property (nonatomic, weak) id<LayerControlTableViewCellDelegate> delegate;

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


@end
