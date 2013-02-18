// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

class Input
{
public:
	virtual size_t read( void* dst, size_t element_size, size_t max_count ) = 0;

	virtual ~Input() {}

	typedef boost::shared_ptr< Input > Ptr;
};

class FileInput : public Input
{
public:
	FileInput( const char* fn )
		: f_( NULL )
	{
		if( 0 == strcmp( "-", fn ) )
		{
			f_ = stdin;
			return;
		}

		f_ = fopen( fn, "rb" );
		if( !f_ )
			THROW_RUNTIME( "unable to open file for reading: " << fn );
	}

	size_t read( void* dst, size_t element_size, size_t max_count )
	{
		assert( max_count > 0 );

		size_t count = fread( dst, element_size, max_count, f_ );

		if( !count )
		{
			if( feof( f_ ) ) return 0;
			else THROW_RUNTIME( "error reading file" );
		}

		//_LOG( dst );

		return count;
	}

	/*size_t read( T& dst, size_t max_size )
	{
		assert( max_size > 0 );

		dst.resize( max_size );

		size_t size = fread( &dst[0], sizeof( typename T::reference ), max_size, f_ );
		dst.resize( size );

		if( !size )
		{
			if( feof( f_ ) ) return 0;
			else THROW_RUNTIME( "error reading file" );
		}

		return size;
	}*/

	~FileInput()
	{
		if( f_ && f_ != stdin ) fclose( f_ );
	}
private:
	FILE* f_;
};
