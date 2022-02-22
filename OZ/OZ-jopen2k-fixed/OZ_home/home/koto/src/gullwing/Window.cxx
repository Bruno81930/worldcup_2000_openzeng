// Window.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "common.hxx"
#include "Window.hxx"

#ifndef NDEBUG
#  include "Window.inl"
#endif

/////////////////////////////////////////////////////////////////////////////
// Window

Window::~Window()
{
	m_destructing = true;
	set(-1);
}

void Window::set(int index)
{
	ASSERT(-1 <= index && index < MAX_PLAYER*2);
	m_mutex.lock();
	if(m_observedIndex != index) {
		if(m_observedIndex != -1) {
			m_observedClient = 0;
			m_observedIndex = -1;
		}
		if(index != -1) {
			ASSERT(0 <= index && index < COUNT_OF(m_clients));
			ASSERT(m_clients[index] != 0);
			m_observedClient = m_clients[index];
			m_observedIndex = index;
		}
	}
	if(!m_destructing)
		updateScreen();
	m_mutex.unlock();
}
