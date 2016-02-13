#ifndef TOF_PICO_DST_DATA_H
#define TOF_PICO_DST_DATA_H



const Int_t kMaxChannels = 16;


struct VpdTriggerSlewPicoDstData {

	Int_t  run, evt;

	// VPD trigger side bbq electronics
	Int_t   adcEast[kMaxChannels], 
			tdcEast[kMaxChannels],
			fastAdcEast, 
			fastChEast, 
			fastTdcEast,
			fastPulserChEast;


	Int_t   adcWest[kMaxChannels], 
			tdcWest[kMaxChannels],
			fastAdcWest, 
			fastChWest, 
			fastTdcWest,
			fastPulserChWest;

	Float_t vertexX, vertexY, vertexZ;

};

#endif