//
//  MapViewController.h
//  idynamicenroute
//
//  Created by Marcin Fraczak on 6/21/11.
//  Copyright 2011 MapText, Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>

#import "IDataSource.h"
#import "DataProjector.h"
#import "MappingSDKSettings.h"
#import <vector>
#import "FeatureRecord.h"
#import "EditingSession.h"
#import "MapViewState.h"
#import "AddonDataSource.h"

#define DROPPIN "droppin"

extern NSString * const kDidFinishRendering;

/**
 * @mainpage Getting Started
 *
 * <a href="#Introduction">1. Introduction</a><br/>
 * <a href="#Overview">2. Architecture</a><br/>
 * <a href="#Guide">3. Adding SDK to your project</a><br/>
 * <a href="#DataPreparation">4. Preparing the map data</a><br/>
 * <a href="#DisplayingMap">5. Displaying the map</a><br/>
 * <a href="#Example">6. Example app</a><br/>
 *
 * \section Introduction 1. Introduction
 *
 * The iOS Mapping SDK component is a high-performance automated map labeling and rendering engine that can be easily embedded into 
 * mobile mapping applications.
 * It offers the following key capabilities:
 * - Customizable maps with full control over symbolization of features and style of text.
 * - On-the-fly rendering of maps from your vector data.
 * - Sophisticated text placement engine.
 * - Offline maps based on high-performance data storage format.
 *
 * The SDK was designed for ease of use and allows you to create a custom map of almost any kind  
 * with minimal amount of coding. At the same time, it's flexible enough to handle complex usage scenarios
 * by exposing events and lower level APIs.
 *
 * \section Overview 2. Architecture
 * 
 * The diagram below shows the main parts of the mapping component.
 * <p>@image html iOSMappingSDKOverview.png</p>
 *
 * The Mapping SDK is working with data serialized into special binary files. Those files are optimized for 
 * quick data retrieval. To generate the files, use the Data Publisher tool which reads shape files and map configuration (that defines
 * symbolization and text placement configuration). See section @ref DataPreparation
 * "Preparing the map data" for details on how to prepare data for your application.
 *
 * @section Guide 3. Adding SDK to your project
 * 
 * The first step is to set up your project to use the SDK. (If you are starting from scratch or you want 
 * to see a complete example of the SDK in action, please check section @ref Example "Example App" which describes a  
 * fully functional example application).
 * 
 * 3.1. Create a container view.
 * <p>Your project needs to provide the basic framework for running an application with at least one
 *    view and its controller. In this view, add a container view which will work as a frame for your map.</p>
 *
 * 3.2. Configure the container view.
 * <p>Make sure to configure the container behavior so that the map behaves in the expected way (e.g. it should
 *    reposition/resize itself accordingly when interface orientation changes). The map view controller does
 *    not override any of the container settings.</p>
 *    <p>@image html MapViewContainer.png "MapView Container nested in the application screen."</p>
 *
 * 3.3. Include Mapping SDK framework.
 * <p>Include the Mapping SDK framework in your project. To do that option-click on the "Frameworks" group of your
 *    project (if you don't have such a group, don't worry, you may create one; you may also call it whatever you
 *    like), and choose "Add Files to 'YourProjectNameHere'..."</p>
 * <p>In the dialog shown, the option "Create groups for any added folders" should be selected. Navigate to a place where
 *    you saved the <tt>MappingSdk.embeddedframework</tt> folder, select it and confirm by clicking "Add". It's important
 *    to select the <tt>MappingSdk.<b>embeddedframework</b></tt>, not the <tt>MappingSdk.<b>framework</b></tt> folder. 
 *    Including the latter will not include the necessary resources in the client project.</p>
 *    <p>@image html AddFramework.png "Adding a reference to the Mapping SDK Framework."</p>
 *
 * 3.4. Include additional dependencies.
 * <p>Include additional FreeType dependency by option-clicking on the "Frameworks" project group again and choosing
 *    the "Add Files to 'YourProjectNameHere'..." item. Then navigate to the libfreetype-universal.a file distributed 
 *    along with the Mapping SDK. Confirm the operation by clicking "Add".</p>
 *
 * <p>Also include the secondary framework dependencies in your project in the following steps:
 *    - select the project node in the Project Navigator
 *    - select the desired target, which should be the application binary
 *    - switch to the <tt>Build Phases</tt> tab and unfold the <tt>Link Binary With Libraries</tt> section
 *    - click the "+" sign in the bottom of the unfolded section
 *    - additionally select the following frameworks repeating the previous steps until 
 *      all listed frameworks are included (of course you may skip the ones your project already include):
 *      -# <tt>CoreGraphics.framework</tt>
 *      -# <tt>Foundation.framework</tt>
 *      -# <tt>MessageUI.framework</tt>
 *      -# <tt>OpenGLES.framework</tt>
 *      -# <tt>QuartzCore.framework</tt>
 *      -# <tt>UIKit.Framework</tt>
 *      -# <tt>libsqlite3.dylib</tt>
 *      -# <tt>libz.dylib</tt>
 *    <p>@image html FrameworksIncluded.png "'Link Binary With Libraries' section after properly including all the required dependencies. Note that after properly adding the MappingSDK.embeddedframework dependency the MapView.nib resource should appear in the 'Copy Bundle Resources' section."</p>
 *
 * 3.5. Set up additional project settings.
 * <p>For projects created in Xcode 4.5 go to project build settings and set "C++ Standard Library" to "Compiler Default".
 * 3.6. Instantiate MapViewController.
 * <p>At this point your application has all pieces of the SDK in place. Now we have to instantiate the bundled 
 * MapViewController and add its view to the application GUI. Sample code may look like this:
 * @code

    #include <MappingSdk/DynamicTextPlacement.h>
    #include <MappingSdk/MTGLMap.h>
    #import <MappingSdk/MapViewController.h>
 
    - (void)viewDidLoad
    {
        [super viewDidLoad];

        // Set the map view size to fully cover the map container
         CGRect size = self.mapViewContainer.frame;
         size.origin.x = 0;
         size.origin.y = 0;
         
         // Instantiate the MapViewController. The .nib name in MappingSDK framework is 'MapView'.
         self.mapViewController = [[[MapViewController alloc] initWithNibName:@"MapView" bundle:nil requiredSize:size] autorelease];

        // Add the controller view as a subview of the map container frame.
        [self.mapViewContainer addSubview:self.mapViewController.view];
 
        // Add the controller to controllers hierarchy or it won't be notified properly of all the events.
        [self addChildViewController:self.mapViewController];
    }
 * @endcode
 * <p><b>NOTE:</b> Classes which are using the SDK should be compiled by Objective-C++ and have extensions <tt>.mm</tt> (and not <tt>.m</tt>).
 *
 * @section DataPreparation 4. Preparing the map data
 *
 * @subsection DataGeneration 4.1 Data Generation.
 * The easiest way to prepare the data for the SDK is to use our Data Publisher tool. The tool is a simple command line utility that 
 * takes shapefiles and map configuration and generates binary map files:
 * <p><center><tt>DTPVisualizer.exe --serialize_map --bin \<data_folder\> \<spe_file_name\> \<mcf_file_name\> \<output_folder\></tt></center></p>
 * where:
 * @arg data_folder is a folder containing shape files
 * @arg spe_file_name is a map specification file
 * @arg mcf_file_name is a map configuration file
 * @arg output_folder is a folder where serialized binary files will be outputted
 *
 * <p>(Note: currently Data Publisher works on MS Windows only. Please contact us if you need MacOS X version.)</p>
 * <p>Data can also be fed to the component by using a lower level API (Dynamic Text Placement API). 
 * In this scenario, all geometries, symbolization rules and text placement configuration is passed to the component 
 * through API calls. If you are interested in this option please contact us for additional documentation.</p>
 * <p>The look of the map is controlled by two configuration files: spe and mcf. The spe file configures layers, scale ranges,
 * feature classes, symbolization of features, text styles and label text. The mcf file is configuring the text placement.
 * We can help you in building map configuration for your specific data.
 * @subsection DataDeployment 4.2 Data Deployment.
 * <p>The serialized dataset consists of a set of binary files. Those files should be deployed on the iOS device into a single
 * folder. Additionally, you should also bundle all the required fonts into the <tt>Fonts</tt> subfolder. Font file names
 * should be all lower case and should match the names used in the map configuration files. For example, if you specified the font name to be Arial Bold, you need to rename the file from <tt>"arialbd.ttf"</tt> to <tt>"arial bold.ttf"</tt>.</p>
 * <P>Once the map package is ready, you have to transfer it to the iOS device. There are several options. The easiest 
 * is to include the data in the application main bundle. Probably a better solution is to transfer the data at runtime 
 * using some kind of download mechanism. The transfer of the data to the device is not part of the SDK, but you can easily reuse 
 * the code from our example app that implements transfer from Amazon S3 web service or from any HTTP server.</P>
 * <P>To test the SDK quickly without developing a download mechanism, use first option and bundle the map with your application.
 * To do that, option-click on the project node in the Project Navigator, choose "Add files to 'YourProjectNameHere'...",
 * then navigate to the folder where you have the map resources saved. It's a good practice to copy them to the project folder structure
 * so that they can remain in a fixed location for the build process to find them. Select the folder and make sure that the "Create folder references for any added folders" option is checked.</p>
 * <p>@image html AddingMapResources.png "Adding the map resources into the client project."</p>
 *
 * @section DisplayingMap 5. Displaying the map
 * <p>Once the map files and font resources are available to the runtime code, you have to initialize the map. The MapViewController
 * class provides a static method initializeSerializedMap:withFontResourcePath:withError: to ease that task. Once the map is deserialized
 * you pass it into the view controller and this should be enough to have the simplest map application running. You may choose to do 
 * this once the application loads or in response to a user action, e.g. clicking a button. Sample code may look like this:
 * 
 * @code
 
        - (IBAction)loadMap:(id)sender
        {
            NSString* fontPath = [NSString stringWithFormat:@"%@/Fonts", [[NSBundle mainBundle] resourcePath]];
            NSString* mapPath = [NSString stringWithFormat:@"%@/Maps/01/map", [[NSBundle mainBundle] resourcePath]];

            MapText::Dtp::Error error;

            // Just in case cancel all the panding operations
            [self.mapViewController stopAction:self];
 
            // Detach the previously loaded map from the controller
            [self.mapViewController setMasterMap:NULL withError:error];
            if ( error )
            {
                [self handleError:error];
                return;
            }

            // Delete the previous map
            delete m_firstMap;
 
            // Load the map
            m_firstMap = [MapViewController initializeSerializedMap:mapPath withFontResourcePath:fontPath withError:error];
            if ( error )
            {
                [self handleError:error];
                return;
            }

            // Assign myself as the delegate of the controller (see MapViewDelegate protocol)
            self.mapViewController.delegate = self;
 
            // Set the font resources path
            [self.mapViewController setMapFontResourcesPath:fontPath];
            // Set the map data path
            [self.mapViewController setMasterMapDataPath:mapPath];
            // Assign the deserialized map as the data source
            [self.mapViewController setMasterMap:m_firstMap withError:error];
            if ( error )
            {
                [self handleError:error];
                return;
            }
            
            // Label the initial frame
            [self.mapViewController labelFrame];
 
            // Redraw the map
            [self.mapViewController drawFrame];
        }
 * @endcode
 *
 * This is the simplest scenario to get a map running. To implement more advanced functions, such as 
 * feature lookup or updating map position according to the GPS location, you need to handle the events raised by 
 * the MapViewController component. To achieve it, implement MapViewControllerDelegate protocol and assign a delegate to the component. 
 * Read more details in the specification of the interface.
 * 
 * @section Example 6. Example app
 * If you want to jump start the development of your application, you can base your app on the example NYC Map that we have developed.
 * This simple app is a ready-to-use dynamic map of the New York Metro area based on OpenStreetMap data (http://www.openstreetmap.org).
 * It has the following features:
 * - Fully-functional data transfer from Amazon S3 or HTTP server (code has to be enabled).
 * <p>@image html ExampleAppDataTransfer.png</p>
 * - Data can also be added using iTunes file sharing (simplest scenario to get a map running)
 * <p>@image html FileSharing.png</p>
 * - Complete map configuration with scale ranges suitable for OpenStreetMap data
 * <p>@image html ExampleAppManhattan.png</p>
 * <p><br>@image html ExampleAppBrooklyn.png</p>
 * - GPS integration with compass mode
 * <p>@image html ExampleAppCompassMode.png</p>
 * The app supports both iPhone and iPad. You may download a fully functional app project from here: <a href="NYCApp.zip">NYCApp.zip</a>. You may also explore the sources through the doxygen documentation available here: <a href="NYCApp/index.html">NYCApp docxygen docs</a>
 * 
 * <p><b>Thanks for trying our SDK and have a good mapping!</b></p>
 * <p>MapText Team<p>
 * 
 */

