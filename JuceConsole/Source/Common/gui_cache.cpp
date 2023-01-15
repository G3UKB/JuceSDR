/*
gui_cache.cpp

Radio interface for the Juce Console

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

#include "gui_cache.h"
#include "../Common/extern.h"

//==============================================================================
// Radio Interface Singleton class
//==============================================================================

//==============================================================================
// Public

//Macro for Singleton
juce_ImplementSingleton(GUICache)

//==============================================================================
// Get/Set methods

void GUICache::setVFOInst(String radio_id, Component* pvfo) {
	if (radio_id == "radio_1")
		vfo_1 = pvfo;
	else if (radio_id == "radio_2")
		vfo_2 = pvfo;
	else
		vfo_3 = pvfo;
}

Component* GUICache::getVFOInst(String radio_id) {
	if (radio_id == "radio_1")
		return vfo_1;
	else if (radio_id == "radio_2")
		return vfo_2;
	else
		return vfo_3;
}
