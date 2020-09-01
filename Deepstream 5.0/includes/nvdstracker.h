/*
 * Copyright (c) 2019-2020, NVIDIA CORPORATION.  All rights reserved.
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
 * <b>DeepStream object tracker API </b>
 *
 * @b Description: This file defines the DeepStream object tracker API.
 */

/**
 * @defgroup  ee_NvMOTracker  Object Tracker API
 *
 * Defines the DeepStream object tracker API.
 *
 * @ingroup NvDsTrackerApi
 * @{
 */

#ifndef _NVMOTRACKER_H_
#define _NVMOTRACKER_H_

#include <stdint.h>
#include <time.h>

#include "nvbufsurface.h"
#include "nvds_tracker_meta.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define NVMOT_MAX_TRANSFORMS 4

typedef uint64_t NvMOTStreamId;

/**
 * @brief Compute target flags.
 *
 * You can select multiple targets; the tracker will optimize across them.
 * Combinations are allowed, e.g. NVTCOMP_GPU or NVTCOMP_PVA
 */
#define NVMOTCOMP_GPU     0x01  /**< Defines the "GPU" compute target flag. */
#define NVMOTCOMP_CPU     0x02  /**< Defines the "CPU" compute target flag. */
#define NVMOTCOMP_PVA     0x04  /**< Defines the "PVA" compute target flag. */
#define NVMOTCOMP_ANY     0xff  /**< Defines a compute target flag for
                                 "any target." */
#define NVMOTCOMP_DEFAULT NVMOTCOMP_ANY
                                /**< Defines the compute target flag for the
                                 default target. */
/** @} */

typedef uint8_t NvMOTCompute;

/**
 * @brief Holds a configuration for batches for an input transform
 *  (a scaling/color conversion).
 */
typedef struct _NvMOTPerTransformBatchConfig
{
    /** Holds the type of buffer. */
    NvBufSurfaceMemType bufferType;
    /** Holds the maximum width of each frame. */
    uint32_t maxWidth;
    /** Holds the maximum height of each frame. */
    uint32_t maxHeight;
    /** Holds the maximum pitch of each buffer. */
    uint32_t maxPitch;
    /** Holds the maximum size of the buffer in bytes. */
    uint32_t maxSize;
    /** Holds the color format: RGB, NV12 etc. */
    uint32_t colorFormat;
} NvMOTPerTransformBatchConfig;


/**
 * @brief Holds miscellaneous configurations.
 */
typedef struct _NvMOTMiscConfig
{
    /** Holds the ID of the GPU to be used. */
    uint32_t gpuId;
    /** Holds the maximum number of objects to track per stream. 0 means
     track an unlimited number of objects. */
    uint32_t maxObjPerStream;
    /** Holds the maximum number of objects to track per batch. 0 means
     track an unlimited number of objects. */
    uint32_t maxObjPerBatch;
    /** Holds a pointer to a callback for logging messages. */
    typedef void (*logMsg) (int logLevel, const char * format, ...);
} NvMOTMiscConfig;

/**
 * @brief Holds a tracker configuration.
 *
 * Holds configuration options for the tracker, applied to the whole context.
 *
 * @note This structure must be deep-copied to be passed to a component that
 * is to use it persistently.
 */
typedef struct _NvMOTConfig
{
    /** Holds the compute target. @see NvMOTCompute. */
    NvMOTCompute computeConfig;
    /** Holds the maximum number of streams in a batch. */
    uint32_t maxStreams;
    /** Holds the number of \ref NvMOTPerTransformBatchConfig entries in
     @a perTransformBatchConfig. */
    uint8_t numTransforms;
    /** Holds a pointer to a list of @a numTransform batch configurations,
     one per transform, including type and resolution. */
    NvMOTPerTransformBatchConfig *perTransformBatchConfig;
    /** Holds miscellaneous configurations. */
    NvMOTMiscConfig miscConfig;
    /** Holds the length of @a customConfigFilePath. */
    uint16_t customConfigFilePathSize;
    /** A pointer to the pathname of the tracker's custom configuration file.
     A null-terminated string. */
    char* customConfigFilePath;
} NvMOTConfig;

