/*
 * Copyright (c) 2017-2020, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 *
 */

/**
 * @file nvdsinfer_custom_impl.h
 * <b>Defines specification for Custom Method Implementations for custom models </b>
 *
 * @b Description: This file defines the API that
 * implements custom methods required by the GStreamer Gst-nvinfer plugin to
 * infer using custom models.
 *
 * All custom functionality must be implemented in an independent shared
 * library. The library is dynamically loaded (using %dlopen()) by the plugin.
 * It implements custom methods which are called as required. The custom library
 * can be specified in the Gst-nvinfer configuration file by the
 * `custom-lib-name` property.
 *
 * @section customparsingfunc Custom Detector Output Parsing Function
 *
 * This section describes the custom bounding box parsing function for custom
 * detector models.
 *
 * The custom parsing function should be of the type `NvDsInferParseCustomFunc`.
 * The custom parsing function can be specified in the Gst-nvinfer
 * configuration file by the properties `parse-bbox-func-name`
 * (name of the parsing function) and `custom-lib-name`. `parse-func must be
 * set to 0.
 *
 * The Gst-nvinfer plugin loads the library and looks for the custom parsing
 * function symbol. The function is called after each inference call is
 * executed.
 *
 * You can call the macro CHECK_CUSTOM_PARSE_FUNC_PROTOTYPE() after
 * defining the function to validate the function definition.
 *
 *
 * @section iplugininterface TensorRT Plugin Factory interface for DeepStream
 *
 * Based on the type of the model (Caffe or UFF), the library
 * must implement one of the functions NvDsInferPluginFactoryCaffeGet() or
 * NvDsInferPluginFactoryUffGet().
 * During model parsing, "nvinfer" looks for either of these functions'
 * symbols in the custom library based on the model framework. If either
 * symbol is found, the plugin calls that function to get a pointer to
 * the PluginFactory instance required for parsing.
 *
 * If the IPluginFactory is needed during deserialization of CUDA engines,
 * the library must implement %NvDsInferPluginFactoryRuntimeGet().
 *
 * Each Get function has a corresponding Destroy function which is
 * called, if defined, when the returned PluginFactory is to be destroyed.
 *
 * A library that implements this interface must use the same function names
 * as the header file. Gst-nvinfer dynamically loads the library and
 * looks for the same symbol names.
 *
 * See the FasterRCNN sample provided with the SDK for a sample implementation
 * of the interface.
 *
 *
 * @section inputlayerinitialization Input layer initialization
 *
 * By default, Gst-nvinfer works with networks having only one input layer
 * for video frames. If a network has more than one input layer, the custom
 * library can implement the @ref NvDsInferInitializeInputLayers interface
 * for initializing the other input layers. Gst-nvinfer assumes that the other
 * input layers have static input information, and hence this method is called
 * only once before the first inference.
 *
 * See the FasterRCNN sample provided with the SDK for a sample implementation
 * of the interface.
 *
 *
 * @section customnetworkbuild Interface for building Custom Networks
 *
 * The "nvinfer" plugin supports two interfaces for to create and build
 * custom networks not directly supported by nvinfer.
 * - IModelParser / NvDsInferCreateModelParser interface
 * - NvDsInferEngineCreateCustomFunc interface
 *
 * In case of IModelParser / NvDsInferCreateModelParser interface, the custom
 * library must derive and implement IModelParser, an interface to parse
 * the custom networks and build the TensorRT network (nvinfer1::INetworkDefinition).
 * The "nvinfer" plugin will then use this TensorRT network to build the
 * inference engine. The plugin will look for the symbol "NvDsInferCreateModelParser"
 * in the library and call the function to get an instance of the model parser
 * implementation from the library.
 *
 * Alternatively, you can use the custom engine creation function to build
 * networks that are not natively supported by nvinfer. The function must be
 * of the type  @ref NvDsInferEngineCreateCustomFunc. You can specify it
 * in the nvinfer element configuration file using
 * the property `engine-create-func-name` (name of the engine creation function)
 * in addition to `custom-lib-name`.
 *
 * The nvinfer plugin loads the custom library dynamically and looks for the
 * engine creation symbol. The function is called only once during
 * initialization of the nvinfer plugin. The function must build and return
 * the `CudaEngine` interface using the supplied nvinfer1::IBuilder instance.
 * The builder instance is already configured with properties like
 * MaxBatchSize, MaxWorkspaceSize, INT8/FP16 precision parameters, etc.
 * The builder instance is managed by nvinfer, and the function may not destroy
 * it.
 *
 * You can call the macro CHECK_CUSTOM_ENGINE_CREATE_FUNC_PROTOTYPE() after
 * the function definition to validate the function definition.
 *
 * Refer to the Yolo sample provided with the SDK for sample implementation of
 * both the interfaces.
 */

