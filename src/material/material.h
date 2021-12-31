/******************************************************************************
 *
 *  Project:	ConverterPIX @ Core
 *  File:		/material/material.h
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

#pragma once

#include "../math/vector.h"
#include "../pix/pix.h"
#include "../structs/pmd.h"

class Material
{
public:
	class Attribute
	{
	public:
		Attribute();
		~Attribute();

		void clear();
		String getFormat() const;

	private:
		String m_name;
		enum { FLOAT, STRING } m_valueType;
		uint32_t m_valueCount;
		Double4 m_value;
		String m_stringValue;

		friend Material;
	};

public:
	bool load(String filePath);
	void destroy();

	/**
	 * @brief Creates PIT definition
	 *
	 * @param[in] prefix The prefix for each line of generated definition
	 * @return @c The definition of material
	 */
	String toDefinition(const String &prefix = "") const;

	String toDeclaration(const String &prefix = "") const;

	Pix::Value toPixDefinition() const;
	Pix::Value toPixDeclaration() const;

	/**
	 * @brief Creates alias of material
	 *
	 * @return @c The alias of material
	 */
	String alias() const;

	void setAlias(String name);

	bool convertTextures(String exportPath) const;

	static void convertAttribIfNeeded(Material::Attribute &attrib, const String &effect, const String &attribName, const Array<String> &values);

private:
	String m_effect;
	Array<Texture> m_textures;
	Array<Attribute> m_attributes;
	String m_filePath;		// @example: /material/example.mat
	String m_alias;

	friend Model;
};

/* eof */
