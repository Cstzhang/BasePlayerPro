//
//  ZBHomeViewController.m
//  BaseProject
//
//  Created by bigfish on 2018/10/31.
//  Copyright © 2018 bigfish. All rights reserved.
//

#import "ZBHomeViewController.h"
#import "IPlayerProxy.h"
#import "ZPlayerView.h"
@interface ZBHomeViewController ()

@property (nonatomic,strong) ZPlayerView *playView;
@end

@implementation ZBHomeViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    self.playView = [[ZPlayerView alloc]initWithFrame:CGRectMake(0, 0, 352, 288)];
//   [self.playView setup];
    [self.view addSubview:self.playView];
    
 
    
    //test http

//    [self testHUD];
//    [self testHttp];
    

}




- (IBAction)TestPlayEvent:(id)sender {
    NSLog(@"test play video");
    void* point = (__bridge void*)self.playView;
    
    ////    NSString * path = @"rtsp://admin:cvte123456@172.18.223.100:554/mpeg4/ch1/sub/av_stream";
    IPlayerProxy::Get()->Init();
    IPlayerProxy::Get()->BindView(point,(void *)(_playView.rCall),(void *)(_playView.sCall));
    NSString *videoPath = [ZBFileUtil bundlePath:@"1080.mp4"];
    IPlayerProxy::Get()->Open(videoPath.UTF8String);
    IPlayerProxy::Get()->Start();
    
}













@end
