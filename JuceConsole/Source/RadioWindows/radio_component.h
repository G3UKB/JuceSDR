/*
radio_component.h

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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../ControlWindow/common_content.h"
#include "Components/vfo.h"
#include "Components/mode_panel.h"
#include "Components/filter_panel.h"
#include "Components/display_panel.h"

//==============================================================================
/*
    Radio component which contains all content for one radio instance
*/
class AudioButton : public TextButton {

public:
	AudioButton::AudioButton(String p_channel);
	AudioButton::~AudioButton() {}

	// Prototypes
	void clicked();

	// State
	int channel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioButton)
};

class RadioBox : public Component
{
public:
	//==============================================================================
	RadioBox(String radio_id);
	~RadioBox();

	void start_ui();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;

private:
	//==============================================================================
	// ID for this radio
	String radio_id;

	// Children
	AudioButton *audio_button;
	VFOComponent *vfo_component;
	ModePanel *mode_panel;
	FilterPanel *filter_panel;
	DisplayPanel *display_panel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RadioBox)
};

class RadioComponent : public Component
{
public:
	//==============================================================================
	RadioComponent(String radio_id);
	~RadioComponent();

	void start_ui();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;

private:
	//==============================================================================
	// ID for this radio
	String radio_id;

	// Children
	RadioBox *radio_box;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RadioComponent)
};



