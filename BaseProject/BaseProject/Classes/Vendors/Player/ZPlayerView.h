//
//  ZPlayerView.h
//  BaseProject
//
//  Created by bigfish on 2018/11/16.
//  Copyright © 2018 bigfish. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DrawCallbackInterface.h"

@interface ZPlayerView : UIView


@property drawTest rCall;
//@property initShaderInterface sCall;

- (void)setup;
- (void)render:(ZTextureType)type w:(int)width h:(int)height data:(unsigned char *[])data;
- (void)setShader:(ZShaderType)tpye;


@end

