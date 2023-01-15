/*
prop_cache.cpp

Property cache for the Juce Console

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

#include "prop_cache.h"

//==============================================================================
// Radio Interface Singleton class
//==============================================================================

//==============================================================================
// Public
PropCache::PropCache() {
	main = new Properties("main", "main_config.cfg");
	radio_1 = new Properties("radio-1", "r1_config.cfg");
	radio_2 = new Properties("radio-2", "r2_config.cfg");
	radio_3 = new Properties("radio-3", "r3_config.cfg");
}

PropCache::~PropCache() {
	save_all();
	clearSingletonInstance();
}

//Macro for Singleton
juce_ImplementSingleton(PropCache)

//==============================================================================
// Save/Restore methods

void PropCache::restore_all() {
	main->load();
	radio_1->load();
	radio_2->load();
	radio_3->load();
}

void PropCache::save_all() {
	main->save();
	radio_1->save();
	radio_2->save();
	radio_3->save();
}

//==============================================================================
// Get/Set methods
// Access via the Properties instance

Properties* PropCache::get_prop_inst(String name) {
	if (name == "main") return main;
	else if (name == "radio-1") return radio_1;
	else if (name == "radio-2") return radio_2;
	else return radio_3;
}