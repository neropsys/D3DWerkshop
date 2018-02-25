#pragma once
class IRender {
	
public:
	virtual ~IRender() {};
	virtual void Update(float delta) const abstract;
	virtual void Draw() const abstract;
};