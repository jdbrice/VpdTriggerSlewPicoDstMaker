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
	vector<int> deltaTACEast, deltaTACWest;

	string crate;

	bool wantVertex = false;
	bool refCorr = false;


	map< string, map<int, int> > pulserMeans;

	int E_TAC_min, E_TAC_max, E_ADC_th;
	int W_TAC_min, W_TAC_max, W_ADC_th;



public:
	virtual const char* classname() const { return "VpdTriggerSlewPicoDstMaker"; }

	VpdTriggerSlewPicoDstMaker() {

	}
	~VpdTriggerSlewPicoDstMaker() {

	}

	virtual void initialize(){
	

		if ( "trgEvent" == config.getString( nodePath + ".input.dst:treeName" ) ){
			pico = shared_ptr<TrgEvent>( new TrgEvent( chain ) );

			INFO( classname(), "iPrePost == " << config.getInt( nodePath + ".iPrePost", 0 ) );
			pico->setiPrePost( config.getInt( nodePath + ".iPrePost", 0 ) );
		}
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

		deltaTACEast = config.getIntVector( nodePath + ".DeltaTACEast", 0, 16 );
		deltaTACWest = config.getIntVector( nodePath + ".DeltaTACWest", 0, 16 );

		crate = config.getString( nodePath + ".Crate" );
		INFO( classname(), "Crate == \"" << crate << "\""  );

		refCorr = config.getBool( nodePath + ".Reference:corr", false );


		for ( string sd : { "East", "West" } ){
			for ( int ch : { 0, 4, 8, 12 } ){
				string name = sd + "_" +ts(ch);
				map<int, int> temp = config.getIntMap( "Pulser_" + name );
				pulserMeans[ name ] = temp;
			}
		}

		// Load in the TAC and ADC thresholds
		E_TAC_min = config.getInt( nodePath + ".GoodHitE:TAC_min", 180 );
		E_TAC_max = config.getInt( nodePath + ".GoodHitE:TAC_max", 3000 );
		E_ADC_th = config.getInt( nodePath + ".GoodHitE:ADC_th", 10 );

		W_TAC_min = config.getInt( nodePath + ".GoodHitW:TAC_min", 180 );
		W_TAC_max = config.getInt( nodePath + ".GoodHitW:TAC_max", 3000 );
		W_ADC_th = config.getInt( nodePath + ".GoodHitW:ADC_th", 10 );


		INFO( classname(), "Thresholds EAST TAC_min=" << E_TAC_min <<", TAC_max=" << E_TAC_max << ", ADC_th=" << E_ADC_th );
		INFO( classname(), "Thresholds WEST TAC_min=" << W_TAC_min <<", TAC_max=" << W_TAC_max << ", ADC_th=" << W_ADC_th );


		// test case
		INFO( classname(), "Mean East 0[ 17047044 ] = " << pulserMeans[ "East_0" ][ 17047044 ]  );

	}

	virtual void preEventLoop(){
		TreeAnalyzer::preEventLoop();

		for ( int nE : { 2, 4, 8 } ){
			for ( int nW : { 2, 4, 8 } ){
				string name = "deltaVz_nE" + ts( nE ) + "_nW" + ts(nW);
				book->clone( "deltaVz_Avg_wo_Jitter", name );		
			}
		}
		

	}

	virtual void postMake(){
		DEBUG( classname(), "Saving Output" );
		outFile->Write();
		outFile->Close();
	}


	virtual bool keepEvent( ){


		// if ( pico->trigger() & ( 1 << 1 ) )
		// 	return true;
		// else 
		// 	return false;

		return true;
	}

	virtual void analyzeEvent() {
		
		// DEBUG( classname(), "" );
		outFile->cd();


		data.run = pico->runNumber();
		data.evt = pico->eventNumber(); 

		data.vertexZ = pico->vZ();
		data.vpdVz = pico->vpdVz();


		if ( fabs(data.vertexZ - pico->vpdVz()) > 3.0 ) return;
		double px = pico->vX();
		double py = pico->vY();

		if ( sqrt( px*px + py*py ) > 1.0 ) return;

		book->fill( "events", 1 );
		if ( wantVertex && data.vertexZ == 0.0 ) return;

		fillVpdTrigger();

		tree->Fill();

	}

	virtual int pulserCh( int tube ) {
		return tube / 4 * 4;
	}


	bool goodHitW( int adc, int tac ){
		if ( adc <= W_ADC_th || tac >= W_TAC_max || tac < W_TAC_min)
			return false;
		return true;
	}
	bool goodHitE( int adc, int tac ){
		if ( adc <= E_ADC_th || tac >= E_TAC_max || tac < E_TAC_min)
			return false;
		return true;
	}

	

	void fillVpdTrigger();

	void bookTree();
	void readParams();
	int corrEast( int iCh, int adc, int tac );
	int corrWest( int iCh, int adc, int tac );
	int corrBin( int iCh, int adc, int *bins );

};




#endif