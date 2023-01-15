/*
display_panel.h

Header for display panel component for the Juce Console

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
// Panadapter and waterfall definitions

// ----------------------------------
// Divisions and margins
// Panadapter grid divs
#define	DIVS 6
// Left, right, top, bottom margins(limits of grid and display)
#define	L_MARGIN 35
#define	R_MARGIN 15
#define	T_MARGIN 10
#define	B_MARGIN 100
// Adjustments for label data to correctly align
#define	X_H_LABEL_ADJ + 5
#define	Y_H_LABEL_ADJ - 10
#define	Y_V_LABEL_ADJ - 3

// Fixed height for waterfall display
#define	WF_HEIGHT 75

// ----------------------------------
// Panadapter db scale to scale the Y axis display
#define	LOW_DB - 140
#define	HIGH_DB - 20

// ----------------------------------
// Frequency
// Panadapter span i.e sample rate
#define	SPAN_FREQ 48000
#define MAX_SIZE_OF_DISPLAY 1920

// ===========================================================
/*
	Multi-instance display component
*/
class DisplayPanel : public Component, public Timer
{
public:
	//==============================================================================
	DisplayPanel(String p_radio_id);
	~DisplayPanel();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void mouseMove(const MouseEvent & event) override;
	void mouseDown(const MouseEvent & event) override;
	void timerCallback();

private:
	//==============================================================================
	// State variables
	String radio_id;
	int i_radio;
	float *buf;
	Path path;
	Image *image = (Image*)NULL;
	bool display_set = false;
	int X;
	int Y;
	bool clicked = false;
	// For calc using floats
	float l_margin = (float)L_MARGIN;
	float r_margin = (float)R_MARGIN;
	float t_margin = (float)T_MARGIN;
	float b_margin = (float)B_MARGIN;

	// Colours for the various elements of the panadapter
	Colour grid_colour = Colour((uint8)0, (uint8)255, (uint8)0, (uint8)80);
	Colour legend_colour = Colour((uint8)255, (uint8)255, (uint8)0, (uint8)125);
	Colour filter_overlay_colour = Colour((uint8)0, (uint8)255, (uint8)0, (uint8)70);
	Colour freq_cursor_colour = Colour((uint8)255, (uint8)255, (uint8)0, (uint8)125);
	Colour pan_colour = Colour((uint8)0, (uint8)255, (uint8)0, (uint8)125);

	// Colours gradients for the waterfall display
	Colour colour_1 = Colour((uint8)0, (uint8)0, (uint8)51, (uint8)255);
	Colour colour_2 = Colour((uint8)25, (uint8)0, (uint8)76, (uint8)255);
	Colour colour_3 = Colour((uint8)51, (uint8)0, (uint8)102, (uint8)255);
	Colour colour_4 = Colour((uint8)76, (uint8)0, (uint8)127, (uint8)255);
	Colour colour_5 = Colour((uint8)127, (uint8)0, (uint8)102, (uint8)255);
	Colour colour_6 = Colour((uint8)178, (uint8)0, (uint8)127, (uint8)255);
	Colour colour_7 = Colour((uint8)220, (uint8)0, (uint8)25, (uint8)255);
	Colour colour_8 = Colour((uint8)255, (uint8)0, (uint8)0, (uint8)255);

	//==============================================================================
	// Method prototypes
	// Drawing
	void draw_all(Graphics& g);
	void draw_horiz(Graphics& g);
	void draw_vert(Graphics& g);
	void draw_filter(Graphics& g);
	void draw_cursor(Graphics& g);
	void draw_pan(Graphics& g);
	void draw_waterfall(Graphics& g);

	// Helpers
	float val_to_coord(float val);
	Colour db_to_colour(int dbM);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DisplayPanel)
};



