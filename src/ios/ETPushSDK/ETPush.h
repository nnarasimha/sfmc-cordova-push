//
//  ETPush.h
//  JB4A-SDK-iOS
//
//  JB4A iOS SDK GitHub Repository
//  https://salesforce-marketingcloud.github.io/JB4A-SDK-iOS/

//  Copyright © 2016 Salesforce Marketing Cloud. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "ETLocationManager.h"
#import "PushConstants.h"
#import <Availability.h>

NS_ASSUME_NONNULL_BEGIN

#pragma mark - Constants
static NSString * const SDKName = @"JB4ASDK";


#pragma mark - Protocol

/**
 Supporting protocol for OpenDirect, part of the Salesforce 2013-02 release.
 
 Implementation of this delegate is not required for OpenDirect to function, but it is provided as a convenience to developers who do not with to parse the push payload on their own.
 
 All OpenDirect data is passed down as a JSON String, so you get it as an NSString. Please remember to parse it appropriately from there. Also, please remember to fail gracefully if you can't take action on the message.
 
 Also, please note that setting an OpenDirect Delegate will negate the automatic webpage loading feature added to MobilePush recently. This is deliberately to not stomp on your URLs and deep links.
 */
@protocol ExactTargetOpenDirectDelegate <NSObject>

/**
 Method called when an OpenDirect payload is received from MobilePush.
 
 @param payload value NSString. The contents of the payload as received from MobilePush.
 @return Doesn't return a value.
 */
-(void)didReceiveOpenDirectMessageWithContents:(NSString *)payload;

#pragma mark - Optional

@optional

/**
 Allows you to define the behavior of OpenDirect based on application state.
 
 If set to YES, the OpenDirect delegate will be called if a push with an OpenDirect payload is received and the application state is running. This is counter to normal push behavior, so the default is NO.
 
 Consider that if you set this to YES, and the user is running the app when a push comes in, the app will start doing things that they didn't prompt it to do. This is bad user experience since it's confusing to the user. Along these lines, iOS won't present a notification if one is received while the app is running.
 
 @return BOOL representing whether or not you want action to be taken.
 */
-(BOOL)shouldDeliverOpenDirectMessageIfAppIsRunning;

@end

/**
 Supporting protocol for Cloud Pages with Alert, part of the Salesforce 2016-04 release.
 
 Implementation of this delegate is not required for CloudPage with Alert to function, but it is provided as a convenience to developers who do not wish to parse the push payload on their own.
 
 All CloudPage data is passed down as a JSON String, so you get it as an NSString. Please remember to parse it appropriately from there. Also, please remember to fail gracefully if you can't take action on the message.
 
 Also, please note that setting a CloudPage with Alert Delegate will negate the automatic webpage load to inbox feature added to MobilePush.
 */
@protocol ExactTargetCloudPageWithAlertDelegate <NSObject>

/**
 Method called when an Cloud Page with Alert payload is received from MobilePush.
 
 @param payload value NSString. The contents of the payload as received from MobilePush.
 @return Doesn't return a value.
 */
-(void)didReceiveCloudPageWithAlertMessageWithContents:(NSString *)payload;

#pragma mark - Optional

@optional

/**
 Allows you to define the behavior of Cloud Page with Alert based on application state.
 
 If set to YES, the Cloud Page with Alert delegate will be called if a push with an Cloud Page with Alert payload is received and the application state is running. This is counter to normal push behavior, so the default is NO.
 
 Consider that if you set this to YES, and the user is running the app when a push comes in, the app will start doing things that they didn't prompt it to do. This is bad user experience since it's confusing to the user. Along these lines, iOS won't present a notification if one is received while the app is running.
 
 @return BOOL representing whether or not you want action to be taken.
 */
-(BOOL)shouldDeliverCloudPageWithAlertMessageIfAppIsRunning;

@end

#pragma mark - Interface

/**
 This is the main interface to the Salesforce MobilePush SDK. It is meant to handle a lot of the heavy lifting with regards to sending data back to Salesforce.
 
 Please note that this is a singleton object, and you should reference it as [ETPush pushManager].
 */
