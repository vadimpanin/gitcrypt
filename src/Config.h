// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

struct Config
{
	enum EMode {
		Invalid = 0,
		Encrypt,
		Decrypt,
		Init,
		Help
	} mode;

	std::string key;
	std::string in;
	std::string out;

	Config()
		: mode( Invalid )
		, in( "-" )
		, out( "-" )
	{}
};

void readConfig( int argc, char* argv[], Config& config )
{
	_LOG( "called new instance\n" );
	for( int i = 1; i < argc; i++ )
		_LOG( "param " << i << " = \"" << argv[i] << "\"\n" );

	std::string mode;
	std::string key;
	std::string in = config.in;
	std::string out = config.out;

	for( int i = 1; i < argc; i++ )
	{
		if( cmdline::readStatic( i, argv, argc, 1, mode ) ||
			cmdline::read( i, argv, argc, "--key", key )  ||
			cmdline::read( i, argv, argc, "-k",    key )  ||
			cmdline::read( i, argv, argc, "--in",  in )   ||
			cmdline::read( i, argv, argc, "-i",    in )   ||
			cmdline::read( i, argv, argc, "--out", out )  ||
			cmdline::read( i, argv, argc, "-o",    out )  ||
			false ){}
		else
		{
			THROW_RUNTIME( "Unknown option: " << argv[i] );
		}
	}

	if( mode.empty() )
		THROW_RUNTIME( "Invalid usage." );

	static const char* modes[] = { "invalid", "encrypt", "decrypt", "init", "--help" };

	for( size_t i = 0; i < sizeof( modes ) / sizeof( *modes ); i++ )
	{
		if( modes[i] == mode )
			config.mode = (Config::EMode) i;
	}

	if( Config::Invalid == config.mode )
		THROW_RUNTIME( "Invalid mode: " << mode );
	
	config.key = key;
	config.in = in;
	config.out = out;
}

