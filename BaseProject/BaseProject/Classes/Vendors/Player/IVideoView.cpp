//
//  IVideoView.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "IVideoView.hpp"

void IVideoView::Update(ZBData data)
{
    this->Render(data);
}
