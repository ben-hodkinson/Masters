#include "AnalysisCamEvent/CamProperty.h"
#include "AnalysisCamEvent/CamObject.h"

//we no longer disable the prefix. The prefix is what it is - up to the user
//to ensure their functions can handle cases with and without prefix
/*bool CamProperty<dbool>::get(CamObject& c) const { bool temp=c.getUsePrefix();c.setUsePrefix(false);bool out=m_val(c);c.setUsePrefix(temp);return out; }
unsigned int CamProperty<duint>::get(CamObject& c) const { bool temp=c.getUsePrefix();c.setUsePrefix(false);uint out=m_val(c);c.setUsePrefix(temp);return out; }
int CamProperty<dint>::get(CamObject& c) const  { bool temp=c.getUsePrefix();c.setUsePrefix(false);int out=m_val(c);c.setUsePrefix(temp);return out; }
float CamProperty<dfloat>::get(CamObject& c) const { bool temp=c.getUsePrefix();c.setUsePrefix(false);float out=m_val(c);c.setUsePrefix(temp);return out;}
double CamProperty<ddouble>::get(CamObject& c) const {bool temp=c.getUsePrefix();c.setUsePrefix(false);double out=m_val(c);c.setUsePrefix(temp);return out; }
void CamProperty<dvoid>::get(CamObject& c) const {bool temp=c.getUsePrefix();c.setUsePrefix(false);  m_val(c); c.setUsePrefix(temp); }*/
/*
bool CamProperty<dbool>::get(CamObject& c)  { m_hasBeenCalled=true;return m_val(c); }
unsigned int CamProperty<duint>::get(CamObject& c)  { m_hasBeenCalled=true;return m_val(c); }
int CamProperty<dint>::get(CamObject& c)   { m_hasBeenCalled=true;return m_val(c); }
float CamProperty<dfloat>::get(CamObject& c)  { m_hasBeenCalled=true;return m_val(c); }
double CamProperty<ddouble>::get(CamObject& c)  { m_hasBeenCalled=true;return m_val(c); }
void CamProperty<dvoid>::get(CamObject& c)  { m_hasBeenCalled=true;return m_val(c); }*/