/*
filter_panel.h

Header for filter buttons component for the Juce Console

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

/*
	A mode button component
*/
class FilterButton : public TextButton
{
public:
	//==============================================================================
	FilterButton(int p_i_radio_id, String p_s_radio_id, String label);
	~FilterButton();

	//==============================================================================
	void clicked();

private:
	//==============================================================================
	// State variables
	int i_radio_id;
	String s_radio_id;

	//==============================================================================
	// Method prototypes

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterButton)
};

/*
	A mode panel component
*/
class FilterPanel : public Component
{
public:
	//==============================================================================
	FilterPanel(String p_radio_id);
	~FilterPanel();

	void resized() override;

	//==============================================================================

private:
	//==============================================================================
	// State variables
	// Which radio
	String radio_id;
	int i_radio;

	// Button vars
	FilterButton *Filt6K0Button;
	FilterButton *Filt4K0Button;
	FilterButton *Filt2K7Button;
	FilterButton *Filt2K4Button;
	FilterButton *Filt2K1Button;
	FilterButton *Filt1K0Button;
	FilterButton *Filt500Button;
	FilterButton *Filt250Button;
	FilterButton *Filt100Button;

	//==============================================================================
	// Method prototypes
	void create_buttons();
	void layout_buttons_in_grid();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterPanel)
};