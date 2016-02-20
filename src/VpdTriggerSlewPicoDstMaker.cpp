#include "VpdTriggerSlewPicoDstMaker.h"



void VpdTriggerSlewPicoDstMaker::fillVpdTrigger(){

	book->cd();

	double toCM = 0.262;
	if ( "bbq" == crate )
		toCM = 0.262;

	//////////////////////////////////////////////////////////////////////
	// Calculate the corrected values and fill tuple data
	// 
	DEBUG( classname(), "Crate = " << crate << ", refCorr = " << refCorr );
	for ( int tube = 0; tube < 16; tube++ ){
		
		int adcEast = pico->adc( "east", crate, tube );
		int tdcEast = pico->tdc( "east", crate, tube );
		DEBUG( classname(), "tdcEast = " << tdcEast );
		int adcWest = pico->adc( "west", crate, tube );
		int tdcWest = pico->tdc( "west", crate, tube );


		int cTdcEast = corrEast( tube, adcEast, tdcEast );
		int cTdcWest = corrWest( tube, adcWest, tdcWest );

		DEBUG( classname(), "ctdcEast = " << cTdcEast );

		if ( pulserCh( tube ) == tube ) {
			cTdcEast = tdcEast;
			cTdcWest = tdcWest;
		}

		if ( adcEast < 10 )
			cTdcEast = 0;
		if ( adcWest < 10 )
			cTdcWest = 0;

		if ( cTdcEast > 3000 )
			cTdcEast = 0;
		if ( cTdcWest > 3000 )
			cTdcWest = 0;

		if ( cTdcEast < 300 )
			cTdcEast = 0;
		if ( cTdcWest < 300 )
			cTdcWest = 0;

		data.jtdcEast[ tube ] = cTdcEast;
		data.jadcEast[ tube ] = adcEast;

		data.jtdcWest[ tube ] = cTdcWest;
		data.jadcWest[ tube ] = adcWest;


		if ( refCorr && pulserCh( tube ) != tube ){
			DEBUG( "", "[" << tube <<"]" << pulserMeanEast[ pulserCh( tube ) ] );
			cTdcEast = cTdcEast - (pico->tdc( "east", crate, pulserCh( tube ) ) - pulserMeanEast[ pulserCh( tube ) ] );
			cTdcWest = cTdcWest - (pico->tdc( "west", crate, pulserCh( tube ) ) - pulserMeanWest[ pulserCh( tube ) ] );	
		}

		if ( adcEast < 10 )
			cTdcEast = 0;
		if ( adcWest < 10 )
			cTdcWest = 0;
		

		if ( cTdcEast > 3000 )
			cTdcEast = 0;
		if ( cTdcWest > 3000 )
			cTdcWest = 0;

		if ( cTdcEast < 300 )
			cTdcEast = 0;
		if ( cTdcWest < 300 )
			cTdcWest = 0;

		DEBUG( classname(), "ctdcEast = " << cTdcEast );

		data.tdcEast[ tube ] = cTdcEast;
		data.adcEast[ tube ] = adcEast;

		data.tdcWest[ tube ] = cTdcWest;
		data.adcWest[ tube ] = adcWest;

	} // tube loop


	//////////////////////////////////////////////////////////////////////
	/// Find Fastest
	/// 
	int fastTdcEast = -1;
	int jfastTdcEast = -1;
	int fastTdcWest = -1;
	int jfastTdcWest = -1;
	for ( int i = 0; i < 16; i++ ){
		if ( 0 == i || 4 == i || 8 == i || 12 == i ) continue;
		
		if ( data.tdcEast[ i ] > fastTdcEast )
			fastTdcEast = data.tdcEast[ i ];
		if ( data.tdcWest[ i ] > fastTdcWest )
			fastTdcWest = data.tdcWest[ i ];

		if ( data.jtdcEast[ i ] > jfastTdcEast )
			jfastTdcEast = data.jtdcEast[ i ];
		if ( data.jtdcWest[ i ] > jfastTdcWest )
			jfastTdcWest = data.jtdcWest[ i ];
	}

	double l0Normal = ( jfastTdcWest - jfastTdcEast ) * toCM - pico->vZ();
	double l0 = ( fastTdcWest - fastTdcEast ) * toCM - pico->vZ();
	
	book->fill( "deltaVz_Early_w_Jitter", l0Normal );
	book->fill( "deltaVz_Early_wo_Jitter", l0 );

	book->fill( "deltaVz_grefmult_Early_w_Jitter", pico->gRefMult()/10.0, l0Normal );
	book->fill( "deltaVz_grefmult_Early_wo_Jitter", pico->gRefMult()/10.0, l0 );


	//////////////////////////////////////////////////////////////////////
	/// Calculate the average
	/// 
	int nEast = 0, nWest = 0;
	int totalEast = 0, totalWest = 0;

	int jnEast = 0, jnWest = 0;
	int jtotalEast = 0, jtotalWest = 0;


	for ( int i = 0; i < 16; i++ ){
		if ( 0 == i || 4 == i || 8 == i || 12 == i ) continue;
		
		if ( data.tdcEast[ i ] > 300 && data.tdcEast[ i ] < 3000 && data.adcEast[ i ] > 10 ){
			nEast++;
			totalEast += data.tdcEast[ i ];
		}

		if ( data.tdcWest[ i ] > 300 && data.tdcWest[ i ] < 3000 && data.adcWest[ i ] > 10 ){
			nWest++;
			totalWest += data.tdcWest[ i ];
		}

		//////////////////////////////////////////////////////////////////////
		/// with jitter versions
		if ( data.jtdcEast[ i ] > 300 && data.jtdcEast[ i ] < 3000 && data.adcEast[ i ] > 10 ){
			jnEast++;
			jtotalEast += data.jtdcEast[ i ];
		}

		if ( data.jtdcWest[ i ] > 300 && data.jtdcWest[ i ] < 3000 && data.adcWest[ i ] > 10 ){
			jnWest++;
			jtotalWest += data.jtdcWest[ i ];
		}
	}

	double avgEast = totalEast / (double) nEast;
	double avgWest = totalWest / (double) nWest;

	double javgEast = jtotalEast / (double) jnEast;
	double javgWest = jtotalWest / (double) jnWest;

	double l0Avg = ( avgWest - avgEast ) * toCM - pico->vZ();
	double jl0Avg = ( javgWest - javgEast ) * toCM - pico->vZ();
	
	book->fill( "deltaVz_Avg_wo_Jitter", l0Avg );
	book->fill( "deltaVz_Avg_w_Jitter", jl0Avg );

	book->fill( "deltaVz_grefmult_Avg_wo_Jitter", pico->gRefMult()/10.0, l0Avg );
	book->fill( "deltaVz_grefmult_Avg_w_Jitter", pico->gRefMult()/10.0, jl0Avg );




} // fillVpdTrigger