/**
 * Forward declarations
 */

class MacProgressReport;
@class EAGLContext;
@class MapPackage;
@class GLMapView;

/**
 * Gesture handling modes
 */
enum GestureHandlingMode
{
    /**
     * In this mode the view will refresh it's contents by adjusting
     * accordingly the displayed map. Delegate will also get notified about the
     * gestures.
     */
    GestureHandlingMode_FullyAutomatic,
    /**
     * In this mode the view will only notify the delegate about the gesture.
     * No update to the view content shall be made.
     */
    GestureHandlingMode_ReportToDelegateOnly,
};

typedef void (^BlockTask)(id owner);

typedef void (^BlockCompletion)();


@class MapViewController;
@protocol PhotoManager;

/**
 * Needs to be implemented to respond to the events generated by the MapViewController, 
 * including error handling.
 */
@protocol MapViewDelegate <NSObject>

@optional

/**
 * Progress reporting for lengthy operations (data reading, labeling)
 * @param controller - controller sending the message
 * @param progress - Progress of the operation [0..1]
 * @param currentOperation - Descriptive name of the operation in progress
 */
- (void) updateProgressInController:(MapViewController*)controller progress:(float)progress forOperation:(NSString*) currentOperation;

/**
 * @name Gesture events
 * Events related to gesture detection.
 * @{
 */

