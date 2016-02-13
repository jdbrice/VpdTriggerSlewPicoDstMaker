#include "VpdTriggerSlewPicoDstMaker.h"



void VpdTriggerSlewPicoDstMaker::fillVpdTrigger(){

	book->cd();

	int fastTdcEast = 0;
	int fastAdcEast = 0;
	int fastChEast = -1;
	int fastTdcWest = 0;
	int fastAdcWest = 0;
	int fastChWest = -1;


	for ( int tube = 0; tube < 16; tube++ ){

		int adcEast = pico->adc( "east", "bbq", tube );
		int tdcEast = pico->tdc( "east", "bbq", tube );

		int adcWest = pico->adc( "west", "bbq", tube );
		int tdcWest = pico->tdc( "west", "bbq", tube );

		int cTdcEast = corrEast( tube, adcEast, tdcEast );
		int cTdcWest = corrWest( tube, adcWest, tdcWest );

		if ( cTdcEast > 4096 )
			cTdcEast = 0;
		if ( cTdcWest > 4096 )
			cTdcWest = 0;

		if ( cTdcEast < 300 )
			cTdcEast = 0;
		if ( cTdcWest < 300 )
			cTdcWest = 0;

		cTdcEast = cTdcEast - tacOffsetEast[ tube ];
		cTdcWest = cTdcWest - tacOffsetWest[ tube ];

		data.tdcEast[ tube ] = cTdcEast;
		data.adcEast[ tube ] = adcEast;

		data.tdcWest[ tube ] = cTdcWest;
		data.adcWest[ tube ] = adcWest;


		if ( channelMask[ tube ] ){
			book->fill( "tdcEast", tube, tdcEast );
			book->fill( "tdcWest", tube, tdcWest );

			book->fill( "tdcEastSlewCorred", tube, cTdcEast );
			book->fill( "tdcWestSlewCorred", tube, cTdcWest );	


			if ( cTdcEast > fastTdcEast ){
				fastTdcEast = cTdcEast;
				fastAdcEast = adcEast;
				fastChEast = tube;
			}

			if ( cTdcWest > fastTdcWest ){
				fastTdcWest = cTdcWest;
				fastAdcWest = adcWest;
				fastChWest = tube;
			}
		} // if channelMask
	} // tube loop


	data.fastTdcEast = fastTdcEast;
	data.fastAdcEast = fastAdcEast;
	data.fastChEast = fastChEast;
	data.fastPulserChEast = pulserMap[ fastChEast ];

	data.fastTdcWest = fastTdcWest;
	data.fastAdcWest = fastAdcWest;
	data.fastChWest = fastChWest;
	data.fastPulserChWest = pulserMap[ fastChWest ];
} // fillVpdTrigger




void VpdTriggerSlewPicoDstMaker::readParams(){
	ifstream inf( config.getString( nodePath + ".Slewing:url" ).c_str() );

	for ( int i = 0; i < 19; i ++ ){
		for ( int j = 0; j < 10; j++){
			eBinEdges[ i ][ j ] = 0;
			wBinEdges[ i ][ j ] = 0;
			eCorrs[ i ][ j ] = 0;
			wCorrs[ i ][ j ] = 0;
		}
	}


	if ( !inf.good() ){
		ERROR( "", "BAD SLEWING" );
		return;
	}

	int iEast = 0;
	int iWest = 0;

	string line;
	while( getline( inf, line ) ){

		cout << line << endl;
		stringstream ss( line );

		string boardId;

		ss >> boardId;

		if ( "0x16" == boardId || "0x18" == boardId ){

			int channel;
			int nBins;
			int opt;

			ss >> channel >> nBins >> opt;
			cout << "nBins " << nBins << endl;

			numBins = nBins;
			
			if ( 0 == opt ) {// bin edges
				for ( int i = 0; i < nBins; i++ ){
					int be;
					ss >> be;
					cout << "[" << i << "] = " << be << endl;
					if ( "0x16" == boardId ) // east
						eBinEdges[iEast][i] = be;
					else if ("0x18" == boardId) // west
						wBinEdges[iWest][i] = be;
				}
			} else if ( 1 == opt ) {// bin corrs
				for ( int i = 0; i < nBins; i++ ){
					int bc;
					ss >> bc;
					cout << "[" << i << "] = " << bc << endl;
					if ( "0x16" == boardId ) // east
						eCorrs[iEast][i] = bc;
					else if ("0x18" == boardId) // west
						wCorrs[iWest][i] = bc;
				}
			}
			if ( "0x16" == boardId && 1 == opt ) // east
				iEast ++;
			else if ("0x18" == boardId && 1 == opt ) // west
				iWest ++;	
		}

	}

	inf.close();
}

int VpdTriggerSlewPicoDstMaker::corrEast( int iCh, int adc, int tac ){
	int bin = corrBin( iCh, adc, eBinEdges[ iCh ] );
	return tac + eCorrs[ iCh ][ bin ];
}
int VpdTriggerSlewPicoDstMaker::corrWest( int iCh, int adc, int tac ){
	int bin = corrBin( iCh, adc, wBinEdges[ iCh ] );
	return tac + wCorrs[ iCh ][ bin ];
}

int VpdTriggerSlewPicoDstMaker::corrBin( int iCh, int adc, int *bins ){
	
	// find the bin
	for ( int iBin = 0; iBin < numBins; iBin++){

		if ( 0 == iBin && adc >= 0 && adc <= bins[ 0 ] ){
			return 0;
		} else {
			if ( adc > bins[ iBin - 1 ] && adc <= bins[ iBin ] )
				return iBin;
		}
	}
	return -1;
}


void VpdTriggerSlewPicoDstMaker::bookTree(){

	outFile->cd();
	string crate = config.getString( nodePath + ".Crate" );
	tree = new TTree( crate.c_str(), "VpdTriggerSlewPicoDst data" );
	tree->SetAutoSave( 1000 );
	
	tree->Branch("adcEast",&data.adcEast,"adcEast[16]/i");
	tree->Branch("adcWest",&data.adcWest,"adcWest[16]/i");
	tree->Branch("tdcEast",&data.tdcEast,"tdcEast[16]/i");
	tree->Branch("tdcWest",&data.tdcWest,"tdcWest[16]/i");

	tree->Branch("fastTdcEast",&data.fastTdcEast,"fastTdcEast/i");
	tree->Branch("fastTdcWest",&data.fastTdcWest,"fastTdcWest/i");
	tree->Branch("fastAdcEast",&data.fastAdcEast,"fastAdcEast/i");
	tree->Branch("fastAdcWest",&data.fastAdcWest,"fastAdcWest/i");

	tree->Branch("fastChEast",&data.fastChEast,"fastChEast/i");
	tree->Branch("fastChWest",&data.fastChWest,"fastChWest/i");

	tree->Branch("fastPulserChEast",&data.fastPulserChEast,"fastPulserChEast/i");
	tree->Branch("fastPulserChWest",&data.fastPulserChWest,"fastPulserChWest/i");

	tree->Branch("vertexX",&data.vertexX,"vertexX/f");
	tree->Branch("vertexY",&data.vertexY,"vertexY/f");
	tree->Branch("vertexZ",&data.vertexZ,"vertexZ/f");

	
}