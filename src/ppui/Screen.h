/////////////////////////////////////////////////////////////////
//
//	PPScreen class
// ----------------------------------------------------------- 
// The PPScreen object acts as container for controls
// Each control is a rectangular area on screen.
// The screen is responsible for drawing the controls if 
// necessary (although the actual painting is implemented by
// the control itself)
// The screen will also send low level events from the OS 
// to the affected control(s)
/////////////////////////////////////////////////////////////////
#ifndef SCREEN__H
#define SCREEN__H

#include "Object.h"
#include "DisplayDeviceBase.h"
#include "SimpleVector.h"
#include "Control.h"

// Forwards
class PPControl;
class PPEvent;
class EventListenerInterface;
class PPContainer;
class GraphicsAbstract;

class PPScreen : public PPObject
{
protected:
	PPDisplayDeviceBase* displayDevice;

	EventListenerInterface* eventListener;

	PPControl* focusedControl;
	PPControl* beforeModalFocusedControl;

	//PPControl* contextMenuControl;
	PPSimpleVector<PPControl>* contextMenuControls;

	PPControl* modalControl;

	PPSimpleVector<PPControl> controls;
	PPSimpleVector<PPControl>* timerEventControls;
	
	bool showDragHilite;
	
	PPContainer* rootContainer;

private:
	PPPoint lastMousePoint;
	PPControl* lastMouseOverControl;
	
	void paintDragHighlite(PPGraphicsAbstract* g);

public:
	PPScreen(PPDisplayDeviceBase* displayDevice, EventListenerInterface* eventListener = NULL);

	virtual ~PPScreen();

	void attachEventListener(EventListenerInterface* eventListener) { this->eventListener = eventListener; }
	EventListenerInterface* detachEventListener() { EventListenerInterface* theListener = eventListener; eventListener = NULL; return theListener; }

	void raiseEvent(PPEvent* event);

	void clear();
	void paint(bool update = true, bool clean = false);
	void paintContextMenuControl(PPControl* control, bool update = true);
	void paintControl(PPControl* control, bool update = true); 
	void paintSplash(const pp_uint8* rawData, pp_uint32 width, pp_uint32 height, pp_uint32 pitch, pp_uint32 bpp, pp_int32 intensity = 256);

	void update();
	void updateControl(PPControl* control);
	
	void pauseUpdate(bool pause);
	void enableDisplay(bool enable);

	void setFocus(PPControl* control, bool repaint = true);

	PPControl* getFocusedControl();

	bool hasFocus(PPControl* control);

	void addControl(PPControl* control);
	bool removeControl(PPControl* control);

	void addTimerEventControl(PPControl* control);
	bool removeTimerEventControl(PPControl* control);

	PPControl* getControlByID(pp_int32 id);

	PPControl* getModalControl() { return modalControl; }

	void releaseCaughtControls();
	void setModalControl(PPControl* control, bool repaint = true);
	
	void setContextMenuControl(PPControl* control, bool repaint = true);
	void addContextMenuControl(PPControl* control, bool repaint = true);
	bool removeContextMenuControl(PPControl* control, bool repaint = true);
	bool removeLastContextMenuControl(bool repaint = true);
	
	bool hasContextMenu(PPControl* control);
	
	PPControl* getContextMenuControl(pp_int32 index = 0) 
	{ 
		return index < contextMenuControls->size() ? contextMenuControls->get(index) : NULL; 
	}

	PPControl* getLastContextMenuControl() 
	{ 
		return contextMenuControls->size() ? contextMenuControls->get(contextMenuControls->size()-1) : NULL; 
	}

	void setShowDragHilite(bool b);

	// -------------- PPDisplayDevice Delegates -------------------
	pp_int32 getWidth();
	pp_int32 getHeight();

	static pp_int32 getDefaultWidth();
	static pp_int32 getDefaultHeight();
	
	void setTitle(const PPSystemString& title);
	
	void setSize(const PPSize& size);

	bool goFullScreen(bool b);
	bool isFullScreen();
	
	void signalWaitState(bool b, const PPColor& color);	
	
	void setMouseCursor(MouseCursorTypes type);
	MouseCursorTypes getCurrentActiveMouseCursor();
	
	void shutDown();
};

#endif