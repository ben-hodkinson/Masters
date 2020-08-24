#ifndef ANALYSISCAM_CAMPROPERTY_H
#define ANALYSISCAM_CAMPROPERTY_H

#include <stdarg.h>
#include <functional>

class CamObject;

//static properties accepting a CamObject
typedef void (*dvoid)(CamObject&);
typedef bool (*dbool)(CamObject&);
typedef int (*dint)(CamObject&);
typedef float (*dfloat)(CamObject&);
typedef double (*ddouble)(CamObject&);
typedef unsigned int (*duint)(CamObject&);

//static properties accepting a CamObject and variable arguments
typedef void (*dvvoid)(CamObject&, va_list);
typedef bool (*dvbool)(CamObject&, va_list);
typedef int (*dvint)(CamObject&, va_list);
typedef float (*dvfloat)(CamObject&, va_list);
typedef double (*dvdouble)(CamObject&, va_list);
typedef unsigned int (*dvuint)(CamObject&, va_list);

//lambdas
typedef std::function<double(CamObject&)> dldouble;
typedef std::function<float(CamObject&)> dlfloat;
typedef std::function<int(CamObject&)> dlint;
typedef std::function<unsigned int(CamObject&)> dluint;
typedef std::function<bool(CamObject&)> dlbool;

#endif