#ifndef _NVDSINFER_CUSTOM_IMPL_H_
#define _NVDSINFER_CUSTOM_IMPL_H_

#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "NvCaffeParser.h"
#include "NvUffParser.h"
#pragma GCC diagnostic pop

#include "nvdsinfer.h"

/*
 * C++ interfaces
 */
#ifdef __cplusplus

/**
 * A model parser interface to translate user-defined model to a TensorRT network.
 *
 * Users can parse any custom model derived from this inferface. Instance would
 * be created by a call to @fn NvDsInferCreateModelParser.
 *
 * Implementations should make sure that all member functions are overriden.
 * This parser will be deleted after the engine (nvinfer1::ICudaEngine) is built.
 */
class IModelParser
{
public:
    IModelParser() = default;
    /**
     * Destructor, make sure all external resource would be released here. */
    virtual ~IModelParser() = default;

    /**
     * Function interface for parsing custom model and building tensorrt
     * network.
     *
     * @param[in, out] network NvDsInfer will create the @a network and
     *                 implementation can setup this network layer by layer.
     * @return NvDsInferStatus indicating if model parsing was sucessful.
     */
    virtual NvDsInferStatus parseModel(
        nvinfer1::INetworkDefinition& network) = 0;

    /**
     * Function interface to check if parser can support full-dimensions.
     */
    virtual bool hasFullDimsSupported() const = 0;

    /**
     * Function interface to get the new model name which is to be used for
     * constructing the serialized engine file path.
     */
    virtual const char* getModelName() const = 0;
};
#endif

/*
 * C interfaces
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Holds the detection parameters required for parsing objects.
 */
typedef struct
{
  /** Holds the number of classes requested to be parsed, starting with
   class ID 0. Parsing functions may only output objects with
   class ID less than this value. */
  unsigned int numClassesConfigured;
  /** Holds a per-class vector of detection confidence thresholds
   to be applied prior to clustering operation.
   Parsing functions may only output an object with detection confidence
   greater than or equal to the vector element indexed by the object's
   class ID. */
  std::vector<float> perClassPreclusterThreshold;
  /* Per class threshold to be applied post clustering operation */
  std::vector<float> perClassPostclusterThreshold;

  /** Deprecated. Use perClassPreclusterThreshold instead. Reference to
   * maintain backward compatibility. */
  std::vector<float> &perClassThreshold = perClassPreclusterThreshold;
} NvDsInferParseDetectionParams;

/**
 * Type definition for the custom bounding box parsing function.
 *
 * @param[in]  outputLayersInfo A vector containing information on the output
 *                              layers of the model.
 * @param[in]  networkInfo      Network information.
 * @param[in]  detectionParams  Detection parameters required for parsing
 *                              objects.
 * @param[out] objectList       A reference to a vector in which the function
 *                              is to add parsed objects.
 */
typedef bool (* NvDsInferParseCustomFunc) (
        std::vector<NvDsInferLayerInfo> const &outputLayersInfo,
        NvDsInferNetworkInfo  const &networkInfo,
        NvDsInferParseDetectionParams const &detectionParams,
        std::vector<NvDsInferObjectDetectionInfo> &objectList);

/**
 * Validates a custom parser function definition. Must be called
 * after defining the function.
 */
