/*
prop_cache.h

Header for the properties cache

Copyright (C) 2019 by G3UKB Bob Cowdery

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

The authors can be reached by email at:

	bob@bobcowdery.plus.com

*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "properties.h"

//==============================================================================
// Singleton instance for access methods to the gui components.
class PropCache
{
public:
	// JUCEy Singleton
	//==============================================================================
	PropCache();
	~PropCache();

	// Macro to create boilerplate methods for singleton
	juce_DeclareSingleton(PropCache, true);

	//==============================================================================
	// Method prototypes
	Properties* get_prop_inst(String name);
	void restore_all();
	void save_all();

private:
	//==============================================================================
	// State variables
	Properties* main;
	Properties* radio_1;
	Properties* radio_2;
	Properties* radio_3;

	//==============================================================================
	// Method prototypes
	
};