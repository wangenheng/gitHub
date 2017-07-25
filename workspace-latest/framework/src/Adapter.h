/*
	��MFCʵ��C++�İ�����̬����������.txt�Ը���û�кû����ֹۺͱ���
	��������綼�й��ף�ǰ�߷����˷ɻ������߷����˽���ɡ��������CObject
	�Ļ���BaseClass��������CRuntimeClass��RuntimeClass����غ�Ķ���ͷ
	�ļ�����:
*/
#ifndef __BASECLASS_H__
#define __BASECLASS_H__

#include <iostream>

class Adapter;

struct _declspec(dllexport) RuntimeClass
{
	char *classname;
	Adapter* (*pCreateFn)();
	
	void* CreateObject();
	static RuntimeClass* LoadByName(std::string classname);
		
	static RuntimeClass *pFirstClass;
	RuntimeClass *pBaseClass;
	RuntimeClass *pNextClass;
};

struct _declspec(dllexport) Class_List
{
	Class_List(RuntimeClass* pNewClass);
};

#define Runtime_Class(classname) \
	(&classname::class##classname)
		
#define Declare_RTTI(classname) \
	public:	\
		static  RuntimeClass  class##classname; \
		virtual RuntimeClass* GetRuntimeClass() const; \
		
#define Implement_RTTI(classname, basename) \
	static char lpsz##classname[] = #classname; \
	RuntimeClass classname::class##classname = \
	{ \
		lpsz##classname, NULL, Runtime_Class(basename), NULL \
	}; \
	static Class_List _init_##classname(&classname::class##classname); \
	RuntimeClass* classname::GetRuntimeClass() const \
	{ \
		return &classname::class##classname; \
	} \
		
#define Declare_DynCreate(classname) \
	Declare_RTTI(classname) \
	static Adapter* CreateObject(); \
	
#define Implement_DynCreate(classname, basename) \
	Adapter* classname::CreateObject() \
	{ \
		return new classname; \
	} \
	static char lpsz##classname[] = #classname; \
	RuntimeClass classname::class##classname = \
	{ lpsz##classname, classname::CreateObject, Runtime_Class(basename), NULL }; \
	static Class_List _init_##classname(&classname::class##classname); \
	RuntimeClass* classname::GetRuntimeClass() const \
	{ \
		return &classname::class##classname; \
	} \
		
class _declspec(dllexport) Adapter
{
public:
	virtual RuntimeClass* GetRuntimeClass() const
	{
		//return &Adapter::classBaseClass;
		return &Adapter::classAdapter;
	}
	virtual void printClassName()
	{
		std::cout << "Adapter!\n";
	}
public:
	static RuntimeClass classAdapter;
};

#endif // end __BASECLASS_H__
