/*
mode_panel.cpp

Mode buttons component for the Juce Console

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
#include "mode_panel.h"

//==============================================================================

ModeButton::ModeButton(int p_i_radio_id, String p_s_radio_id, String label) {

	setButtonText(label);
	setClickingTogglesState(true);
	i_radio_id = p_i_radio_id;
	s_radio_id = p_s_radio_id;
}

ModeButton::~ModeButton() {}

void ModeButton::clicked() {

	if (getToggleState()) {
		int mode_id = getComponentID().getIntValue();
		RadioInterface::getInstance()->ri_server_set_rx_mode(i_radio_id, mode_id);
		PropCache::getInstance()->get_prop_inst(s_radio_id)->set_value("MODE", var(mode_id));
	}
}

//==============================================================================
// The panel
ModePanel::ModePanel(String p_radio_id) {

	// Local vars
	radio_id = p_radio_id;

	// Radio interface requires a numeric id
	if (radio_id == "radio-1") i_radio = 0;
	else if (radio_id == "radio-2") i_radio = 1;
	else i_radio = 2;

	// Create all mode buttons
	create_buttons();
	// Retrieve last mode
	int mode = PropCache::getInstance()->get_prop_inst(radio_id)->getIntValue("MODE", var(0));
	// Toggle appropriate button
	switch (mode) {
	case 0: LSBButton->setToggleState(true, NotificationType::dontSendNotification); break;
	case 1: USBButton->setToggleState(true, NotificationType::dontSendNotification); break;
	case 2: DSBButton->setToggleState(true, NotificationType::dontSendNotification); break;
	case 3: CWLButton->setToggleState(true, NotificationType::dontSendNotification); break;
	case 4: CWUButton->setToggleState(true, NotificationType::dontSendNotification); break;
	case 5: FMButton->setToggleState(true, NotificationType::dontSendNotification); break;
	case 6: AMButton->setToggleState(true, NotificationType::dontSendNotification); break;
	case 7: DIGUButton->setToggleState(true, NotificationType::dontSendNotification); break;
	case 8: SPECButton->setToggleState(true, NotificationType::dontSendNotification); break;
	case 9: DIGLButton->setToggleState(true, NotificationType::dontSendNotification); break;
	case 10: SAMButton->setToggleState(true, NotificationType::dontSendNotification); break;
	case 11: DRMButton->setToggleState(true, NotificationType::dontSendNotification); break;
	default: LSBButton->setToggleState(true, NotificationType::dontSendNotification);
	}
	// Set radio mode
	RadioInterface::getInstance()->ri_server_set_rx_mode(i_radio, mode);
}

ModePanel::~ModePanel() {}

void ModePanel::resized() {
	// This is called when the mode panel is resized.
	layout_buttons_in_grid();
}

//==============================================================================
// Private
void ModePanel::create_buttons() {

	LSBButton = new ModeButton(i_radio, radio_id, "LSB");
	LSBButton->setComponentID("0");
	LSBButton->setRadioGroupId(1);
	addAndMakeVisible(LSBButton);

	USBButton = new ModeButton(i_radio, radio_id, "USB");
	USBButton->setComponentID("1");
	USBButton->setRadioGroupId(1);
	addAndMakeVisible(USBButton);

	DSBButton = new ModeButton(i_radio, radio_id, "DSB");
	DSBButton->setComponentID("2");
	DSBButton->setRadioGroupId(1);
	addAndMakeVisible(DSBButton);

	CWLButton = new ModeButton(i_radio, radio_id, "CW-L");
	CWLButton->setComponentID("3");
	CWLButton->setRadioGroupId(1);
	addAndMakeVisible(CWLButton);

	CWUButton = new ModeButton(i_radio, radio_id, "CW-U");
	CWUButton->setComponentID("4");
	CWUButton->setRadioGroupId(1);
	addAndMakeVisible(CWUButton);

	FMButton = new ModeButton(i_radio, radio_id, "FM");
	FMButton->setComponentID("5");
	FMButton->setRadioGroupId(1);
	addAndMakeVisible(FMButton);

	AMButton = new ModeButton(i_radio, radio_id, "AM");
	AMButton->setComponentID("6");
	AMButton->setRadioGroupId(1);
	addAndMakeVisible(AMButton);

	DIGUButton = new ModeButton(i_radio, radio_id, "DIG-U");
	DIGUButton->setComponentID("7");
	DIGUButton->setRadioGroupId(1);
	addAndMakeVisible(DIGUButton);

	SPECButton = new ModeButton(i_radio, radio_id, "SPEC");
	SPECButton->setComponentID("8");
	SPECButton->setRadioGroupId(1);
	addAndMakeVisible(SPECButton);

	DIGLButton = new ModeButton(i_radio, radio_id, "DIG-L");
	DIGLButton->setComponentID("9");
	DIGLButton->setRadioGroupId(1);
	addAndMakeVisible(DIGLButton);

	SAMButton = new ModeButton(i_radio, radio_id, "DIG-L");
	SAMButton->setComponentID("10");
	SAMButton->setRadioGroupId(1);
	addAndMakeVisible(SAMButton);

	DRMButton = new ModeButton(i_radio, radio_id, "DRM");
	DRMButton->setComponentID("11");
	DRMButton->setRadioGroupId(1);
	addAndMakeVisible(DRMButton);
}

void ModePanel::layout_buttons_in_grid() {

	// Local grid as its just a bag of behaviour
	Grid grid;

	// Add digits in one horizontal row
	using Track = Grid::TrackInfo;

	// Layout in 4 cols by 3 rows
	grid.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr) };
	grid.templateRows = { Track(1_fr),  Track(1_fr),  Track(1_fr) };

	// Add items to the grid
	grid.items = {
		GridItem(LSBButton),
		GridItem(USBButton),
		GridItem(DSBButton),
		GridItem(CWLButton),
		GridItem(CWUButton),
		GridItem(FMButton),
		GridItem(AMButton),
		GridItem(DIGUButton),
		GridItem(SPECButton),
		GridItem(DIGLButton),
		GridItem(SAMButton),
		GridItem(DRMButton)
	};

	grid.justifyItems = Grid::JustifyItems::stretch;
	// Add gaps if required
	//grid.rowGap = Grid::Px::Px(10.0f);
	//grid.columnGap = Grid::Px::Px(10.0f);
	grid.performLayout(getLocalBounds());
}