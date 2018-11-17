//
//  ZPlayerView.m
//  BaseProject
//
//  Created by bigfish on 2018/11/16.
//  Copyright © 2018 bigfish. All rights reserved.
//

#import "ZPlayerView.h"
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#include "ZShader.h"

void RrenderWith(void * caller ,ZTextureType type, int width, int height, unsigned char *data[]){
    [(__bridge id)caller render:type w:width h:height data:data];
}

void InitShaderWith(void * caller ,ZShaderType type){
    [(__bridge id)caller setShader:type];
}

@interface ZPlayerView()
{
    CAEAGLLayer *_eaglLayer;
    EAGLContext *_context;
    GLuint       _framebuffer;
    GLuint       _renderbuffer;
    ZShader      _shader;
}



@end

@implementation ZPlayerView


- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        _rCall = RrenderWith;
        _sCall = InitShaderWith;
        [self setupLayer];
        [self setupContext];
        [self setupRenderBuffer];
        [self setupFrameBuffer];
        NSError *error;
        NSAssert1([self checkFramebuffer:&error], @"%@",error.userInfo[@"ErrorMessage"]);
    }
    return self;
}


+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)setupLayer {
    // 用于显示的layer
    _eaglLayer = (CAEAGLLayer *)self.layer;
    
    // CALayer 默认是透明的（opaque = NO），而透明的层对性能负荷很大。所以将其关闭。
    _eaglLayer.opaque = YES;
    //以及颜色存储格式。
    _eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                         [NSNumber numberWithBool:NO],
                                     kEAGLDrawablePropertyRetainedBacking,
                                         kEAGLColorFormatRGBA8,
                                     kEAGLDrawablePropertyColorFormat,
                                         nil];
}

- (void)setupContext {
    if (!_context) {
        // 创建GL环境上下文
        // EAGLContext 管理所有通过 OpenGL ES 进行渲染的信息.
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        //setCurrentContext
        BOOL re;
        re = [EAGLContext setCurrentContext:_context];
        if (!re) {
            NSLog(@"setCurrentContext failed!");
        }
    }else{
        NSLog(@"creat EAGLContext failed!");
    }
//    NSAssert(_context && [EAGLContext setCurrentContext:_context], @"初始化GL环境失败");
   
}

- (void)setupRenderBuffer {
    // 生成 renderbuffer ( renderbuffer = 用于展示的窗口 )
    glGenRenderbuffers(1, &_renderbuffer);
    // 绑定 renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
    // GL_RENDERBUFFER 的内容存储到实现 EAGLDrawable 协议的 CAEAGLLayer
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
}

- (void)setupFrameBuffer {
    // 释放旧的 framebuffer
    if (_framebuffer) {
        glDeleteFramebuffers(1, &_framebuffer);
        _framebuffer = 0;
    }
    
    // 生成 framebuffer ( framebuffer = 画布 )
    glGenFramebuffers(1, &_framebuffer);
    // 绑定 fraembuffer
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    
    // framebuffer 不对渲染的内容做存储, 所以这一步是将 framebuffer 绑定到 renderbuffer ( 渲染的结果就存在 renderbuffer )
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, _renderbuffer);
}


- (BOOL)checkFramebuffer:(NSError *__autoreleasing *)error {
    // 检查 framebuffer 是否创建成功
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    NSString *errorMessage = nil;
    BOOL result = NO;
    
    switch (status)
    {
        case GL_FRAMEBUFFER_UNSUPPORTED:
            errorMessage = @"framebuffer不支持该格式";
            result = NO;
            break;
        case GL_FRAMEBUFFER_COMPLETE:
            NSLog(@"framebuffer 创建成功");
            result = YES;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            errorMessage = @"Framebuffer不完整 缺失组件";
            result = NO;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
            errorMessage = @"Framebuffer 不完整, 附加图片必须要指定大小";
            result = NO;
            break;
        default:
            // 一般是超出GL纹理的最大限制
            errorMessage = @"未知错误 error !!!!";
            result = NO;
            break;
    }
    
    NSLog(@"%@",errorMessage ? errorMessage : @"");
    *error = errorMessage ? [NSError errorWithDomain:@"com.colin.error"
                                                code:status
                                            userInfo:@{@"ErrorMessage" : errorMessage}] : nil;
    
    return result;
}

- (void)render:(ZTextureType)type w:(int)width h:(int)height data:(unsigned char *[])data{
 dispatch_async(dispatch_get_main_queue(), ^{
    _shader.GetTexture(0,width,height,data[0]);  // Y
    
    if(type == ZTEXTURE_YUV420P)
    {
        // ZLOGI("===== ZTEXTURE_YUV420P ======");
        _shader.GetTexture(1,width/2,height/2,data[1]);  // U
        _shader.GetTexture(2,width/2,height/2,data[2]);  // V
    }
    else
    {
        //ZLOGI("===== ZTEXTURE_nv12 ======");
        _shader.GetTexture(1,width/2,height/2,data[1], true);  // UV
    }
    _shader.Draw();

    glClearColor(0, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 做完所有绘制操作后，最终呈现到屏幕上
    [_context presentRenderbuffer:GL_RENDERBUFFER];
         });
}

- (void)setShader:(ZShaderType)tpye
{
        dispatch_async(dispatch_get_main_queue(), ^{
            _shader.Close();
            _shader.Init(tpye);
    
    
        });
 
    
}

//- (void)setup{
//    [self setupLayer];
//    [self setupContext];
//    [self setupRenderBuffer];
//    [self setupFrameBuffer];
//    NSError *error;
//    NSAssert1([self checkFramebuffer:&error], @"%@",error.userInfo[@"ErrorMessage"]);
//}


//-(void)dealloc
//{
//   
//    [super dealloc];
//     _shader.Close();
//}
@end
