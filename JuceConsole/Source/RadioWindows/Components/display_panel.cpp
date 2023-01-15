/*
display_panel.cpp

Display panel component for the Juce Console

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

#include "display_panel.h"
#include "../../Common/extern.h"
#include "../../RadioInterface/radio_interface.h"
#include "../../Common/gui_cache.h"
#include "vfo.h"

//==============================================================================
// Display Panel
//==============================================================================

//==============================================================================
// Public

//==============================================================================
// Constructor/Destructor
DisplayPanel::DisplayPanel(String p_radio_id)
{
	// Local vars
	radio_id = p_radio_id;
	// Make buffer
	buf = (float*)malloc(MAX_SIZE_OF_DISPLAY * sizeof(float));
	// Get id
	if (radio_id == "radio-1") i_radio = 0;
	else if (radio_id == "radio-2") i_radio = 1;
	else i_radio = 2;
	// Start rendering
	startTimer(200);
}

DisplayPanel::~DisplayPanel()
{
}

//==============================================================================
// GUI Events

//----------------------------------------------------------------------------
void DisplayPanel::paint(Graphics& g)
{
	// Our component is opaque, so we must completely fill the background with a solid colour
	g.fillAll(Colours::black);
	draw_all(g);
}

//----------------------------------------------------------------------------
void DisplayPanel::resized()
{
	// This is called when the display is resized.
	if (RadioInterface::getInstance()->is_radio_running()) {
		display_set = true;
		c_server_set_display(i_radio, getWidth() - L_MARGIN - R_MARGIN);
		if (image != (Image*)NULL) free(image);
		image = new Image(Image::RGB, getWidth() - L_MARGIN - R_MARGIN, WF_HEIGHT, true);
	}
}

//----------------------------------------------------------------------------
void DisplayPanel::timerCallback() {
	repaint();
}

//----------------------------------------------------------------------------
void DisplayPanel::mouseMove(const MouseEvent & event) {
	// Remember pos if within display client area
	Point<int> p = event.getPosition();
	if (p.x > L_MARGIN && 
		p.x < getWidth() - R_MARGIN &&
		p.y > T_MARGIN &&
		p.y < getHeight() - B_MARGIN) {
		X = p.x - L_MARGIN;
	}
	else {
		X = -1;
	}
	Y = p.y;
}

void DisplayPanel::mouseDown(const MouseEvent & event) {
	if (event.mouseWasClicked()) {
		clicked = true;
	}
}

//==============================================================================
// Private

void DisplayPanel::draw_all(Graphics& g) {

	// Draw grid with labels within our component bounds
	g.setColour(grid_colour);
	draw_horiz(g);
	draw_vert(g);

	// Draw dynamic parts of the display
	if (RadioInterface::getInstance()->is_radio_running()) {
		// Set display width if not set
		if (!display_set) {
			c_server_set_display(i_radio, getWidth() - L_MARGIN - R_MARGIN);
			display_set = true;
		}
		// Create a new image if first time in
		if (image == (Image*)NULL)
			image = new Image(Image::RGB, getWidth() - L_MARGIN - R_MARGIN, WF_HEIGHT, true);
		// Do drawing
		g.setColour(filter_overlay_colour);
		draw_filter(g);
		g.setColour(freq_cursor_colour);
		draw_cursor(g);

		int r = c_server_get_display_data(i_radio, (void*)buf);
		if (r == 1) {
			g.setColour(pan_colour);
			draw_pan(g);
			draw_waterfall(g);
		}
	}
}

//----------------------------------------------------------------------------
// Draw horizontal grid lines and dbM scale
void DisplayPanel::draw_horiz(Graphics& g) {
	
	// One horizontal line per 20 db
	int i, j;
	int db_divs = (abs(LOW_DB) - abs(HIGH_DB)) / 20;
	float db_pixels_per_div = (((float)getHeight() - t_margin - b_margin) / (float)db_divs);
	for (i = 0, j = HIGH_DB; i <= db_divs; i++, j -= 20) {
		g.setColour(legend_colour);
		g.drawText(String(j), 5, Y_V_LABEL_ADJ + (i * (int)db_pixels_per_div), 40, 20, Justification(Justification::left));
		g.setColour(grid_colour);
		g.drawLine(
			l_margin,
			t_margin + (i * db_pixels_per_div),
			(float)getWidth() - r_margin,
			t_margin + (i * db_pixels_per_div)
		);
	}
}

//----------------------------------------------------------------------------
// Draw vertical grid lines and frequency scale
void DisplayPanel::draw_vert(Graphics& g) {
	// DIVS number of vertical bars
	int i;
	float j;
	String sfreq;
	// Get current frequency
	int freq = RadioInterface::getInstance()->get_current_frequency(i_radio);
	float start_freq = (float)(freq - (SPAN_FREQ / 2));
	float freq_inc = (float)SPAN_FREQ / (float)(DIVS);
	float pixels_per_div = ((float)getWidth() - l_margin - r_margin) / (float)DIVS;
	for (i = 0, j = start_freq; i <= DIVS; i++, j += freq_inc) {
		// Centre line in red
		if (i == DIVS / 2) g.setColour(Colours::red); else g.setColour(Colours::green);
		sfreq = String(j / 1000000.0f , 3);
		g.setColour(legend_colour);
		g.drawText(sfreq, 10 + (i * pixels_per_div), getHeight() - B_MARGIN + X_H_LABEL_ADJ, 40, 20, Justification(Justification::left));
		g.setColour(grid_colour);
		g.drawLine(
			l_margin + (i * pixels_per_div),
			t_margin,
			l_margin + (i * pixels_per_div),
			(float)getHeight() - b_margin
		);
	}
}

//----------------------------------------------------------------------------
// Shade area for filter bandwidth
void DisplayPanel::draw_filter(Graphics& g) {
	struct filter_desc d = RadioInterface::getInstance()->get_current_rx_filter_desc(i_radio);
	int freq = RadioInterface::getInstance()->get_current_frequency(i_radio);
	int low = d.f_lower;
	int high = d.f_upper;
	int diff;
	float pix_w, pix_centre, ppf, x_left, x_right;
	// Display area width
	pix_w = (float)(getWidth() - L_MARGIN - R_MARGIN);
	// Display area centre relative to left edge
	pix_centre = pix_w / 2.0f;
	// Pixels per Hz in display area
	ppf = pix_w / (float)SPAN_FREQ;
	// Overlay a semi-transparent rectangle over the filter area
	// We have to map low and high frequency to an x position
	if (low < freq) {
		diff = freq - low;
		x_left = pix_centre - (float)(diff * ppf);
	} else {
		diff = low - freq;
		x_left = pix_centre + (float)(diff * ppf);
	}
	if (high < freq) {
		diff = freq - high;
		x_right = pix_centre - (float)(diff * ppf);
	}
	else {
		diff = high - freq;
		x_right = pix_centre + (float)(diff * ppf);
	}
	
	g.fillRect(x_left + (float)L_MARGIN, (float)T_MARGIN, x_right - x_left, (float)getHeight() - (float)T_MARGIN - (float)B_MARGIN);
}

//----------------------------------------------------------------------------
// Draw frequency at mouse pointer
void DisplayPanel::draw_cursor(Graphics& g) {
	int freq = RadioInterface::getInstance()->get_current_frequency(i_radio);
	float pix_w, pix_centre, ppf, f;
	// Display area width
	pix_w = (float)(getWidth() - L_MARGIN - R_MARGIN);
	// Hz per pixel in display area
	ppf =  (float)SPAN_FREQ / pix_w;
	// Current frequency at mouse position
	String sf;
	// Get frequency at mouse X location
	if (X != -1) {
		f = (((float)X * ppf) + (float)(freq - (SPAN_FREQ/2)));
		sf = String(f / 1000000.0f, 3);
	}
	// Track frequency at mouse pointer
	g.drawText(sf, X, Y, 50, 20, Justification::centredLeft);

	// See if we need to change frequency
	if (clicked) {
		clicked = false;
		if (radio_id == "radio-1")
			RadioInterface::getInstance()->ri_server_cc_out_set_rx_1_freq((int)f);
		else if (radio_id == "radio-2")
			RadioInterface::getInstance()->ri_server_cc_out_set_rx_2_freq((int)f);
		else
			RadioInterface::getInstance()->ri_server_cc_out_set_rx_3_freq((int)f);
		VFOComponent* vfo = (VFOComponent*)(GUICache::getInstance()->getVFOInst(radio_id));
		vfo->set_freq_from_hz((int)f);
	}
}

//----------------------------------------------------------------------------
// Draw panadapter
void DisplayPanel::draw_pan(Graphics& g) {
	int x;
	float y;
	
	// Data is a float for each pixel in the display width
	// We need to convert the float value to a Y coordinate
	// Note that the data is reversed as in data[0] is last x position.
	path.clear();
	// We start the path at the bottom right origin of the grid as we need
	// a closed area in order to fill.
	path.startNewSubPath(l_margin, (float)getHeight() - b_margin);
	for (x = 0; x < getWidth() - L_MARGIN - R_MARGIN; x++) {
		y = val_to_coord(buf[getWidth() - L_MARGIN - R_MARGIN - x]);
		path.lineTo((float)(x + L_MARGIN), y);
	}
	// We end up at the left hand grid boundary
	// We need to move to the bottom and then to the  bottom right 
	// origin to complete the closed area to fill
	path.lineTo((float)(x + L_MARGIN), (float)(getHeight() - B_MARGIN));
	path.closeSubPath();
	g.fillPath(path);
}

//----------------------------------------------------------------------------
// Draw waterfall

void DisplayPanel::draw_waterfall(Graphics& g) {
	int x, disp_width;

	// Data is a float for each pixel in the display width
	// We need to convert the float value to a colour and set the 
	// corresponding pixel in the image (X coordinate) then scroll
	// the image and render underneath the panadapter.
	// Note that the data is reversed as in data[0] is last x position.
	disp_width = getWidth() - L_MARGIN - R_MARGIN;
	for (x = 0; x < disp_width; x++) {
		Colour c = db_to_colour(buf[getWidth() - L_MARGIN - R_MARGIN - x]);
		image->setPixelAt(x, WF_HEIGHT - 1, c);
	}
	image->moveImageSection(0, 0, 0, 1, disp_width, WF_HEIGHT - 1);
	g.drawImageAt(*image, L_MARGIN, getHeight() - WF_HEIGHT);
}

//----------------------------------------------------------------------------
// Convert a dBM value to a Y coordinate
float DisplayPanel::val_to_coord(float val) {
	// y-coord = disp-height - ((abs(low-dBm) - abs(dBm)) * (disp-height/span_db))
	float disp_height = (float)(getHeight() - T_MARGIN - B_MARGIN);
	float y = (disp_height - (((float)(abs(LOW_DB) - abs((int)val))) * (disp_height / (float)(abs(LOW_DB) - abs(HIGH_DB)))));
	return y;
}

//----------------------------------------------------------------------------
// Convert a dBM value to a colour
Colour DisplayPanel::db_to_colour(int dbM) {
	if (dbM >= -160 && dbM < -135) return colour_1;
	if (dbM >= -135 && dbM < -130) return colour_2;
	if (dbM >= -130 && dbM < -125) return colour_3;
	if (dbM >= -125 && dbM < -120) return colour_4;
	if (dbM >= -120 && dbM < -115) return colour_5;
	if (dbM >= -115 && dbM < -110) return colour_6;
	if (dbM >= -110 && dbM < -100) return colour_7;
	if (dbM >= -100 && dbM < 0) return colour_8;
	return colour_1;
}