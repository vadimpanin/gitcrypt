// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>

#include "Exception.h"

namespace cmdline {

	template< typename T >
	static bool read( int& i, char * argv[], int argc, const std::string& name, T& val )
	{
		if( name != argv[ i ] ) return false;

		if( i == argc - 1) // end
			THROW_RUNTIME( "Invalid " << name << " usage" );

		val = boost::lexical_cast< T >( argv[ i + 1 ] );
		i++;

		return true;
	}

	static bool readPresense( int& i, char * argv[], int argc, const std::string& name, bool& val )
	{
		if( name != argv[ i ] ) return false;

		val = true;
		return true;
	}

	template< typename T >
	static bool readStatic( int& i, char * argv[], int argc, int n, T& val )
	{
		if( i != n ) return false;

		val = boost::lexical_cast< T >( argv[ i ] );
		return true;
	}
}