@interface ETPush : NSObject
{
    NSDate *_sessionStart;
    NSString *_messageID;
    
    BOOL _showLocalAlert;
    
    // OpenDirect Delegate stuff
    id<ExactTargetOpenDirectDelegate> _openDirectDelegate;
    
    // CloudPageWithAlert Delegate stuff
    id<ExactTargetCloudPageWithAlertDelegate> _cloudPageWithAlertDelegate;
    
}

#pragma mark - Configure the App for ETPush
/**---------------------------------------------------------------------------------------
 * @name Configuring the app for ETPush
 *  ---------------------------------------------------------------------------------------
 */

/**
 Returns (or initializes) the shared pushManager instance.
 
 @return The singleton instance of an ETPush pushManager.
 */
+(nullable instancetype)pushManager;
-(nullable instancetype)init;

/**
 This is the main configuration method, responsible for setting credentials needed to communicate with Salesforce. If you are unsure of your accessToken or environment, please visit Code@ExactTarget
 
 Each of the flags in the method are used to control various aspects of the MobilePush SDK. The act as global on/off switches, meaning that if you disable one here, it is off eveywhere.
 
 @param etAppID The App ID generated by Code@ExactTarget to identify the consumer app
 @param accessToken The designed token given to you by Code@ExactTarget that allows you access to the API
 @param analyticsState Whether or not to send analytic data back to Salesforce
 @param locState Whether or not to use Location Services
 @param proximityState Whether or not to use Proximity Services. Using proximity services requires setting locState to YES
 @param cpState Whether or not to use Cloud Pages
 @param pIAnalyticsState Whether or not to send Web and Mobile analytic data back to Salesforce
 @param configureError NSError object describing the error
 @return Returns YES if successful or NO if failed. Do not proceed if NO is returned
 */
- (BOOL)configureSDKWithAppID:(NSString *)etAppID
               andAccessToken:(NSString *)accessToken
                withAnalytics:(BOOL)analyticsState
          andLocationServices:(BOOL)locState
         andProximityServices:(BOOL)proximityState
                andCloudPages:(BOOL)cpState
              withPIAnalytics:(BOOL)pIAnalyticsState
                        error:(NSError **)configureError;

/**
 Sets the OpenDirect delegate.
 
 @param delegate The object you wish to be called when an OpenDirect message is delivered.
 @return Doesn't return a value.
 */
-(void)setOpenDirectDelegate:(nullable id<ExactTargetOpenDirectDelegate>)delegate;

/**
 Returns the OpenDirect delegate.
 
 @return delegate The named OpenDirect delegate, or nil if there isn't one.
 */
-(nullable id<ExactTargetOpenDirectDelegate>)openDirectDelegate;

/**
 Sets the cloudPageWithAlert delegate.
 
 @param delegate The object you wish to be called when an OpenDirect message is delivered.
 @return Doesn't return a value.
 */
-(void)setCloudPageWithAlertDelegate:(nullable id<ExactTargetCloudPageWithAlertDelegate>)delegate;

/**
 Returns the cloudPageWithAlert delegate.
 
 @return delegate The named cloudPageWithAlert delegate, or nil if there isn't one.
 */
-(nullable id<ExactTargetCloudPageWithAlertDelegate>)cloudPageWithAlertDelegate;

/**
 Triggers an immediate send of Registration data to Salesforce Marketing Cloud and will wait 60 seconds to send 
 for all calls made after the first call was made while the app is in foreground.
 
 This is not normally needed as each method (setTag(), setSubscriberKey(), addAttribute() etc) will trigger
 a send to the SFMC 60 seconds after the first request to change any Registration data.
 */
-(void)updateET;

#pragma mark - Push Lifecycle
/**---------------------------------------------------------------------------------------
 * @name Push Lifecycle
 *
 * These methods are required to make push function on iOS, and to enable the ET SDK to utilize it. All of these methods are required.
 *  ---------------------------------------------------------------------------------------
 */

// Refer to Availability.h for the reasoning behind why the following #if's are used.
// Basically, this will allow the code to be compiled for different IPHONEOS_DEPLOYMENT_TARGET values to
// maintain backward compatibility for running on IOS 6.0 and up as well allowing for using different versions
// of the IOS SDK compiled using XCode 5.X, XCode 6.X and up without getting depricated warnings or undefined warnings.

