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
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>
#include "ZShader.h"

void RrenderWith(void * caller ,ZTextureType type, int width, int height, unsigned char *data[]){
    [(__bridge id)caller render:type w:width h:height data:data];
}

void InitShaderWith(void * caller ,ZShaderType type){
    [(__bridge id)caller setShader:type];
}

@interface ZPlayerView()
{
    //设置基础的几个成员变量
//    CAEAGLLayer *_eaglLayer;
//    EAGLContext *_context;
    GLuint       _framebuffer;
    GLuint       _renderbuffer;
//    CGSize       _bufferSize;
  //  ZShader      _shader;
    GLuint _texture_YUV[3];    //纹理数组。 分别用来存放Y,U,V
    
}
@property (strong,nonatomic) EAGLContext * context;       //上下文
@property (strong,nonatomic) CAEAGLLayer * drawLayer;     //画布
@property (assign,nonatomic) GLuint        program_handle;//程序集句柄。
@property (assign,nonatomic) GLuint        width;
@property (assign,nonatomic) GLuint        height;
@property (nonatomic) FILE     *file;

@end

@implementation ZPlayerView


- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        NSLog(@"%@",[NSThread currentThread]);
//        _rCall = RrenderWith;
   
//        [self textureSetup];
//        [self setupRenderBuffer];
//        [self shader_programSetup];
//        NSError *error;
//        NSAssert1([self checkFramebuffer:&error], @"%@",error.userInfo[@"ErrorMessage"]);
    }
    return self;
}

- (void)setup {
    [self setupLayer];
    [self setupContext];
    [self textureSetup];//纹理
    [self shader_programSetup];//着色器
    
}


+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)setupLayer {
    // 用于显示的layer
    self.drawLayer = (CAEAGLLayer *)self.layer;
    
    // CALayer 默认是透明的（opaque = NO），而透明的层对性能负荷很大。所以将其关闭。
    self.drawLayer.opaque = YES;
    
    //保持跟手机主屏幕一致，在不同手机上自适应
    self.drawLayer.contentsScale = [UIScreen mainScreen].scale;
    //以及颜色存储格式。
    self.drawLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                         [NSNumber numberWithBool:NO],
                                     kEAGLDrawablePropertyRetainedBacking,
                                         kEAGLColorFormatRGBA8,
                                     kEAGLDrawablePropertyColorFormat,
                                         nil];
}

- (void)setupContext {
    if (!_context) {
        NSLog(@"%@",[NSThread currentThread]);
//        NSLog(@"%@",NSThread.currentThread);
        // 创建GL环境上下文
        // EAGLContext 管理所有通过 OpenGL ES 进行渲染的信息.
        self.context  = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        //setCurrentContext
        BOOL re;
        re = [EAGLContext setCurrentContext:self.context ];
        if (!re) {
            NSLog(@"setCurrentContext failed!");
        }
    }else{
        NSLog(@"creat EAGLContext failed!");
    }
//    NSAssert(_context && [EAGLContext setCurrentContext:_context], @"初始化GL环境失败");
   
}

- (void)setupRenderBuffer {
    
    NSLog(@"%@",[NSThread currentThread]);
    if (_framebuffer) {
        glDeleteFramebuffers(1, &_framebuffer);
        _framebuffer = 0;
    }
    // 生成 framebuffer ( framebuffer = 画布 ) 帧缓冲区
    glGenFramebuffers(1, &_framebuffer);
    
    // 生成 renderbuffer ( renderbuffer = 用于展示的窗口 )绘制缓冲区
    glGenRenderbuffers(1, &_renderbuffer);
    
    // 绑定 fraembuffer  绑定帧缓冲区到渲染管线
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    // 绑定 renderbuffer 绑定绘制缓冲区到渲染管线
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
    // 为绘制缓冲区分配存储区 此处把CAEAGLLayer的绘制存储区作为绘制缓冲区的存储区
    [self.context  renderbufferStorage:GL_RENDERBUFFER fromDrawable:self.drawLayer];
    
    //获取绘制缓冲区的像素宽高
    GLint width,height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    
    //决定视见区域
    glViewport(0, 0, width, height);
    
    //将绘制缓冲区绑定到帧缓冲区
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
    
    
        __weak __typeof(self)weakSelf = self;
        dispatch_async(dispatch_get_main_queue(), ^{
             __strong __typeof(weakSelf)strongSelf = weakSelf;
            //设置着色器属性
            [strongSelf setVertexAttributeWidth:width height:height];
            
            /*定义2d图层
             之所以要定义，是因为
             glTexSubImage2D:定义一个存在的一维纹理图像的一部分,但不能定义新的纹理
             glTexImage2D:   定义一个二维的纹理图
             所以每次宽高变化的时候需要调用glTexImage2D重新定义一次
             */
            [strongSelf image2DdefineWidth:width height:height];
            
            
            //设置width，height
            [strongSelf setWidth:width height:height];
            
            //YUV420p 4个Y对应一套UV，平面结构。 YUV的分布：YYYYUV。Y在一开始。长度等于点数，即宽高的积。
            //            glBindTexture(GL_TEXTURE_2D, self->_texture_YUV[index]);
            //            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE,buf[0]);
            
            //            YUV420p 4个Y对应一套UV，平面结构。 YUV的分布：YYYYUV。Y在一开始。长度等于点数，即宽高的积。
            glBindTexture(GL_TEXTURE_2D, strongSelf->_texture_YUV[0]);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, data[0]);
            //U在Y之后，长度等于点数的1/4，即宽高的积的1/4。
            glBindTexture(GL_TEXTURE_2D, strongSelf->_texture_YUV[1]);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, data[1]);
            //V在U之后，长度等于点数的1/4，即宽高的积的1/4。
            glBindTexture(GL_TEXTURE_2D, strongSelf->_texture_YUV[2]);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, data[2]);
            //❀
            
            [strongSelf render];
            
