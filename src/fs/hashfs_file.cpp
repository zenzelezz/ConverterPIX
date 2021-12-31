/******************************************************************************
 *
 *  Project:	ConverterPIX @ Core
 *  File:		/fs/hashfs_file.cpp
 *
 *		  _____                          _            _____ _______   __
 *		 / ____|                        | |          |  __ \_   _\ \ / /
 *		| |     ___  _ ____   _____ _ __| |_ ___ _ __| |__) || |  \ V /
 *		| |    / _ \| '_ \ \ / / _ \ '__| __/ _ \ '__|  ___/ | |   > <
 *		| |___| (_) | | | \ V /  __/ |  | ||  __/ |  | |    _| |_ / . \
 *		 \_____\___/|_| |_|\_/ \___|_|   \__\___|_|  |_|   |_____/_/ \_\
 *
 *
 *  Copyright (C) 2017 Michal Wojtowicz.
 *  All rights reserved.
 *
 *   This software is ditributed WITHOUT ANY WARRANTY; without even
 *   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *   PURPOSE. See the copyright file for more information.
 *
 *****************************************************************************/

#include "../prerequisites.h"

#include "hashfilesystem.h"
#include "hashfs_file.h"

HashFsFile::HashFsFile(const String &filepath, HashFileSystem *filesystem, const prism::hashfs_entry_t *header)
	: m_filepath(filepath)
	, m_filesystem(filesystem)
	, m_header(header)
	, m_position(0)
{
	using namespace prism;

	if (m_header->m_flags & HASHFS_COMPRESSED)
	{
		inflateInitialize();
	}
}

HashFsFile::~HashFsFile()
{
	using namespace prism;

	if (m_header->m_flags & HASHFS_COMPRESSED)
	{
		inflateDestroy();
	}
}

uint64_t HashFsFile::write(const void *buffer, uint64_t elementSize, uint64_t elementCount)
{
	return 0;
}

uint64_t HashFsFile::read(void *buffer, uint64_t elementSize, uint64_t elementCount)
{
	using namespace prism;

	if (!(m_header->m_flags & HASHFS_COMPRESSED))
	{
		if (m_position >= m_header->m_size)
		{
			return 0;
		}

		if (m_filesystem->ioRead(buffer, elementSize * elementCount, m_header->m_offset + m_position))
		{
			uint64_t result = std::min(elementSize * elementCount, m_header->m_size - m_position);
			m_position += elementSize * elementCount;
			if (m_position > m_header->m_size)
			{
				m_position = m_header->m_size;
			}
			return result;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		const uint64_t chunk = 1024 * 4;
		uint8_t inbuffer[chunk];
		uint64_t bufferOffset = 0;

		while (m_position < m_header->m_compressed_size && bufferOffset < (elementSize * elementCount))
		{
			uint64_t left = m_header->m_compressed_size - m_position;
			uint64_t bytes = std::min(chunk, left);
			if (bytes == 0) {
				break;
			}

			if (!m_filesystem->ioRead(inbuffer, bytes, m_header->m_offset + m_position))
			{
				error("hashfs", m_filepath, "Unable to read from filesystem file");
				return 0;
			}

			m_stream.avail_in = static_cast<unsigned int>(bytes);
			m_stream.next_in = inbuffer;

			m_stream.avail_out = static_cast<unsigned int>((elementSize * elementCount) - bufferOffset);
			m_stream.next_out = (uint8_t *)buffer + bufferOffset;

			int ret = inflate(&m_stream, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);

			if (ret != Z_OK && ret != Z_STREAM_END)
			{
				error_f("hashfs", m_filepath, "zLib error: %s", zError(ret));
				return 0;
			}

			uint64_t wroteToBuffer = ((elementSize * elementCount) - bufferOffset) - m_stream.avail_out;
			bufferOffset += wroteToBuffer;
			assert(bufferOffset <= (elementSize * elementCount));
			m_position += (bytes - m_stream.avail_in);
		}
		return bufferOffset;
	}
}

uint64_t HashFsFile::size()
{
	return m_header->m_size;
}

bool HashFsFile::seek(uint64_t offset, Attrib attr)
{
	if (m_header->m_flags & prism::HASHFS_COMPRESSED)
	{
		if (offset == 0 && attr == SeekSet)
		{
			if (m_position != 0)
			{
				inflateDestroy();
				inflateInitialize();
			}
			return true;
		}
		return false; // random access is not allowed
	}
	else
	{
		if (attr == SeekSet)
		{
			m_position = offset;
		}
		else if (attr == SeekCur)
		{
			m_position += offset;
		}
		else if (attr == SeekEnd)
		{
			m_position = size() - offset;
		}
		return true;
	}
}

void HashFsFile::rewind()
{
	seek(0, SeekSet);
}

uint64_t HashFsFile::tell() const
{
	return m_position;
}

void HashFsFile::flush()
{
}

void HashFsFile::inflateInitialize()
{
	m_stream.zalloc = Z_NULL;
	m_stream.zfree = Z_NULL;
	m_stream.opaque = Z_NULL;
	m_stream.avail_in = 0;
	m_stream.next_in = Z_NULL;
	if (inflateInit(&m_stream) != Z_OK)
	{
		error("hashfs", "", "Failed to inflate init");
		return;
	}
}

void HashFsFile::inflateDestroy()
{
	inflateEnd(&m_stream);
}

/* eof */
