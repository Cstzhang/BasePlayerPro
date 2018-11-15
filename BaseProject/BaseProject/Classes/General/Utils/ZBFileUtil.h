//
//  ZBFileUtil.h
//  BaseProject
//
//  Created by bigfish on 2018/11/14.
//  Copyright © 2018 bigfish. All rights reserved.
//

#import <Foundation/Foundation.h>



@interface ZBFileUtil : NSObject

+(NSString *)bundlePath:(NSString *)fileName;

+(NSString *)documentsPath:(NSString *)fileName;

@end


