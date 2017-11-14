//
// Created by Grzegorz Zaborowski on 13/07/2017.
//

#import <Foundation/Foundation.h>


@protocol PhotoManagerDelegate

- (void)downloadStarted;
- (void)downloadProgressRatioChanged:(CGFloat)newRatio;
- (void)downloadFinished:(NSString *)localPath;
- (void)downloadFailed:(NSError *)error;

@end


@protocol PhotoManager

/**
 * By subscribing a photo for download you may expect that the download process will start automatically.
 * PhotoManager will be calling back the delegate with the progress and at the end it will inform about success/error.
 *
 * NOTE:
 * You may subscribe the same photo many times.
 * It will either create a new download process or connect to the existing one.
 */
- (void)subscribePhotoForDownload:(NSString *)filename forMap:(NSString *)mapID withDelegate:(id <PhotoManagerDelegate>)delegate;

@end