/** 
 * Pan gesture is caught.
 * @param controller - The sender.
 * @param recognizer - A recognizer reporting the gesture.
 */
- (void) handlePanGestureInController:(MapViewController*)controller withRecognizer:(UIPanGestureRecognizer *)recognizer;
/**
 * Pinch gesture is caught.
 * @param controller - The sender.
 * @param recognizer - A recognizer reporting the gesture.
 */
- (void) handlePinchGestureInController:(MapViewController*)controller withRecognizer:(UIPinchGestureRecognizer *)recognizer;
/**
 * Tap gesture is caught.
 * @param controller - The sender.
 * @param recognizer - A recognizer reporting the gesture.
 */
- (void) handleTapGestureInController:(MapViewController*)controller withRecognizer:(UITapGestureRecognizer*)recognizer;

/**
 * Double tap gesture is caught.
 * @param controller - The sender.
 * @param recognizer - A recognizer reporting the gesture.
 */
- (void) handleDoubleTapGestureInController:(MapViewController*)controller withRecognizer:(UITapGestureRecognizer*)recognizer;

/**
 * Long pressure gesture is caught.
 * @param controller - The sender.
 * @param recognizer - A recognizer reporting the gesture.
 */
- (void) handleLongPressureGestureInController:(MapViewController*)controller withRecognizer:(UILongPressGestureRecognizer*)recognizer;

/**
 * Tells the delegate that the map view will start tracking the user’s position
 * @param controller - The sender.
 */
- (void)mapViewWillStartLocatingUser:(MapViewController*)controller;

/**
 * Tells the delegate that the map view stopped tracking the user’s location
 * @param controller - The sender.
 */
- (void)mapViewDidStopLocatingUser:(MapViewController*)controller;

/**
 * Tells the delegate that the location of the user was updated
 * @param controller - The sender.
 */
- (void)didUpdateUserLocation:(MapViewController*)controller;

/**
 * Tells the delegate that an attempt to locate the user’s position failed
 * @param controller - The sender.
 * @param error - error object
 */
- (void)mapViewController:(MapViewController*)controller didFailToLocateUserWithError:(NSError*)error;

/**
 * Tells the delegate that the user tracking mode changed
 * @param controller - The sender.
 */
- (void)didChangeUserTrackingMode:(MapViewController*)controller;


- (NSString*) getResourceIDOfCurrentDocument;
- (BOOL) isSynchronizationSupported;

/**@}*/

/**
 * @name Labeling steps events
 * Events occuring when labeling of the map takes place.
 * @{ 
 */

/**
 * The labeling map has been created as a clone of the master map. No features have been
 * added to it, they will have to be processed & added with each frame. The client code
 * may perform any additional setup on the labeling map here (alter the configuration etc.).
 * Caution: features collection will be repopulated, adding anything to it will probably
 * not yield expected results. Features have to be inserted into the master map to be used
 * during labeling.
 * @param controller - The sender.
 * @param masterMap - Reference to the master map used by the controller.
 * @param labelingMap - The newly cloned labeling map.
 */
- (void) labelingMapInstantiated:(MapViewController*)controller withMasterMap:(MapText::Dtp::Map&) masterMap withLabelingMap:(MapText::Dtp::Map&) labelingMap;

/**
 * Invoked right before the labeling is about to begin.
 * @param controller - The sender.
 * @param masterMap - Reference to the master map used by the controller.
 * @param labelingMap - Reference to the labeling map.
 */
- (void) beforeLabeling:(MapViewController*)controller withMasterMap:(MapText::Dtp::Map&) masterMap withLabelingMap:(MapText::Dtp::Map&) labelingMap;

/**
 * Invoked right after the labeling has ended.
 * @param controller - The sender.
 * @param masterMap - Reference to the master map used by the controller.
 * @param labelingMap - Reference to the labeling map.
 */
- (void) afterLabeling:(MapViewController*)controller withMasterMap:(MapText::Dtp::Map&) masterMap withLabelingMap:(MapText::Dtp::Map&) labelingMap;


-(void) afterEndingEditingSession:(MapViewController*)controller sessionType:(EditingSessionType)editingSessionType;
/**@}*/

/**
 * @name Background activity
 * @{
 */
/**
 * A background operation starts - user interaction should be disabled. Automatic event
 * handling done by the map view itself is disabled automatically. Only client-specific UI
 * should be disabled additionally when this event is reported.
 * @param controller - Sender of the message.
 */
- (void) backgroundOperationStarted:(MapViewController*)controller;

/**
 * A background operation has finished - user interaction may be enabled. Automatic event
 * handling done by the map view itself is enabled automatically. Only client-specific UI
 * should be enabled additionally when this event is reported.
 * @param controller - Sender of the message.
 */
- (void) backgroundOperationFinished:(MapViewController*)controller;

/**
 * Invoked to tell the client the id of the tapped feature on the clients datasource
 * @param controller - Sender of the message.
 * @param featureId - feature id of the tapped feature
 */
- (void) didFindFeature:(MapViewController*)controller withFeatureId:(NSString*)featureId andBoundingBox:(MapText::Dtp::UprightRectangle)bbox;

/**
 * Invoked to tell the client that the feature search controller popover was dismissed
 * @param controller - Sender of the message.
 */
- (void) featureSearchControllerWasDismissed:(MapViewController*)controller;

/**
 * Invoked to tell the client that the attribute lookup controller popover was dismissed
 * @param controller - Sender of the message.
 */
- (void) dataLookupControllerWasDismissed:(MapViewController*)controller;

/**
 * Invoked to tell the client that a feature record was saved in the DB as a result of editing in the
 * data lookup dialog (attribute lookup, search, form editing).
 * @param controller - Sender of the message.
 */
- (void) dataLookupSavedChanges:(MapViewController*)controller;

/**
 *
 **/
- (ResultsCompletionBlock)tracingCompletionBlock:(MapViewController*)controller;
/**
 * @}
 */

@required

/**
 * An error occured.
 * @param controller - Sender of the message.
 * @param error - The error.
 */
- (void) errorReported:(MapViewController*)controller error:(MapText::Dtp::Error&) error;

@end

class MapViewControllerError
{
public:
    enum ErrorCode
    {
        ErrorCode_MapIsNotReady = 1,
        ErrorCode_RegionIsOutOfDataExtent,
        ErrorCode_MapDoesNotHaveASchema,
    };
};


/**
 * Provider of external (not embedded inside the map) HTML forms for editing map features.
 */
@protocol MapFormsProvider <NSObject>
/**
 * Searches for external forms that can be used with the map.
 * @return  A map of NSStrings to NSStrings containing mappings of form name to full paths for externally provided HTML forms for this Map VC.
 */
