//
// Created by bigfish on 2018/10/16.
//

#include "GLVideoView.h"
#include "ZTexture.h"
#include "ZLog.h"
void  GLVideoView:: SetRender(void *win,void *randerCall,void *shaderCall)
{
    view = win;
    rCall = randerCall;
    sCall = shaderCall;
}


void  GLVideoView::Close()
{
    mux.lock();

    if(txt)
    {
        txt->Drop();
        txt = 0;
    }

    mux.unlock();
}

void  GLVideoView::Render(ZData data)
{
    if(!view){
        ZLOGE("view is nil! txt init failed! \n");
        return;
    }
    
    if(!txt)
    {
        //创建 texture
        txt = ZTexture::Create();
        txt->Init(view,rCall,sCall,(ZTextureType)data.format);
    }
    txt->Draw(data.datas,data.width,data.height);

}
