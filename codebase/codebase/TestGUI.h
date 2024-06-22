#ifndef __TESTGUI__
#define __TESTGUI__

class TestGUI
{
public:
	TestGUI();
	~TestGUI();

	int Init();
	int Exit();

	void Events();
	void Updates();
	void Render();

private:
	int initState = -1;
};

#endif // !__TESTGUI__
