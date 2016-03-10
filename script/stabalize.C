

map< string, map<int, int> > pulserMeans;
map< string, map<int, int> > pulserJumpIndex;
map< string, map<int, int> > newPulserMeans;
int threshold = 50;




long long int meanForJump( int jumpMean, map<int, int> &mp ){
	INFO( "jumpMean = " << jumpMean );

	long long int total = 0;
	int n = 0;
	for ( auto kv : mp ){
		
		if( abs(kv.second - jumpMean ) < threshold  ){
			total += kv.second;
			n++;
		}
	}

	cout << "total = " << total << endl;
	cout << "n = " << n << endl;
	return (int) (total / (double) n);

}




void stabalize() {
	
	XmlConfig config( "bbq_map.xml" );
	Logger::setGlobalLogLevel( "info" );

	



	for ( string sd : { "East", "West" } ){
		for ( int ch : { 0, 4, 8, 12 } ){
			string name = sd + "_" +ts(ch);
			map<int, int> temp = config.getIntMap( "Pulser_" + name );
			pulserMeans[ name ] = temp;
		}
	}


	vector<int> iJumps;
	
	for ( string sd : { "East", "West" } ){
		for ( int ch : { 0, 4, 8, 12 } ){
			string name = sd + "_" +ts(ch);

			
			int total = 0;
			int nSet = 0;
			int cMean = 0;

			INFO("");
			for ( auto kv : pulserMeans[ name ] ){
				// INFO( "Hello " << kv.first );
				bool newJump = true;
				for ( int iJ : iJumps ){
					if( abs(kv.second - pulserMeans[ name ][ iJ ] ) < threshold  ){
						newJump = false;
						pulserJumpIndex[ name ][ kv.first ] = iJ;
						break;
					}
				}

				if ( newJump ){
					iJumps.push_back( kv.first );
					pulserJumpIndex[ name ][ kv.first ] = kv.first;
					INFO( "New Jump @ " << kv.first  );
				}
			}
		}
	}


	for ( string sd : { "East", "West" } ){
		for ( int ch : { 0, 4, 8, 12 } ){
			string name = sd + "_" +ts(ch);

			for (  int iJ : iJumps  ){

				int m = meanForJump( pulserMeans[ name ][iJ], pulserMeans[ name ] );

				INFO( name );
				INFO( "Mean for [" << iJ << " ]" << m );

				newPulserMeans[ name ][ iJ ] = m;

			}

		}
	}



	for ( string sd : { "East", "West" } ){
		for ( int ch : { 0, 4, 8, 12 } ){
			string name = sd + "_" +ts(ch);

			cout << "<Pulser_" + name << ">" << endl;
			for ( auto kv : pulserMeans[ name ] ){

				cout << kv.first << " :: " << newPulserMeans[ name ][ pulserJumpIndex[name][ kv.first ] ] << "," << endl;

			}
			cout << "</Pulser_" + name << ">" << endl << endl;
		}
	}






}