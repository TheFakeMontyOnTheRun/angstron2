///@brief Abstract class for input.
///@note In the end, its like killing an ant with a cannonball. But it wasnt all that badly designed. I suspect it will become more useful as BZK get more
///@note diferent uses...
class BZK_DeviceInput {
public:
//==============================================================================================
//----------------------------------------------------------------------------------------------
	virtual char *DeviceName() { return "Abstract Input Device"; }

//----------------------------------------------------------------------------------------------
	virtual bool SignalReady() { return false; }

//----------------------------------------------------------------------------------------------
	virtual bool FilterEvents(BZK_SystemEvent *aEvent) { return true; }//I dont like relying that much on SDL...
//----------------------------------------------------------------------------------------------
};
