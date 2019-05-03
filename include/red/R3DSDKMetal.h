/* R3D SDK library version 7.1 header file. Do *NOT* use this
   header file with any other version of the R3D SDK library!
   
   This header file and everything else included with the R3D
   SDK is Copyright (c) 2008-2019 RED Digital Cinema. All
   rights reserved. Redistribution of this header is prohibited!
*/

#ifndef R3DSDKMETAL_H
#define R3DSDKMETAL_H

#include <Metal/Metal.h>
#include "R3DSDK.h"

namespace R3DSDK
{


class DebayerMetalJob
{
private:
    unsigned int reserved1;
    unsigned int reserved2;

    DebayerMetalJob();
    ~DebayerMetalJob();
public:
    //returns the size in bytes of the result frame that the debayer job passed in will produce.
    static size_t ResultFrameSize( const DebayerMetalJob &debayerJob );

    //This will ensure all objects used for the frame are disposed of.
    //This call will block until the debayer on the queue executes, if the debayer has already executed no blocking will occur
    //This function must be called at a point after passing the DebayerMetalJob into REDMetal::processAsync
    //This will automatically be called upon destruction the DebayerMetalJob if it is not manually called earlier
    void completeAsync();
    
    void *raw_host_mem; //sized from r3dsdk buffer
    id<MTLBuffer> raw_device_mem;  //sized from r3dsdk buffer

    size_t output_device_mem_size; //size of the output buffer
                                      //it must be a minimum of ResultFrameSize(job)

    id<MTLBuffer> output_device_mem; //result/output buffer on the GPU
                              //    object of the size output_device_mem_size

							  
    R3DSDK::VideoDecodeMode mode;

	//Image Processing Settings. This cannot be NULL.
    R3DSDK::ImageProcessingSettings *imageProcessingSettings;

    //Supported Types are:
    //  R3DSDK::PixelType_16Bit_RGB_Interleaved 
    //  R3DSDK::PixelType_HalfFloat_RGB_Interleaved
    //  R3DSDK::PixelType_HalfFloat_RGB_ACES_Int
    R3DSDK::VideoPixelType pixelType;

private:
    //Reserved pointer do not alter
    void *reserved;
};

class REDMetal
{
public:
    enum Status
    {
        Status_Ok = 0,
        Status_ErrorProcessing = 1,
        Status_InvalidJobParameter = 2,
        
        //mode value passed in is not compatible with this SDK or the mode used with the DecodeForGpuSdk call is not compatible
        Status_InvalidJobParameter_mode = 3,//mode is not compatible with this SDK or the mode used with the DecodeForGpuSdk call
        
        //pointer is NULL, data is not from DecodeForGpuSdk, R3DSDK and GPUSDK versions are incompatible or buffer is not actually in host memory.
        Status_InvalidJobParameter_raw_host_mem = 4,
        Status_InvalidJobParameter_raw_device_mem = 5,
        
        //unsupported pixel type
        Status_InvalidJobParameter_pixelType = 6,
        
        //Output buffer Size must be non zero. As of GPUSDK Version 6.0.0 we no longer support creating the output buffer in the SDK.
        //Output buffer must be allocated prior to passing it into the sdk
        Status_InvalidJobParameter_output_device_mem_size = 7,
        Status_InvalidJobParameter_output_device_mem = 8,
        
        //Image processing settings ColorVersion was set to ColorVersion1 which is not supported by this SDK
        Status_InvalidJobParameter_ColorVersion1 = 9,
        
        //GPU Device did not meet minimum requirements.
        Status_UnableToUseGPUDevice = 10,
        
        //Error loading R3DSDK dynamic library
        Status_UnableToLoadLibrary = 11,
		
        Status_ParameterUnsupported = 12,
		
		Status_InvalidAPIObject = 13
    };


	DebayerMetalJob *createDebayerJob();

    void releaseDebayerJob(DebayerMetalJob *job);

    REDMetal();
    ~REDMetal();

    //checks to see if a context and command queue are compatible with REDMetal
    //This call may take several seconds on slower cards
    Status checkCompatibility(id<MTLCommandQueue> queue, int &error);
    
    //processes a frame.
    //Thread safe blocking call
    //This function will enqueue the Metal commands and wait for the processing to complete
    //  to ensure memory buffers are not released prior to completion of kernel
    Status process(id<MTLCommandQueue> queue, DebayerMetalJob *debayerJob, int &error);

    //enqueues all red processing on the current command queue
    //once processAsync has been called you will need to call debayerJob->completeAsync
    //      This will wait for commands enqueued by this sdk for the current frame to complete and then releases any additional resources.
    //      If the commands have already completed there is no wait involved
    Status processAsync(id<MTLCommandQueue> queue, DebayerMetalJob *debayerJob, int &error);

private:
    void *reserved;
};
}

#endif

