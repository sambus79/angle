//
// Copyright (c) 2002-2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// IndexDataManager.h: Defines the IndexDataManager, a class that
// runs the Buffer translation process for index buffers.

#ifndef LIBANGLE_INDEXDATAMANAGER_H_
#define LIBANGLE_INDEXDATAMANAGER_H_

#include <GLES2/gl2.h>

#include "common/angleutils.h"
#include "common/mathutil.h"
#include "libANGLE/Error.h"
#include "libANGLE/renderer/d3d/RendererD3D.h"

namespace
{
enum
{
    INITIAL_INDEX_BUFFER_SIZE = 4096 * sizeof(GLuint)
};
}

namespace gl
{
class Buffer;
}

namespace rx
{
class IndexBufferInterface;
class StaticIndexBufferInterface;
class StreamingIndexBufferInterface;
class IndexBuffer;
class BufferD3D;
class RendererD3D;

struct SourceIndexData
{
    BufferD3D *srcBuffer;
    const void *srcIndices;
    unsigned int srcCount;
    GLenum srcIndexType;
    bool srcIndicesChanged;
};

struct TranslatedIndexData
{
    gl::IndexRange indexRange;
    unsigned int startIndex;
    unsigned int startOffset;  // In bytes

    IndexBuffer *indexBuffer;
    BufferD3D *storage;
    GLenum indexType;
    unsigned int serial;

    SourceIndexData srcIndexData;
};

class IndexDataManager : angle::NonCopyable
{
  public:
    explicit IndexDataManager(BufferFactoryD3D *factory, RendererClass rendererClass);
    virtual ~IndexDataManager();

    bool usePrimitiveRestartWorkaround(bool primitiveRestartFixedIndexEnabled, GLenum type);
    bool isStreamingIndexData(const gl::Context *context, GLenum srcType);
    gl::Error prepareIndexData(GLenum srcType,
                               GLsizei count,
                               gl::Buffer *glBuffer,
                               const void *indices,
                               TranslatedIndexData *translated,
                               bool primitiveRestartFixedIndexEnabled);

  private:
    gl::Error streamIndexData(const void *data,
                              unsigned int count,
                              GLenum srcType,
                              GLenum dstType,
                              bool usePrimitiveRestartFixedIndex,
                              TranslatedIndexData *translated);
    gl::Error getStreamingIndexBuffer(GLenum destinationIndexType,
                                      IndexBufferInterface **outBuffer);

    BufferFactoryD3D *const mFactory;
    RendererClass mRendererClass;
    StreamingIndexBufferInterface *mStreamingBufferShort;
    StreamingIndexBufferInterface *mStreamingBufferInt;
};
}

#endif  // LIBANGLE_INDEXDATAMANAGER_H_