- (NSDictionary *) resolveForms;
@end

/**
 * @brief This is the main SDK component responsible for displaying a map.
 * MapViewController class along with the corresponding .nib file are contained in the SDK framework.
 * You can instantiate it in your code and add it to your UI hierarchy. Check the main guide section @ref Guide.
 */
@interface MapViewController : UIViewController  <UIGestureRecognizerDelegate, UISearchBarDelegate, UIAlertViewDelegate, UIActionSheetDelegate, UIPopoverControllerDelegate >

/**
 * @name Lifecycle
 */
/**@{*/
/**
 * Initializes the view controller providing the desired size of the view.
 * The default initializeWithNibName call will assume roughly the iPad screen size.
 * @param nibNameOrNil - See [UIVIew initWithNibName:bundle].
 * @param nibBundleOrNil - See [UIVIew initWithNibName:bundle].
 * @param size - The size of the view to set. The view also is set to "stretch"
 *                       according to the owner size afterwards, so there's no need to
 *                       adjust the size manually.
 */
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil requiredSize:(CGRect)size;

/**@}*/

/**
 * @name Basic operations
 */
/**@{*/

/**
 * Set the master map to use for display. It also retrieves some of the settings from the map
 * metadata and apllies them to the view:
 * <ul><li>min and max allowed scale</li>
 *     <li>sets the view center to the center of map extent</li>
 *     <li>applies additional symbology scaling factor</li>
 *     <li>sets the allowable map panning area to the map extent</li></ul>
 * The method also sets the masterMap main thread worker to itself (the view controller).
 * @param masterMap - Ownership is NOT taken. Client should still manage the
 *                    lifecycle of the object. Pass NULL to detach.
 * @param error - Error info.
 */
- (void) setMasterMap:(MapText::Dtp::Map*) masterMap withError:(MapText::Dtp::Error&) error;

/**
 * Call this method to add a data source to the collection of processed sources.
 * All sources will be processed by the SDK to build a consistent map. The order
 * in which the sources are added defines the default drawing order.
 * @param source - The source to add to processed collection.
 * @param error - Error info.
 */
- (void)addDataSource:(MapText::Dtp::IDataSource*)source withError:(MapText::Dtp::Error&) error;

/**
 * Adds an add-on data source to this map controller.
 * The add-on data source is added like a regular data source and additionally it is visible in the map TOC,
 * where its visibility can be toggled.
 * @param source - The source to add to processed collection.
 * @param error - Error info.
 */
- (void)addAddonDataSource:(MapText::Dtp::AddonDataSource*)source withError:(MapText::Dtp::Error&) error;

/**
 * Removes an add-on data source added previously with 'addAddonDataSource' from this map controller.
 * Works like 'removeAddonDataSource' additionally removing the data source from the map TOC.
 *
 * @param source - The source to be removed from processed collection.
 * @param error - Error info if removal fails.
 */

- (void)removeAddonDataSource:(MapText::Dtp::AddonDataSource *)source withError:(MapText::Dtp::Error &)error;

/**
 * Removes the passed in data source from the processed collection. The object is not deleted,
 * caller should assume ownership over it from that point on. If the passed source is not part
 * of the collection this method has no effect, it also doesn't report an error.
 * @param source - The source to be removed from processed collection.
 * @param error - Error info if removal fails.
 */
- (void)removeDataSource:(MapText::Dtp::IDataSource*)source withError:(MapText::Dtp::Error&) error;

/**
 * If you want to fully replace the base map, call this method and re-populate the data sources.
 */
- (void)resetMasterMapAndDataSources;

/**
 * Returns a copy of the internal collection of data source references.
 * @return - The list of the list of data sources to process. This is
 *           a shallow copy of the list, meaning the same instances of the sources are
 *           actually being returned.
 */
- (std::vector<MapText::Dtp::IDataSource*>) dataSourcesInUse;

// TODO: this is most likely a temporary utility method and shall be removed
+ (MapText::Dtp::Shape*)featureShapeFromWkt:(NSString*)wkt withProjector:(MapText::Dtp::DataProjector*)projector withError:(MapText::Dtp::Error&)error;
+ (MapText::Dtp::AnnotationShapeItem*)annotationBackGroundFromLabel:(MapText::Dtp::RegularLabel*)regLabel withOriginalMapScale:(float)origScale withError:(MapText::Dtp::Error&)error;

/**
 * Redraw the current frame
 */
- (void) drawFrame;

/**
 * Re-label the current frame. At the end of labeling the map will be redrawn. 
 * <b>CAUTION: This method invokes background thread to do the processing.</b>
 * You should expect the backgroundOperationStarted event reported to the delegate
 * immediately after this call. Do not call any other operation immediately after invoking it,
 * instead wait for the backgroundOperationFinished event which will be send to the delegate after
 * this operation finishes. In automatic event handling mode the component will remain inactive with
 * respect to the events until this operation finishes.
 */
- (void) labelFrame;

/**
 * Stop a lengthy background operation
 * @param sender - Message sender.
 */
- (void) stopAction:(id)sender;

/**
 * Clears the cache in memory for the loaded map. This method locks and waits for the internal map
 * object to become available, i.e. not used by any internal operations (rendering, data loading etc.)
 * It should not be called if main thread callbacks are pending, or it may lead to a deadlock.
 */
- (bool) clearCache;

/**
 * Initializes sending a bug report containing the currently visible map extent along with whatever
 * details the user puts into the email.
 */
- (void)sendBugReport;

/**
 * Initializes a serialized map from the disk.
 * @param mapFileCorePath - Core (without the extension, which is serializer specific)
 *                          filename, including the full path of the map file 
 *                          (e.g. <tt> /Applications/.../Library/Caches/map </tt>)
 * @param fontPath - Path to the folder containing fonts used by the map. Font resources
 *                   have to be available for reading for the map component to render any
 *                   text and font-based symbols.
 * @param[out] error - Error info.
 */
//- (MapText::Dtp::Map*) initializeSerializedMap:(NSString*)mapFileCorePath withFontResourcePath:(NSString*)fontPath withError:(MapText::Dtp::Error&)error;

/**
 * Using this method client code can quickly obtain a list of features which reside at the given screen coordinates.
 * The spatial query is defined as a square of selectionBufferInches size centered on the location point. 
 * NOTE: only features which are currently loaded into the master map collection will be reported.
 * If some features are not currently loaded, e.g. because they are not shown in this zoom level, they will NOT
 * be reported. 
 * @param location - The location in screen coordinates (i.e. pixels). 
 * @param selectionBufferInches - Size of the buffer around the location in screen inches which should be applied.
 * @param[out] featuresList - The list of features at the specified region.
 */
- (void) getFeaturesAtScreenCoordinates:(CGPoint)location selectionBufferInches:(float)selectionBufferInches featuresList:(std::vector<MapText::Dtp::Feature*>&)featuresList;

