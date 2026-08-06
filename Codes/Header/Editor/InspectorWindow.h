#pragma once
#include "EditorWindow.h"

class InspectorWindow :
    public EditorWindow
{
private:

    GameObject* m_selection;

public:

    void Draw(Scene& scene) override;

    void DrawComponent(Component* component);

    void DrawProperty(PropertyBase* property);

	void SetSelection(GameObject* obj)
	{
		m_selection = obj;
	}
};