// IPHONEOS_DEPLOYMENT_TARGET = 6.X or 7.X
#if __IPHONE_OS_VERSION_MIN_REQUIRED < 80000
// Supports IOS SDK 8.X (i.e. XCode 6.X and up)
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 80000
/**
 Wrapper for iOS' application:registerForRemoteNotification; call. It will check that push is allowed, and if so, register with Apple for a token. If push is not enabled, it will notify Salesforce that push is disabled.
 
 @return Doesn't return a value
 */
-(void)registerForRemoteNotifications;

/**
 Wrapper for iOS' isRegisteredForRemoteNotifications; call.
 
 @return BOOL
 */
- (BOOL)isRegisteredForRemoteNotifications;

/**
 Wrapper for iOS' application:registerUserNotificationSettings; call.
 
 @param notificationSettings The UIUserNotificationSettings object that the application would like to use for push. These are pipe-delimited, and use Apple's native values
 @return Doesn't return a value
 */
- (void)registerUserNotificationSettings:(UIUserNotificationSettings *)notificationSettings;

/**
 Wrapper for iOS' currentUserNotificationSettings; call.
 
 @return Doesn't return a value
 */
- (nullable UIUserNotificationSettings *)currentUserNotificationSettings;

/**
 Wrapper for iOS' didRegisterUserNotificationSettings; callback.
 
 @param notificationSettings a UIUserNotificationSettings class value.
 @return no value returned.
 */
- (void)didRegisterUserNotificationSettings:(UIUserNotificationSettings *)notificationSettings;

/**
 Wrapper for iOS' application:registerForRemoteNotificationTypes; call. It will check that push is allowed, and if so, register with Apple for a token. If push is not enabled, it will notify Salesforce that push is disabled.
 
 @deprecated June 4 2015
 @param types The UIRemoteNotificationTypes that the application would like to use for push. These are pipe-delimited, and use Apple's native values
 @return Doesn't return a value
 */
-(void)registerForRemoteNotificationTypes:(UIRemoteNotificationType)types;
#else
// Supports IOS SDKs < 8.X (i.e. XCode 5.X or less)
/**
 Wrapper for iOS' application:registerForRemoteNotificationTypes; call. It will check that push is allowed, and if so, register with Apple for a token. If push is not enabled, it will notify Salesforce that push is disabled.
 
 @param types The UIRemoteNotificationTypes that the application would like to use for push. These are pipe-delimited, and use Apple's native values
 @return Doesn't return a value
 */
-(void)registerForRemoteNotificationTypes:(UIRemoteNotificationType)types;
#endif
#else
// IPHONEOS_DEPLOYMENT_TARGET >= 8.X
// Supports IOS SDK 8.X (i.e. XCode 6.X and up)
/**
 Wrapper for iOS' application:registerForRemoteNotification; call. It will check that push is allowed, and if so, register with Apple for a token. If push is not enabled, it will notify Salesforce that push is disabled.
 
 @return Doesn't return a value
 */
-(void)registerForRemoteNotifications;

/**
 Wrapper for iOS' isRegisteredForRemoteNotifications; call.
 
 @return BOOL
 */
- (BOOL)isRegisteredForRemoteNotifications;

/**
 Wrapper for iOS' application:registerUserNotificationSettings; call.
 
 @param notificationSettings The UIUserNotificationSettings object that the application would like to use for push. These are pipe-delimited, and use Apple's native values
 @return Doesn't return a value
 */
- (void)registerUserNotificationSettings:(UIUserNotificationSettings *)notificationSettings;

/**
 Wrapper for iOS' currentUserNotificationSettings; call.
 
 @return Returns the current UIUserNotificationSettings object
 */
- (UIUserNotificationSettings *)currentUserNotificationSettings;

/**
 Wrapper for iOS' didRegisterUserNotificationSettings; callback.
 
 @return Doesn't return a value
 */
- (void)didRegisterUserNotificationSettings:(UIUserNotificationSettings *)notificationSettings;
#endif

