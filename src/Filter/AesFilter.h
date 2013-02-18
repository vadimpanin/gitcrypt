// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "ReversibleFilter.h"
#include "Alg/Aes.h"

template< class T >
class AesFilter : public ReversibleFilter< T >
{
public:
	AesFilter( const std::string key )
		: engine_( key ) {}

	T& filter( const T& in )
	{
		assert( &in != &out_ );

		out_.clear();
		engine_.encrypt( in, out_);
		return out_;
	}

	T& reverse( const T& in )
	{
		assert( &in != &out_ );

		out_.clear();
		engine_.decrypt( in, out_);
		return out_;
	}

private:
	alg::aes::Engine engine_;
	T out_;
};
