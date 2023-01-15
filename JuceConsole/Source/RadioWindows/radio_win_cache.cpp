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

#include "radio_win_cache.h"

//==============================================================================
// Radio Interface Singleton class
//==============================================================================

//==============================================================================
// Public
RadioWinCache::RadioWinCache() {
	
}

RadioWinCache::~RadioWinCache() {
	clearSingletonInstance();
}

//Macro for Singleton
juce_ImplementSingleton(RadioWinCache)


//==============================================================================
// 
RadioWindow *RadioWinCache::create_window(String p_radio_id) {
	if (p_radio_id == "radio-1") {
		win_r1 = new RadioWindow(p_radio_id);
		return win_r1;
	}
	else if (p_radio_id == "radio-2") {
		win_r2 = new RadioWindow(p_radio_id);
		return win_r2;
	}
	else {
		win_r3 = new RadioWindow(p_radio_id);
		return win_r3;
	}
}

void RadioWinCache::delete_window(String p_radio_id) {
	if (p_radio_id == "radio-1") {
		if (win_r1 != nullptr) win_r1->removeFromDesktop();
		win_r1 = nullptr;
	}
	else if (p_radio_id == "radio-2") {
		if (win_r2 != nullptr) win_r2->removeFromDesktop();
		win_r2 = nullptr;
	}
	else {
		if (win_r3 != nullptr) win_r3->removeFromDesktop();
		win_r3 = nullptr;
	}

}
