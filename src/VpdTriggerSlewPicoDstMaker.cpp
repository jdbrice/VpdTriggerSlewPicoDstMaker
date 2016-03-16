#include "VpdTriggerSlewPicoDstMaker.h"



void VpdTriggerSlewPicoDstMaker::byteChecker(){

	

	book->cd( "byteChecker" );



	int run = pico->runNumber();

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
		
		int adcWest = pico->adc( "west", crate, tube );
		int tdcWest = pico->tdc( "west", crate, tube );

		int cTdcEast = corrEast( tube, adcEast, tdcEast );
		int cTdcWest = corrWest( tube, adcWest, tdcWest );

		if ( pulserCh( tube ) == tube ) {
			cTdcEast = tdcEast;
			cTdcWest = tdcWest;
		}
		
		if ( refCorr && pulserCh( tube ) != tube ){
			DEBUG( "", "[" << tube <<"]" << pulserMeanEast[ pulserCh( tube ) ] );

			cTdcEast = cTdcEast - (pico->tdc( "east", crate, pulserCh( tube ) ) - pulserMeanEast[ pulserCh( tube ) ] );
			cTdcWest = cTdcWest - (pico->tdc( "west", crate, pulserCh( tube ) ) - pulserMeanWest[ pulserCh( tube ) ] );	

		}

		if ( false == goodHitE( adcEast, cTdcEast ) )
			cTdcEast = 0;
		if ( false == goodHitW( adcWest, cTdcWest ) )
			cTdcWest = 0;

		data.tdcEast[ tube ] = cTdcEast;
		data.adcEast[ tube ] = adcEast;

		data.tdcWest[ tube ] = cTdcWest;
		data.adcWest[ tube ] = adcWest;

	} // tube loop


	//////////////////////////////////////////////////////////////////////
	/// Calculate the average
	/// 
	int nEast = 0, nWest = 0;
	int totalEast = 0, totalWest = 0;
	int totalADCEast = 0, totalADCWest = 0;

	for ( int i = 0; i < 16; i++ ){
		if ( pulserCh( i ) == i ) continue;
		
		if ( goodHitE( data.adcEast[ i ], data.tdcEast[ i ] ) ){
			nEast++;
			totalEast += data.tdcEast[ i ];
			totalADCEast += data.adcEast[ i ];
		}

		if ( goodHitW( data.adcWest[ i ], data.tdcWest[ i ] ) ){
			nWest++;
			totalWest += data.tdcWest[ i ];
			totalADCWest += data.adcWest[ i ];
		}
	}


	book->fill( "On_Off_nGoodHitWest", pico->nGood( "west" ), nWest );
	book->fill( "On_Off_nGoodHitEast", pico->nGood( "east" ), nEast );
	
	book->fill( "On_Off_sumTacWest", pico->sumTAC( "west" ), totalWest );
	book->fill( "On_Off_sumTacEast", pico->sumTAC( "east" ), totalEast );

	int ttotalADCWest = totalADCWest & 0xfff;
	if ( totalADCWest > 4095 )
		ttotalADCWest = 4095;
	int ttotalADCEast = totalADCEast & 0xfff;
	if ( totalADCEast > 4095 )
		ttotalADCEast = 4095;

	book->fill( "On_Off_sumAdcWest", pico->sumADC( "west" ), ttotalADCWest );
	book->fill( "On_Off_sumAdcEast", pico->sumADC( "east" ), ttotalADCEast );



	// cout << "total adcWest = " << totalADCWest << ", " << pico->sumADC( "west" )<< endl;

	book->fill( "int_sumTacEast", pico->sumTAC( "east" ) == totalEast );
	book->fill( "int_sumTacWest", pico->sumTAC( "west" ) == totalWest );

	book->fill( "int_sumAdcEast", pico->sumADC( "east" ) == totalADCEast );
	book->fill( "int_sumAdcWest", pico->sumADC( "west" ) == totalADCWest );

	book->fill( "int_nGoodEast", pico->nGood( "east" ) == nEast );
	book->fill( "int_nGoodWest", pico->nGood( "west" ) == nWest );

	// if ( pico->nGood( "east" ) != nEast ){
	// 	ERROR( classname(), "MISMATCH nEast BYTE" );
	// }
	// if ( pico->nGood( "west" ) != nWest ){
	// 	ERROR( classname(), "MISMATCH nWest BYTE" );
	// }
	// if ( 	pico->sumTAC( "west" ) != totalWest ||
	// 		pico->sumTAC( "east" ) != totalEast ){
	// 	ERROR( classname(), "MISMATCH sumTAC BYTE" );
	// }
	// if ( 	pico->sumADC( "west" ) != ttotalADCWest ||
	// 		pico->sumADC( "east" ) != ttotalADCEast ){
	// 	ERROR( classname(), "MISMATCH sumADC BYTE" );
	// 	ERROR( classname(),  "dWEST = " << pico->sumADC( "west" ) - ttotalADCWest );
	// 	ERROR( classname(),  "dEAST = " << pico->sumADC( "east" ) - ttotalADCEast );
	// }
}


