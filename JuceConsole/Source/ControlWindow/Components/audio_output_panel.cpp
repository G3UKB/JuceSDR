/*
audio_output_panel.cpp

Audio output component for the Juce Console

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

#include "../../RadioInterface/radio_interface.h"
#include "../../Common/extern.h"
#include "audio_output_panel.h"

//==============================================================================
// Audio Model
// Constructor
AudioModel::AudioModel(AudioType p_type) {
	
	table = new TableListBox();
	table->setModel(this);
	table->setColour(TableListBox::backgroundColourId, Colours::darkgrey);

	table->setHeaderHeight(22);
	table->getHeader().addColumn("Device", 1, 150, 100, 200, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("Rx-1", 2, 30, 30, 30, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("Rx-2", 3, 30, 30, 30, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("Rx-3", 4, 30, 30, 30, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("To", 5, 80, 80, 80, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("Host", 6, 100, 100, 150, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("Set", 7, 30, 30, 30, TableHeaderComponent::defaultFlags);

	// Get the output enumeration
	audio_outputs = c_server_enum_audio_outputs();
	// We now copy this into a state structure to drive our table
	dsl = new DeviceStateList;
	dsl->entries = audio_outputs->entries;
	for (int i = 0; i < audio_outputs->entries; i++) {
		dsl->devices[i].name = audio_outputs->devices[i].name;
		dsl->devices[i].host_api = audio_outputs->devices[i].host_api;
		dsl->devices[i].active = false;
		dsl->devices[i].dest = 0;
		dsl->devices[i].rx_1 = false;
		dsl->devices[i].rx_2 = false;
		dsl->devices[i].rx_3 = false;
	}

	num_rows = dsl->entries;
}

//=====================================================================================================
// Overrides

int AudioModel::getNumRows() {
	return num_rows;
}

// Highlight active rows
void AudioModel::paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) {
	if(dsl->devices[rowNumber].active)
		g.fillAll(Colours::lightblue);
	else
		g.fillAll(Colours::darkgrey);
	// This is the only place that always gets called
	// This will update the custom components
	table->updateContent();
	// This will repaint so the highlighting works
	table->repaint();
}

// Manage fixed text
void AudioModel::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
	g.setFont(12);
	if (dsl->devices[rowNumber].active)
		g.fillAll(Colours::lightblue);
	else
		g.fillAll(Colours::darkgrey);

	switch (columnId) {
	case 1 :
		g.drawText(dsl->devices[rowNumber].name, 2, 0, width - 4, height, Justification::centredLeft, true);
		break;
	case 6:
		g.drawText(dsl->devices[rowNumber].host_api, 2, 0, width - 4, height, Justification::centredLeft, true);
	}
}

// Manage custom components
Component* AudioModel::refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) {
	// Not custom components
	if (columnId == 1 || columnId == 6) {
		jassert(existingComponentToUpdate == nullptr);
		return nullptr;
	}

	// Destination component
	if (columnId == 5) {
		ToColumnCustomComponent *new_to_col;
		auto* to_col = static_cast<ToColumnCustomComponent*> (existingComponentToUpdate);
		if (to_col == nullptr)
			new_to_col = new ToColumnCustomComponent(*this);
		else
			new_to_col = to_col;
		new_to_col->setRowAndColumn(rowNumber, columnId);
		return new_to_col;
	}

	// Set button
	if (columnId == 7) {
		// Need to establish if this row is a candidate for activation.
		bool enable = false;
		if ((get_dest(rowNumber) != 0) &&
			(get_rx_1(rowNumber) || get_rx_2(rowNumber) || get_rx_3(rowNumber)))
			enable = true;

		SetColumnCustomComponent *new_set_col;
		auto* set_col = static_cast<SetColumnCustomComponent*> (existingComponentToUpdate);
		if (set_col == nullptr)
			new_set_col = new SetColumnCustomComponent(*this);
		else
			new_set_col = set_col;

		new_set_col->setRowAndColumn(rowNumber, columnId);
		new_set_col->setEnabled(enable);
		if (!enable && get_active(rowNumber))
			// We were active but something has changed so we are not now eligible
			set_active(rowNumber, false);
		return new_set_col;
	}

	// Remainder are RX components
	int id;
	if (columnId == 2) id = 1;
	else if (columnId == 3) id = 2;
	else id = 3;

	RxColumnCustomComponent *new_rx_col;
	auto* rx_col = static_cast<RxColumnCustomComponent*> (existingComponentToUpdate);
	if (rx_col == nullptr)
		new_rx_col = new RxColumnCustomComponent(*this, id);
	else
		new_rx_col = rx_col;
	new_rx_col->setRowAndColumn(rowNumber, columnId);
	return new_rx_col;
}

//=====================================================================================================
// Return our table
TableListBox *AudioModel::get_table() {
	return table;
}

//=====================================================================================================
// Get/set methods

bool AudioModel::get_active(int row) {
	return dsl->devices[row].active;
}
void AudioModel::set_active(int row, bool state) {
	dsl->devices[row].active = state;
}

int AudioModel::get_dest(int row) {
	return dsl->devices[row].dest;
}
void AudioModel::set_dest(int row, int index, String item) {
	dsl->devices[row].dest = index;
	dsl->devices[row].sdest = item;
}

bool AudioModel::get_rx_1(int row) {
	return dsl->devices[row].rx_1;
}
void AudioModel::set_rx_1(int row, bool state) {
	dsl->devices[row].rx_1 = state;
}

bool AudioModel::get_rx_2(int row) {
	return dsl->devices[row].rx_2;
}
void AudioModel::set_rx_2(int row, bool state) {
	dsl->devices[row].rx_2 = state;
}

bool AudioModel::get_rx_3(int row) {
	return dsl->devices[row].rx_3;
}
void AudioModel::set_rx_3(int row, bool state) {
	dsl->devices[row].rx_3 = state;
}

DeviceStateList*  AudioModel::get_state() {
	return dsl;
}

//==============================================================================
// Audio Panel
// Constructor
AudioPanel::AudioPanel() {

	// Audio selection
	audioModel = new AudioModel(AudioType::OUTPUT);
	addAndMakeVisible(audioModel->get_table());
	// Apply button
	applyButton = new ApplyButton(audioModel, "Apply");
	addAndMakeVisible(applyButton);
}

void AudioPanel::resized() {
	// This is called when the radio panel is resized.
	layout_components_in_grid();
}

void AudioPanel::layout_components_in_grid() {

	// Local grid as its just a bag of behaviour
	Grid grid;

	// Layout in 1 row by 4 cols so that exit stays right and other stay left
	using Track = Grid::TrackInfo;
	grid.templateColumns = { Track(1_fr) };
	grid.templateRows = { Track(1_fr),  Track(25_px) };
	grid.autoColumns = Track(1_fr);
	grid.autoRows = Track(1_fr);
	grid.autoFlow = Grid::AutoFlow::column;
	grid.justifyItems = Grid::JustifyItems::center;

	// Add items to the grid
	grid.items.addArray({
		GridItem(audioModel->get_table()),
		GridItem(applyButton)
		});

	grid.performLayout(getLocalBounds());
}

//==============================================================================
// Apply
void ApplyButton::clicked() {
	DeviceStateList *dsl = owner->get_state();

	// Halt audio processes
	if (!c_server_clear_audio_routes()) {
		std::cout << "Failed to clear audio routes!" << std::endl;
		return;
	}

	// Reset audio routes using active data
	// We assume here that validity has already been enforced in the UI
	// such that there are no conflicts or invalid combinations.
	for (int i = 0; i < dsl->entries; i++) {
		if (dsl->devices[i].active) {
			// We have an active route to add
			int rx_count = 0;
			if (dsl->devices[i].rx_1) rx_count++;
			if (dsl->devices[i].rx_2) rx_count++;
			if (dsl->devices[i].rx_3) rx_count++;
			printf("Count: %d\n", rx_count);
			String dest;
			switch (dsl->devices[i].dest) {
			case 1: dest = HPSDR; break;
			case 2: dest = LOCAL; break;
			case 3: dest = LOCAL_AF; break;
			case 4: dest = LOCAL_IQ; break;
			}

			if (rx_count == 1) {
				if (dsl->devices[i].rx_1) set_route(dest, 1, dsl->devices[i].host_api, dsl->devices[i].name, BOTH);
				if (dsl->devices[i].rx_2) set_route(dest, 2, dsl->devices[i].host_api, dsl->devices[i].name, BOTH);
				if (dsl->devices[i].rx_3) set_route(dest, 3, dsl->devices[i].host_api, dsl->devices[i].name, BOTH);
			}
			else if (rx_count == 2) {
				if (dsl->devices[i].rx_1 && dsl->devices[i].rx_2) {
					set_route(dest, 1, dsl->devices[i].host_api, dsl->devices[i].name, LEFT);
					set_route(dest, 2, dsl->devices[i].host_api, dsl->devices[i].name, RIGHT);
				}
				else {
					set_route(dest, 2, dsl->devices[i].host_api, dsl->devices[i].name, LEFT);
					set_route(dest, 3, dsl->devices[i].host_api, dsl->devices[i].name, RIGHT);
				}
			}
		}
	}

	// Restart audio processes
	if (!c_server_restart_audio_routes()) {
		std::cout << "Failed to restart audio routes!" << std::endl;
	}
}

// Set a single audio route
void ApplyButton::set_route(String dest, int rx, String host_api, String dev, String channel) {

	c_server_set_audio_route(
		(int)AudioType::OUTPUT,
		(char*)dest.toRawUTF8(),
		rx,
		(char*)host_api.toRawUTF8(),
		(char*)dev.toRawUTF8(),
		(char*)channel.toRawUTF8()
	);
}