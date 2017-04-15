#pragma once
class Stage
{
public:
	Stage();
	~Stage();

	virtual void Setup() {};
	virtual bool Render() { return true; };
	virtual void Draw() {};
	virtual void Reset() {};
	virtual void DebugNum(int key) {};
	virtual void DebugClick(int x, int y) {};
	virtual char* GetMessage() { return nullptr; };
};

