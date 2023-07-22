#ifndef GLPIPLINE_H
#define GLPIPLINE_H

#include "RenderChain.h"

namespace glkit
{
    /**
     * @brief 渲染节点
     *
     */
    class GLPipline
    {
    private:
        /* data */
    public:
        GLPipline();

        /**
         * @brief 处理本次渲染请求
         *
         * @param chain 链
         * @return int 返回一个GL Texture ID
         */

        virtual int draw(RenderChain chain) = 0;

        ~GLPipline();
    };

};

#endif // GLPIPLINE_H
