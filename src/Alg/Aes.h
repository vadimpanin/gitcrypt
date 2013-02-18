// Copyright 2010 Vadim Schetinin (vadim -at- schetin.in).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <openssl/engine.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/evp.h>

namespace alg {	// begin namespace alg
namespace aes {	// begin namespace aes

class Engine
{
public:
	Engine( const std::string& key )
	{
		memset( _key, 0, sizeof( _key ) );
		memcpy( _key, key.data(), std::min< size_t>( key.size(), sizeof( _key ) ) );
	}

	enum { cipher_block_size = 16 };

	void encrypt( const std::string& src, std::string& dst ) const
	{
		if( !src.size() )
			THROW_LOGIC( "zero size input" );

		EVP_CIPHER_CTX ctx;
		EVP_CIPHER_CTX_init( &ctx );

		if( !EVP_EncryptInit_ex( &ctx, EVP_aes_256_cbc(), NULL, _key, NULL ) )
			THROW_LOGIC( "encrypt init error" );

		int dst_oldsize = static_cast< int>( dst.size() );
		int src_size = static_cast< int>( src.size() );
		int written = src_size + cipher_block_size + 1;
		dst.resize( dst_oldsize + written );

		if( !EVP_EncryptUpdate( &ctx, (uint8_t *)&dst[ dst_oldsize ], &written, (const uint8_t *)src.data(), src_size ) )
			THROW_LOGIC( "encrypt update error" );
		dst_oldsize += written;

		if( !EVP_EncryptFinal_ex( &ctx, (uint8_t *)&dst[ dst_oldsize ], &written ) )
			THROW_LOGIC( "encrypt final error" );

		dst_oldsize += written;

		dst.resize( dst_oldsize );

		EVP_CIPHER_CTX_cleanup(&ctx);
	}


	void decrypt( const std::string& src, std::string& dst ) const
	{
		if( !src.size() )
			THROW_LOGIC( "zero size input" );

		EVP_CIPHER_CTX ctx;
		EVP_CIPHER_CTX_init( &ctx );

		if( !EVP_DecryptInit_ex( &ctx, EVP_aes_256_cbc(), NULL, _key, NULL ) )
			THROW_LOGIC( "decrypt init error" );

		int dst_oldsize = static_cast< int >( dst.size() );
		int written = static_cast< int >( src.size() );
		dst.resize( dst_oldsize + written );

		if( !EVP_DecryptUpdate( &ctx, (uint8_t *)&dst[ dst_oldsize ], &written, (const uint8_t *)src.data(), (int) src.size() ) )
			THROW_LOGIC( "decrypt update error" );
		dst_oldsize += written;

		if( !EVP_DecryptFinal_ex( &ctx, (uint8_t *)&dst[ dst_oldsize ], &written ) )
			THROW_LOGIC( "decrypt final error" );
		dst_oldsize += written;

		dst.resize( dst_oldsize );

		EVP_CIPHER_CTX_cleanup(&ctx);
	}

private:
	unsigned char _key[ 32 ];
};

}
}
