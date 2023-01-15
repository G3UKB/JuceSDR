/*
MainComponent.cpp

Main Component for the Juce Console

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

#include "MainComponent.h"
#include "Common/gui_cache.h"
#include "Properties/prop_cache.h"

//==============================================================================
MainComponent::MainComponent() {
}

MainComponent::~MainComponent() {
}

void MainComponent::start_ui()
{
	tabPanel = new TabPanel();
	addAndMakeVisible(tabPanel);

	// Restore metics
	int W = PropCache::getInstance()->get_prop_inst("main")->getIntValue("Width", var(300));
	int H = PropCache::getInstance()->get_prop_inst("main")->getIntValue("Height", var(200));
	setSize(W, H);
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // The component is opaque, so we must completely fill the background with a solid colour
	g.fillAll(Colours::darkgrey);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // Resize children
	tabPanel->setBounds(10, 10, getWidth() - 20, getHeight() - 20);

	// Save metrics
	if (getWidth() > 0 && getHeight() > 0) {
		PropCache::getInstance()->get_prop_inst("main")->set_value("Width", var(getWidth()));
		PropCache::getInstance()->get_prop_inst("main")->set_value("Height", var(getHeight()));
	}
}