/**
 Responsible for sending a received token back to Salesforce. It marks the end of the token registration flow. If it is unable to reach ET server, it will save the token and try again later.
 
 This method is necessary to implementation of ET Push.
 
 @param deviceToken Token as received from Apple, still an NSData object
 @return Doesn't return a value
 */
-(void)registerDeviceToken:(NSData *)deviceToken;

/**
 Returns the device token as a NSString. As requested via GitHub (Issue #3).
 
 @return A stringified version of the Device Token
 */
-(nullable NSString *)deviceToken;

/**
 Handles a registration failure.
 
 @param error The error returned to the application on a registration failure
 @return Doesn't return a value
 */
-(void)applicationDidFailToRegisterForRemoteNotificationsWithError:(NSError *)error;

/**
Reset the application's badge number to zero (aka, remove it). Call updateET to refresh the server with the current badge number. Note: updateET may not be fully processed by the server for a number of minutes; the server's badge value may be out of sync with the app for a short amount of time.
 
 @return Doesn't return a value
 */
-(void)resetBadgeCount;

/**
 Tell the SDK to display a UIAlertView if a push is received while the app is already running. Default behavior is set to NO.
 
 Please note that all push notifications received by the application will be processed, but iOS will *not* present an alert to the user if the app is running when the alert is received. If you set this value to true (YES), then the SDK will generate and present the alert for you. It will not play a sound, though.
 
 @param desiredState YES/NO if you want to display an alert view while the app is running.
 @return Doesn't return a value
 */
-(void)shouldDisplayAlertViewIfPushReceived:(BOOL)desiredState;


#pragma mark - Application Lifecycle
/**---------------------------------------------------------------------------------------
 * @name Application Lifecycle
 *
 * These methods are not necessary for the Push lifecycle, but are required to make the ET Push SDK perform as expected
 *  ---------------------------------------------------------------------------------------
 */

/**
 Notifies the ET SDK of an app launch, including the dictionary sent to the app by iOS. The launchOptions dictionary is necessary because it will include the APNS dictionary, necessary for processing opens and other analytic information.
 
 @param launchOptions The dictionary passed to the application by iOS on launch.
 @return Doesn't return a value
 */
-(void)applicationLaunchedWithOptions:(nullable NSDictionary *)launchOptions;

/**
 Notifies the ET SDK of an app termination. Internally, this method does a lot of cleanup.
 
 @return Doesn't return a value (but how could it - the app terminated)
 */
-(void)applicationTerminated;

/**
Handles a push notification received by the app when the application is already running.

 This method must be implemented in UIApplication sharedApplication didReceiveRemoteNotification:userInfo.
 Sometimes, when a push comes in, the application will already be running (it happens). This method rises to the occasion of handing that notification, displaying an Alert (if the SDK is configured to do so), and calling all of the analytic methods that wouldn't be called otherwise.
 
 @param userInfo The dictionary containing the push notification
 @param applicationState State of the application at time of notification
 @return Doesn't return a value.
 */
-(void)handleNotification:(NSDictionary *)userInfo forApplicationState:(UIApplicationState)applicationState;

/**
 Handles a local notification received by the application.
 
 Sometimes the SDK will use local notifications to indicate something to the user. These are handled differently by iOS, and as such, need to be implemented differently in the SDK. Sorry about that.
 
 @param notification The received UILocalNotification
 @return Doesn't return a value
 */
-(void)handleLocalNotification:(UILocalNotification *)notification;


#pragma mark - Data Interaction
/**---------------------------------------------------------------------------------------
 * @name Data Interaction
 *  ---------------------------------------------------------------------------------------
 */

/**
 Accepts and sets the Subscriber Key for the device's user.
 
 Cannot be nil or blank.
 
 Will trim leading and trailing whitespace.
 
 @param subscriberKey The subscriber key to attribute to the user.
 @return YES if set successfully
 */
-(BOOL)setSubscriberKey:(NSString *)subscriberKey;

/**
 Returns the subscriber key for the active user, in case you need it.
 
 @return subscriberKey The code-set subscriber key.
 */
