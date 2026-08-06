#pragma once

#include "EditorWindow.h"

class HierarchyWindow :
    public EditorWindow
{
public:

    void Draw(Scene& scene) override;

	void SetSelection(GameObject* obj)
	{
		m_selection = obj;
	}

private:

    GameObject* m_selection = nullptr;
};