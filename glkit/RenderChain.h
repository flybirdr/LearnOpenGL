#ifndef RENDERCHAIN_H
#define RENDERCHAIN_H

#include "GLPipline.h"
#include <vector>
#include <glad/glad.h>

namespace glkit
{

    class RenderChain
    {
    private:
        /**
         * @brief 当前索引
         *
         */
        int mIndex;
        std::vector<GLPipline> mPiplines;

        /**
         * @brief 缓存上一个节点的TextureID
         *
         */
        int mTextureId;

    public:
        RenderChain(int index, std::vector<GLPipline> piplines, int textureId);

        GLuint getTextureId() const;
        /**
         * @brief 使用下一个渲染管线进行渲染
         *
         * @return int
         */
        void proceed();

        ~RenderChain();
    };

};

#endif // RENDERCHAIN_H
