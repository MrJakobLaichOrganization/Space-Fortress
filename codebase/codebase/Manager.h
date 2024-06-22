#ifndef MANAGER_INTERFACE_H
#define MANAGER_INTERFACE_H
class Manager
{
	int initState = -1;
public:

	virtual int Init() = 0;
	virtual int Exit() = 0;

	virtual void Event() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};
#endif