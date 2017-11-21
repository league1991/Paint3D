#include "StdAfx.h"
#include "GLContext.h"

GLContext* GLContext::context = NULL;

GLContext::GLContext(void)
{
}

GLContext::~GLContext(void)
{
}

GLContext* GLContext::instance()
{
	if (context == NULL)
	{
		context = new GLContext;
	}
	return context;
}
