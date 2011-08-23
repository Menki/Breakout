//
//  BreakoutAppController.h
//  Breakout
//
//  Created by Augusto Souza on 8/23/11.
//  Copyright Menki Mobile 2011. All rights reserved.
//

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController	*viewController;
}

@end

