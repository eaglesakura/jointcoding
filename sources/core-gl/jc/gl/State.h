/*
 * JCGLState.h
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#ifndef JCGLSTATE_H_
#define JCGLSTATE_H_

#include    "jc/mem/SmartPtr.h"
#include    "jc/gl/GL.h"
#include    "jc/math/Rect.h"
#include    "jc/graphics/Color.h"

namespace jc {
namespace gl {

#define     CLEAR_GL_ERROR       { glGetError(); }
#define     PRINT_GL_ERROR     { GLState::printGLHasError(__FILE__, __LINE__); }

/**
 * 無効な属性インデックスを示す定数
 */
#define ATTRIBUTE_DISABLE_INDEX -1

/**
 * 無効な属性インデックスを示す
 */
#define UNIFORM_DISABLE_INDEX -1

// #define  STATE_NO_CHECK

/**
 * GLのステート情報を保持する。
 * ステートが一致する場合、無駄な呼び出しを極力抑えるように動作する。
 */
class GLState: public Object {
    enum {
        /**
         * テクスチャユニットの最大数
         */
        MAX_TEXTURE_UNIT = 16,

        /**
         * GL_ARRAY_BUFFERの値を取り出したい
         */
        INDEX_GL_ARRAY_BUFFER = 0,

        /**
         * GL_ELEMENT_ARRAY_BUFFERの値を取り出したい
         */
        INDEX_GL_ELEMENT_ARRAY_BUFFER = 1,

        /**
         * バインド可能なバッファ数
         * GL_ARRAY_BUFFER / GL_ELEMENT_ARRAY_BUFFER
         */
        MAX_BINDBUFFER = 2,

        /**
         * 頂点情報の最大値
         * 仕様上の最大であり、実機ではGPUの設定が優先される。
         */
        MAX_VERTEX_ATTRIBUTE = 16,
    };

    /**
     * 画面クリア関連の情報
     */
    struct {
        Color clearColor;
    } clearContext;

    struct {
        /**
         * 利用中のシェーダープログラム
         */
        GLuint usingProgram;
    } shaderProgramContext;

    /**
     * glBindTexture()されているテクスチャ情報
     */
    struct {
        /**
         * GLESの定数として、index0〜31までは確保されている。
         * ただし、それがハードウェアとして対応しているかは問わない
         * GL_TEXTURE0〜GL_TEXTURE31に対応
         */
        GLuint textures[MAX_TEXTURE_UNIT];

        /**
         * アクティブ化されているテクスチャユニットを保持する
         */
        s32 active;
    } textureContext;

    /**
     * カレントに設定されているVBOのステート
     */
    struct {
        /**
         * 最後にglBindBufferされたオブジェクト
         */
        GLuint buffers[MAX_BINDBUFFER];
    } bindBufferContext;

    /**
     * glVertexAttributePointerに指定するデータ
     */
    struct VertexAttributePointerData {
        /**
         * 頂点サイズ
         */
        GLint size;

        /**
         * 頂点タイプ
         * GL_FLOAT | GL_FIXED | GL_SHORT
         */
        GLenum type;

        /**
         * 正規化フラグ
         */
        GLboolean normalized;

        /**
         * 頂点間の隙間バイト数
         */
        GLsizei stride;

        /**
         * 実際のポインタ
         */
        GLvoid* ptr;

        /**
         * 先頭ポインタからのオフセット
         */
        u32 offset;
    };

    struct {
        /**
         * 各Attrインデックスに格納されている頂点情報
         * このステートはContextに対してグローバルなため、Shaderを変更しても変更されない。
         */
        struct {
            /**
             * 有効になっている場合true
             */
            jcboolean enable;

            /**
             * 指定されたポインタのデータ
             */
            VertexAttributePointerData pointerData;
        } buffers[MAX_VERTEX_ATTRIBUTE];
    } vertexAttrContext;

    /**
     * glEnable()関連のステート
     */
    struct {
        /**
         * テクスチャ
         * GL_TEXTURE_2D
         */
        jcboolean texture2d;

    } enableContext;

    struct {
        /**
         * DepthFunc
         */
        GLint func;

        /**
         * test
         */
        jcboolean enable;
    } depthContext;

    struct {
        /**
         * ブレンド有効・無効設定
         */
        jcboolean enable;

        /**
         * source
         */
        GLint src;

        /**
         * dst
         */
        GLint dst;
    } blendContext;

    struct {
        /**
         * シザーテスト
         * GL_SCISSOR_TEST
         */
        jcboolean enable;

        /**
         * シザーボックスのXYWH
         */
        RectI box;
    } scissorContext;

    /**
     * ViewPort情報
     */
    RectI viewportContext;

public:
    GLState();
    virtual ~GLState();

    /**
     * 現在のコンテキストと同期を取る。
     * この呼出は非常に重くなると予想されるので、極力呼び出さないようにする
     */
    virtual void syncContext();

    /**
     * glClear()呼び出し
     */
    inline void clear(const u32 flags = (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)) {
        glClear(flags);
    }

