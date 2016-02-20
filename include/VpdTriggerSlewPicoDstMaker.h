#ifndef VPD_TRIGGER_SLEW_PICO_DST_MAKER_H
#define VPD_TRIGGER_SLEW_PICO_DST_MAKER_H

// STL
#include <memory>

// ROOT
#include "TTree.h"
#include "TFile.h"


#include "TreeAnalyzer.h"
using namespace jdb;

#include "VpdTriggerSlewPicoDstData.h"
#include "TrgEvent.h"
#include "MuDst.h"
#include "VpdPico.h"


class VpdTriggerSlewPicoDstMaker : public TreeAnalyzer
{
protected:
	
	shared_ptr<DataAdapter> pico;
	VpdTriggerSlewPicoDstData data;

	TTree * tree = nullptr;
	TFile * outFile = nullptr;

	int eBinEdges[19][10];
	int eCorrs[19][10];

	int wBinEdges[19][10];
	int wCorrs[19][10];

	int numBins;

	vector<int> channelMask;
	vector<int> tacOffsetEast, tacOffsetWest;

	vector<int> pulserMeanEast, pulserMeanWest;

	string crate;

	bool wantVertex = false;
	bool refCorr = false;



public:
	virtual const char* classname() const { return "VpdTriggerSlewPicoDstMaker"; }

	VpdTriggerSlewPicoDstMaker() {

	}
	~VpdTriggerSlewPicoDstMaker() {

	}

	virtual void initialize(){
	

		if ( "trgEvent" == config.getString( nodePath + ".input.dst:treeName" ) )
			pico = shared_ptr<TrgEvent>( new TrgEvent( chain ) );
		else if ( "MuDst" == config.getString( nodePath + ".input.dst:treeName" ) ){
			pico = shared_ptr<MuDst>( new MuDst( chain ) );

			chain->SetBranchStatus( "*", 0 );
			chain->SetBranchStatus( "MuEvent.mEventSummary.mPrimaryVertexPos*", 1 );
			chain->SetBranchStatus( "MuEvent.mVpdTriggerDetector*", 1 );
			wantVertex = true;
		}
		else if ( "vpdPico" == config.getString( nodePath + ".input.dst:treeName" ) ){
			pico = shared_ptr<VpdPico>( new VpdPico( chain ) );
			wantVertex = true;
		} else {
			ERROR( classname(), "COULD not make a DataAdapter" );
		}

		string filename = config[ nodePath + ".output.tree" ];
		outFile = new TFile(filename.c_str(), "RECREATE");
		bookTree();

		if ( config.exists( nodePath + ".Slewing" ) )
			readParams();

		channelMask = config.getIntVector( nodePath + ".ChannelMask", 1, 16 );
		tacOffsetEast = config.getIntVector( nodePath + ".TacOffsetEast", 0, 16 );
		tacOffsetWest = config.getIntVector( nodePath + ".TacOffsetWest", 0, 16 );

		pulserMeanEast = config.getIntVector( nodePath + ".PulserMeanEast", 0, 16 );
		pulserMeanWest = config.getIntVector( nodePath + ".PulserMeanWest", 0, 16 );

		crate = config.getString( nodePath + ".Crate" );
		INFO( classname(), "Crate == \"" << crate << "\""  );

		refCorr = config.getBool( nodePath + ".Reference:corr", false );

	}

	virtual void postMake(){
		DEBUG( classname(), "Saving Output" );
		outFile->Write();
		outFile->Close();
	}


	virtual bool keepEvent( ){
		return true;
	}

	virtual void analyzeEvent() {
		
		// DEBUG( classname(), "" );
		outFile->cd();


		data.run = pico->runNumber();
		data.evt = pico->eventNumber(); 

		data.vertexZ = pico->vZ();

		if ( fabs(data.vertexZ - pico->vpdVz()) > 3.0 ) return;
		double px = pico->vX();
		double py = pico->vY();

		if ( sqrt( px*px + py*py ) > 1.0 ) return;


		if ( wantVertex && data.vertexZ == 0.0 ) return;

		fillVpdTrigger();

		tree->Fill();

	}

	virtual int pulserCh( int tube ) {
		return tube / 4 * 4;
	}


	

	void fillVpdTrigger();

	void bookTree();
	void readParams();
	int corrEast( int iCh, int adc, int tac );
	int corrWest( int iCh, int adc, int tac );
	int corrBin( int iCh, int adc, int *bins );

};




#endif