#pragma once

class GLContext
{
public:
	~GLContext(void);

	static GLContext* instance();
	QGLFunctions* getQGLFunctions(){return &glFunctions;}
private:
	GLContext(void);
	static GLContext* context;
	QGLFunctions glFunctions;
};
