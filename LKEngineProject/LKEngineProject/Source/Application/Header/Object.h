#pragma once

#include <memory>

#include "../../Utility/Header/Macro.h"

LK_SPACE_BEGIN

class Object
{
private :
	std::string name;

public:
	Object() { }
	virtual ~Object() { }

	void SetName(const std::string& newName) { name = newName; }

	std::string GetName() const { return name; }

	virtual std::string ToString() { return "Object " + name; }
};

LK_SPACE_END