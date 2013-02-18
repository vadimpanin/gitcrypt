// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define USE_ZLIB_DIRECTLY 1

#include "Types.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <io.h>
#endif

#include <fcntl.h>

#include <vector>
#include <string>
#include <iostream>

#include "Filter/ZlibFilter.h"
#include "Filter/AesFilter.h"
#include "Filter/Base64Filter.h"
#include "Filter/EndlineFilter.h"

#include "CmdLine.h"

#include <boost/foreach.hpp>

#include "log.h"
#include "Exception.h"
#include "Input.h"
#include "Output.h"
#include "Config.h"

// We need some recognizable header to recognize encrypted files.
// I didn't dare to use XML for actual file body (due to performance concerns),
// but it's perfect for the header.
static const std::string MagicBegin = "<gitcrypt_header>\n";
static const std::string HeaderText = "<version>0.01</version>\n";
static const std::string MagicEnd = "</gitcrypt_header>\n";
static const size_t MaxBlock = 5120;
static const char EndLine = 0x0A;

#include "Encoder.h"
#include "Decoder.h"
#include "Initializer.h"

template< class Processor >
static void processFile( const Config& config )
{
	typedef typename Processor::Line Line;

	if( config.key.empty() )
		throw std::runtime_error( "key is not specified" );

	if( config.in.empty() )
		throw std::runtime_error( "input is not specified" );

	if( config.out.empty() )
		throw std::runtime_error( "output is not specified" );
	
	ReversibleFiltersChain< Line > chain;
	chain.push_back( typename ReversibleFilter< Line >::Ptr( new ZlibFilter< Line >() ) );
	chain.push_back( typename ReversibleFilter< Line >::Ptr( new AesFilter< Line >( config.key ) ) );
	chain.push_back( typename ReversibleFilter< Line >::Ptr( new Base64Filter< Line >( ) ) );
	chain.push_back( typename ReversibleFilter< Line >::Ptr( new EndlineFilter< Line >( ) ) );

	typename Input::Ptr in;
	if( config.in == "-" )
		in.reset( new FileInput( "-" ) );
	else
		in.reset( new FileInput( config.in.c_str() ) );

	typename Output::Ptr out;
	if( config.out == "-" )
		out.reset( new FileOutput( "-" ) );
	else
		out.reset( new FileOutput( config.out.c_str() ) );

	Processor processor;
	processor.process( in.get(), out.get(), chain );
}

void usage()
{
	std::cerr << "Usage: gitcrypt <action> [options]\n"
	"To init repository in trusted location:\n"
	"\tgitcrypt init --key <key>\n"
	"To encrypt or decrypt file manually:\n"
	"\tgitcrypt encrypt|decrypt -in <filename> --out <filename> --key <key>\n"
	"\tUse \'-\' to specify stdin/stdout as input/ouput.\n"
	"Key can be up to 32 characters length.\n";
}

int main( int argc, char* argv[] )
{
	typedef std::string Line;

#ifdef WIN32
	_setmode( _fileno(stdout), O_BINARY );
	_setmode( _fileno(stdin), O_BINARY );
#endif

	Config config;

	try
	{
		readConfig( argc, argv, config );
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		usage();
		return 1;
	}

	try
	{
		switch( config.mode )
		{
			case Config::Decrypt:
				processFile< Decoder< Line > >( config );
				break;

			case Config::Encrypt:
				processFile< Encoder< Line > >( config );
				break;

			case Config::Init:
				{
					Initializer ir( config );
					ir();

					std::cout << "Repository has been initialized.\n"
						"If the repository is not empty, run `git reset --hard` in order to decrypt the files.\n";
				}
				break;

			case Config::Help:
				usage();
				break;

			default:
				THROW_RUNTIME( "Invalid mode" );
		}
	}
	catch( std::exception& e)
	{
		std::cerr << e.what() << "\n";
		fflush( stderr );

		_LOG( "exception: " << e.what() << "\n" );

		return 1;
	}

	fflush( stdout );

	return 0;
}

