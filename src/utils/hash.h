/******************************************************************************
 *
 *  Project:    ConverterPIX @ Core
 *  File:       /utils/hash.h
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

#pragma once

class Hash
{
public:
    Hash(const char *const data, uint32_t seed = 0)
    {
        m_value = seed;
        for (const char *s = data; *s; m_value = m_value * 101 + *s++);
    }

    Hash(const String &s, uint32_t seed = 0)
        : Hash(s.c_str(), seed)
    {
    }

private:
    uint32_t m_value;
};

/* eof */