/**
 * Using this method client code can quickly obtain a list of features which reside in the given polygon.
 * NOTE: only features which are currently loaded into the master map collection will be reported.
 * If some features are not currently loaded, e.g. because they are not shown in this zoom level, they will NOT
 * be reported.
 * @param polygon
 * @param[out] center - center of the polygon bounding box
 * @param[out] featuresList - The list of features at the specified region.
 */
- (void) getFeaturesInPolygon:(MapText::Dtp::Polygon*)polygon featuresList:(std::vector<MapText::Dtp::Feature*>&)featuresList center:(CGPoint&)center;

/**
 * This will show the vector editing tools and allow the user to create features
 */
- (void) beginVectorEditingSession:(EditingCompletionBlock)completion removeSessionCompletionButtons:(BOOL)removeButtons;

- (void) endVectorEditingSessionAndShouldSaveFeatures:(BOOL)saveFeatures;

/**
 * This will allow the client to zoom to a redline datasource in the map
 * @param datasource - redline datasourcer
 */
- (void) zoomToRedlineDatasource:(MapText::Dtp::IDataSource*)datasource;

/**
 * This will show the vector editing tools and allow the user to create redline annoations
 * @param dataSource - redline datasource to edit
 * @param completion - completion block to use when ending editing session
 */
- (void) beginRedlineEditingSession:(MapText::Dtp::IDataSource*)dataSource withCompletion:(EditingCompletionBlock)completion;

/**
 * This add to begin an editing session and setup of the toolbar specifically for fieldman
 * @param completion - completion block to use when ending editing session
 */
- (void) beginFieldMapEditingSessionWithCompletion:(EditingCompletionBlock)completion;

/**
 * This add to begin an editing session for damage assessment
 * @param jsonString - jsonString of damage report if editing a previously saved report
 * @param completion - completion block to use when ending editing session
 */
- (void) beginDamageAssessmentEditingSessionWithCompletionWithReport:(NSString*)jsonString completion:(ResultsCompletionBlock)completion;

/**
 * This will allow the client to begin an editing session for running a trace on a network
 * @param jsonString - jsonString of trace report if editing a previously saved report
 * @param completion - completion block to use when ending editing session
 */
- (void) beginTraceToolEditingSessionWithCompletionWithReport:(NSString*)jsonString completion:(ResultsCompletionBlock)completion;

/**
 This is the current editing session
 */
@property (nonatomic, strong) EditingSession* editingSession;

/**
 * Call into this method will persist the current editing session without removing the features from the editor.
 * After calling this the client should call endEditingSessionAndShouldSaveFeatures:withError: choosing NOT to save
 * the features, or they will be persisted twice. If endEditingSessionAndShouldSaveFeatures:withError: is called 
 * with YES to save the features, this method doesn't need to be called into.
 */
- (void)persistCurrentEditingSessionWithError:(MapText::Dtp::Error&)error;

/**
 * Temporary hack to make features editable
 */
- (void) makeFeaturesEditable:(NSArray *)featureIDs withError:(MapText::Dtp::Error&)error;

/**
 * This will allow the user to find out how many objects have been created currently during editing
 */
@property (readonly) int numberOfCurrentEditingObjects;

/** This allows the name (login id email) to be passed. The name could be used to save the login-id
 of the user into the data during data collection if desired by the user*/
@property (nonatomic, copy) NSString* userLogin;

/**
 * This will allow the client to move the geometry toolbar up or down
 */
- (void) setToolBarVerticalPosition:(int)y withAnimation:(BOOL)animate;

/**
 * Dismisses the currently visible popover controller.
 */
- (void) dismissCurrentlyVisiblePopoverController;

/**
 * @name UI interaction configuration.
 * Configure if and how the view should respond to user actions.
 */
/**@{*/

/**
 * Current PopoverController that is visible on the screen, or nil.
 */
@property (nonatomic, strong) UIPopoverController* currentlyVisiblePopoverController;

/**
 * If set to true the view will be responding to the selected gesture. Also set the 
 * ...GestureHandlingMode to control how the handling is done.
 */
@property (nonatomic) bool panGestureHandlingEnabled;
/**
 * If set to true the view will be responding to the selected gesture. Also set the 
 * ...GestureHandlingMode to control how the handling is done.
 */
@property (nonatomic) bool pinchGestureHandlingEnabled;    
/**
 * If set to true the view will be responding to the selected gesture. Also set the 
 * ...GestureHandlingMode to control how the handling is done.
 */
@property (nonatomic) bool tapGestureHandlingEnabled;    
/**
 * If set to true the view will be responding to the selected gesture. Also set the 
 * ...GestureHandlingMode to control how the handling is done.
 */
@property (nonatomic) bool doubleTapGestureHandlingEnabled;    
/**
 * If set to true the view will be responding to the selected gesture. Also set the 
 * ...GestureHandlingMode to control how the handling is done.
 */
@property (nonatomic) bool longPressureGestureHandlingEnabled;

/**
 * Configure how the view should respond to gestures. Applies only if handleXXXGestures
 * is set to true.
 */
@property (nonatomic) GestureHandlingMode panGestureHandlingMode;    
/**
 * Configure how the view should respond to gestures. Applies only if handleXXXGestures
 * is set to true.
 */
@property (nonatomic) GestureHandlingMode pinchGestureHandlingMode;    
/**
 * Configure how the view should respond to gestures. Applies only if handleXXXGestures
 * is set to true.
 */
@property (nonatomic) GestureHandlingMode tapGestureHandlingMode;    
/**
 * Configure how the view should respond to gestures. Applies only if handleXXXGestures
 * is set to true.
 */
@property (nonatomic) GestureHandlingMode doubleTapGestureHandlingMode;    
/**
 * Configure how the view should respond to gestures. Applies only if handleXXXGestures
 * is set to true.
 */
@property (nonatomic) GestureHandlingMode longPressureGestureHandlingMode;    

/**
 * Configure if view should respond to tap gestures by attempting a datalookup.
 * This will currently be ignored altogether during an editing session
 */
@property (nonatomic) bool shouldAttemptDataLookupOnTap;

/**
 * Configure if view should respond to tap gestures by checking if a feature
 * from the current read/write datasource has been tapped.
 * This will currently be ignored altogether during an editing session
 */
@property (nonatomic) bool shouldAttemptDataSourceFeatureLookup;

/**
 * Configure by how much the view should zoom in if a double-tap event is caught
 */
@property (nonatomic) float doubleTapZoomingPercentage;

/**
 * If set to true the pinch gesture will always zoom the map in/out around the 
 * center of the screen. When set to false (default) it'll use the center of
 * user gesture instead.
 */
@property (nonatomic) bool keepCurrentMapCenterDuringZooming;

/**
 * Defines a slight delay before labeling of the frame is invoked after paning has ended.
 * Values <0.5s allows the user to pan fast few times before the frame is re-labeled.
 * Caution: large value will make the map display significantly slower delaying every
 * frame with labeling. Value is defined in seconds.
 */
@property (nonatomic) float fastPanLabelingDelay;

/**
 * Configures whether or not the map should decelerate to a final position after the user stops panning.
 */
