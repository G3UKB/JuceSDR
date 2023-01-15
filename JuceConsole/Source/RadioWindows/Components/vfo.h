/*
vfo.h

Header for VFO component for the Juce Console

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

//==============================================================================
// Defines
#define MHZ_FONT 35.0f
#define KHZ_FONT 35.0f
#define HZ_FONT 25.0f
#define MHZ_FONT_OVER 40.0f
#define KHZ_FONT_OVER 40.0f
#define HZ_FONT_OVER 30.0f
#define MHZ_COLOR Colours::white
#define KHZ_COLOR Colours::white
#define HZ_COLOR Colours::orange
#define RX 0
#define TX 1
#define MIN_FREQ 1000
#define MAX_FREQ 500000000

class VFOComponent;

/*
	A VFO Digit component
*/
class VFODigit : public Label
{
public:
	//==============================================================================
	VFODigit(VFOComponent *parent, String text, Colour colour, float size);
	~VFODigit();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void mouseEnter(const MouseEvent& event) override;
	void mouseExit(const MouseEvent& event) override;
	void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override;

private:
	//==============================================================================
	// State variables
	VFOComponent *my_parent;

	//==============================================================================
	// Method prototypes

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VFODigit)
};

//==============================================================================
/*
	Multi-instance VFO component
*/
class VFOComponent : public Component
{
public:
	//==============================================================================
	VFOComponent(String p_vfo_id, int p_vfo_type);
	~VFOComponent();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;

	void set_freq_inc(String);
	void reset_freq_inc();
	void freq_plus();
	void freq_minus();
	String convertFreq(int freq);
	void set_freq(String freq);
	void set_freq_from_hz(int freq);

private:
	//==============================================================================
	// State variables
	int vfo_type = RX;			// RX|TX
	String radio_id = "";		// Radio id of VFO instance
	int freq_inc = -1;			// Last frequence increment in MHz
	int current_freq = 7100000;	// Current frequency in MHz

	// Map for lookup of increments
	std::map<String, int> freq_inc_map;

	// Digits
	VFODigit *d_100MHz;
	VFODigit *d_10MHz;
	VFODigit *d_1MHz;
	VFODigit *d_100KHz;
	VFODigit *d_10KHz;
	VFODigit *d_1KHz;
	VFODigit *d_100Hz;
	VFODigit *d_10Hz;
	VFODigit *d_1Hz;
	Label *sep_1;
	Label *sep_2;

	//==============================================================================
	// Method prototypes
	void create_digits();
	void layout_digits_in_grid();
	void set_radio_freq();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VFOComponent)
};