    /**
     * glColor
     * ステートを変更した場合trueを返す
     */
    inline jcboolean clearColorf(const GLclampf r, const GLclampf g, const GLclampf b, const GLclampf a) {
        Color temp = Color::fromRGBAf(r, g, b, a);
#ifndef STATE_NO_CHECK
        if (temp != clearContext.clearColor) {
#endif
            // 色が違うからコマンド呼び出し
            glClearColor(r, g, b, a);

            // 最後に呼び出した色を保存
            clearContext.clearColor = temp;

            return jctrue;
#ifndef STATE_NO_CHECK
        }
        return jcfalse;
#endif
    }

    /**
     * デプステストの有効・無効を設定する
     */
    inline jcboolean depthTestEnable(const jcboolean enable) {
        if (depthContext.enable != enable) {
            depthContext.enable = enable;
            if (enable) {
                glEnable(GL_DEPTH_TEST);
            } else {
                glDisable(GL_DEPTH_TEST);
            }
            return jctrue;
        }
        return jcfalse;
    }

    /**
     * デプステストの関数を設定する
     */
    inline jcboolean depthFunc(const GLenum func) {
        if (depthContext.func != func) {
            depthContext.func = func;
            glDepthFunc(func);
            return jctrue;
        }
        return jcfalse;
    }

    /**
     * ブレンド状態を更新する
     */
    inline jcboolean blendEnable(const jcboolean enable) {
        if (blendContext.enable != enable) {
            blendContext.enable = enable;
            glEnable(GL_BLEND);
            return jctrue;
        }
        return jcfalse;
    }

    /**
     * ブレンディング設定を行う
     */
    inline jcboolean blendFunc(const GLint sfactor, const GLint dfactor) {
        if (blendContext.src != sfactor || blendContext.dst != dfactor) {
            blendContext.src = sfactor;
            blendContext.dst = dfactor;
            glBlendFunc(sfactor, dfactor);
            return jctrue;
        }
        return jcfalse;
    }

    /**
     * glViewport
     * ステートを変更した場合trueを返す
     */
    inline jcboolean viewport(const GLint x, const GLint y, const GLint width, const GLint height) {
        const s32 left = x;
        const s32 top = y;
        const s32 right = x + width;
        const s32 bottom = y + height;
#ifndef STATE_NO_CHECK
        if (!viewportContext.equalsLTRB(left, top, right, bottom)) {
#endif
            glViewport(x, y, width, height);
            viewportContext.setXYWH(x, y, width, height);
            return jctrue;
#ifndef STATE_NO_CHECK
        }
        return jcfalse;
#endif
    }

    /**
     * glBindBuffer
     * ステートを変更した場合trueを返す
     * @param target GL_ARRAY_BUFFER | GL_ELEMENT_ARRAY_BUFFER
     */
    inline jcboolean bindBuffer(GLenum target, GLuint buffer) {
        const s32 index = target - GL_ARRAY_BUFFER;
#ifndef STATE_NO_CHECK
        if (bindBufferContext.buffers[index] != buffer) {
#endif
            // バッファが一致しないから呼び出す
            glBindBuffer(target, buffer);
            // バッファを保存する
            bindBufferContext.buffers[index] = buffer;
            return jctrue;
#ifndef STATE_NO_CHECK
        }
        return jcfalse;
#endif
    }

    /**
     * 指定したバッファがバインドされていたらtrue
     */
    inline jcboolean isBindedBuffer(GLenum target, GLuint buffer) {
        const s32 index = target - GL_ARRAY_BUFFER;
        return bindBufferContext.buffers[index] == buffer;
    }

    /**
     * テクスチャユニットの定数をテクスチャ番号に変換する
     */
    inline static s32 toTextureIndex(const GLenum textureUnit) {
        return textureUnit - GL_TEXTURE0;
    }

    /**
     * テクスチャ番号をテクスチャユニット定数に変換する
     */
    inline static s32 toTextureUnit(const s32 index) {
        return index + GL_TEXTURE0;
    }

    /**
     * テクスチャユニットをActiveにする
     */
    inline jcboolean activeTexture(const s32 index) {
        const s32 unit = toTextureUnit(index);

        // 違うユニットがアクティブ化されていたら、アクティブにし直す
#ifndef STATE_NO_CHECK
        if (unit != (s32) textureContext.active) {
#endif
            textureContext.active = unit;
            glActiveTexture(unit);

            return jctrue;
#ifndef STATE_NO_CHECK
        }
        return jcfalse;
#endif
    }


    /**
     * 現在アクティブになっているテクスチャの番号を取得する。
     */
    inline s32 getActiveTextureIndex() {
        return toTextureIndex(textureContext.active);
    }

    /**
     * 引数のテクスチャがどれかにバインドされている場合、バインドを解除する。
     */
    inline void unbindTexture(const GLuint texture) {
        for (int i = 0; i < MAX_TEXTURE_UNIT; ++i) {
            if (textureContext.textures[i] == texture) {
                // テクスチャが一致したからunbind
                activeTexture(i);
                bindTexture(GL_TEXTURE_2D, 0);
            }
        }
    }