//              NSLog(@"%@",NSThread.currentThread);
//              __strong __typeof(weakSelf)strongSelf = weakSelf;
//
//
//                strongSelf->_shader.GetTexture(0,width,height,data[0]);  // Y
//
//                if(type == ZTEXTURE_YUV420P || type == ZTEXTURE_YUVJ420P)
//                {
//                    //  ZLOGI("===== ZTEXTURE_YUV420P ======");
//                    //  NSLog(@"===== ZTEXTURE_YUV420P ======");
//                    strongSelf->_shader.GetTexture(1,width/2,height/2,data[1]);  // U
//                    strongSelf->_shader.GetTexture(2,width/2,height/2,data[2]);  // V
//                }
//                else
//                {
//                    //ZLOGI("===== ZTEXTURE_nv12 ======");
//                    strongSelf->_shader.GetTexture(1,width/2,height/2,data[1], true);  // UV
//                }
////                glClear(GL_COLOR_BUFFER_BIT);
////                glClearColor(0, 1, 1, 1);
//
//                strongSelf->_shader.Draw();
//
//                // 做完所有绘制操作后，最终呈现到屏幕上
//                [strongSelf->_context presentRenderbuffer:GL_RENDERBUFFER];

 
        });
}

- (void)setShader:(ZShaderType)tpye
{
//     __weak __typeof(self)weakSelf = self;
//        dispatch_async(dispatch_get_main_queue(), ^{
//             __strong __typeof(weakSelf)strongSelf = weakSelf;
////            NSLog(@"%@",NSThread.currentThread);
//            strongSelf->_shader.Init(tpye);
//
//        });
 
    
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

- (void)textureSetup { // 对应 XShader 中的 getTexture
    
    //YUV三个位置。
    int Y = 0,U = 1,V = 2;
    
    if (_texture_YUV)//如果存在先删除。
    {
        glDeleteTextures(3, _texture_YUV);
    }
    
    //创建纹理
    glGenTextures(3, _texture_YUV);
    
    if (!_texture_YUV[Y] || !_texture_YUV[U] || !_texture_YUV[V])
    {
        NSLog(@"glGenTextures faild.");
        return;
    }
    
    
    
    //分别对Y,U,V进行设置。
    
    //Y
    //glActiveTexture:选择可以由纹理函数进行修改的当前纹理单位
    //并绑定
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture_YUV[Y]);
    //纹理过滤
    //GL_LINEAR 线性取平均值纹素，GL_NEAREST 取最近点的纹素
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//放大过滤。
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//缩小过滤
    //纹理包装
    //包装模式有：GL_REPEAT重复，GL_CLAMP_TO_EDGE采样纹理边缘，GL_MIRRORED_REPEAT镜像重复纹理。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//纹理超过S轴
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//纹理超过T轴
    
    
    //U
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _texture_YUV[U]);
    //纹理过滤
    //GL_LINEAR 线性取平均值纹素，GL_NEAREST 取最近点的纹素
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//放大过滤。
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//缩小过滤
    //纹理包装
    //包装模式有：GL_REPEAT重复，GL_CLAMP_TO_EDGE采样纹理边缘，GL_MIRRORED_REPEAT镜像重复纹理。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//纹理超过S轴
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//纹理超过T轴
    
    
    //V
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _texture_YUV[V]);
    //纹理过滤
    //GL_LINEAR 线性取平均值纹素，GL_NEAREST 取最近点的纹素
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//放大过滤。
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//缩小过滤
    //纹理包装
    //包装模式有：GL_REPEAT重复，GL_CLAMP_TO_EDGE采样纹理边缘，GL_MIRRORED_REPEAT镜像重复纹理。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//纹理超过S轴
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//纹理超过T轴
}


