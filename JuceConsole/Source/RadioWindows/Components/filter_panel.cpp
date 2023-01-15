/*
filter_panel.cpp

Filter buttons component for the Juce Console

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

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../Common/extern.h"
#include "../../RadioInterface/radio_interface.h"
#include "../../Properties/prop_cache.h"
#include "filter_panel.h"

//==============================================================================

FilterButton::FilterButton(int p_i_radio_id, String p_s_radio_id, String label) {

	setButtonText(label);
	setClickingTogglesState(true);

	i_radio_id = p_i_radio_id;
	s_radio_id = p_s_radio_id;
}

FilterButton::~FilterButton() {}

void FilterButton::clicked() {
	
	if (getToggleState()) {
		int filter_id = getComponentID().getIntValue();
		RadioInterface::getInstance()->ri_server_set_rx_filter_freq(i_radio_id, filter_id);
		PropCache::getInstance()->get_prop_inst(s_radio_id)->set_value("FILTER", var(filter_id));
	}
}

//==============================================================================
// The panel
FilterPanel::FilterPanel(String p_radio_id) {

	// Local vars
	radio_id = p_radio_id;

	// Radio interface requires a numeric id
	if (radio_id == "radio-1") i_radio = 0;
	else if (radio_id == "radio-2") i_radio = 1;
	else i_radio = 2;

	// Create all filter buttons
	create_buttons();
	// Retrieve last filter
	int filter = PropCache::getInstance()->get_prop_inst(radio_id)->getIntValue("FILTER", var(0));
	// Toggle appropriate button
	switch (filter) {
	case 0: Filt6K0Button->setToggleState(true, NotificationType::dontSendNotification); break;
	case 1: Filt4K0Button->setToggleState(true, NotificationType::dontSendNotification); break;
	case 2: Filt2K7Button->setToggleState(true, NotificationType::dontSendNotification); break;
	case 3: Filt2K4Button->setToggleState(true, NotificationType::dontSendNotification); break;
	case 4: Filt2K1Button->setToggleState(true, NotificationType::dontSendNotification); break;
	case 5: Filt1K0Button->setToggleState(true, NotificationType::dontSendNotification); break;
	case 6: Filt500Button->setToggleState(true, NotificationType::dontSendNotification); break;
	case 7: Filt250Button->setToggleState(true, NotificationType::dontSendNotification); break;
	case 8: Filt100Button->setToggleState(true, NotificationType::dontSendNotification); break;
	default: Filt2K4Button->setToggleState(true, NotificationType::dontSendNotification);
	}
	// Set radio filter
	RadioInterface::getInstance()->ri_server_set_rx_filter_freq(i_radio, filter);
}

FilterPanel::~FilterPanel() {}

void FilterPanel::resized() {
	// This is called when the filter panel is resized.
	layout_buttons_in_grid();
}

//==============================================================================
// Private
void FilterPanel::create_buttons() {

	Filt6K0Button = new FilterButton(i_radio, radio_id, "6.0 KHz");
	Filt6K0Button->setComponentID("0");
	Filt6K0Button->setRadioGroupId(1);
	addAndMakeVisible(Filt6K0Button);

	Filt4K0Button = new FilterButton(i_radio, radio_id, "4.0 KHz");
	Filt4K0Button->setComponentID("1");
	Filt4K0Button->setRadioGroupId(1);
	addAndMakeVisible(Filt4K0Button);

	Filt2K7Button = new FilterButton(i_radio, radio_id, "2.7 KHz");
	Filt2K7Button->setComponentID("2");
	Filt2K7Button->setRadioGroupId(1);
	addAndMakeVisible(Filt2K7Button);

	Filt2K4Button = new FilterButton(i_radio, radio_id, "2.4KHz");
	Filt2K4Button->setComponentID("3");
	Filt2K4Button->setRadioGroupId(1);
	addAndMakeVisible(Filt2K4Button);

	Filt2K1Button = new FilterButton(i_radio, radio_id, "2.1KHz");
	Filt2K1Button->setComponentID("4");
	Filt2K1Button->setRadioGroupId(1);
	addAndMakeVisible(Filt2K1Button);

	Filt1K0Button = new FilterButton(i_radio, radio_id, "1.0KHz");
	Filt1K0Button->setComponentID("5");
	Filt1K0Button->setRadioGroupId(1);
	addAndMakeVisible(Filt1K0Button);

	Filt500Button = new FilterButton(i_radio, radio_id, "500 Hz");
	Filt500Button->setComponentID("6");
	Filt500Button->setRadioGroupId(1);
	addAndMakeVisible(Filt500Button);

	Filt250Button = new FilterButton(i_radio, radio_id, "250 Hz");
	Filt250Button->setComponentID("7");
	Filt250Button->setRadioGroupId(1);
	addAndMakeVisible(Filt250Button);

	Filt100Button = new FilterButton(i_radio, radio_id, "100 Hz");
	Filt100Button->setComponentID("8");
	Filt100Button->setRadioGroupId(1);
	addAndMakeVisible(Filt100Button);
}

void FilterPanel::layout_buttons_in_grid() {

	// Local grid as its just a bag of behaviour
	Grid grid;

	// Add digits in one horizontal row
	using Track = Grid::TrackInfo;

	// Layout in 3 cols by 3 rows
	grid.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr) };
	grid.templateRows = { Track(1_fr),  Track(1_fr),  Track(1_fr) };

	// Add items to the grid
	grid.items = {
		GridItem(Filt6K0Button),
		GridItem(Filt4K0Button),
		GridItem(Filt2K7Button),
		GridItem(Filt2K4Button),
		GridItem(Filt2K1Button),
		GridItem(Filt1K0Button),
		GridItem(Filt500Button),
		GridItem(Filt250Button),
		GridItem(Filt100Button)
	};

	grid.justifyItems = Grid::JustifyItems::stretch;
	// Add gaps if required
	//grid.rowGap = Grid::Px::Px(10.0f);
	//grid.columnGap = Grid::Px::Px(10.0f);
	grid.performLayout(getLocalBounds());
}