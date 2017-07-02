#pragma once
#include <iostream>

class ICar
{
public:
	ICar() {}
	virtual	~ICar() {}

	virtual void testV() const = 0;
};

class CBus : public ICar
{
public:
	CBus() {}
	virtual ~CBus() {}

	virtual void testV() const override { std::cout << "Bus::test" << std::endl; }
};

class CCar : public ICar
{
public:
	CCar() {}
	virtual ~CCar() {}

	virtual void testV() const override { std::cout << "Car::test" << std::endl; }
};

class CBase
{
public:
	virtual	~CBase() {}

	virtual void funcV() {}
};

class CDerivedA : public CBase
{
public:
	CDerivedA(int vA, double vB) : m_A(vA), m_B(vB){}
	virtual ~CDerivedA() {}

	virtual void funcV() override { std::cout << "CDerivedA::funcV is " << m_A + m_B << std::endl; }

private:
	int m_A;
	double m_B;
};

class CDerivedB : public CBase
{
public:
	virtual ~CDerivedB() {}

	virtual void funcV() override { std::cout << "CDerivedB::funcV" << std::endl; }
};

class CObject
{
public:
	CObject(CBase* vBase) : m_BasePtr(vBase) {}
	~CObject()
	{
		if (m_BasePtr != nullptr)
		{
			delete m_BasePtr;
			m_BasePtr = nullptr;
		}
	}

	void func()
	{
		m_BasePtr->funcV();
	}

private:
	CBase* m_BasePtr;
};