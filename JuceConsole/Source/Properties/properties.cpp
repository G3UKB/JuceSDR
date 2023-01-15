/*
properties.cpp

Properties save/restore for the Juce Console

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
#include "properties.h"


//==============================================================================
// Display Panel
//==============================================================================

//==============================================================================
// Public

//==============================================================================
// Constructor/Destructor
Properties::Properties(String p_common_name, String p_filename)
{
	// Local vars
	common_name = p_common_name;
	fprop = new File(p_filename);
	PropertiesFile::Options options = PropertiesFile::Options::Options();
	options.applicationName = "JuceConsole";
	//options.filenameSuffix = "cfg";
	//options.folderName = ".";
	
	properties_file = new PropertiesFile(*fprop, options);
}

Properties::~Properties()
{
}

//==============================================================================
// Get/Set
void Properties::set_value(String key, var value) {
	properties_file->setValue(key, value);
}

void Properties::save_if_needed() {
	properties_file->saveIfNeeded();
}

void Properties::save() {
	properties_file->save();
}

void Properties::load() {
	properties_file->reload();
}

String Properties::getValue(StringRef keyName, const String &defaultReturnValue) {
	return properties_file->getValue(keyName, defaultReturnValue);
}

int Properties::getIntValue(StringRef keyName, int defaultReturnValue) {
	return properties_file->getIntValue(keyName, defaultReturnValue);
}

double Properties::getDoubleValue(StringRef keyName, double defaultReturnValue) {
	return properties_file->getDoubleValue(keyName, defaultReturnValue);
}

bool Properties::getBoolValue(StringRef keyName, bool defaultReturnValue) {
	return properties_file->getBoolValue(keyName, defaultReturnValue);
}
