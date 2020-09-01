/*
 * Copyright (c) 2019, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 *
 */

/**
 * @file nvbufsurftransform.h
 * <b>NvBufSurfTransform Interface </b>
 *
 * This file specifies the NvBufSurfTransform image transformation APIs.
 *
 * The NvBufSurfTransform API provides methods to set and get session parameters
 * and to transform and composite APIs.
 */
#ifndef NVBUFSURFTRANSFORM_H_
#define NVBUFSURFTRANSFORM_H_
#include <cuda.h>
#include <cuda_runtime.h>
#include "nvbufsurface.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup ds_bbb NvBufSurfTransform Types and Functions
 * Defines types and functions of the \ref NvBufSurfTransform
 * application programming interface.
 * @ingroup ds_nvbuf_api
 * @{ */

/**
 * Specifies compute devices used by \ref NvBufSurfTransform.
 */
typedef enum
{
  /** Specifies VIC as a compute device for Jetson or dGPU for an x86_64
   system. */
  NvBufSurfTransformCompute_Default,
  /** Specifies that the GPU is the compute device. */
  NvBufSurfTransformCompute_GPU,
  /** Specifies that the VIC as a compute device. Supported only for Jetson. */
  NvBufSurfTransformCompute_VIC
} NvBufSurfTransform_Compute;


/**
 * Specifies video flip methods. Supported only for Jetson.
 */
typedef enum
{
  /** Specifies no video flip. */
  NvBufSurfTransform_None,
  /** Specifies rotating 90 degrees clockwise. */
  NvBufSurfTransform_Rotate90,
  /** Specifies rotating 180 degree clockwise. */
  NvBufSurfTransform_Rotate180,
  /** Specifies rotating 270 degree clockwise. */
  NvBufSurfTransform_Rotate270,
  /** Specifies video flip with respect to the X-axis. */
  NvBufSurfTransform_FlipX,
  /** Specifies video flip with respect to the Y-axis. */
  NvBufSurfTransform_FlipY,
  /** Specifies video flip transpose. */
  NvBufSurfTransform_Transpose,
  /** Specifies video flip inverse transpose. */
  NvBufSurfTransform_InvTranspose,
} NvBufSurfTransform_Flip;


/**
 * Specifies video interpolation methods.
 */
typedef enum
{
  /** Specifies Nearest Interpolation Method interpolation. */
  NvBufSurfTransformInter_Nearest = 0,
  /** Specifies Bilinear Interpolation Method interpolation. */
  NvBufSurfTransformInter_Bilinear,
  /** Specifies GPU-Cubic, VIC-5 Tap interpolation. */
  NvBufSurfTransformInter_Algo1,
  /** Specifies GPU-Super, VIC-10 Tap interpolation. */
  NvBufSurfTransformInter_Algo2,
  /** Specifies GPU-Lanzos, VIC-Smart interpolation. */
  NvBufSurfTransformInter_Algo3,
  /** Specifies GPU-Ignored, VIC-Nicest interpolation. */
  NvBufSurfTransformInter_Algo4,
  /** Specifies GPU-Nearest, VIC-Nearest interpolation. */
  NvBufSurfTransformInter_Default
} NvBufSurfTransform_Inter;

/**
 * Specifies error codes returned by \ref NvBufSurfTransform functions.
 */
typedef enum
{
  /** Specifies an error in source or destination ROI. */
  NvBufSurfTransformError_ROI_Error = -4,
  /** Specifies invalid input parameters. */
  NvBufSurfTransformError_Invalid_Params = -3,
  /** Specifies a runtime execution error. */
  NvBufSurfTransformError_Execution_Error = -2,
  /** Specifies an unsupported feature or format. */
  NvBufSurfTransformError_Unsupported = -1,
  /** Specifies a successful operation. */
  NvBufSurfTransformError_Success = 0
} NvBufSurfTransform_Error;

/**
 * Specifies transform types.
 */
typedef enum {
  /** Specifies a transform to crop the source rectangle. */
  NVBUFSURF_TRANSFORM_CROP_SRC   = 1,
  /** Specifies a transform to crop the destination rectangle. */
  NVBUFSURF_TRANSFORM_CROP_DST   = 1 << 1,
  /** Specifies a transform to set the filter type. */
  NVBUFSURF_TRANSFORM_FILTER     = 1 << 2,
  /** Specifies a transform to set the flip method. */
  NVBUFSURF_TRANSFORM_FLIP       = 1 << 3,
} NvBufSurfTransform_Transform_Flag;

/**
 * Specifies types of composition operations.
 */
typedef enum {
  /** Specifies a flag to describe the requested compositing operation. */
  NVBUFSURF_TRANSFORM_COMPOSITE  = 1,
} NvBufSurfTransform_Composite_Flag;

/**
 * Holds the coordinates of a rectangle.
 */
typedef struct
{
  /** Holds the rectangle top. */
  uint32_t top;
  /** Holds the rectangle left side. */
  uint32_t left;
  /** Holds the rectangle width. */
  uint32_t width;
  /** Holds the rectangle height. */
  uint32_t height;
}NvBufSurfTransformRect;

/**
 * Holds configuration parameters for a transform/composite session.
 */
typedef struct _NvBufSurfTransformConfigParams
{
  /** Holds the mode of operation: VIC (Jetson) or GPU (iGPU + dGPU)
   If VIC is configured, \a gpu_id is ignored. */
  NvBufSurfTransform_Compute compute_mode;

  /** Holds the GPU ID to be used for processing. */
  int32_t gpu_id;

  /** User configure stream to be used. If NULL, the default stream is used.
   Ignored if VIC is used. */
  cudaStream_t cuda_stream;

} NvBufSurfTransformConfigParams;

/**
 * Holds transform parameters for a transform call.
 */
typedef struct _NvBufSurfaceTransformParams
{
  /** Holds a flag that indicates which transform parameters are valid. */
  uint32_t transform_flag;
  /** Holds the flip method. */
  NvBufSurfTransform_Flip transform_flip;
  /** Holds a transform filter. */
  NvBufSurfTransform_Inter transform_filter;
  /** Holds a pointer to a list of source rectangle coordinates for
   a crop operation. */
  NvBufSurfTransformRect *src_rect;
  /** Holds a pointer to list of destination rectangle coordinates for
   a crop operation. */
  NvBufSurfTransformRect *dst_rect;
}NvBufSurfTransformParams;

/**
 * Holds composite parameters for a composite call.
 */
typedef struct _NvBufSurfTransformCompositeParams
{
  /** Holds a flag that indicates which composition parameters are valid. */
  uint32_t composite_flag;
  /** Holds the number of input buffers to be composited. */
  uint32_t input_buf_count;
 /** Holds source rectangle coordinates of input buffers for compositing. */
  NvBufSurfTransformRect *src_comp_rect;
  /** Holds destination rectangle coordinates of input buffers for
   compositing. */
  NvBufSurfTransformRect *dst_comp_rect;
}NvBufSurfTransformCompositeParams;

/**
 * \brief  Sets user-defined session parameters.
 *
 * If user-defined session parameters are set, they override the
 * NvBufSurfTransform() function's default session.
 *
 * @param[in] config_params     A pointer to a structure that is populated
 *                              with the session parameters to be used.
 *
 * @return  An \ref NvBufSurfTransform_Error value indicating
 *  success or failure.
 */
NvBufSurfTransform_Error NvBufSurfTransformSetSessionParams
(NvBufSurfTransformConfigParams *config_params);

/**
 * \brief Gets the session parameters used by NvBufSurfTransform().
 *
 * @param[out] config_params    A pointer to a caller-allocated structure to be
 *                              populated with the session parameters used.
 *
 * @return  An \ref NvBufSurfTransform_Error value indicating
 *  success or failure.
 */
NvBufSurfTransform_Error NvBufSurfTransformGetSessionParams
(NvBufSurfTransformConfigParams *config_params);

/**
 * \brief Performs a transformation on batched input images.
 *
 * If user-defined session parameters are to be used, call
 * NvBufSurfTransformSetSessionParams() before calling this function.
 *
 * @param[in]  src  A pointer to input batched buffers to be transformed.
 * @param[out] dst  A pointer to a caller-allocated location where
 *                  transformed output is to be stored.
 *                  @par When destination cropping is performed, memory outside
 *                  the crop location is not touched, and may contain stale
 *                  information. The caller must perform a memset before
 *                  calling this function if stale information must be
 *                  eliminated.
 * @param[in]  transform_params
 *                  A pointer to an \ref NvBufSurfTransformParams structure
 *                  which specifies the type of transform to be performed. They
 *                  may include any combination of scaling, format conversion,
 *                  and cropping for both source and destination.
 *                  Flipping and rotation are supported on VIC.
 * @return  An \ref NvBufSurfTransform_Error value indicating
 *  success or failure.
 */
NvBufSurfTransform_Error NvBufSurfTransform (NvBufSurface *src, NvBufSurface *dst,
    NvBufSurfTransformParams *transform_params);

/**
 * \brief  Composites batched input images.
 *
 * The compositer scales and stitches
 * batched buffers indicated by \a src into a single destination buffer, \a dst.
 *
 * If user-defined session parameters are to be used, call
 * NvBufSurfTransformSetSessionParams() before calling this function.
 *
 * @param[in]  src  A pointer to input batched buffers to be transformed.
 * @param[out] dst  A pointer a caller-allocated location (a single buffer)
 *                  where composited output is to be stored.
 * @param[in]  composite_params
 *                  A pointer to an \ref NvBufSurfTransformCompositeParams
 *                  structure which specifies the compositing operation to be
 *                  performed, e.g., the source and destination rectangles
 *                  in \a src and \a dst.
 * @return An \ref NvBufSurfTransform_Error value indicating success or failure.
 */
NvBufSurfTransform_Error NvBufSurfTransformComposite (NvBufSurface *src,
    NvBufSurface *dst, NvBufSurfTransformCompositeParams *composite_params);
/** @} */
#ifdef __cplusplus
}
#endif
#endif
