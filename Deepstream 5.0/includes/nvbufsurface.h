/*
 * Copyright (c) 2019, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

/**
 * @file nvbufsurface.h
 * <b>NvBufSurface Interface </b>
 *
 * This file specifies the NvBufSurface management API.
 *
 * The NvBufSurface API provides methods to allocate / deallocate, map / unmap
 * and copy batched buffers.
 */
 /**
 * @defgroup  ds_nvbuf_api Buffer Management API module
 *
 * This section describes types and functions of NvBufSurface application
 * programming interface.
 *
 */

#ifndef NVBUFSURFACE_H_
#define NVBUFSURFACE_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup ds_aaa NvBufSurface Types and Functions
 * Defines types and functions of \ref NvBufSurface application
 * programming interface.
 * @ingroup ds_nvbuf_api
 * @{ */

/** Defines the default padding length for reserved fields of structures. */
#define STRUCTURE_PADDING  4

/** Defines the maximum number of planes. */
#define NVBUF_MAX_PLANES   4

/**
 * Specifies mapping types for \ref NvBufSurface.
 */
typedef enum
{
  NVBUF_MAP_READ,       /**< Specifies \ref NvBufSurface mapping type "read." */
  NVBUF_MAP_WRITE,      /**< Specifies \ref NvBufSurface mapping type
                            "write." */
  NVBUF_MAP_READ_WRITE, /**< Specifies \ref NvBufSurface mapping type
                            "read/write." */
} NvBufSurfaceMemMapFlags;

/**
 * Specifies color formats for \ref NvBufSurface.
 */
typedef enum
{
  /** Specifies an invalid color format. */
  NVBUF_COLOR_FORMAT_INVALID,
  /** Specifies 8 bit GRAY scale - single plane */
  NVBUF_COLOR_FORMAT_GRAY8,
  /** Specifies BT.601 colorspace - YUV420 multi-planar. */
  NVBUF_COLOR_FORMAT_YUV420,
  /** Specifies BT.601 colorspace - YUV420 multi-planar. */
  NVBUF_COLOR_FORMAT_YVU420,
  /** Specifies BT.601 colorspace - YUV420 ER multi-planar. */
  NVBUF_COLOR_FORMAT_YUV420_ER,
  /** Specifies BT.601 colorspace - YVU420 ER multi-planar. */
  NVBUF_COLOR_FORMAT_YVU420_ER,
  /** Specifies BT.601 colorspace - Y/CbCr 4:2:0 multi-planar. */
  NVBUF_COLOR_FORMAT_NV12,
  /** Specifies BT.601 colorspace - Y/CbCr ER 4:2:0 multi-planar. */
  NVBUF_COLOR_FORMAT_NV12_ER,
  /** Specifies BT.601 colorspace - Y/CbCr 4:2:0 multi-planar. */
  NVBUF_COLOR_FORMAT_NV21,
  /** Specifies BT.601 colorspace - Y/CbCr ER 4:2:0 multi-planar. */
  NVBUF_COLOR_FORMAT_NV21_ER,
  /** Specifies BT.601 colorspace - YUV 4:2:2 planar. */
  NVBUF_COLOR_FORMAT_UYVY,
  /** Specifies BT.601 colorspace - YUV ER 4:2:2 planar. */
  NVBUF_COLOR_FORMAT_UYVY_ER,
  /** Specifies BT.601 colorspace - YUV 4:2:2 planar. */
  NVBUF_COLOR_FORMAT_VYUY,
  /** Specifies BT.601 colorspace - YUV ER 4:2:2 planar. */
  NVBUF_COLOR_FORMAT_VYUY_ER,
  /** Specifies BT.601 colorspace - YUV 4:2:2 planar. */
  NVBUF_COLOR_FORMAT_YUYV,
  /** Specifies BT.601 colorspace - YUV ER 4:2:2 planar. */
  NVBUF_COLOR_FORMAT_YUYV_ER,
  /** Specifies BT.601 colorspace - YUV 4:2:2 planar. */
  NVBUF_COLOR_FORMAT_YVYU,
  /** Specifies BT.601 colorspace - YUV ER 4:2:2 planar. */
  NVBUF_COLOR_FORMAT_YVYU_ER,
  /** Specifies BT.601 colorspace - YUV444 multi-planar. */
  NVBUF_COLOR_FORMAT_YUV444,
  /** Specifies RGBA-8-8-8-8 single plane. */
  NVBUF_COLOR_FORMAT_RGBA,
  /** Specifies BGRA-8-8-8-8 single plane. */
  NVBUF_COLOR_FORMAT_BGRA,
  /** Specifies ARGB-8-8-8-8 single plane. */
  NVBUF_COLOR_FORMAT_ARGB,
  /** Specifies ABGR-8-8-8-8 single plane. */
  NVBUF_COLOR_FORMAT_ABGR,
  /** Specifies RGBx-8-8-8-8 single plane. */
  NVBUF_COLOR_FORMAT_RGBx,
  /** Specifies BGRx-8-8-8-8 single plane. */
  NVBUF_COLOR_FORMAT_BGRx,
  /** Specifies xRGB-8-8-8-8 single plane. */
  NVBUF_COLOR_FORMAT_xRGB,
  /** Specifies xBGR-8-8-8-8 single plane. */
  NVBUF_COLOR_FORMAT_xBGR,
  /** Specifies RGB-8-8-8 single plane. */
  NVBUF_COLOR_FORMAT_RGB,
  /** Specifies BGR-8-8-8 single plane. */
  NVBUF_COLOR_FORMAT_BGR,
  /** Specifies BT.601 colorspace - Y/CbCr 4:2:0 10-bit multi-planar. */
  NVBUF_COLOR_FORMAT_NV12_10LE,
  /** Specifies BT.601 colorspace - Y/CbCr 4:2:0 12-bit multi-planar. */
  NVBUF_COLOR_FORMAT_NV12_12LE,
  /** Specifies BT.709 colorspace - YUV420 multi-planar. */
  NVBUF_COLOR_FORMAT_YUV420_709,
  /** Specifies BT.709 colorspace - YUV420 ER multi-planar. */
  NVBUF_COLOR_FORMAT_YUV420_709_ER,
  /** Specifies BT.709 colorspace - Y/CbCr 4:2:0 multi-planar. */
  NVBUF_COLOR_FORMAT_NV12_709,
  /** Specifies BT.709 colorspace - Y/CbCr ER 4:2:0 multi-planar. */
  NVBUF_COLOR_FORMAT_NV12_709_ER,
  /** Specifies BT.2020 colorspace - YUV420 multi-planar. */
  NVBUF_COLOR_FORMAT_YUV420_2020,
  /** Specifies BT.2020 colorspace - Y/CbCr 4:2:0 multi-planar. */
  NVBUF_COLOR_FORMAT_NV12_2020,
  /** Specifies BT.601 colorspace - Y/CbCr ER 4:2:0 10-bit multi-planar. */
  NVBUF_COLOR_FORMAT_NV12_10LE_ER,
  /** Specifies BT.709 colorspace - Y/CbCr 4:2:0 10-bit multi-planar. */
  NVBUF_COLOR_FORMAT_NV12_10LE_709,
  /** Specifies BT.709 colorspace - Y/CbCr ER 4:2:0 10-bit multi-planar. */
  NVBUF_COLOR_FORMAT_NV12_10LE_709_ER,
  /** Specifies BT.2020 colorspace - Y/CbCr 4:2:0 10-bit multi-planar. */
  NVBUF_COLOR_FORMAT_NV12_10LE_2020,
  /** Specifies color format for packed 2 signed shorts  */
  NVBUF_COLOR_FORMAT_SIGNED_R16G16,

  NVBUF_COLOR_FORMAT_LAST
} NvBufSurfaceColorFormat;

/**
 * Specifies layout formats for \ref NvBufSurface video planes.
 */
typedef enum
{
  /** Specifies pitch layout. */
  NVBUF_LAYOUT_PITCH,
  /** Specifies block linear layout. */
  NVBUF_LAYOUT_BLOCK_LINEAR,
} NvBufSurfaceLayout;

/**
 * Specifies memory types for \ref NvBufSurface.
 */
typedef enum
{
  /** Specifies the default memory type, i.e. \ref NVBUF_MEM_CUDA_DEVICE
   for dGPU, \ref NVBUF_MEM_SURFACE_ARRAY for Jetson. Use \ref NVBUF_MEM_DEFAULT
   to allocate whichever type of memory is appropriate for the platform. */
  NVBUF_MEM_DEFAULT,
  /** Specifies CUDA Host memory type. */
  NVBUF_MEM_CUDA_PINNED,
  /** Specifies CUDA Device memory type. */
  NVBUF_MEM_CUDA_DEVICE,
  /** Specifies CUDA Unified memory type. */
  NVBUF_MEM_CUDA_UNIFIED,
  /** Specifies NVRM Surface Array type. Valid only for Jetson. */
  NVBUF_MEM_SURFACE_ARRAY,
  /** Specifies NVRM Handle type. Valid only for Jetson. */
  NVBUF_MEM_HANDLE,
  /** Specifies memory allocated by malloc(). */
  NVBUF_MEM_SYSTEM,
} NvBufSurfaceMemType;

/**
 * Holds the planewise parameters of a buffer.
 */
typedef struct NvBufSurfacePlaneParams
{
  /** Holds the number of planes. */
  uint32_t num_planes;
  /** Holds the widths of planes. */
  uint32_t width[NVBUF_MAX_PLANES];
  /** Holds the heights of planes. */
  uint32_t height[NVBUF_MAX_PLANES];
  /** Holds the pitches of planes in bytes. */
  uint32_t pitch[NVBUF_MAX_PLANES];
  /** Holds the offsets of planes in bytes. */
  uint32_t offset[NVBUF_MAX_PLANES];
  /** Holds the sizes of planes in bytes. */
  uint32_t psize[NVBUF_MAX_PLANES];
  /** Holds the number of bytes occupied by a pixel in each plane. */
  uint32_t bytesPerPix[NVBUF_MAX_PLANES];

  void * _reserved[STRUCTURE_PADDING * NVBUF_MAX_PLANES];
} NvBufSurfacePlaneParams;

/**
 * Holds parameters required to allocate an \ref NvBufSurface.
 */
typedef struct NvBufSurfaceCreateParams {
  /** Holds the GPU ID. Valid only for a multi-GPU system. */
  uint32_t gpuId;
  /** Holds the width of the buffer. */
  uint32_t width;
  /** Holds the height of the buffer. */
  uint32_t height;
  /** Holds the amount of memory to be allocated. Optional; if set, all other
   parameters (width, height, etc.) are ignored. */
  uint32_t size;
  /** Holds a "contiguous memory" flag. If set, contiguous memory is allocated
   for the batch. Valid only for CUDA memory types. */
  bool isContiguous;
  /** Holds the color format of the buffer. */
  NvBufSurfaceColorFormat colorFormat;
  /** Holds the surface layout. May be Block Linear (BL) or Pitch Linear (PL).
   For a dGPU, only PL is valid. */
  NvBufSurfaceLayout layout;
  /** Holds the type of memory to be allocated. */
  NvBufSurfaceMemType memType;
} NvBufSurfaceCreateParams;

/**
 * Holds pointers for a mapped buffer.
 */
typedef struct NvBufSurfaceMappedAddr {
  /** Holds planewise pointers to a CPU mapped buffer. */
  void * addr[NVBUF_MAX_PLANES];
  /** Holds a pointer to a mapped EGLImage. */
  void *eglImage;

  void * _reserved[STRUCTURE_PADDING];
} NvBufSurfaceMappedAddr;

/**
 * Holds information about a single buffer in a batch.
 */
typedef struct NvBufSurfaceParams {
  /** Holds the width of the buffer. */
  uint32_t width;
  /** Holds the height of the buffer. */
  uint32_t height;
  /** Holds the pitch of the buffer. */
  uint32_t pitch;
  /** Holds the color format of the buffer. */
  NvBufSurfaceColorFormat colorFormat;
  /** Holds BL or PL. For dGPU, only PL is valid. */
  NvBufSurfaceLayout layout;
  /** Holds a DMABUF FD. Valid only for \ref NVBUF_MEM_SURFACE_ARRAY and
   \ref NVBUF_MEM_HANDLE type memory. */
  uint64_t bufferDesc;
  /** Holds the amount of allocated memory. */
  uint32_t dataSize;
  /** Holds a pointer to allocated memory. Not valid for
   \ref NVBUF_MEM_SURFACE_ARRAY or \ref NVBUF_MEM_HANDLE. */
  void * dataPtr;
  /** Holds planewise information (width, height, pitch, offset, etc.). */
  NvBufSurfacePlaneParams planeParams;
  /** Holds pointers to mapped buffers. Initialized to NULL
   when the structure is created. */
  NvBufSurfaceMappedAddr mappedAddr;

  void * _reserved[STRUCTURE_PADDING];
} NvBufSurfaceParams;

/**
 * Holds information about batched buffers.
 */
typedef struct NvBufSurface {
  /** Holds a GPU ID. Valid only for a multi-GPU system. */
  uint32_t gpuId;
  /** Holds the batch size. */
  uint32_t batchSize;
  /** Holds the number valid and filled buffers. Initialized to zero when
   an instance of the structure is created. */
  uint32_t numFilled;
  /** Holds an "is contiguous" flag. If set, memory allocated for the batch
   is contiguous. */
  bool isContiguous;
  /** Holds type of memory for buffers in the batch. */
  NvBufSurfaceMemType memType;
  /** Holds a pointer to an array of batched buffers. */
  NvBufSurfaceParams *surfaceList;

  void * _reserved[STRUCTURE_PADDING];
} NvBufSurface;

/**
 * \brief  Allocates a batch of buffers.
 *
 * Allocates memory for \a batchSize buffers and returns a pointer to an
 * allocated \ref NvBufSurface. The \a params structure must have
 * the allocation parameters of a single buffer. If \a params.size
 * is set, a buffer of that size is allocated, and all other
 * parameters (width, height, color format, etc.) are ignored.
 *
 * Call NvBufSurfaceDestroy() to free resources allocated by this function.
 *
 * @param[out] surf         An indirect pointer to the allocated batched
 *                           buffers.
 * @param[in]  batchSize    Batch size of buffers.
 * @param[in]  params       A pointer to an \ref NvBufSurfaceCreateParams
 *                           structure.
 *
 * @return 0 if successful, or -1 otherwise.
 */
int NvBufSurfaceCreate (NvBufSurface **surf, uint32_t batchSize,
                        NvBufSurfaceCreateParams *params);

/**
 * \brief  Frees batched buffers previously allocated by NvBufSurfaceCreate().
 *
 * @param[in] surf  A pointer to an \ref NvBufSurface to be freed.
 *
 * @return 0 if successful, or -1 otherwise.
 */
int NvBufSurfaceDestroy (NvBufSurface *surf);

/**
 * \brief  Maps hardware batched buffers to the HOST or CPU address space.
 *
 * Valid for \ref NVBUF_MEM_CUDA_UNIFIED type memory for dGPU and
 * \ref NVBUF_MEM_SURFACE_ARRAY and \ref NVBUF_MEM_HANDLE type memory for
 * Jetson.
 *
 * This function fills an array of pointers at
 * \a surf->surfaceList->mappedAddr->addr.
 * \a surf is a pointer to an \ref NvBufSurface.
 * \a surfaceList is a pointer to an \ref NvBufSurfaceParams.
 * \a mappedAddr is a pointer to an \ref NvBufSurfaceMappedAddr.
 * \a addr is declared as an array of pointers to void, and holds pointers
 * to the buffers.
 *
 * The client must call NvBufSurfaceSyncForCpu() with the virtual address
 * populated by this function before accessing mapped memory in the CPU.
 *
 * After memory mapping is complete, mapped memory modification
 * must be coordinated between the CPU and the hardware device as
 * follows:
 * - CPU: If the CPU modifies mapped memory, the client must call
 *   NvBufSurfaceSyncForDevice() before any hardware device accesses the memory.
 * - Hardware device: If a hardware device modifies mapped memory, the client
 *   must call NvBufSurfaceSyncForCpu() before the CPU accesses the memory.
 *
 * Use NvBufSurfaceUnMap() to unmap buffer(s) and release any resource.
 *
 * @param[in,out] surf  A pointer to an NvBufSurface structure. The function
 *                      stores pointers to the buffers in a descendant of this
 *                      structure; see the notes above.
 * @param[in] index     Index of a buffer in the batch. -1 refers to all buffers
 *                      in the batch.
 * @param[in] plane     Index of a plane in buffer. -1 refers to all planes
 *                      in the buffer.
 * @param[in] type      A flag for mapping type.
 *
 * @return 0 if successful, or -1 otherwise.
 */
int NvBufSurfaceMap (NvBufSurface *surf, int index, int plane, NvBufSurfaceMemMapFlags type);

/**
 * \brief  Unmaps previously mapped buffer(s).
 *
 * @param[in] surf      A pointer to an \ref NvBufSurface structure.
 * @param[in] index     Index of a buffer in the batch. -1 indicates
 *                      all buffers in the batch.
 * @param[in] plane     Index of a plane in the buffer. -1 indicates
 *                      all planes in the buffer.
 *
 * @return  0 if successful, or -1 otherwise.
 */
int NvBufSurfaceUnMap (NvBufSurface *surf, int index, int plane);

/**
 * \brief  Copies the content of source batched buffer(s) to destination
 * batched buffer(s).
 *
 * You can use this function to copy source buffer(s) of one memory type
 * to destination buffer(s) of another memory type,
 * e.g. CUDA host to CUDA device, malloc'ed memory to CUDA device, etc.
 *
 * The source and destination \ref NvBufSurface objects must have same
 * buffer and batch size.
 *
 * @param[in] srcSurf   A pointer to the source NvBufSurface structure.
 * @param[in] dstSurf   A pointer to the destination NvBufSurface structure.
 *
 * @return 0 if successful, or -1 otherwise.
 */
int NvBufSurfaceCopy (NvBufSurface *srcSurf, NvBufSurface *dstSurf);

/**
 * \brief  Syncs the hardware memory cache for the CPU.
 *
 * Valid only for memory types \ref NVBUF_MEM_SURFACE_ARRAY and
 * \ref NVBUF_MEM_HANDLE.
 *
 * @param[in] surf      A pointer to an \ref NvBufSurface structure.
 * @param[in] index     Index of the buffer in the batch. -1 refers to
 *                      all buffers in the batch.
 * @param[in] plane     Index of a plane in the buffer. -1 refers to all planes
 *                      in the buffer.
 *
 * @return 0 if successful, or -1 otherwise.
 */
int NvBufSurfaceSyncForCpu (NvBufSurface *surf, int index, int plane);

/**
 * \brief  Syncs the hardware memory cache for the device.
 *
 * Valid only for memory types \ref NVBUF_MEM_SURFACE_ARRAY and
 * \ref NVBUF_MEM_HANDLE.
 *
 * @param[in] surf      A pointer to an \ref NvBufSurface structure.
 * @param[in] index     Index of a buffer in the batch. -1 refers to all buffers
 *                      in the batch.
 * @param[in] plane     Index of a plane in the buffer. -1 refers to all planes
 *                      in the buffer.
 *
 * @return 0 if successful, or -1 otherwise.
 */
int NvBufSurfaceSyncForDevice (NvBufSurface *surf, int index, int plane);

/**
 * \brief  Gets the \ref NvBufSurface from the DMABUF FD.
 *
 * @param[in]  dmabuf_fd    DMABUF FD of the buffer.
 * @param[out] buffer       A pointer to the NvBufSurface.
 *
 * @return 0 for success, or -1 otherwise.
 */
int NvBufSurfaceFromFd (int dmabuf_fd, void **buffer);

/**
 * \brief  Fills each byte of the buffer(s) in an \ref NvBufSurface with a
 * provided value.
 *
 * You can also use this function to reset the buffer(s) in the batch.
 *
 * @param[in] surf  A pointer to the NvBufSurface structure.
 * @param[in] index Index of a buffer in the batch. -1 refers to all buffers
 *                  in the batch.
 * @param[in] plane Index of a plane in the buffer. -1 refers to all planes
 *                  in the buffer.
 * @param[in] value The value to be used as fill.
 *
 * @return 0 if successful, or -1 otherwise.
 */
int NvBufSurfaceMemSet (NvBufSurface *surf, int index, int plane, uint8_t value);

/**
 * \brief  Creates an EGLImage from the memory of one or more
 * \ref NvBufSurface buffers.
 *
 * Only memory type \ref NVBUF_MEM_SURFACE_ARRAY is supported.
 *
 * This function returns the created EGLImage by storing its address at
 * \a surf->surfaceList->mappedAddr->eglImage. (\a surf is a pointer to
 * an NvBufSurface. \a surfaceList is a pointer to an \ref NvBufSurfaceParams.
 * \a mappedAddr is a pointer to an \ref NvBufSurfaceMappedAddr.
 * \a eglImage is declared as a pointer to void, and holds an
 * EGLImageKHR.)
 *
 * You can use this function in scenarios where a CUDA operation on Jetson
 * hardware memory (identified by \ref NVBUF_MEM_SURFACE_ARRAY) is required.
 * The EGLImageKHR struct provided by this function can then be registered
 * with CUDA for further CUDA operations.
 *
 * @param[in,out] surf  A pointer to an NvBufSurface structure. The function
 *                      stores a pointer to the created EGLImage in
 *                      a descendant of this structure; see the notes above.
 * @param[in]     index Index of a buffer in the batch. -1 specifies all buffers
 *                      in the batch.
 *
 * @return 0 for success, or -1 otherwise.
 */
int NvBufSurfaceMapEglImage (NvBufSurface *surf, int index);

/**
 * \brief  Destroys the previously created EGLImage object(s).
 *
 * @param[in] surf      A pointer to an \ref NvBufSurface structure.
 * @param[in] index     The index of a buffer in the batch. -1 specifies all
 *                      buffers in the batch.
 *
 * @return 0 if successful, or -1 otherwise.
 */
int NvBufSurfaceUnMapEglImage (NvBufSurface *surf, int index);

/** @} */

#ifdef __cplusplus
}
#endif
#endif /* NVBUFSURFACE_H_ */
