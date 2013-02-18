// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <sstream>

#ifdef _DEBUG
#define WRITE_LOG
#endif

extern void __log( const char * sz );

#ifdef WRITE_LOG
#define _LOG( w ) { std::stringstream ss; ss << w; __log( ss.str().c_str() ); }
#else
#define _LOG( ... ) {}
#endif
