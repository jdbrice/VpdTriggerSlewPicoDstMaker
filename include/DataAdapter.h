#ifndef DATA_ADAPTER_H
#define DATA_ADAPTER_H

#include <string>
using namespace std;

class DataAdapter
{
public:

	TTree          *fChain;

	int iPrePost;
	DataAdapter() {}
	~DataAdapter() {}


	virtual int tdc( string side = "east", string crate="bbq", int tube = 0 ){
		if ( "east" == side || "East" == side || "EAST" == side ){
			if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){

			} else {

			}
		}
		if ( "west" == side || "West" == side || "WEST" == side ){
			if ( "bbq" == crate || "Bbq" == crate || "BBQ" == crate ){

			} else {

			}
		}
		return 0;
	}

	virtual int sumTAC( string side = "east" ){

		if ( "east" == side || "East" == side || "EAST" == side ){
			
		}
		if ( "west" == side || "West" == side || "WEST" == side ){
			
		}
		return 0;
	}
	virtual int sumADC( string side = "east" ){
		if ( "east" == side || "East" == side || "EAST" == side ){
			
		}
		if ( "west" == side || "West" == side || "WEST" == side ){
			
		}
		return 0;
	}

	virtual int nGood( string side = "east" ) {
		if ( "west" == side || "West" == side || "WEST" == side ){

		} else if ( "east" == side || "East" == side || "EAST" == side ){

		}
		return 0;
	}

	virtual int adc( string side = "east", string crate="bbq", int tube = 0 ) { return 0; }

	virtual int fastTdc( string side = "east", string crate="bbq" ) { return 0; }

	virtual Float_t vpdVz() {
		return 0;
	}

	virtual int gRefMult(){
		return 0;
	}

	virtual Float_t vX() {
		return 0;
	}
	virtual Float_t vY() {
		return 0;
	}
	virtual Float_t vZ() {
		return 0;
	}

	virtual Int_t runNumber(){
		return 0;
	}
	virtual Int_t eventNumber(){
		return 0;
	}


	virtual Int_t trigger(){
		return 0;
	}

	virtual void setiPrePost( int _iPrePost ){
		iPrePost = _iPrePost;
	}


	
};


#endif