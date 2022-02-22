// Server.cxx
//
/////////////////////////////////////////////////////////////////////////////

#include "common.hxx"
#include "Server.hxx"

#ifndef NDEBUG
#  include "Server.inl"
#endif

#include "basic.hxx"

/////////////////////////////////////////////////////////////////////////////
// Server

Server Server::theServer;

Server::~Server()
{
}

Server::Server()
{
#	define P(type_, name_, value_) m_##name_ = value_;
#	define Q(type_, name_, value_) m_##name_##0 = value_;
#	include "parameters.hxx"
#	undef P
#	undef Q
	normalize();
	m_sayBits = -1;
}

void Server::normalize()
{
	m_simulator_step = (int)((double)(m_simulator_step0 + m_TIMEDELTA - 1) / m_TIMEDELTA) * m_TIMEDELTA;
	m_send_step = (int)((double)(m_send_step0/4 + m_TIMEDELTA - 1) / m_TIMEDELTA) * m_TIMEDELTA * 4;
	m_recv_step = (int)((double)(m_recv_step0 + m_TIMEDELTA - 1) / m_TIMEDELTA) * m_TIMEDELTA;
	m_sense_body_step = (int)((double)(m_sense_body_step0 + m_TIMEDELTA - 1) / m_TIMEDELTA) * m_TIMEDELTA;
	m_send_vi_step = (int)((double)(m_send_vi_step0 + m_TIMEDELTA - 1) / m_TIMEDELTA) * m_TIMEDELTA;
}

bool Server::readConfigFile(const char* filename)
{
	ASSERT(filename);
#if OS_TYPE == WINDOWS
	FILE* fp = fopen(filename, "rt");
#else
	FILE* fp = fopen(filename, "r");
#endif
	if(!fp) {
		const int size = 1024;
		char buffer[size] = "error: can't open file '";
		strncat(buffer, filename, size - strlen(buffer));
		buffer[size-2] = '\0';
		strcat(buffer, "'");
		perror(buffer);
		return false;
	}
	
	const int size = 1024;
	char buffer[size];
	char name[size];
	char value[size];
	int line = 0;
	while(fgets(buffer, size, fp) != NULL) {
		line++;
		if (buffer[0] == '#' || buffer[0] == '\n')
			continue;
		
		char *p = buffer;
		while(*p != '\0') {
			if(*p == ':')
				*p = ' ';
			p++;
		}
		int n = sscanf(buffer, "%s %s", name, value);
		bool valid = false;
		if(n == 2) {
			if(setValue(name, value))
				valid = true;
		}
		if(!valid)
			report("warning: %s (%d) : %s\n", filename, line, buffer);
	}
	
	fclose(fp);
	
	return true;
}

enum
{
	v_chars,
		v_int,
		v_double,
		v_bool,
};

bool Server::setValue(const char* name, const char* value)
{
	ASSERT(name && value);
	
	int type;
	void* variable;
#	define P(type_, name_, value_) if(strcmp(name, #name_) == 0) { type = v_##type_; variable = &m_##name_; } else
#	define Q(type_, name_, value_) if(strcmp(name, #name_) == 0) { type = v_##type_; variable = &m_##name_##0; } else
#	include "parameters.hxx"
	{ return false; }
#	undef P
#	undef Q
	
	int scanned;
	switch(type) {
	case v_chars:
		return false;
		break;
	case v_int:
		scanned = sscanf(value, "%d", variable);
		if(scanned < 1 || scanned == EOF)
			return false;
		break;
	case v_double:
		scanned = sscanf(value, "%lf", variable);
		if(scanned < 1 || scanned == EOF)
			return false;
		break;
	case v_bool:
		*(bool*)variable = (!value || strcmp(value, "on") == 0);
		break;
	default:
		ASSERT(false);
		return false;
	}

	if(variable == &m_SAY_MESSAGE_LETTERS) {
		m_sayBits = -1;
	}

	normalize();
	
	return true;
}

