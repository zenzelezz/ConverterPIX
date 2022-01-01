/******************************************************************************
 *
 *  Project:    ConverterPIX @ Core
 *  File:       /structs/dds.cpp
 *
 *          _____                          _            _____ _______   __
 *         / ____|                        | |          |  __ \_   _\ \ / /
 *        | |     ___  _ ____   _____ _ __| |_ ___ _ __| |__) || |  \ V /
 *        | |    / _ \| '_ \ \ / / _ \ '__| __/ _ \ '__|  ___/ | |   > <
 *        | |___| (_) | | | \ V /  __/ |  | ||  __/ |  | |    _| |_ / . \
 *         \_____\___/|_| |_|\_/ \___|_|   \__\___|_|  |_|   |_____/_/ \_\
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

#include "../fs/file.h"
#include "../fs/sysfilesystem.h"

#include "dds.h"

namespace dds
{
    void print_debug(String filepath)
    {
        auto file = getSFS()->open(filepath, FileSystem::read | FileSystem::binary);
        if (!file)
        {
            printf("Cannot open dds file: \"%s\"! %s" SEOL, filepath.c_str(), strerror(errno));
            return;
        }
        else
        {
            const size_t fileSize = static_cast<size_t>(file->size());
            UniquePtr<uint8_t[]> buffer(new uint8_t[fileSize]);
            file->read((char *)buffer.get(), sizeof(char), fileSize);
            file.reset();

            const uint32_t magic = *(uint32_t *)(buffer.get());
            if (magic != dds::MAGIC)
            {
                printf("Invalid dds magic: %i expected: %i\n", magic, dds::MAGIC);
                return;
            }
            dds::header *header = (dds::header *)(buffer.get() + 4);
            if (header->m_pixel_format.m_flags & dds::PF_FOUR_CC)
            {
                printf("compression: %s\n", dds::uint2s(header->m_pixel_format.m_four_cc).c_str());
            }
            else
            {
                const auto pxformat = dds::recognize_pixel_format(&header->m_pixel_format);
                if (pxformat)
                {
                    printf("pixel format = %s\n", pxformat->m_name);
                }
                else
                {
                    printf("pixel format not known\n");
                }
                printf("bits: %i masks(rgba): %X/%X/%X/%X flags: %X\n",
                       header->m_pixel_format.m_rgb_bit_count,
                       header->m_pixel_format.m_r_bit_mask,
                       header->m_pixel_format.m_g_bit_mask,
                       header->m_pixel_format.m_b_bit_mask,
                       header->m_pixel_format.m_a_bit_mask,
                       header->m_pixel_format.m_flags);
            }
            printf("mipmaps count: %u\n", header->m_mip_map_count);
        }
    }
}

/* eof */
