//
//  YUVView.h
//  OpenGL_YUV_detec
//
//  Created by Ruiwen Feng on 2017/5/19.
//  Copyright © 2017年 Ruiwen Feng. All rights reserved.
//

#import <UIKit/UIKit.h>
//#import "ObjectInterface.h"
#import "DrawCallbackInterface.h"
@interface YUVView : UIView

- (void)setUp;
//- (void)displayYUVI420Data:(void *)data width:(GLuint)width height:(GLuint)height;
@property drawTest call; // 回调函数

-(void)GetTexture:(GLuint)index width:(int)width height:(int)height buf:(unsigned char * []) buf isa:(bool)isa;

@end
