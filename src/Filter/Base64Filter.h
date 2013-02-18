// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "ReversibleFilter.h"
#include "Alg/Base64.h"

template< class T >
class Base64Filter : public ReversibleFilter< T >
{
public:
	T& filter( const T& in )
	{
		assert( &in != &out_ );

		out_.clear();
		Base64::encode( in, out_ );
		return out_;
	}

	T& reverse( const T& in )
	{
		assert( &in != &out_ );

		out_.clear();
		Base64::decode( in, out_ );
		return out_;
	}

private:
	T out_;
};