-(nullable NSString *)getSubscriberKey;

/**
 Adds the provided Tag (NSString) to the set of unique tags.
 
 Will trim leading and trailing whitespace.
 
 Cannot be nil or blank.
 
 @param  tag A string to add to the list of tags
 @return YES if added successfully.
 */
-(BOOL)addTag:(NSString *)tag;

/**
 Removes the provided Tag (NSString) from the list of tags.
 
 @param tag A string to remove from the list of tags
 @return tag Echoes the tag back on successful removal, or nil if something failed.
 */
-(nullable NSString *)removeTag:(NSString *)tag;

/**
 Returns the list of tags for this device.
 
 @deprecated Feb 29 2016
 @return All tags associated.
 */
-(NSSet *)allTags DEPRECATED_MSG_ATTRIBUTE("allTags is deprecated. Please use getTags instead.");

/**
 Returns the list of tags for this device.
 
 @return All tags associated.
 */
-(NSSet *)getTags;

/**
 Adds an attribute to the data set sent to Salesforce.
 
 The Attribute Name cannot be nil or blank, or one of the reserved words.
 
 Will trim leading and trailing whitespace from the name and value.
 
 The attribute must be defined within the SFMC Contact model prior to adding a value.  If the attribute does not exist within the
 SFMC Contact model, then this attribute will be accepted by the SDK, but will be ignored within the SFMC.
 
 If you previously added a value for the named attribute, then the value will be updated with the new value and sent to the SFMC.
 
 If you send in a blank value, then the value will be sent to the SFMC to remove that value from the Contact record.

 All attribute values set with this method persist through the installation of the app on your customer device.
 
 Note that attribute mapping is case sensitive, and spaces should be avoided when setting up new attributes in the SFMC Contact model.
 
 @param name The name of the attribute you wish to send. This will be the key of the pair.
 @param value The value to set for the data pair.
 @return YES if added successfully
 */
- (BOOL)addAttributeNamed:(NSString*)name value:(NSString*)value;

/**
 Removes the provided attribute from the data set to send to Salesforce.  The value is not changed on the SFMC.
 
 @param name The name of the attribute you wish to remove.
 @return Returns the value that was set. It will no longer be sent back to Salesforce.
 */
- (nullable NSString*)removeAttributeNamed:(NSString*)name;

/**
 Returns a read-only copy of the Attributes dictionary as it is right now.
 
 @deprecated Feb 29 2016
 @return All attributes currently set
 */
-(NSDictionary *)allAttributes DEPRECATED_MSG_ATTRIBUTE("allAttributes has been deprecated. Please use getAttributes instead");

/**
 Returns a read-only copy of the Attributes dictionary as it is right now.
 
 @return All attributes currently set
 */
-(NSDictionary *)getAttributes;

/**
 @name Listeners for UIApplication events
 */

/**
 Sets up the listeners.
 */
-(void)startListeningForApplicationNotifications;

/**
 Drops the listeners.
 */
-(void)stopListeningForApplicationNotifications;

/**
 Responds to the UIApplicationDidBecomeActiveNotification notification
 */
-(void)applicationDidBecomeActiveNotificationReceived; // UIApplicationDidBecomeActiveNotification

/**
 Responds to the UIApplicationDidEnterBackgroundNotification notification
 */
-(void)applicationDidEnterBackgroundNotificationReceived; // UIApplicationDidEnterBackgroundNotification

#pragma mark - ETPush Convenience Methods
/**
 @name ETPush Convenience Methods
 */

/**
 Gets the Apple-safe, unique Device Identifier that ET will later use to identify the device.
 
 Note that this method is compliant with Apple's compliance rules, but may not be permanent.
 */
+(NSString *)safeDeviceIdentifier;

/**
 Returns the hardware identification string, like "iPhone1,1". Salesforce uses this data for segmentation.
 
 @return A string of the hardware identification.
 */
+(NSString *)hardwareIdentifier;

/**
 Returns whether this device can receive Push Notifications.
 
 Push is considered enabled if the application is able to present an alert (banner, alert, sound) to the user.  If UIUserNotificationTypeNone (ios8+) or
 UIRemoteNotificationTypeNone (iOS7) is true, then Push is disabled and this method returns false.  For iOS8+, isRegisteredForRemoteNotifications
 must also return true.
 */
