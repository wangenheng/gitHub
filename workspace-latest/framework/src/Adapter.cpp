#include "Adapter.h"
#include <string.h>

RuntimeClass* RuntimeClass::pFirstClass = NULL;
	
static char lpszAdapter[] = "Adapter";

RuntimeClass Adapter::classAdapter = { lpszAdapter, NULL, NULL, NULL };
	
static Class_List _init_Adapter(&Adapter::classAdapter);
	
Class_List::Class_List(RuntimeClass* pNewClass)
{
	pNewClass->pNextClass = RuntimeClass::pFirstClass;
	RuntimeClass::pFirstClass = pNewClass;
}

void* RuntimeClass::CreateObject()
{
	if (pCreateFn == NULL)
	{
		return NULL;
	}
	
	void* pObject = NULL;
	pObject = (*pCreateFn)();
	
	return pObject;
}

RuntimeClass* RuntimeClass::LoadByName(std::string classname)
{
	RuntimeClass* pClass;
	
	for (pClass = pFirstClass; pClass != NULL; pClass = pClass->pNextClass)
	{
		if (strcmp(classname.c_str(), pClass->classname) == 0)
			return pClass;
	}
	
	return NULL;
}

