/*
audio_out_content.h

Header for audio out content component for the Juce Console

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
#include "Components/audio_output_panel.h"

/*
	A radio panel component
*/
class AudioOutPanel : public Component
{
public:
	//==============================================================================
	AudioOutPanel();
	~AudioOutPanel();

	void resized() override;
	void paint(Graphics& g) override;

	//==============================================================================

private:
	//==============================================================================
	// State variables
	// Output audio panel
	AudioPanel *audioPanel;

	//==============================================================================
	// Method prototypes
	void create_components();
	void layout_components_in_grid();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioOutPanel)
};