/**
 * @brief Defines configuration request return codes.
 */
typedef enum
{
    NvMOTConfigStatus_OK,
    NvMOTConfigStatus_Error,
    NvMOTConfigStatus_Invalid,
    NvMOTConfigStatus_Unsupported
} NvMOTConfigStatus;

/**
 * @brief Holds a tracker's configuration status.
 *
 * Holds the status of a configuration request; includes both summary and
 * per-configuration status.
 */
typedef struct _NvMOTConfigResponse
{
    /** Holds the summary status of the entire configuration request. */
    NvMOTConfigStatus summaryStatus;
    /** Holds the compute target request status. */
    NvMOTConfigStatus computeStatus;
    /** Holds the transform batch configuration request status:
     summary status for all transforms. */
    NvMOTConfigStatus transformBatchStatus;
    /** Holds the status of the miscellaneous configurations. */
    NvMOTConfigStatus miscConfigStatus;
    /** Holds the status of the custom configurations. */
    NvMOTConfigStatus customConfigStatus;
} NvMOTConfigResponse;

/**
 * @brief Defines generic status codes for tracking operations.
 */
typedef enum
{
    NvMOTStatus_OK,
    NvMOTStatus_Error,
    NvMOTStatus_Invalid_Path
} NvMOTStatus;

/**
 * @brief Holds the definition of a rectangle.
 */
typedef struct _NvMOTRect
{
    /** Holds the left edge position of the object bounding box, in pixels. */
    float x;
    /** Holds the top edge position of the object bounding box, in pixels. */
    float y;
    /** Holds the width of the bounding box, in pixels. */
    float width;
    /** Holds the height of the bounding box, in pixels. */
    float height;
} NvMOTRect;

/**
 * @brief Holds information about an object to be tracked.
 *
 * NvMOT creates an instance of this structure for each tracked object.
 */
typedef struct _NvMOTObjToTrack
{
    /** Holds the class of the object. */
    uint16_t classId;
    /** Holds the bounding box of the object. */
    NvMOTRect bbox;
    /** Holds the detection confidence of the object. */
    float confidence;
    /** Holds a Boolean which is true if NvMOT is to track this object. */
    bool doTracking;
    /** Holds a pointer to client data associated with the object. */
    void *pPreservedData;
} NvMOTObjToTrack;

/**
 * @brief Holds a list of objects.
 */
typedef struct _NvMOTObjToTrackList
{
    /** Holds a Boolean which is true if detection was done on this frame
     even if the list of objects to track is empty. */
    bool detectionDone;
    /** Holds a pointer to a list or array of object information blocks. */
    NvMOTObjToTrack* list;
    /** Holds the number of blocks allocated for the list. */
    uint32_t numAllocated;
    /** Holds the number of populated blocks in the list. */
    uint32_t numFilled;
} NvMOTObjToTrackList;

/**
 * @brief Holds a frame containing the image and objects to be tracked.
 *
 * @note @a numBuffers is supposed to be less than or equal to
 *  @a numTransforms in @ref NvMOTConfig.
 * @note The metadata in the NvBufSurfaceParams structures
 *  which @a bufferList points to must be checked with the parameters
 *  specified in @a perTransformBatchConfig in NvMOTConfig.
 */
typedef struct _NvMOTFrame
{
    /** Holds the stream ID of the stream source for this frame. */
    NvMOTStreamId streamID;
    /** Holds the sequential frame number that identifies the frame
     within the stream. */
    uint32_t frameNum;
    /** Holds the timestamp of the frame at the time of capture. */
    time_t timeStamp;
    /** Holds a Boolean which is true if the timestamp value is properly
     populated. */
    bool timeStampValid;
    /** Holds a Boolean which is true if objects in this frame are to be
     tracked. */
    bool doTracking;
    /** Holds a Boolean which is true to reset tracking for the stream. */
    bool reset;
    /** Holds the number of entries in @a bufferList. */
    uint8_t numBuffers;
    /** Holds a pointer to an array of pointers to buffer parameter
     structures. */
    NvBufSurfaceParams** bufferList;
    /** Holds a list of objects in this frame which are to be tracked.
     Boundary boxes are scaled for the first buffer configuration. */
    NvMOTObjToTrackList objectsIn;
} NvMOTFrame;