@property (nonatomic) bool applyPanningInertia;

/**
 * If set to true then the view automatically adjusts the map to be rendered upright.
 * If set to false the client has to force the map refresh. 
 * Also @see setInterfaceOrientationIsAllowed.
 */
@property (nonatomic) bool autoRotateViewContentToViewOrientation;

/**
 * Set this to the max number of labels the app wants to be placed. This comes 
 * handy when number of labels should be restricted,  * e.g. in a client with limited
 * screen real estate as a way to avoid cluttering with too many labels 
 * or as a performance tweak to shorten processing time.
 */
@property (nonatomic) int maxPlacedLabels;

/**
 * Set the flag whether a particular orientation is allowed for this view. It'll get returned
 * when shouldAutorotateToInterfaceOrientation is called.
 * @param allowed - Pass in true to allow the orientation, false otherwise.
 * @param orientation - The orientation being configured.
 */
- (void) setInterfaceOrientationIsAllowed:(bool) allowed forOrientation:(UIInterfaceOrientation) orientation;

/**
 * Configure the boundaries of the map in which user can pan. Only used in the automatic
 * panning handling mode.
 * @param allowedPanningArea - The area in which user may pan.
 */
- (void) setAllowedPanningArea:(MapText::Dtp::Rectangle&)allowedPanningArea;

/**
 * Minimal scale to which the view will zoom in when in automatic UI events handling mode.
 * It's the denominator of the scale (e.g. for 1:5'000 set 5'000 here).
 */
@property (nonatomic) float minAllowedScale;

/**
 * Maximal scale to which the view will zoom out when in automatic UI events handling mode.
 * It's the denominator of the scale (e.g. for 1:5'000'000 set 5'000'000 here).
 */
@property (nonatomic) float maxAllowedScale;

/**
 * If set to true the view will maintain north-up orientation of the map when in automatic
 * UI events handling mode. You also have to set the poleLocation for this functionality to work
 * properly.
 */
@property (nonatomic) bool maintainNorthUp;


/**
 * Location of the pole in map coordinates. It may be the north or south pole, depending on map
 * coverage. Hint: if the map covers both northern and southern hemispheres and still requires
 * north-up viewing the delegate should listen to the panning events and change the pole
 * coordinates from north pole to south pole when the center of the map passes the equator.
 * When the south pole coordinates are set you should also set the southPoleDefined flag
 * accordingly.
 * @see southPoleDefined.
 */
@property (nonatomic) CGPoint poleLocation;

/**
 * When the poleLocation reflects the south pole set this flag to true.
 */
@property (nonatomic) bool southPoleDefined;

/**@}*/

/**
 * The delegate who will get all the event notifications.
 */
@property (nonatomic, weak) id<MapViewDelegate> delegate;

/**
 * For the partial data loading of the master map define it's storage folder here.
 */
@property (nonatomic, strong) NSString* masterMapDataPath;

/**
 * Configure the font resources path (required).
 */
@property (nonatomic, strong) NSString* mapFontResourcesPath;

/**
 * If set to true it suppresses the default handling of memory warnings.
 * The user will need to handle clearing the cache.
 */
@property (nonatomic) bool dontClearCache;

/**
 * If set to true it suppresses the default error reporting (UIAlertViews).
 * It has no effect on the error events reported to the delegate.
 */
@property (nonatomic) bool dontShowErrorsAsAlerts;

/**
 * Check whether a background operation is in progress
 */
@property (nonatomic, readonly) bool backgroundOperationInProgress;

/**
 * @name Current display parameters
 * With those props you may configure the currently displayed frame
 */
/**@{*/

/**
 * Set current zoom level. It will define the current logical scale denominator
 * by multiplying the original map scale by itself:
 * @code logicalScale = originalLogicalScale/zoom @endcode
 */
@property (nonatomic) float zoom;

/**
 * Set current logical scale denominator. It will define the current zoom:
 * by multiplying the original map scale by itself:
 * @code zoom = originalLogicalScale/logicalScale @endcode
 */
@property (nonatomic) float logicalScale;

/**
 * Set the current angle of the map. The angle is interpreted as 
 * radian value of counter-clockwise rotation around the center of the view.
 */
@property (nonatomic) float mapAngle;

/**
 * Access the master map which is currently being used by the SDK.
 */
- (MapText::Dtp::Map*)map;

/**
 * Set the center of the viewport in master map coordinates.
 */
@property (nonatomic) CGPoint viewCenter;

/**
 * Lat long of the view center.
 */
- (CLLocation*) viewCenterLocationWithError:(MapText::Dtp::Error&)error;

/**
 * Lat long of the last touch location.
 */
- (CLLocation*) lastTouchedLocationWithError:(MapText::Dtp::Error&)error;

/**
 * Set the lat long location of the view center.
 * @param viewCenterLocation - The new location to set.
 * @param error - Error info. Setting the location through lat/long will not be possible
 *        if a data source without the projection information is used.
 */
- (void)setViewCenterLocation:(CLLocation *)viewCenterLocation withError:(MapText::Dtp::Error&)error;

/**
 * Access the PROJ4 projection string of the projection used to display the map.
 * *Discussion*
 * All data sources may report the projection in which the data is stored internally.
 * The first data source which is added to the MapView will implicitely define the display
 * projection if it hasn't been set before. Otherwise all the data will be reprojected
 * to the display projection. Data sources without projection information are supported 
 * but their geometries will not be reprojected at all, which may result in data not
 * lining up.
 */
@property (nonatomic, copy, readonly) NSString* displayProjectionString;


/**
 * Set the display projection by passing in a valid PROJ4 projection string.
 * @param projectionString - A valid PROJ4 string.
 * @param error - Error info (e.g. PROJ4 string is invalid).
 */
- (void)setDisplayProjection:(NSString*)projectionString withError:(MapText::Dtp::Error&)error;

/**
 * The area currently displayed by the map view.
 * Mimmics MKMapViewController region property.
 * *Caution*
 * This property will return a zero region if a data source without the projection
 * information is used.
 * *Discussion*
 * The region encompasses both the latitude and longitude point on which the map is
 * centered and the span of coordinates to display. The span values provide an implicit
 * zoom value for the map. The larger the displayed area, the lower the amount of zoom.
 * Similarly, the smaller the displayed area, the greater the amount of zoom.
 */
@property(nonatomic, readonly) MKCoordinateRegion viewRegion;

/**
 * Set the region to be displayed. The center of the region will become the center of the
 * screen and a zoom level will be picked up to fit the max dimension of the region passed.
 * @param region - Region to set the viewport to.
 * @param error - Error info. Setting the location through lat/long will not be possible
 *        if a data source without the projection information is used.
 */
- (void)setViewRegion:(MKCoordinateRegion)region withError:(MapText::Dtp::Error&)error;

/**
 * Set the region to be displayed in map units.
 * The center of the region will become the center of the
 * screen and a zoom level will be picked up to fit the max dimension of the region passed.
 * @param region - Region to set the viewport to, defined using map coordinates.
 * @param error - Error info.
 */
