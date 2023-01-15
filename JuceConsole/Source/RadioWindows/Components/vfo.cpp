/*
vfo.cpp

VFO component for the Juce Console

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
#include "../../RadioInterface/radio_interface.h"
#include "../../Properties/prop_cache.h"
#include "vfo.h"

//==============================================================================
// Main VFO Component Panel
//==============================================================================

//==============================================================================
// Constructor/Destructor
VFOComponent::VFOComponent(String p_radio_id, int p_vfo_type)
{
	// Local vars
	vfo_type = p_vfo_type;
	radio_id = p_radio_id;

	// WE make content here but arrange it in resized
	create_digits();

	// Create freq_inc_map
	freq_inc_map.insert(std::pair<String, int> ("100MHz", 100000000));
	freq_inc_map.insert(std::pair<String, int>("10MHz", 10000000));
	freq_inc_map.insert(std::pair<String, int>("1MHz", 1000000));
	freq_inc_map.insert(std::pair<String, int>("100KHz", 100000));
	freq_inc_map.insert(std::pair<String, int>("10KHz", 10000));
	freq_inc_map.insert(std::pair<String, int>("1KHz", 1000));
	freq_inc_map.insert(std::pair<String, int>("100Hz", 100));
	freq_inc_map.insert(std::pair<String, int>("10Hz", 10));
	freq_inc_map.insert(std::pair<String, int>("1Hz", 1));

	// Update frequency
	int freq = PropCache::getInstance()->get_prop_inst(radio_id)->getIntValue("FREQ", 7100000);
	set_freq(convertFreq(freq));
	current_freq = freq;
	set_radio_freq();
}

VFOComponent::~VFOComponent()
{
}

void VFOComponent::set_freq_inc(String id) {
	freq_inc = freq_inc_map.at(id);
}

void VFOComponent::reset_freq_inc() {
	freq_inc = -1;
}

void VFOComponent::freq_plus() {
	if (freq_inc > 0) {
		int ifreq = current_freq + freq_inc;
		if (ifreq <= MAX_FREQ) {
			current_freq = ifreq;
			set_freq(convertFreq(current_freq));
			set_radio_freq();
			PropCache::getInstance()->get_prop_inst(radio_id)->set_value("FREQ", var(current_freq));
		}
	}
}

void VFOComponent::freq_minus() {
	if (freq_inc > 0) {
		int ifreq = current_freq - freq_inc;
		if (ifreq >= MIN_FREQ) {
			current_freq = ifreq;
			set_freq(convertFreq(current_freq));
			set_radio_freq();
			PropCache::getInstance()->get_prop_inst(radio_id)->set_value("FREQ", var(current_freq));
		}
	}
}

String VFOComponent::convertFreq(int freq) {
	// Convert to a string representation of the frequency in Hz
	String sfreq = String(freq);
	// Add leading zeros to make it a 9 digit string
	// Number of leading zeros to add
	int l = 9 - sfreq.length();
	String leading_zeros;
	for ( int i = 0; i < l; i++) {
		leading_zeros += "0";
	}
	return (leading_zeros += sfreq);
}

void VFOComponent::set_freq(String freq) {
	const wchar_t * wcstr = freq.toWideCharPointer();
	d_100MHz->setText(String::charToString(wcstr[0]), dontSendNotification);
	d_10MHz->setText(String::charToString(wcstr[1]), dontSendNotification);
	d_1MHz->setText(String::charToString(wcstr[2]), dontSendNotification);
	d_100KHz->setText(String::charToString(wcstr[3]), dontSendNotification);
	d_10KHz->setText(String::charToString(wcstr[4]), dontSendNotification);
	d_1KHz->setText(String::charToString(wcstr[5]), dontSendNotification);
	d_100Hz->setText(String::charToString(wcstr[6]), dontSendNotification);
	d_10Hz->setText(String::charToString(wcstr[7]), dontSendNotification);
	d_1Hz->setText(String::charToString(wcstr[8]), dontSendNotification);
}

void VFOComponent::set_freq_from_hz(int freq) {
	set_freq(convertFreq(freq));
}

//==============================================================================
// GUI Events
void VFOComponent::paint(Graphics& g)
{
	// Our component is opaque, so we must completely fill the background with a solid colour
	g.fillAll(Colours::darkgrey);
}

void VFOComponent::resized()
{
	// This is called when the VFOComponent is resized.
	layout_digits_in_grid();
}

//==============================================================================
// Private
void VFOComponent::create_digits() {

	// Create digits
	d_100MHz = new VFODigit(this, String("0"), MHZ_COLOR, MHZ_FONT);
	d_100MHz->setComponentID("100MHz");
	addAndMakeVisible(d_100MHz);
	d_10MHz = new VFODigit(this, String("0"), MHZ_COLOR, MHZ_FONT);
	d_10MHz->setComponentID("10MHz");
	addAndMakeVisible(d_10MHz);
	d_1MHz = new VFODigit(this, String("0"), MHZ_COLOR, MHZ_FONT);
	d_1MHz->setComponentID("1MHz");
	addAndMakeVisible(d_1MHz);
	d_100KHz = new VFODigit(this, String("0"), KHZ_COLOR, KHZ_FONT);
	d_100KHz->setComponentID("100KHz");
	addAndMakeVisible(d_100KHz);
	d_10KHz = new VFODigit(this, String("0"), KHZ_COLOR, KHZ_FONT);
	d_10KHz->setComponentID("10KHz");
	addAndMakeVisible(d_10KHz);
	d_1KHz = new VFODigit(this, String("0"), KHZ_COLOR, KHZ_FONT);
	d_1KHz->setComponentID("1KHz");
	addAndMakeVisible(d_1KHz);
	d_100Hz = new VFODigit(this, String("0"), HZ_COLOR, HZ_FONT);
	d_100Hz->setComponentID("100Hz");
	addAndMakeVisible(d_100Hz);
	d_10Hz = new VFODigit(this, String("0"), HZ_COLOR, HZ_FONT);
	d_10Hz->setComponentID("10Hz");
	addAndMakeVisible(d_10Hz);
	d_1Hz = new VFODigit(this, String("0"), HZ_COLOR, HZ_FONT);
	d_1Hz->setComponentID("1Hz");
	addAndMakeVisible(d_1Hz);
	
	// Create two spacers
	sep_1 = new Label("spacer", ".");
	sep_1->setText(".", NotificationType::dontSendNotification);
	sep_1->setColour(Label::textColourId, Colours::antiquewhite);
	addAndMakeVisible(sep_1);
	sep_2 = new Label("spacer", ".");
	sep_2->setText(".", NotificationType::dontSendNotification);
	sep_2->setColour(Label::textColourId, Colours::antiquewhite);
	addAndMakeVisible(sep_2);

	set_freq(convertFreq(current_freq));
}

void VFOComponent::layout_digits_in_grid() {

	Grid grid;

	// Add digits in one horizontal row
	using Track = Grid::TrackInfo;
	
	grid.templateRows = { Track(1_fr) };
	grid.templateColumns = { 
		Track(1_fr), Track(1_fr), Track(1_fr),
		Track(15_px),
		Track(1_fr), Track(1_fr), Track(1_fr),
		Track(15_px),
		Track(1_fr), Track(1_fr), Track(1_fr)
	};
	
	grid.items = {
		GridItem(d_100MHz),
		GridItem(d_10MHz), 
		GridItem(d_1MHz),
		GridItem(sep_1),
		GridItem(d_100KHz), 
		GridItem(d_10KHz), 
		GridItem(d_1KHz),
		GridItem(sep_2),
		GridItem(d_100Hz), 
		GridItem(d_10Hz), 
		GridItem(d_1Hz) 
	};

	grid.justifyItems = Grid::JustifyItems::stretch;
	// Add gaps if required
	//grid.rowGap = Grid::Px::Px(10.0f);
	//grid.columnGap = Grid::Px::Px(10.0f);
	grid.performLayout(getLocalBounds());
}

void VFOComponent::set_radio_freq() {
	if (radio_id == "radio-1")
		RadioInterface::getInstance()->ri_server_cc_out_set_rx_1_freq(current_freq);
	else if (radio_id == "radio-2")
		RadioInterface::getInstance()->ri_server_cc_out_set_rx_2_freq(current_freq);
	else if (radio_id == "radio-3")
		RadioInterface::getInstance()->ri_server_cc_out_set_rx_3_freq(current_freq);
}

//==============================================================================
// A VFO Digit
//==============================================================================

VFODigit::VFODigit(VFOComponent *parent, String text, Colour colour, float size) {
	
	Label::Label();
	my_parent = parent;
	// Create a digit ready to add to the grid
	setText(text, dontSendNotification);
	setJustificationType(Justification::centred);
	setColour(Label::textColourId, colour);
	setFont(Font(size, Font::plain));
};

VFODigit::~VFODigit() {

}

void VFODigit::paint(Graphics& g) {
	// Add any additional painting here
	Label::paint(g);
};

void VFODigit::resized() {
	
};

void VFODigit::mouseEnter(const MouseEvent& event) {
	String id = event.eventComponent->getComponentID();
	my_parent->set_freq_inc(id);
	
	// Grow font by 5 points
	Font f = getFont();
	int h = f.getHeight();
	setFont(Font(h + 5, Font::plain));
}

void VFODigit::mouseExit(const MouseEvent& event) {
	my_parent->reset_freq_inc();

	// Shrink font by 5 points
	Font f = getFont();
	int h = f.getHeight();
	setFont(Font(h - 5, Font::plain));
}

void VFODigit::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) {
	float dir = wheel.deltaY;
	if (dir < 0) {
		// Freq going down
		my_parent->freq_minus();
	}
	else {
		// Freq going up
		my_parent->freq_plus();
	}
}