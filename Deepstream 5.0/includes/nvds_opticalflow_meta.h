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
 * @file
 * <b>NVIDIA Optical Flow Metadata </b>
 *
 * @b Description: This file defines the optical flow metadata.
 */

/**
 * @defgroup  ee_opticalflow_meta  Optical flow metadata
 *
 * Defines the optical flow metadata.
 * @ingroup NvDsMetaApi
 * @{
 */

#ifndef _NVDS_OPTICALFLOW_META_H_
#define _NVDS_OPTICALFLOW_META_H_

#include <gst/gst.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Holds motion vector information about an element.
 */
typedef struct _NvOFFlowVector
{
    /** Holds the motion vector X component. */
    gshort flowx;

    /** Holds the motion vector Y component. */
    gshort flowy;
} NvOFFlowVector;

/**
 * Holds optical flow metadata about a frame.
 */
typedef struct
{
    /** Holds the number of rows in the frame for a given block size,
     e.g. if block size is 4 and frame height is 720, then the number of
     rows is (720/4) = 180. */
    guint rows;
    /** Holds the number of columns in the frame for given block size,
     e.g. if block size is 4 and frame width is 1280, then the number of
     columns is (1280/4) = 320. */
    guint cols;
    /** Holds the size of the motion vector. @see NvOFFlowVector. */
    guint mv_size;
    /** Holds the current frame number of the source. */
    gulong frame_num;
    /** Holds a pointer to the motion vector. */
    void *data;
    /** Reserved for internal use. */
    void *priv;
    /** Reserved for internal use. */
    void *reserved;
} NvDsOpticalFlowMeta;

#ifdef __cplusplus
}
#endif

#endif

/** @} */