- (void)setViewRegionToMapRegion:(const MapText::Dtp::UprightRectangle &)region withError:(MapText::Dtp::Error &)error;

/**
 * Get a DataProjector allowing to convert data from lat/long (WGS-84) to map space coordinates.
 * @param error Error info. It will be populated in case of an error and a return value of NULL may be expected.
 * @return The projector which may be used by the client to perform the desired projection.
 */
- (MapText::Dtp::DataProjector*)getLatLongToMapUnitsProjectorWithError:(MapText::Dtp::Error&)error;

/**
 * Get a DataProjector allowing to convert data from map space coordinates to lat/long (WGS-84).
 * @param error Error info. It will be populated in case of an error and a return value of NULL may be expected.
 * @return The projector which may be used by the client to perform the desired projection.
 */
- (MapText::Dtp::DataProjector*)getMapUnitsToLatLongProjectorWithError:(MapText::Dtp::Error&)error;

/**
 * If set to YES, the map view keeps on showing the device location even after the location tracking has been
 * disabled (userTracking mode property == MKUserTrackingModeNone). The location will show as a gray ball instead
 * of the blue one to mark the location as
 * outdated. The default value is NO. It has no effect if shouldShowUserLocationWhenTracking is set to NO.
 */
@property (nonatomic) BOOL shouldShowLastKnownUserLocation;

/**
 * If set to YES the map view will show the current device location as a blue ball in the map. This will only be
 * shown when the location tracking is on by the user (userTracking mode property != MKUserTrackingModeNone)
 */
@property (nonatomic) BOOL shouldShowUserLocationWhenTracking;

///**
// * A Boolean value indicating whether the device’s current location is visible in the map view. (read-only)
// */
//@property(nonatomic, readonly) BOOL userLocationVisible;
//

/**
 * The user’s current location.
 */
- (CLLocation*) userLocation;

/**
 * The mode used to track the user location.
 */
@property(nonatomic) MKUserTrackingMode userTrackingMode;



/**
 * Change/check the symbology scaling factor. This allows you to make the symbology
 * larger/smaller without modifying the map itself. After setting the value
 * you should call labelMap method to update the labels and redraw content.
 * Caution: MapViewController provides a GUI component allowing the user
 * to change the scaling factor value. The slider position will be updated to reflect
 * the value set here, also the min/max allowed value of the slider will get
 * updated to allow the new value.
 */
@property (nonatomic) float mapSymbologyScalingFactor;

/**@}*/

/**
 * @name GUI components
 * These properties expose the GUI elements of the map view
 */
/**@{*/

/**
 * This is the attribution label. You can modify its properties using this
 * accessor. By default it shows as a 10pt, System font, 25% opacity text in the
 * lower right corner of the map.
 */
//@property (readonly) UILabel* attributionLabel;

/**
 * This is the map DPI control panel component. Accessing it allows the client
 * code to show/hide/change the appearance of it at will. Changing the symbology
 * DPI will make the text, symbols, lines etc. change size while maintaining 
 * the same map extent.
 */
@property (strong, nonatomic) IBOutlet UIView* symbologyDpiControlPanel;

/**
 * This is the map perspective control panel component.  Accessing it allows the client
 * code to show/hide/change the appearance of it at will.  The same map extent is used but
 * it is simply rendered with at a different perspective.
 * @warning The perspective feature is experimental at the moment.
 */
@property (strong, nonatomic) IBOutlet UIView* perspectiveControlPanel;

/**
 * This is the slider which allows the user to change the dpi used with the symbology.
 * Client code may set the min/max values the user can adjust within. The default range of DPI
 * is initialized to some reasonable defaults also taking the display resolution into account.
 */
@property (strong, nonatomic) IBOutlet UISlider *dpiSlider;

/**
 * This is the slider which allows the user to change the perspective amount used for rendering.
 * Client code may set the min/max values the user can adjust within. The default range of the perspective
 * is initialized to some reasonable defaults.
 * @warning The perspective feature is experimental at the moment.
 */
@property (strong, nonatomic) IBOutlet UISlider *perspectiveSlider;

/**
 * This is the layer control to allow the user to turn layers on/off in the  map schema
 */
@property (strong, nonatomic, readonly) UIViewController* layerControlViewController;

/**
 * This is the search filter control to allow the user to turn layers on/off in the map schema
 */
@property (strong, nonatomic, readonly) UIViewController* searchControlViewController;

@property (strong, nonatomic, readonly) UIViewController* geometryNetworkTraceToolsViewController;

/**
 * This tells the user if any layers in the map schema are setup for augmented reality
 */
@property (nonatomic, readonly) BOOL mapSchemaHasARAvailable;

/**
 * This gets the array of quick edit buttons that have been publidshed to the mapschema
 */
- (MapText::Dtp::OrderElements)mapSchemaEditableLayers;

/**
 * Set the active layer for editing
 */
- (void)setActiveEditLayer:(const char*)layerName;

/**
 * Client should embed this searchbar in the place iof its liking to allow the user
 * to search the map for features. Seach configuration is included in the map metadata and
 * currently not modifiable at runtime.
 */
@property (strong, nonatomic, readonly) UISearchBar* featureSearchBar;
@property (nonatomic, strong, readonly) UIBarButtonItem* settingsBarButtonItem;
@property (nonatomic, strong, readonly) UIBarButtonItem* layerControlBarButtonItem;
@property (nonatomic, strong, readonly) UIBarButtonItem* searchMapBarButtonItem;
@property (nonatomic, strong, readonly) UIBarButtonItem* mapScaleBarItem;
@property (nonatomic, strong, readonly) UIBarButtonItem* userTrackingModeBarButtonItem;
@property (nonatomic, strong, readonly) UIBarButtonItem* measureToolBarButtonItem;
@property (nonatomic, strong, readonly) UIBarButtonItem* dataCollectionBarButtonItem;

@property (nonatomic) bool mapViewGpsLocaterButtonHidden;
@property (nonatomic) bool shouldShowScaleBar;
/**@}*/

/**
 * Call into this method will resize the attribution label vertically and reposition it
 * to maintan the same padding from the map view bottom edge. It's useful to call into it
 * when the attribution may be of variable length on different maps.
 */
- (void) resizeAttributionLabelVerticallyToText;

/**
 * Add a png image to the map at specified location
 * @param pngId - id to uniquely represent this image
 * @param symbolId - id to uniquely represent the texture for this image
 * @param filePath - path to the PNG file
 * @param locaiton - location in the map to display the image
 */
- (void)addPNGToMapWithId:(std::string)pngId withSymbolId:(std::string)symbolId andFilePath:(std::string)filePath atLocation:(MapText::Dtp::Point&)location;

/**
 * Removed a png image from the map
 * @param pngId - id to uniquely represent this image
 */
- (void)removePNGFromMapWithID:(std::string)pngId;

/**
 * Add a drop pin to the map at specified location
 * @param pinId - id to uniquely identify this pin
 * @param location - location in the map to display the image
 * @param animated - flag should be animated.
 * @param error - Error info.
 * @param completion - Error info.
 */
