/*
properties.h

Header for properties save/restore

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

//==============================================================================

// ===========================================================
/*
	Multi-instance display component
*/
class Properties
{
public:
	//==============================================================================
	Properties(String p_common_name, String p_filename);
	~Properties();

	//==============================================================================
	// Method prototypes
	void set_value(String key, var value);
	void save();
	void save_if_needed();
	void load();
	String getValue(StringRef keyName, const String &defaultReturnValue = String());
	int getIntValue(StringRef keyName, int defaultReturnValue = 0);
	double getDoubleValue(StringRef keyName, double defaultReturnValue = 0.0);
	bool getBoolValue(StringRef keyName, bool defaultReturnValue = false);

private:
	//==============================================================================
	// State variables
	PropertiesFile *properties_file;
	File *fprop;
	String common_name;
	
	//==============================================================================
	// Method prototypes

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Properties)
};