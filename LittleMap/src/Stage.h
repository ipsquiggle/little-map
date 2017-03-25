#pragma once
class Stage
{
public:
	Stage();
	~Stage();

	virtual void Setup() {};
	virtual void Render() {};
	virtual void Draw() {};
};

