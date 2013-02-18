// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

class Output
{
public:
	virtual size_t write( const void* dst, size_t element_size, size_t count ) = 0;

	virtual ~Output() {}

	typedef boost::shared_ptr< Output > Ptr;
};

class FileOutput : public Output
{
public:
	FileOutput( const char* fn )
		: f_( NULL )
	{
		if( 0 == strcmp( "-", fn ) )
		{
			f_ = stdout;
			return;
		}

		f_ = fopen( fn, "wb" );
		if( !f_ )
			THROW_RUNTIME( "unable to open file for writing: " << fn );
	}

	size_t write( const void* dst, size_t element_size, size_t count )
	{
		return fwrite( dst, element_size, count, f_ );

		//if( fwrite( dst, element_size, count, f_ ) != count )
		//	THROW_RUNTIME( "error writing file" );
	}

	~FileOutput()
	{
		if( f_ && f_ != stdin ) fclose( f_ );
	}
private:
	FILE* f_;
};
