/*
radio_window.cpp

Window to host a radio instance

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
#include "radio_component.h"
#include "radio_window.h"
#include "../Properties/prop_cache.h"

RadioWindow::RadioWindow(String p_radio_id) : DocumentWindow("RadioWindow",
	Desktop::getInstance().getDefaultLookAndFeel()
	.findColour(ResizableWindow::backgroundColourId),
	DocumentWindow::allButtons)
{
	radio_id = p_radio_id;
	if (radio_id == "radio-1") setName("Receiver-1");
	else if (radio_id == "radio-2") setName("Receiver-2");
	else setName("Receiver-3");

	setUsingNativeTitleBar(true);

	c = new RadioComponent(radio_id);
	setContentOwned(c, true);
	

#if JUCE_IOS || JUCE_ANDROID
	setFullScreen(true);
#else
	setResizable(true, true);
#endif

	// Restore position
	int X = PropCache::getInstance()->get_prop_inst(radio_id)->getIntValue("X", var(100));
	int Y = PropCache::getInstance()->get_prop_inst(radio_id)->getIntValue("Y", var(100));
	setTopLeftPosition(X, Y);

	setVisible(true);
}

void RadioWindow::moved() {
	DocumentWindow::moved();
	if (getX() > 0 && getY() > 0) {
		// Set new position
		PropCache::getInstance()->get_prop_inst(radio_id)->set_value("X", var(getX()));
		PropCache::getInstance()->get_prop_inst(radio_id)->set_value("Y", var(getY()));
	}
}

RadioComponent *RadioWindow::get_component() {
	return c;
}