#define CHECK_CUSTOM_PARSE_FUNC_PROTOTYPE(customParseFunc) \
    static void checkFunc_ ## customParseFunc (NvDsInferParseCustomFunc func = customParseFunc) \
        { checkFunc_ ## customParseFunc (); }; \
    extern "C" bool customParseFunc (std::vector<NvDsInferLayerInfo> const &outputLayersInfo, \
           NvDsInferNetworkInfo  const &networkInfo, \
           NvDsInferParseDetectionParams const &detectionParams, \
           std::vector<NvDsInferObjectDetectionInfo> &objectList);

/**
 * Type definition for the custom bounding box and instance mask parsing function.
 *
 * @param[in]  outputLayersInfo A vector containing information on the output
 *                              layers of the model.
 * @param[in]  networkInfo      Network information.
 * @param[in]  detectionParams  Detection parameters required for parsing
 *                              objects.
 * @param[out] objectList       A reference to a vector in which the function
 *                              is to add parsed objects and instance mask.
 */
typedef bool (* NvDsInferInstanceMaskParseCustomFunc) (
        std::vector<NvDsInferLayerInfo> const &outputLayersInfo,
        NvDsInferNetworkInfo  const &networkInfo,
        NvDsInferParseDetectionParams const &detectionParams,
        std::vector<NvDsInferInstanceMaskInfo> &objectList);

/**
 * Validates a custom parser function definition. Must be called
 * after defining the function.
 */
#define CHECK_CUSTOM_INSTANCE_MASK_PARSE_FUNC_PROTOTYPE(customParseFunc) \
    static void checkFunc_ ## customParseFunc (NvDsInferInstanceMaskParseCustomFunc func = customParseFunc) \
        { checkFunc_ ## customParseFunc (); }; \
    extern "C" bool customParseFunc (std::vector<NvDsInferLayerInfo> const &outputLayersInfo, \
           NvDsInferNetworkInfo  const &networkInfo, \
           NvDsInferParseDetectionParams const &detectionParams, \
           std::vector<NvDsInferInstanceMaskInfo> &objectList);

/**
 * Type definition for the custom classifier output parsing function.
 *
 * @param[in]  outputLayersInfo  A vector containing information on the
 *                               output layers of the model.
 * @param[in]  networkInfo       Network information.
 * @param[in]  classifierThreshold
                                 Classification confidence threshold.
 * @param[out] attrList          A reference to a vector in which the function
 *                               is to add the parsed attributes.
 * @param[out] descString        A reference to a string object in which the
 *                               function may place a description string.
 */
typedef bool (* NvDsInferClassiferParseCustomFunc) (
        std::vector<NvDsInferLayerInfo> const &outputLayersInfo,
        NvDsInferNetworkInfo  const &networkInfo,
        float classifierThreshold,
        std::vector<NvDsInferAttribute> &attrList,
        std::string &descString);

/**
 * Validates the classifier custom parser function definition. Must be called
 * after defining the function.
 */
#define CHECK_CUSTOM_CLASSIFIER_PARSE_FUNC_PROTOTYPE(customParseFunc) \
    static void checkFunc_ ## customParseFunc (NvDsInferClassiferParseCustomFunc func = customParseFunc) \
        { checkFunc_ ## customParseFunc (); }; \
    extern "C" bool customParseFunc (std::vector<NvDsInferLayerInfo> const &outputLayersInfo, \
           NvDsInferNetworkInfo  const &networkInfo, \
           float classifierThreshold, \
           std::vector<NvDsInferAttribute> &attrList, \
           std::string &descString);

typedef struct _NvDsInferContextInitParams NvDsInferContextInitParams;

/**
 * Type definition for functions that build and return a @c CudaEngine for
 * custom models.
 *
 * @deprecated  The NvDsInferCudaEngineGet interface is replaced by
 * NvDsInferEngineCreateCustomFunc().
 *
 * The implementation of this interface must build the
 * nvinfer1::ICudaEngine instance using nvinfer1::IBuilder instance
 * @a builder. The builder instance is managed by the caller;
 * the implementation must not destroy it.
 *
 * Properties like @a MaxBatchSize, @a MaxWorkspaceSize, INT8/FP16
 * precision parameters, and DLA parameters (if applicable) are set on the
 * builder before it is passed to the interface. The corresponding Get
 * functions of the nvinfer1::IBuilder interface can be used to get
 * the property values.
 *
 * The implementation must make sure not to reduce the @a MaxBatchSize of the
 * returned @c CudaEngine.
 *
 * @param[in]  builder      An nvinfer1::IBuilder instance.
 * @param[in]  initParams   A pointer to the structure to be used for
 *                          initializing the NvDsInferContext instance.
 * @param[in]  dataType     Data precision.
 * @param[out] cudaEngine   A pointer to a location where the function is to
 *                          store a reference to the nvinfer1::ICudaEngine
 *                          instance it has built.
 * @return  True if the engine build was successful, or false otherwise. TBD Shaunak asked to have the original "deprecated" description restored. That would be redundant; there's a @deprecated command near the top of the comment.
 */
typedef bool (* NvDsInferEngineCreateCustomFunc) (
        nvinfer1::IBuilder * const builder,
        const NvDsInferContextInitParams * const initParams,
        nvinfer1::DataType dataType,
        nvinfer1::ICudaEngine *& cudaEngine);

/**
 * A macro that validates a custom engine creator function definition.
 * Call this macro after the function is defined.
*/
#define CHECK_CUSTOM_ENGINE_CREATE_FUNC_PROTOTYPE(customEngineCreateFunc) \
    static void checkFunc_ ## customEngineCreateFunc (NvDsInferEngineCreateCustomFunc = customEngineCreateFunc) \
        { checkFunc_ ## customEngineCreateFunc(); }; \
    extern "C" bool customEngineCreateFunc (  \
        nvinfer1::IBuilder * const builder,  \
        const NvDsInferContextInitParams const *initParams, \
        nvinfer1::DataType dataType, \
        nvinfer1::ICudaEngine *& cudaEngine);

/**
 * Specifies the type of the Plugin Factory.
 */
typedef enum
{
  /** Specifies nvcaffeparser1::IPluginFactory or
   nvuffparser::IPluginFactory. */
  PLUGIN_FACTORY,
  /** Specifies nvcaffeparser1::IPluginFactoryExt or
   nvuffparser::IPluginFactoryExt. */
  PLUGIN_FACTORY_EXT,
  /** Specifies nvcaffeparser1::IPluginFactoryV2. Used only for Caffe models. */
  PLUGIN_FACTORY_V2
} NvDsInferPluginFactoryType;

/**
 * Holds a pointer to a heap-allocated Plugin Factory object required during
 * Caffe model parsing.
 */
typedef union
{
  nvcaffeparser1::IPluginFactory *pluginFactory;
  nvcaffeparser1::IPluginFactoryExt *pluginFactoryExt;
  nvcaffeparser1::IPluginFactoryV2 *pluginFactoryV2;
} NvDsInferPluginFactoryCaffe;

/**
 * Holds a  pointer to a heap-allocated Plugin Factory object required during
 * UFF model parsing.
 */
typedef union
{
  nvuffparser::IPluginFactory *pluginFactory;
  nvuffparser::IPluginFactoryExt *pluginFactoryExt;
} NvDsInferPluginFactoryUff;

/**
 * Gets a new instance of a Plugin Factory interface to be used
 * during parsing of Caffe models. The function must set the correct @a type and
 * the correct field in the @a pluginFactory union, based on the type of the
 * Plugin Factory, (i.e. one of @a pluginFactory, @a pluginFactoryExt, or
 * @a pluginFactoryV2).
 *
 * @param[out] pluginFactory    A reference to the union that contains
 *                              a pointer to the Plugin Factory object.
 * @param[out] type             Specifies the type of @a pluginFactory, i.e.
 *                              which member the @a pluginFactory union
 *                              is valid.
 * @return  True if the Plugin Factory was created successfully, or false
 *  otherwise.
 */
bool NvDsInferPluginFactoryCaffeGet (NvDsInferPluginFactoryCaffe &pluginFactory,
    NvDsInferPluginFactoryType &type);

/**
 * Destroys a Plugin Factory instance created by
 * NvDsInferPluginFactoryCaffeGet().
 *
 * @param[in] pluginFactory A reference to the union that contains a
 *                          pointer to the Plugin Factory instance returned
 *                          by NvDsInferPluginFactoryCaffeGet().
 */
void NvDsInferPluginFactoryCaffeDestroy (NvDsInferPluginFactoryCaffe &pluginFactory);

/**
 * Returns a new instance of a Plugin Factory interface to be used
 * during parsing of UFF models. The function must set the correct @a type and
 * the correct field in the @a pluginFactory union, based on the type of the
 * Plugin Factory (i.e. @a pluginFactory or @a pluginFactoryExt).
 *
 * @param[out] pluginFactory    A reference to a union that contains a pointer
 *                              to the Plugin Factory object.
 * @param[out] type             Specifies the type of @a pluginFactory, i.e.
 *                              which member of the @a pluginFactory union
 *                              is valid.
 * @return  True if the Plugin Factory was created successfully, or false
 *  otherwise.
 */
bool NvDsInferPluginFactoryUffGet (NvDsInferPluginFactoryUff &pluginFactory,
    NvDsInferPluginFactoryType &type);

/**
 * Destroys a Plugin Factory instance created by NvDsInferPluginFactoryUffGet().
 *
 * @param[in] pluginFactory     A reference to the union that contains a
 *                              pointer to the Plugin Factory instance returned
 *                              by NvDsInferPluginFactoryUffGet().
 */
void NvDsInferPluginFactoryUffDestroy (NvDsInferPluginFactoryUff &pluginFactory);

/**
 * Returns a new instance of a Plugin Factory interface to be used
 * during parsing deserialization of CUDA engines.
 *
 * @param[out] pluginFactory    A reference to nvinfer1::IPluginFactory*
 *                              in which the function is to place a pointer to
 *                              the instance.
 * @return  True if the Plugin Factory was created successfully, or false
 *  otherwise.
 */
bool NvDsInferPluginFactoryRuntimeGet (nvinfer1::IPluginFactory *& pluginFactory);

/**
 * Destroys a Plugin Factory instance created by
 * NvDsInferPluginFactoryRuntimeGet().
 *
 * @param[in] pluginFactory     A pointer to the Plugin Factory instance
 *                              returned by NvDsInferPluginFactoryRuntimeGet().
 */
void NvDsInferPluginFactoryRuntimeDestroy (nvinfer1::IPluginFactory * pluginFactory);

/**
 * Initializes the input layers for inference. This function is called only once
 * during before the first inference call.
 *
 * @param[in] inputLayersInfo   A reference to a vector containing information
 *                              on the input layers of the model. This does not
 *                              contain the NvDsInferLayerInfo structure for
 *                              the layer for video frame input.
 * @param[in] networkInfo       A reference to anetwork information structure.
 * @param[in] maxBatchSize      The maximum batch size for inference.
 *                              The input layer buffers are allocated
 *                              for this batch size.
 * @return  True if input layers are initialized successfully, or false
 *  otherwise.
 */
bool NvDsInferInitializeInputLayers (std::vector<NvDsInferLayerInfo> const &inputLayersInfo,
        NvDsInferNetworkInfo const &networkInfo,
        unsigned int maxBatchSize);
/**
 * The NvDsInferCudaEngineGet interface has been deprecated and has been
 * replaced by NvDsInferEngineCreateCustomFunc function.
 */
bool NvDsInferCudaEngineGet(nvinfer1::IBuilder *builder,
        NvDsInferContextInitParams *initParams,
        nvinfer1::DataType dataType,
        nvinfer1::ICudaEngine *& cudaEngine)
        __attribute__((deprecated("Use 'engine-create-func-name' config parameter instead")));

/**
 * Create a customized neural network parser for user-defined models.
 *
 * User need to implement a new IModelParser class with @a initParams
 * referring to any model path and/or customNetworkConfigFilePath.
 *
 * @param[in] initParams with model paths or config files.
 * @return Instance of IModelParser implementation.
 */
IModelParser* NvDsInferCreateModelParser(
    const NvDsInferContextInitParams* initParams);

#ifdef __cplusplus
}
#endif

#endif

