// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifdef _DEBUG

#include <boost/thread/mutex.hpp>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

void __log( const char * sz )
{
	static boost::mutex mutex;

	boost::mutex::scoped_lock sl( mutex );

	static FILE * f = NULL;
	if( !f )
	{
		char filename[ 1024 ];
		_snprintf( filename, 1024, "/tmp/gitcrypt_log_%d.txt", GetCurrentProcessId() );
		f = fopen( filename, "a+" );
	}

	if( !f ) return;
	fputs( sz, f );
	fflush( f );
}

#endif
