/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#include "SimStatus.h"
#include <iostream>

namespace mhydasdk { namespace base {


SimulationInfo::SimulationInfo(wxDateTime StartTime,
                               wxDateTime EndTime,
                               int TimeStep)

{
  wxTimeSpan DeltaTime;
  
  m_StartTime = StartTime;
  m_EndTime = EndTime;
  

  m_TimeStep = TimeStep;

  DeltaTime = EndTime-StartTime;
  
  m_StepsCount = ((DeltaTime.GetSeconds().ToLong()) / TimeStep) + 1;  
  
}

// =====================================================================
// =====================================================================

SimulationInfo::~SimulationInfo()
{

}



// =====================================================================
// =====================================================================



SimulationStatus::SimulationStatus(wxDateTime StartTime,
                                   wxDateTime EndTime,
                                   int TimeStep)
                : SimulationInfo(StartTime,EndTime,TimeStep)                   

{

  m_CurrentStep = 0;
  m_CurrentTime = m_StartTime;
  
  m_IsFirstStep = true;
    
  m_IsLastStep = false;    
  
  
}

// =====================================================================
// =====================================================================

SimulationStatus::~SimulationStatus()
{

}

// =====================================================================
// =====================================================================



bool SimulationStatus::switchToNextStep()
{
  wxDateTime NextTime(m_CurrentTime + wxTimeSpan(0,0,m_TimeStep,0));

//  std::cerr << NextTime.Format(wxT("%Y-%m-%d %H:%M:%S")).mb_str(wxConvUTF8) << std::endl;
  
  if (NextTime <=  m_EndTime)
  {
    m_CurrentStep++;

    m_CurrentTime = wxDateTime(m_CurrentTime + wxTimeSpan(0,0,m_TimeStep,0));
   
    m_IsFirstStep =  (m_CurrentStep == 0);
    m_IsLastStep = (NextTime ==  m_EndTime);    
    
    return true;
  }
  else return false;

}


} } // namespace mhydasdk::core
