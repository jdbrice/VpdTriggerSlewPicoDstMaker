#define vpdPico_cxx

#include "vpdPico.h"


int getTdc( vpdPico * p, string crate = "bbq", string side = "east", int ch = 0 ){


	if ( "bbq" == crate ){
		if ( "east" == side )
			return p->bbqTdcEast[ ch ];
		if ( "west" == side )
			return p->bbqTdcWest[ ch ];
	}
	if ( "mxq" == crate ){
		if ( "east" == side )
			return p->mxqTdcEast[ ch ];
		if ( "west" == side )
			return p->mxqTdcWest[ ch ];
	}
	return 0;

}



void makeMeanMapFor( string crate = "bbq", string side = "east", int ch = 0) {
	using namespace jdb;
	

	Logger::setGlobalLogLevel( "info" );
	TChain * c = new TChain( "vpdPico" );
	ChainLoader::loadList( c, "../bin/filelist.lis" );

	// DataSource * ds = new DataSource( "vpdPico", c );
	vpdPico * pico = new vpdPico( c );


	HistoBook book( ( "pulserMean_" + crate + "_" + side + "_" + ts( ch ) + ".root" ) );


	int N = c->GetEntries();


	map<int, int> binMap;
	int cBin = 1;

	TH2D * pulser = new TH2D( "pulser", "", 250, 0, 250, 4096, 1, 4096 );
	TH2D * pulserOrder = new TH2D( "pulserOrder", "", 250, 0, 250, 4096, 1, 4096 );
	TH2C * orderedBinMap = new TH2C( "orderedBinMap", "", 250, 0, 250, 1000, 17041000, 17052000 );
	TH2D * pulserRun = new TH2D( "pulserRun", "", 11000, 17041000, 17052000, 410, 1, 4096 );

	for ( int i = 0; i < N; i++ ){
		c->GetEntry( i );

		int run = pico->runId;
		if ( binMap.count( run ) <= 0 ){
			binMap[ run ] = cBin;
			TRACE( "Adding Run# " << run << " at bin index " << cBin );
			cBin++;
		}

		int tdc = getTdc( pico, crate, side, ch );
		if ( tdc < 300 || tdc > 3000 ) continue;
		pulser->Fill( binMap[ run ], tdc );

	}

	int binOrdered = 1;
	for ( auto kv : binMap ) {

		int bin = kv.second+1;
		int run = kv.first;
		int runBin = run - 17041000;

		orderedBinMap->Fill( binOrdered, run );

		INFO( "run # " << run << ", binOrdered = " << binOrdered << ", bin = " << bin );
		for ( int i = 1; i < 4096; i++ ){
			int binVal = pulser->GetBinContent( bin, i );
			pulserOrder->SetBinContent( binOrdered, i, binVal );

			pulserRun->SetBinContent( runBin, i/10, binVal );
		}

		binOrdered ++;

	}


	// finally make a map of run# to pulser mean
	pulserOrder->FitSlicesY(  );
	TH1D* means = (TH1D*)gDirectory->Get( "pulserOrder_1" );

	ofstream fout( ( "pulserMean_" + crate + "_" + side + "_" + ts( ch ) + ".txt" ).c_str() );
	fout << "<Pulser_" << side << "_" << ch << ">" << endl; 
	binOrdered = 1;
	string sep = "";
	for ( auto kv : binMap ) {
		int run = kv.first;
		int mean = means->GetBinContent( binOrdered );
		binOrdered++;

		fout << sep << endl << run << " :: " << mean;
		sep = ",";
	}

	fout << "</Pulser_" << side << "_" << ch << ">" << endl;

	fout.close();




	book.save();

}


void makeMeanMap(  ) {

	vector<string> crates = { "bbq", "mxq" };
	vector<string> sides = { "east", "west" };
	vector<int> chs = { 0, 4, 8, 12 };

	for ( auto c : crates ){
		for ( auto s : sides ){
			for ( int ch : chs ){
				makeMeanMapFor( c, s, ch );
			}
		}
	}
	
}