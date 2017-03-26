#pragma once
class Stage
{
public:
	Stage();
	~Stage();

	virtual void Setup() {};
	virtual bool Render() { return true; };
	virtual void Draw() {};
};