- (void)addDropPin:(NSString*)pinId withLocation:(CLLocation*)location animated:(BOOL)animated withError:(MapText::Dtp::Error&)error completion:(BlockCompletion)block;

/**
 * Get an image from the current map on screen
 */
- (UIImage*)getMapImage:(MapText::Dtp::Error&)error;

- (std::vector<std::string>)displayFeaturePrimitiveFromGeoJson:(NSString*)geoJson error:(MapText::Dtp::Error&)error;

- (std::string)generateGeoJsonForLayer:(NSString*)layerName error:(MapText::Dtp::Error&)error;

- (std::string)exportEditsAsGeoJson:(NSString*)photosDirectory error:(MapText::Dtp::Error &)error;

- (void)importFeaturesFromGeoJson:(std::string)geoJsonString error:(MapText::Dtp::Error &)error;

- (void)removeFeaturePrimitiveWithUniqueId:(const char*)uniqueId;

- (void)removeFeaturePrimitivesWithUniqueIds:(std::vector<std::string>)uniqueIds;

/**
 * This method will export all the edits from the currently active editable data source to a
 * DtpHybridDataSource at the specified path. The generated DS will have the exact same schema
 * as the source, but will only contain edited features.
 * @param path - The path where exports DB shall be created.
 * @param zipContent - If YES the export DB will be zipped into a single archive. NOTE: this is required
 *                     for emailing the exports. NO will generate the export in the given path without
 *                     zipping. It will place the required resources in the correct places though (fonts, symbols etc).
 * @param sendAsEmail - If YES the user will be presented with the email composer the the zipped edits
 *        will be attached to the email draft. If the user cancels, or the sending fails for some reason
 *        this method will make sure the attachment is saved in the Documents folder, so that the client
 *        app can allow the user to download it using iTunes (iTunes file sharing has to be enabled on
 *        the app side). If set to YES the zipContent also has to be set to YES.
 * @param error - Error info.
 */
- (void)exportEditsToDbAtPath:(NSString*)path andZipTheContent:(BOOL)zipContent withCompletion:(void (^)(MapText::Dtp::Error& error))completion;

/**
 * This method will export all the edits from the currently active editable data source to a
 * csv file at the specified path.
 * @param path - The path where exports cvs shall be created.
 * @param error - Error info.
 */
- (void)exportEditsToCSVAtPath:(NSString*)path withCompletion:(void (^)(MapText::Dtp::Error& error))completion;


/**
 * This method will reset the 'edited' and 'geometry edited' special attributes across the whole
 * map. It may be useful to avoid exporting the same features multiple times.
 * @param error - Error info.
 */
- (void)unflagEditedFeaturesWithCompletion:(void (^)(MapText::Dtp::Error& error))completion;

/**
 * Test if the DB contains any user edits.
 * @return - true if any edits are present. False otherwise.
 */
- (bool)doesDatabaseContainAnyEditedFeaturesWithError:(MapText::Dtp::Error&)error;

/**
 * Create the vector outline of the size corresponding to the size of the PNG at the current dpi of the component.
 * This is needed only because the PointSymbol does not explicitly support raster point symbols.
 * So we need to rasterize the image and create a vector outline of the proper size up-front 
 * for the engine.
 * TODO: This should be removed once the proper PointSymbol extension is implemented. Then it should
 * be performed by the engine itself.
 * @param[out] outSymbol - The symbol to supply with outline based on the PNG.
 * filePath
 * @param symbolAngle - angle of symbol
 * @param filePath - Full path to the PNG file.
 * @param error - Error info.
 */
- (void)buildPointSymbol:(MapText::Dtp::PointSymbol&)outSymbol basedOnPngAtPath:(NSString*)filePath andAngle:(int)symbolAngle withError:(MapText::Dtp::Error&)error;

+ (NSDictionary*)loadSymbolConfigFromFile:(NSString*)fileName withError:(MapText::Dtp::Error&) error;

/**
 * This is a *temporary* interface on the SDK to read feature's attribution. This has to go away once DataSource 
 * implements reading of attributes and the Data Access Layer is complete. Also this method assumes there's only one
 * data source and uses it's database for attibutes retrieval, which is very limiting.
 * @param features - List of features which the client code wants to read attributes for.
 * @param[out] featureRecords - The list of DB records read for the list of features provided.
 * @param[out] error - Error information.
 */
- (void)readAttibutesForFeatures:(const std::vector<MapText::Dtp::Feature*>&)features toFeatureRecordList:(MapText::Dtp::FeatureRecords&)featureRecords withError:(MapText::Dtp::Error&)error;


/**
	Get a feature lookup controller with the provided list of feature records as the model.
    All the controller
    delegation is handled by the SDK. The client only has to include the controller
    in the view controller/view hierarchy (e.g. show it as a modal view or inside a
    popover). Example:
 
    <code>
        UIViewController* dataLookup = [self.mapViewController getFeatureLookupControllerForFeatureRecordList:featureRecords];
        
        UINavigationController* naviController = [[[UINavigationController alloc] initWithRootViewController:dataLookup] autorelease];
        naviController.topViewController.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"Dismiss"
                                                                                                              style:UIBarButtonItemStylePlain 
                                                                                                             target:self 
                                                                                                             action:@selector(dismissModalViewControllerAnimated:)];
        [self presentViewController:naviController animated:YES completion:nil];
    </code>
	@param featureRecords - Input list of feature records. They will become the rows in the
           view controller. User is then able to get into details of any of them. The ownership 
           over the records is *not* taken. As a consequence the client code has to make sure they
           are not being deallocated while the view controller is visible.
	@returns - DataLookupController as an opaque UIViewController. The controller is autoreleased.
 */
- (UIViewController*)getFeatureLookupControllerForFeatureRecordList:(MapText::Dtp::FeatureRecords&)featureRecords;

- (void) updateStatus;

- (void) performBlock:(BlockTask)block;

/**
 * Current SDK settings.
 */
@property (nonatomic, strong) MappingSDKSettings* settings;

- (void)addMappingSDKSetting:(NSString*)name groupName:(NSString*)groupName withActionBlock:(ActionBlock)actionBlock withConfigurationBlock:(CellConfigurationBlock)configBlock;

/**
 * The current state of the map view
 */
@property (nonatomic, strong) MapViewState* mapViewState;

/**
 * Allow user to update the current mapview state
 * @param mapViewState - a dictionary with the new map view state
 */
- (void) updateMapWithMapViewState:(MapViewState*)mapViewState;

/**
 * If set to NO, user will have attribute lookup table displayed in read-only mode
 */
- (void) setAttributeLookupEditable:(bool)value;

/**
 * External provider of HTML forms to use with the current map.
 */
@property(nonatomic, strong) id<MapFormsProvider> externalFormsProvider;

/**
 * External provider of PhotoManager which can be used for photo downloading process
 */
@property(nonatomic, strong) id<PhotoManager> photoManager;


@end
