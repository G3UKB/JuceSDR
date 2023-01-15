/*
common_content.h

Header for common_content component for the Juce Console

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

#include "../../JuceLibraryCode/JuceHeader.h"
//#include "Components/audio_output_panel.h"
#include "Components/radio_button.h"
#include "Components/exit_button.h"
#include "Components/radio_select_frame.h"

/*
	A radio panel component
*/
class ControlPanel : public Component
{
public:
	//==============================================================================
	ControlPanel();
	~ControlPanel();

	void resized() override;
	void paint(Graphics& g) override;

	RadioButton *get_start_button();

	//==============================================================================

private:
	//==============================================================================
	// State variables

	// Button vars
	RadioButton *StartButton;
	RadioButton *DiscoverButton;
	ExitButton *exit_button;
	GroupComponent *select_frame;

	//==============================================================================
	// Method prototypes
	void create_components();
	void layout_components_in_grid();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlPanel)
};
