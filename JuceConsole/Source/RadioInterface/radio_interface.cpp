/*
radio_interface.cpp

Radio interface for the Juce Console

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

#include "radio_interface.h"
#include "../Common/extern.h"

//==============================================================================
// Radio Interface Singleton class
//==============================================================================

//==============================================================================
// Public

//Macro for Singleton
juce_ImplementSingleton(RadioInterface)

RadioInterface::RadioInterface() {
	all_state = new AllState;
}

void RadioInterface::reset() {
	audio_set = false;
	server_running = false;
	radio_discovered = false;
	radio_running = false;
}
//==============================================================================
// Call down methods

bool RadioInterface::ri_set_default_audio() {
	int direction;
	char* host_api;
	char* dev;

	if (!audio_set) {
		audio_set = true;
		// Set up a default route for RX1 to Speaker output
		DeviceEnumList* l = c_server_enum_audio_outputs();
		for (int i = 0; i < l->entries; i++) {
			printf("%s", l->devices[i].name);
			if (String(l->devices[i].name).startsWith("Speakers")) {
				if (String(l->devices[i].host_api) == "MME") {
					direction = l->devices[i].direction;
					host_api = l->devices[i].host_api;
					dev = l->devices[i].name;
					break;
				}
			}
		}
		c_server_set_audio_route(direction, LOCAL, 1, host_api, dev, BOTH);
	}
	return true;
}

bool RadioInterface::ri_server_start() {

	if (!server_running) {
		// Start server
		if (c_server_start()) {
			server_running = true;
		} else {
			std::cout << "Interface: Failed to start server!" << std::endl;
			return false;
		}
	}
	return true;
}

bool RadioInterface::ri_server_terminate() {

}

bool RadioInterface::ri_radio_discover() {
	if (!radio_discovered) {
		// Discover radio
		if (c_radio_discover()) {
			radio_discovered = true;
		} else {
			std::cout << "Interface: Radio hardware not found!" << std::endl;
			return false;
		}
	}
	return true;
}

bool RadioInterface::ri_radio_start(int wbs) {
	if (!radio_running) {
		// Start radio
		if (c_radio_start(wbs)) {
			radio_running = true;
		} else {
			std::cout << "Interface: Failed to start radio!" << std::endl;
			return false;
		}
	}
	return true;
}

bool RadioInterface::ri_radio_stop() {
	if (radio_running) {
		radio_running = false;
		// Stop radio
		if (!c_radio_stop()) {
			printf("Interface: Failed to stop radio!");
			return false;
		}
	}
	return true;
}

void RadioInterface::ri_server_set_rx_mode(int channel, int mode) {
	if (server_running) {
		set_current_mode(channel, mode);
		set_mode_filter(channel, mode, get_current_filt_low(channel), get_current_filt_high(channel));
	}
}

void RadioInterface::ri_server_set_rx_filter_freq(int channel, int filter) {

	int low;
	int high;

	if (server_running) {
		switch (filter) {
		case 0: low = 100; high = 6100; break;
		case 1: low = 100; high = 4100; break;
		case 2: low = 300; high = 3000; break;
		case 3: low = 300; high = 2700; break;
		case 4: low = 300; high = 2400; break;
		case 5: low = 300; high = 1300; break;
		case 6: low = 500; high = 1000; break;
		case 7: low = 600; high = 850; break;
		case 8: low = 700; high = 800; break;
		default: low = 300; high = 2400;
		}
		set_current_filt_low(channel, low);
		set_current_filt_high(channel, high);
		set_mode_filter(channel, get_current_mode(channel), low, high);
	}
}

void RadioInterface::ri_server_cc_out_set_rx_1_freq(unsigned int freq_in_hz) {
	c_server_cc_out_set_rx_1_freq(freq_in_hz);
	set_current_freq(0, freq_in_hz);
}

void RadioInterface::ri_server_cc_out_set_rx_2_freq(unsigned int freq_in_hz) {
	c_server_cc_out_set_rx_2_freq(freq_in_hz);
	set_current_freq(1, freq_in_hz);
}

void RadioInterface::ri_server_cc_out_set_rx_3_freq(unsigned int freq_in_hz) {
	c_server_cc_out_set_rx_3_freq(freq_in_hz);
	set_current_freq(2, freq_in_hz);
}

//==============================================================================
// Get methods
bool RadioInterface::is_radio_discovered() {
	return radio_discovered;
}

bool RadioInterface::is_server_running() {
	return server_running;

}

bool RadioInterface::is_radio_running() {
	return radio_running;
}

int RadioInterface::get_current_frequency(int channel) {
	return get_current_freq(channel);
}

int RadioInterface::get_current_rx_mode(int channel) {
	return get_current_mode(channel);
}

filter_desc RadioInterface::get_current_rx_filter_desc(int channel) {
	filter_desc filter_desc;
	if (radio_running) {
		set_mode_filter(channel , get_current_mode(channel), get_current_filt_low(channel), get_current_filt_high(channel), false);
	}
	filter_desc.f_lower = get_current_filt_freq_low(channel);
	filter_desc.f_upper = get_current_filt_freq_high(channel);
	return filter_desc;
}

//==============================================================================
// Private
void RadioInterface::set_mode_filter(int channel, int mode, int filter_low, int filter_high, bool set_radio) {
	int low;
	int high;
	if ((MODES)mode == MODES::CH_LSB || (MODES)mode == MODES::CH_CWL || (MODES)mode == MODES::CH_DIGL) {
		low = -filter_high;
		high = -filter_low;
		set_current_filt_freq_low (channel, get_current_freq(channel) - filter_high);
		set_current_filt_freq_high (channel, get_current_freq(channel) - filter_low);
	}
	else if ((MODES)mode == MODES::CH_USB || (MODES)mode == MODES::CH_CWU || (MODES)mode == MODES::CH_DIGU) {
		low = filter_low;
		high = filter_high;
		set_current_filt_freq_low (channel, get_current_freq(channel) + filter_low);
		set_current_filt_freq_high (channel, get_current_freq(channel) + filter_high);
	}
	else {
		low = -filter_high;
		high = filter_high;
		set_current_filt_freq_low (channel, get_current_freq(channel) - filter_high);
		set_current_filt_freq_high (channel, get_current_freq(channel) + filter_high);
	}

	// Set new filter and/or mode
	if (set_radio) {
		c_server_set_rx_mode(channel, mode);
		c_server_set_rx_filter_freq(channel, low, high);
	}
}

// Get/Set methods
void RadioInterface::set_current_freq(int channel, int freq) {
	switch (channel) {
	case 0: all_state->rx_1.freq = freq; break;
	case 1: all_state->rx_2.freq = freq; break;
	case 2: all_state->rx_3.freq = freq; break;
	}
}
void RadioInterface::set_current_mode(int channel, int mode) {
	switch (channel) {
	case 0: all_state->rx_1.mode = mode; break;
	case 1: all_state->rx_2.mode = mode; break;
	case 2: all_state->rx_3.mode = mode; break;
	}
}
void RadioInterface::set_current_filt_low(int channel, int filt_low) {
	switch (channel) {
	case 0: all_state->rx_1.filt_lower = filt_low; break;
	case 1: all_state->rx_2.filt_lower = filt_low; break;
	case 2: all_state->rx_3.filt_lower = filt_low; break;
	}
}
void RadioInterface::set_current_filt_high(int channel, int filt_high) {
	switch (channel) {
	case 0: all_state->rx_1.filt_upper = filt_high; break;
	case 1: all_state->rx_2.filt_upper = filt_high; break;
	case 2: all_state->rx_3.filt_upper = filt_high; break;
	}
}
void RadioInterface::set_current_filt_freq_low(int channel, int filt_freq_low) {
	switch (channel) {
	case 0: all_state->rx_1.filt_freq_lower = filt_freq_low; break;
	case 1: all_state->rx_2.filt_freq_lower = filt_freq_low; break;
	case 2: all_state->rx_3.filt_freq_lower = filt_freq_low; break;
	}
}
void RadioInterface::set_current_filt_freq_high(int channel, int filt_freq_high) {
	switch (channel) {
	case 0: all_state->rx_1.filt_freq_upper = filt_freq_high; break;
	case 1: all_state->rx_2.filt_freq_upper = filt_freq_high; break;
	case 2: all_state->rx_3.filt_freq_upper = filt_freq_high; break;
	}
}
int RadioInterface::get_current_freq(int channel) {
	switch (channel) {
	case 0: return all_state->rx_1.freq;
	case 1: return all_state->rx_2.freq;
	case 2: return all_state->rx_3.freq;
	}
}
int RadioInterface::get_current_mode(int channel) {
	switch (channel) {
	case 0: return all_state->rx_1.mode;
	case 1: return all_state->rx_2.mode;
	case 2: return all_state->rx_3.mode;
	}
}
int RadioInterface::get_current_filt_low(int channel) {
	switch (channel) {
	case 0: return all_state->rx_1.filt_lower;
	case 1: return all_state->rx_2.filt_lower;
	case 2: return all_state->rx_3.filt_lower;
	}
}
int RadioInterface::get_current_filt_high(int channel) {
	switch (channel) {
	case 0: return all_state->rx_1.filt_upper;
	case 1: return all_state->rx_2.filt_upper;
	case 2: return all_state->rx_3.filt_upper;
	}
}
int RadioInterface::get_current_filt_freq_low(int channel) {
	switch (channel) {
	case 0: return all_state->rx_1.filt_freq_lower;
	case 1: return all_state->rx_2.filt_freq_lower;
	case 2: return all_state->rx_3.filt_freq_lower;
	}
}
int RadioInterface::get_current_filt_freq_high(int channel) {
	switch (channel) {
	case 0: return all_state->rx_1.filt_freq_upper;
	case 1: return all_state->rx_2.filt_freq_upper;
	case 2: return all_state->rx_3.filt_freq_upper;
	}
}