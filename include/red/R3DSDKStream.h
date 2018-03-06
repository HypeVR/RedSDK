/* R3D SDK library version 7.0 header file. Do *NOT* use this
   header file with any other version of the R3D SDK library!
   
   This header file and everything else included with the R3D
   SDK is Copyright (c) 2008-2017 RED Digital Cinema. All
   rights reserved. Redistribution of this header is prohibited!
   
   The SDK is thread-safe for the most part, but it may
   synchronize access when needed.
*/

#ifndef R3DSDKSTREAM_H
#define R3DSDKSTREAM_H

namespace R3DSDK {

enum VerifyStatus
{
    /**
     * Packet is verified.  OK to pass on to WritePacketData(...)
     */
    VS_OK = 0,

    /**
     * Packet should be dropped.  Do not pass to WritePacketData(...)
     */
    VS_DROP = 1,

    /**
     * Missing callback to send an ACK.  Do not pass to WritePacketData(...)
     */
    VS_MISSING_CALLBACK = 2,

    /**
     * Error communicating with dynamic library.
     */
	VS_INTERNAL_ERROR = 3,

    /**
     * This (version of the) SDK cannot handle this stream format.
     */
	VS_UNSUPPORTED_STREAM_FORMAT = 4
};

// Class that will help in filtering the incoming RCP stream from the camera
// for tethering. 
class CameraStream
{
public:
	/**
	* Callback method for notifying the API user that an ACK packet must be
	* sent to the camera.
	*
	* ackData will be destroyed when this function returns.  Send the data
	* to the camera immediately.
	*/
	typedef void (*AckCallback)(void *userData, const unsigned char *ackData, unsigned int len);

	CameraStream(AckCallback ackCallback, void *ackUserData);
	~CameraStream();

	// call from network thread with packet received, try to do nothing
	// else in this thread to make sure no packets are dropped! If return
	// status is VS_OK pass the packet to an instance of R3DStream::WritePacketData()
	VerifyStatus ProcessRdpPacket(const void * packet, size_t packetLength);

private:
	void * reserved;
};

// class to stream the incoming data to an R3D clip. A new instance should be created
// for each individual clip so the state can be properly tracked and clips correctly closed.
class R3DStream
{
public:
	// create new instance, 
	R3DStream(const char * outputPath, unsigned int cameraReelId, unsigned int cameraClipId);
	~R3DStream();

	// call from processing thread if ProcessRdpPacket() returned true
	// returns CSFrameAdded if a block is complete, otherwise returns CSDone or error code
	CreateStatus WritePacketData(const void * packet, size_t packetLength, Metadata * metadataToFill = NULL);
	CreateStatus WritePacketData(const void * packet, size_t packetLength, bool & isDroppedFrame, Metadata * metadataToFill = NULL);

private:
	void * reserved;
};

#ifdef RCP_TYPES_PUBLIC_H
// This class can be used to update the UI state on the camera while streaming
// data over the tether. Camera should be told if the stream is being saved or
// not and the reel & clip ID that the clip is being saved under.
class CameraUIState
{
public:
	/**
	 * Function pointer to the rcp_set_int() function.
	 *
	 * User can pass us this function pointer to allow us to set clip number, record indicator color, etc.
	 */
	typedef rcp_error_t (*RcpSetInt)(rcp_camera_connection_t *con, rcp_param_t id, int val);
	
	/**
	 * Function pointer to the rcp_set_uint() function
	 */
	typedef rcp_error_t (*RcpSetUInt)(rcp_camera_connection_t *con, rcp_param_t id, unsigned int val);

	CameraUIState(RcpSetInt rcpsi, RcpSetUInt rcpsui, rcp_camera_connection_t *con);
	~CameraUIState();

	// set camera UI record state
	// returns RCP_ERROR_BAD_PARAM if no rcp_set_uint() function pointer or camera connection was set
	rcp_error_t SetUIRecordState(bool showRecordState) const;

	// let camera know the total space the volume being written to
	rcp_error_t SetTotalStorageSpace(unsigned int megabytesTotalStorage) const;

	// let camera know how much space out of the total storage space is in use
	rcp_error_t SetStorageSpaceLeft(unsigned int megabytesStorageLeft) const;

	// set UI reel ID
	// reelId must be in range 1 - 999
	rcp_error_t SetReelId(unsigned int reelId) const;

	// set UI clip ID
	// clipId must be in range 1 - 999
	rcp_error_t SetClipId(unsigned int clipId) const;
	
private:
	void * reserved;
};
#endif

}
#endif