    /**
     * 空いているテクスチャユニット番号を取得する
     */
    inline s32 getFreeTextureUnitIndex() {
        for (int i = 0; i < MAX_TEXTURE_UNIT; ++i) {
            if (textureContext.textures[i] == 0) {
                return i;
            }
        }

        // どのテクスチャユニットも空いていない
        jclog("free texture unit not found...");
        {
            for (int i = 0; i < MAX_TEXTURE_UNIT; ++i) {
                jclogf("texture[%d] = %x", i, textureContext.textures[i]);
            }
        }
        return -1;
    }

    /**
     * TEXTURE_2Dにバインドする。
     */
    inline jcboolean bindTexture(const GLuint texture) {
        return bindTexture(GL_TEXTURE_2D, texture);
    }

    /**
     * 現在activeになっているテクスチャユニットに対してバインドを行う。
     */
    inline jcboolean bindTexture(const GLenum target, const GLuint texture) {
        const s32 index = getActiveTextureIndex();
        const GLuint currentTex = textureContext.textures[index];

        // 違うテクスチャがユニットに設定されていたら、バインドし直す
        if (currentTex != texture) {
            textureContext.textures[index] = texture;
            glBindTexture(GL_TEXTURE_2D, texture);

            return jctrue;
        }

        return jcfalse;
    }

    /**
     * シェーダプログラムを有効化する
     * ステートを変更した場合trueを返す
     */
    inline jcboolean useProgram(const GLuint shaderProgram) {
        if (shaderProgramContext.usingProgram != shaderProgram) {
            glUseProgram(shaderProgram);
            shaderProgramContext.usingProgram = shaderProgram;

            return jctrue;
        }
        return jcfalse;
    }

    /**
     * コンテキストに設定されているシェーダーと引数が同じであれば、trueを返す
     */
    inline jcboolean isUseProgram(const GLuint shaderProgram) const {
        return shaderProgramContext.usingProgram == shaderProgram;
    }

    /**
     * 頂点属性のポインタを設定する。
     * ステートを変更した場合trueを返す
     */
    inline jcboolean vertexAttribPointer(const GLuint index, const GLint size, const GLenum type, const GLboolean normalized, const GLsizei stride, const GLvoid* ptr, const u32 offset) {
        VertexAttributePointerData *attr = &vertexAttrContext.buffers[index].pointerData;

        // この命令でバインドされるポインタ値
        void* checkPtr = (ptr ? (void*) ptr : (void*) bindBufferContext.buffers[INDEX_GL_ARRAY_BUFFER]);

        if (attr->size != size || attr->type != type || attr->normalized != normalized || attr->stride != stride || attr->ptr != checkPtr || attr->offset != offset) {
            glVertexAttribPointer(index, size, type, normalized, stride, (const void*) (((u8*) ptr) + offset));

            // 属性情報を書き換える
            attr->size = size;
            attr->type = type;
            attr->normalized = normalized;
            attr->stride = stride;
            attr->ptr = checkPtr;
            attr->offset = offset;
            return jctrue;
        }
        return jcfalse;
    }

    /**
     * 頂点属性を有効化する。
     * ステートを変更した場合trueを返す
     */
    inline jcboolean enableVertexAttribArray(const GLuint index) {
        // 無効化されているため、有効化する
#ifndef STATE_NO_CHECK
        if (!vertexAttrContext.buffers[index].enable) {
#endif
            vertexAttrContext.buffers[index].enable = jctrue;
            glEnableVertexAttribArray(index);
            return jctrue;
#ifndef STATE_NO_CHECK
        }
        return jcfalse;
#endif
    }

    /**
     * 頂点属性を有効化する。
     * ステートを変更した場合trueを返す
     */
    inline jcboolean disableVertexAttribArray(const GLuint index) {
        // 有効化されているため、無効化する
#ifndef STATE_NO_CHECK
        if (vertexAttrContext.buffers[index].enable) {
#endif
            vertexAttrContext.buffers[index].enable = jcfalse;
            glDisableVertexAttribArray(index);
            return jcfalse;
#ifndef STATE_NO_CHECK
        }
        return jcfalse;
#endif
    }

    /**
     * シェーダーに関するエラーを出力する。
     * エラーが発生していたらtrueを返す。
     */
    static jcboolean printShaderError(const GLuint shaderObject, const GLint status);

    /**
     * シェーダープログラムに関するエラーを出力する
     * エラーが発生していたらtrueを返す
     */
    static jcboolean printProgramError(const GLuint programObject, const GLint status);

    /**
     * 現在のステートをログ出力する
     */
    void print(const charactor* file, const s32 line) const;

    /**
     * GLがエラーを持っている場合出力して、それ以外は何もしない。
     */
    static void printGLHasError(const charactor* file, const s32 line);

    /**
     * GLエラー出力を行う
     */
    static void printGLError(const charactor* file, const s32 line);

    /**
     * エラーを出力する
     * GL_NOERROR以外だったらjctrueを返す
     */
    static jcboolean printGLError(const charactor* file, const s32 line, GLenum error);

    /*
     * GLに保存されているエラーをクリアする
     */
    static void clearGLError();

}
;

/**
 * managed
 */
typedef jc_sp<GLState> MGLState;

}
}
#endif /* JCGLSTATE_H_ */
