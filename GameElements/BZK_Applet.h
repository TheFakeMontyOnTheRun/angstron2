///@brief interface for application model supporting a limited yet functional life cycle. Inherits from the application architecture.
//=============================================================================================
class BZK_Applet : public BZK_App {
public:
///@note everything here is pure virtual for obvious reasons. All names, except when noted are pretty much obvious
//--------------------------------------------------------------------------------------------
	virtual void Start() = 0;

//--------------------------------------------------------------------------------------------
	virtual void Refresh() = 0;

//--------------------------------------------------------------------------------------------
	virtual void FilterEvents(BZK_SystemEvent *Events) = 0;

//--------------------------------------------------------------------------------------------
///@brief "should we heartbeat again?"
	virtual bool WaitForNextCycle() = 0;
//--------------------------------------------------------------------------------------------
};