void VpdTriggerSlewPicoDstMaker::readParams(){
	ifstream inf( config.getString( nodePath + ".Slewing:url" ).c_str() );
	DEBUG( classname(), "Reading Slewing Params : " << config.getString( nodePath + ".Slewing:url" ) );

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

		DEBUG( classname(), line );
		stringstream ss( line );

		string boardId;

		ss >> boardId;

		if ( "0x16" == boardId || "0x18" == boardId ){

			int channel;
			int nBins;
			int opt;

			ss >> channel >> nBins >> opt;
			DEBUG( classname(), "nBins " << nBins );

			numBins = nBins;
			
			if ( 0 == opt ) {// bin edges
				for ( int i = 0; i < nBins; i++ ){
					int be;
					ss >> be;
					DEBUG( classname(), "[" << i << "] = " << be );
					if ( "0x16" == boardId ) // east
						eBinEdges[iEast][i] = be;
					else if ("0x18" == boardId) // west
						wBinEdges[iWest][i] = be;
				}
			} else if ( 1 == opt ) {// bin corrs
				for ( int i = 0; i < nBins; i++ ){
					int bc;
					ss >> bc;
					DEBUG( classname(), "[" << i << "] = " << bc );
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
	
	tree->Branch("run",&data.run,"run/i");
	tree->Branch("evt",&data.evt,"evt/i");
	
	tree->Branch("adcEast",&data.adcEast,"adcEast[16]/i");
	tree->Branch("adcWest",&data.adcWest,"adcWest[16]/i");

	tree->Branch("tdcEast",&data.tdcEast,"tdcEast[16]/i");
	tree->Branch("tdcWest",&data.tdcWest,"tdcWest[16]/i");


	tree->Branch("jadcEast",&data.jadcEast,"jadcEast[16]/i");
	tree->Branch("jadcWest",&data.jadcWest,"jadcWest[16]/i");

	tree->Branch("jtdcEast",&data.jtdcEast,"jtdcEast[16]/i");
	tree->Branch("jtdcWest",&data.jtdcWest,"jtdcWest[16]/i");


	tree->Branch("vertexX",&data.vertexX,"vertexX/f");
	tree->Branch("vertexY",&data.vertexY,"vertexY/f");
	tree->Branch("vertexZ",&data.vertexZ,"vertexZ/f");

	
}