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
 * <b>Defines dewarping metadata </b>
 *
 * @b Description: This file defines metadata concerning dewarping.
 */

/**
 * @defgroup ee_dewarping_group Dewarping Metadata
 * Defines metadata concerning dewarping.
 * @ingroup NvDsMetaApi
 * @{
 */

#ifndef _NVDS_DEWARPER_META_H_
#define _NVDS_DEWARPER_META_H_

#include <gst/gst.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Defines DeepStream Dewarper metadata.
 */

/**
 * Maximum number of dewarped surfaces per frame supported
 */
#define MAX_DEWARPED_VIEWS 4

/**
 * Defines metadata surface types.
 */
typedef enum
{
    NVDS_META_SURFACE_NONE=0,
    /** Defines the pushbroom surface type. */
    NVDS_META_SURFACE_FISH_PUSHBROOM=1,
    /** Defines the vertical radical cylindrical surface type. */
    NVDS_META_SURFACE_FISH_VERTCYL=2,
    /** Defines the perspective projection with Brown distortion model surface. */
    NVDS_META_SURFACE_PERSPECTIVE_PERSPECTIVE=3,
} NvDsSurfaceType;

/**
 * Holds a set of dewarper metadata.
 */
typedef struct _NvDewarperSurfaceMeta {
  /** Holds an array of the types of dewarped surfaces. */
  guint type[MAX_DEWARPED_VIEWS];
  /** Holds an array of indices of dewarped surfaces. */
  guint index[MAX_DEWARPED_VIEWS];
  /** Holds the source ID of the frame, e.g. the camera ID. */
  guint source_id;
  /** Holds the number of filled surfaces in the frame. */
  guint num_filled_surfaces;
}NvDewarperSurfaceMeta;

#ifdef __cplusplus
}
#endif

#endif

/** @} */
