///@brief abstract pointing device. As we deal with touchscreen too, we only handle one button, to enforce the designer to
///take touchscreen into consideration

class BZK_DevMouse : public BZK_DeviceInput {
public:
	///@brief position of the last tap or click.
	BZK_Vec2f iLastClick;

//----------------------------------------------------------------------------------------------
	BZK_DevMouse() {
		iLastClick.SetX(0);
		iLastClick.SetY(0);
	}

//----------------------------------------------------------------------------------------------
	virtual bool FilterEvents(SDL_MouseButtonEvent &aEvent) {
		int x, y;
		x = aEvent.x;
		y = aEvent.y;
		iLastClick.SetX(BZK_FastMath::Int32toFix32(x));
		iLastClick.SetY(BZK_FastMath::Int32toFix32(y));

	}
	//----------------------------------------------------------------------------------------------
};
