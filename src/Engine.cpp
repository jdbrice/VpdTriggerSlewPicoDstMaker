

#include "XmlConfig.h"
#include "Engine.h"
using namespace jdb;

#include <iostream>
#include <exception>

#include "VpdTriggerSlewPicoDstMaker.h"

int main( int argc, char* argv[] ) {

	TaskFactory::registerTaskRunner<VpdTriggerSlewPicoDstMaker>( "VpdTriggerSlewPicoDstMaker" );
	Engine engine( argc, argv );

	return 0;
}
