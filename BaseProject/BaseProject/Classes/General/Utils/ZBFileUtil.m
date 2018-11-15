//
//  ZBFileUtil.m
//  BaseProject
//
//  Created by bigfish on 2018/11/14.
//  Copyright © 2018 bigfish. All rights reserved.
//

#import "ZBFileUtil.h"

@implementation ZBFileUtil

+ (NSString *)bundlePath:(NSString *)fileName {
    return [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:fileName];
}

+ (NSString *)documentsPath:(NSString *)fileName {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    return [documentsDirectory stringByAppendingPathComponent:fileName];
}


@end