void VpdTriggerSlewPicoDstMaker::megabyteChecker(){

	

	// book->cd( "megabyteChecker" );


	// this is the run# (filenames like "run17075040.1.dat.root")
	string dir = chain->GetFile()->GetName();
	dir = dir.substr(64, 11);

	book->cd( dir );
	if ( !book->exists( "On_Off_nGoodHitWest" ) ){

		book->clone( "", "On_Off_nGoodHitWest", dir, "On_Off_nGoodHitWest" );
		book->clone( "", "On_Off_nGoodHitEast", dir, "On_Off_nGoodHitEast" );
		
		book->clone( "", "On_Off_sumTacWest", dir, "On_Off_sumTacWest" );
		book->clone( "", "On_Off_sumTacEast", dir, "On_Off_sumTacEast" );

		book->clone( "", "On_Off_sumAdcWest", dir, "On_Off_sumAdcWest" );
		book->clone( "", "On_Off_sumAdcEast", dir, "On_Off_sumAdcEast" );

		book->clone( "", "int_sumTdcEast", dir, "int_sumTacEast" );
		book->clone( "", "int_sumTdcEast", dir, "int_sumTacWest" );
		book->clone( "", "int_sumTdcEast", dir, "int_sumAdcEast" );
		book->clone( "", "int_sumTdcEast", dir, "int_sumAdcWest" );
		book->clone( "", "int_sumTdcEast", dir, "int_nGoodEast" );
		book->clone( "", "int_sumTdcEast", dir, "int_nGoodWest" );	
		book->clone( "", "int_sumTdcEast", dir, "int_All" );
	}

	

	//////////////////////////////////////////////////////////////////////
	// Calculate the corrected values and fill tuple data
	// 
	DEBUG( classname(), "Crate = " << crate << ", refCorr = " << refCorr );
	for ( int tube = 0; tube < 16; tube++ ){
		
		int adcEast = pico->adc( "east", crate, tube );
		int tdcEast = pico->tdc( "east", crate, tube );
		
		int adcWest = pico->adc( "west", crate, tube );
		int tdcWest = pico->tdc( "west", crate, tube );

		int cTdcEast = corrEast( tube, adcEast, tdcEast );
		int cTdcWest = corrWest( tube, adcWest, tdcWest );

		if ( pulserCh( tube ) == tube ) {
			cTdcEast = tdcEast;
			cTdcWest = tdcWest;
		}
		
		if ( refCorr && pulserCh( tube ) != tube ){
			DEBUG( "", "[" << tube <<"]" << pulserMeanEast[ pulserCh( tube ) ] );

			cTdcEast = cTdcEast - (pico->tdc( "east", crate, pulserCh( tube ) ) - pulserMeanEast[ pulserCh( tube ) ] );
			cTdcWest = cTdcWest - (pico->tdc( "west", crate, pulserCh( tube ) ) - pulserMeanWest[ pulserCh( tube ) ] );	

		}

		if ( false == goodHitE( adcEast, cTdcEast ) )
			cTdcEast = 0;
		if ( false == goodHitW( adcWest, cTdcWest ) )
			cTdcWest = 0;

		data.tdcEast[ tube ] = cTdcEast;
		data.adcEast[ tube ] = adcEast;

		data.tdcWest[ tube ] = cTdcWest;
		data.adcWest[ tube ] = adcWest;

	} // tube loop


	//////////////////////////////////////////////////////////////////////
	/// Calculate the average
	/// 
	int nEast = 0, nWest = 0;
	int totalEast = 0, totalWest = 0;
	int totalADCEast = 0, totalADCWest = 0;

	for ( int i = 0; i < 16; i++ ){
		if ( pulserCh( i ) == i ) continue;
		
		if ( goodHitE( data.adcEast[ i ], data.tdcEast[ i ] ) ){
			nEast++;
			totalEast += data.tdcEast[ i ];
			totalADCEast += data.adcEast[ i ];
		}

		if ( goodHitW( data.adcWest[ i ], data.tdcWest[ i ] ) ){
			nWest++;
			totalWest += data.tdcWest[ i ];
			totalADCWest += data.adcWest[ i ];
		}
	}


	book->fill( "On_Off_nGoodHitWest", pico->nGood( "west" ), nWest );
	book->fill( "On_Off_nGoodHitEast", pico->nGood( "east" ), nEast );
	
	book->fill( "On_Off_sumTacWest", pico->sumTAC( "west" ), totalWest );
	book->fill( "On_Off_sumTacEast", pico->sumTAC( "east" ), totalEast );

	int ttotalADCWest = totalADCWest & 0xfff;
	if ( totalADCWest > 4095 )
		ttotalADCWest = 4095;
	int ttotalADCEast = totalADCEast & 0xfff;
	if ( totalADCEast > 4095 )
		ttotalADCEast = 4095;

	book->fill( "On_Off_sumAdcWest", pico->sumADC( "west" ), ttotalADCWest );
	book->fill( "On_Off_sumAdcEast", pico->sumADC( "east" ), ttotalADCEast );


	book->fill( "int_sumTacEast", pico->sumTAC( "east" ) == totalEast );
	book->fill( "int_sumTacWest", pico->sumTAC( "west" ) == totalWest );

	book->fill( "int_sumAdcEast", pico->sumADC( "east" ) == totalADCEast );
	book->fill( "int_sumAdcWest", pico->sumADC( "west" ) == totalADCWest );

	book->fill( "int_nGoodEast", pico->nGood( "east" ) == nEast );
	book->fill( "int_nGoodWest", pico->nGood( "west" ) == nWest );

	bool perfect = false;
	perfect = ( pico->sumTAC( "east" ) == totalEast && pico->sumTAC( "west" ) == totalWest && pico->sumADC( "east" ) == totalADCEast && pico->sumADC( "west" ) == totalADCWest && pico->nGood( "east" ) == nEast && pico->nGood( "west" ) == nWest );
	book->fill( "int_All", perfect );

}