+(BOOL)isPushEnabled;

/**
 Get SDK Version Name
 */
+(NSString *)getSDKVersionName;

/**
 Get SDK Version Code
 
 @return NSString value.
 */
+(NSString *)getSDKVersionCode;

/**
 Set the Log Level
 
 @param state type BOOL value.
 @return nothing to return.
 */
+(void)setETLoggerToRequiredState:(BOOL)state;

/**
 To override the logging that the SDK does with your own logging code.  Adding this logging handler will
 replace the logs that the SDK prints to the console.
 
 Typical uses would be to save the logs in your own logging datastore instead of printing to the console.
 
 NOTE: Do not base any program logic on the contents of the String received as a parameter as we do not guarantee
 the contents of a log string will remain the same from one release to another.
 
 @param customLogger - A completion handler that will be called each time the SDK logs to the console.  Instead of
                       calling NSLog to log to the console, the SDK will call this customLogger and provide the String
                       that was going to be logged.
 @return - No value returned.
 */
+(void) setLoggerWithHandler:( void (^__nullable)(NSString *))customLogger;

/**
 Outputs a formatted, easily readable block of text describing the current status of the SDK.
 
 @return JSON string with values of the current state of the SDK
 */
+(nullable NSString *)getSDKState;

/**
 Ask ETPush to update its data. ETPush will throttle attempts based on the time since the last time this was called.
 
 @param completionHandler The UIBackgroundFetchResult completion handler. This method will be called with UIBackgroundFetchResultNoData if no attempt was made to update data, otherwise it will be called with UIBackgroundFetchResultNewData after the update completes. If nil is passed, then process of the completion handler must be managed by the caller.
 @return True if ETPush did make an attempt at updating data
 */
- (BOOL) refreshWithFetchCompletionHandler:(void (^__nullable)(UIBackgroundFetchResult result))completionHandler;

/**
 Used to retrieve a JSON formatted string of recommendations from the Predictive Web section of the Marketing Cloud.
 
 @param mid An identifier used to locate the application specific recommendations
 @param page A recommendations page that has been created within the Predictive Web section of the Marketing Cloud
 @param error A pointer to a location to store an NSError object describing any error that occurred while sanity checking the input parameters. Can be nil.
 @param completionHandler A pointer to a user-supplied completion handler block. The completion handler will be called if requestPIRecommendations returns True. The result parameter of the completion handler will contain a JSON formatted string with the recommendations on a successfull call otherwise it will be nil and the error parameter will be filled with an NSError object describing the error.
 @return Returns T/F based on sanity checks of the input parameters. If False is returned the user completion handler will not be called.
 */
+(BOOL)requestPIRecommendations:(NSString *)mid page:(NSString *)page error:(NSError **)error completionHandler:(void (^)(NSString *result, NSError *error))completionHandler;
/**
 Used to retrieve a JSON formatted string of recommendations from the Predictive Web section of the Marketing Cloud.
 
 @param mid An identifier used to locate the application specific recommendations
 @param page A recommendations page that has been created within the Predictive Web section of the Marketing Cloud
 @param retailer An identifier used to locate the application specific recommendations
 @param error A pointer to a location to store an NSError object describing any error that occurred while sanity checking the input parameters. Can be nil.
 @param completionHandler A pointer to a user-supplied completion handler block. The completion handler will be called if requestPIRecommendations returns True. The result parameter of the completion handler will contain a JSON formatted string with the recommendations on a successfull call otherwise it will be nil and the error parameter will be filled with an NSError object describing the error.
 @return Returns T/F based on sanity checks of the input parameters. If False is returned the user completion handler will not be called.
 */
+(BOOL)requestPIRecommendations:(NSString *)mid page:(NSString *)page retailer:(NSString *)retailer error:(NSError **)error completionHandler:(void (^)(NSString *result, NSError *error))completionHandler;


+(NSArray *)getCloudPages;

@end
NS_ASSUME_NONNULL_END

