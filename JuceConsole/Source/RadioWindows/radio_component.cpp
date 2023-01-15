/*
radio_component.cpp

Content for a radio window.

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

#include "radio_component.h"
#include "../Common/gui_cache.h"
#include "../Common/extern.h"
#include "../RadioInterface/radio_interface.h"
#include "../Properties/prop_cache.h"

//==============================================================================
//==============================================================================
// The document component
RadioComponent::RadioComponent(String p_radio_id)
{
	radio_id = p_radio_id;
}

RadioComponent::~RadioComponent()
{
}

void RadioComponent::start_ui()
{
	// Create the inner container
	radio_box = new RadioBox(radio_id);
	addAndMakeVisible(radio_box);

	// Instantiate all the inner contained components
	radio_box->start_ui();

	// Restore metics
	int W = PropCache::getInstance()->get_prop_inst(radio_id)->getIntValue("Width", var(600));
	int H = PropCache::getInstance()->get_prop_inst(radio_id)->getIntValue("Height", var(450));

	// Set size
	// NOTE: This calls resized() down the hierarchy so do not call this before components are instantiated.
	setSize(W, H);
}

//==============================================================================
// Events
void RadioComponent::paint(Graphics& g)
{
	// The component is opaque, so we must completely fill the background with a solid colour
	g.fillAll(Colours::darkgrey);
}

void RadioComponent::resized()
{
	// Resize children
	// Allow a small border around inner component
	radio_box->setBounds(5, 5, getWidth() - 10, getHeight() - 10);

	// Save metrics
	if (getWidth() > 0 && getHeight() > 0) {
		PropCache::getInstance()->get_prop_inst(radio_id)->set_value("Width", var(getWidth()));
		PropCache::getInstance()->get_prop_inst(radio_id)->set_value("Height", var(getHeight()));
	}
}

//==============================================================================
//==============================================================================
// The inner box component for layout
RadioBox::RadioBox(String p_radio_id)
{
	radio_id = p_radio_id;
}

RadioBox::~RadioBox()
{
}

void RadioBox::start_ui()
{
	audio_button = new AudioButton(radio_id);
	addAndMakeVisible(audio_button);

	vfo_component = new VFOComponent(radio_id, RX);
	addAndMakeVisible(vfo_component);
	GUICache::getInstance()->setVFOInst(radio_id, vfo_component);

	mode_panel = new ModePanel(radio_id);
	addAndMakeVisible(mode_panel);

	filter_panel = new FilterPanel(radio_id);
	addAndMakeVisible(filter_panel);

	display_panel = new DisplayPanel(radio_id);
	addAndMakeVisible(display_panel);
}

//==============================================================================
// Events
void RadioBox::paint (Graphics& g)
{
    // The component is opaque, so we must completely fill the background with a solid colour
	g.fillAll(Colours::darkgrey);
	g.setColour(Colours::grey);
	// Draw horizontal lines
	g.fillRect(0, 33, getWidth(), 2);
	g.fillRect(0, 85, getWidth(), 2);
}

void RadioBox::resized()
{
    // Layout children in a grid layout
	Grid grid;
	grid.rowGap = 10_px;
	grid.columnGap = 10_px;
	
	// Set grid parameters
	using Track = Grid::TrackInfo;
	// Just need to right number of rows
	grid.templateRows = { Track(25_px), Track(50_px), Track(1_fr), Track(3_fr) };
	// Allow 16 colums so we can give each component the right amount of space
	grid.templateColumns = { 
		Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr), 
		Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr),
		Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr),
		Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr) };
	grid.autoColumns = Track(1_fr);
	grid.autoRows = Track(1_fr);
	grid.autoFlow = Grid::AutoFlow::row;
	grid.justifyContent = Grid::JustifyContent::spaceAround;
	grid.alignContent = Grid::AlignContent::center;

	// Add items to grid
	grid.items.addArray({ 
		GridItem(audio_button).withArea(1,GridItem::Span(3)),
		GridItem(vfo_component).withArea(2,GridItem::Span(8)),
		GridItem(mode_panel).withArea(3,GridItem::Span(6)),
		GridItem(filter_panel).withArea(3,GridItem::Span(5)),
		GridItem(display_panel).withArea(4,GridItem::Span(16))
	});
	// Finally apply layout
	grid.performLayout(getLocalBounds());
}

//==============================================================================
//==============================================================================
// The audio button component
AudioButton::AudioButton(String p_radio_id) {

	setButtonText("Audio>");
	if (p_radio_id == "radio-1") channel = 0;
	else if (p_radio_id == "radio-2") channel = 1;
	else channel = 2;
}

void AudioButton::clicked() {
	c_server_change_audio_outputs(channel, "BOTH");
}