/*
 1.创建着色器
 2.指定着色器源代码字符串
 3.编译着色器
 
 
 4.创建着色器可执行程序
 5.向可执行程序中添加着色器
 6.链接可执行程序
 */
- (void)shader_programSetup {
    
    //着色器文件位置
    NSString * vertex_shader_path = [[NSBundle mainBundle] pathForResource:@"file" ofType:@"vsh"];
    NSString * fragment_shader_path = [[NSBundle mainBundle] pathForResource:@"file" ofType:@"fsh"];
    
    //读取编译shader
    //顶点shader
    GLuint vertexShader = [self compileShader:vertex_shader_path withType:GL_VERTEX_SHADER];
    //片元shader
    GLuint fragmentShader = [self compileShader:fragment_shader_path withType:GL_FRAGMENT_SHADER];
    
    //创建程序
    self.program_handle = glCreateProgram();
    
    //向program中添加顶点着色器
    glAttachShader(self.program_handle, vertexShader);
    //向program中添加片元着色器
    glAttachShader(self.program_handle, fragmentShader);
    //绑定position属性到顶点着色器的0位置，绑定TexCoordIn到顶点着色器的1位置
    glBindAttribLocation(self.program_handle, 0, "position");
    glBindAttribLocation(self.program_handle, 1, "TexCoordIn");
    //链接程序
    glLinkProgram(self.program_handle);
    
    //查看状态
    GLint linkSuccess;
    glGetProgramiv(self.program_handle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(self.program_handle, sizeof(messages), 0, &messages[0]);
        NSString *messageString = [NSString stringWithUTF8String:messages];
        NSLog(@"%@", messageString);
        exit(1);
    }
    
    //删除
    if (vertexShader)   glDeleteShader(vertexShader);
    if (fragmentShader) glDeleteShader(fragmentShader);
    
    
    //使用当前程序。
    glUseProgram(self.program_handle);
    
    //从片元着色器中获取到Y,U,V变量。
    GLuint textureUniformY = glGetUniformLocation(self.program_handle, "SamplerY");
    GLuint textureUniformU = glGetUniformLocation(self.program_handle, "SamplerU");
    GLuint textureUniformV = glGetUniformLocation(self.program_handle, "SamplerV");
    //分别设置为0，1，2.
    glUniform1i(textureUniformY, 0);
    glUniform1i(textureUniformU, 1);
    glUniform1i(textureUniformV, 2);
    
}
//编译shader
- (GLuint)compileShader:(NSString*)path withType:(GLenum)shaderType
{
    //创建shader句柄
    GLuint shaderHandle = glCreateShader(shaderType);
    
    //读取文件内容
    NSString* content = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil];
    const GLchar* source = (GLchar *)[content UTF8String];
    
    //将文件内容设置给shader
    glShaderSource(shaderHandle, 1,&source,NULL);
    //编译shader
    glCompileShader(shaderHandle);
    GLint compileSuccess;
    //获取状态
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        NSString *messageString = [NSString stringWithUTF8String:messages];
        NSLog(@"%@", messageString);
        exit(1);
    }
    
    return shaderHandle;
}


- (void)setWidth:(GLuint)width height:(GLuint)height {
    if (self.width == width && self.height == height) {
        return;
    }
    //取宽高。
    self.width = width;
    self.height = height;
}

