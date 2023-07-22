#include "RenderChain.h"

using namespace glkit;

RenderChain::RenderChain(int index,
                         std::vector<GLPipline> piplines,
                         int textureId)
    : mIndex(index),
      mPiplines(piplines),
      mTextureId(textureId)
{
}

void RenderChain::proceed()
{

    /**
     * @brief 取出当前索引的渲染节点并执行渲染
     * 
     */
    GLPipline &pipline = mPiplines[mIndex];
    GLuint textureId = pipline.draw(*this);

    /**
     * @brief 停止处理
     *
     */
    if (mIndex == mPiplines.size() - 1)
    {
        return;
    }
    /**
     * @brief 处理下一个渲染节点
     * 
     */
    RenderChain newChain(mIndex + 1, mPiplines, textureId);
    newChain.proceed();
}

RenderChain::~RenderChain()
{
}