void VpdTriggerSlewPicoDstMaker::fillVpdTrigger(){

	book->cd();

	int run = pico->runNumber();

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

		// APPLY DELTA TAC OFFSETS FOR TESTING
		// cTdcEast = cTdcEast - deltaTACEast[ tube ];
		// cTdcWest = cTdcWest - deltaTACWest[ tube ];
		

		// Save the non-jitter corrected values
		data.jadcEast[ tube ] = adcEast;
		if ( false == goodHitE( adcEast, cTdcEast ) )
			data.jtdcEast[ tube ] = 0;
		else 	
			data.jtdcEast[ tube ] = cTdcEast;
		
		data.jadcWest[ tube ] = adcWest;
		if ( false == goodHitW( adcWest, cTdcWest ) )
			data.jtdcWest[ tube ] = 0;
		else 	
			data.jtdcWest[ tube ] = cTdcWest;
		
		if ( refCorr && pulserCh( tube ) != tube ){
			DEBUG( "", "[" << tube <<"]" << pulserMeanEast[ pulserCh( tube ) ] );

			cTdcEast = cTdcEast - (pico->tdc( "east", crate, pulserCh( tube ) ) - pulserMeanEast[ pulserCh( tube ) ] );
			cTdcWest = cTdcWest - (pico->tdc( "west", crate, pulserCh( tube ) ) - pulserMeanWest[ pulserCh( tube ) ] );	

		}

		if ( false == goodHitE( adcEast, cTdcEast ) )
			cTdcEast = 0;
		if ( false == goodHitW( adcWest, cTdcWest ) )
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

	TRACE( classname(), "Averages");
	//////////////////////////////////////////////////////////////////////
	/// Calculate the average
	/// 
	int nEast = 0, nWest = 0;
	int totalEast = 0, totalWest = 0;
	int totalEast2 = 0, totalWest2 = 0, totalEast4 = 0, totalWest4 = 0, totalEast8 = 0, totalWest8 = 0;

	int jnEast = 0, jnWest = 0;
	int jtotalEast = 0, jtotalWest = 0;
	int jtotalAdcEast = 0, jtotalAdcWest = 0;


	for ( int i = 0; i < 16; i++ ){
		if ( 0 == i || 4 == i || 8 == i || 12 == i ) continue;
		
		if ( goodHitE( data.adcEast[ i ], data.tdcEast[ i ] ) ){
		//if ( data.tdcEast[ i ] > 300 && data.tdcEast[ i ] < 3000 && data.adcEast[ i ] > 10 ){
			nEast++;
			totalEast += data.tdcEast[ i ];
			if ( nEast == 2 )
				totalEast2 = totalEast;
			if ( nEast == 4 )
				totalEast4 = totalEast;
			if ( nEast == 8 )
				totalEast8 = totalEast; 
		}


		// if ( data.tdcWest[ i ] > 300 && data.tdcWest[ i ] < 3000 && data.adcWest[ i ] > 10 ){
		if ( goodHitW( data.adcWest[ i ], data.tdcWest[ i ] ) ){
			nWest++;
			totalWest += data.tdcWest[ i ];
			if ( nWest == 2 )
				totalWest2 = totalWest;
			if ( nWest == 4 )
				totalWest4 = totalWest;
			if ( nWest == 8 )
				totalWest8 = totalWest; 
		}

		//////////////////////////////////////////////////////////////////////
		/// with jitter versions
		if ( goodHitE( data.jadcEast[ i ], data.jtdcEast[ i ] ) ){
			jnEast++;
			jtotalEast += data.jtdcEast[ i ];
			jtotalAdcEast += data.jadcEast[ i ];
		}

		if ( goodHitW( data.jadcWest[ i ], data.jtdcWest[ i ] ) ){
			jnWest++;
			jtotalWest += data.jtdcWest[ i ];
			jtotalAdcWest += data.jadcWest[ i ];
		}
	}


	double avgEast = totalEast / (double) nEast;
	double avgWest = totalWest / (double) nWest;

	double javgEast = jtotalEast / (double) jnEast;
	double javgWest = jtotalWest / (double) jnWest;

	double l0Avg = ( avgWest - avgEast ) * toCM - pico->vZ();
	double jl0Avg = ( javgWest - javgEast ) * toCM - pico->vZ();

	TRACE( classname(), "Online vs. Offline");
	if ( jnEast == 0 || jnWest == 0 ){
		ERROR( classname(), "div0" );
		return;
	}

	bool JITTER_CORR = true;

	int offNWest = jnWest;
	int offNEast = jnEast;
	int offTotalEast = jtotalEast;
	int offTotalWest = jtotalWest;

	if ( JITTER_CORR ){
		offNEast = nEast;
		offNWest = nWest;
		offTotalEast = totalEast;
		offTotalWest = totalWest;
	}

	book->fill( "On_Off_nGoodHitWest", pico->nGood( "west" ), offNWest );
	book->fill( "On_Off_nGoodHitEast", pico->nGood( "east" ), offNEast );
	
	book->fill( "On_Off_sumTacWest", pico->sumTAC( "west" ), offTotalWest );
	book->fill( "On_Off_sumTacEast", pico->sumTAC( "east" ), offTotalEast );

	book->fill( "On_Off_sumAdcWest", pico->sumADC( "west" ), jtotalAdcWest );
	book->fill( "On_Off_sumAdcEast", pico->sumADC( "east" ), jtotalAdcEast );

	int tjtotalAdcWest = jtotalAdcWest & 0xfff;
	if ( jtotalAdcWest > 4095 )
		tjtotalAdcWest = 4095;
	int tjtotalAdcEast = jtotalAdcEast & 0xfff;
	if ( jtotalAdcEast > 4095 )
		tjtotalAdcEast = 4095;

	book->fill( "On_Off_tsumAdcWest", pico->sumADC( "west" ), tjtotalAdcWest );
	book->fill( "On_Off_tsumAdcEast", pico->sumADC( "east" ), tjtotalAdcEast );

	if ( tjtotalAdcEast != pico->sumADC( "east" ) ){
		book->fill( "On_Off_sumAdcEastWrong", pico->sumADC( "east" ), jtotalAdcEast );
	}

	book->fill( "On_tsumAdcEast", pico->sumADC( "east" ));

	book->fill( "On_Off_avgEast", (pico->sumTAC( "east" ) / (pico->nGood( "east" ))), jtotalEast / jnEast );
	book->fill( "On_Off_avgWest", (pico->sumTAC( "west" ) / (pico->nGood( "west" ))), jtotalWest / jnWest );

	if ( pico->nGood( "east" ) == jnEast )
		book->fill( "On_Off_avgEast_equalN", (pico->sumTAC( "east" ) / (pico->nGood( "east" ))), jtotalEast / jnEast );
	if ( pico->nGood( "west" ) == jnWest )
		book->fill( "On_Off_avgWest_equalN", (pico->sumTAC( "west" ) / (pico->nGood( "west" ))), jtotalWest / jnWest );

	book->fill( "l0On_Diff", (pico->sumTAC( "east" ) / (pico->nGood( "east" ))) - (pico->sumTAC( "west" ) / (pico->nGood( "west" ))) );
	book->fill( "l0Off_Diff",  (jtotalEast / jnEast) - (jtotalWest / jnWest) );


	book->fill( "sumTdcEastWrong", pico->sumTAC( "east" ) - jtotalEast );	
	if ( pico->sumTAC( "east" ) != totalEast ){
		book->fill( "int_sumTdcEast", 5 );
		// if ( 4 != (pico->sumTAC( "east" ) - jtotalEast) &&
		// 	128 != (pico->sumTAC( "east" ) - jtotalEast) &&
		// 	132 != (pico->sumTAC( "east" ) - jtotalEast) )
		// cout << (pico->sumTAC( "east" ) - jtotalEast) << endl;
		// cout << "SumTacOnline = " << pico->sumTAC( "east" ) << endl;
		// cout << "SumTacOffline = " << jtotalEast << endl; 
		book->fill( "sumTdcEastWrong2", pico->sumTAC( "east" ) - jtotalEast );	
	} else {
		book->fill( "int_sumTdcEast", 1 );
	}

	if ( pico->sumTAC( "west" ) != totalWest ){
		book->fill( "int_sumTdcWest", 5 );
	} else {
		book->fill( "int_sumTdcWest", 1 );
	}

	// 	




	TRACE( classname(), "pairwise");

	// book->fill( "l0Vertex", jl0Avg );
	
	book->fill( "deltaVz_Avg_wo_Jitter", l0Avg );
	book->fill( "deltaVz_Avg_w_Jitter", jl0Avg );

	book->fill( "deltaVz_grefmult_Avg_wo_Jitter", pico->gRefMult()/10.0, l0Avg );
	book->fill( "deltaVz_grefmult_Avg_w_Jitter", pico->gRefMult()/10.0, jl0Avg );

	book->fill( "deltaVz_nTubes_Avg_wo_Jitter", (nEast + nWest), l0Avg );


	book->fill( "Avg_wo_Jitter_grefmult_nTubes", pico->gRefMult()/10.0, (nEast + nWest) );

	book->fill( "East_Avg_wo_Jitter_grefmult_nTubes", pico->gRefMult()/10.0, nEast );
	book->fill( "West_Avg_wo_Jitter_grefmult_nTubes", pico->gRefMult()/10.0, nWest );


	string name = "deltaVz_nE2_nW2";
	book->fill( name, ( (totalWest2 / 2.0) - (totalEast2 / 2.0) ) * toCM - pico->vZ() );
	name = "deltaVz_nE4_nW4";
	book->fill( name, ( (totalWest4 / 4.0) - (totalEast4 / 4.0) ) * toCM - pico->vZ() );
	name = "deltaVz_nE8_nW8";
	book->fill( name, ( (totalWest8 / 8.0) - (totalEast8 / 8.0) ) * toCM - pico->vZ() );


	// for ( int i = 0; i < 16; i++ ){
	// 	if ( 0 == i || 4 == i || 8 == i || 12 == i ) continue;
	// 	if ( data.jtdcEast[ i ] > TAC_min && data.jtdcEast[ 1 ] > TAC_min )
	// 		book->fill( "deltaTAC_East", i, data.jtdcEast[ i ] - data.jtdcEast[ 1 ] );
		
	// 	if ( data.jtdcWest[ i ] > TAC_min && data.jtdcWest[ 2 ] > TAC_min )
	// 		book->fill( "deltaTAC_West", i, data.jtdcWest[ i ] - data.jtdcWest[ 2 ] );
	// }

	// book->fill( "onlineL0", jnEast * pico->fastTdc( "west" ) - jnWest * pico->fastTdc( "east" ) );



	book->fill( "avg_wo_Jitter_TAC_Vz", pico->vZ(), avgWest - avgEast );
	book->fill( "l0_TAC_Vz", pico->vZ(), (pico->sumTAC( "west" ) / (pico->nGood( "west" ))) - (pico->sumTAC( "east" ) / (pico->nGood( "east" )))  );




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
	tree->Branch("vpdVz",&data.vpdVz,"vpdVz/f");

	
}