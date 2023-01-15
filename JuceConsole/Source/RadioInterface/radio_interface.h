/*
radio_interface.h

Header for interface to the co-located server for the Juce Console

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

#define RX 0
#define TX 1

enum class MODES {
	CH_LSB,
	CH_USB,
	CH_DSB,
	CH_CWL,
	CH_CWU,
	CH_FM,
	CH_AM,
	CH_DIGU,
	CH_SPEC,
	CH_DIGL,
	CH_SAM,
	CH_DRM
};

// Descriptor of filter width and position
struct filter_desc {
	int f_lower;
	int f_upper;
};

//==============================================================================
// Singleton instance for access methods to the co-located 'C' server. Some of these
// interact e.g. a change of mode may need to change the filter bounds and visa-versa.
// Also we don't want to be updating radio parameters if its not running etc.
class RadioInterface
{
public:
	// JUCEy Singleton
	//==============================================================================
	RadioInterface();
	~RadioInterface()
	{
		clearSingletonInstance();
	}

	// Macro to create boilerplate methods for singleton
	juce_DeclareSingleton(RadioInterface, true);

	//==============================================================================
	// Method prototypes
	// Call down to server
	bool ri_set_default_audio();
	bool ri_server_start();
	bool ri_server_terminate();
	bool ri_radio_discover();
	bool ri_radio_start(int wbs);
	bool ri_radio_stop();
	void ri_server_set_rx_mode(int channel, int mode);
	void ri_server_set_rx_filter_freq(int channel, int filter);
	void ri_server_cc_out_set_rx_1_freq(unsigned int freq_in_hz);
	void ri_server_cc_out_set_rx_2_freq(unsigned int freq_in_hz);
	void ri_server_cc_out_set_rx_3_freq(unsigned int freq_in_hz);

	// Get methods
	bool is_server_running();
	bool is_radio_running();
	bool is_radio_discovered();
	int get_current_frequency(int channel);
	int get_current_rx_mode(int channel);
	struct filter_desc get_current_rx_filter_desc(int channel);

	// Reset interface
	void reset();
	
private:
	//==============================================================================
	// State variables
	bool audio_set = false;
	bool server_running = false;
	bool radio_discovered = false;
	bool radio_running = false;
	
	typedef struct RxState {
		int freq = 7100000;
		int mode = (int)MODES::CH_LSB;
		int filt_lower = 300;
		int filt_upper = 2400;
		int filt_freq_lower = 7100000;
		int filt_freq_upper = 7100000;
	}RxState;
	
	typedef struct AllState {
		RxState rx_1;
		RxState rx_2;
		RxState rx_3;
	}AllState;
	AllState *all_state;

	//==============================================================================
	// Method prototypes
	void set_mode_filter(int channel, int mode, int filter_low, int filter_high, bool set_radio = true);

	void set_current_freq(int channel, int freq);
	void set_current_mode(int channel, int mode);
	void set_current_filt_low (int channel, int filt_low);
	void set_current_filt_high(int channel, int filt_high);
	void set_current_filt_freq_low(int channel, int filt_freq_low);
	void set_current_filt_freq_high(int channel, int filt_freq_high);
	int get_current_freq(int channel);
	int get_current_mode(int channel);
	int get_current_filt_low(int channel);
	int get_current_filt_high(int channel);
	int get_current_filt_freq_low(int channel);
	int get_current_filt_freq_high(int channel);

};