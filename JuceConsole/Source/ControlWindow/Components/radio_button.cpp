/*
radio_button.cpp

Radio button component for the Juce Console

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

//#include "../common_controls.h"
//#include "../Common/extern.h"
#include "../../RadioInterface/radio_interface.h"
#include "../../RadioWindows/radio_window.h"
#include "../../RadioWindows/radio_win_cache.h"
#include "radio_button.h"

//==============================================================================
// Radio control buttons
RadioButton::RadioButton(BUTTON_TYPE p_type, String p_label, String p_label_down, Colour p_normal, Colour p_down) {

	type = p_type;
	label = p_label;
	label_down = p_label_down;

	setButtonText(p_label);
	if (type == BUTTON_TYPE::START_STOP) {
		setClickingTogglesState(true);
		setColour(textColourOffId, p_normal);
		setColour(textColourOnId, p_down);
		if (RadioInterface::getInstance()->is_radio_discovered())
			setEnabled(true);
		else
			setEnabled(false);
	}
	else if (type == BUTTON_TYPE::DISCOVER) {
		setColour(textColourOffId, p_normal);
		setColour(textColourOnId, p_normal);
		if (RadioInterface::getInstance()->is_radio_discovered())
			setEnabled(false);
		else
			setEnabled(true);
	}
}

RadioButton::~RadioButton() {}

void RadioButton::clicked() {

	if (type == BUTTON_TYPE::START_STOP) {
		if (getToggleState()) {
			setButtonText(label_down);
			if (!RadioInterface::getInstance()->ri_radio_start(0)) return;
		}
		else {
			// Switching off
			// Stop radio
			setButtonText(label);
			RadioInterface::getInstance()->ri_radio_stop();
		}
	}
	else if (type == BUTTON_TYPE::DISCOVER) {
		if (!RadioInterface::getInstance()->ri_radio_discover()) {
			// Radio hardware not running or comms problem
			std::cout << "Failed to discover radio, please check hardware and try again." << std::endl;
		}
		else {
			if (!RadioInterface::getInstance()->ri_server_start()) {
				// Something also kinda wrong here, cant continue
				std::cout << std::endl << "Failed to start server processes, unable to continue!" << std::endl;
				JUCEApplicationBase::quit();
			}
			else {
				// Start radio 1 instance
				setEnabled(false);
				ControlPanel *p = (ControlPanel *)getParentComponent();
				p->get_start_button()->setEnabled(true);
				std::cout << "Starting radio 1" << std::endl;
				RadioWindow * w1 = RadioWinCache::getInstance()->create_window("radio-1");
				w1->get_component()->start_ui();
			}
		}
	}
}