/**
 * @brief Holds information about each tracked object.
 */
typedef struct _NvMOTTrackedObj
{
    /** Holds the class ID of the object to be tracked. */
    uint16_t classId;
    /** Holds a unique ID for the object, assigned by the tracker. */
    uint64_t trackingId;
    /** Holds the bounding box. */
    NvMOTRect bbox;
    /** Holds the tracking confidence of the object. */
    float confidence;
    /** Holds the track length in frames. */
    uint32_t age;
    /** Holds a pointer to the associated input object, if there is one. */
    NvMOTObjToTrack *associatedObjectIn;
    uint8_t reserved[128];
} NvMOTTrackedObj;

/**
 * @brief Holds a list of tracked objects.
 */
typedef struct _NvMOTTrackedObjList
{
    /** Holds the stream ID of the stream associated with objects in the list.*/
    NvMOTStreamId streamID;
    /** Holds the frame number for objects in the list. */
    uint32_t frameNum;
    /** Holds a Boolean which is true if this entry in the batch is valid. */
    bool valid;
    /** Holds a pointer to a list or array of object information blocks. */
    NvMOTTrackedObj* list;
    /** Holds the number of blocks allocated for the list. */
    uint32_t numAllocated;
    /** Holds the number of populated blocks in the list. */
    uint32_t numFilled;
} NvMOTTrackedObjList;

/**
 * @brief Holds a batch of lists of tracked objects.
 */
typedef struct _NvMOTTrackedObjBatch
{
    /** Holds a pointer to an array of object lists. */
    NvMOTTrackedObjList *list;
    /** Holds the number of blocks allocated for the list. */
    uint32_t numAllocated;
    /** Holds the number of filled blocks in the list. */
    uint32_t numFilled;
} NvMOTTrackedObjBatch;

/**
 * @brief Holds parameters for processing a batch.
 *
 * @see NvMOTProcessFrame.
 */
typedef struct _NvMOTProcessParams
{
    uint32_t numFrames;     /**< Holds the number of frames in the batch. */
    NvMOTFrame *frameList;  /**< Holds a pointer to an array of frame data. */
} NvMOTProcessParams;

typedef struct _NvMOTQuery
{
    /** Holds flags for supported compute targets. @see NvMOTCompute. */
    NvMOTCompute computeConfig;
    /** Holds the number of \ref NvMOTPerTransformBatchConfig entries
     in perTransformBatchConfig. */
    uint8_t numTransforms;
    /** Holds the color formats for input buffers; a required value. */
    NvBufSurfaceColorFormat colorFormats[NVMOT_MAX_TRANSFORMS];
    /** Holds the preferred memory type for input buffers. */
    NvBufSurfaceMemType memType;
    /** Holds a Boolean which is true if batch processing is supported. */
    bool supportBatchProcessing;
    /** Holds a Boolean which is true if outputing past frame is supported. */
    bool supportPastFrame;
} NvMOTQuery;

/**
 * @brief Holds an opaque context handle.
 */
struct NvMOTContext;
typedef struct NvMOTContext* NvMOTContextHandle;

/**
 * @brief Initializes a tracking context for a batch of one or more image
 *  streams.
 *
 * If successful, the context is configured as specified by @a pConfigIn.
 *
 * @param [in]  pConfigIn       A pointer to to a structure specifying
 *                              the configuration.
 * @param [out] pContextHandle  A pointer to a handle for the stream context.
 *                              The stream context is created and owned
 *                              by the tracker. The returned context handle
 *                              must be included in
 *                              all subsequent calls for the specified stream.
 * @param [out] pConfigResponse A pointer to a structure that describes the
 *                              operation's status.
 * @return  The outcome of the initialization attempt.
 */
