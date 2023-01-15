/*
common_content.cpp

Common content for the Juce Console, i.e. not radio dependent

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

#include "../Common/extern.h"
#include "../RadioInterface/radio_interface.h"
#include "../RadioWindows/radio_window.h"
#include "../RadioWindows/radio_win_cache.h"
#include "common_content.h"

//==============================================================================
// The Panel
ControlPanel::ControlPanel() {

	// Local vars

	// Create all radio buttons
	create_components();
}

ControlPanel::~ControlPanel() {}

void ControlPanel::resized() {
	// This is called when the radio panel is resized.
	layout_components_in_grid();
}

RadioButton *ControlPanel::get_start_button() {
	return StartButton;
}

void ControlPanel::paint(Graphics& g)
{
	// The component is opaque, so we must completely fill the background with a solid colour
	g.fillAll(Colours::darkgrey);
	// Draw horizontal lines
	g.setColour(Colours::grey);
	g.fillRect(0, 45, getWidth(), 2);
}

//==============================================================================
// Private
void ControlPanel::create_components() {

	// Radio control buttons
	StartButton = new RadioButton(BUTTON_TYPE::START_STOP, "Start", "Stop", Colours::green, Colours::red);
	addAndMakeVisible(StartButton);
	DiscoverButton = new RadioButton(BUTTON_TYPE::DISCOVER, "Discover", "", Colours::orange, Colours::orange);
	addAndMakeVisible(DiscoverButton);
	// Exit app button
	exit_button = new ExitButton("Exit");
	addAndMakeVisible(exit_button);
	// Select radios buttons
	select_frame = new SelectFrame("Radios");
	addAndMakeVisible(select_frame);
	// Audio panel
	//audioPanel = new AudioPanel();
	//addAndMakeVisible(audioPanel);
}

void ControlPanel::layout_components_in_grid() {

	// Local grid as its just a bag of behaviour
	Grid grid;

	// Layout in 1 row by 4 cols so that exit stays right and other stay left
	using Track = Grid::TrackInfo;
	grid.templateColumns = { Track(80_px), Track(80_px), Track(20_px), Track(1_fr), Track(20_px), Track(80_px) };
	grid.templateRows = { Track(35_px),  Track(20_px) };
	grid.autoColumns = Track(1_fr);
	grid.autoRows = Track(1_fr);
	grid.autoFlow = Grid::AutoFlow::row;
	grid.justifyItems = Grid::JustifyItems::center;

	// Add items to the grid
	grid.items.addArray({
		GridItem(StartButton).withJustifySelf(GridItem::JustifySelf::start),
		GridItem(DiscoverButton).withJustifySelf(GridItem::JustifySelf::start),
		GridItem(exit_button).withArea(1,6),
		GridItem(select_frame).withArea(1,4),
	});

	grid.performLayout(getLocalBounds());
}