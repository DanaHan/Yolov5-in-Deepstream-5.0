/*
 * Copyright (c) 2019-2020, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

/**
 * @file
 * <b>NVIDIA DeepStream API for importing Transfer Learning Toolkit
 *  encoded models </b>
 *
 * @b Description: This file specifies the API to decode and create
 * a CUDA engine file from a Tranfer Learning Toolkit (TLT) encoded model.
 */

/**
 * @defgroup ee_nvdsinfer_tlt  Import Transfer Learning Toolkit Encoded Models
 *
 * Defines an API for importing Transfer Learning Toolkit encoded models.
 *
 * @ingroup NvDsInferApi
 * @{
 */

#ifndef __NVDSINFER_TLT_H__
#define __NVDSINFER_TLT_H__

#include <nvdsinfer_custom_impl.h>

/**
 * \brief  Decodes and creates a CUDA engine file from a TLT encoded model.
 *
 * This function implements the @ref NvDsInferCudaEngineGet interface. The
 * correct key and model path must be provided in the @a tltModelKey and
 * @a tltEncodedModelFilePath members of @a initParams. Other parameters
 * applicable to UFF models also apply to TLT encoded models.
 */
extern "C"
bool NvDsInferCudaEngineGetFromTltModel(nvinfer1::IBuilder * const builder,
        const NvDsInferContextInitParams * const initParams,
        nvinfer1::DataType dataType,
        nvinfer1::ICudaEngine *& cudaEngine);

#endif

/** @} */