- (void)image2DdefineWidth:(GLuint)width  height:(GLuint)height{
    if (self.width == width&&self.height == height) {
        return;
    }
    //根据宽高生成空的YUV数据
    void *blackData = malloc(width * height * 1.5);
    //全部填0，实际出来的是一张绿色的图- -；但是没有去渲染就直接替换了，所以不会造成影响。只起定义作用。
    if(blackData) memset(blackData, 0x0, width * height * 1.5);
    
    
    /*
     
     GL_APIENTRY glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
     
     //target参数用于定义二维纹理；
     //如果提供了多种分辨率的纹理图像，可以使用level参数，否则level设置为0；
     //internalformat确定了哪些成分(RGBA, 深度, 亮度和强度)被选定为图像纹理单元
     //width和height表示纹理图像的宽度和高度；
     //border参数表示边框的宽度
     //format和type参数描述了纹理图像数据的格式和数据类型
     //pixels参数包含了纹理图像的数据，这个数据描述了纹理图像本身和它的边框
     
     */
    
    //Y
    glBindTexture(GL_TEXTURE_2D, _texture_YUV[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
    //U
    glBindTexture(GL_TEXTURE_2D, _texture_YUV[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width/2, height/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
    //V
    glBindTexture(GL_TEXTURE_2D, _texture_YUV[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width/2, height/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
    
    free(blackData);
    
}

- (void)setVertexAttributeWidth:(GLuint)width height:(GLuint)height {
    
    if (self.width == width && self.height == height) {
        return;
    }
    
    
    
    CGSize size = self.bounds.size;
    //视口变换函数
    /*
     glViewPort(x:GLInt;y:GLInt;Width:GLSizei;Height:GLSizei);
     其中，参数X，Y指定了视见区域的左下角在窗口中的位置
     Width和Height指定了视见区域的宽度和高度。注意OpenGL使用的窗口坐标和WindowsGDI使用的窗口坐标是不一样的
     */
    glViewport(1, 1, size.width, size.height);
    
    
    //以屏幕中心为原点。
    /*
     这里的排布 按三个点确定一个面原则。
     由GL_TRIANGLE_STRIP定义：
     
     先取前三个坐标组成一个三角形。
     再取除了去掉第一个坐标，剩下的组成一个三角形。
     
     这样组成一个矩形最少需要4个坐标，并且排序规则为相邻的三个点第一个点为第四个点的对角。
     */
    
    //这个是用于传给顶点着色器的坐标。
    static const GLfloat squareVertices[] = {
        -1.0f,-1.0f,  //左下角。
        1.0f ,-1.0f,  //右下角。
        -1.0f,1.0f,   //左上角
        1.0f ,1.0f,   //右上角
    };
    
    //这个是用于传给片元着色器的坐标，由顶点着色器代传。
    //由于图像的存放一般是以左上角为原点，从上到下，但是OpenGL的处理是从左下角由下到上，所以图像的上下是颠倒的。
    //所以需要把其中一个坐标的上下改为相反的。左右不用换，不然左右又不对了。
    static const GLfloat coordVertices[] = {
        0.0f, 1.0f,   //左上角
        1.0f, 1.0f,   //右上角
        0.0f, 0.0f,   //左下角
        1.0f, 0.0f,   //右下角
    };
    //    static const GLfloat coordVertices[] = {
    //
    //        1.0f, 1.0f,   //右上角
    //        0.0f, 1.0f,   //左上角
    //        1.0f, 0.0f,   //右下角
    //        0.0f, 0.0f,   //左下角
    //    };
    
    
    /*激活顶点着色器属性*/
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    //设置顶点着色器的属性。如果视图不变化，就不用变。
    /*
     GL_APIENTRY glVertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr)
     
     indx       着色器代码对应变量ID
     size       此类型数据的个数
     type       数据类型
     normalized 是否对非float类型数据转化到float时候进行归一化处理
     stride     此类型数据在数组中的重复间隔宽度，byte类型计数，0为紧密排布。
     ptr        数据指针， 这个值受到VBO的影响
     */
    glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, squareVertices);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, coordVertices);
    
}

- (void)layoutSubviews {
    [self destoryFrameAndRenderBuffer];
    [self bufferCreate];
}

- (void)render {
    
    /*
     glDrawArrays 根据顶点数组中的坐标数据和指定的模式，进行绘制。
     glDrawArrays (GLenum mode, GLint first, GLsizei count);
     mode，绘制方式
     GL_TRIANGLES:
     第一次取1，2，3，第二次取4，5，6，以此类推，不足三个就停止。
     
     GL_TRIANGLE_STRIP:
     从第一个开始取前三个1，2，3，第二次从第二开始取2，3，4，以此类推到不足3个停止。
     
     GL_TRIANGLE_FAN:
     从第一个开始取，1，2，3，第二次的第二个坐标从3开始，1，3，4，以此类推，到不足三个停止。
     
     first，从数组缓存中的哪一位开始绘制，一般为0。
     count，数组中顶点的数量。
     */
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    [self.context presentRenderbuffer:GL_RENDERBUFFER];
}


- (BOOL)bufferCreate {
    
    //生成framebuffer和renderbuffer
    glGenFramebuffers(1, &_framebuffer);
    glGenRenderbuffers(1, &_renderbuffer);
    
    //绑定到OpenGL
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
    
    if (![self.context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.drawLayer])
    {
        NSLog(@"attach渲染缓冲区失败");
    }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderbuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"创建缓冲区错误 0x%x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return NO;
    }
    return YES;
}


- (void)destoryFrameAndRenderBuffer
{
    if (_framebuffer)
    {
        glDeleteFramebuffers(1, &_framebuffer);
    }
    
    if (_renderbuffer)
    {
        glDeleteRenderbuffers(1, &_renderbuffer);
    }
    
    _framebuffer = 0;
    _renderbuffer = 0;
}
@end
