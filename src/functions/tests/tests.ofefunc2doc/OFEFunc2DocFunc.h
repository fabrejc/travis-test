/**
  \file PrimitivesProdFunc.h
  \brief Header of ...
*/

#ifndef __OFEFUNC2DOCFUNC_H__
#define __OFEFUNC2DOCFUNC_H__

#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS


// =====================================================================
// =====================================================================


/**

*/
class OFEFunc2DocFunction : public openfluid::base::PluggableFunction
{
  private:

    int m_RepeatMessages;

  public:
    /**
      Constructor
    */
    OFEFunc2DocFunction();

    /**
      Destructor
    */
    ~OFEFunc2DocFunction();

    bool initParams(openfluid::core::FuncParamsMap_t Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif  // __OFEFUNC2DOCFUNC_H__