/*
radio_select_frame.cpp

Radio select component for the Juce Console

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

#include "../../Common/extern.h"
#include "../common_content.h"
#include "../../RadioInterface/radio_interface.h"
#include "../../RadioWindows/radio_window.h"
#include "../../RadioWindows/radio_win_cache.h"
#include "radio_select_frame.h"

//==============================================================================
// Radio select buttons
// Frame
SelectFrame::SelectFrame(String label) {

	// Create buttons
	setText(label);


	select_button_r1 = new SelectButton("radio-1", "1");
	select_button_r2 = new SelectButton("radio-2", "2");
	select_button_r3 = new SelectButton("radio-3", "3");
	addAndMakeVisible(select_button_r1);
	addAndMakeVisible(select_button_r2);
	addAndMakeVisible(select_button_r3);
}

void SelectFrame::resized() {

	// Local grid as its just a bag of behaviour
	Grid grid;

	// Layout in 1 row by 3 cols
	using Track = Grid::TrackInfo;
	grid.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr) };
	grid.templateRows = { Track(15_px) };
	grid.autoColumns = Track(1_fr);
	grid.autoRows = Track(1_fr);
	grid.autoFlow = Grid::AutoFlow::row;
	grid.justifyItems = Grid::JustifyItems::center;
	grid.alignItems = Grid::AlignItems::center;
	grid.justifyContent = Grid::JustifyContent::center;
	grid.alignContent = Grid::AlignContent::center;

	// Add items to the grid
	grid.items.addArray({
		GridItem(select_button_r1),
		GridItem(select_button_r2),
		GridItem(select_button_r3),
		});

	grid.performLayout(Rectangle<int>(10, 15, getWidth() - 20, getHeight() - 20));
}

//==============================================================================
// Select buttons
SelectButton::SelectButton(String p_radio_id, String label) {

	// Make it a toggle in the select group and set its radio id
	radio_id = p_radio_id;
	setButtonText(label);
	setClickingTogglesState(true);
	setRadioGroupId(1);
	setComponentID(p_radio_id);
	// ToDo: should persist state
	if (p_radio_id == "radio-1")
		setToggleState(true, NotificationType::dontSendNotification);
}

void SelectButton::clicked() {

	if (getToggleState()) {
		// Button toggle on
		// Get number of receivers
		int num_rx;
		if (getComponentID() == "radio-1") num_rx = 1;
		else if (getComponentID() == "radio-2") num_rx = 2;
		else num_rx = 3;

		// Stop radio and reset interface
		RadioInterface::getInstance()->ri_radio_stop();
		RadioInterface::getInstance()->reset();

		// Turn off start button
		ControlPanel *p = (ControlPanel *)(getParentComponent()->getParentComponent());
		p->get_start_button()->setToggleState(false, NotificationType::dontSendNotification);
		p->get_start_button()->setButtonText("Start");

		// Stop any radio instances
		RadioWinCache::getInstance()->delete_window("radio-1");
		RadioWinCache::getInstance()->delete_window("radio-2");
		RadioWinCache::getInstance()->delete_window("radio-3");

		// Terminate server as we can't patch this in to a running server
		if (!c_server_terminate()) {
			printf("Failed to terminate server, unable to continue!");
			JUCEApplicationBase::quit();
		}
		// Wait a bit to ensure terminated
		Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 1000);

		// Now start up from scratch
		// Initialise server
		if (!c_server_init()) {
			printf("Failed to initialise server, unable to continue!");
			JUCEApplicationBase::quit();
		}
		// Set new number of receivers
		c_server_set_num_rx(num_rx);

		if (!RadioInterface::getInstance()->ri_set_default_audio()) {
			// Something is kinda wrong here, can't continue
			std::cout << std::endl << "Failed to configure audio, unable to continue!" << std::endl;
			JUCEApplicationBase::quit();
		}

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
				// Start radio instances
				std::cout << "Starting radios" << std::endl;
				RadioWindow * w1 = RadioWinCache::getInstance()->create_window("radio-1");
				w1->get_component()->start_ui();
				if (num_rx > 1) {
					RadioWindow * w2 = RadioWinCache::getInstance()->create_window("radio-2");
					w2->get_component()->start_ui();
				}
				if (num_rx > 2) {
					RadioWindow * w3 = RadioWinCache::getInstance()->create_window("radio-3");
					w3->get_component()->start_ui();
				}
			}
		}
	}
}
