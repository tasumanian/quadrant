#pragma once

class Component;

class ComponentRegistry
{
public:

static void Register();

static Component*
Create();

};