NvMOTStatus NvMOT_Init(NvMOTConfig *pConfigIn,
                       NvMOTContextHandle *pContextHandle,
                       NvMOTConfigResponse *pConfigResponse);

/**
 * @brief Deinitializes a stream context
 *
 * The specified context is retired and may not be used again.
 *
 * @param contextHandle     The handle for the stream context to be retired.
 */
void NvMOT_DeInit(NvMOTContextHandle contextHandle);

/**
 * @brief Processes a batch.
 *
 * Given a context and a batch of frames, processes the batch as the current
 * frames in their respective streams. Once processed, each frame becomes part
 * of the history and the previous frame in its stream.
 *
 * @param [in]  contextHandle  A context handle obtained from NvMOTInit().
 * @param [in]  pParams        A pointer to parameters for the batch
 *                             to be processed.
 * @param [out] pTrackedObjectsBatch
 *                             A pointer to a batch of lists of tracked object
 *                             slots to be filled by the tracker. The batch is
 *                             allocated by the client. Bounding boxes are
 *                             scaled to the resolution of the first input
 *                             image transform buffer.
 * @return  The status of batch processing.
 */
NvMOTStatus NvMOT_Process(NvMOTContextHandle contextHandle,
                          NvMOTProcessParams *pParams,
                          NvMOTTrackedObjBatch *pTrackedObjectsBatch);

/**
 * @brief Process the past-frame data in the low-level tracker lib and retrieve
 *
 * Given a context and batch of frame(s), process the past-frame data of each tracked object stored in the low-level tracker lib
 * , put it into the past-frame data strcture, and retrieve it
 *
 * @param [in] pContext The context handle obtained from NvMOTInit()
 * @param [in] pParams Pointer to parameters for the batch of frames with the available stream ID
 * @param [out] pPastFrameObjBatch Batch of lists of tracked objects that are stored by the low-level tracker in the past frames
                BBoxes are scaled to the resolution of the first input image transform buffer.
 * @return Status of batch processing
 */
NvMOTStatus NvMOT_ProcessPast(NvMOTContextHandle contextHandle,
                          NvMOTProcessParams *pParams,
                          NvDsPastFrameObjBatch *pPastFrameObjBatch);
/**
 * @brief Queries the tracker library's capabilities and requirements.
 *
 * Answer query for this tracker lib's capabilities and requirements.
 * The tracker's custom config file is provided for optional consultation.
 *
 * @param [in]  customConfigFilePathSize Length of the custom configuration
 *                                      file's pathname.
 * @param [in]  pCustomConfigFilePath   A pointer to the custom configuration
 *                                      file's pathname.
 * @param [out] pQuery                  A pointer to a query structure to be
 *                                      filled by the tracker.
 * @return  Status of the query.
 */
NvMOTStatus NvMOT_Query(uint16_t customConfigFilePathSize, char* pCustomConfigFilePath, NvMOTQuery *pQuery);


/**
 * @brief Removes streams from a batch.
 *
 * An optional function used in batch processing mode only. It notifies
 * the tracker library that a stream has been removed,
 * and will not be present in future batches. Any per-stream resources
 * associated with the removed stream may be freed at this time.
 *
 * This function may be called only when all processing is quiesced.
 *
 * @param [in] contextHandle    The context handle obtained from NvMOTInit().
 * @param [in] streamIdMask     A mask for finding streams to remove.
 *                              The function reports removal of all streams
 *                              where (streamId & streamIdMask) == streamIdMask.
 */
void NvMOT_RemoveStreams(NvMOTContextHandle contextHandle, NvMOTStreamId streamIdMask);

/** @} */ // end of API group

#ifdef __cplusplus
}
#endif

#endif
