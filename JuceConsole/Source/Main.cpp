/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "Common/extern.h"
#include "Common/gui_cache.h"
#include "Properties/properties.h"
#include "Properties/prop_cache.h"
#include "RadioInterface/radio_interface.h"
#include "RadioWindows/radio_component.h"
#include "RadioWindows/radio_window.h"
#include "RadioWindows/radio_win_cache.h"

//==============================================================================
class JuceConsoleApplication  : public JUCEApplication
{
public:
    //==============================================================================
    JuceConsoleApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return false; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // Application initialisation
		std::cout << std::endl << "Start initialisation" << std::endl;

		// Restore all properties
		PropCache::getInstance()->restore_all();

		// Create main window
        mainWindow.reset (new MainWindow (getApplicationName()));
	
		// Make wisdom file if not already made
		c_server_make_wisdom("E:/Projects/SDRLibEConsole/trunk/src/cpp/wisdom");
		
		// Initialise server
		if (!c_server_init()) {
			printf("Failed to initialise server!");
			return;
		}
		c_server_set_num_rx(1);
		if (!RadioInterface::getInstance()->ri_set_default_audio()) {
			// Something is kinda wrong here, can't continue
			std::cout << std::endl << "Failed to configure audio, unable to continue!" << std::endl;
			quit();
		}
		if (!RadioInterface::getInstance()->ri_radio_discover()) {
			// Radio hardware not running or comms problem
			std::cout << "Failed to discover radio, please check hardware." << std::endl;
			// Start the main UI only
			MainComponent* c = mainWindow->get_component();
			mainWindow->get_component()->start_ui();
		} else {
			if (!RadioInterface::getInstance()->ri_server_start()) {
				// Something also kinda wrong here, cant continue
				std::cout << std::endl << "Failed to start server processes, unable to continue!" << std::endl;
				quit();
			} else {
				// Start the main UI
				MainComponent* c = mainWindow->get_component();
				mainWindow->get_component()->start_ui();

				// Start radio 1 instance
				RadioWindow * w1 = RadioWinCache::getInstance()->create_window("radio-1");
				w1->get_component()->start_ui();
			}
		}
		std::cout << std::endl << "End initialisation" << std::endl;
    }

    void shutdown() override
    {
        // Shutdown
        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    //Desktop::getInstance().getDefaultLookAndFeel()
                                                    //                      .findColour (ResizableWindow::backgroundColourId),
													Colours::darkgrey,
                                                    DocumentWindow::allButtons)
        {
			setUsingNativeTitleBar (true);
			c = new MainComponent();
            setContentOwned (c, true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            //centreWithSize (getWidth(), getHeight());
           #endif

			// Restore position
			int X = PropCache::getInstance()->get_prop_inst("main")->getIntValue("X", var(100));
			int Y = PropCache::getInstance()->get_prop_inst("main")->getIntValue("Y", var(100));
			setTopLeftPosition(X, Y);

            setVisible (true);

        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */
		void moved() {
			DocumentWindow::moved();
			if (getX() > 0 && getY() > 0) {
				// Set new position
				PropCache::getInstance()->get_prop_inst("main")->set_value("X", var(getX()));
				PropCache::getInstance()->get_prop_inst("main")->set_value("Y", var(getY()));
			}
		}

		MainComponent *get_component() {
			return c;
		}

    private:
		MainComponent *c;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (JuceConsoleApplication)
